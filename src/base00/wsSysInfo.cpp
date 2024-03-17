// ---------------------------< wsSysInfo.cpp >------------------------------------
#include "wsSysInfo.h"

using namespace m5avatar;
extern Avatar avatar;

// bool SYSINFO_DISP_STATE = false;
int SYSINFO_NO = 0;

void wsHandleSysInfo(String txS, String dispS, String modeS)
{
  SYSINFO_MSG = "";

  if (txS == "" && dispS == "" && modeS == "")
  {
    sysInfoDispMake(0);
    webpage = SYSINFO_MSG;
    return;
  }

  // -- system Information 個別情報送信　-----
  if (txS != "")
  {
    String tx_data = "";
    if (sysInfoGet(txS, tx_data))
      webpage = tx_data;
    return;
  }

  if (dispS != "")
  {
    if (dispS == "off")
    {
      avatarResume();
      // sysInfoDispEnd();
      webpage = "OK";
    }
    else if (dispS == "on")
    {
      avatarStop();
      avatarStop2();
      sysInfoDispStart(0);
      webpage = SYSINFO_MSG;
      return;
    }
    return;
  }

  if (modeS != "")
  {
    if (modeS == "all")
    {
      sysInfoDispMake(0);
      webpage = SYSINFO_MSG;
    }
    else
    {
      uint8_t mode_no = (uint8_t)modeS.toInt();
      sysInfoDispMake(mode_no);
      webpage = SYSINFO_MSG;
    }
    return;
  }

  return;
}

bool sysInfoGet(String txArg, String &txData)
{
  String msg = "";
  char msg2[100];

  if (txArg == "version")
  {
    txData = WSS_VERSION;
  }
  else if (txArg == "IP_Addr")
  {
    txData = "IP_Addr = " + IP_ADDR;
    Serial.println(txData);
  }
  else if (txArg == "SSID")
  {
    txData = "SSID = " + SSID;
  }
  else if (txArg == "batteryLevel")
  {
    sprintf(msg2, "batteryLevel = %d %%", getBatteryLevel());
    txData = msg2;
  }
  else if (txArg == "volume")
  {
    sprintf(msg2, "volume = %d", VOLUME_VALUE);
    txData = msg2;
  }
  else if (txArg == "vSpeakerNo" || txArg == "vSpkNo")
  {
    sprintf(msg2, "vSpkNo = %d", TTS_vSpkNo);
    txData = msg2;
  }
  else if (txArg == "timer")
  {
    sprintf(msg2, "timer = %d sec", TM_SEC_VAL);
    txData = msg2;
  }
  else if (txArg == "mute")
  {
    if (MUTE_ON_STATE)
    {
      msg = "mute = on";
      txData = msg;
    }
    else
    {
      msg = "mute = off";
      txData = msg;
    }
  }
  else if (txArg == "randomSpeak")
  {
    if (RANDOM_SPEAK_STATE)
    {
      msg = "randomSpeak = on";
      txData = msg;
    }
    else
    {
      msg = "randomSpeak = off";
      txData = msg;
    }
  }
  else if (txArg == "uptime")
  {
    uint32_t uptime = millis() / 1000;
    uint16_t up_sec = uptime % 60;
    uint16_t up_min = (uptime / 60) % 60;
    uint16_t up_hour = uptime / (60 * 60);
    sprintf(msg2, "uptime = %02d:%02d:%02d", up_hour, up_min, up_sec);
    txData = msg2;
  }
  else if (txArg == "toneMode")
  {
    sprintf(msg2, "toneMode = %d", TONE_MODE);
    txData = msg2;
  }
  else if (txArg == "servo")
  {
    if (SV_USE)
    {
      msg = "servo = on";
      txData = msg;
    }
    else
    {
      msg = "servo = off";
      txData = msg;
    }
  }
  else if (txArg == "servoPort")
  {
    if (SV_PORT == "portC")
    {
      msg = "servoPort = portC";
      txData = msg;
    }
    else
    {
      msg = "servo = portA";
      txData = msg;
    }
  }
  else if (txArg == "led")
  {
    if (LED_OnOff_STATE == true)
    {
      msg = "led = on";
      txData = msg;
    }
    else
    {
      msg = "led = off";
      txData = msg;
    }
  }
  else if (txArg == "WK_errorNo")
  {
    sprintf(msg2, "WK_errorNo = %d", WK_LAST_ERR_NO);
    txData = msg2;
  }
  else if (txArg == "WK_errorCode")
  {
    sprintf(msg2, "WK_errorCode = %d", WK_LAST_ERR_CODE);
    txData = msg2;
  }

  // ---- Network Settings ---
  else if (txArg == "SSID_PASSWD")
  {
    txData = "SSID_PASSWD = " + SSID_PASSWD;
  }
  else if (txArg == "openAiApiKey")
  {
    txData = "openAiApiKey = " + OPENAI_API_KEY;
  }
  // else if (txArg == "voiceTextApiKey")
  // {
  //   txData = "voiceTextApiKey = " + VOICETEXT_API_KEY;
  // }
  else if (txArg == "voicevoxApiKey")
  {
    txData = "voicevoxApiKey = " + VOICEVOX_API_KEY;
  }
  else
  {
    Serial.println(txArg);
    Serial.println(txData);
    return false;
  }

  Serial.println(txArg);
  Serial.println(txData);
  return true;
}


