// ----------------------------<wsBtnMng.h>------------------------------------
#ifndef _WS_BTNMNG_H
#define _WS_BTNMNG_H
// ---------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <M5Unified.h>
#include <AudioGeneratorMP3.h>
// #include <ESP32-targz.h>
// #include <M5StackUpdater.h>
// #define TFCARD_CS_PIN 4
// #define TIMEOUT00 10000       // lobby countdouwn timeout : msec
// #define TIMEOUT01 20000       // lobby countdouwn timeout : msec

#define BtnREQ_A 1
#define BtnREQ_B 2
#define BtnREQ_C 3
#define BtnREQ_MA 4
#define BtnREQ_MB 5
#define BtnREQ_MC 6
#define BtnREQ_UA 7
#define BtnREQ_UB 8
#define BtnREQ_UC 9

extern AudioGeneratorMP3 *mp3;
extern String webpage;
extern bool TM_STARTED;
extern bool RANDOM_SPEAK_STATE;
extern bool RANDOM_SPEAK_ON_GET;
extern bool RANDOM_SPEAK_OFF_GET;

extern void BoxServoDo();
extern void timerStop2(); 
extern void randomSpeakStop2();
extern void timerStart();
extern void timerStop();
extern void sysInfoDispStart(uint8_t mode_no);
extern void tone(int mode);
extern void SST_ChatGPT();
extern bool TM_STOP_GET;
extern bool TM_GO_GET;
extern void statusPrev();
extern void statusNext();
extern bool statusLineOneState;
extern void statusModeSelect();

extern const String WSS_NAME;
extern const String WSS_SD_BIN;
extern void SDU_lobby();
extern void SDU_fromSD();
extern void SDU_saveBin(String flname);

// --- status Mode  ----
#define STM0X_NONE 0
#define STM1X_LINE 1
#define STM2X_SYSINFO 2
#define STM3X_SETTING 3
#define STM_LEN 4
extern int statusMode;

extern void avatarStop();
extern void avatarStop2();


// --------------------------------------------------------
void wsHandleBtn(String arg);
void BtnReqGet();
void buttonManage();
void BtnA_Do();
void BtnB_Do();
void BtnC_Do();
void BtnUA_Do();
void BtnUB_Do();
void BtnUC_Do();
void BtnMB_Do();
void BoxTouchSetup();
void SDU_disp();

// --------------------------------------------------------

#endif
