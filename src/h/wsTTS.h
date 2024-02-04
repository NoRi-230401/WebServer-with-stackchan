// ---------------------------< wsTTS.h >------------------------------------
#ifndef _WS_TTS_H
#define _WS_TTS_H
// --------------------------------------------------------------------------
#include <Arduino.h>
#include <nvs.h>
#include "wsVoiceVox.h"

using namespace m5avatar;
extern const char *SETTING_NVS;
// -----------------------
extern Avatar avatar;
extern HTTPClient http;
extern WiFiClient client;
extern const Expression expr_table[];
extern void errSTOP();
extern String webpage;
extern int REQ_SHUTDOWN_REBOOT;
extern int REQUEST_NO;
extern String REQ_MSG;
extern void log_free_size(const char *text);
extern void stackchanReq(const String& speakStr, int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1 );
extern const String EXPR_STR[];

// --- Wss StaTe (WST) define ------- 
extern uint16_t WST ;
// --- Wss StaTe (WST) define ------- 
#define WST_INIT          0x0000
#define WST_SETUP_start   0x1000
#define WST_SETUP_done    0x1900
#define WST_LOOP          0x2000
#define WST_STT           0x3000
#define WST_chatGPT       0x4000
#define WST_TTS_start     0x5000
#define WST_TTS_mp3urlGet 0x5100
#define WST_TTS_talkStart 0x5200
#define WST_TTS_talking   0x5300
#define WST_TTS_exit      0x5800
#define WST_TTS_talkDone  0x5900

#define EXE_TM_MD0 0      // (default) disp and timer reset
#define EXE_TM_MD1 1      // disp and timer no reset
#define EXE_TM_MD2 2      // no disp and timer reset
#define EXE_TM_MD_START 3 // no disp and timer reset for START 
extern void showExeTime(String msg, int mode = EXE_TM_MD0);

//------------------------------------------------------------
void wsHandleSpeech(String sayS, String expressionS, String balloonS, String voiceS, String afterExpS);
void ttsDo( const String& speechText );
// String execute_voiceText(const String &speechText, uint8_t spk_no);
// void ttsDo2(const String &speechText);
bool isTalking();
// void execute_talk(String url);
String execute_voicevox(const String &text, uint8_t spk_no);
#define TTS_VSPKNO_MAX 66
#define TTS_VSPKNO_INIT 3
void setTTSvSpkNo(int spkNo);
uint8_t getTTSvSpkNofmNVS();
// --------------------------------------------------------------------------

#endif
