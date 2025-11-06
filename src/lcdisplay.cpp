#include <Arduino.h>
//library for LCD display
#include <LiquidCrystal_I2C.h>

//structure for stationlist
typedef struct {
  char url[100];  //stream url
  char name[100]; //stations name
} Station;

// External declarations from main.cpp
extern uint8_t curStation;
extern uint8_t actStation;
extern Station stationlist[];

//instance for LCD display
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Special character to show a speaker icon for current station
uint8_t speaker[8]  = {0x3,0x5,0x19,0x11,0x19,0x5,0x3};

//show name of current station on LCD display
//show the speaker symbol in front if current station = active station
void showStation() {
  int loff=0;
  lcd.clear();
  if (curStation == actStation) {
    lcd.home();
    lcd.print(char(1));
    loff=2;
  }
  lcd.setCursor(loff,0);
  lcd.print(curStation+1);
  if (curStation > 8) {
    lcd.setCursor(loff+2,0);
  } else {
    lcd.setCursor(loff+1,0);
  }
  lcd.print(":");
  String name = String(stationlist[curStation].name);
  if (name.length() < 12-loff)
    lcd.print(name);
  else {
    uint8_t p = name.lastIndexOf(" ",15); //if name does not fit, split line on space
    lcd.print(name.substring(0,p));
    lcd.setCursor(0,1);
    lcd.print(name.substring(p+1,p+17));
  }
}


// to be called in 'setup()'
void setup_lcdisplay()
{
  //init the LCD display
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, speaker);
  lcd.clear();
  lcd.home();
  lcd.setCursor(4,0);
  lcd.print("WLAN");  // display shows 'WLAN' until radio ist connected and playing
}