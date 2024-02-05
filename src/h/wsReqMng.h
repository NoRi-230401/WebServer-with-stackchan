// ---------------------------< wsReqMng.h >----------------------------
#ifndef _WS_REQMNG_H
#define _WS_REQMNG_H
// ---------------------------------------------------------------------
#include <Arduino.h>
// #include <Avatar.h>
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
#define REQ_SV_MD_ADJUST 9
#define REQ_STACKCHAN 10
#define REQ_REBOOT  98
#define REQ_SHUTDOWN  99

// -------------------------

// using namespace m5avatar;
// extern Avatar avatar;
// extern const Expression expr_table[];
// extern char *tts_parms_table[];
// extern char tts_parms2[];
extern void ttsDo( const String& text );
extern bool SV_ADJUST_STATE;
extern int SV_MD;
extern uint16_t SHUTDOWN_TM_SEC;
extern void REBOOT();
extern void POWER_OFF();
extern bool isTalking();
extern String SPEECH_TEXT;
extern void setAvatarExpr(int expr);
extern void setAvatarBalloon(String msg);
extern void clearAvatarBalloon();
extern void stackchanReq(const String& speakStr, int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1 );
extern void stackchanNow( int expr = -1, const String balloonStr = "$$SKIP$$");

// ---------------------------------------------------------------------
// extern void sendReq2(int reqNo, const String& speakStr="", int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1);
// void sendReq2(int reqNo, const String& speakStr, int expr, const String balloonStr, int afterExpr);
extern void sendReq_stackchan(const String& speakStr="", int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1);
void sendReq_stackchan(const String &speakStr, int expr, const String balloonStr, int afterExpr);
void requestManage();
void sendReq(int reqNo);
void Req_stackchanDo();
// ---------------------------------------------------------------------
#endif
