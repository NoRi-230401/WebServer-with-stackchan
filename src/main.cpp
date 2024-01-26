// ---- <main.cpp>-------------------------------------------------------
#include "main.h"
const String WSS_NAME = "WebServer-with-stackchan";
const String WSS_VER = "V305i-240126";
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
  WST = WST_setupStart;
  // ** initial Setting **
  M5StackConfig();
  log_free_size("初期化開始：");
  M5SpeakerConfig();

  // ** Setting files **
  startupSetting();
  apikeySetting();
  servoSetting();

  // *** Network  ***
  wifiSetup();
  serverSetup();
  clockSetup();
  WiFiInfo();

  // *** Servo, chatGpt ***
  servoSetup();
  chatGptSetup();
  delay(3000);

  //*** Wake UP! STACKCHAN ***
  avatarSTART();
  WST = WST_setupDone;
}


void loop()
{

  buttonManage();
  requestManage();
  statusLineManage();
  chatGptManage();
  timerManage();

  stateManage();

}

