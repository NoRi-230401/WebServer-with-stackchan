// -------------------------< main.h >--------------------
#ifndef WS_MAIN_H
#define WS_MAIN_H
// -------------------------------------------------------
#include <Arduino.h>


extern const String WS_VERSION;

extern void M5StackConfig();
extern bool apiKeyFileRead();
extern bool apiKeyTxtRead();
extern bool startupFileRead();
extern void nvsSaveAll();
extern bool servoFileRead();
extern void wifiSetup();
extern void serverSetup();
extern void servoSetup();
extern void ttsSetup();
extern void chatGptSetup();
extern void avatarSTART();
extern void chatGptManage();
extern void TimerManage();
extern void ButtonManage();
extern void RequestManage();
extern void SpeechManage();

// ----------------------------------------------------------
void setup();
void loop();
// ---------------end of < WS_MAIN_H > -----------------------
#endif
