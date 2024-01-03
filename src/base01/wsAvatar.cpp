// ----------------------------<wsAvatar.cpp>------------------------------------
#include "wsAvatar.h"

using namespace m5avatar;
Avatar avatar;

const String EXPRESSION_STRING[] = {"Neutral", "Happy", "Sleepy", "Doubt", "Sad", "Angry", ""};

extern const Expression expressions_table[];
const Expression expressions_table[] = {
    Expression::Neutral,
    Expression::Happy,
    Expression::Sleepy,
    Expression::Doubt,
    Expression::Sad,
    Expression::Angry};

bool statusLineOnOffState = true;
bool statusLineOneState = false;
#define STATUS_MD_ICON 0
#define STATUS_MD_NUM 1
#define STATUS_MD_CLOCK 2
#define STATUS_MD_RSSI 3
#define STATUS_MD_IP 4
#define STATUS_MD_VOL 5
#define STATUS_MD_MAX 6
int StatusLineMode = STATUS_MD_ICON;

constexpr int duration_1013 = 1 * 1013;       // 1.013秒: statusLineCheck_time
constexpr int duration_10000 = 10 * 1000;     // 10秒    : statusLineOne_time
uint32_t statusLineOne_time = 0; 
uint32_t statusLineCheck_time = 0;



void avatarSTART()
{
  avatar.init(8);
  set_avatar_color();
  
  avatar.setSpeechFont(&fonts::efontJA_16);
  avatar.addTask(lipSync, "lipSync");
  avatar.addTask(servo, "servo");

  // --- status line initial setup ---
  StatusLineMode = STATUS_MD_ICON;
  statusLineOnOffState = true;
  statusLineOneState=false;
  avatar.setBatteryIcon(true, BATTERY_MD_ICON);
  avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel(), "");
  // 一度balloon表示しないとBatteryIconのフォント設定が反映されない？？ -- by NoRi 240101 --
  avatar.setSpeechText("スタックチャン");
  delay(1000);
  avatar.setSpeechText("");
}

void statusLineSelect()
{
  if (!statusLineOnOffState)
    return;

  StatusLineMode++;
  StatusLineMode = StatusLineMode % STATUS_MD_MAX;

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
  case STATUS_MD_IP:
  case STATUS_MD_VOL:
    avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
    break;

  default:
    break;
  }
}

void statusLineOne()
{
  if (statusLineOneState || statusLineOnOffState)
    return;

  statusLineOneState = true;

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
  case STATUS_MD_IP:
  case STATUS_MD_VOL:
    avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
    break;

  default:
    break;
  }
  statusLineOne_time = millis();
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
    case STATUS_MD_IP:
    case STATUS_MD_VOL:
      avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
      break;

    default:
      break;
    }
  }
}

void statusLineOneManage()
{
  if (!statusLineOneState || statusLineOnOffState)
    return;

  if ( (millis() - statusLineOne_time) < duration_10000)
    return;

  statusLineOneState = false;
  statusLineOnOffState = false;
  avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
  statusLineOne_time = millis();
}



void statusLineCheckManage()
{
  if (millis() - statusLineCheck_time >= duration_1013)
  {
    bool isCharging = (bool)M5.Power.isCharging();
    int batteryLevel = (int)M5.Power.getBatteryLevel();
    String msg = "";
    char s[40];

    switch (StatusLineMode)
    {
    case STATUS_MD_ICON:
    case STATUS_MD_NUM:
      break;

    case STATUS_MD_CLOCK:
      msg = getDateTime();
      break;

    case STATUS_MD_RSSI:
      msg = "WiFi  Rssi=" + String(WiFi.RSSI()) + "dB   Chan=" + String(WiFi.channel());
      break;

    case STATUS_MD_IP:
      msg = String(WiFi.localIP().toString()) + "  " + SERVER_NAME;
      break;

    case STATUS_MD_VOL:
      sprintf(s, "vol=%3d  vSpk=%2d  chara=%d", VOLUME_VALUE, TTS2_SPEAKER_NO.toInt(), CHARA_NO);
      msg = String(s);
      break;

    default:
      break;
    }

    avatar.setBatteryStatus(isCharging, batteryLevel, msg);
    statusLineCheck_time = millis();
  }
}

