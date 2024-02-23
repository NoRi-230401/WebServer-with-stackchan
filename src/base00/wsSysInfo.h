// ---------------------------< wsSysInfo.h >------------------------------------
#ifndef _WS_SYSINFO_H
#define _WS_SYSINFO_H
// ---------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Avatar.h>
#include <AudioOutput.h>
#include <deque>
#include <SD.h>
#include <FS.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <nvs.h>
#include <WiFi.h>  
extern String EncryptionType(wifi_auth_mode_t encryptionType);

// ---- servo define
#define SV_CENTER_X 90
#define SV_CENTER_Y 90
#define SV_PIN_X_CORE2_PA 33 // Core2 PORT A
#define SV_PIN_Y_CORE2_PA 32
#define SV_PIN_X_CORE2_PC 13 // Core2 PORT C
#define SV_PIN_Y_CORE2_PC 14

extern String SERVER_NAME;
extern int WK_LAST_ERR_NO;
extern int WK_LAST_ERR_CODE;
extern bool RANDOM_SPEAK_STATE;
extern uint8_t VOLUME_VALUE;
extern bool MUTE_ON_STATE;
extern uint8_t TONE_MODE;
extern String SYSINFO_MSG;
extern String IP_ADDR;
extern String SSID;
extern String SSID_PASSWD;
extern bool KEYLOCK_STATE;
extern String VOICEVOX_API_KEY;
extern String OPENAI_API_KEY;
extern String webpage;
extern uint8_t TTS_vSpkNo;
extern bool LED_OnOff_STATE;
extern uint16_t TM_SEC_VAL;
extern void timerStop2(); 
extern bool SV_USE;
extern int SV_HOME_X;
extern int SV_HOME_Y;
extern String SV_PORT;
extern const String SV_MD_NAME[];
extern int SV_MD_NAME_NO;
extern const String WSS_VERSION;
extern const String WSS_NAME;
extern const String WSS_VER;
extern void avatarStop();
extern void avatarStop2();
extern void avatarResume();

//------------------------------------------------------------
void wsHandleSysInfo(String txS, String dispS, String modeS);

bool sysInfoGet(String txArg, String &txData);
void sysInfoDispStart(uint8_t mode_no);
void sysInfoDispEnd();
uint8_t getBatteryLevel();
void sysInfoDispMake(uint8_t mode_no);

#define STM2X_SYSINFO_LEN 7
void sysInfo_m00_DispMake();
void sysInfo_m01_DispMake();
void sysInfo_m02_DispMake();
void sysInfo_m03_DispMake();
void sysInfo_m04_DispMake();
void sysInfo_m05_DispMake();
void sysInfo_m06_DispMake();

void randomSpeakStop2();
void EPS32_system_info(void);
//------------------------------------------------------------

#endif
