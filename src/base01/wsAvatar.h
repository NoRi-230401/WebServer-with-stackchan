// ----------------------------<wsAvatar.cpp>------------------------------------
#ifndef WS_AVATAR_H
#define WS_AVATAR_H
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

extern const String EXPRESSION_STRING[];
extern AudioOutputM5Speaker out;
extern bool SV_USE;
extern int SV_MD;
extern int SV_HOME_X;
extern int SV_HOME_Y;
extern void servo2(int mode);
extern void sv_setEaseToX(int x);
extern void sv_setEaseToY(int y);
extern String webpage;
extern String getDateTime();
extern String SERVER_NAME;
extern size_t VOLUME_VALUE;
extern String TTS_SPEAKER_NO;
extern int CHARA_NO;
extern String getHeapFreeSize();
// -----------------------------------
void wsHandleFace(String expression);
void statusLineNext();
void statusLinePrev();
void statusLineOnOff();
void StatusLineManage();
void statusLineCheckManage();
// void statusLineOneManage();
void setStatusLineMode(int mode);
void set_avatar_color();
void servo(void *args);
bool setFace(int expr);
void lipSync(void *args);
void avatarSTART();
// -----------------------------------
#endif
