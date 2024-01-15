// -----------------------<wsChatGPT.h>---------------------------------
#ifndef WS_CHATGPT_H
#define WS_CHATGPT_H
// ---------------------------------------------------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Avatar.h>
// #include <AudioGeneratorMP3.h>
// #include <AudioOutput.h>
#include <deque>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <nvs.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
// #include "../lib/AudioOutputM5Speaker.h"
#include "../lib/rootCACertificate.h"

// -- Request ---------
#define REQ_MSG_CLS 0
#define REQ_SPEAK_ADJUST 1
#define REQ_BALOON_ADJUST 2
#define REQ_SPEAK_BALOON_ADJUST 3
#define REQ_SPEAK 4
#define REQ_BALOON 5
#define REQ_SV_MD_ADJUST 9
// -----------------------

#define DOC_SIZE 1024 * 4

#define CHARA_SIZE 9 * 1024
#define FLTYPE_SPIFFS 1
#define FLTYPE_SD 2
extern bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String readFile);
extern bool jsonSave(DynamicJsonDocument &jsonDoc, String saveFile);

using namespace m5avatar;
extern Avatar avatar;
extern const Expression expressions_table[];
extern bool REQ_chatGPT_GET;
extern String REQ_MSG;
extern bool SYSINFO_DISP_STATE;
extern char tts_parms2[];
extern bool KEYLOCK_STATE;
extern String VOICEVOX_API_KEY;
extern String OPENAI_API_KEY;
extern String SPEECH_TEXT;
extern String SPEECH_TEXT_BUFFER;
extern String TTS_SPEAKER_NO;
extern String TTS_SPEAKER;
extern String TTS_PARMS;
// extern AudioGeneratorMP3 *mp3;
extern bool LED_OnOff_STATE;
extern void led_setColor2(uint16_t n, uint32_t c);
extern void led_setColor4(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
extern uint32_t led_ColorLED3(uint8_t r, uint8_t g, uint8_t b);
extern void led_show();
extern void led_clear();
extern void led_allOff();
extern void timerStop2(); 
extern void REBOOT();
extern const String random_words[];
extern const String json_ChatString;
extern const char *SETTING_NVS; // setting --NVS の設定用ファイル
// extern void ttsDo( const String& text );
extern String webpage;
extern int TTS_PARMS_NO;
extern void sysInfoDispEnd();
// extern void ReqSpkBaloon_adjust(String spkMsg);
// extern void ReqSpkOnly(String spkMsg);
extern bool isTalking();
extern void sendReq(int reqNo,String msg);
// extern void SpeechText1st();
// -----------------------------------------------------------------------------
void chatGptManage();
void wsHandleRandomSpeak(String modeS);
void wsHandelChat(String textS, String voiceS);
void wsHandelChatGpt(String historyS,String charaS);
void wsHandelChatCharacter(String ch_NoS,String ch_nameS, String ch_voiceS,String ch_rollS);
bool initCharaJson(DynamicJsonDocument &charaJson);
void wsHandleRoleSet(String roleS);
void wsHandleRoleGet();
bool chatDocInit();
void randomSpeakStop2();
void randomSpeak(bool mode);
bool init_chat_doc(const char *data);
String https_post_json(const char *url, const char *json_string, const char *root_ca);
String chatGpt(String json_string);
void exec_chatGPT(String text);
bool save_json();
void chatGptSetup();
void chatHistoryCls();
// ---- -------------end of < WS_CHATGPT_H > --------------------------------------
#endif


