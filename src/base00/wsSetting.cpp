// ---------------------------< wsSetting.cpp >------------------------------------
#include "wsSetting.h"

const char *SETTING_NVS = "setting"; // setting --NVS の設定用ファイル
const String APIKEY_SPIFFS = "/wsApiKey.json";
const String APIKEY_TXT_SD = "/apikey.txt";
const String STARTUP_SPIFFS = "/wsStartup.json";
const String OffOn[] = {"off", "on"};
const String jsonAPIKEY = "{\"apikey\":[{\"openAiApiKey\":\"***\",\"voicevoxApiKey\":\"***\"}]}";
const String jsonSTARTUP = "{\"startup\":[{\"serverName\":\"stackchan\",\"vSpeakerNo\":\"3\",\"volume\":\"200\",\"led\":\"on\",\"randomSpeak\":\"off\",\"toneMode\":\"1\",\"mute\":\"off\",\"keyLock\":\"off\",\"timer\":\"180\"}]}";

size_t VOLUME_VALUE;
bool MUTE_ON_STATE = false;
#define TONE_MODE_INIT 0
// uint8_t TONE_MODE = 1; // 0:allOff 1:buttonOn 2:extCommOn 3:allOn
uint8_t TONE_MODE = TONE_MODE_INIT; // 0:allOff(default) 1:buttonOn 2:extCommOn 3:allOn
String SYSINFO_MSG = "";
String IP_ADDR = "";
String SSID = "";
String SSID_PASSWD = "";
String OPENAI_API_KEY = "";
String VOICEVOX_API_KEY = "";
// String VOICETEXT_API_KEY = "";
String STT_API_KEY = "";
bool SD_ENABLE = false;

void startupSetting()
{
  startupFileRead();
  nvsSaveAll();
}

void apikeySetting()
{
  if (!apiKeyFileRead())
    apiKeyTxtRead();
}

