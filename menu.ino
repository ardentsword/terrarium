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
  {"Read DHT22"},
  {"overview"},
  {"three"},
  {"four"},
  {"five"},
  {"six"},
};

void menuDraw(){
  clearScreen();

  for(int i = 0;i < menuSize; i++){
    if(i == menuSelected){
      myButtons.addButton(MENU_OFFSET_X, MENU_OFFSET_Y + 40*i, 300, 30, items[i].name);
    }else{
      myButtons.addButton(MENU_OFFSET_X, MENU_OFFSET_Y + 40*i, 300, 30, items[i].name, BUTTON_DISABLED);
    }
  }
  myButtons.drawButtons();

  // draw the top menu bar
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 479, 20);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("Menu: ", 2, 2);
  myGLCD.setBackColor(0, 0, 0);
}

void menuSelect(int item){
  if(item >= menuSize){
    item = menuSize-1;
  }
  if(item < 0){
    item = 0;
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
