// ----------------------------<wsStLnMng.cpp>------------------------------------
#include "wsStatusMng.h"

// bool statusLineOnOffState = false;
int StatusLineMode;
int statusMode = STM_NONE;
uint32_t statusLineCheck_time = 0;

void statusLineManage()
{
  if ((millis() - statusLineCheck_time) >= DURATION_1013)
  {
    String statusLineMsg = "";
    char s[40];
    statusLineCheck_time = millis();

    switch (StatusLineMode)
    {
    case STATUS_MD_ICON:
    case STATUS_MD_NUM:
      avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());
      return;

    case STATUS_MD_CLOCK:
      int tmSec;
      statusLineMsg = getDateTime(tmSec);
      break;

    case STATUS_MD_RSSI:
      statusLineMsg = "WiFi  Rssi=" + String(WiFi.RSSI()) + "dB   Chan=" + String(WiFi.channel());
      break;

    case STATUS_MD_VOL:
      sprintf(s, "Vol=%3d vSpk=%2d Chara=%d", VOLUME_VALUE, TTS_vSpkNo, CHARA_NO);
      statusLineMsg = String(s);
      break;

    case STATUS_MD_MEM:
      statusLineMsg = getHeapFreeSize();
      break;

    case STATUS_MD_IP:
      statusLineMsg = String(WiFi.localIP().toString()) + "  " + SERVER_NAME;
      break;

    default:
      return;
    }
    avatar.setStatusLineText(statusLineMsg);
  }
}

void statusLineSetup()
{
  // -- batteryStatusLine init Setup ---
  statusMode = STM_NONE;
  StatusLineMode = STATUS_MD_IP;
  SYSINFO_NO =0;

  avatar.setStatusLineText("");
  avatar.setStatusLineFont(&fonts::Font0);
  avatar.setBatteryIcon(false); //  statusLine = off

  
  // 一度balloon表示しないとBatteryIconのフォント設定が反映されない？？ -- by NoRi 240101 --
  avatar.setSpeechText("スタックチャン");
  delay(1000);
  avatar.setSpeechText("");
}

void statusPrev()
{
  if (statusMode == STM_SYSINFO)
  {
    if (SYSINFO_NO == 0)
    {
      SYSINFO_NO = SYSINFO_LEN - 1;
    }
    else
    {
      SYSINFO_NO--;
      SYSINFO_NO = SYSINFO_NO % SYSINFO_LEN;
    }
    sysInfoDispStart(SYSINFO_NO);
  }

  else if (statusMode == STM_LINE)
  {
    if (StatusLineMode == 0)
      StatusLineMode = STATUS_MD_LEN - 1;
    else
      StatusLineMode--;

    StatusLineMode = StatusLineMode % STATUS_MD_LEN;
    setStatusLineMode(StatusLineMode);
  }
}

void statusNext()
{
  if (statusMode == STM_SYSINFO)
  {
    SYSINFO_NO++;
    SYSINFO_NO = SYSINFO_NO % SYSINFO_LEN;
    sysInfoDispStart(SYSINFO_NO);
  }
  else if (statusMode == STM_LINE)
  {
    StatusLineMode++;
    StatusLineMode = StatusLineMode % STATUS_MD_LEN;
    setStatusLineMode(StatusLineMode);
  }
}

void setStatusLineMode(int mode)
{
  avatar.setStatusLineText("");

  switch (mode)
  {
  case STATUS_MD_ICON:
    avatar.setBatteryIcon(true, BATTERY_MD_ICON);
    break;

  case STATUS_MD_NUM:
    avatar.setBatteryIcon(true, BATTERY_MD_NUM);
    break;

  case STATUS_MD_CLOCK:
  case STATUS_MD_RSSI:
  case STATUS_MD_VOL:
    avatar.setStatusLineFont(&fonts::lgfxJapanGothicP_12);
    avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
    break;

  case STATUS_MD_MEM:
  case STATUS_MD_IP:
    avatar.setStatusLineFont(&fonts::Font0);
    avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
    break;

  default:
    break;
  }
}

void statusModeSelect()
{
  statusMode++;
  statusMode = statusMode % STM_LEN;
  Serial.println("statusMode = " + String(statusMode, DEC));

  switch (statusMode)
  {
  case STM_NONE:
    sysInfoDispEnd();
    // statusLineOnOffState = false;
    avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
    break;

  case STM_LINE:
    // statusLineOnOffState = true;
    switch (StatusLineMode)
    {
    case STATUS_MD_ICON:
      avatar.setBatteryIcon(true, BATTERY_MD_ICON);
      break;

    case STATUS_MD_NUM:
      avatar.setBatteryIcon(true, BATTERY_MD_NUM);
      break;

    case STATUS_MD_CLOCK:
    case STATUS_MD_RSSI:
    case STATUS_MD_VOL:
    case STATUS_MD_MEM:
    case STATUS_MD_IP:
      avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
      break;
    }
    break;

  case STM_SYSINFO:
    avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
    delay(10);
    sysInfoDispStart(SYSINFO_NO);
    break;

  default:
    break;
  }
}

