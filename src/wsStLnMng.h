// -----------------------<wsStLnMng.h>---------------------------------
#ifndef WS_STLNMNG_H
#define WS_STLNMNG_H
// ---------------------------
#include <WiFi.h>  
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Avatar.h>
// #include <AudioOutput.h>
// #include <deque>
// #include <SPIFFS.h>
#include <SD.h>
#include <nvs.h>
#include <M5Unified.h>
// #define SUPPRESS_HPP_WARNING
// #include <ServoEasing.h>
// #include <AudioGeneratorMP3.h>
// #include "lib/AudioOutputM5Speaker.h"
using namespace m5avatar;
extern Avatar avatar;
extern String getDateTime();
extern String SERVER_NAME;
extern uint8_t VOLUME_VALUE;
extern uint8_t TTS_vSpkNo;
extern int CHARA_NO;
extern String getHeapFreeSize();

// -----------------------------------
void statusLineNext();
void statusLinePrev();
void statusLineOnOff();
void StatusLineManage();
void setStatusLineMode(int mode);
// ---------------------------------------------------------------------
#endif


