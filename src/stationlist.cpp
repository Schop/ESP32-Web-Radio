#include <Arduino.h>
#include <Preferences.h>

//structure for stationlist
typedef struct {
  char url[100];  //stream url
  char name[100]; //stations name
} Station;

// External declarations from main.cpp
extern Station stationlist[];
extern Preferences station;

// initializing Station list by flash-values - or, if not present, by defaults
void setup_stationList()
{
  station.begin("stationlist", false);

  strcpy(stationlist[0].name, station.getString("n1", "Sky Radio NL").c_str());
  strcpy(stationlist[0].url, station.getString("u1", "http://playerservices.streamtheworld.com/api/livestream-redirect/SKYRADIO.mp3").c_str());
  strcpy(stationlist[1].name, station.getString("n2", "Radio 538").c_str());
  strcpy(stationlist[1].url, station.getString("u2", "http://playerservices.streamtheworld.com/api/livestream-redirect/RADIO538.mp3").c_str());
  strcpy(stationlist[2].name, station.getString("n3", "Q-Music NL").c_str());
  strcpy(stationlist[2].url, station.getString("u3", "http://playerservices.streamtheworld.com/api/livestream-redirect/QMUSIC.mp3").c_str());
  strcpy(stationlist[3].name, station.getString("n4", "Radio Veronica").c_str());
  strcpy(stationlist[3].url, station.getString("u4", "http://playerservices.streamtheworld.com/api/livestream-redirect/VERONICA.mp3").c_str());
  strcpy(stationlist[4].name, station.getString("n5", "NPO Radio 2").c_str());
  strcpy(stationlist[4].url, station.getString("u5", "http://icecast.omroep.nl/radio2-bb-mp3").c_str());
  strcpy(stationlist[5].name, station.getString("n6", "NPO Radio 1").c_str());
  strcpy(stationlist[5].url, station.getString("u6", "http://icecast.omroep.nl/radio1-bb-mp3").c_str());
  strcpy(stationlist[6].name, station.getString("n7", "NPO 3FM").c_str());
  strcpy(stationlist[6].url, station.getString("u7", "http://icecast.omroep.nl/3fm-bb-mp3").c_str());
  strcpy(stationlist[7].name, station.getString("n8", "Radio 10").c_str());
  strcpy(stationlist[7].url, station.getString("u8", "http://playerservices.streamtheworld.com/api/livestream-redirect/RADIO10.mp3").c_str());
  strcpy(stationlist[8].name, station.getString("n9", "100% NL").c_str());
  strcpy(stationlist[8].url, station.getString("u9", "http://playerservices.streamtheworld.com/api/livestream-redirect/HONDERDPROCENTNL.mp3").c_str());
  strcpy(stationlist[9].name, station.getString("n10", "BBC Radio 1").c_str());
  strcpy(stationlist[9].url, station.getString("u10", "http://stream.live.vc.bbcmedia.co.uk/bbc_radio_one").c_str());
  strcpy(stationlist[10].name, station.getString("n11", "Arrow Classic Rock").c_str());
  strcpy(stationlist[10].url, station.getString("u11", "http://playerservices.streamtheworld.com/api/livestream-redirect/ARROWCLASSICROCK.mp3").c_str());
  strcpy(stationlist[11].name, station.getString("n12", "Sublime FM").c_str());
  strcpy(stationlist[11].url, station.getString("u12", "http://playerservices.streamtheworld.com/api/livestream-redirect/SUBLIMEFM.mp3").c_str());
  strcpy(stationlist[12].name, station.getString("n13", "SLAM!").c_str());
  strcpy(stationlist[12].url, station.getString("u13", "http://playerservices.streamtheworld.com/api/livestream-redirect/SLAMFM.mp3").c_str());
  strcpy(stationlist[13].name, station.getString("n14", "JOE").c_str());
  strcpy(stationlist[13].url, station.getString("u14", "http://playerservices.streamtheworld.com/api/livestream-redirect/JOE.mp3").c_str());
  strcpy(stationlist[14].name, station.getString("n15", "NPO Radio 4").c_str());
  strcpy(stationlist[14].url, station.getString("u15", "http://icecast.omroep.nl/radio4-bb-mp3").c_str());
  strcpy(stationlist[15].name, station.getString("n16", "NPO Radio 5").c_str());
  strcpy(stationlist[15].url, station.getString("u16", "http://icecast.omroep.nl/radio5-bb-mp3").c_str());
  strcpy(stationlist[16].name, station.getString("n17", "BNR News Radio").c_str());
  strcpy(stationlist[16].url, station.getString("u17", "http://playerservices.streamtheworld.com/api/livestream-redirect/BNR_NIEUWSRADIO.mp3").c_str());
  strcpy(stationlist[17].name, station.getString("n18", "Classic FM NL").c_str());
  strcpy(stationlist[17].url, station.getString("u18", "http://playerservices.streamtheworld.com/api/livestream-redirect/CLASSICFM.mp3").c_str());
  strcpy(stationlist[18].name, station.getString("n19", "Kink FM").c_str());
  strcpy(stationlist[18].url, station.getString("u19", "http://playerservices.streamtheworld.com/api/livestream-redirect/KINK.mp3").c_str());
  strcpy(stationlist[19].name, station.getString("n20", "Empty Station").c_str());
  strcpy(stationlist[19].url, station.getString("u20", "").c_str());
  
}