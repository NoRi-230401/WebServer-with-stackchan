// ----------------------------<wsStackchan.cpp>------------------------------------
#include "wsStackchan.h"

using namespace m5avatar;
Avatar avatar;

const String EXPR_STR[] = {"Neutral", "Happy", "Sleepy", "Doubt", "Sad", "Angry", ""};
extern const Expression expr_table[];
const Expression expr_table[] = {
    Expression::Neutral,
    Expression::Happy,
    Expression::Sleepy,
    Expression::Doubt,
    Expression::Sad,
    Expression::Angry};

void stackchanReq(const String &speakStr, int expr, const String balloonStr, int afterExpr)
{
  sendReq_stackchan(speakStr, expr, balloonStr, afterExpr);
}


void stackchanNow( int expr, const String balloonStr)
{
  if(balloonStr != "$$SKIP$$")
    avatar.setSpeechText(balloonStr.c_str());

  if (expr >= 0 && expr <= 5)
    avatar.setExpression(expr_table[expr]);
}


static bool AVATAR_STATUS = true;
void avatarStop()
{
  if(!AVATAR_STATUS)
    return;

  avatar.suspend();
  AVATAR_STATUS = false;
  delay(100);
  Serial.println("avatar suspended");
}

void avatarStop2()
{
  M5.Lcd.setTextFont(0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextDatum(0);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.fillScreen(BLACK);
  delay(100);
}


void avatarResume()
{
  if(AVATAR_STATUS)
    return;

  avatar.resume();
  Serial.println("avatar resumed");
  AVATAR_STATUS = true;
  delay(100);
}


void avatarSTART()
{
  avatar.init(8);
  setAvatarcolor();
  avatar.setSpeechFont(&fonts::efontJA_16);

  if(SV_USE)
    avatar.addTask(servo, "servo");
  
  AVATAR_STATUS = true;
}

// アバターの色
#define config_color1_red 0     // 背景の色
#define config_color1_green 0   // 背景の色
#define config_color1_blue 0    // 背景の色
#define config_color2_red 255   // 目口の色
#define config_color2_green 255 // 目口の色
#define config_color2_blue 255  // 目口の色
#define config_color3_red 248   // ほっぺの色
#define config_color3_green 171 // ほっぺの色
#define config_color3_blue 166  // ほっぺの色
void setAvatarcolor()
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
  if (expr < 0 || expr > 5)
    return;

  stackchanReq("", expr);
  webpage = "face No. =  " + String(expr, DEC) + " : " + EXPR_STR[expr];
}

void setAvatarExpr(int expr)
{
  if (expr >= 0 && expr <= 5)
    avatar.setExpression(expr_table[expr]);
}

void wsHandleBalloon(String text)
{
  stackchanReq("", -1, text);

  if (text == "" || text == "void")
  {
    // clearAvatarBalloon();
    webpage = "clear speech balloon";
  }
  else
  {
    // setAvatarBalloon(text);
    webpage = "speech balloon text = " + text;
  }
}

void setAvatarBalloon(String msg)
{
  avatar.setSpeechText(msg.c_str());
}

void clearAvatarBalloon()
{
  avatar.setSpeechText("");
}
