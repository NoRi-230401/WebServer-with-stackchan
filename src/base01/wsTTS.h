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
extern void sendReq(int reqNo,String msg);
extern Avatar avatar;
extern HTTPClient http;
extern WiFiClient client;
extern const Expression expr_table[];
extern void errSTOP();
extern String webpage;
// extern int REQ_AVATAR_EXPR;
extern int REQ_SHUTDOWN_REBOOT;
extern int REQUEST_NO;
extern String REQ_MSG;
extern void log_free_size(const char *text);
// extern void stackchan(const String& speakStr, int expr=-1, const String balloonStr="$$SKIP$$");
extern void stackchan(const String& speakStr, int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1 );
extern const String EXPR_STR[];

// --- Wss StaTe (WST) define ------- 
extern uint16_t WST ;
#define WST_setupStart   0x1000
#define WST_setupDone    0x1900
#define WST_loop         0x2000
#define WST_stt          0x3000
#define WST_chatGPT      0x4000
#define WST_ttsStart     0x5000
#define WST_mp3UrlGet    0x5100
#define WST_talkStart    0x5200
#define WST_ttsExit      0x5800
#define WST_talkDone     0x5900

//------------------------------------------------------------
// void wsHandleSpeech(String sayS, String expressionS, String voiceS);
void wsHandleSpeech(String sayS, String expressionS, String balloonS, String voiceS, String afterExpS);
void ttsDo( const String& speechText );
// void setSpeaker(uint8_t spkNo);
// void setAfterExpr(int expr);
bool isTalking();
void execute_talk(String url);
String execute_voicevox(const String &text, uint8_t spk_no);
#define TTS_VSPKNO_MAX 66
#define TTS_VSPKNO_INIT 3
void setTTSvSpkNo(int spkNo);
uint8_t getTTSvSpkNofmNVS();
// --------------------------------------------------------------------------

#endif
