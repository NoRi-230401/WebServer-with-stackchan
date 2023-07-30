// ---------------------------< wsSetting.cpp >------------------------------------
#include "wsSetting.h"

const char *SETTING_NVS = "setting"; // setting --NVS の設定用ファイル
const char *APIKEY_NVS = "apikey";   // apikey  -- NVS の設定用ファイル

const String APIKEY_SPIFFS = "/wsApiKey.json";
const String APIKEY_TXT_SD = "/apikey.txt";
const String STARTUP_SPIFFS = "/wsStartup.json";
// const String STARTUP_ITEM[] = {
//     "ttsSelect", "voicevoxSpeakerNo", "lang", "volume",
//     "led", "randomSpeak", "toneMode", "mute", "keyLock", "timer"};
const String STARTUP_ITEM[] = {
    "serverName", "voicevoxSpeakerNo", "lang", "volume",
    "led", "randomSpeak", "toneMode", "mute", "keyLock", "timer"};

const String APIKEY_ITEM[] = {"openAiApiKey", "voiceTextApiKey", "voicevoxApiKey"};

size_t VOLUME_VALUE;
bool MUTE_ON_STATE = false;
uint8_t TONE_MODE = 1; // 0:allOff 1:buttonOn 2:extCommOn 3:allOn
String SYSINFO_MSG = "";
String IP_ADDR = "";
String SSID = "";
String SSID_PASSWD = "";
String OPENAI_API_KEY = "";
String VOICEVOX_API_KEY = "";
String VOICETEXT_API_KEY = "";
String STT_API_KEY = "";
extern String webpage;

void wsHandleSetting2(String langS, String ttsNameS, String muteS, String keyLockS, String toneModeS)
{
  uint32_t nvs_handle;

  // ---- lang -------
  if (langS != "")
  {
    LANG_CODE = langS;

    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    {
      nvs_set_str(nvs_handle, "lang", (char *)langS.c_str());
    }
    nvs_close(nvs_handle);

    webpage = "lang = " + LANG_CODE;
  }

  // ---- ttsSelect -------
  if (ttsNameS != "")
  {
    ttsNameS.toUpperCase();
    // if (ttsName_str == TTS_NAME[0])
    if (ttsNameS == "VOICETEXT")
      TTS_TYPE = 0;
    else if (ttsNameS == "VOICEVOX")
      TTS_TYPE = 2;
    else
      TTS_TYPE = 1;

    String ttsName_tmp = TTS_NAME[TTS_TYPE];
    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    {
      nvs_set_str(nvs_handle, "ttsSelect", (char *)ttsName_tmp.c_str());
    }
    nvs_close(nvs_handle);
    webpage = "ttsName = " + ttsName_tmp;
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

    webpage = "mute = " + muteS;
  }

  // ---- keyLock -------
  if (keyLockS != "")
  {
    keyLockS.toLowerCase();
    if (keyLockS == "on")
    {
      KEYLOCK_STATE = true;
      if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
        nvs_set_str(nvs_handle, "keyLock", "on");
      nvs_close(nvs_handle);
    }
    else if (keyLockS == "off")
    {
      KEYLOCK_STATE = false;
      if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
        nvs_set_str(nvs_handle, "keyLock", "off");
      nvs_close(nvs_handle);
    }
    Serial.println("setting?keyLock = " + keyLockS);
    webpage = "keyLock = " + keyLockS;
  }

  // ---- toneMode -------
  if (toneModeS != "")
  {
    if (toneModeS == "next")
      TONE_MODE = (TONE_MODE + 1) % 4;
    else
    {
      TONE_MODE = toneModeS.toInt();
      if ((TONE_MODE < 0) || (TONE_MODE > 3))
      {
        TONE_MODE = 1;
      }
    }

    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    {
      nvs_set_u32(nvs_handle, "toneMode", TONE_MODE);
    }
    nvs_close(nvs_handle);

    webpage = "toneMode = " + String(TONE_MODE, DEC);
  }

  Serial.println(webpage);
}

