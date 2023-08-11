// ----------------------------<wsTimer.h>------------------------------------
#ifndef WS_TIMER_H
#define WS_TIMER_H
// ---------------------------------------------------------------------------
#include <Arduino.h>
#include <Avatar.h>
#include <M5Unified.h>
#include <Adafruit_NeoPixel.h>
#include <nvs.h>

#define NUM_LEDS 10                // LEDの数を指定する
#define TIMER_INIT 180          // タイマー初期値：３分
#define TIMER_MIN 30            // 最小タイマー設定値：３０秒
#define TIMER_MAX (60 * 60 - 1) // 最大タイマー設定値：６０分未満 (59分59秒)

extern String TTS2_PARMS;

extern const char tts_parms2[];
extern const char tts_parms6[];
using namespace m5avatar;
extern Avatar avatar;
extern bool SYSINFO_DISP_STATE;
extern void sysInfoDispEnd();
extern void led_setColor2(uint16_t n, uint32_t c);
extern void led_setColor4(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
extern uint32_t led_ColorLED3(uint8_t r, uint8_t g, uint8_t b);
extern void led_show();
extern void led_clear();
void ttsDo( const String& text );
extern String webpage;
extern void randomSpeakStop2();
extern String SPEECH_TEXT;
extern String SPEECH_TEXT_BUFFER;

// ---------------------------------------------------------------------------
void wsHandleTimer(String TmSecS, String TmMinS, String timerModeS);
void TimerManage();
void timerStop2(); 
void timerStart();
void timerStop();
void timerStarted();
void timerEnd();
// ---------------------------------------------------------------------------
#endif
