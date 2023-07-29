// ---------------------------< wsTts.h >------------------------------------
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
#include "../lib/AudioOutputM5Speaker.h"

#define SV_REQ_MSG_CLS 0
#define SV_REQ_SPEAK 1
#define SV_REQ_MSG 2
#define SV_REQ_SPEAK_MSG 3
#define REQ_SPEAK_MSG2 4
#define SV_REQ_MD_ADJUST 9

using namespace m5avatar;
extern Avatar avatar;
extern HTTPClient http;
extern WiFiClient client;
extern const Expression expressions_table[];
// extern void errSTOP(const String msg);
extern void errSTOP();

extern String webpage;
extern int REQ_SPK_EXPR;
extern int REQ_SPK_PARMS_NO;
extern int REQ_SHUTDOWN_REBOOT;
extern int REQUEST_GET;
extern String REQ_MSG;

void wsHandleSpeech(String sayS, String expressionS, String voiceS);
void SpeechManage();
void SpeechText1st();
void SpeechTextNext();
void ttsDo( const String& speechText );
bool isJP();
void ttsSetup();

// --------------------------------------------------------------------------
#endif
