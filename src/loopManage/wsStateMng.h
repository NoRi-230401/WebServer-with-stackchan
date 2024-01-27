// -----------------------<wsStateMng.h>---------------------------------
#ifndef WS_STATEMNG_H
#define WS_STATEMNG_H
// ---------------------------
#include <WiFi.h>  
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Avatar.h>
using namespace m5avatar;
extern Avatar avatar;

extern void log_free_size(const char *text);
extern String getHeapFreeSize();
extern void log_free_size(const char *text);

// --- showExeTime ------- 
#define EXE_TM_MD0 0 // (default) disp and timer reset
#define EXE_TM_MD1 1 // disp and timer no reset
#define EXE_TM_MD2 2 // no disp and timer reset
#define EXE_TM_MD_START 3 // no disp and timer reset for START 
extern void showExeTime(String msg, int mode = EXE_TM_MD0);
extern const Expression expr_table[];
// --- Wss StaTe (WST) define ------- 
#define WST_INIT            0x0000
#define WST_SETUP_start     0x1000
#define WST_SETUP_done      0x1900
#define WST_LOOP            0x2000

#define WST_STT             0x3000

#define WST_chatGPT_start   0x4000
#define WST_chatGPT_reqGet  0x4200
#define WST_chatGPT_exit    0x4800
#define WST_chatGPT_done    0x4900

#define WST_TTS_start       0x5000
#define WST_TTS_mp3urlGet   0x5100
#define WST_TTS_talkStart   0x5200
#define WST_TTS_talking     0x5300
#define WST_TTS_exit        0x5800
#define WST_TTS_talkDone    0x5900


extern int REQ_EXPR_AFTER;
extern void setAvatarExpr(int expr);
// -----------------------------------------------------
void stateManage();


// -----------------------------------------------------
#endif


