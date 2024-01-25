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
extern int REQ_AVATAR_EXPR;
extern int REQ_SHUTDOWN_REBOOT;
extern int REQUEST_NO;
extern String REQ_MSG;
extern void log_free_size(const char *text);
extern void stackchan(const String& speakStr, int expr=-1, const String balloonStr="$$SKIP$$");

//------------------------------------------------------------
// void wsHandleSpeech(String sayS, String expressionS, String voiceS);
void wsHandleSpeech(String sayS, String expressionS, String balloonS, String voiceS);
void ttsDo( const String& speechText );
void setSpeaker(uint8_t spkNo);
bool isTalking();
void execute_talk(String url);
String execute_voicevox(const String &text, uint8_t spk_no);
#define TTS_VSPKNO_MAX 66
#define TTS_VSPKNO_INIT 3
void setTTSvSpkNo(int spkNo);
uint8_t getTTSvSpkNofmNVS();
// --------------------------------------------------------------------------

#endif
