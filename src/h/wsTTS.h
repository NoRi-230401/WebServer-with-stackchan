// ---------------------------< wsTTS.h >------------------------------------
#ifndef WS_TTS_H
#define WS_TTS_H
// --------------------------------------------------------------------------
#include <Arduino.h>
#include <nvs.h>
#include <ArduinoJson.h>
#include <Avatar.h>
#include <AudioOutput.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>
#include <AudioFileSourcePROGMEM.h>
#include <M5Unified.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "../lib/AudioFileSourceHTTPSStream.h"
#include "../lib/WebVoiceVoxTTS.h"
#include "VoiceVox.h"

using namespace m5avatar;

// -- Request ---------
#define REQ_MSG_CLS 0
#define REQ_SPEAK_ADJUST 1
#define REQ_BALOON_ADJUST 2
#define REQ_SPEAK_BALOON_ADJUST 3
#define REQ_SPEAK 4
#define REQ_BALOON 5
#define REQ_SV_MD_ADJUST 9

extern const char *SETTING_NVS;
// -----------------------
// extern void sendReq(int reqNo,String msg);
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
bool isTalking();
void execute_talk(String url);
String execute_voicevox(const String &text, uint8_t spk_no);
#define TTS_VSPKNO_MAX 66
#define TTS_VSPKNO_INIT 3
void setTTSvSpkNo(int spkNo);
uint8_t getTTSvSpkNofmNVS();
// --------------------------------------------------------------------------

#endif
