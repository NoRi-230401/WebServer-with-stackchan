// -----------------------<wsChatGPT.h>---------------------------------
#ifndef _WS_CHATGPT_H
#define _WS_CHATGPT_H
// ---------------------------------------------------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <deque>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <nvs.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "../lib/rootCACertificate.h"

#define DOC_SIZE 1024 * 4
#define CHARA_SIZE 9 * 1024
#define FLTYPE_SPIFFS 1
#define FLTYPE_SD 2
extern bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String readFile);
extern bool jsonDocSave(DynamicJsonDocument &jsonDoc, String saveFile);

// extern bool REQ_chatGPT_GET;
extern String REQ_MSG;
// extern bool SYSINFO_DISP_STATE;
extern char tts_parms2[];
extern bool KEYLOCK_STATE;
extern String VOICEVOX_API_KEY;
extern String OPENAI_API_KEY;
extern uint8_t TTS_vSpkNo;
extern bool LED_OnOff_STATE;
extern void ledSetColor2(uint16_t n, uint32_t c);
extern void ledSetColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
extern uint32_t ledGetColorNo(uint8_t r, uint8_t g, uint8_t b);
extern void ledShow();
extern void ledClear();
extern void ledClearAll();
extern void timerStop2(); 
extern void REBOOT();
extern const String random_words[];
extern const String chatStrIData;
extern const char *SETTING_NVS; // setting --NVS の設定用ファイル
extern String webpage;
extern int TTS_PARMS_NO;
// extern void sysInfoDispEnd();
extern bool isTalking();
// extern void sendReq(int reqNo,String msg);
extern void log_free_size(const char *text);
// --- status Mode ----
#define STM_NONE 0  // nomal
#define STM_LINE 1  // statusLine
#define STM_SYSINFO 2 // sysInfo
#define STM_LEN 3
extern int statusMode;

// --- exe timer define ------- 
#define EXE_TM_MD0 0 // (default) disp and timer reset
#define EXE_TM_MD1 1 // disp and timer no reset
#define EXE_TM_MD2 2 // no disp and timer reset
#define EXE_TM_MD_START 3 // no disp and timer reset for START 
extern void showExeTime(String msg, int mode = EXE_TM_MD0);
extern void stackchanReq(const String& speakStr, int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1 );
extern void stackchanNow( int expr = -1, const String balloonStr = "$$SKIP$$");

//-- Avatar expressions --
#define EXPR_NEUTRAL 0
#define EXPR_HAPPY 1
#define EXPR_SLEEPY 2
#define EXPR_DOUBT 3
#define EXPR_SAD 4
#define EXPR_ANGRY 5

// --- Wss StaTe (WST) define ------- 
extern uint16_t WST ;
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
#define WST_TTS_ttsExit     0x5800
#define WST_TTS_talkDone    0x5900

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
bool setChatDoc(const String& data);

String https_post_json(const char *url, const char *json_string, const char *root_ca);
String chatGpt(String json_string);
void exec_chatGPT(String text);
bool saveChatDoc();
void chatGptSetup();
void chatHistoryCls();
// ---- -------------end of < WS_CHATGPT_H > --------------------------------------
#endif