void wsHandleSetting(String volumeS, String volumeDS, String speakerS, String ledS)
{
  uint32_t nvs_handle;

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
    {
      nvs_set_u32(nvs_handle, "volume", VOLUME_VALUE);
    }
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
    uint8_t led_onoff;
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
      led_onoff = 0;
    }
    else if (ledStr == "on")
    {
      LED_OnOff_STATE = true;
      led_onoff = 1;
    }

    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    {
      nvs_set_u8(nvs_handle, "led", led_onoff);
    }
    nvs_close(nvs_handle);

    webpage = "led = " + ledStr;
  }

  // ---- Speaker -------
  size_t speaker_no;
  if (speakerS != "")
  {
    speaker_no = speakerS.toInt();
    if (speaker_no > 66)
    {
      speaker_no = 3;
    }
    TTS2_SPEAKER_NO = String(speaker_no);
    TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;

    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    {
      nvs_set_u8(nvs_handle, "speaker", speaker_no);
      Serial.print("NVS Write : speaker_no = ");
      Serial.println(speaker_no, DEC);
    }
    nvs_close(nvs_handle);
    webpage = "speaker = " + String(speaker_no, DEC);
  }
  Serial.println(webpage);
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

  if (!SPIFFS.begin(true))
  {
    Serial.println("Error preparing SPIFFS Filing System...");
    // StartupErrors = true;
  }

  if (!SD_begin())
  {
    Serial.println("Error preparing SD Filing System...");
  }
}

// void NVS_setting(void)
// {
//   uint32_t nvs_handle;
//   if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
//   {
//     size_t volume;
//     uint8_t led_onoff;
//     uint8_t speaker_no;
//     nvs_get_u32(nvs_handle, "volume", &volume);
//     if (volume > 255)
//       volume = 255;
//     M5.Speaker.setVolume(volume);
//     M5.Speaker.setChannelVolume(m5spk_virtual_channel, volume);
//     nvs_get_u8(nvs_handle, "led", &led_onoff);
//     // if(led_onoff == 1) Use_LED = true;
//     // else  Use_LED = false;
//     nvs_get_u8(nvs_handle, "speaker", &speaker_no);
//     if (speaker_no > 60)
//       speaker_no = 3;
//     TTS2_SPEAKER_NO = String(speaker_no);
//     TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;
//     // nvs_close(nvs_handle);
//   }
//   else
//   {
//     if (ESP_OK == nvs_open("setting", NVS_READWRITE, &nvs_handle))
//     {
//       size_t volume = 180;
//       uint8_t led_onoff = 0;
//       uint8_t speaker_no = 3;
//       nvs_set_u32(nvs_handle, "volume", volume);
//       nvs_set_u8(nvs_handle, "led", led_onoff);
//       nvs_set_u8(nvs_handle, "speaker", speaker_no);
//       nvs_close(nvs_handle);
//       M5.Speaker.setVolume(volume);
//       M5.Speaker.setChannelVolume(m5spk_virtual_channel, volume);
//       // Use_LED = false;
//       TTS2_SPEAKER_NO = String(speaker_no);
//       TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;
//     }
//   }
//   nvs_close(nvs_handle);
// }

// void NVS_apikey()
// {
//   uint32_t nvs_handle;
//   if (ESP_OK == nvs_open("apikey", NVS_READONLY, &nvs_handle))
//   {
//     Serial.println("nvs_open");

//     size_t length1;
//     size_t length2;
//     size_t length3;
//     if (ESP_OK == nvs_get_str(nvs_handle, "openai", nullptr, &length1) &&
//         ESP_OK == nvs_get_str(nvs_handle, "voicevox", nullptr, &length2) &&
//         ESP_OK == nvs_get_str(nvs_handle, "sttapikey", nullptr, &length3) &&
//         length1 && length2 && length3)
//     {
//       Serial.println("nvs_get_str");
//       char openai_apikey[length1 + 1];
//       char voicevox_apikey[length2 + 1];
//       char stt_apikey[length3 + 1];
//       if (ESP_OK == nvs_get_str(nvs_handle, "openai", openai_apikey, &length1) &&
//           ESP_OK == nvs_get_str(nvs_handle, "voicevox", voicevox_apikey, &length2) &&
//           ESP_OK == nvs_get_str(nvs_handle, "sttapikey", stt_apikey, &length3))
//       {
//         OPENAI_API_KEY = String(openai_apikey);
//         VOICEVOX_API_KEY = String(voicevox_apikey);
//         // STT_API_KEY = String(stt_apikey);
//         STT_API_KEY = String(openai_apikey);
//         // Serial.println(OPENAI_API_KEY);
//         // Serial.println(VOICEVOX_API_KEY);
//         // Serial.println(STT_API_KEY);
//       }
//     }
//     // nvs_close(nvs_handle);
//   }
//   nvs_close(nvs_handle);
// }

const String jsonAPIKEY = "{\"apikey\":[{\"openAiApiKey\":\"***\",\"voicevoxApiKey\":\"***\"}]}";

bool jsonAPIKEYinit(DynamicJsonDocument &jsonDoc)
{
  return (jsonInitSave(jsonDoc, jsonAPIKEY, APIKEY_SPIFFS));
}

