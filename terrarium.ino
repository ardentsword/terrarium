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

//#include "SixteenSegment32x48.c"

#include <UTFT.h>
#include <UTFT_Buttons.h>

#include <DHT.h>

#include <Time.h>
#include <TimeAlarms.h>

#include <Wire.h>
#include "RTClib.h"

#include <Bounce2.h>

#define encoder0PinA 2
#define encoder0PinB 3
#define encoder0Button 6
#define lampRelay 8
#define speakerOut 9
#define OUT_UU2 10 // unused orange cable

#define DHT1PIN 7
#define DHT2PIN 7
#define DHTTYPE DHT22   // DHT 21 (AM2301)

#define SCREEN_H 319
#define SCREEN_W 479

#define UPDATE_DELAY 5 // seconds between update() call

volatile bool encoder0Up = false;
volatile bool encoder0Down = false;

// Declare which fonts we will be using
extern uint8_t BigFont[];
extern uint8_t SixteenSegment[];
extern uint8_t Dingbats1_XL[];

UTFT myGLCD(ILI9481,38,39,40,41);
UTFT_Buttons myButtons(&myGLCD);

DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);

RTC_DS3231 rtc;

Bounce but0 = Bounce();

settings mySettings = settings();

float temperature = 0.0f;
float humidity = 0.0f;

// output light states
bool lamp0 = false;

enum menuState{
  menu,
  overview,
  settingsMenu
};

menuState myState = menuState::overview;

enum lampState{
  cold,
  warm,
  _day,
  night
};

lampState lamp0state = night;

void update() {
  // Read temperature as Celsius (the default)
  temperature = dht1.readTemperature();
  humidity = dht1.readHumidity();

  // check the time and temps
  if( mySettings.lampAuto ){
    bool isDay = false;
    if( mySettings.lampStartH < hour() && hour() < mySettings.lampStopH ){
      isDay = true;
    }else if( mySettings.lampStartH == hour() && minute() > mySettings.lampStartM ){
      isDay = true;
    }else if( mySettings.lampStopH == hour() && minute() < mySettings.lampStopM ){
      isDay = true;
    }

    if( temperature < mySettings.lampMinTemp ){
      lamp0 = true;
      lamp0state = lampState::cold;
    }else if( temperature > mySettings.lampMaxTemp){
      lamp0 = false;
      lamp0state = lampState::warm;
    }else if( isDay ){
      lamp0 = true;
      lamp0state = lampState::_day;
    }else{
      lamp0 = false;
      lamp0state = lampState::night;
    }
  }
}

long unsigned int getNow(){
  return rtc.now().unixtime();
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
  pinMode(lampRelay, OUTPUT);
  pinMode(speakerOut, OUTPUT);
  myGLCD.print("IO's initialized", 5, 5+20*tHeight++);

  but0.attach(encoder0Button);
  but0.interval(5); // interval in ms
  myGLCD.print("Debouncer initialized", 5, 5+20*tHeight++);

  attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoder, CHANGE);  // encoder pin on interrupt INT5 - pin PE5
  myGLCD.print("interrupts initialized", 5, 5+20*tHeight++);

  myButtons.setTextFont(BigFont);
  myButtons.setSymbolFont(Dingbats1_XL);
  myGLCD.print("lcd button lib initialized", 5, 5+20*tHeight++);

  dht1.begin();
  myGLCD.print("dht22's initialized", 5, 5+20*tHeight++);

  for(int i = 0; i<10; i++){
    myGLCD.print(".", 5+10*i, 5+20*tHeight);
    Alarm.delay(1000);
    if(!digitalRead(encoder0Button)){
      break;
    }
  }
  update();

  // set the update function every minute
  Alarm.timerRepeat(UPDATE_DELAY, update);
}

void loop()
{
  // update debouncers
  but0.update();

  if(myState == menu){
    menuLoop();
  }else if(myState == overview){
    overviewLoop();
  }else if(myState == settingsMenu){
    settingsLoop();
  }

  if(digitalRead(lampRelay) != lamp0){
    digitalWrite(lampRelay, lamp0);
  }

  if(encoder0Up){
    encoder0Up = false;
  }
  if(encoder0Down){
    encoder0Down = false;
  }

  Alarm.delay(1);
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
