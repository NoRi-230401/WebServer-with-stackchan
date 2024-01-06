// ----------------------------<wsUtil.cpp>------------------------------------
#include "wsUtil.h"
uint16_t SHUTDOWN_TM_SEC;
bool isSD_enable = false;

void wsHandleShutdown(String reboot_get_str, String time_get_str)
{
  uint16_t time_sec = SHUTDOWN_MIN_TM;

  if (time_get_str != "")
  {
    time_sec = time_get_str.toInt();

    if (time_sec < SHUTDOWN_MIN_TM)
      time_sec = SHUTDOWN_MIN_TM;

    if (time_sec > 60)
      time_sec = 60;
  }

  if (reboot_get_str == "on")
  {
    SHUTDOWN_TM_SEC = time_sec;
    REQ_SHUTDOWN_REBOOT = 2; // reboot
    SV_MD = SV_MD_HOME;

    webpage = "reboot : after " + String(time_sec, DEC) + "sec";
    Serial.println(webpage);
    return;
  }

  // --- shutdown
  SHUTDOWN_TM_SEC = time_sec;
  REQ_SHUTDOWN_REBOOT = 1; // shutdown
  SV_MD = SV_MD_HOME;
  webpage = "shutdown : after " + String(time_sec, DEC) + "sec";
  Serial.println(webpage);
  return;
}

void errSTOP()
{
  Serial.println("Stop : Fatal Error Occurred!");
  SD.end();
  SPIFFS.end();
  led_allOff();
  delay(5000);

  for (;;)
  {
    delay(1000);
  }
}

void REBOOT()
{
  Serial.println(" *** Reboot ***");
  SD.end();
  SPIFFS.end();
  led_allOff();
  delay(5000);
  ESP.restart();

  for (;;)
  { // never
    delay(1000);
  }
}

void POWER_OFF()
{
  Serial.println(" *** POWER OFF ***");

  SD.end();
  SPIFFS.end();
  led_allOff();
  delay(5000);
  M5.Power.powerOff();

  for (;;)
  { // never
    delay(1000);
  }
}

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void)isUnicode; // Punt this ball for now
  // Note that the type and string may be in PROGMEM, so copy them to RAM for printf
  char s1[32], s2[64];
  strncpy_P(s1, type, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  strncpy_P(s2, string, sizeof(s2));
  s2[sizeof(s2) - 1] = 0;
  Serial.printf("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);
  Serial.flush();
}

// Called when there's a warning or error (like a buffer underflow or decode hiccup)
void StatusCallback(void *cbData, int code, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  // Note that the string may be in PROGMEM, so copy it to RAM for printf
  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}

bool SD_begin()
{
  if (!SD.begin(GPIO_NUM_4, SPI, 25000000))
  { // SD無効な時
    Serial.println("Fail : Begin SD");
    SD.end();
    isSD_enable = false;
    return false;
  }
  else
  {
    isSD_enable = true;
    return true;
  }
}

void SD_end()
{
  isSD_enable = false;
  SD.end();
}

File SD_open(const String path, const char *mode)
{
  // 途中で SD.end()を呼び出している箇所があるので必ずおこなう。
  // if (!isSD_enable)
  SD_begin();

  return (SD.open(path, mode));
}

static File FILE_PT;
File fileOpen(int flType, const String path, const char *mode)
{

  if (flType == FLTYPE_SPIFFS)
  {
    FILE_PT = SPIFFS.open(path.c_str(), mode);

    Serial.println("###  SPIFFS.open(" + path + "," + String(mode) + ")  ###");
    return FILE_PT;
  }
  else if (flType == FLTYPE_SD)
  {
    FILE_PT = SD_open(path.c_str(), mode);
    Serial.println("###  SD.open(" + path + "," + String(mode) + ")  ###");
    return FILE_PT;
  }

  return ((File)0);
}

bool jsonRead(int flType, DynamicJsonDocument &jsonDoc, String filePath)
{
  File file = fileOpen(flType, filePath, "r");
  if (!file)
  {
    Serial.println("Fail : Open file for reading #: " + filePath);
    file.close();
    return false;
  }

  DeserializationError error = deserializeJson(jsonDoc, file);
  file.close();

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  return true;
}

bool jsonSave(DynamicJsonDocument &jsonDoc, const String filePath)
{
  File file = SPIFFS.open(filePath.c_str(), "w");
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }

  serializeJsonPretty(jsonDoc, file);
  file.close();
  return true;
}

bool jsonInitSave(DynamicJsonDocument &jsonDoc, const String inJson, const String saveFile)
{
  bool success = jsonInit(jsonDoc, inJson);

  if (!success)
    return false;

  File fl_SPIFFS = SPIFFS.open(saveFile, "w");
  if (!fl_SPIFFS)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }
  // JSONデータをシリアル化して書き込む
  serializeJsonPretty(jsonDoc, fl_SPIFFS);
  // serializeJson(jsonDoc, fl_SPIFFS);
  fl_SPIFFS.close();

  return true;
}

bool jsonInit(DynamicJsonDocument &jsonDoc, const String inJson)
{
  DeserializationError error = deserializeJson(jsonDoc, inJson.c_str());
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  String json_str;
  serializeJsonPretty(jsonDoc, json_str);
  Serial.println(json_str);

  return true;
}

bool setJsonItem(String flName, String item, String setData, DynamicJsonDocument &jsonDoc, String arrayName)
{
  if (!jsonRead(FLTYPE_SPIFFS, jsonDoc, flName))
  {
    Serial.println("faile to Read jsonDoc in SPIFFS");
    return false;
  }

  JsonArray jsonArray = jsonDoc[arrayName];
  JsonObject object = jsonArray[0];
  object[item] = setData;

  bool success = jsonSave(jsonDoc, flName);
  if (!success)
  {
    return false;
  }
  return true;
}

bool getJsonItem(String flName, String item, String &getData, DynamicJsonDocument &jsonDoc, String arrayName)
{
  if (!jsonRead(FLTYPE_SPIFFS, jsonDoc, flName))
  {
    Serial.println("faile to Read jsonDoc in SPIFFS");
    return false;
  }

  JsonArray jsonArray = jsonDoc[arrayName];
  JsonObject object = jsonArray[0];
  String data_tmp = object[item];
  if (data_tmp != "")
  {
    getData = data_tmp;
    return true;
  }
  getData = "";
  return false;
}

// 空きメモリをシリアル出力 from つゆきぱぱさん
void log_free_size(const char *text)
{
  M5.Log.printf("%s メモリ残/最大ブロック残（DEFAULT->DMA->SPIRAM）：%4dKB/%4dKB %3dKB/%3dKB %4dKB/%4dKB\n", text,
                heap_caps_get_free_size(MALLOC_CAP_DEFAULT) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT) / 1024,
                heap_caps_get_free_size(MALLOC_CAP_DMA) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_DMA) / 1024,
                heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024, heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM) / 1024);
}
