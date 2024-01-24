// ----------------------------<wsApi.h>------------------------------------
#ifndef WS_API_H
#define WS_API_H
// ---------------------------
#include <Avatar.h>
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

#define WK_CNT_MAX 10           // 「わかりません」が何回連続すると初期化するか
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
#define SV_MD_SWING 4
#define SV_MD_STOP 5
#define SV_MD_ADJUST 6
#define SV_MD_POINT 7
#define SV_MD_DELTA 8
#define SV_MD_NONE 99

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
#define BtnREQ_A 1
#define BtnREQ_B 2
#define BtnREQ_C 3
#define BtnREQ_BOX_SERVO 4
#define BtnREQ_BOX_STT 5

// extern const String chatStrIniData;
extern bool REQ_chatGPT_GET;
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
extern String REQ_MSG;
extern char *SETTING_NVS;
// extern uint8_t m5spk_virtual_channel;
extern String IP_ADDR;
extern uint8_t VOLUME_VALUE;
extern bool MUTE_ON_STATE;
extern String webpage;
extern String HTML_Header();
extern String HTML_Footer();
extern String HTML_Header3();
extern String HTML_Footer3();
extern AsyncWebServer server;
// extern void Req_MsgCls();
extern void servoSetup2();
extern void muteOff();
extern void muteOn();
extern bool SYSINFO_DISP_STATE;
extern bool KEYLOCK_STATE;
extern uint8_t TONE_MODE;
extern bool LED_OnOff_STATE;
// extern String TTS_SPEAKER_NO;
// extern String TTS_PARMS;
// extern String TTS_SPEAKER;
extern void ledClearAll();
extern void tone(int);
extern bool sysInfoGet(String txArg, String &txData);
extern void sysInfoDispEnd();
extern String SYSINFO_MSG;
extern void sysInfoDispMake(uint8_t mode_no);
extern void sysInfoDispStart(uint8_t mode_no);
extern bool RANDOM_SPEAK_STATE;
extern bool RANDOM_SPEAK_ON_GET;

extern void wsHandleSpeech(String message, String expression, String speaker);
extern void timerStop2();
extern bool jsonDocSave(DynamicJsonDocument &jsonDoc, String saveFile);
extern bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String readFile);
extern bool getServo(String item, String &data, DynamicJsonDocument &servoJson);
extern bool setServo(String item, String data, DynamicJsonDocument &servoJson);
extern bool getApiKey(String item, String &data, DynamicJsonDocument &apikeyJson);
extern bool setApiKey(String item, String data, DynamicJsonDocument &apikeyJson);
extern bool getStartup(String item, String &data, DynamicJsonDocument &startupJson);
extern bool setStartup(String item, String data, DynamicJsonDocument &startupJson);
extern bool initWifiJson(DynamicJsonDocument &wifiJson);
extern void wsHandleStartup(String serverNameS, String volumeS, String ledS, String toneModeS,
   String muteS, String keyLockS, String vSpeakerNoS, String randomSpeakS,
   String timerS, String txS);
extern bool setAvatarExpr(int expr);
extern void wsHandleApikeySetting(String openAiS, String voicevoxS,String txS);
extern void wsHandleSetting(String volumeS, String volumeDS, String speakerS, String ledS, String muteS, String keyLockS, String toneModeS);
extern void wsHandleBtn(String arg);
extern void wsHandleShutdown(String reboot_get_str, String time_get_str);
extern void wsHandleTimer(String TmSecS, String TmMinS, String timerModeS);
extern void wsHandleSysInfo(String txS, String dispS, String modeS);
extern void wsHandleServo(String swingXYS,String swingXS, String swingYS,
  String pointXS, String pointYS, String deltaXS, String deltaYS,
  String txS, String modeS);
extern void wsHandleRoleGet();
extern void wsHandleFace(String expression);
extern void wsHandleBalloon(String text);
extern void wsHandelChat(String textS, String voiceS);
extern void wsHandelChatCharacter(String ch_NoS,String ch_nameS, String ch_voiceS,String ch_rollS);
extern void wsHandelChatGpt(String historyS,String charaS);
extern void wsHandleRandomSpeak(String modeS);
extern void wsServoSetting(String txS,String servoS, String servoPortS,
      String servoModeS, String servoHomeXS, String servoHomeYS);

extern int BTN_REQ;
using namespace m5avatar;
extern Avatar avatar;
// extern uint16_t TM_SEC_VAL;
extern bool TM_STOP_GET;
extern bool TM_GO_GET;
extern bool TM_STARTED;
extern void randomSpeakStop2();
extern void timerStart();
// extern String CHATDOC_INIT_BUF;
// extern bool setChatDoc(const char *data);
// extern bool save_json();
extern void chatHistoryCls();
extern void wsHandleWifiSetting(String initS, String ssidS, String passwdS,String removeS,
    String ipS, String gatewayS, String subnetS, String dnsS  );
extern void wsHandleRoleSet(String roleS);
extern void serverSend(AsyncWebServerRequest *request);
extern void serverSend3(AsyncWebServerRequest *request);

// ---------------------------------------------------------

void setupApiHandler();
void handle_button(AsyncWebServerRequest *request);
void handle_setting(AsyncWebServerRequest *request);
void handle_sysInfo(AsyncWebServerRequest *request);
void handle_randomSpeak(AsyncWebServerRequest *request);
void handle_servo(AsyncWebServerRequest *request);
void handle_servoSetting(AsyncWebServerRequest *request);
void handle_apikeySetting(AsyncWebServerRequest *request);
void handle_startupSetting(AsyncWebServerRequest *request);
void handle_wifiSetting(AsyncWebServerRequest *request);
void handle_shutdown(AsyncWebServerRequest *request);
void handle_timer(AsyncWebServerRequest *request);
void handle_speech(AsyncWebServerRequest *request);
void handle_face(AsyncWebServerRequest *request);
void handle_balloon(AsyncWebServerRequest *request);
void handle_chat(AsyncWebServerRequest *request);
void handle_chatCharacter(AsyncWebServerRequest *request);
void handle_chatGpt(AsyncWebServerRequest *request);
void handle_role_set(AsyncWebServerRequest *request);
void handle_role_get(AsyncWebServerRequest *request);
// ---------------------------------------------------------

#endif
