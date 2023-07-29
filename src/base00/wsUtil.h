// ----------------------------<wsUtil.h>------------------------------------
#ifndef WS_UTIL_H
#define WS_UTIL_H
// ---------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#define WIFIJSON_SIZE 5 * 256
#define FLTYPE_SPIFFS 1
#define FLTYPE_SD 2

#define SHUTDOWN_MIN_TM 3
extern int SV_MD;

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

extern void led_allOff();
extern int REQUEST_GET;
extern String REQ_MSG;
extern String webpage;
extern int REQ_SHUTDOWN_REBOOT;

void wsHandleShutdown(String reboot_get_str, String time_get_str);
// void errSTOP(const String msg);
// void REBOOT(const String msg);
// void POWER_OFF(const String msg);
void errSTOP();
void REBOOT();
void POWER_OFF();
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string);
void StatusCallback(void *cbData, int code, const char *string);

bool jsonInitSave(DynamicJsonDocument &jsonDoc,const String inJson, const String saveFile);
bool jsonSave(DynamicJsonDocument &jsonDoc, const String saveFile);
bool jsonInit(DynamicJsonDocument &jsonDoc, const String inJson);

bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String readFile);
bool setJsonItem(String flName, String item, String setData, DynamicJsonDocument &jsonDoc, String arrayName);
bool getJsonItem(String flName, String item, String& getData, DynamicJsonDocument &jsonDoc, String arrayName);

bool SD_begin();
void SD_end();
File SD_open(const String path, const char *mode);
File fileOpen(int flType, const String path, const char *mode);

#endif
