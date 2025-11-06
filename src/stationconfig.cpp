#include <Arduino.h>
// must use asynchronous webserver, otherwise playing will be interrupted :)
#include "ESPAsyncWebServer.h"
#include <Preferences.h>

#include "stationconf.h"  //HTML code template

//structure for stationlist
typedef struct {
  char url[100];  //stream url
  char name[100]; //stations name
} Station;

#define STATIONS 20 //number of available stations

// External declarations from main.cpp
extern Station stationlist[];
extern uint8_t curStation;
extern uint8_t actStation;
extern Preferences pref;

// Webserver on port 80
AsyncWebServer serverC(80);

// Webserver-config - called once in 'Setup()' 
void setup_stationConfig() {

  // eventhandling when loading site: "http://<radio-IP>/"
  serverC.on("/", HTTP_ANY, [](AsyncWebServerRequest *request){

    Preferences station;                 // preferences-instance for stationlist
    station.begin("stationlist", false);

    int paramsNr = request->params();   // if submit, here we'll get 40 POST-parameters
    Serial.println(paramsNr);
    int paramCountOk = 0;
    int gotAnswer = false;
    int j;
    String para = "";
    int needReboot = false;

    // if user has chosen to play a station on webinterface
    if (paramsNr == 1) {
      AsyncWebParameter* p2 = request->getParam(0);
      if (p2->name() == "play") {
        curStation = atoi((p2->value()).c_str());
        actStation = curStation;
        pref.putUShort("station",curStation);
        
        // ### not nice, but working :(
        // The call of 'startUrl()' and 'showStation()' throws an exception ?!?!?
        // That's why a reboot is prepared here (reboots after HTML-code is delivered)
        //startUrl();
        //showStation();

        pref.end();
        paramsNr = 0;       // don't get in next for-loop
        needReboot = true;
      }
    }

    
    // ### Parameter handling
    // is only called, when list-form is "submitted". In this case 
    // it loops STATIONS*2 times
    for(int i=0;i<paramsNr;i++){ 
  
      AsyncWebParameter* p = request->getParam(i);    // read parameter
      Serial.print("Param name: ");
      Serial.println(p->name());
      Serial.print("Param value: ");
      Serial.println(p->value());
      Serial.println("------");

      // ### search parameter in 'stationlist' and actualize
      for (j=0; j<STATIONS; j++) {
        para = "name";
        para.concat(j+1);
        if (p->name() == para  ) { strcpy(stationlist[j].name, (p->value()).c_str()); paramCountOk++; }
      }
      for (j=0; j<STATIONS; j++) {
        para = "url";
        para.concat(j+1);
        if (p->name() == para ) { strcpy(stationlist[j].url, (p->value()).c_str()); paramCountOk++; }
      }

      // ### search parameter in preferences object 'station' and update
      if (paramCountOk >= paramsNr) {
        for(j=0; j<STATIONS; j++) {
          para = "n";
          para.concat(j+1);
          station.putString(para.c_str(), stationlist[j].name);
        }
        for(j=0; j<STATIONS; j++) {
          para = "u";
          para.concat(j+1);
          station.putString(para.c_str(), stationlist[j].url);
        }

        station.end();
        gotAnswer = true;
      }
    }

    // ### Fill HTML template
    String s = STATION_page;         // read HTML template
    String fields = "";             // workmemory for field-HTML-code


    for (j=0; j<STATIONS; j++){
      fields.concat("<p>\n<h4>Station ");
      fields.concat(j+1);
      fields.concat(":</h4>\n");
      if (j == curStation) {
        fields.concat("<span class='playing'>&#9836;</span>");
      } else {
        fields.concat("<a title='Play this station' href='?play=");
        fields.concat(j);
        fields.concat("'>&#9654;</a>");
      }
      fields.concat("&nbsp;<input type='text' name='name");
      fields.concat(j+1);
      fields.concat("' value='");
      fields.concat(stationlist[j].name);
      fields.concat("' size='20'>\n<input type='text' name='url");
      fields.concat(j+1);
      fields.concat("' value='");
      fields.concat(stationlist[j].url);
      fields.concat("' size='100'>\n</p>\n");
    }
    s.replace("*fields*", fields);  // put fieldcode into template

    // if something is saved, feedback should be given
    if (gotAnswer) {
      s.replace("*feedback1*", "<div style='color:#CC0000'><h1>Saved!</h1></div>");
      s.replace("*feedback2*", "<div style='color:#CC0000'><h1>Saved!</h1></div>");
    } else {
      s.replace("*feedback1*", "");
      s.replace("*feedback2*", "");
    }

    // ### Send HTML-code to Browser
    request->send(200, "text/html", s);

    // ### Reboot if a sender was chosen by webinterface (see above)
    if (needReboot) {
      delay(1000);
      ESP.restart();
    }  

  }); 

  // ### activate the above configured webserver
  serverC.begin();
  Serial.println("Webserver started");

}