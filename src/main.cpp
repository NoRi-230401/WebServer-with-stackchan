// ---- <main.cpp>-------------------------------------------------------
#include "main.h"
const String WS_VERSION = "WebServer-with-stackchan_V304-240106";

// ---------------------------------------------------------------------
// Extended from
//  ESPAsynch_Server_v1.1       : by David Bird 2022
//  AiStackChanEx_v112          : by NoRi 2023-06-23
// ----------------------------------------------------------------------

void setup()
{
  // ** initial Setting **
  M5StackConfig();
  log_free_size("初期化開始：");

  // ** Setting files **
  startupSetting();
  apikeySetting();
  servoSetting();
  
  // *** Network  ***
  wifiSetup();
  serverSetup();
  clockSetup();
  networkInformation();
  
  // *** Servo,TTS,chatGpt ***
  servoSetup();
  ttsSetup();
  chatGptSetup();
  
  delay(3000);
    
 //*** Wake UP! STACKCHAN ***
  avatarSTART();
  log_free_size("初期化終了：");
}


void loop()
{
  chatGptManage();
  TimerManage();
  ButtonManage();
  RequestManage();
  SpeechManage();
  StatusLineManage();
}

