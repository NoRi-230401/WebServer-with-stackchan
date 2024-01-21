// ---- <main.cpp>-------------------------------------------------------
#include "main.h"
const String WS_NAME = "WebServer-with-stackchan";
const String WS_VER = "V305-240126";
const String WS_VERSION = WS_NAME + " " + WS_VER;

// ---------------------------------------------------------------------
//  *** Extended from ***
// AI_StackChan2                         : robo8080さん
// m5stack-avatar                        : ししかわさん/タカオさん
// ESPAsynch_Server_v1.1　　　　　　　　　: David Bird 2022
// M5Stack_Stack-chan_another_dimension  : つゆきぱぱさん
// ----------------------------------------------------------------------

void setup()
{
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
  log_free_size("初期化終了：");
  showExeTime("setup()  --- End --- ");
}

void loop()
{
  ButtonManage();
  RequestManage();
  StatusLineManage();
  chatGptManage();
  TimerManage();
}
