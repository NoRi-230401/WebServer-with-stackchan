// ----------------------------<wsAvatar.cpp>------------------------------------
#ifndef WS_AVATAR_H
#define WS_AVATAR_H
// ---------------------------

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Avatar.h>
#include <AudioOutput.h>
#include <deque>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <nvs.h>

#define SUPPRESS_HPP_WARNING
#include <ServoEasing.h>
#include <AudioGeneratorMP3.h>
#include "lib/AudioOutputM5Speaker.h"


#define EX_TIMER_INIT 180          // タイマー初期値：３分
#define EX_TIMER_MIN 30            // 最小タイマー設定値：３０秒
#define EX_TIMER_MAX (60 * 60 - 1) // 最大タイマー設定値：６０分未満 (59分59秒)
#define EX_WK_CNT_MAX 10           // 「わかりません」が何回連続すると初期化するか
#define EX_SHUTDOWN_MIN_TM 3

// ---- servo define
#define SV_CENTER_X 90
#define SV_CENTER_Y 90
#define SV_PIN_X_CORE2_PA 33 // Core2 PORT A
#define SV_PIN_Y_CORE2_PA 32
#define SV_PIN_X_CORE2_PC 13 // Core2 PORT C
#define SV_PIN_Y_CORE2_PC 14

// -- Servo Mode define ----
#define SV_MD_MOVING 0
#define SV_MD_HOME 1
#define SV_MD_RANDOM 2
#define SV_MD_CENTER 3
// ------------------------
#define SV_MD_SWING 4
#define SV_MD_STOP 5
// ------------------------
#define SV_MD_ADJUST 6
#define SV_MD_POINT 7
#define SV_MD_DELTA 8
// ------------------------
#define SV_MD_NONE 99

#define REQ_MSG_CLS 0
#define REQ_SPEAK 1
#define REQ_MSG_ONLY 2
#define REQ_SPEAK_MSG 3
#define REQ_SPEAK_MSG2 4
#define SV_REQ_MD_ADJUST 9

#define SV_SWING_AXIS_X 0
#define SV_SWING_AXIS_Y 1
#define SV_SWING_AXIS_XY 2

#define SV_X_MIN 0
#define SV_X_MAX 180
#define SV_Y_MIN 50
#define SV_Y_MAX 100

#define EX_APIKEYJSON_SIZE 5 * 128
#define EX_STARTUPJSON_SIZE 15 * 128
#define EX_SERVOJSON_SIZE 5 * 128
#define EX_DOC_SIZE 1024 * 2

extern const String EXPRESSION_STRING[];

// --- exReq.cpp ---
extern void RequestManage();
extern void ReqSpkMsgDo2();
extern void servoReqSpkMsg();
extern void Req_SpkMsgDo(); 
extern void Req_SpkDo();
extern void Req_MsgOnlyDo();
extern void Req_MsgOnly();
extern void Req_MsgCls();

// --- exSetting.cpp ---
extern size_t VOLUME_VALUE;
extern bool MUTE_ON_STATE;
extern uint8_t TONE_MODE;
extern bool SYSINFO_DISP_STATE;
extern String SYSINFO_MSG;
extern String IP_ADDR;
extern String SSID;
extern String SSID_PASSWD;
extern String VOICETEXT_API_KEY;
extern bool KEYLOCK_STATE;
extern String VOICEVOX_API_KEY;
extern String OPENAI_API_KEY;

// --- exTts.cpp ---
extern char tts_parms1[];
extern char tts_parms2[];
extern char tts_parms3[];
extern char tts_parms4[];
extern char tts_parms5[];
extern char tts_parms6[];
extern char *tts_parms_table[];
extern uint8_t TTS_TYPE;
extern String LANG_CODE;
extern const char *TTS_NAME[];
extern const char *LANG_CODE_JP;
extern const char *LANG_CODE_EN;
extern int EXPRESSION_INDX;
extern String VoiceText_TTS_USER;
extern String SPEECH_TEXT;
extern String SPEECH_TEXT_BUFFER;
extern String TTS2_SPEAKER_NO;
extern String TTS2_SPEAKER;
extern String TTS2_PARMS;
extern String KEYWORDS[];
extern AudioOutputM5Speaker out;
extern AudioGeneratorMP3 *mp3;
extern uint8_t m5spk_virtual_channel;


// --exLed.cpp -------
extern bool LED_OnOff_STATE;
extern void led_setColor2(uint16_t n, uint32_t c);
extern void led_setColor4(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
extern uint32_t led_ColorLED3(uint8_t r, uint8_t g, uint8_t b);
extern void led_show();
extern void led_clear();
extern void led_allOff();

// -- exTimer.cpp --
extern uint16_t TIMER_SEC_VALUE;
extern bool TIMER_STARTED;
extern uint32_t TIMER_START_MILLIS;
extern uint16_t TIMER_ELEAPSE_SEC;
extern bool TIMER_STOP_GET;
extern bool TIMER_GO_GET;
extern void timerStop2(); 
extern void timerStart();
extern void timerStop();
extern void timerStarted();
extern void timerEnd();
extern void TimerManage();

// -- exServo.cpp --
extern ServoEasing servo_x;
extern ServoEasing servo_y;
extern bool SV_ADJUST_STATE;
extern bool SV_USE;
extern int SV_MD;
extern int SV_PT_X;
extern int SV_PT_Y;
extern int SV_HOME_X;
extern int SV_HOME_Y;
extern bool SV_HOME_STATE;
extern String SV_PORT;
extern const char *SV_MD_NAME[];
extern int SV_PREV_PT_X;
extern int SV_PREV_PT_Y;
extern int SV_SWING_CNT;
extern int SV_SWING_AXIS;
extern int SV_SWING_LEN;
extern bool SV_MD_RANDOM_1st;
extern int SV_NEXT_PT_X;
extern int SV_NEXT_PT_Y;
extern int SV_MD_NAME_NO;
extern int SV_PIN_X;
extern int SV_PIN_Y;
extern const String WS_VERSION;
extern uint16_t SHUTDOWN_TM_SEC;
extern int REQ_SPK_PARMS_NO;
extern int REQ_SPK_EXPR;
extern String REQ_MSG;
extern void servo2(int mode);
extern void sv_setEaseToX(int x);
extern void sv_setEaseToY(int y);
extern void sv_setEaseToXY(int x, int y);
extern void tone(int mode);

void servo(void *args);
bool setFace(int expr);
void lipSync(void *args);
void avatarSetup();

#endif
