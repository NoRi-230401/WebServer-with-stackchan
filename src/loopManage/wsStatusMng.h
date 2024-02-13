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

//--- statusLine Mode define ----
#define STATUS_MD_ICON 0
#define STATUS_MD_NUM 1
#define STATUS_MD_CLOCK 2
#define STATUS_MD_RSSI 3
#define STATUS_MD_VOL 4
#define STATUS_MD_MEM 5
#define STATUS_MD_IP 6
#define STATUS_MD_LEN 7
#define DURATION_1013 1013UL // 1.013秒: statusLineCheck_time

// --- status Mode  ----
#define STM_NONE 0
#define STM_LINE 1
#define STM_SYSINFO 2
#define STM_LEN 3

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
extern void sysInfoDispStart(uint8_t mode_no);
extern void sysInfoDispEnd();

extern int SYSINFO_NO;
#define SYSINFO_LEN 7

// -----------------------------------
void statusLineSetup();
void statusNext();
void statusPrev();
void statusLineOnOff();
void statusLineManage();
void setStatusLineMode(int mode);
// ---------------------------------------------------------------------
#endif


