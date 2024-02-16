// ---- <main.cpp>-------------------------------------------------------
#include "main.h"

const String WSS_NAME = "WebServer-with-stackchan";
const String WSS_VER = "v307a-240216";
const String WSS_VERSION = WSS_NAME + " " + WSS_VER;
// ---------------------------------------------------------------------
//  *** Extended from ***
// AI_StackChan2                         : robo8080さん
// m5stack-avatar                        : ししかわさん/タカオさん
// ESPAsynch_Server_v1.1　　　　　　　　　: David Bird 2022
// M5Stack_Stack-chan_another_dimension  : つゆきぱぱさん
// ----------------------------------------------------------------------

void setup()
{
  WST = WST_SETUP_start;
  // ** initial Setting **
  M5StackConfig();
  M5LedBegin();
  ledRed();
  M5SpeakerConfig();
  M5FileSystemBegin();
  
  // ** Setting files **
  startupSetting();
  apikeySetting();
  servoSetting();

  // *** Network  ***
  ledGreen();
  wifiSetup();
  serverSetup();
  clockSetup();

  // *** Servo, chatGpt ***
  servoSetup();
  chatGptSetup();
  ledBlue();
  delay(3000);

  //*** Wake UP! STACKCHAN ***
  avatarSTART();
  statusLineSetup();
  BoxTouchSetup();
  muteOff();
  ledClearAll();
  WST = WST_SETUP_done;
}

void loop()
{
  stateManage();
  buttonManage();
  requestManage();
  statusLineManage();

  chatGptManage();
  timerManage();
}

