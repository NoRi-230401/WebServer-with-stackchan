// ----------------------------<wsStackchan.h>------------------------------------
#ifndef _WS_STACKCHAN_H
#define _WS_STACKCHAN_H
// ---------------------------
#include <WiFi.h>  
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Avatar.h>
// #include <AudioOutput.h>
#include <deque>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <nvs.h>
#define SUPPRESS_HPP_WARNING
#include <ServoEasing.h>
// #include <AudioGeneratorMP3.h>
// #include "lib/AudioOutputM5Speaker.h"

// -- Servo Mode define ----
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

extern const String EXPR_STR[];
// extern AudioOutputM5Speaker out;
extern bool SV_USE;
extern int SV_MD;
extern int SV_HOME_X;
extern int SV_HOME_Y;
extern void servo2(int mode);
extern void sv_setEaseToX(int x);
extern void sv_setEaseToY(int y);
extern String webpage;
// extern String getDateTime();
extern String SERVER_NAME;
extern uint8_t VOLUME_VALUE;
extern uint8_t TTS_vSpkNo;
extern int CHARA_NO;
extern String getHeapFreeSize();

//-- statusLine Mode define --
// extern  bool statusLineOnOffState;
extern int StatusLineMode;
#define STATUS_MD_ICON 0
#define STATUS_MD_NUM 1
#define STATUS_MD_CLOCK 2
#define STATUS_MD_RSSI 3
#define STATUS_MD_VOL 4
#define STATUS_MD_MEM 5
#define STATUS_MD_IP 6
#define STATUS_MD_LEN 7

//-- Avatar expressions --
#define EXPR_NEUTRAL 0
#define EXPR_HAPPY 1
#define EXPR_SLEEPY 2
#define EXPR_DOUBT 3
#define EXPR_SAD 4
#define EXPR_ANGRY 5

// -- Request ---------
#define REQ_SV_MD_ADJUST 9
#define REQ_STACKCHAN 10
#define REQ_SDUPDATER_SAVE 21
#define REQ_SDUPDATER_SAVE2 22
#define REQ_REBOOT  98
#define REQ_SHUTDOWN  99

extern void sendReq_stackchan(const String& speakStr="", int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1);
extern void stackchanReq(const String& speakStr, int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1 );
void stackchanReq(const String& speakStr,int expr, const String balloonStr, int afterExpr);
extern void stackchanNow( int expr = -1, const String balloonStr = "$$SKIP$$");
void stackchanNow( int expr, const String balloonStr);

// -----------------------------------
void wsHandleFace(String expression);
void wsHandleBalloon(String text);
void setAvatarExpr(int expr);
void setAvatarBalloon(String msg);
void clearAvatarBalloon();
void statusNext();
void statusPrev();
void statusLineManage();
void setStatusLineMode(int mode);
void setAvatarcolor();
extern void servo(void *args);
void avatarStop();
void avatarStop2();
void avatarResume();
void avatarSTART();
// -----------------------------------
#endif
