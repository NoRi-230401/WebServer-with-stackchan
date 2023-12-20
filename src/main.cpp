// ---- <main.cpp>-------------------------------------------------------
#include "main.h"
const String WS_VERSION = "WebServer-with-stackchan_V302-231222";

// ---------------------------------------------------------------------
// Extended from
//  ESPAsynch_Server_v1.1       : by David Bird 2022
//  AiStackChanEx_v112          : by NoRi 2023-06-23
// ----------------------------------------------------------------------

void setup()
{
  // ** initial Setting **
  M5StackConfig();
  
  // ** Setting files **
  startupSetting();
  apikeySetting();
  servoSetting();
  
  // *** Network  ***
  wifiSetup();
  serverSetup();

  // *** Servo,TTS,chatGpt ***
  servoSetup();
  ttsSetup();
  chatGptSetup();
  
  delay(3000);
    
 //*** Wake UP! STACKCHAN ***
  avatarSTART();
}

 
void loop()
{
  chatGptManage();
  TimerManage();
  ButtonManage();
  RequestManage();
  SpeechManage();
}

