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

// --- status Mode  ----
#define STM0X_NONE 0
#define STM1X_LINE 1
#define STM2X_SYSINFO 2
#define STM3X_SETTING 3
#define STM_LEN 4
extern int statusMode;

//--- statusLine Mode define ----
#define STM10_ICON 0
#define STM11_NUM 1
#define STM12_CLOCK 2
#define STM13_RSSI 3
#define STM14_VOL 4
#define STM15_MEM 5
#define STM16_IP 6
#define STM1X_LINE_LEN 7
#define DURATION_1013 1013UL // 1.013秒: statusLineCheck_time

extern int SYSINFO_NO;
#define STM2X_SYSINFO_LEN 7

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
extern void avatarStop();
extern void avatarResume();
extern const String WSS_NAME;

// -----------------------------------
void SDU_disp();
void statusLineSetup();
void statusNext();
void statusPrev();
void statusLineManage();
void setStatusLineMode(int mode);
// ---------------------------------------------------------------------
#endif