void wsHandleSetting(String volumeS, String volumeDS, String vSpeakerNoS,
                     String ledS, String muteS, String keyLockS, String toneModeS)
{
  uint32_t nvs_handle;
  DynamicJsonDocument startupJson(STARTUPJSON_SIZE);

  // ---- volume -------
  if (volumeS != "")
  {
    int volumeVal = volumeS.toInt();
    if (volumeVal > 255)
      volumeVal = 255;
    if (volumeVal <= 0)
      volumeVal = 0;

    VOLUME_VALUE = volumeVal;
    M5.Speaker.setVolume(VOLUME_VALUE);
    M5.Speaker.setChannelVolume(m5spk_virtual_channel, VOLUME_VALUE);

    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
      nvs_set_u32(nvs_handle, "volume", VOLUME_VALUE);
    nvs_close(nvs_handle);

    webpage = "volume = " + String(volumeVal, DEC);
  }

  // ---- volumeDelta -------
  if (volumeDS != "")
  {
    int volumeVal = volumeDS.toInt();
    volumeVal += VOLUME_VALUE;
    if (volumeVal > 255)
      volumeVal = 255;
    if (volumeVal <= 0)
      volumeVal = 0;

    VOLUME_VALUE = volumeVal;
    Serial.print("volume = ");
    Serial.println(VOLUME_VALUE, DEC);

    M5.Speaker.setVolume(VOLUME_VALUE);
    M5.Speaker.setChannelVolume(m5spk_virtual_channel, VOLUME_VALUE);
    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    {
      nvs_set_u32(nvs_handle, "volume", VOLUME_VALUE);
    }
    nvs_close(nvs_handle);

    webpage = "volume = " + String(volumeVal, DEC);
  }

  // ---- led -------
  if (ledS != "")
  {
    String ledStr = ledS;
    if (ledStr == "toggle")
    {
      if (LED_OnOff_STATE)
        ledStr = "off";
      else
        ledStr = "on";
    }

    if (ledStr == "off")
    {
      led_allOff();
      LED_OnOff_STATE = false;
    }
    else if (ledStr == "on")
    {
      LED_OnOff_STATE = true;
    }

    uint8_t led_onoff = 0;
    if (LED_OnOff_STATE)
      led_onoff = 1;
    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
      nvs_set_u8(nvs_handle, "led", led_onoff);
    nvs_close(nvs_handle);

    webpage = "led = " + ledStr;
  }

  // ---- toneMode -------
  if (toneModeS != "")
  {
    if (toneModeS == "next")
    {
      TONE_MODE++;
      TONE_MODE = TONE_MODE % 4;
    }
    else
    {
      TONE_MODE = toneModeS.toInt();
      if ((TONE_MODE < 0) || (TONE_MODE > 3))
      {
        TONE_MODE = TONE_MODE_INIT;
      }
    }

    uint8_t tMode = TONE_MODE;
    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
      nvs_set_u32(nvs_handle, "toneMode", tMode);
    nvs_close(nvs_handle);

    webpage = "toneMode = " + String(TONE_MODE, DEC);
  }

  // ---- mute -------
  if (muteS != "")
  {
    if (muteS == "toggle")
    {
      if (MUTE_ON_STATE)
        muteS = "off";
      else
        muteS = "on";
    }

    if (muteS == "on")
    {
      if (!MUTE_ON_STATE)
      {
        muteOn();
      }
    }
    else if (muteS == "off")
    {
      if (MUTE_ON_STATE)
      {
        muteOff();
      }
    }

    uint8_t mute_onoff = 0;
    if (MUTE_ON_STATE)
      mute_onoff = 1;
    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
      nvs_set_u8(nvs_handle, "mute", mute_onoff);
    nvs_close(nvs_handle);

    webpage = "mute = " + muteS;
  }

  // ---- keyLock -------
  if (keyLockS != "")
  {
    if (keyLockS == "toggle")
    {
      if (KEYLOCK_STATE)
        keyLockS = "off";
      else
        keyLockS = "on";
    }

    if (keyLockS == "on")
    {
      KEYLOCK_STATE = true;
    }
    else if (keyLockS == "off")
    {
      KEYLOCK_STATE = false;
    }

    uint8_t keyLock_onoff = 0;
    if (KEYLOCK_STATE)
      keyLock_onoff = 1;
    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
      nvs_set_u8(nvs_handle, "keyLock", keyLock_onoff);
    nvs_close(nvs_handle);

    Serial.println("setting?keyLock = " + keyLockS);
    webpage = "keyLock = " + keyLockS;
  }

  // ---- Speaker -------
  size_t speaker_no;
  if (vSpeakerNoS != "")
  {
    speaker_no = vSpeakerNoS.toInt();
    if (speaker_no > 66)
    {
      speaker_no = 3;
    }
    TTS2_SPEAKER_NO = String(speaker_no);
    TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;

    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    {
      nvs_set_u32(nvs_handle, "vSpeakerNo", speaker_no);
      Serial.println("NVS Write : vSpeakerNo = " + String(speaker_no, DEC));
    }
    nvs_close(nvs_handle);
    webpage = "vSpeakerNo = " + String(speaker_no, DEC);
  }
  Serial.println(webpage);
}

