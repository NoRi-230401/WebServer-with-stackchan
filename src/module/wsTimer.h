// ----------------------------<wsTimer.h>------------------------------------
#ifndef _WS_TIMER_H
#define _WS_TIMER_H
// ---------------------------------------------------------------------------
#include <Arduino.h>
#include <M5Unified.h>
#include <nvs.h>

#define TM_INIT 180          // タイマー初期値：３分
#define TM_MIN 30            // 最小タイマー設定値：３０秒
#define TM_MAX (60 * 60 - 1) // 最大タイマー設定値：６０分未満 (59分59秒)

//-- Avatar expressions --
#define EXPR_NEUTRAL 0
#define EXPR_HAPPY 1
#define EXPR_SLEEPY 2
#define EXPR_DOUBT 3
#define EXPR_SAD 4
#define EXPR_ANGRY 5

// --- status Mode ----
#define STM_NONE 0  // nomal
#define STM_LINE 1  // statusLine
#define STM_SYSINFO 2 // sysInfo
#define STM_LEN 3
extern int statusMode;

// extern bool SYSINFO_DISP_STATE;
extern void sysInfoDispEnd();
extern void stackchanReq(const String& speakStr, int expr=-1, const String balloonStr="$$SKIP$$", int afterExpr=-1 );
extern String webpage;
extern void randomSpeakStop2();
extern bool isTalking();
extern void ledMoveSec(uint16_t sec);
void ledClearAll();
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