bool apiKeyTxtRead()
{
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
  String getStr0 = object[APIKEY_ITEM[0]];
  if (getStr0 != "" && (getStr0 != "null"))
  {
    OPENAI_API_KEY = getStr0;
    STT_API_KEY = getStr0;
    Serial.println("ApiKey : " + APIKEY_ITEM[0] + " = " + getStr0);
    cnt++;
  }

  // voiceTextApiKey
  // String getStr1 = object[APIKEY_ITEM[1]];
  // if (getStr1 != "" && (getStr1 != "null"))
  // {
  //   VoiceText_TTS_USER = getStr1;
  //   VOICETEXT_API_KEY = VoiceText_TTS_USER;
  //   Serial.println("ApiKey : " + APIKEY_ITEM[1] + " = " + getStr1);
  //   cnt++;
  // }

  // voicevoxApiKey
  String getStr2 = object[APIKEY_ITEM[2]];
  if (getStr2 != "" && (getStr2 != "null"))
  {
    VOICEVOX_API_KEY = getStr2;
    Serial.println("ApiKey : " + APIKEY_ITEM[2] + " = " + getStr2);
    cnt++;
  }

  Serial.println("** wsApiKey.json total " + String(cnt, DEC) + " item read **");
  return true;
}

const String jsonSTARTUP = "{\"startup\":[{\"serverName\":\"stackchan\",\"voicevoxSpeakerNo\":\"-1\",\"volume\":\"-1\",\"led\":\"on\",\"randomSpeak\":\"off\",\"toneMode\":\"1\",\"mute\":\"off\",\"keyLock\":\"off\",\"timer\":\"180\"}]}";

bool jsonSTARTUPinit(DynamicJsonDocument &jsonDoc)
{
  return (jsonInitSave(jsonDoc, jsonSTARTUP, STARTUP_SPIFFS));
}

// const String jsonSERVO = "{\"servo\":[{\"servo\":\"on\",\"servoPort\":\"portC\",\"servoMode\":\"home\",\"servoHomeX\":\"90\",\"servoHomeY\":\"80\"}]}";

// bool jsonSERVOinit(DynamicJsonDocument &jsonDoc)
// {
//   return (jsonInitSave(jsonDoc,jsonSERVO,SERVO_SPIFFS));
// }

