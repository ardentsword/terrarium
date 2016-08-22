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

#define letterH 48
#define letterW 32

#define padding 7 // padd 1 mm

enum settingsState{
  minTemp,
  minTempS,
  maxTemp,
  maxTempS,
  monTimeH,
  monTimeM,
  eveTimeH,
  eveTimeM,
  settingsStateFirst = minTemp,
  settingsStateLast = eveTimeM
};

settingsState sMenuState = settingsState::minTemp;

void settingsHighLight(){
  switch(sMenuState){
    case settingsState::minTemp: myGLCD.drawRect(box1x+padding-2, box0y+padding+letterH+1, box1x+padding+2*letterW+2, box0y+padding+letterH+2); break;
    case settingsState::minTempS: myGLCD.drawRect(box1x+padding+3*letterW-2, box0y+padding+letterH+1, box1x+padding+5*letterW+2, box0y+padding+letterH+2); break;
    case settingsState::maxTemp: myGLCD.drawRect(box1x+padding-2, box1y+padding+letterH+1, box1x+padding+2*letterW+2, box1y+padding+letterH+2); break;
    case settingsState::maxTempS: myGLCD.drawRect(box1x+padding+3*letterW-2, box1y+padding+letterH+1, box1x+padding+5*letterW+2, box1y+padding+letterH+2); break;
    case settingsState::monTimeH: myGLCD.drawRect(box1x+padding-2, box2y+padding+letterH+1, box1x+padding+2*letterW+2, box2y+padding+letterH+2); break;
    case settingsState::monTimeM: myGLCD.drawRect(box1x+padding+3*letterW-2, box2y+padding+letterH+1, box1x+padding+5*letterW+2, box2y+padding+letterH+2); break;
    case settingsState::eveTimeH: myGLCD.drawRect(box1x+padding-2, box3y+padding+letterH+1, box1x+padding+2*letterW+2, box3y+padding+letterH+2); break;
    case settingsState::eveTimeM: myGLCD.drawRect(box1x+padding+3*letterW-2, box3y+padding+letterH+1, box1x+padding+5*letterW+2, box3y+padding+letterH+2); break;
    default: break;
  }
}

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

  myGLCD.print(String(mySettings.lampMinTemp)+"gC", box1x+padding, box0y+padding);
  myGLCD.print(String(mySettings.lampMaxTemp)+"gC", box1x+padding, box1y+padding);
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
    settingsHighLight();
    settingsDrawn = true;
  }

  if(encoder0Down){
    switch(sMenuState){
      case settingsState::minTemp: mySettings.lampMinTemp++; break;
      case settingsState::minTempS: mySettings.lampMinTemp+=0.2f; break;
      case settingsState::maxTemp: mySettings.lampMaxTemp++; break;
      case settingsState::maxTempS: mySettings.lampMaxTemp+=0.2f; break;
      case settingsState::monTimeH: mySettings.lampStartH++; break;
      case settingsState::monTimeM: mySettings.lampStartM+=5; break;
      case settingsState::eveTimeH: mySettings.lampStopH++; break;
      case settingsState::eveTimeM: mySettings.lampStopM+=5; break;
      default: break;
    }
    settingsRefresh();
  }

  if(encoder0Up){
    switch(sMenuState){
      case settingsState::minTemp: mySettings.lampMinTemp--; break;
      case settingsState::minTempS: mySettings.lampMinTemp-=0.2f; break;
      case settingsState::maxTemp: mySettings.lampMaxTemp--; break;
      case settingsState::maxTempS: mySettings.lampMaxTemp-=0.2f; break;
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
      myGLCD.setColor(VGA_BLACK);
      settingsHighLight(); // remove old HL
      myGLCD.setColor(VGA_WHITE);
      sMenuState = (settingsState)(sMenuState+1);
      settingsHighLight();
    }else{
        myState = menuState::menu;
        settingsDrawn = false;
        saveSettings();
    }
  }
}
