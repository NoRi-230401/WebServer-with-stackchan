// ----------------------------<wsServer2.h>------------------------------------
#ifndef _WS_SERVER2_H
#define _WS_SERVER2_H
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

extern String SERVER_NAME;
extern String webpage;
extern void Home();
extern String HTML_Header();
extern String HTML_Footer();
extern AsyncWebServer server;
extern String ConvBinUnits(int bytes, int resolution);
typedef struct
{
  String filename;
  String ftype;
  String fsize;
} fileinfo;
extern fileinfo Filenames[]; // Enough for most purposes!
extern void setupApiHandler();
extern void setupUserHandler();
extern void setupMDNS();
extern void serverSetup1();
extern bool StartMDNSservice(const char *Name);
extern void FilesDirList();

//-----------------------------------------------------
void setupMDNS();
void serverSetup();
void serverSetup2();
void FilesDirList();
void handle_fileSystem(AsyncWebServerRequest *request);
void handle_root_sd();
void Select_Dir_For_Function(String title, String function);
void Handle_chdir(String filename);
void Handle_rmdir(String filename);
void Handle_mkdir(AsyncWebServerRequest *request);
void Dir_Make();
void SelectInputDirName(String Heading, String Command, String Arg_name);
void DirsList();

// -------------------------------------------------------

#endif
