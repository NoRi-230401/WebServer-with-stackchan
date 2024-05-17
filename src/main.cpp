// ---- <main.cpp>-------------------------------------------------------
#include "main.h"

const String WSS_NAME = "WebServer-with-stackchan";
const String WSS_VER = "v310a-240517";
const String WSS_VERSION = WSS_NAME + " " + WSS_VER;
const String WSS_SD_BIN = "/wss.bin";  // default-SDUpdater-bin-fileName 

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
  M5StackConfig01();
  SDU_lobby();
  M5StackConfig02();
  
  Serial.println(WSS_VERSION);
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

  delay(2000);

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
