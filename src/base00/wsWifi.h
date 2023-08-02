// ----------------------------<wsWifi.h>------------------------------------

#ifndef WS_WIFI_H
#define WS_WIFI_H
// ---------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <sys/socket.h> //アドレスドメイン
#include <sys/types.h>  //ソケットタイプ
#include <arpa/inet.h>  //バイトオーダの変換に利用

#define WIFIJSON_SIZE 5 * 256
#define FLTYPE_SPIFFS 1
#define FLTYPE_SD 2
#define CONNECT_FAIL 99

extern String IP_ADDR;
extern String SSID;
extern String SSID_PASSWD;
extern String webpage;
extern const String WIFI_SPIFFS;
extern const String WIFI_SD;
extern void REBOOT();
extern bool jsonSave(DynamicJsonDocument &jsonDoc, String saveFile);
extern bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String readFile);
extern File fileOpen(int flType, const String path, const char *mode);
extern bool jsonInitSave(DynamicJsonDocument &jsonDoc,const String inJson, const String saveFile);

void wsHandleWifiSetting(String initS, String ssidS, String passwdS,String removeS, String ipS, String gatewayS, String subnetS, String dnsS  );
bool initWifiJson(DynamicJsonDocument &wifiJson);
bool jsonInitSave_wifi(DynamicJsonDocument &jsonDoc);
bool wifiNoSetupFileConnect();
bool wifiSmartConfigConnect();
void addSuccessAP();
int wifiConnect2();
bool wifiSelect(int flType);
bool wifiTxtRead();
bool wifiTxtConnect();
bool IPstrToIntArray(String strIPaddr, int *iAddr);

#endif
