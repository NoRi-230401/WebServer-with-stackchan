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

extern void setupApiHandler();
extern void setupUserHandler();
extern void Home();
extern String HTML_Header();
extern String HTML_Footer();
extern bool SD_ENABLE;

// -------------------------------------------------------
void serverSetup();
void Directory();
void Directory_org();
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
void Page_Not_Found();
void Display_System_Info();
String ConvBinUnits(int bytes, int resolution);
String EncryptionType(wifi_auth_mode_t encryptionType);
bool StartMDNSservice(const char *Name);
void handle_fileSystem(AsyncWebServerRequest *request);
void wait_SD();
// -------------------------------------------------------

#endif
