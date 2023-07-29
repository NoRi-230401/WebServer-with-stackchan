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

void avatarSTART()
{
  avatar.init();
  avatar.addTask(lipSync, "lipSync");
  avatar.addTask(servo, "servo");
  avatar.setSpeechFont(&fonts::efontJA_16);
}

extern String webpage;
void wsHandleFace(String expression);


void wsHandleFace(String expression)
{
  int expr = expression.toInt();

  if (setFace(expr))
    webpage = "face No. =  " + String(expr,DEC) + " : " + EXPRESSION_STRING[expr] ;
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
    avatar->setRotation(gazeX * 5);
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
