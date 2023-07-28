// ---------------------------< wsSetting.h >------------------------------------
#ifndef WS_SETTING_H
#define WS_SETTING_H
// ---------------------------

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPIFFS.h>
#include <nvs.h>
#include <M5Unified.h>
#include <Avatar.h>

#define APIKEYJSON_SIZE 5 * 128
#define STARTUPJSON_SIZE 15 * 128
// #define SERVOJSON_SIZE 5 * 128
#define FLTYPE_SPIFFS 1
#define FLTYPE_SD 2

// ---- servo define ----------
#define SV_CENTER_X 90
#define SV_CENTER_Y 90
#define SV_PIN_X_CORE2_PA 33 // Core2 PORT A
#define SV_PIN_Y_CORE2_PA 32
#define SV_PIN_X_CORE2_PC 13 // Core2 PORT C
#define SV_PIN_Y_CORE2_PC 14

extern String SERVER_NAME;
extern void BoxTouchSetup();
extern bool KEYLOCK_STATE;
extern bool SYSINFO_DISP_STATE;
extern bool RANDOM_SPEAK_STATE;

extern String TTS2_SPEAKER_NO;
extern String TTS2_SPEAKER;
extern String TTS2_PARMS;
extern String VoiceText_TTS_USER;
extern uint8_t TTS_TYPE;
extern String LANG_CODE;
extern bool LED_OnOff_STATE;
extern bool RANDOM_SPEAK_ON_GET;

extern uint16_t TIMER_SEC_VALUE;
extern const char *TTS_NAME[];
extern uint8_t m5spk_virtual_channel;
extern const char *LANG_CODE_JP;
extern int RANDOM_TIME;
extern bool TIMER_STARTED;

extern void led_allOff();
extern void EX_touchOperation();
extern void timerStop2(); 
extern void randomSpeak(bool mode);
extern void randomSpeakStop2();
extern void timerStart();
extern void timerStop();
extern void timerStop2(); 
extern void report_batt_level();
extern void sysInfoDispStart(uint8_t mode_no);
extern void EX_sysInfoDisp();
extern void sysInfoDispEnd();
extern bool jsonSave(int flType, DynamicJsonDocument &jsonDoc, String saveFile);
extern bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String readFile);
extern bool setJsonItem(String flName, String item, String setData, DynamicJsonDocument &jsonDoc, String arrayName);
extern bool SD_begin();

using namespace m5avatar;
extern Avatar avatar;
void wsHandleSetting(String volumeS, String volumeDS, String speakerS, String ledS);
void wsHandleSetting2(String langS, String ttsNameS, String muteS, String keyLockS, String toneModeS);
void wsHandleApikeySet(String openai, String voicevox, String voicetext,String sttapikey );

bool setGetStrToApiKeySetting(const char *item, DynamicJsonDocument &apikeyJson, String get_str);
bool setGetStrToStartSetting(const String item, DynamicJsonDocument &startupJson, String get_str);

bool apiKeyFileRead();
bool startupFileRead();

void wsHandleStartup(String ttsSelectS, String vvoxSpeakerNoS,String langS,
  String volumeS, String ledS, String randomSpeakS, String toneModeS, 
  String muteS, String keyLockS, String timerS, String txS );

bool setStartup(String item, String data, DynamicJsonDocument &startupJson);
bool setApiKey(String item, String data, DynamicJsonDocument &apikeyJson);
bool setServo(String item, String setData, DynamicJsonDocument &servoJson);

extern bool getJsonItem(String flName, String item, String& getData, DynamicJsonDocument &jsonDoc, String arrayName);
bool getStartup(String item, String &data, DynamicJsonDocument &startupJson);
bool getApiKey(String item, String &data, DynamicJsonDocument &apikeyJson);
bool getServo(String item, String &getData, DynamicJsonDocument &servoJson);

void toneOn();
void tone(int mode);
void muteOn();
void muteOff();
void M5StackConfig();


#endif
