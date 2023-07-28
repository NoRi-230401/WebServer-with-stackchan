// ---------------------------< wsTts.h >------------------------------------
#ifndef WS_TTS_H
#define WS_TTS_H
// ---------------------------
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
// #include "../lib/AudioFileSourceVoiceTextStream.h"
#include "../lib/WebVoiceVoxTTS.h"
#include "../lib/AudioOutputM5Speaker.h"
// #include "../lib/rootCAgoogle.h"
// #include <google-tts.h>

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
extern void errSTOP(const String msg);
extern String webpage;
// extern bool REQ_chatGPT_GET;
extern int REQ_SPK_EXPR;
extern int REQ_SPK_PARMS_NO;
extern int REQ_SHUTDOWN_REBOOT;
extern int REQUEST_GET;
extern String REQ_MSG;


void wsHandleSpeech(String sayS, String expressionS, String voiceS);
void SpeechManage();
void SpeechText1st();
void SpeechTextNext();
// void ttsDo(char *text, char *tts_parms);
void ttsDo(char *text, const char *tts_parms);
bool isJP();
void VoiceText_tts(char *text, char *tts_parms);
void addPeriodBeforeKeyword(String &input, const String keywords[], int numKeywords);
// void addPeriodBeforeKeyword(String &input, String keywords[], int numKeywords);
void ttsSetup();
void getExpression(String &sentence, int &expressionIndx);
int search_separator(String text, int tbl);
// void google_tts(char *text, char *lang);


#endif
