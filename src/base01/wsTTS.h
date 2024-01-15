// ---------------------------< wsTTS.h >------------------------------------
#ifndef WS_TTS_H
#define WS_TTS_H
// --------------------------------------------------------------------------
#include <Arduino.h>
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
// #include "../lib/AudioOutputM5Speaker.h"

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
extern void sendReq(int reqNo,String msg);
// -----------------------



extern Avatar avatar;
extern HTTPClient http;
extern WiFiClient client;
extern const Expression expressions_table[];
extern void errSTOP();
extern String webpage;
extern int REQ_SPK_EXPR;
// extern int REQ_SPK_PARMS_NO;
extern int REQ_SHUTDOWN_REBOOT;
extern int REQUEST_GET;
extern String REQ_MSG;
extern void log_free_size(const char *text);

//------------------------------------------------------------
void wsHandleSpeech(String sayS, String expressionS, String voiceS);
void ttsDo( const String& speechText );

void setSpeaker(uint8_t spkNo);

bool isTalking();
void execute_talk(String url);
// String execute_voicevox(String text, uint8_t spk_no);
String execute_voicevox(const String &text, uint8_t spk_no);
// --------------------------------------------------------------------------

#endif
