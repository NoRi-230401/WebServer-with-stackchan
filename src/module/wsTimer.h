// ----------------------------<wsTimer.h>------------------------------------
#ifndef WS_TIMER_H
#define WS_TIMER_H
// ---------------------------------------------------------------------------
#include <Arduino.h>
#include <Avatar.h>
#include <M5Unified.h>
#include <Adafruit_NeoPixel.h>
#include <nvs.h>

#define NUM_LEDS 10             // LEDの数を指定する
#define TM_INIT 180          // タイマー初期値：３分
#define TM_MIN 30            // 最小タイマー設定値：３０秒
#define TM_MAX (60 * 60 - 1) // 最大タイマー設定値：６０分未満 (59分59秒)

// -- Request ---------
#define REQ_MSG_CLS 0
#define REQ_SPEAK_ADJUST 1
#define REQ_BALOON_ADJUST 2
#define REQ_SPEAK_BALOON_ADJUST 3
#define REQ_SPEAK 4
#define REQ_BALOON 5
#define REQ_SV_MD_ADJUST 9
extern void sendReq(int reqNo,String msg);
// -----------------------

using namespace m5avatar;
extern Avatar avatar;
extern bool SYSINFO_DISP_STATE;
extern void sysInfoDispEnd();

// extern void ledSetColor2(uint16_t n, uint32_t c);
// extern void ledSetColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
// extern uint32_t ledGetColorNo(uint8_t r, uint8_t g, uint8_t b);
// extern void ledShow();
// extern void ledClear();

extern String webpage;
extern void randomSpeakStop2();
extern bool isTalking();
// ---------------------------------------------------------------------------
void wsHandleTimer(String TmSecS, String TmMinS, String timerModeS);
void timerManage();
void timerStop2(); 
void timerStart();
void timerStop();
void timerStarted();
void timerEnd();
// ---------------------------------------------------------------------------
#endif
