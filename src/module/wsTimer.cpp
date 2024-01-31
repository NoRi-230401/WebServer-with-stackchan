// ----------------------------<wsTimer.cpp>------------------------------------
#include "../h/wsTimer.h"

uint32_t TM_SEC_VAL = TM_INIT; // Timer時間設定(sec)
bool TM_STARTED = false;
static uint32_t TM_START_MILLIS = 0;
static uint16_t TM_ELEAPSE_SEC = 0;
bool TM_STOP_GET = false;
bool TM_GO_GET = false;

void wsHandleTimer(String TmSecS, String TmMinS, String timerModeS)
{
  if ((TmSecS != "") || (TmMinS != ""))
  {
    int timerSec = 0;
    int timerMin = 0;

    if (TmSecS != "")
      timerSec = TmSecS.toInt();

    if (TmMinS != "")
      timerMin = TmMinS.toInt();

    TM_SEC_VAL = (timerMin * 60) + timerSec;

    if (TM_SEC_VAL > TM_MAX)
      TM_SEC_VAL = TM_MAX;

    if (TM_SEC_VAL < TM_MIN)
      TM_SEC_VAL = TM_MIN;

    // NVS に保存
    uint32_t nvs_handle;
    size_t timer_value = (size_t)TM_SEC_VAL;
    if (ESP_OK == nvs_open("setting", NVS_READWRITE, &nvs_handle))
    {
      nvs_set_u32(nvs_handle, "timer", timer_value);
    }
    nvs_close(nvs_handle);

    TM_STOP_GET = false;
    TM_GO_GET = false;
    webpage = "timer = " + String(TM_SEC_VAL, DEC) + "sec";
  }

  if (timerModeS != "")
  {
    timerModeS.toUpperCase();

    if (timerModeS == "START" || timerModeS == "GO")
    {
      if (!TM_STARTED)
      {
        if (SYSINFO_DISP_STATE)
          sysInfoDispEnd();

        randomSpeakStop2();
        TM_GO_GET = true;
        TM_STOP_GET = false;
        webpage = "timer Start : " + String(TM_SEC_VAL, DEC) + "sec";
      }
      else
      {
        webpage = "timer has already started";
      }
      Serial.println(webpage);
      return;
    }
    else if (timerModeS == "STOP")
    {
      if (TM_STARTED)
      {
        TM_STOP_GET = true;
        TM_GO_GET = false;
        webpage = "timer Stop";
      }
      else
      {
        webpage = "timer has already stopped";
      }
      Serial.println(webpage);
      return;
    }
  }
}

void timerManage()
{
  if (TM_STARTED)
  { // Timer 起動中
    if (SYSINFO_DISP_STATE)
      sysInfoDispEnd();

    uint32_t elapsedTimeMillis = millis() - TM_START_MILLIS;
    uint16_t currentElapsedSeconds =(uint16_t)(elapsedTimeMillis / 1000);

    if (currentElapsedSeconds >= TM_SEC_VAL)
    { // 指定時間が経過したら終了
      timerEnd();
    }
    // else if (TM_STOP_GET && !isTalking())
    else if (TM_STOP_GET )
    { // ---Timer停止---
      timerStop();
    }
    else if (currentElapsedSeconds != TM_ELEAPSE_SEC)
    { // --- Timer途中経過の処理------
      TM_ELEAPSE_SEC = currentElapsedSeconds;
      timerStarted();
    }
  }
  else
  {
    if (TM_GO_GET && !isTalking())
      timerStart();
  }
}

void timerStop2()
{
  // --- Timer を途中で停止 ------
  TM_STARTED = false;
  TM_GO_GET = false;
  TM_STOP_GET = false;
  TM_ELEAPSE_SEC = 0;
}

void timerStart()
{
  // ---- Timer 開始 ----------------
  if ((TM_SEC_VAL < TM_MIN) || (TM_SEC_VAL > TM_MAX))
  {
    Serial.println(TM_SEC_VAL, DEC);
    return;
  }

  // Timer Start Go ----
  TM_START_MILLIS = millis();

  int timer_min = TM_SEC_VAL / 60;
  int timer_sec = TM_SEC_VAL % 60;

  String msg = "";
  if (timer_min >= 1)
  {
    msg = String(timer_min, DEC) + "分";
  }
  if (timer_sec >= 1)
  {
    msg += String(timer_sec, DEC) + "秒";
  }
  String msg2 = msg;
  msg2 += "タイマー"; 
  
  msg += String("の、タイマーを開始します");
  Serial.println(msg);
  stackchan(msg, EXPR_HAPPY, msg2);

  TM_STARTED = true;
  TM_GO_GET = false;
  TM_STOP_GET = false;
}

void timerStop()
{
  // --- Timer を途中で停止 ------
  TM_STARTED = false;
  TM_GO_GET = false;
  TM_STOP_GET = false;
  TM_ELEAPSE_SEC = 0;

  String msg = String("タイマーを停止します");
  Serial.println(msg);

  // if(!isTalking())
    stackchan(msg, EXPR_SAD, msg, EXPR_NEUTRAL);

  ledClearAll();
}

void timerStarted()
{
  if (TM_ELEAPSE_SEC > 15)
    ledMoveSec(TM_ELEAPSE_SEC);

  //20秒間隔で読み上げ
  if ((TM_ELEAPSE_SEC % 20 == 0) && (TM_ELEAPSE_SEC < TM_SEC_VAL))
  {
    String msg ="";
    if (TM_ELEAPSE_SEC < 60)
      msg = String(TM_ELEAPSE_SEC,DEC) + "秒";
    else
    {
      int minutes = TM_ELEAPSE_SEC / 60;
      int seconds = TM_ELEAPSE_SEC % 60;
      if (seconds != 0)
        msg = String(minutes,DEC) + "分" + String(seconds,DEC) + "秒";
      else
        msg = String(minutes,DEC) + "分経過" ;
    }
    stackchan(msg, EXPR_HAPPY, msg, EXPR_NEUTRAL);
  }
}

void timerEnd()
{
  TM_STARTED = false;
  TM_GO_GET = false;
  TM_STOP_GET = false;
  TM_ELEAPSE_SEC = 0;

  String msg = String("設定時間になりました");
  Serial.println(msg);
  stackchan(String(msg), EXPR_HAPPY, msg, EXPR_NEUTRAL);
  ledClearAll();
}
