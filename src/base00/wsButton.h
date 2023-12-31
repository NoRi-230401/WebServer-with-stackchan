// ----------------------------<wsButton.h>------------------------------------
#ifndef WS_BUTTON_H
#define WS_BUTTON_H
// ---------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <M5Unified.h>
#include <AudioGeneratorMP3.h>
#define BtnREQ_A 1
#define BtnREQ_B 2
#define BtnREQ_C 3
#define BtnREQ_BOX_SERVO 4
#define BtnREQ_BOX_STT 5

extern AudioGeneratorMP3 *mp3;
extern bool SYSINFO_DISP_STATE;
extern String webpage;
extern bool TIMER_STARTED;
extern bool RANDOM_SPEAK_STATE;
extern bool RANDOM_SPEAK_ON_GET;
extern bool RANDOM_SPEAK_OFF_GET;

extern String BoxServoDo();
extern void timerStop2(); 
extern void randomSpeakStop2();
extern void report_batt_level();
extern void timerStart();
extern void timerStop();
extern void sysInfoDispEnd();
extern void tone(int mode);
extern void SST_ChatGPT();
extern bool TIMER_STOP_GET;
extern bool TIMER_GO_GET;
extern void batteryIconSelect();
extern void batteryIconOnOff();
// --------------------------------------------------------
void wsHandleBtn(String arg);
void BtnReqGet();
void ButtonManage();
void BtnA_Do();
void BtnB_Do();
void BtnC_Do();
void BoxSttDo();
void BoxTouchSetup();
void BoxBatteryIconDoOnOff();
void BoxBatteryIconDoSelect();
// --------------------------------------------------------
#endif
