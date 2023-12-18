// ---------------------------< wsReq.h >------------------------------------
#ifndef WS_REQ_H
#define WS_REQ_H
// ---------------------------------------------------------------------
#include <Arduino.h>
#include <Avatar.h>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>

// --- servo ----------
#define SV_MD_MOVING 0
#define SV_MD_HOME 1
#define SV_MD_RANDOM 2
#define SV_MD_CENTER 3
#define SV_MD_SWING 4
#define SV_MD_STOP 5
#define SV_MD_ADJUST 6
#define SV_MD_POINT 7
#define SV_MD_DELTA 8
#define SV_MD_NONE 99
// -- Request ---------
#define REQ_MSG_CLS 0
#define REQ_SPEAK 1
#define REQ_MSG_ONLY 2
#define REQ_SPEAK_MSG 3
#define REQ_SPEAK_MSG2 4
#define REQ_SV_MD_ADJUST 9

extern String TTS2_PARMS;
using namespace m5avatar;
extern Avatar avatar;
extern const Expression expressions_table[];
extern char *tts_parms_table[];
extern char tts_parms2[];
extern void ttsDo( const String& text );
extern bool SV_ADJUST_STATE;
extern int SV_MD;
extern uint16_t SHUTDOWN_TM_SEC;
extern void REBOOT();
extern void POWER_OFF();

// ---------------------------------------------------------------------
void RequestManage();
void Req_SpkMsgDo2();
// void ReqSpkMsg();
void ReqSpkMsg(String spkMsg);
void ReqSpkMsg2(String spkMsg);

void Req_SpkMsgDo(); 
void Req_SpkDo();
void Req_MsgOnlyDo();
void Req_MsgOnly();
void Req_MsgCls();
// ---------------------------------------------------------------------
#endif
