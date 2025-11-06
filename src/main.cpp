#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include "esp_task_wdt.h"

//structure for stationlist
typedef struct {
  char url[100];  //stream url
  char name[100]; //stations name
} Station;

#define STATIONS 20 //number of available stations

//station list (stations can now be modified by webinterface)
Station stationlist[STATIONS];

//instances of prefernces
Preferences pref;
Preferences station;

//global variables
uint8_t curStation = 0;   //index for current selected station in stationlist
uint8_t actStation = 0;   //index for current station in station list used for streaming 
uint32_t lastchange = 0;  //time of last selection change

// Forward declarations
void setup_stationList();
void setup_audio();
void setup_rotary();
void setup_lcdisplay();
bool makeWLAN();
void setup_stationConfig();
void showStation();
void startUrl();
bool loop_audio();
void rotary_loop();

//setup
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== ESP32 Web Radio Starting ===");
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
  
  // Disable watchdog temporarily during setup
  esp_task_wdt_init(30, true); // 30 second timeout
  esp_task_wdt_add(NULL);
  
  // -------------------
  Serial.println("Setting up station list...");
  setup_stationList();
  
  Serial.println("Setting up audio...");
  setup_audio();
  
  Serial.println("Setting up rotary encoder...");
  setup_rotary();
  
  Serial.println("Setting up LCD display...");
  setup_lcdisplay();
  
  //init WiFi
  Serial.println("Connecting to WiFi");
  while (!makeWLAN()) {
    Serial.println("Cannot connect :(");
    esp_task_wdt_reset(); // Reset watchdog during WiFi connection attempts
    delay(1000);
  }
  Serial.println("WiFi Connected successfully");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Configure WiFi for better stability
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  
  Serial.println("Setting up web server...");
  setup_stationConfig();
  // -------------------

  //set current station to 0
  curStation = 0;
  //start preferences instance
  pref.begin("radio", false);
  //Force station 0 for testing - don't load saved station for now
  Serial.println("Using station 0 (Radio Swiss Pop) for testing");
  //set active station to current station 
  //show on display and start streaming
  actStation = curStation;
  Serial.println("Showing station on display...");
  showStation();
  
  Serial.println("Starting audio stream...");
  startUrl();
  
  Serial.println("=== Setup Complete ===");
  Serial.print("Total free heap: ");
  Serial.println(ESP.getFreeHeap());
}

// main loop
void loop() {
  // Feed the watchdog to prevent resets
  esp_task_wdt_reset();
  
  // Static variables to track status
  static int audio_failures = 0;
  static unsigned long last_success = millis();
  static unsigned long last_wifi_check = 0;
  
  // Check WiFi connection every 30 seconds
  if (millis() - last_wifi_check > 30000) {
    last_wifi_check = millis();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected! Attempting reconnect...");
      WiFi.reconnect();
      delay(5000);
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi reconnect failed, continuing...");
      } else {
        Serial.print("WiFi reconnected, IP: ");
        Serial.println(WiFi.localIP());
      }
    }
  }
  
  //check if stream has ended normally not on ICY streams
  if (!loop_audio()) {
    audio_failures++;
    Serial.print("MP3 error occurred, failure count: ");
    Serial.println(audio_failures);
    
    // Check if WiFi is still connected
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi connection lost during audio failure");
      delay(10000);
      return; // Skip audio restart if no WiFi
    }
    
    // If too many failures in short time, don't restart immediately  
    if (audio_failures > 3) {
      Serial.println("Too many audio failures, longer wait...");
      delay(30000); // Wait 30 seconds
      audio_failures = 0; // Reset counter
    } else if (audio_failures == 1) {
      delay(2000); // Short delay on first failure
    } else {
      delay(10000); // Wait 10 seconds for subsequent failures
    }
    
    // Check memory before restarting audio
    Serial.print("Free heap before restart: ");
    Serial.println(ESP.getFreeHeap());
    
    if (ESP.getFreeHeap() < 50000) {
      Serial.println("LOW MEMORY! Performing full restart...");
      delay(2000);
      ESP.restart();
    }
    
    // If too many failures, try a full system restart instead
    if (audio_failures >= 3) {
      Serial.println("Multiple audio failures - performing system restart...");
      delay(3000);
      ESP.restart();
    }
    
    // Try to restart audio instead of full ESP restart
    Serial.println("Attempting to restart audio stream...");
    startUrl();
  } else {
    // Audio is working fine
    if (millis() - last_success > 60000) { // Reset failure counter every minute
      audio_failures = 0;
      last_success = millis();
    }
  }
  
  //read events from rotary encoder
  rotary_loop();
  
  // Small delay to prevent watchdog issues
  delay(10);
}