void sysInfoDispStart(uint8_t mode_no)
{
  M5.Display.setTextColor(TFT_WHITE, TFT_BLUE);
  M5.Display.fillScreen(BLUE);
  M5.Display.setTextDatum(0);
  M5.Display.setCursor(0, 0);
  
  delay(50);
  sysInfoDispMake(mode_no);
  M5.Display.print(SYSINFO_MSG);
  // SYSINFO_DISP_STATE = true;
}

// void sysInfoDispEnd()
// {
//   // if (!SYSINFO_DISP_STATE)
//   // {
//   //   return;
//   // }
//   // delay(100);
//   // SYSINFO_DISP_STATE = false;
// }

uint8_t getBatteryLevel()
{
  return (M5.Power.getBatteryLevel());
}

void sysInfoDispMake(uint8_t mode_no)
{
  switch (mode_no)
  {
  case 0:
    sysInfo_m00_DispMake();
    break;

  case 1:
    sysInfo_m01_DispMake();
    break;

  case 2:
    sysInfo_m02_DispMake();
    break;

  case 3:
    sysInfo_m03_DispMake();
    break;

  case 4:
    sysInfo_m04_DispMake();
    break;

  case 5:
    sysInfo_m05_DispMake();
    break;

  case 6:
    sysInfo_m06_DispMake();
    break;

  default:
    sysInfo_m00_DispMake();
    break;
  }
}

void sysInfo_m00_DispMake()
{
  String msg = "";
  char msg2[100];
  String page = "[1/" + String(STM2X_SYSINFO_LEN, DEC) + "] ";
  SYSINFO_MSG = page + " -  SYSTEM Info  -\n";

  SYSINFO_MSG += WSS_NAME;
  SYSINFO_MSG += "\n" + WSS_VER;
  SYSINFO_MSG += "\n\nIP = " + IP_ADDR;
  SYSINFO_MSG += "\nSSID = " + SSID;
  SYSINFO_MSG += "\nserverName = " + SERVER_NAME;

  sprintf(msg2, "\nbatteryLevel = %d %%", getBatteryLevel());
  SYSINFO_MSG += msg2;

  sprintf(msg2, "\nvolume = %d", VOLUME_VALUE);
  SYSINFO_MSG += msg2;
  SYSINFO_MSG += "\nvSpkNo = " + String(TTS_vSpkNo, DEC);

  if (RANDOM_SPEAK_STATE)
  {
    msg = "\nrandomSpeak = on";
    SYSINFO_MSG += msg;
  }
  else
  {
    msg = "\nrandomSpeak = off";
    SYSINFO_MSG += msg;
  }

  if (LED_OnOff_STATE)
  {
    sprintf(msg2, "\nled = on");
    SYSINFO_MSG += msg2;
  }
  else
  {
    sprintf(msg2, "\nled = off");
    SYSINFO_MSG += msg2;
  }

  if (KEYLOCK_STATE)
  {
    msg = "\nkeyLock = on";
    SYSINFO_MSG += msg;
  }
  else
  {
    msg = "\nkeyLock = off";
    SYSINFO_MSG += msg;
  }

  sprintf(msg2, "\ntoneMode = %d", TONE_MODE);
  SYSINFO_MSG += msg2;

  sprintf(msg2, "\nWK_ERR: No = %d Code = %d", WK_LAST_ERR_NO, WK_LAST_ERR_CODE);
  SYSINFO_MSG += msg2;
}

void sysInfo_m01_DispMake()
{
  String page = "[2/" + String(STM2X_SYSINFO_LEN, DEC) + "] ";
  SYSINFO_MSG = page + " - Passwd/ApiKey -\n";

  SYSINFO_MSG += "\nSSID: " + SSID;
  SYSINFO_MSG += "\nSSID_PASS: " + SSID_PASSWD;
  SYSINFO_MSG += "\n\nopenAiApiKey :\n " + OPENAI_API_KEY;
  SYSINFO_MSG += "\n\nvoicevoxApiKey :\n " + VOICEVOX_API_KEY;
}