bool startupFileRead()
{
  // ****** 初期値設定　**********
  SERVER_NAME = "stackchan";
  TTS_TYPE = 2; // VOICEVOX
  TTS2_SPEAKER_NO = "3";
  LANG_CODE = String(LANG_CODE_JP);
  VOLUME_VALUE = 180;
  LED_OnOff_STATE = true;
  RANDOM_TIME = -1;
  RANDOM_SPEAK_ON_GET = false;
  RANDOM_SPEAK_STATE = false;
  TONE_MODE = 1;
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

  // ttsSelect
  // String getStr0 = object[STARTUP_ITEM[0]];
  // if (getStr0 != "" && (getStr0 != "null"))
  // {
  //   String getData = getStr0;
  //   getData.toUpperCase();
  //   if (getData == "VOICETEXT")
  //     TTS_TYPE = 0;
  //   if (getData == "GOOGLETTS")
  //     TTS_TYPE = 1;
  //   if (getData == "VOICEVOX")
  //     TTS_TYPE = 2;

  //   Serial.println("Startup : " + STARTUP_ITEM[0] + " = " + TTS_NAME[TTS_TYPE]);
  //   cnt++;
  // }

  // serverName
  String getStr0 = object[STARTUP_ITEM[0]];
  if (getStr0 != "" && (getStr0 != "null"))
  {
    String getData = getStr0;
    SERVER_NAME = getData;
    Serial.println("Startup : " + STARTUP_ITEM[0] + " = " + SERVER_NAME);
    cnt++;
  }

  // --- SPEAKER ---
  String getStr1 = object[STARTUP_ITEM[1]];
  if ((getStr1 != "") && (getStr1 != "-1") && (getStr1 != "null"))
  {
    TTS2_SPEAKER_NO = getStr1;
    Serial.println("Startup : " + STARTUP_ITEM[1] + " = " + getStr1);
    cnt++;
  }
  else
  {
    uint8_t speaker_no;
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open("setting", NVS_READONLY, &nvs_handle))
    {
      nvs_get_u8(nvs_handle, "speaker", &speaker_no);
      if (speaker_no > 66)
        speaker_no = 3;
      TTS2_SPEAKER_NO = String(speaker_no);
      nvs_close(nvs_handle);
    }
    Serial.println("Startup : NVS " + STARTUP_ITEM[1] + " = " + String(speaker_no, DEC));
    cnt++;
  }
  TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;

  // lang
  // String getStr2 = object[STARTUP_ITEM[2]];
  // if (getStr2 != "" && (getStr2 != "null"))
  // {
  //   LANG_CODE = getStr2;
  //   Serial.println("Startup : " + STARTUP_ITEM[2] + " = " + getStr2);
  //   cnt++;
  // }

  // volume
  String getStr3 = object[STARTUP_ITEM[3]];
  if ((getStr3 != "") && (getStr3 != "-1") && (getStr3 != "null"))
  {
    int getVal = getStr3.toInt();
    if (getVal < 0)
      getVal = 0;
    if (getVal > 255)
      getVal = 255;

    VOLUME_VALUE = (size_t)getVal;
    Serial.println("Startup : " + STARTUP_ITEM[3] + " = " + getStr3);
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
      Serial.println("Startup : NVS " + STARTUP_ITEM[3] + " = " + String(volume, DEC));
      cnt++;
    }
  }
  M5.Speaker.setVolume(VOLUME_VALUE);
  M5.Speaker.setChannelVolume(m5spk_virtual_channel, VOLUME_VALUE);

  // led
  String getStr4 = object[STARTUP_ITEM[4]];
  if (getStr4 != "" && (getStr4 != "null"))
  {
    String getData = getStr4;
    getData.toLowerCase();
    if (getData == "off")
    {
      LED_OnOff_STATE = false;
      // led_onoff = 0;
    }
    Serial.println("Startup : " + STARTUP_ITEM[4] + " = " + getStr4);
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
      Serial.println("Startup : NVS " + STARTUP_ITEM[4] + " = " + getStr4);
      cnt++;
    }
  }

  // randomSpeak
  String getStr5 = object[STARTUP_ITEM[5]];
  if (getStr5 != "" && (getStr5 != "null"))
  {
    String getData = getStr5;
    getData.toLowerCase();
    if (getData == "on")
      RANDOM_SPEAK_ON_GET = true;
    Serial.println("Startup : " + STARTUP_ITEM[5] + " = " + getStr5);
    cnt++;
  }

  // toneMode
  String getStr6 = object[STARTUP_ITEM[6]];
  if (getStr6 != "" && (getStr6 != "-1") && (getStr6 != "null"))
  {
    int getVal = getStr6.toInt();
    if (getVal < 0 || getVal > 3)
      getVal = 1;
    TONE_MODE = getVal;
    Serial.println("Startup : " + STARTUP_ITEM[6] + " = " + getStr6);
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
        mode = 1;
      TONE_MODE = mode;
      nvs_close(nvs_handle);
      Serial.println("Startup : NVS " + STARTUP_ITEM[6] + " = " + String(mode, DEC));
      cnt++;
    }
  }

  // mute
  String getStr7 = object[STARTUP_ITEM[7]];
  if (getStr7 != "" && (getStr7 != "null"))
  {
    String getData = getStr7;
    getData.toLowerCase();
    if (getData == "on")
      muteOn();
    Serial.println("Startup : " + STARTUP_ITEM[7] + " = " + getStr7);
    cnt++;
  }

  // keyLock
  String getStr8 = object[STARTUP_ITEM[8]];
  if ((getStr8 != "") && (getStr8 != "null"))
  {
    String getData = getStr8;
    getData.toLowerCase();
    if (getData == "on")
      KEYLOCK_STATE = true;
    Serial.println("Startup : " + STARTUP_ITEM[8] + " = " + getStr8);
    cnt++;
  }
  else
  {
    Serial.println("Error " + STARTUP_ITEM[8] + " is VOID or NULL");
  }

  // timer
  String getStr9 = object[STARTUP_ITEM[9]];
  if (getStr9 != "" && (getStr9 != "null"))
  {
    int getVal = getStr9.toInt();
    if (getVal <= 30)
      getVal = 30;

    if (getVal >= 3599)
      getVal = 180;

    TIMER_SEC_VALUE = getVal;
    Serial.println("Startup : " + STARTUP_ITEM[9] + " = " + getStr9);
    cnt++;
  }

  Serial.println("** wsStartup.json total " + String(cnt, DEC) + " item read ** ");
  return true;
}

// void wsHandleStartup(String ttsSelectS, String vvoxSpeakerNoS, String langS,
//                      String volumeS, String ledS, String randomSpeakS, String toneModeS,
//                      String muteS, String keyLockS, String timerS, String txS)

