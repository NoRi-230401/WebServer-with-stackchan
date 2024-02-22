// ----------------------------<wsStLnMng.cpp>------------------------------------
#include "wsStatusMng.h"

// bool statusLineOnOffState = false;
int StatusLineMode;
int statusMode = STM0X_NONE;
uint32_t statusLineCheck_time = 0;




void statusModeSelect()
{
  statusMode++;
  statusMode = statusMode % STM_LEN;
  Serial.println("statusMode = " + String(statusMode, DEC));

  switch (statusMode)
  {
  case STM0X_NONE:
    avatarResume();
    avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
    break;

  case STM1X_LINE:
    switch (StatusLineMode)
    {
    case STM10_ICON:
      avatar.setBatteryIcon(true, BATTERY_MD_ICON);
      break;

    case STM11_NUM:
      avatar.setBatteryIcon(true, BATTERY_MD_NUM);
      break;

    case STM12_CLOCK:
    case STM13_RSSI:
    case STM14_VOL:
    case STM15_MEM:
    case STM16_IP:
      avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
      break;
    }
    break;

  case STM2X_SYSINFO:
    avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
    delay(10);
    avatarStop();
    avatarStop2();
    sysInfoDispStart(SYSINFO_NO);
    break;

  case STM3X_SETTING:
    // avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
    // delay(10);
    // avatarStop();
    SDU_disp();
    break;

  default:
    break;
  }
}

void SDU_disp()
{
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextDatum(0);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.print( "**  SD-Updater  **\n\n");
  M5.Lcd.print( WSS_NAME.c_str());
  M5.Lcd.print("\n\n\nBtnA: lobby screen appear\n\n");
  M5.Lcd.print("BtnB: load menu binary\n\n");
  M5.Lcd.print("BtnC: store BIN file to SD\n");
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
    case STM10_ICON:
    case STM11_NUM:
      avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());
      return;

    case STM12_CLOCK:
      int tmSec;
      statusLineMsg = getDateTime(tmSec);
      break;

    case STM13_RSSI:
      statusLineMsg = "WiFi  Rssi=" + String(WiFi.RSSI()) + "dB   Chan=" + String(WiFi.channel());
      break;

    case STM14_VOL:
      sprintf(s, "Vol=%3d vSpk=%2d Chara=%d", VOLUME_VALUE, TTS_vSpkNo, CHARA_NO);
      statusLineMsg = String(s);
      break;

    case STM15_MEM:
      statusLineMsg = getHeapFreeSize();
      break;

    case STM16_IP:
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
  statusMode = STM0X_NONE;
  StatusLineMode = STM16_IP;
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
  if (statusMode == STM2X_SYSINFO)
  {
    if (SYSINFO_NO == 0)
    {
      SYSINFO_NO = STM2X_SYSINFO_LEN - 1;
    }
    else
    {
      SYSINFO_NO--;
      SYSINFO_NO = SYSINFO_NO % STM2X_SYSINFO_LEN;
    }
    sysInfoDispStart(SYSINFO_NO);
  }

  else if (statusMode == STM1X_LINE)
  {
    if (StatusLineMode == 0)
      StatusLineMode = STM1X_LINE_LEN - 1;
    else
      StatusLineMode--;

    StatusLineMode = StatusLineMode % STM1X_LINE_LEN;
    setStatusLineMode(StatusLineMode);
  }
}

void statusNext()
{
  if (statusMode == STM2X_SYSINFO)
  {
    SYSINFO_NO++;
    SYSINFO_NO = SYSINFO_NO % STM2X_SYSINFO_LEN;
    sysInfoDispStart(SYSINFO_NO);
  }
  else if (statusMode == STM1X_LINE)
  {
    StatusLineMode++;
    StatusLineMode = StatusLineMode % STM1X_LINE_LEN;
    setStatusLineMode(StatusLineMode);
  }
}

void setStatusLineMode(int mode)
{
  avatar.setStatusLineText("");

  switch (mode)
  {
  case STM10_ICON:
    avatar.setBatteryIcon(true, BATTERY_MD_ICON);
    break;

  case STM11_NUM:
    avatar.setBatteryIcon(true, BATTERY_MD_NUM);
    break;

  case STM12_CLOCK:
  case STM13_RSSI:
  case STM14_VOL:
    avatar.setStatusLineFont(&fonts::lgfxJapanGothicP_12);
    avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
    break;

  case STM15_MEM:
  case STM16_IP:
    avatar.setStatusLineFont(&fonts::Font0);
    avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
    break;

  default:
    break;
  }
}

