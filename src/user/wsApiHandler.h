// ----------------------------<wsApiHandler.h>------------------------------------
#ifndef WS_APIHANDLER_H
#define WS_APIHANDLER_H
// ---------------------------
#include <Avatar.h>
// #include <Expression.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <Wire.h>
#include <nvs.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_wifi_types.h"
#include "esp_bt.h"
#include "lib/AudioOutputM5Speaker.h"
#include <deque>

// #define TIMER_INIT 180          // タイマー初期値：３分
// #define TIMER_MIN 30            // 最小タイマー設定値：３０秒
// #define TIMER_MAX (60 * 60 - 1) // 最大タイマー設定値：６０分未満 (59分59秒)
#define WK_CNT_MAX 10           // 「わかりません」が何回連続すると初期化するか
// #define SHUTDOWN_MIN_TM 3

// ---- servo define
#define SV_CENTER_X 90
#define SV_CENTER_Y 90
#define SV_PIN_X_CORE2_PA 33 // Core2 PORT A
#define SV_PIN_Y_CORE2_PA 32
#define SV_PIN_X_CORE2_PC 13 // Core2 PORT C
#define SV_PIN_Y_CORE2_PC 14

#define SV_MD_MOVING 0
#define SV_MD_HOME 1
#define SV_MD_ADJUST 2
#define SV_MD_STOP 3
#define SV_MD_CENTER 4
#define SV_MD_POINT 5
#define SV_MD_DELTA 6
#define SV_MD_SWING 7
#define SV_MD_RANDOM 8
#define SV_MD_NONE 9

#define SV_REQ_MSG_CLS 0
#define SV_REQ_SPEAK 1
#define SV_REQ_MSG 2
#define SV_REQ_SPEAK_MSG 3
#define REQ_SPEAK_MSG2 4
#define SV_REQ_MD_ADJUST 9

#define SV_SWING_AXIS_X 0
#define SV_SWING_AXIS_Y 1
#define SV_SWING_AXIS_XY 2

#define SV_X_MIN 0
#define SV_X_MAX 180
#define SV_Y_MIN 50
#define SV_Y_MAX 100

#define WIFIJSON_SIZE 5 * 256
#define APIKEYJSON_SIZE 5 * 128
#define STARTUPJSON_SIZE 15 * 128
#define SERVOJSON_SIZE 5 * 128
#define SHUTDOWN_MIN_TM 3

#define FLTYPE_SPIFFS 1
#define FLTYPE_SD 2



extern const String json_ChatString;
// extern DynamicJsonDocument CHAT_DOC;
extern bool REQ_chatGPT_GET;
// // extern AsyncWebServerRequest *SERVER_REQUEST;
// extern int REQ_SPK_EXPR;
// extern int REQ_SPK_PARMS_NO;
// extern int REQ_SHUTDOWN_REBOOT;
extern int SV_MD;
extern int SV_HOME_X;
extern int SV_HOME_Y;
extern int SV_PT_X;
extern int SV_PT_Y;
extern int SV_PREV_PT_X;
extern int SV_PREV_PT_Y;
extern int SV_NEXT_PT_X;
extern int SV_NEXT_PT_Y;
extern bool SV_MD_RANDOM_1st;
extern int SV_SWING_CNT;
extern int SV_SWING_AXIS;
extern int SV_SWING_LEN;
// extern int REQUEST_GET;
extern String REQ_MSG;

extern char *SERVO_SD;
extern char *APIKEY_SD;
extern char *STARTUP_SD;

extern char *SETTING_NVS;
extern uint8_t m5spk_virtual_channel;
extern String IP_ADDR;
// extern String LANG_CODE;
extern uint8_t TTS_TYPE;
extern const char *TTS_NAME[];
extern size_t VOLUME_VALUE;
extern bool MUTE_ON_STATE;



// extern String processorS00(const String &var);

extern void wsHandleSpeech(String message, String expression, String speaker);


// -- from exServer.cpp ---
extern String webpage;
extern String HTML_Header();
extern String HTML_Footer();
extern AsyncWebServer server;

// -- from main.cpp ---
extern void Req_MsgCls();
extern void servoSetup2();
extern void muteOff();
extern void muteOn();
extern bool SYSINFO_DISP_STATE;
extern bool KEYLOCK_STATE;
extern uint8_t TONE_MODE;
extern bool LED_OnOff_STATE;
extern String TTS2_SPEAKER_NO;
extern String TTS2_PARMS;
extern String TTS2_SPEAKER;
extern void led_allOff();
extern void tone(int);
extern bool sysInfoGet(String txArg, String &txData);
extern void sysInfoDispEnd();
extern String SYSINFO_MSG;
extern void sysInfoDispMake(uint8_t mode_no);
extern void sysInfoDispStart(uint8_t mode_no);
extern bool RANDOM_SPEAK_STATE;
extern bool RANDOM_SPEAK_ON_GET;
extern void timerStop2();
// extern void randomSpeak(bool);

extern bool jsonSave(DynamicJsonDocument &jsonDoc, String saveFile);
extern bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String readFile);


