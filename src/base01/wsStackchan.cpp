// ----------------------------<wsStackchan.cpp>------------------------------------
#include "../h/wsStackchan.h"

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
  // sendReq2(REQ_STACKCHAN, speakStr, expr, balloonStr, afterExpr);
  sendReq_stackchan(speakStr, expr, balloonStr, afterExpr);
}



void stackchanNow( int expr, const String balloonStr)
{
  if(balloonStr != "$$SKIP$$")
    avatar.setSpeechText(balloonStr.c_str());

  if (expr >= 0 && expr <= 5)
    avatar.setExpression(expr_table[expr]);
}


void avatarSTART()
{
  avatar.init(8);
  setAvatarcolor();
  avatar.setSpeechFont(&fonts::efontJA_16);
  avatar.addTask(servo, "servo");

  // // -- batteryStatusLine setup ---
  // StatusLineMode = STATUS_MD_IP;
  // statusLineOnOffState = false;
  // avatar.setStatusLineText("");
  // avatar.setStatusLineFont(&fonts::Font0);
  // avatar.setBatteryIcon(true, BATTERY_MD_INVISIBLE);
  // // avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());

  // // 一度balloon表示しないとBatteryIconのフォント設定が反映されない？？ -- by NoRi 240101 --
  // avatar.setSpeechText("スタックチャン");
  // delay(1000);
  // avatar.setSpeechText("");
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
