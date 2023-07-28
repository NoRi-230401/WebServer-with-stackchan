// ---------------------------< wsUser.h >------------------------------------
#ifndef WS_USER_H
#define WS_USER_H
// ---------------------------
#include <Arduino.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;
extern String IP_ADDR;
extern String ServerName;
extern size_t VOLUME_VALUE;
extern const String WS_VERSION;
extern String webpage;

void setupUserHandler();
// String processor01(const String &var);
// String processor02(const String &var);
// String processor03(const String &var);
// String processor04(const String &var);
String processor05(const String &var);
String HTML_Footer();
String HTML_Header();
String HTML_Footer2();
String HTML_Header2();
String HTML_Header2Ng();

void Home();
void handle_s01();
void handle_s02();
void handle_s03();
void handle_s04();
bool htmlConv(const String flname);
void handle_test(AsyncWebServerRequest *request);
void checkWebReq(AsyncWebServerRequest *request);


// ---- end of < WS_USER_H > ------------------------------------------------
#endif