void wsHandleStartup(String serverNameS, String volumeS, String ledS, String toneModeS,
                     String muteS, String keyLockS, String vSpeakerNoS, String randomSpeakS,
                     String timerS, String txS)
{
  DynamicJsonDocument startupJson(STARTUPJSON_SIZE);

  // -------------------------------------------------------
  if (txS != "")
  {
    String getStr = "";
    if (getStartup(txS, getStr, startupJson))
      webpage = "wsSetting.json : " + txS + " = " + getStr;
    return;
  }

  if (serverNameS != "")
  {
    if (setStartup("serverName", serverNameS, startupJson))
      webpage = "wsSetting.Json : serverName = " + serverNameS;
    return;
  }

  if (volumeS != "")
  {
    if (setStartup("volume", volumeS, startupJson))
      webpage = "wsSetting.Json : volume = " + volumeS;
    return;
  }

  if (ledS != "")
  {
    if (setStartup("led", ledS, startupJson))
      webpage = "wsSetting.Json : led = " + ledS;
    return;
  }

  if (toneModeS != "")
  {
    if (setStartup("toneMode", toneModeS, startupJson))
      webpage = "wsSetting.Json : toneMode = " + toneModeS;
    return;
  }

  if (muteS != "")
  {
    if (setStartup("mute", muteS, startupJson))
      webpage = "wsStartup.Json : mute = " + muteS;
    return;
  }

  if (keyLockS != "")
  {
    if (setStartup("keyLock", keyLockS, startupJson))
      webpage = "wsSetting.Json : keyLock = " + keyLockS;
    return;
  }

  // ---------------------------------------------------------
  if (vSpeakerNoS != "")
  {
    if (setStartup("vSpeakerNo", vSpeakerNoS, startupJson))
      webpage = "wsSetting.Json : vSpeakerNo = " + vSpeakerNoS;
    return;
  }

  if (randomSpeakS != "")
  {
    if (setStartup("randomSpeak", randomSpeakS, startupJson))
      webpage = "wsSetting.Json : randomSpeak = " + randomSpeakS;
    return;
  }

  if (timerS != "")
  {
    if (setStartup("timer", timerS, startupJson))
      webpage = "wsSetting.Json : timer = " + timerS;
    return;
  }

  // -------------------------------------------------------
  // HTML形式でファイルを表示する
  {
    if (!jsonRead(FLTYPE_SPIFFS, startupJson, STARTUP_SPIFFS))
    {
      Serial.println("faile to Read wsSetting.json in SPIFFS");
      return;
    }

    String html = "";
    serializeJsonPretty(startupJson, html);
    Serial.println(html);
    webpage = html;
    return;
  }
}

void wsHandleApikeySetting(String openAiS, String voicevoxS, String txS)
{
  DynamicJsonDocument apikeyJson(APIKEYJSON_SIZE);

  if (openAiS == "" && voicevoxS == "" && txS == "")
  { // HTML形式でファイルを表示する
    if (!jsonRead(FLTYPE_SPIFFS, apikeyJson, APIKEY_SPIFFS))
    {
      Serial.println("faile to Read wsApikey.json in SPIFFS");
      return;
    }

    String html = "";
    serializeJsonPretty(apikeyJson, html);
    Serial.println(html);
    webpage = html;
    return;
  }

  // -------------------------------------------------------
  if (txS != "")
  {
    String getStr = "";
    if (getApiKey(txS, getStr, apikeyJson))
    {
      webpage = "wsApikey.json : " + txS + " = " + getStr;
      Serial.println(webpage);
    }
    return;
  }

  // -------------------------------------------------------
  if (openAiS != "")
  {
    if (setApiKey("openAiApiKey", openAiS, apikeyJson))
    {
      OPENAI_API_KEY = openAiS;
      STT_API_KEY = openAiS;
      webpage = "wsApikey.json : openAiApiKey = " + openAiS;
      Serial.println(webpage);
    }
    return;
  }

  // -------------------------------------------------------
  if (voicevoxS != "")
  {
    if (setApiKey("voicevoxApiKey", voicevoxS, apikeyJson))
    {
      VOICEVOX_API_KEY = voicevoxS;
      webpage = "wsApikey.json : voicevoxApiKey = " + voicevoxS;
      Serial.println(webpage);
    }
    return;
  }

  return;
}

