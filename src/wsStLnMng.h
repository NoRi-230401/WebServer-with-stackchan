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
extern String getDateTime(int &tmSec);
extern String SERVER_NAME;
extern uint8_t VOLUME_VALUE;
extern uint8_t TTS_vSpkNo;
extern int CHARA_NO;
extern String getHeapFreeSize();
extern const uint16_t rLED[];
extern const uint16_t lLED[];

extern void setLedColor4(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
extern void led_show();
extern void led_clear();

// -----------------------------------
void statusLineNext();
void statusLinePrev();
void statusLineOnOff();
void StatusLineManage();
void setStatusLineMode(int mode);
// ---------------------------------------------------------------------
#endif


