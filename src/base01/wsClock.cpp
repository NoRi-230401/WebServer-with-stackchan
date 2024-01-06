// ----------------------------<wsClock.cpp>------------------------------------
#include "wsClock.h"

// const char *week[] = {"(Sun)", "(Mon)", "(Tue)", "(Wed)", "(Thu)", "(Fri)", "(Sat)"}; // 曜日
const char *week[] = {"(日)", "(月)", "(火)", "(水)", "(木)", "(金)", "(土)"}; // 曜日
#define JST 3600 * 9        // グリニッジ標準時と日本標準時の差

void clockSetup()
{
  // NTPサーバとローカルのタイムゾーンを設定
  configTime(JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  // getDateTime();
}

// 現在日時を表示
String getDateTime()
{
  struct tm timeInfo;
  char s[40];
  String dateTime ="2024/01/01(月) 00:00:00";

// 時刻を取得できなかった場合
  if (!getLocalTime(&timeInfo))
  {
    Serial.println("getDateTime : Failed to obtain time. ");
    return dateTime;
  }

  // sprintf(s, " %04d/%02d/%02d%s %02d:%02d:%02d",
  //         timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
  //         week[timeInfo.tm_wday],
  //         timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);

  sprintf(s, "%04d/%02d/%02d%s  %2d:%02d:%02d",
          timeInfo.tm_year + 1900,timeInfo.tm_mon + 1, timeInfo.tm_mday, week[timeInfo.tm_wday],
          timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);

  // Serial.println(String(s));
  dateTime = String(s);
  
  return dateTime;
}
