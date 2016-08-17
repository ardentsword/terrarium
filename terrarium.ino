#include <Arduino.h>
#include "types.h"

// UTFT_Demo_480x320
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution
// of 480x320 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>
#include <UTFT_Buttons.h>

#include <DHT.h>

#include <Time.h>

#include <Wire.h>
#include "RTClib.h"

#define encoder0PinA 2
#define encoder0PinB 3
#define encoder0Button 6

#define DHT1PIN 7
#define DHT2PIN 7
#define DHTTYPE DHT22   // DHT 21 (AM2301)

#define SCREEN_H 319
#define SCREEN_W 479

volatile bool encoder0Up = false;
volatile bool encoder0Down = false;

// Declare which fonts we will be using
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t Dingbats1_XL[];

UTFT myGLCD(ILI9481,38,39,40,41);
UTFT_Buttons myButtons(&myGLCD);

DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);

RTC_DS3231 rtc;

float temperature = 0.0f;
float humidity = 0.0f;

int prevSeconds = 0;

enum state{
  menu,
  overview
};

state myState = menu;

void update() {
  // Read temperature as Celsius (the default)
  temperature = dht1.readTemperature();
  humidity = dht1.readHumidity();

  if(myState == overview){
    refreshOverview();
  }
}

long unsigned int getNow(){
  DateTime now = rtc.now();
  return now.unixtime();
}

void setup()
{
  //randomSeed(analogRead(0));
  int tHeight = 0;

  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);

  clearScreen();
  myGLCD.print("LCD initialized", 5, 5+20*tHeight++);

  // set the time
  bool rtcB = rtc.begin();
  myGLCD.print("rtc begin: "+String(rtcB), 5, 5+20*tHeight++);
  setSyncProvider(getNow);
  setSyncInterval(10);
  myGLCD.print("rtc sync set: "+String(hour())+":"+String(minute())+":"+String(second()), 5, 5+20*tHeight++);
  //setTime(getNow());

  // Setup the rotary encoder
  pinMode(encoder0PinA, INPUT_PULLUP);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT_PULLUP);       // turn on pullup resistor
  pinMode(encoder0Button, INPUT_PULLUP);       // turn on pullup resistor
  myGLCD.print("IO's initialized", 5, 5+20*tHeight++);

  attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoder, CHANGE);  // encoder pin on interrupt INT5 - pin PE5
  myGLCD.print("interrupts initialized", 5, 5+20*tHeight++);

  myButtons.setTextFont(BigFont);
  myButtons.setSymbolFont(Dingbats1_XL);
  myGLCD.print("lcd button lib initialized", 5, 5+20*tHeight++);

  dht1.begin();
  myGLCD.print("dht22's initialized", 5, 5+20*tHeight++);

  for(int i = 0; i<10; i++){
    myGLCD.print(".", 5+10*i, 5+20*tHeight);
    delay(1000);
  }

  menuDraw();
  update();
}

void loop()
{
  if(myState == menu){
    if(encoder0Up){
      menuUp();
      encoder0Up = false;
    }
    if(encoder0Down){
      menuDown();
      encoder0Down = false;
    }
    if(!digitalRead(encoder0Button)){
      int s = getMenuSelected();

      myGLCD.setColor(VGA_WHITE);
      myGLCD.print("Pressed:", 350, 50);
      myGLCD.print(String(s+1), 350, 70);

      if(s == 0){
        update();
        myGLCD.print("T:"+String(temperature)+"c", 350, 100);
        myGLCD.print("H:"+String(humidity)+"%", 350, 120);
      }else if(s == 1){
        myState = overview;
      }
      delay(10);
    }
  }else if(myState == overview){
    drawOverview();

    while(true){
      delay(10);
      if(prevSeconds != second()){
        prevSeconds = second();
        refreshOverview();
      }

      if(!digitalRead(encoder0Button)){
        myState = menu;
        menuDraw();
      }
    }
  }
}

void doEncoder() {
  // readout the rotary encoder
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Up = true;
    encoder0Down = false;
  } else {
    encoder0Down = true;
    encoder0Up = false;
  }
}
