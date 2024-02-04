// -----------------------<wsStLnMng.h>---------------------------------
#ifndef _WS_STLNMNG_H
#define _WS_STLNMNG_H
// ---------------------------
#include <WiFi.h>  
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Avatar.h>
#include <SD.h>
#include <nvs.h>
#include <M5Unified.h>
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
extern void ledSetColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
extern void ledShow();
extern void ledClear();

// -----------------------------------
void statusLineSetup();
void statusLineNext();
void statusLinePrev();
void statusLineOnOff();
void statusLineManage();
void setStatusLineMode(int mode);
// ---------------------------------------------------------------------
#endif


