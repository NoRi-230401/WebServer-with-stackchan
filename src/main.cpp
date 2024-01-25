// ---- <main.cpp>-------------------------------------------------------
#include "main.h"
const String WSS_NAME = "WebServer-with-stackchan";
const String WSS_VER = "V305h-240124";
const String WSS_VERSION = WSS_NAME + " " + WSS_VER;

// ---------------------------------------------------------------------
//  *** Extended from ***
// AI_StackChan2                         : robo8080さん
// m5stack-avatar                        : ししかわさん/タカオさん
// ESPAsynch_Server_v1.1　　　　　　　　　: David Bird 2022
// M5Stack_Stack-chan_another_dimension  : つゆきぱぱさん
// ----------------------------------------------------------------------

uint16_t WSS_STATUS = 0;


void setup()
{
  WSS_STATUS = 0x0000;
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
  WSS_STATUS = 0x1000;
}

void StatusManage();
void StatusManage()
{
  if(WSS_STATUS != 0x2000)
  {
    switch(WSS_STATUS)
    {
      case 0x1000:
        WSS_STATUS = 0x2000;
        break;

      case 0x9000:
        WSS_STATUS = 0x2000;
        break;  
    }
  }
}

void loop()
{
  ButtonManage();
  RequestManage();
  StatusLineManage();
  chatGptManage();
  TimerManage();
  StatusManage();
}

