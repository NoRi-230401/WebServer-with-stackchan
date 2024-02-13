// ---------------------------< wsSetting.h >------------------------------------
#ifndef _WS_SETTING_H
#define _WS_SETTING_H
// ---------------------------

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPIFFS.h>
#include <nvs.h>
#include <M5Unified.h>
#include <Avatar.h>

#define TIMER_INIT 180          // タイマー初期値：３分
#define TIMER_MIN 30            // 最小タイマー設定値：３０秒
#define TIMER_MAX (60 * 60 - 1) // 最大タイマー設定値：６０分未満 (59分59秒)
#define APIKEYJSON_SIZE 3 * 128
#define STARTUPJSON_SIZE 10 * 128
#define FLTYPE_SPIFFS 1
#define FLTYPE_SD 2

extern String SERVER_NAME;
extern void BoxTouchSetup();
extern bool KEYLOCK_STATE;
// extern bool SYSINFO_DISP_STATE;
extern bool RANDOM_SPEAK_STATE;
extern uint8_t TTS_vSpkNo;

extern bool LED_OnOff_STATE;
extern bool RANDOM_SPEAK_ON_GET;
extern uint16_t TM_SEC_VAL;
extern bool TM_STARTED;
extern void ledClearAll();
extern void timerStop2();
extern void randomSpeakStop2();
extern void timerStart();
extern void timerStop();
extern void timerStop2();
extern void sysInfoDispStart(uint8_t mode_no);
extern bool jsonDocSave(DynamicJsonDocument &jsonDoc, String saveFile);
extern bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String readFile);
extern bool setJsonItem(String flName, String item, String setData, DynamicJsonDocument &jsonDoc, String arrayName);
extern bool SD_begin();
extern bool jsonStrSave(DynamicJsonDocument &jsonDoc, const String inJson, const String saveFile);
extern bool toJsonDoc(DynamicJsonDocument &jsonDoc, const String inJson);
extern File fileOpen(int flType, const String path, const char *mode);
extern bool getJsonItem(String flName, String item, String &getData, DynamicJsonDocument &jsonDoc, String arrayName);
using namespace m5avatar;
extern Avatar avatar;
extern String webpage;
extern int isSPIFFS;
extern uint8_t VOLUME_VALUE;

#define TONE_MODE_INIT 4
#define TONE_MODE_MAX 5
extern uint8_t TONE_MODE;
extern bool MUTE_ON_STATE;
#define VAL_NVS_NOSAVE 0
#define VAL_NVS_SAVE 1
// extern uint8_t setVolumeVal(uint8_t volumeVal, int save_flag);
extern void setVolume(int vol);
extern void muteOn();
extern void muteOff();
#define TTS_VSPKNO_MAX 66
#define TTS_VSPKNO_INIT 3
extern void setTTSvSpkNo(int spkNo);
extern uint8_t getTTSvSpkNofmNVS();
extern void M5LedBegin();
//-------------------------------------------------------------------------
void startupSetting();
void startupSetting00();
void wsHandleSetting(String volumeS, String volumeDS, String speakerS,
                     String ledS, String muteS, String keyLockS, String toneModeS);
void wsHandleStartup(String serverNameS, String volumeS, String ledS,
                     String toneModeS, String muteS, String keyLockS, String vSpeakerNoS,
                     String randomSpeakS, String timerS, String txS);
void wsHandleApikeySetting(String openAiS, String voicevoxS, String txS);
void M5StackConfig();
void M5FileSystemBegin();
bool jsonAPIKEYinit(DynamicJsonDocument &jsonDoc);

void apikeySetting();
bool apiKeyJsonSDRead();
bool apiKeyTxtSDRead();
bool apiKeyJsonSpiffsRead();

bool jsonSTARTUPinit(DynamicJsonDocument &jsonDoc);
void nvsSaveAll();
bool startupFileRead();
bool setApiKey(String item, String data, DynamicJsonDocument &apikeyJson);
bool getApiKey(String item, String &data, DynamicJsonDocument &apikeyJson);
bool setStartup(String item, String data, DynamicJsonDocument &startupJson);
bool getStartup(String item, String &getData, DynamicJsonDocument &startupJson);

//-------------------------------------------------------------------------

#endif