void wsHandleStartup(String serverNameS, String vvoxSpeakerNoS, String volumeS, String ledS,
                     String randomSpeakS, String toneModeS, String muteS, String keyLockS, String timerS, String txS)
{
  DynamicJsonDocument startupJson(STARTUPJSON_SIZE);

  // -------------------------------------------------------
  if (txS != "")
  {
    String getStr = "";
    if (getStartup(txS, getStr, startupJson))
      webpage = "wsStartup.json : " + txS + " = " + getStr;
    return;
  }

  // if(ttsSelectS !="")
  // {
  //   if(setStartup("ttsSelect",ttsSelectS,startupJson))
  //     webpage = "wsStartup.Json : ttsSlect = " + ttsSelectS ;
  //   return;
  // }

  if (serverNameS != "")
  {
    if (setStartup("serverName", serverNameS, startupJson))
      webpage = "wsStartup.Json : serverName = " + serverNameS;
    return;
  }

  if (vvoxSpeakerNoS != "")
  {
    if (setStartup("voicevoxSpeakerNo", vvoxSpeakerNoS, startupJson))
      webpage = "wsStartup.Json : voicevoxSpeakerNo = " + vvoxSpeakerNoS;
    return;
  }

  // if(langS !="")
  // {
  //   if(setStartup("lang",langS,startupJson))
  //     webpage = "wsStartup.Json : lang = " + langS ;
  //   return;
  // }

  if (volumeS != "")
  {
    if (setStartup("volume", volumeS, startupJson))
      webpage = "wsStartup.Json : volume = " + volumeS;
    return;
  }

  if (ledS != "")
  {
    if (setStartup("led", ledS, startupJson))
      webpage = "wsStartup.Json : led = " + ledS;
    return;
  }

  if (randomSpeakS != "")
  {
    if (setStartup("randomSpeak", randomSpeakS, startupJson))
      webpage = "wsStartup.Json : randomSpeak = " + randomSpeakS;
    return;
  }

  if (toneModeS != "")
  {
    if (setStartup("toneMode", toneModeS, startupJson))
      webpage = "wsStartup.Json : toneMode = " + toneModeS;
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
      webpage = "wsStartup.Json : keyLock = " + keyLockS;
    return;
  }

  if (timerS != "")
  {
    if (setStartup("timer", timerS, startupJson))
      webpage = "wsStartup.Json : timer = " + timerS;
    return;
  }

  // -------------------------------------------------------
  // HTML形式でファイルを表示する
  {
    if (!jsonRead(FLTYPE_SPIFFS, startupJson, STARTUP_SPIFFS))
    {
      Serial.println("faile to Read wsStartup.json in SPIFFS");
      return;
    }

    String html = "";
    serializeJsonPretty(startupJson, html);
    Serial.println(html);
    webpage = html;
    return;
  }
}

void wsHandleApikeySetting(String openAiS, String voicevoxS, String voiceTextS, String txS)
{
  DynamicJsonDocument apikeyJson(APIKEYJSON_SIZE);

  if (openAiS == "" && voicevoxS == "" && voiceTextS == "" && txS == "")
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
      webpage = "wsApikey.json : voicevoxApiKey = " + voicevoxS;
      Serial.println(webpage);
    }
    return;
  }

  // -------------------------------------------------------
  if (voiceTextS != "")
  {
    if (setApiKey("voiceTextApiKey", voiceTextS, apikeyJson))
    {
      webpage = "wsApikey.json : voiceTextApiKey = " + voiceTextS;
      Serial.println(webpage);
    }
    return;
  }

  return;
}

bool setApiKey(String item, String setData, DynamicJsonDocument &apikeyJson)
{
  return (setJsonItem(APIKEY_SPIFFS, item, setData, apikeyJson, "apikey"));
}

bool setStartup(String item, String setData, DynamicJsonDocument &startupJson)
{
  return (setJsonItem(STARTUP_SPIFFS, item, setData, startupJson, "startup"));
}

bool getStartup(String item, String &getData, DynamicJsonDocument &startupJson)
{
  return (getJsonItem(STARTUP_SPIFFS, item, getData, startupJson, "startup"));
}

bool getApiKey(String item, String &getData, DynamicJsonDocument &apikeyJson)
{
  return (getJsonItem(APIKEY_SPIFFS, item, getData, apikeyJson, "apikey"));
}

void toneOn()
{
  // M5.Mic.end();
  M5.Speaker.tone(1000, 100);
  // delay(500);
  // M5.Speaker.end();
  // M5.Mic.begin();
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
