#include <Arduino.h>

// the main menu

typedef struct{
  char name[30];
  char alt[30];
} MenuItem;

#define MENU_OFFSET_X 10
#define MENU_OFFSET_Y 30

// menu vars
int menuSelected = 0;
const int menuSize = 6;

MenuItem items[menuSize] = {
  {"Overview"},
  {"Read DHT22"},
  {"two"},
  {"three"},
  {"Lamp auto","Lamp manual"},
  {"five"},
};

bool buttonsAdded = false;

void menuDraw(){
  clearScreen();

  if(!buttonsAdded){
    for(int i = 0;i < menuSize; i++){
      if(i == menuSelected){
        myButtons.addButton(MENU_OFFSET_X, MENU_OFFSET_Y + 40*i, 300, 30, items[i].name);
      }else{
        myButtons.addButton(MENU_OFFSET_X, MENU_OFFSET_Y + 40*i, 300, 30, items[i].name, BUTTON_DISABLED);
      }
    }
    buttonsAdded = true;
  }
  if(mySettings.lampAuto){
    myButtons.relabelButton(4, items[4].name);
  }else{
    myButtons.relabelButton(4, items[4].alt);
  }

  myButtons.drawButtons();

  // draw the top menu bar
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 479, 20);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("Menu", 2, 2);
  myGLCD.setBackColor(0, 0, 0);
}

void menuSelect(int item){
  if(item >= menuSize){
    item = 0;
  }
  if(item < 0){
    item = menuSize - 1;
  }

  myButtons.disableButton(menuSelected, true);
  menuSelected = item;
  myButtons.enableButton(menuSelected, true);
}

void menuUp(){
  menuSelect(menuSelected - 1);
}

void menuDown(){
  menuSelect(menuSelected + 1);
}

int getMenuSelected(){
  return menuSelected;
}

bool menuDrawn = false;

void menuLoop(){
  if(!menuDrawn){
    menuDraw();
    menuDrawn = true;
  }

  if(encoder0Up){
    menuUp();
    encoder0Up = false;
  }
  if(encoder0Down){
    menuDown();
    encoder0Down = false;
  }

  if( but0.fell() ){
    // the select button is pressed
    int s = getMenuSelected();

    // debug print button pressed
    myGLCD.setColor(VGA_WHITE);
    myGLCD.print("Pressed:", 350, 50);
    myGLCD.print(String(s), 350, 70);

    if(s == 0){
      myState = menuState::overview;
      menuDrawn = false;
    }else if(s == 1){
      update();
      myGLCD.print("T:"+String(temperature)+"c", 350, 100);
      myGLCD.print("H:"+String(humidity)+"%", 350, 120);
    }else if(s == 4){
      mySettings.lampAuto = !mySettings.lampAuto;
      if(mySettings.lampAuto){
        myButtons.relabelButton(4, items[4].name, true);
      }else{
        myButtons.relabelButton(4, items[4].alt, true);
      }
    }
  }
}
