// ---------------------------< wsSysInfo.cpp >------------------------------------
#include "wsSysInfo.h"

using namespace m5avatar;
extern Avatar avatar;


void wsHandleSysInfo(String txS, String dispS, String modeS)
{
  SYSINFO_MSG = "";

  if(txS=="" && dispS=="" && modeS=="")
  {
      sysInfoDispMake(0);
      SYSINFO_MSG += "\n";
      sysInfoDispMake(1);
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
      sysInfoDispEnd();
      webpage = "OK";
    }
    else if (dispS == "on")
    {
      // uint8_t mode_no =(uint8_t) modeS.toInt();
      // sysInfoDispStart(mode_no);
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
      SYSINFO_MSG += "\n";
      sysInfoDispMake(1);
      webpage = SYSINFO_MSG;
    }
    else
    {
      uint8_t mode_no =(uint8_t) modeS.toInt();
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
    txData = WS_VERSION;
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
  else if (txArg == "vSpeakerNo" || txArg =="vSpkNo")
  {
    sprintf(msg2, "vSpkNo = %d", TTS_vSpkNo);
    txData = msg2;
  }
  else if (txArg == "timer")
  {
    sprintf(msg2, "timer = %d sec", TIMER_SEC_VALUE);
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
  if (!SYSINFO_DISP_STATE)
  {
    avatar.suspend();
    Serial.println("avatar suspended");

    randomSpeakStop2();
    timerStop2();
    M5.Display.setTextFont(1);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.setTextDatum(0);
  }

  M5.Display.setCursor(0, 0);
  delay(50);
  M5.Display.fillScreen(BLACK);
  delay(50);

  sysInfoDispMake(mode_no);
  M5.Display.print(SYSINFO_MSG);

  if (mode_no == 99)
  { // Test mode Display
    // errStop("EX_errFatal_STOP Called !!");
    // reboot("EX_errFatal_REBOOT Called !!");
  }
  SYSINFO_DISP_STATE = true;
}

void sysInfoDispEnd()
{
  if (!SYSINFO_DISP_STATE)
  {
    return;
  }

  // avatar.start();
  avatar.resume();
  Serial.println("avatar resumed");

  delay(200);
  // muteOff();
  SYSINFO_DISP_STATE = false;
}

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

  case 99:
    sysInfo_m99_DispMake();
    break;

  default:
    sysInfo_m00_DispMake();
    break;
  }
}

void sysInfo_m01_DispMake()
{
  String msg = "";
  char msg2[100];

  SYSINFO_MSG += "\nSSID_PASSWD = " + SSID_PASSWD;
  SYSINFO_MSG += "\nopenAiApiKey = " + OPENAI_API_KEY;
  // SYSINFO_MSG += "\nvoiceTextApiKey = " + VOICETEXT_API_KEY;
  SYSINFO_MSG += "\nvoicevoxApiKey = " + VOICEVOX_API_KEY;
  // SYSINFO_MSG += "\nsttApiKey = " + STT_API_KEY;
  SYSINFO_MSG += "\ntimer = " + String(TIMER_SEC_VALUE,DEC) + "sec";

  if (MUTE_ON_STATE)
    SYSINFO_MSG += "\nmute = on";
  else
    SYSINFO_MSG += "\nmute = off";

  if (SV_USE)
    SYSINFO_MSG += "\nservo = on";
  else
    SYSINFO_MSG += "\nservo = on";

  SYSINFO_MSG += "\nservoPort = " + SV_PORT;
  SYSINFO_MSG += "\nservoMode = " + SV_MD_NAME[SV_MD_NAME_NO] ;
  SYSINFO_MSG += "\nservoHomeX = " + String(SV_HOME_X,DEC);
  SYSINFO_MSG += "\nservoHomeY = " + String(SV_HOME_Y,DEC);

  uint32_t uptime = millis() / 1000;
  uint16_t up_sec = uptime % 60;
  uint16_t up_min = (uptime / 60) % 60;
  uint16_t up_hour = uptime / (60 * 60);
  sprintf(msg2, "\nuptime = %02d:%02d:%02d", up_hour, up_min, up_sec);
  SYSINFO_MSG += msg2;
}


void sysInfo_m00_DispMake()
{
  String msg = "";
  char msg2[100];

  // SYSINFO_MSG = "*** System Information ***\n";
  SYSINFO_MSG = "";
  SYSINFO_MSG += WS_VERSION;
  SYSINFO_MSG += "\nIP_Addr = " + IP_ADDR;
  SYSINFO_MSG += "\nSSID = " + SSID;
  SYSINFO_MSG += "\nserverName = " + SERVER_NAME ;

  sprintf(msg2, "\nbatteryLevel = %d %%", getBatteryLevel());
  SYSINFO_MSG += msg2;

  sprintf(msg2, "\nvolume = %d", VOLUME_VALUE);
  SYSINFO_MSG += msg2;
  // SYSINFO_MSG += "\nttsSelect = " + String(TTS_NAME[TTS_TYPE]);
  
  // SYSINFO_MSG += "\nvSpkNo = " + TTS_SPEAKER_NO;
  SYSINFO_MSG += "\nvSpkNo = " + String(TTS_vSpkNo,DEC);
  
  // SYSINFO_MSG += "\nlang = " + LANG_CODE;

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

void sysInfo_m99_DispMake()
{
  SYSINFO_MSG = "*** Test Mode  *** ";
}
