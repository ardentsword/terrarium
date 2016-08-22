#include <Arduino.h>

// the settings menu
/*
void settingsDraw(){
  settingsRefresh();
}

settingsRefresh(){

}

bool settingsDrawn = false;

void settingsLoop(){
    if(!settingsDrawn){
      settingsDraw();
      settingsDraw = true;
    }
}
menu entry prototype
*/

// the boxes
#define box0x 0
#define box1x 212

#define box0y 0
#define box1y 71
#define box2y 141
#define box3y 212

#define padding 7 // padd 1 mm

enum settingsState{
  minTemp,
  maxTemp,
  monTimeH,
  monTimeM,
  eveTimeH,
  eveTimeM,
  settingsStateFirst = minTemp,
  settingsStateLast = eveTimeM
};

settingsState sMenuState = settingsState::minTemp;

void settingsDraw(){
  clearScreen();

  myGLCD.setFont(SixteenSegment);

  myGLCD.print("MIN TP", box0x+padding, box0y+padding);
  myGLCD.print("MAX TP", box0x+padding, box1y+padding);
  myGLCD.print("MON TM", box0x+padding, box2y+padding);
  myGLCD.print("EVE TM", box0x+padding, box3y+padding);

  myGLCD.setFont(BigFont);

  settingsRefresh();
}

void settingsRefresh(){
  myGLCD.setFont(SixteenSegment);

  myGLCD.print(String(mySettings.lampMinTemp)+"C", box1x+padding, box0y+padding);
  myGLCD.print(String(mySettings.lampMaxTemp)+"C", box1x+padding, box1y+padding);
  myGLCD.print(String(mySettings.lampStartH)+":"+String(mySettings.lampStartM), box1x+padding, box2y+padding);
  myGLCD.print(String(mySettings.lampStopH)+":"+String(mySettings.lampStopM), box1x+padding, box3y+padding);

  myGLCD.setFont(BigFont);

  myGLCD.print(String(sMenuState), padding, 300);
}

bool settingsDrawn = false;

void settingsLoop(){
  if(!settingsDrawn){
    // reset the menu state the first time
    sMenuState = settingsState::settingsStateFirst;
    settingsDraw();
    settingsDrawn = true;
  }

  if(encoder0Up){
    switch(sMenuState){
      case settingsState::minTemp: mySettings.lampMinTemp++; break;
      case settingsState::maxTemp: mySettings.lampMaxTemp++; break;
      case settingsState::monTimeH: mySettings.lampStartH++; break;
      case settingsState::monTimeM: mySettings.lampStartM+=5; break;
      case settingsState::eveTimeH: mySettings.lampStopH++; break;
      case settingsState::eveTimeM: mySettings.lampStopM+=5; break;
      default: break;
    }
    settingsRefresh();
  }

  if(encoder0Down){
    switch(sMenuState){
      case settingsState::minTemp: mySettings.lampMinTemp--; break;
      case settingsState::maxTemp: mySettings.lampMaxTemp--; break;
      case settingsState::monTimeH: mySettings.lampStartH--; break;
      case settingsState::monTimeM: mySettings.lampStartM-=5; break;
      case settingsState::eveTimeH: mySettings.lampStopH--; break;
      case settingsState::eveTimeM: mySettings.lampStopM-=5; break;
      default: break;
    }
    settingsRefresh();
  }

  if( but0.fell() ){
    if(sMenuState != settingsState::settingsStateLast){
      sMenuState = (settingsState)(sMenuState+1);
    }else{
        myState = menuState::menu;
        settingsDrawn = false;
    }
  }
}