void M5StackConfig()
{
  // ********** M5 config **************
  auto cfg = M5.config();

  cfg.external_spk = true; /// use external speaker (SPK HAT / ATOMIC SPK)
                           // cfg.external_spk_detail.omit_atomic_spk = true; // exclude ATOMIC SPK
  // cfg.external_spk_detail.omit_spk_hat    = true; // exclude SPK HAT
  // cfg.output_power = true;
  M5.begin(cfg);
  // delay(500);

  // { // Mic Setting
  //   auto micConfig = M5.Mic.config();
  //   micConfig.stereo = false;
  //   micConfig.sample_rate = 16000;
  //   M5.Mic.config(micConfig);
  // }
  // M5.Mic.begin();

  { // Speaker Setting
    auto spk_cfg = M5.Speaker.config();
    /// Increasing the sample_rate will improve the sound quality instead of increasing the CPU load.
    spk_cfg.sample_rate = 96000; // default:64000 (64kHz)  e.g. 48000 , 50000 , 80000 , 96000 , 100000 , 128000 , 144000 , 192000 , 200000
    spk_cfg.task_pinned_core = APP_CPU_NUM;
    M5.Speaker.config(spk_cfg);
  }
  M5.Speaker.begin();

  M5.Lcd.setTextSize(2);
  led_allOff();
  BoxTouchSetup();

  // --- SPIFFS begin ----
  if (!SPIFFS.begin(true))
  {
    Serial.println("Error preparing SPIFFS Filing System...");
    // StartupErrors = true;
  }
  isSPIFFS = 1;  
  
  // --- SD begin -------
  int i = 0;
  bool success = false;
  // Serial.println("SD.begin Start ...");
  while (i < 3)
  { // SDカードマウント待ち
    // success = SD.begin(GPIO_NUM_4, SPI, 15000000, "/sdcard", 10, false);
    // success = SD.begin(GPIO_NUM_4, SPI, 16000000,"/sd", 10, false);
    // success = SD.begin(GPIO_NUM_4, SPI, 15000000,"/sd", 10, false);
    success = SD.begin(GPIO_NUM_4, SPI, 15000000U,"/sd", 10U, false);
    
    if (success)
      break;

    Serial.println("SD Wait...");
    delay(500);
    i++;
  }
  
  if (i >= 3)
  {
    Serial.println("SD.begin faile ...");
    SD_ENABLE = false;
  }
  else
    SD_ENABLE = true;

}



bool jsonAPIKEYinit(DynamicJsonDocument &jsonDoc)
{
  return (jsonInitSave(jsonDoc, jsonAPIKEY, APIKEY_SPIFFS));
}

bool apiKeyTxtRead()
{
  if(!SD_ENABLE)
    return false;
    
  File fs = fileOpen(FLTYPE_SD, APIKEY_TXT_SD, "r");
  if (!fs)
  {
    fs.close();
    Serial.println("Fail : apikey.txt not open ");
    return false;
  }

  size_t sz = fs.size();
  char buf[sz + 1];
  fs.read((uint8_t *)buf, sz);
  buf[sz] = 0;
  fs.close();

  int y = 0;
  int z = 0;
  for (int x = 0; x < sz; x++)
  {
    if (buf[x] == 0x0a || buf[x] == 0x0d)
      buf[x] = 0;
    else if (!y && x > 0 && !buf[x - 1] && buf[x])
      y = x;
    else if (!z && x > 0 && !buf[x - 1] && buf[x])
      z = x;
  }

  OPENAI_API_KEY = String(buf);
  VOICEVOX_API_KEY = String(&buf[y]);
  STT_API_KEY = OPENAI_API_KEY;
  Serial.println("** Read data from apikey.txt in SD **");

  DynamicJsonDocument apikeyJson(APIKEYJSON_SIZE);
  setApiKey("openAiApiKey", OPENAI_API_KEY, apikeyJson);
  setApiKey("voicevoxApiKey", VOICEVOX_API_KEY, apikeyJson);
  Serial.println("** and Save to wsApikey.json in SPISS  **");

  return true;
}

bool apiKeyFileRead()
{
  // ****** 初期値設定　**********
  OPENAI_API_KEY = "***";
  VOICEVOX_API_KEY = "***";
  STT_API_KEY = "***";
  // VOICETEXT_API_KEY = "***";
  //----------------------------------
  DynamicJsonDocument apikeyJson(APIKEYJSON_SIZE);

  if (!jsonRead(FLTYPE_SPIFFS, apikeyJson, APIKEY_SPIFFS))
  {
    Serial.println("initialize wsApiKey.json in SPIFFS");
    jsonAPIKEYinit(apikeyJson);
    return false;
  }

  JsonArray jsonArray = apikeyJson["apikey"];
  JsonObject object = jsonArray[0];

  int cnt = 0;
  // String getStr;

  // openAiApiKey
  String getStr0 = object["openAiApiKey"];
  if (getStr0 != "" && (getStr0 != "null"))
  {
    OPENAI_API_KEY = getStr0;
    STT_API_KEY = getStr0;
    Serial.println("ApiKey : openAiApiKey = " + getStr0);
    cnt++;
  }

  // voicevoxApiKey
  String getStr2 = object["voicevoxApiKey"];
  if (getStr2 != "" && (getStr2 != "null"))
  {
    VOICEVOX_API_KEY = getStr2;
    Serial.println("ApiKey : voicevoxApiKey = " + getStr2);
    cnt++;
  }

  Serial.println("** wsApiKey.json total " + String(cnt, DEC) + " item read **");
  return true;
}

