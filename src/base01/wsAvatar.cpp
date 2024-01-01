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

int bIconState = 1;
bool bIconOnOff = true;
#define bIconStateMax 3

void avatarSTART()
{
  // --- BatteryIcon -------------
  // avatar.setBatteryIcon(true);
  // avatar.setBatteryIcon(false);
  // --- -Extended form NoRi 231230 -------------------
  // avatar.setBatteryIcon(false, BATTERY_MD_INVISIBLE);
  avatar.setBatteryIcon(true, BATTERY_MD_ICON);
  // avatar.setBatteryIcon(true, BATTERY_MD_NUM);
  // avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
  bIconState = 1;
  bIconOnOff = true;
  // ---------------------------------------------------

  avatar.setSpeechFont(&fonts::efontJA_16);
  // avatar.setSpeechFont(&fonts::efontJA_16_b);

  avatar.init(8);
  set_avatar_color();

  avatar.addTask(lipSync, "lipSync");
  avatar.addTask(servo, "servo");

  avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel(), "Hello StackChan");
  // 一度balloon表示しないとBatteryIconのテキスト設定うまくいかない為
  avatar.setSpeechText("スタックチャン");
  delay(1000);
  avatar.setSpeechText("");
}

void batteryIconSelect()
{
  if (bIconOnOff == false)
    return;

  bIconState++;
  bIconState = bIconState % bIconStateMax;

  switch (bIconState)
  {
  case 0:
    avatar.setBatteryIcon(true, BATTERY_MD_ICON);
    break;

  case 1:
    avatar.setBatteryIcon(true, BATTERY_MD_NUM);
    break;

  case 2:
    avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
    break;

  default:
    break;
  }
}

void batteryIconOnOff()
{
  if (bIconOnOff == true)
  {
    bIconOnOff = false;
    avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
  }
  else
  {
    bIconOnOff = true;
    switch (bIconState)
    {
    case 0:
      avatar.setBatteryIcon(true, BATTERY_MD_ICON);
      break;

    case 1:
      avatar.setBatteryIcon(true, BATTERY_MD_NUM);
      break;

    case 2:
      avatar.setBatteryIcon(true, BATTERY_MD_LINE_DISP);
      break;

    default:
      break;
    }
  }
}

constexpr int duration_500 = 500;             // 500ミリ秒
constexpr int duration_1013 = 1 * 1013;       // 1.013秒
constexpr int duration_1000 = 1 * 1013;       // 1秒
constexpr int duration_5000 = 5 * 1000;       // 5秒
constexpr int duration_10000 = 10 * 1000;     // 10秒
constexpr int duration_60000 = 60 * 1000;     // 60秒
constexpr int duration_90000 = 90 * 1000;     // 90秒
constexpr int duration_600000 = 600 * 1000;   // 10分
constexpr int duration_1800000 = 1800 * 1000; // 30分

uint32_t battery_time = millis(); // 前回チェック：バッテリー
void batteryIconManage()
{
  // バッテリー状態を更新
  if (millis() - battery_time >= duration_1013)
  {
    bool isCharging = (bool)M5.Power.isCharging();
    int batteryLevel = (int)M5.Power.getBatteryLevel();
    String msg = "Hello StackChan";
    msg = getDateTime();
    
    avatar.setBatteryStatus(isCharging, batteryLevel, msg);
    // avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());

    battery_time = millis();
  }
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
