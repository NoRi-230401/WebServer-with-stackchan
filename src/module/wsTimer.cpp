// ----------------------------<wsTimer.cpp>------------------------------------
#include "wsTimer.h"

uint16_t TM_SEC_VAL = TM_INIT; // Timer時間設定(sec)
bool TM_STARTED = false;
uint32_t TM_START_MILLIS = 0;
uint16_t TM_ELEAPSE_SEC = 0;
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
    uint16_t currentElapsedSeconds = elapsedTimeMillis / 1000;

    if (currentElapsedSeconds >= TM_SEC_VAL)
    { // 指定時間が経過したら終了
      timerEnd();
    }
    else if (TM_STOP_GET && !isTalking())
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

  // ledClear();
  // ledShow();
  // ledSetColor2(2, ledGetColorNo(0, 0, 255));
  // ledSetColor2(7, ledGetColorNo(0, 0, 255));

  int timer_min = TM_SEC_VAL / 60;
  int timer_sec = TM_SEC_VAL % 60;
  char timer_min_str[10] = "";
  char timer_sec_str[10] = "";
  char timer_msg_str[100] = "";

  if (timer_min >= 1)
  {
    sprintf(timer_min_str, "%d分", timer_min);
  }
  if (timer_sec >= 1)
  {
    sprintf(timer_sec_str, "%d秒", timer_sec);
  }

  char EX_TmrSTART_TXT[] = "の、タイマーを開始します。";
  sprintf(timer_msg_str, "%s%s%s", timer_min_str, timer_sec_str, EX_TmrSTART_TXT);
  Serial.println(timer_msg_str);
  avatar.setExpression(Expression::Happy);
  sendReq(REQ_SPEAK,String(timer_msg_str));

  // ledShow();
  // delay(3000); // 3秒待機
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

  // ledClear();
  // ledShow();

  // ledSetColor2(2, ledGetColorNo(255, 0, 0));
  // ledSetColor2(7, ledGetColorNo(255, 0, 0));

  char EX_TmrSTOP_TXT[] = "タイマーを停止します。";
  avatar.setExpression(Expression::Neutral);
  sendReq(REQ_SPEAK,String(EX_TmrSTOP_TXT));

  // ledShow();
  // delay(2000); // 2秒待機

  // // 全てのLEDを消灯
  // ledClear();
  // ledShow();
  // delay(500); // 0.5秒待機
}

void timerStarted()
{
  // timer開始後の途中経過の処理

  // 0.5秒ごとにLEDを更新する処理を追加
  int phase = (TM_ELEAPSE_SEC / 5) % 2; // 往復の方向を決定
  int pos = TM_ELEAPSE_SEC % 5;
  int ledIndex1, ledIndex2;

  if (phase == 0)
  { // 前進
    ledIndex1 = pos;
    ledIndex2 = NUM_LEDS - 1 - pos;
  }
  else
  { // 後退
    ledIndex1 = 4 - pos;
    ledIndex2 = 5 + pos;
  }

  // ledClear();                         // すべてのLEDを消す
  // ledSetColor(ledIndex1, 0, 0, 255); // 現在のLEDを青色で点灯
  // ledSetColor(ledIndex2, 0, 0, 255); // 現在のLEDを青色で点灯
  // ledShow();                          // LEDの状態を更新

  // 10秒->20秒間隔で読み上げ
  if ((TM_ELEAPSE_SEC % 20 == 0) && (TM_ELEAPSE_SEC < TM_SEC_VAL))
  {
    char buffer[64];
    if (TM_ELEAPSE_SEC < 60)
    {
      sprintf(buffer, "%d秒。", TM_ELEAPSE_SEC);
    }
    else
    {
      int minutes = TM_ELEAPSE_SEC / 60;
      int seconds = TM_ELEAPSE_SEC % 60;
      if (seconds != 0)
      {
        sprintf(buffer, "%d分%d秒。", minutes, seconds);
      }
      else
      {
        sprintf(buffer, "%d分経過。", minutes);
      }
    }
    // avatar.setExpression(Expression::Happy);
    sendReq(REQ_SPEAK,String(buffer));
    // avatar.setExpression(Expression::Neutral);
  }
}

void timerEnd()
{
  // 指定時間が経過したら終了
  // 全てのLEDを消す処理を追加
  // ledClear();
  // ledShow();
  // ledSetColor2(2, ledGetColorNo(0, 255, 0));
  // ledSetColor2(7, ledGetColorNo(0, 255, 0));
  // ledShow();

  // avatar.setExpression(Expression::Happy);
  sendReq(REQ_SPEAK,String("設定時間になりました。"));
  avatar.setExpression(Expression::Neutral);

  // 全てのLEDを消す処理を追加
  // ledClear();
  // ledShow(); // LEDの状態を更新

  // カウントダウンをリセット
  TM_STARTED = false;
  TM_GO_GET = false;
  TM_STOP_GET = false;
  TM_ELEAPSE_SEC = 0;
}
