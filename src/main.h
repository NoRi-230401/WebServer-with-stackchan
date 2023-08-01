// -------------------------< main.h >--------------------
#ifndef WS_MAIN_H
#define WS_MAIN_H
// -------------------------------------------------------
#include <Arduino.h>

extern const String WS_VERSION;

extern void M5StackConfig();
extern void startupSetting();
extern void apikeySetting();
extern void servoSetting();
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
