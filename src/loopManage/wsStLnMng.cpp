// ----------------------------<wsStLnMng.cpp>------------------------------------
#include "wsStLnMng.h"

bool statusLineOnOffState = false;
int StatusLineMode;

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
  StatusLineMode = STATUS_MD_IP;
  statusLineOnOffState = false;
  avatar.setStatusLineText("");
  avatar.setStatusLineFont(&fonts::Font0);
  avatar.setBatteryIcon(false);  //  statusLine = off
  
  // 一度balloon表示しないとBatteryIconのフォント設定が反映されない？？ -- by NoRi 240101 --
  avatar.setSpeechText("スタックチャン");
  delay(1000);
  avatar.setSpeechText("");
}

void statusLinePrev()
{
  if (!statusLineOnOffState)
    return;

  if (StatusLineMode == 0)
    StatusLineMode = STATUS_MD_MAX - 1;
  else
    StatusLineMode--;

  StatusLineMode = StatusLineMode % STATUS_MD_MAX;
  setStatusLineMode(StatusLineMode);
}

void statusLineNext()
{
  if (!statusLineOnOffState)
    return;

  StatusLineMode++;
  StatusLineMode = StatusLineMode % STATUS_MD_MAX;
  setStatusLineMode(StatusLineMode);
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

void statusLineOnOff()
{
  if (statusLineOnOffState)
  {
    statusLineOnOffState = false;
    avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
  }
  else
  {
    statusLineOnOffState = true;
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

    default:
      break;
    }
  }
}