bool jsonSTARTUPinit(DynamicJsonDocument &jsonDoc)
{
  return (jsonInitSave(jsonDoc, jsonSTARTUP, STARTUP_SPIFFS));
}

void nvsSaveAll()
{
  size_t volume = (size_t)VOLUME_VALUE;
  size_t timer_value = (size_t)TIMER_SEC_VALUE;
  size_t tone_mode = (size_t)TONE_MODE;
  size_t speaker_no = (size_t)TTS2_SPEAKER_NO.toInt();

  uint8_t led_onoff = 0;
  if (LED_OnOff_STATE)
    led_onoff = 1;

  uint8_t mute_onoff = 0;
  if (MUTE_ON_STATE)
    mute_onoff = 1;

  uint8_t keylock_onoff = 0;
  if (KEYLOCK_STATE)
    keylock_onoff = 1;

  uint8_t randomSpeak_onoff = 0;
  if (RANDOM_SPEAK_STATE)
    randomSpeak_onoff = 1;

  uint32_t nvs_handle;
  if (ESP_OK == nvs_open("setting", NVS_READWRITE, &nvs_handle))
  {
    nvs_set_u32(nvs_handle, "volume", volume);
    nvs_set_u32(nvs_handle, "timer", timer_value);
    nvs_set_u32(nvs_handle, "toneMode", tone_mode);
    nvs_set_u32(nvs_handle, "vSpeakerNo", speaker_no);

    nvs_set_u8(nvs_handle, "led", led_onoff);
    nvs_set_u8(nvs_handle, "mute", mute_onoff);
    nvs_set_u8(nvs_handle, "keyLock", keylock_onoff);
    nvs_set_u8(nvs_handle, "randomSpeak", randomSpeak_onoff);
  }
  nvs_close(nvs_handle);
}

