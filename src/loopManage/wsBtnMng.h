// ----------------------------<wsBtnMng.h>------------------------------------
#ifndef _WS_BTNMNG_H
#define _WS_BTNMNG_H
// ---------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <M5Unified.h>
#include <AudioGeneratorMP3.h>
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
extern bool SYSINFO_DISP_STATE;
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
extern void sysInfoDispEnd();
extern void sysInfoDispStart(uint8_t mode_no);
extern void tone(int mode);
extern void SST_ChatGPT();
extern bool TM_STOP_GET;
extern bool TM_GO_GET;
extern void statusLineOnOff();
extern void statusLinePrev();
extern void statusLineNext();
extern bool statusLineOnOffState;
extern bool statusLineOneState;
extern void statusModeSelect();

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
void BtnMA_Do();
void BtnMC_Do();
void BoxTouchSetup();
// --------------------------------------------------------

#endif
