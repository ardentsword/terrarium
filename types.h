#include <Arduino.h>

// ID of the settings block
#define CONFIG_VERSION "ls1"

typedef struct _settings{
  char version[4];
  float lampMinTemp;
  float lampMaxTemp;
  int lampStartH;// hours
  int lampStartM;// minutes
  int lampStopH;// hours
  int lampStopM;// minutes
  bool lampAuto;

  _settings(){
    strcpy(version, CONFIG_VERSION);
    lampMinTemp = 15.0f;
    lampMaxTemp = 28.0f;
    lampStartH = 11;
    lampStartM = 0;
    lampStopH = 20;
    lampStopM = 00;
    lampAuto = true;
  }
} settings;
