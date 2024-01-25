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

// --- Wss StaTe (WST) define ------- 
uint16_t WST = 0; 
#define WST_setupStart   0x1000
#define WST_setupDone    0x2000
#define WST_loop         0x3000
#define WST_stt          0x5000
#define WST_chatGPT      0x6000
#define WST_voicevox     0x7000
#define WST_talkDone     0x9000
void stateManage();


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
  log_free_size("初期化終了：");
  showExeTime("setup()  --- End --- ");
  WST = WST_setupDone;
}

void stateManage()
{
  if(WST != WST_loop)
  {
    switch(WST)
    {
      case WST_setupDone:
        WST = WST_loop;
        break;

      case WST_talkDone:
        WST = WST_loop;
        break;  
    }
  }
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

