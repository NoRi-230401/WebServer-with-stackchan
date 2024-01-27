// -------------------------< main.h >--------------------
#ifndef WS_MAIN_H
#define WS_MAIN_H
// -------------------------------------------------------
#include <Arduino.h>
extern const String WSS_VERSION;
extern void M5StackConfig();
extern void M5SpeakerConfig();
extern void startupSetting();
// extern void startupSetting00();
extern void apikeySetting();
extern void servoSetting();
extern void wifiSetup();
extern void serverSetup();
extern void servoSetup();
extern void chatGptSetup();
extern void avatarSTART();

extern void chatGptManage();
extern void timerManage();
extern void buttonManage();
extern void requestManage();
extern void statusLineManage();
extern void clockSetup();
extern void WiFiInfo();
extern void muteOff();

extern void log_free_size(const char *text);
#define EXE_TM_MD0 0 // (default) disp and timer reset
#define EXE_TM_MD1 1 // disp and timer no reset
#define EXE_TM_MD2 2 // no disp and timer reset
#define EXE_TM_MD_START 3 // no disp and timer reset for START 
// extern void showExeTime(String msg, int mode = EXE_TM_MD0);

// --- Wss StaTe (WST) define ------- 
extern uint16_t WST ;
#define WST_INIT          0x0000
#define WST_SETUP_start   0x1000
#define WST_SETUP_done    0x1900
#define WST_LOOP          0x2000
#define WST_STT           0x3000
#define WST_chatGPT       0x4000
#define WST_TTS_start     0x5000
#define WST_TTS_mp3urlGet 0x5100
#define WST_TTS_talkStart 0x5200
#define WST_TTS_ttsExit   0x5800
#define WST_TTS_talkDone  0x5900
extern void stateManage();

// ----------------------------------------------------------

void setup();
void loop();
// ---------------end of < WS_MAIN_H > -----------------------
#endif