bool startupFileRead()
{
  // ****** 初期値設定　**********
  SERVER_NAME = "stackchan";
  // TTS_TYPE = 2; // VOICEVOX
  TTS2_SPEAKER_NO = "3";
  // LANG_CODE = String(LANG_CODE_JP);
  VOLUME_VALUE = 200;
  LED_OnOff_STATE = true;
  RANDOM_TIME = -1;
  RANDOM_SPEAK_ON_GET = false;
  RANDOM_SPEAK_STATE = false;
  TONE_MODE = TONE_MODE_INIT;
  MUTE_ON_STATE = false;
  KEYLOCK_STATE = false;
  TIMER_SEC_VALUE = 180;

  //----------------------------------
  DynamicJsonDocument startupJson(STARTUPJSON_SIZE);

  if (!jsonRead(FLTYPE_SPIFFS, startupJson, STARTUP_SPIFFS))
  {
    Serial.println("DeserializationError in wsStartup.json in SPIFFS");

    Serial.println("initialize wsStartup.json in SPIFFS");
    jsonSTARTUPinit(startupJson);
  }

  JsonArray jsonArray = startupJson["startup"];
  JsonObject object = jsonArray[0];

  int cnt = 0;

  // serverName
  String getStr0 = object["serverName"];
  if (getStr0 != "" && (getStr0 != "null"))
  {
    String getData = getStr0;
    SERVER_NAME = getData;
    Serial.println("Startup : serverName  = " + SERVER_NAME);
    cnt++;
  }

  // volume
  String getStr3 = object["volume"];
  if ((getStr3 != "") && (getStr3 != "***") && (getStr3 != "-1") && (getStr3 != "null"))
  {
    int getVal = getStr3.toInt();
    if (getVal < 0)
      getVal = 0;
    if (getVal > 255)
      getVal = 255;

    VOLUME_VALUE = (size_t)getVal;
    Serial.println("Startup : volume = " + getStr3);
    cnt++;
  }
  else
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
    {
      size_t volume;
      nvs_get_u32(nvs_handle, "volume", &volume);
      if (volume > 255)
        volume = 255;
      if (volume < 0)
        volume = 0;

      VOLUME_VALUE = volume;
      nvs_close(nvs_handle);
      Serial.println("Startup : NVS volume = " + String(volume, DEC));
      cnt++;
    }
  }
  M5.Speaker.setVolume(VOLUME_VALUE);
  M5.Speaker.setChannelVolume(m5spk_virtual_channel, VOLUME_VALUE);

  // led
  String getStr4 = object["led"];
  if (getStr4 != "" && (getStr4 != "***") && (getStr4 != "null"))
  {
    String getData = getStr4;
    if (getData.equalsIgnoreCase("off"))
    {
      LED_OnOff_STATE = false;
    }
    Serial.println("Startup : led = " + getStr4);
    cnt++;
  }
  else
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
    {
      uint8_t led_onoff;
      nvs_get_u8(nvs_handle, "led", &led_onoff);
      nvs_close(nvs_handle);
      if (led_onoff == 0)
      {
        LED_OnOff_STATE = false;
      }
      Serial.println("Startup : NVS led = " + OffOn[led_onoff]);
      cnt++;
    }
  }

  // toneMode
  String getStr6 = object["toneMode"];
  if (getStr6 != "" && (getStr6 != "***") && (getStr6 != "-1") && (getStr6 != "null"))
  {
    int getVal = getStr6.toInt();
    if (getVal < 0 || getVal > 3)
      getVal = TONE_MODE_INIT;
    TONE_MODE = getVal;
    Serial.println("Startup : toneMode = " + getStr6);
    cnt++;
  }
  else
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
    {
      size_t mode;
      nvs_get_u32(nvs_handle, "toneMode", &mode);
      if (mode < 0 || mode > 3)
        mode = TONE_MODE_INIT;
      TONE_MODE = mode;
      nvs_close(nvs_handle);
      Serial.println("Startup : NVS toneMode = " + String(mode, DEC));
      cnt++;
    }
  }

  // mute
  String getStr7 = object["mute"];
  if ((getStr7 != "") && (getStr7 != "***") && (getStr7 != "null"))
  {
    String getData = getStr7;
    if (getData.equalsIgnoreCase("on"))
      muteOn();
    Serial.println("Startup : mute = " + getStr7);
    cnt++;
  }
  else
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
    {
      uint8_t mute_onoff = 0;
      nvs_get_u8(nvs_handle, "mute", &mute_onoff);
      nvs_close(nvs_handle);
      if (mute_onoff == 1)
      {
        muteOn();
      }
      Serial.println("Startup : NVS mute = " + OffOn[mute_onoff]);
      cnt++;
    }
  }

  // keyLock
  String getStr8 = object["keyLock"];
  if ((getStr8 != "") && (getStr8 != "***") && (getStr8 != "null"))
  {
    String getData = getStr8;
    if (getData.equalsIgnoreCase("on"))
      KEYLOCK_STATE = true;
    Serial.println("Startup : keyLock = " + getStr8);
    cnt++;
  }
  else
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
    {
      uint8_t keyLock_onoff = 0;
      nvs_get_u8(nvs_handle, "keyLock", &keyLock_onoff);
      nvs_close(nvs_handle);
      if (keyLock_onoff == 1)
      {
        KEYLOCK_STATE = true;
      }
      Serial.println("Startup : NVS keyLock = " + OffOn[keyLock_onoff]);
      cnt++;
    }
  }

  // --------------------------------------------------------------------------

  // --- SPEAKER ---
  String getStr1 = object["vSpeakerNo"];
  if ((getStr1 != "") && (getStr1 != "***") && (getStr1 != "-1") && (getStr1 != "null"))
  {
    TTS2_SPEAKER_NO = getStr1;
    Serial.println("Startup : vSpeakerNo = " + TTS2_SPEAKER_NO);
    cnt++;
  }
  else
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
    {
      size_t speaker_no;
      nvs_get_u32(nvs_handle, "vSpeakerNo", &speaker_no);
      if (speaker_no > 66)
        speaker_no = 3;
      TTS2_SPEAKER_NO = String(speaker_no);
      nvs_close(nvs_handle);
    }
    Serial.println("Startup : NVS vSpeakerNo = " + TTS2_SPEAKER_NO);
    cnt++;
  }
  TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;

  // randomSpeak
  String getStr5 = object["randomSpeak"];
  if ((getStr5 != "") && (getStr5 != "***") && (getStr5 != "null"))
  {
    String getData = getStr5;
    if (getData.equalsIgnoreCase("on"))
      RANDOM_SPEAK_ON_GET = true;
    Serial.println("Startup : randomSpeak = " + getStr5);
    cnt++;
  }
  else
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
    {
      uint8_t randomSpeak_onoff = 0;
      nvs_get_u8(nvs_handle, "randomSpeak", &randomSpeak_onoff);
      nvs_close(nvs_handle);
      if (randomSpeak_onoff == 1)
      {
        RANDOM_SPEAK_ON_GET = true;
      }
      Serial.println("Startup : NVS randomSpeak = " + OffOn[randomSpeak_onoff]);
      cnt++;
    }
  }

  // timer
  String getStr9 = object["timer"];
  if ((getStr9 != "") && (getStr9 != "***") && (getStr9 != "-1") && (getStr9 != "null"))
  {
    int getVal = getStr9.toInt();
    if (getVal <= 30)
      getVal = 30;

    if (getVal >= 3599)
      getVal = 180;

    TIMER_SEC_VALUE = getVal;
    Serial.println("Startup : timer = " + getStr9);
    cnt++;
  }
  else
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
    {
      size_t timer;
      nvs_get_u32(nvs_handle, "timer", &timer);
      if (timer <= 30)
        timer = 30;
      if (timer >= 3599)
        timer = 180;

      TIMER_SEC_VALUE = timer;
      nvs_close(nvs_handle);
      Serial.println("Startup : NVS timer = " + String(timer, DEC));
      cnt++;
    }
  }

  // -----------------------------------------------------------------------------
  Serial.println("** wsStartup.json total " + String(cnt, DEC) + " item read ** ");
  return true;
}

