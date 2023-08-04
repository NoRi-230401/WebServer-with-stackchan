// ----------------------------<wsServer.h>------------------------------------
#ifndef WS_SERVER_H
#define WS_SERVER_H
// ---------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>          
#include <ESPAsyncWebServer.h> 
#include "esp_system.h"        
#include "esp_spi_flash.h"     
#include "esp_wifi_types.h"    
#include "esp_bt.h"            
#include <ESPmDNS.h>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include <WiFi.h>  
#include <Wire.h>

extern String SERVER_NAME;
extern const String WS_VERSION;
extern String NAME_U01;
extern String NAME_U02;
extern String NAME_U03;
extern String NAME_U04;
extern String NAME_U05;
extern String USER_U01;
extern String USER_U02;
extern String USER_U03;
extern String USER_U04;
extern String USER_U05;
extern void setupApiHandler();
extern void setupUserHandler();
extern String IP_ADDR;
extern void wait_SD();

void serverSetup();
void LogOut();
void Directory();
void Dir(AsyncWebServerRequest *request);
void UploadFileSelect();
void Format();
void handleFileUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
void File_Stream();
void File_Delete();
void Handle_File_Delete(String filename);
void File_Rename();
void Handle_File_Rename(AsyncWebServerRequest *request, String filename, int Args);
void notFound(AsyncWebServerRequest *request);
void Handle_File_Download();
String getContentType(String filenametype);
void Select_File_For_Function(String title, String function);
void SelectInput(String Heading, String Command, String Arg_name);
int GetFileSize(String filename);
void Home();
void LogOut();
void Page_Not_Found();
void Display_System_Info();
String ConvBinUnits(int bytes, int resolution);
String EncryptionType(wifi_auth_mode_t encryptionType);
bool StartMDNSservice(const char *Name);
String HTML_Header();
String HTML_Footer();

#endif