void StatusLineManage()
{
  statusLineCheckManage();
  statusLineOneManage();
}


uint8_t config_color1_red = 0;     // 背景の色
uint8_t config_color1_green = 0;   // 背景の色
uint8_t config_color1_blue = 0;    // 背景の色
uint8_t config_color2_red = 255;   // 目口の色
uint8_t config_color2_green = 255; // 目口の色
uint8_t config_color2_blue = 255;  // 目口の色
uint8_t config_color3_red = 248;   // ほっぺの色
uint8_t config_color3_green = 171; // ほっぺの色
uint8_t config_color3_blue = 166;  // ほっぺの色

// アバターの色
void set_avatar_color()
{
  ColorPalette cp;
  cp.set(COLOR_BACKGROUND, M5.Lcd.color565(config_color1_red, config_color1_green, config_color1_blue));
  cp.set(COLOR_PRIMARY, M5.Lcd.color565(config_color2_red, config_color2_green, config_color2_blue));
  cp.set(COLOR_SECONDARY, M5.Lcd.color565(config_color3_red, config_color3_green, config_color3_blue));
  cp.set(COLOR_BALLOON_FOREGROUND, M5.Lcd.color565(config_color1_red, config_color1_green, config_color1_blue));
  cp.set(COLOR_BALLOON_BACKGROUND, M5.Lcd.color565(config_color2_red, config_color2_green, config_color2_blue));
  avatar.setColorPalette(cp);
}

void wsHandleFace(String expression)
{
  int expr = expression.toInt();

  if (setFace(expr))
    webpage = "face No. =  " + String(expr, DEC) + " : " + EXPRESSION_STRING[expr];
}

bool setFace(int expr)
{
  if (expr > 5)
    return false;

  switch (expr)
  {
  case 0:
    avatar.setExpression(Expression::Neutral);
    break;
  case 1:
    avatar.setExpression(Expression::Happy);
    break;
  case 2:
    avatar.setExpression(Expression::Sleepy);
    break;
  case 3:
    avatar.setExpression(Expression::Doubt);
    break;
  case 4:
    avatar.setExpression(Expression::Sad);
    break;
  case 5:
    avatar.setExpression(Expression::Angry);
    break;
  }
  return true;
}

void lipSync(void *args)
{
  float gazeX, gazeY;
  int level = 0;
  DriveContext *ctx = (DriveContext *)args;
  Avatar *avatar = ctx->getAvatar();
  for (;;)
  {
    level = abs(*out.getBuffer());
    if (level < 100)
      level = 0;
    if (level > 15000)
    {
      level = 15000;
    }
    float open = (float)level / 15000.0;
    avatar->setMouthOpenRatio(open);

    avatar->getGaze(&gazeY, &gazeX);
    // avatar->setRotation(gazeX * 5);

    delay(50);
  }
}

void servo(void *args)
{
  float gazeX, gazeY;
  DriveContext *ctx = (DriveContext *)args;
  Avatar *avatar = ctx->getAvatar();
  for (;;)
  {
    if (SV_USE)
    {
      int mode = SV_MD;

      if (mode == SV_MD_MOVING)
      {
        avatar->getGaze(&gazeY, &gazeX);
        sv_setEaseToX(SV_HOME_X + (int)(15.0 * gazeX));

        if (gazeY < 0)
        {
          int tmp = (int)(10.0 * gazeY);
          if (tmp > 10)
            tmp = 10;
          sv_setEaseToY(SV_HOME_Y + tmp);
        }
        else
        {
          sv_setEaseToY(SV_HOME_Y + (int)(10.0 * gazeY));
        }
        synchronizeAllServosStartAndWaitForAllServosToStop();
      }
      else
      {
        servo2(mode);
      }
      delay(50);
    }
  }
}