// extern bool jsonFlRd_Sd(const char *flName_SD, DynamicJsonDocument &jsonName);
extern bool getServo(String item, String &data, DynamicJsonDocument &servoJson);
extern bool setServo(String item, String data, DynamicJsonDocument &servoJson);
extern bool getApiKey(String item, String &data, DynamicJsonDocument &apikeyJson);
extern bool setApiKey(String item, String data, DynamicJsonDocument &apikeyJson);
extern bool getStartup(String item, String &data, DynamicJsonDocument &startupJson);
extern bool setStartup(String item, String data, DynamicJsonDocument &startupJson);

extern bool initWifiJson(DynamicJsonDocument &wifiJson);
extern bool EX_wifiSelctFLSv(DynamicJsonDocument &wifiJson);
extern bool wsWifiFLRd(DynamicJsonDocument &wifiJson);


extern void wsHandleStartup(String serverNameS, String volumeS, String ledS, String toneModeS,
   String muteS, String keyLockS, String vSpeakerNoS, String randomSpeakS,
   String timerS, String txS);



extern char *tts_parms_table[];
extern bool setFace(int expr);
// extern void wsHandleApikeySet(String openai, String voicevox, String voicetext, String sttapikey);
extern void wsHandleApikeySetting(String openAiS, String voicevoxS,String txS);
//    String muteS, String keyLockS, String timerS, String txS );
extern void wsHandleSetting(String volumeS, String volumeDS, String speakerS, String ledS, String muteS, String keyLockS, String toneModeS);


// ----------------------------------------------------------
bool setGetStrToServoSetting(const char *item, DynamicJsonDocument &servoJson, AsyncWebServerRequest *request);
bool setGetStrToStartSetting(const char *item, DynamicJsonDocument &startupJson, AsyncWebServerRequest *request);
bool setGetStrToApiKeySetting(const char *item, DynamicJsonDocument &apikeyJson, String get_str);
// bool setGetStrToApiKeySetting(const char *item, DynamicJsonDocument &apikeyJson, AsyncWebServerRequest *request);


// String processr(const String &var);
extern void wsHandleBtn(String arg);
extern void wsHandleShutdown(String reboot_get_str, String time_get_str);
extern void wsHandleTimer(String TmSecS, String TmMinS, String timerModeS);
extern void wsHandleSetting01(String langS,String ttsNameS,String muteS,String keyLockS,String toneModeS );
extern void wsHandleSetting00(String volumeS,String volumeDS,String speakerS,String ledS );
extern void wsHandleSysInfo(String txS, String dispS, String modeS);
extern void wsHandleServo(String swingXYS,String swingXS, String swingYS,
  String pointXS, String pointYS, String deltaXS, String deltaYS,
  String txS, String modeS);
extern void wsHandleRoleSet(String roleS);
extern void wsHandleRoleGet();
extern void wsHandleFace(String expression);
extern void wsHandelChat(String textS, String voiceS);
extern void wsHandleRandomSpeak(String modeS);
extern void wsServoSetting(String txS,String servoS, String servoPortS,
      String servoModeS, String servoHomeXS, String servoHomeYS);


void serverSend(AsyncWebServerRequest *request);
void setupApiHandler();
void handle_button(AsyncWebServerRequest *request);
void handle_setting(AsyncWebServerRequest *request);
void handle_setting2(AsyncWebServerRequest *request);
void handle_sysInfo(AsyncWebServerRequest *request);
void handle_randomSpeak(AsyncWebServerRequest *request);
void handle_servo(AsyncWebServerRequest *request);
void handle_servoSetting(AsyncWebServerRequest *request);
void handle_apikey(AsyncWebServerRequest *request);
void handle_startup(AsyncWebServerRequest *request);
void handle_WifiSetting(AsyncWebServerRequest *request);
void handle_shutdown(AsyncWebServerRequest *request);
void handle_timer(AsyncWebServerRequest *request);
void handle_speech(AsyncWebServerRequest *request);
void handle_face(AsyncWebServerRequest *request);
void handle_chat(AsyncWebServerRequest *request);

void handle_role_set(AsyncWebServerRequest *request);
void handle_role_get(AsyncWebServerRequest *request);
void handle_apikey_set(AsyncWebServerRequest *request);

#define BtnREQ_A 1
#define BtnREQ_B 2
#define BtnREQ_C 3
#define BtnREQ_BOX_SERVO 4
#define BtnREQ_BOX_STT 5
extern int BTN_REQ;

// ----------------------------------------------------------
using namespace m5avatar;
extern Avatar avatar;

extern uint16_t TIMER_SEC_VALUE;
extern bool TIMER_STOP_GET;
extern bool TIMER_GO_GET;
extern bool TIMER_STARTED;

extern void randomSpeakStop2();
extern void timerStart();
// uint32_t TIMER_START_MILLIS = 0;
// uint16_t TIMER_ELEAPSE_SEC = 0;
// bool TIMER_STOP_GET = false;
// bool TIMER_GO_GET = false;

// extern int TTS_PARMS_NO;
extern String INIT_BUFFER;
extern bool init_chat_doc(const char *data);
// extern String Role_JSON;
extern bool save_json();
extern void chatHistoryCls();
extern void wsHandleWifiSetting(String initS, String ssidS, String passwdS,String removeS,
    String ipS, String gatewayS, String subnetS, String dnsS  );


#endif
