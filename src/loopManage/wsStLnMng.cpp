// ----------------------------<wsStLnMng.cpp>------------------------------------
#include "../h/wsStLnMng.h"

bool statusLineOnOffState = false;
#define STATUS_MD_ICON 0
#define STATUS_MD_NUM 1
#define STATUS_MD_CLOCK 2
#define STATUS_MD_RSSI 3
#define STATUS_MD_VOL 4
#define STATUS_MD_MEM 5
#define STATUS_MD_IP 6
#define STATUS_MD_MAX 7
int StatusLineMode;
#define DURATION_1013 1013UL // 1.013秒: statusLineCheck_time
uint32_t statusLineCheck_time = 0;

void statusLineSetup()
{
// -- batteryStatusLine setup ---
  StatusLineMode = STATUS_MD_IP;
  statusLineOnOffState = false;
  avatar.setStatusLineText("");
  avatar.setStatusLineFont(&fonts::Font0);
  avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
  // avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());

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

      // if ((tmSec % 10) == 0)
      // {
      //   // Serial.println("tmSec = " + String(tmSec, DEC));
      //   switch (tmSec)
      //   {
      //   case 0:
      //   case 30:
      //     ledClear();
      //     ledShow();
      //     break;

      //   case 10:
      //     ledSetColor(rLED[0], 0, 155, 0);
      //     ledShow();
      //     break;

      //   case 20:
      //     ledSetColor(rLED[0], 0, 0, 0);
      //     ledSetColor(rLED[3], 0, 155, 0);
      //     ledShow();
      //     break;

      //   case 40:
      //     ledSetColor(lLED[3], 0, 155, 0);
      //     ledShow();
      //     break;

      //   case 50:
      //     ledSetColor(lLED[3], 0, 0, 0);
      //     ledSetColor(lLED[0], 0, 155, 0);
      //     ledShow();
      //     break;
      //   }
      //   break;
      // }
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
