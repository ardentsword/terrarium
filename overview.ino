#include <Arduino.h>

// the overview showing all the information

#define PXper1MM 7.0922 // pixels per mm
#define PXper5MM 35.46 // pixels per 5 mm

// the boxes
#define box00x 0
#define box01x 106
#define box02x 212
#define box03x 318

#define box10x 0
#define box11x 247

#define box0y 0
#define box1y 177

#define box0text0y 0 // first text height of first box
#define box0text1y 35 // second text height of first box
#define box0text2y 71 // second text height of first box

#define padding 7 // padd 1 mm

#define clockCenterX 390 // box03x + 2 x PXper5MM
#define clockCenterY 106 // box0text0y

void drawSec(int s)
{
  float x1, y1, x2, y2;
  int ps = s-1;

  myGLCD.setColor(0, 0, 0);
  if (ps==-1)
    ps=59;
  ps=ps*6;
  ps=ps+270;

  x1=43*cos(ps*0.0175);
  y1=43*sin(ps*0.0175);
  x2=36*cos(ps*0.0175);
  y2=36*sin(ps*0.0175);

  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);

  myGLCD.setColor(255, 0, 0);
  s=s*6;
  s=s+270;

  x1=43*cos(s*0.0175);
  y1=43*sin(s*0.0175);
  x2=36*cos(s*0.0175);
  y2=36*sin(s*0.0175);

  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);
}

void drawMin(int m)
{
  float x1, y1, x2, y2;
  int pm = m-1;

  myGLCD.setColor(0, 0, 0);
  if (pm==-1)
    pm=59;
  pm=pm*6;
  pm=pm+270;

  x1=35*cos(pm*0.0175);
  y1=35*sin(pm*0.0175);
  x2=5*cos(pm*0.0175);
  y2=5*sin(pm*0.0175);

  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);

  myGLCD.setColor(0, 255, 0);
  m=m*6;
  m=m+270;

  x1=35*cos(m*0.0175);
  y1=35*sin(m*0.0175);
  x2=5*cos(m*0.0175);
  y2=5*sin(m*0.0175);

  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);
}

void drawHour(int h, int m)
{
  float x1, y1, x2, y2;
  int ph = h;

  myGLCD.setColor(0, 0, 0);
  if (m==0){
    ph=((ph-1)*30)+((m+59)/2);
  }else{
    ph=(ph*30)+((m-1)/2);
  }
  ph=ph+270;

  x1=30*cos(ph*0.0175);
  y1=30*sin(ph*0.0175);
  x2=5*cos(ph*0.0175);
  y2=5*sin(ph*0.0175);

  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);

  myGLCD.setColor(255, 255, 0);
  h=(h*30)+(m/2);
  h=h+270;

  x1=30*cos(h*0.0175);
  y1=30*sin(h*0.0175);
  x2=5*cos(h*0.0175);
  y2=5*sin(h*0.0175);

  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);
}

void drawMark(int h)
{
  float x1, y1, x2, y2;

  h=h*30;
  h=h+270;

  x1=55*cos(h*0.0175);
  y1=55*sin(h*0.0175);
  x2=50*cos(h*0.0175);
  y2=50*sin(h*0.0175);

  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);
}

void drawClock(){
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  for (int i=0; i<1; i++)
  {
    myGLCD.drawCircle(clockCenterX, clockCenterY, 59-i);
  }
  for (int i=4; i<5; i++)
  {
    myGLCD.drawCircle(clockCenterX, clockCenterY, i);
  }

  myGLCD.setColor(192, 192, 255);
  myGLCD.print("3", clockCenterX+42, clockCenterY-8);
  myGLCD.print("6", clockCenterX-8, clockCenterY+43);
  myGLCD.print("9", clockCenterX-58, clockCenterY-8);
  myGLCD.print("12", clockCenterX-16, clockCenterY-55);

  for (int i=0; i<12; i++)
  {
    if ((i % 3)!=0)
      drawMark(i);
  }
}

void overviewDraw(){
  clearScreen();
  drawClock();
  // draw separator lines
  myGLCD.setColor(33, 169, 247);
  // draw box boundaries
  myGLCD.drawRect(box10x, box1y, SCREEN_W, box1y+1);

  myGLCD.drawRect(box01x, box0y, box01x+1, box1y);
  myGLCD.drawRect(box02x, box0y, box02x+1, box1y);
  myGLCD.drawRect(box03x, box0y, box03x+1, box1y);

  myGLCD.drawRect(box11x, box1y, box11x+1, SCREEN_H);

  overviewRefresh();
}

void overviewRefresh(){
  //draw box 0 values
  myGLCD.setColor(VGA_RED);
  myGLCD.print(String(mySettings.lampMinTemp)+"C" ,box00x+padding, box0text0y+padding);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.print(String(mySettings.lampMaxTemp)+"C" ,box00x+padding, box0text1y+padding);

  myGLCD.setColor(15, 15, 220);
  //draw box 1 values
  myGLCD.print(String(mySettings.lampStartH)+":"+String(mySettings.lampStartM), box01x+padding, box0text0y+padding);
  myGLCD.print(String(mySettings.lampStopH)+":"+String(mySettings.lampStopM), box01x+padding, box0text1y+padding);

  // draw box 2 lamp state
  myGLCD.print("Lamp", box02x+padding, box0text0y+padding);
  if(mySettings.lampAuto){
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.print("auto", box02x+padding, box0text1y+padding);
    if(lamp0){
      myGLCD.setColor(VGA_GREEN);
    }else{
      myGLCD.setColor(VGA_RED);
    }
    String l0s;
    switch(lamp0state){
      case lampState::cold: l0s = "cold"; break;
      case lampState::warm: l0s = "warm"; break;
      case lampState::_day: l0s = "day"; break;
      case lampState::night: l0s = "night"; break;
      default: l0s = "unk"; break;
    }
    myGLCD.print(l0s, box02x+padding, box0text2y+padding);
  }else{
    myGLCD.setColor(VGA_AQUA);
    myGLCD.print("manual", box02x+padding, box0text1y+padding);
    if(lamp0){
      myGLCD.setColor(VGA_GREEN);
      myGLCD.print("on ", box02x+padding, box0text2y+padding);
    }else{
      myGLCD.setColor(VGA_RED);
      myGLCD.print("off", box02x+padding, box0text2y+padding);
    }
  }

  // draw box 3 time
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print(String(hour())+":"+String(minute())+":"+String(second()), box03x+padding, box0y+padding);

  // draw big bottom text
  myGLCD.setColor(15, 15, 220);
  myGLCD.setFont(SixteenSegment);
  myGLCD.print(String(temperature)+"C", box10x+padding, box1y+padding);
  myGLCD.print(String(humidity)+"%", box11x+padding, box1y+padding);
  myGLCD.setFont(BigFont);

  // redraw the clock
  drawSec(second());
  drawMin(minute());
  drawHour(hour(), minute());
}

bool overviewDrawn = false;
int prevSeconds = 0;

void overviewLoop(){
  if(!overviewDrawn){
    overviewDraw();
    overviewDrawn = true;
  }
  // refresh every second
  if(prevSeconds != second()){
    prevSeconds = second();
    overviewRefresh();
  }

  if(encoder0Up){
    encoder0Up = false;
  }
  if(encoder0Down){
    if( !mySettings.lampAuto ){
      lamp0 = !lamp0;
      overviewRefresh();
    }
    encoder0Down = false;
  }

  if( but0.fell() ){
    myState = menuState::menu;
    overviewDrawn = false;
  }
}