void sysInfo_m02_DispMake()
{
  String page = "[3/" + String(STM2X_SYSINFO_LEN, DEC) + "] ";
  SYSINFO_MSG = page + " -  MAC  -\n";

  uint8_t mac0[6];
  char s[200];
  esp_efuse_mac_get_default(mac0);
  sprintf(s, "\n\ndef_MAC= %02X:%02X:%02X:%02X:%02X:%02X", mac0[0], mac0[1], mac0[2], mac0[3], mac0[4], mac0[5]);
  SYSINFO_MSG += String(s);

  uint8_t mac3[6];
  esp_read_mac(mac3, ESP_MAC_WIFI_STA);
  sprintf(s, "\n\nStation= %02X:%02X:%02X:%02X:%02X:%02X", mac3[0], mac3[1], mac3[2], mac3[3], mac3[4], mac3[5]);
  SYSINFO_MSG += String(s);

  uint8_t mac4[7];
  esp_read_mac(mac4, ESP_MAC_WIFI_SOFTAP);
  sprintf(s, "\n\nSoftAP = %02X:%02X:%02X:%02X:%02X:%02X", mac4[0], mac4[1], mac4[2], mac4[3], mac4[4], mac4[5]);
  SYSINFO_MSG += String(s);

  uint8_t mac5[6];
  esp_read_mac(mac5, ESP_MAC_BT);
  sprintf(s, "\n\nBLE_MAC= %02X:%02X:%02X:%02X:%02X:%02X", mac5[0], mac5[1], mac5[2], mac5[3], mac5[4], mac5[5]);
  SYSINFO_MSG += String(s);

  uint8_t mac6[6];
  esp_read_mac(mac6, ESP_MAC_ETH);
  sprintf(s, "\n\nETH_MAC= %02X:%02X:%02X:%02X:%02X:%02X", mac6[0], mac6[1], mac6[2], mac6[3], mac6[4], mac6[5]);
  SYSINFO_MSG += String(s);
}

void sysInfo_m03_DispMake()
{
  String page = "[4/" + String(STM2X_SYSINFO_LEN, DEC) + "] ";
  SYSINFO_MSG = page + " -  CPU Info  -\n";

  uint64_t chipid;
  char s[200];

  // The chip ID is essentially its MAC address(length: 6 bytes).
  chipid = ESP.getEfuseMac();

  // print High 2 bytes
  sprintf(s, "\nESP32 Chip ID = %04X", (uint16_t)(chipid >> 32));
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nChip Revision %d", ESP.getChipRevision());
  SYSINFO_MSG += String(s);

  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  sprintf(s, "\n\nNumber of Core: %d", chip_info.cores);
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nCPU Frequency: %d MHz", ESP.getCpuFreqMHz());
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nFlash Memory Size = %d MB", ESP.getFlashChipSize() / (1024 * 1024));
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nFlash Chip Speed = %d MHz", ESP.getFlashChipSpeed() / (1000 * 1000));
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nESP-IDF version = %s", esp_get_idf_version());
  SYSINFO_MSG += String(s);
}

// -- Core2 Memory size bytes define ----
#define Core2_SRAM 532480L
#define Core2_PSRAM 4194304L
#define Core2_TOTAL_RAM (Core2_SRAM + Core2_PSRAM)
#define Core2_SYS_RAM_USED 204800L
#define Core2_USR_MAX_RAM (Core2_TOTAL_RAM - Core2_SYS_RAM_USED)

void sysInfo_m04_DispMake()
{
  String page = "[5/" + String(STM2X_SYSINFO_LEN, DEC) + "] ";
  SYSINFO_MSG = page + " -  RAM  -\n";

  char s[200];

  int mDEF = heap_caps_get_free_size(MALLOC_CAP_DEFAULT) / 1024;
  int mPSRAM = heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024;
  int mDMA = heap_caps_get_free_size(MALLOC_CAP_DMA) / 1024;

  int usrMax = Core2_USR_MAX_RAM / 1024;
  int usedRAM = usrMax - mDEF;

  sprintf(s, "\nRAM  = %4d KB : 100%%", usrMax);
  SYSINFO_MSG += String(s);

  sprintf(s, "\nUsed = %4d KB : %3.1f%%", usedRAM, (usedRAM * 100.0) / usrMax);
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\n\n -- Free Heap size --");
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nDefault = %4d KB : %3.1f%%", mDEF, (mDEF * 100.0) / usrMax);
  SYSINFO_MSG += String(s);

  sprintf(s, "\n  PSRAM = %4d KB", mPSRAM);
  SYSINFO_MSG += String(s);

  sprintf(s, "\n  DMA   = %4d KB", mDMA);
  SYSINFO_MSG += String(s);
}

