// ---------------------------< wsServo.h >------------------------------------
#ifndef WS_SERVO_H
#define WS_SERVO_H
// ---------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <ServoEasing.hpp>

// ---- servo define ----------
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


// -- Request ---------
#define REQ_MSG_CLS 0
#define REQ_SPEAK_ADJUST 1
#define REQ_BALOON_ADJUST 2
#define REQ_SPEAK_BALOON_ADJUST 3
#define REQ_SPEAK 4
#define REQ_BALOON 5
#define REQ_SV_MD_ADJUST 9


#define SV_SWING_AXIS_X 0
#define SV_SWING_AXIS_Y 1
#define SV_SWING_AXIS_XY 2
#define SV_X_MIN 0
#define SV_X_MAX 180
#define SV_Y_MIN 50
#define SV_Y_MAX 100
#define SERVOJSON_SIZE 5 * 128
#define FLTYPE_SPIFFS 1
#define FLTYPE_SD 2

extern void sendReq(int reqNo, String msg);
extern bool setJsonItem(String flName, String item, String setData, DynamicJsonDocument &jsonDoc, String arrayName);
extern bool getJsonItem(String flName, String item, String& getData, DynamicJsonDocument &jsonDoc, String arrayName);
extern ServoEasing servo_x;
extern ServoEasing servo_y;
extern int REQ_NO_GET;
extern String REQ_MSG;
extern bool jsonDocSave(DynamicJsonDocument &jsonDoc, String saveFile);
extern bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String readFile);
extern void Req_SpkBaloonDo_adjust(); 
extern void Req_BaloonDo_adjust();
extern void Req_SpkDo_adjust();
// extern void Req_Baloon_adjust();
extern bool KEYLOCK_STATE;
// extern void ReqSpkMsg();
// extern void ReqSpkBaloon_adjust(String spkMsg);
extern String webpage;
extern void BaloonClear();
extern void tone(int mode);
extern bool jsonStrSave(DynamicJsonDocument &jsonDoc,const String inJson, const String saveFile);
extern void sysInfoDispEnd();
extern bool SYSINFO_DISP_STATE;

// -----------------------------------------------------------------
bool jsonSERVOinit(DynamicJsonDocument &jsonDoc);
void BoxServoDo();
void wsHandleServo(String swingXYS,String swingXS, String swingYS,
  String pointXS, String pointYS, String deltaXS, String deltaYS,
  String txS, String modeS);
void wsServoSetting(String txS,String servoS, String servoPortS,
      String servoModeS, String servoHomeXS, String servoHomeYS);
void servo2(int mode);
void servoSetup();
void servoSetup2();
void servoSetting();
bool servoFileRead();
void sv_setEaseToX(int x);
void sv_setEaseToY(int y);
void sv_setEaseToXY(int x, int y);
void sv_easeToX(int x);
void sv_easeToY(int y);
void sv_easeToXY(int x, int y);
void servoSwing(int sw_mode, int repeatNum, int len);
void SV_random();
// -----------------------------------------------------------------

#endif
