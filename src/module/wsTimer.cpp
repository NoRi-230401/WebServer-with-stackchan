// ----------------------------<wsTimer.cpp>------------------------------------
#include "wsTimer.h"

uint16_t TIMER_SEC_VALUE = TIMER_INIT; // Timer時間設定(sec)
bool TIMER_STARTED = false;
uint32_t TIMER_START_MILLIS = 0;
uint16_t TIMER_ELEAPSE_SEC = 0;
bool TIMER_STOP_GET = false;
bool TIMER_GO_GET = false;

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

    TIMER_SEC_VALUE = (timerMin * 60) + timerSec;

    if (TIMER_SEC_VALUE > TIMER_MAX)
      TIMER_SEC_VALUE = TIMER_MAX;

    if (TIMER_SEC_VALUE < TIMER_MIN)
      TIMER_SEC_VALUE = TIMER_MIN;

    // NVS に保存
    uint32_t nvs_handle;
    size_t timer_value = (size_t)TIMER_SEC_VALUE;
    if (ESP_OK == nvs_open("setting", NVS_READWRITE, &nvs_handle))
    {
      nvs_set_u32(nvs_handle, "timer", timer_value);
    }
    nvs_close(nvs_handle);

    TIMER_STOP_GET = false;
    TIMER_GO_GET = false;
    webpage = "timer = " + String(TIMER_SEC_VALUE, DEC) + "sec";
  }

  // String timer_mode = request->arg("mode");
  if (timerModeS != "")
  {
    timerModeS.toUpperCase();

    if (timerModeS == "START" || timerModeS == "GO")
    {
      if (!TIMER_STARTED)
      {
        if (SYSINFO_DISP_STATE)
          sysInfoDispEnd();

        randomSpeakStop2();
        TIMER_GO_GET = true;
        TIMER_STOP_GET = false;

        webpage = "timer Start : " + String(TIMER_SEC_VALUE, DEC) + "sec";
        Serial.println(webpage);
        return;
      }
    }
    else if (timerModeS == "STOP")
    {
      if (TIMER_STARTED)
      {
        TIMER_STOP_GET = true;
        TIMER_GO_GET = false;
        webpage = "timer Stop";
        Serial.println(webpage);
        return;
      }
    }
  }
}

void TimerManage()
{
  if (TIMER_STARTED)
  { // Timer 起動中
    if (SYSINFO_DISP_STATE)
      sysInfoDispEnd();

    uint32_t elapsedTimeMillis = millis() - TIMER_START_MILLIS;
    uint16_t currentElapsedSeconds = elapsedTimeMillis / 1000;

    if (currentElapsedSeconds >= TIMER_SEC_VALUE)
    { // 指定時間が経過したら終了
      timerEnd();
    }
    else if (TIMER_STOP_GET && (SPEECH_TEXT_BUFFER == "") && (SPEECH_TEXT == ""))
    { // ---Timer停止---
      timerStop();
    }
    else if (currentElapsedSeconds != TIMER_ELEAPSE_SEC)
    { // --- Timer途中経過の処理------
      TIMER_ELEAPSE_SEC = currentElapsedSeconds;
      timerStarted();
    }
  }
  else
  {
    if ( TIMER_GO_GET && (SPEECH_TEXT_BUFFER == "") && (SPEECH_TEXT == ""))
      timerStart();
  }
}

void timerStop2()
{
  // --- Timer を途中で停止 ------
  TIMER_STARTED = false;
  TIMER_GO_GET = false;
  TIMER_STOP_GET = false;
  TIMER_ELEAPSE_SEC = 0;
}

void timerStart()
{
  // ---- Timer 開始 ----------------
  if ((TIMER_SEC_VALUE < TIMER_MIN) || (TIMER_SEC_VALUE > TIMER_MAX))
  {
    Serial.println(TIMER_SEC_VALUE, DEC);
    return;
  }

  // Timer Start Go ----
  TIMER_START_MILLIS = millis();

  led_clear();
  led_show();
  led_setColor2(2, led_ColorLED3(0, 0, 255));
  led_setColor2(7, led_ColorLED3(0, 0, 255));

  int timer_min = TIMER_SEC_VALUE / 60;
  int timer_sec = TIMER_SEC_VALUE % 60;
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
  // ttsDo(timer_msg_str, tts_parms2);
  ttsDo(String(timer_msg_str));
  led_show();

  delay(3000); // 3秒待機
  TIMER_STARTED = true;
  TIMER_GO_GET = false;
  TIMER_STOP_GET = false;
}

void timerStop()
{
  // --- Timer を途中で停止 ------
  TIMER_STARTED = false;
  TIMER_GO_GET = false;
  TIMER_STOP_GET = false;
  TIMER_ELEAPSE_SEC = 0;

  led_clear();
  led_show();

  led_setColor2(2, led_ColorLED3(255, 0, 0));
  led_setColor2(7, led_ColorLED3(255, 0, 0));

  char EX_TmrSTOP_TXT[] = "タイマーを停止します。";
  ttsDo(String(EX_TmrSTOP_TXT));
  led_show();
  delay(2000); // 2秒待機

  // 全てのLEDを消灯
  led_clear();
  led_show();
  delay(500); // 0.5秒待機
}

void timerStarted()
{
  // timer開始後の途中経過の処理

  // 0.5秒ごとにLEDを更新する処理を追加
  int phase = (TIMER_ELEAPSE_SEC / 5) % 2; // 往復の方向を決定
  int pos = TIMER_ELEAPSE_SEC % 5;
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

  led_clear();                         // すべてのLEDを消す
  led_setColor4(ledIndex1, 0, 0, 255); // 現在のLEDを青色で点灯
  led_setColor4(ledIndex2, 0, 0, 255); // 現在のLEDを青色で点灯
  led_show();                          // LEDの状態を更新

  // 10秒間隔で読み上げ
  if ((TIMER_ELEAPSE_SEC % 10 == 0) && (TIMER_ELEAPSE_SEC < TIMER_SEC_VALUE))
  {
    char buffer[64];
    if (TIMER_ELEAPSE_SEC < 60)
    {
      sprintf(buffer, "%d秒。", TIMER_ELEAPSE_SEC);
    }
    else
    {
      int minutes = TIMER_ELEAPSE_SEC / 60;
      int seconds = TIMER_ELEAPSE_SEC % 60;
      if (seconds != 0)
      {
        sprintf(buffer, "%d分%d秒。", minutes, seconds);
      }
      else
      {
        sprintf(buffer, "%d分経過。", minutes);
      }
    }
    avatar.setExpression(Expression::Happy);
    ttsDo(String(buffer));
    avatar.setExpression(Expression::Neutral);
  }
}

void timerEnd()
{
  // 指定時間が経過したら終了
  // 全てのLEDを消す処理を追加
  led_clear();
  led_show();
  led_setColor2(2, led_ColorLED3(0, 255, 0));
  led_setColor2(7, led_ColorLED3(0, 255, 0));
  led_show();

  avatar.setExpression(Expression::Happy);
  ttsDo(String("設定時間になりました。"));
  avatar.setExpression(Expression::Neutral);

  // 全てのLEDを消す処理を追加
  led_clear();
  led_show(); // LEDの状態を更新

  // カウントダウンをリセット
  TIMER_STARTED = false;
  TIMER_GO_GET = false;
  TIMER_STOP_GET = false;
  TIMER_ELEAPSE_SEC = 0;
}
