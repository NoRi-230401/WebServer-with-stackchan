// ---- <main.cpp>-------------------------------------------------------
#include "main.h"
const String WS_VERSION = "wsStackChan_V201-230721";
// ----------------------------------------------------------------------
// Extended from
//  ESPAsynch_Server_v1.1              : 2022        David Bird
//  AiStackChanEx_v112                 : 2023-06-23  NoRi
// -----------------------------------------------------------------------

void setup()
{
  // *** initial Setting ***
  M5StackConfig();
  
  // *** Setting files ***
  apiKeyFileRead();
  startupFileRead();
  servoFileRead();

  // *** Network  ***
  wifiSetup();
  serverSetup();

  // *** Servo,TTS,chatGpt ***
  servoSetup();
  ttsSetup();
  chatGptSetup();
  
  delay(3000);
    
  // ** Avater Start **
  avatarSetup();
}


void loop()
{
  chatGptManage();
  TimerManage();
  ButtonManage();
  RequestManage();
  SpeechManage();
}