bool setApiKey(String item, String setData, DynamicJsonDocument &apikeyJson)
{
  return (setJsonItem(APIKEY_SPIFFS, item, setData, apikeyJson, "apikey"));
}

bool getApiKey(String item, String &getData, DynamicJsonDocument &apikeyJson)
{
  return (getJsonItem(APIKEY_SPIFFS, item, getData, apikeyJson, "apikey"));
}

bool setStartup(String item, String setData, DynamicJsonDocument &startupJson)
{
  return (setJsonItem(STARTUP_SPIFFS, item, setData, startupJson, "startup"));
}

bool getStartup(String item, String &getData, DynamicJsonDocument &startupJson)
{
  return (getJsonItem(STARTUP_SPIFFS, item, getData, startupJson, "startup"));
}

void toneOn()
{
  M5.Speaker.tone(1000, 100);
}

void tone(int mode)
{
  switch (TONE_MODE)
  {
  case 0: // always toneOff
    break;

  case 1: // toneOn when buttons pressed
    if (mode == TONE_MODE)
      toneOn();
    break;

  case 2: // toneOn whenn external command rcv
    if (mode == TONE_MODE)
      toneOn();
    break;

  case 3: // toneOn every time
    toneOn();
    break;

  default:
    break;
  }
}

void muteOn()
{
  M5.Speaker.setVolume(0);
  M5.Speaker.setChannelVolume(m5spk_virtual_channel, 0);
  MUTE_ON_STATE = true;
}

void muteOff()
{
  M5.Speaker.setVolume(VOLUME_VALUE);
  M5.Speaker.setChannelVolume(m5spk_virtual_channel, VOLUME_VALUE);
  MUTE_ON_STATE = false;
}
