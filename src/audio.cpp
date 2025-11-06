#include <Arduino.h>
#include <Preferences.h>
//Includes from ESP8266audio
#include "AudioFileSourceICYStream.h" //input stream
#include "AudioFileSourceBuffer.h"    //input buffer
#include "AudioGeneratorMP3.h"        //decoder
#include "AudioOutputI2S.h"           //output stream

//structure for stationlist
typedef struct {
  char url[100];  //stream url
  char name[100]; //stations name
} Station;

// External declarations from main.cpp
extern Station stationlist[];
extern uint8_t actStation;

//buffer size for stream buffering
const int preallocateBufferSize = 80*1024;
const int preallocateCodecSize = 29192;         // MP3 codec max mem needed
//pointer to preallocated memory
void *preallocateBuffer = NULL;
void *preallocateCodec = NULL;

//instances for audio components
AudioGenerator *decoder = NULL;
AudioFileSourceICYStream *file = NULL;
AudioFileSourceBuffer *buff = NULL;
AudioOutputI2S *out;

//callback function will be called if meta data were found in input stream
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void) isUnicode; // Punt this ball for now
  
  // Check for NULL pointers to prevent crashes
  if (!type || !string) {
    Serial.println("METADATA: NULL pointer received");
    return;
  }
  
  const char *ptr = reinterpret_cast<const char *>(cbData);
  if (!ptr) ptr = "NULL";
  
  // Note that the type and string may be in PROGMEM, so copy them to RAM for printf
  char s1[32], s2[64];
  strncpy_P(s1, type, sizeof(s1)-1);
  s1[sizeof(s1)-1]=0;
  strncpy_P(s2, string, sizeof(s2)-1);
  s2[sizeof(s2)-1]=0;
  
  Serial.print("METADATA(");
  Serial.print(ptr);
  Serial.print(") '");
  Serial.print(s1);
  Serial.print("' = '");
  Serial.print(s2);
  Serial.println("'");
  Serial.flush();
}

//stop playing the input stream release memory, delete instances
void stopPlaying() {
  if (decoder)  {
    decoder->stop();
    delete decoder;
    decoder = NULL;
  }
  if (buff)  {
    buff->close();
    delete buff;
    buff = NULL;
  }
  if (file)  {
    file->close();
    delete file;
    file = NULL;
  }
}

//start playing a stream from current active station
void startUrl() {
  stopPlaying();  //first close existing streams
  
  // Check if we have valid station data
  if (actStation >= 20 || strlen(stationlist[actStation].url) == 0) {
    Serial.println("ERROR: Invalid station or URL");
    return;
  }
  
  //open input file for selected url
  Serial.print("Active station: ");
  Serial.println(stationlist[actStation].url);
  
  // Check WiFi connection before starting
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("ERROR: No WiFi connection for streaming");
    return;
  }
  
  try {
    Serial.println("Creating audio file source...");
    file = new AudioFileSourceICYStream(stationlist[actStation].url);
    if (!file) {
      Serial.println("ERROR: Failed to create audio file source");
      return;
    }
    Serial.println("Audio file source created successfully");
    
    //register callback for meta data
    file->RegisterMetadataCB(MDCallback, (void*)"STREAM"); 
    
    //create a new buffer which uses the preallocated memory
    buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
    if (!buff) {
      Serial.println("ERROR: Failed to create audio buffer");
      return;
    }
    Serial.print("sourcebuffer created - Free mem=");
    Serial.println(ESP.getFreeHeap());
    
    //create and start a new decoder
    decoder = (AudioGenerator*) new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
    if (!decoder) {
      Serial.println("ERROR: Failed to create MP3 decoder");
      return;
    }
    Serial.println("created decoder");
    Serial.println("Decoder start...");
    
    // Add small delay before starting decoder
    delay(500);
    
    if (!decoder->begin(buff, out)) {
      Serial.println("ERROR: Failed to start decoder");
      Serial.println("Retrying decoder start...");
      delay(1000);
      if (!decoder->begin(buff, out)) {
        Serial.println("ERROR: Decoder start failed again");
        stopPlaying();
        return;
      }
    }
    
    Serial.println("Audio streaming started successfully");
  } catch (...) {
    Serial.println("ERROR: Exception caught in startUrl()");
    stopPlaying();
  }
}

// to be called in 'setup()'
void setup_audio()
{
  //reserve buffer für for decoder and stream
  preallocateBuffer = malloc(preallocateBufferSize);          // Stream-file-buffer
  preallocateCodec = malloc(preallocateCodecSize);            // Decoder- buffer
  if (!preallocateBuffer || !preallocateCodec)
  {
    Serial.printf_P(PSTR("FATAL ERROR:  Unable to preallocate %d bytes for app\n"), preallocateBufferSize+preallocateCodecSize);
    while(1){
      yield(); // Infinite halt
    }
  } 
  //create I2S output do use with decoder
  //the second parameter 1 means use the internal DAC
  out = new AudioOutputI2S(0,1);
  
}

// to be called in 'loop()'
bool loop_audio()
{
  if (!decoder) {
    return false;
  }
  
  if (decoder->isRunning()) {
    try {
      if (!decoder->loop()) {
        Serial.println("Decoder loop ended naturally");
        decoder->stop();
        return false;
      }
      return true;
    } catch (...) {
      Serial.println("ERROR: Exception in decoder loop");
      stopPlaying();
      return false;
    }
  } else {
    // Decoder exists but not running
    return false;
  }
}