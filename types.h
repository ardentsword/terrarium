typedef struct _settings{
  float lampMinTemp;
  float lampMaxTemp;
  int lampStartH;// hours
  int lampStartM;// minutes
  int lampStopH;// hours
  int lampStopM;// minutes

  _settings(){
      lampMinTemp = 15.0f;
      lampMaxTemp = 28.0f;
      lampStartH = 11;
      lampStartM = 0;
      lampStopH = 20;
      lampStopM = 00;
  }
} settings;