void sysInfo_m05_DispMake()
{
  String page = "[6/" + String(STM2X_SYSINFO_LEN, DEC) + "] ";
  SYSINFO_MSG = page + " - SPIFFS Space -\n";

  int total_b = SPIFFS.totalBytes();
  int used_b = SPIFFS.usedBytes();
  int total_kb = total_b / 1024;
  int used_kb = used_b / 1024;
  int free_kb = total_kb - used_kb;

  float used_percent = (float)((100.0 * used_b) / total_b);
  float free_percent = (float)(100.0 - used_percent);

  char s[200];
  sprintf(s, "\n\n\nTotal:100.0%%   %4d KB", total_kb);
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nUsed : %4.1f%%   %4d KB", used_percent, used_kb);
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nFree : %4.1f%%   %4d KB", free_percent, free_kb);
  SYSINFO_MSG += String(s);
}

void sysInfo_m06_DispMake()
{
  String page = "[7/" + String(STM2X_SYSINFO_LEN, DEC) + "] ";
  SYSINFO_MSG = page + " -  NVS stats  -\n";

  nvs_stats_t stats;

  esp_err_t err = nvs_get_stats("nvs", &stats);
  if (err != ESP_OK)
  {
    Serial.println("failed to nvs_get_stats()");
    return;
  }

  size_t total_ent = stats.total_entries;
  size_t used_ent = stats.used_entries;
  size_t free_ent = stats.free_entries;
  size_t namespace_cnt = stats.namespace_count;

  char s[200];
  sprintf(s, "\n\n\nTotal_entries   = %4d", total_ent);
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nUsed_entries    = %4d", used_ent);
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nFree_entries    = %4d", free_ent);
  SYSINFO_MSG += String(s);

  sprintf(s, "\n\nNamespace_count = %4d", namespace_cnt);
  SYSINFO_MSG += String(s);
}

void EPS32_system_info(void)
{
  //  Serial.begin(115200);
  Serial.println("\r\n-----------------------------");
  uint64_t chipid;

  // The chip ID is essentially its MAC address(length: 6 bytes).
  chipid = ESP.getEfuseMac();

  // print High 2 bytes
  Serial.printf("ESP32 Chip ID = %04X\r\n", (uint16_t)(chipid >> 32));

  Serial.printf("Chip Revision %d\r\n", ESP.getChipRevision());
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  Serial.printf("Number of Core: %d\r\n", chip_info.cores);
  Serial.printf("CPU Frequency: %d MHz\r\n", ESP.getCpuFreqMHz());
  Serial.printf("Flash Chip Size = %d byte\r\n", ESP.getFlashChipSize());
  Serial.printf("Flash Frequency = %d Hz\r\n", ESP.getFlashChipSpeed());
  Serial.printf("ESP-IDF version = %s\r\n", esp_get_idf_version());

  // 利用可能なヒープのサイズを取得
  Serial.printf("Available Heap Size= %d\r\n", esp_get_free_heap_size());

  // 利用可能な内部ヒープのサイズを取得
  Serial.printf("Available Internal Heap Size = %d\r\n", esp_get_free_internal_heap_size());

  // これまでに利用可能だった最小ヒープを取得します
  Serial.printf("Minimum Free Heap Ever Available Size = %d\r\n", esp_get_minimum_free_heap_size());
  Serial.println();

  uint8_t mac0[6];
  esp_efuse_mac_get_default(mac0);
  Serial.printf("Default Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac0[0], mac0[1], mac0[2], mac0[3], mac0[4], mac0[5]);

  uint8_t mac3[6];
  esp_read_mac(mac3, ESP_MAC_WIFI_STA);
  Serial.printf("[Wi-Fi Station] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac3[0], mac3[1], mac3[2], mac3[3], mac3[4], mac3[5]);

  uint8_t mac4[7];
  esp_read_mac(mac4, ESP_MAC_WIFI_SOFTAP);
  Serial.printf("[Wi-Fi SoftAP] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac4[0], mac4[1], mac4[2], mac4[3], mac4[4], mac4[5]);

  uint8_t mac5[6];
  esp_read_mac(mac5, ESP_MAC_BT);
  Serial.printf("[Bluetooth] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac5[0], mac5[1], mac5[2], mac5[3], mac5[4], mac5[5]);

  uint8_t mac6[6];
  esp_read_mac(mac6, ESP_MAC_ETH);
  Serial.printf("[Ethernet] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac6[0], mac6[1], mac6[2], mac6[3], mac6[4], mac6[5]);
}

void info_spiffs()
{
  float total_mb = SPIFFS.totalBytes() / (1024 * 1024);
  float used_mb = SPIFFS.usedBytes() / (1024 * 1024);
  float free_mb = total_mb - used_mb;

  char s[200];
  sprintf(s, "Total Space = %.3f MB", total_mb);
  Serial.println(s);

  sprintf(s, "Used Space = %.3f MB", used_mb);
  Serial.println(s);

  sprintf(s, "Free Space = %.3f MB", free_mb);
  Serial.println(s);
}
