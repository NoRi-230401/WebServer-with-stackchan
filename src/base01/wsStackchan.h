// ----------------------------<wsStackchan.h>------------------------------------
#ifndef WS_STACKCHAN_H
#define WS_STACKCHAN_H
// ---------------------------
#include <WiFi.h>  
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Avatar.h>
#include <AudioOutput.h>
#include <deque>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <nvs.h>
#define SUPPRESS_HPP_WARNING
#include <ServoEasing.h>
#include <AudioGeneratorMP3.h>
#include "lib/AudioOutputM5Speaker.h"

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
extern AudioOutputM5Speaker out;
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

#define STATUS_MD_ICON 0
#define STATUS_MD_NUM 1
#define STATUS_MD_CLOCK 2
#define STATUS_MD_RSSI 3
#define STATUS_MD_VOL 4
#define STATUS_MD_MEM 5
#define STATUS_MD_IP 6
#define STATUS_MD_MAX 7
extern  bool statusLineOnOffState;
extern int StatusLineMode;

#define EXPR_NEUTRAL 0
#define EXPR_HAPPY 1
#define EXPR_SLEEPY 2
#define EXPR_DOUBT 3
#define EXPR_SAD 4
#define EXPR_ANGRY 5

// -- Request ---------
#define REQ_MSG_CLS 0
#define REQ_SPEAK_ADJUST 1
#define REQ_BALOON_ADJUST 2
#define REQ_SPEAK_BALOON_ADJUST 3
#define REQ_SPEAK 4
#define REQ_BALOON 5
#define REQ_SV_MD_ADJUST 9
#define REQ_STACKCHAN 10
// -------------------------


extern void sendReq2(int reqNo, const String& speakStr="", int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1);
// void sendReq2(int reqNo, const String& speakStr, int expr, const String balloonStr, int afterExpr);

extern void stackchan(const String& speakStr, int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1 );
void stackchan(const String& speakStr,int expr, const String balloonStr, int afterExpr);


// -----------------------------------
void wsHandleFace(String expression);
void wsHandleBalloon(String text);
void setAvatarExpr(int expr);
void setAvatarBalloon(String msg);
void clearAvatarBalloon();
void statusLineNext();
void statusLinePrev();
void statusLineOnOff();
void statusLineManage();
void setStatusLineMode(int mode);
void setAvatarcolor();
void servo(void *args);

void avatarSTART();
// -----------------------------------
#endif
