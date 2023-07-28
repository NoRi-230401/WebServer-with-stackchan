// -------------------------< main.h >--------------------
#ifndef WS_MAIN_H
#define WS_MAIN_H
// -------------------------------------------------------
#include <Arduino.h>


extern const String WS_VERSION;
// extern String SERVER_NAME;

extern void M5StackConfig();
extern void apiKeyFileRead();
extern void startupFileRead();
extern void servoFileRead();
extern void wifiSetup();
extern void serverSetup();
extern void servoSetup();
extern void ttsSetup();
extern void chatGptSetup();
extern void avatarSetup();
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
