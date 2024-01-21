// -------------------------< main.h >--------------------
#ifndef WS_MAIN_H
#define WS_MAIN_H
// -------------------------------------------------------
#include <Arduino.h>
extern const String WSS_VERSION;
extern void M5StackConfig();
extern void M5SpeakerConfig();
extern void startupSetting();
extern void startupSetting00();
extern void apikeySetting();
extern void servoSetting();
extern void wifiSetup();
extern void serverSetup();
extern void servoSetup();
// extern void ttsSetup();
extern void chatGptSetup();
extern void avatarSTART();
extern void chatGptManage();
extern void TimerManage();
extern void ButtonManage();
extern void RequestManage();
extern void StatusLineManage();
extern void clockSetup();
extern void WiFiInfo();
extern void log_free_size(const char *text);
#define EXE_TM_MD0 0 // (default) disp and timer reset
#define EXE_TM_MD1 1 // disp and timer no reset
#define EXE_TM_MD2 2 // no disp and timer reset
#define EXE_TM_MD_START 3 // no disp and timer reset for START 

extern void showExeTime(String msg, int mode = EXE_TM_MD0);
// ----------------------------------------------------------

void setup();
void loop();
// ---------------end of < WS_MAIN_H > -----------------------
#endif
