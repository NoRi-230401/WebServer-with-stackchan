// ---------------------------< wsReqMng.cpp >------------------------------------
#include "wsReqMng.h"

int REQUEST_NO2 = 0; // 0 : no request
String REQ_SPEAK_STR = "";
int REQ_EXPR = -1;
String REQ_BALOON_STR = "";

int REQUEST_NO = 0; // 0 : no request
String REQ_MSG = "";
bool REQ_chatGPT_GET = false;
int REQ_SHUTDOWN_REBOOT = 0;
int REQ_AVATAR_EXPR;

void sendReq2(int reqNo, const String &speakStr, int expr, const String balloonStr)
{
  REQ_SPEAK_STR = speakStr;
  REQ_EXPR = expr;
  REQ_BALOON_STR = balloonStr;
  REQUEST_NO2 = reqNo;
}

void stackchan(const String &speakStr, int expr, const String balloonStr)
{
  sendReq2(REQ_STACKCHAN, speakStr, expr, balloonStr);
}

void Req_stackchanDo()
{
  if (REQ_EXPR >= 0 && REQ_EXPR <= 5)
    setAvatarExpr(REQ_EXPR);

  if (REQ_BALOON_STR != "$$SKIP$$")
  {
    if (REQ_BALOON_STR == "" || REQ_BALOON_STR == "void")
      clearAvatarBalloon();
    else
      setAvatarBalloon(REQ_BALOON_STR);
  }

  if (REQ_SPEAK_STR != "" && REQ_SPEAK_STR != "$$SKIP$$")
  {
    SPEECH_TEXT = REQ_SPEAK_STR;
    ttsDo(SPEECH_TEXT);
    SPEECH_TEXT = "";
  }
}

void sendReq(int reqNo, String msg)
{
  REQ_MSG = msg;
  REQUEST_NO = reqNo;
}

void RequestManage()
{
  if (REQUEST_NO2 != 0 )
  {
    if( (REQUEST_NO2==REQ_STACKCHAN) && !isTalking() )
    {
      REQUEST_NO2 = 0;
      Req_stackchanDo();
    }
  }

  if (REQUEST_NO != 0)
  {
    int req = REQUEST_NO;

    switch (req)
    {
    case REQ_SPEAK:
      if (!isTalking())
      {
        REQUEST_NO = 0;
        Req_SpkDo();
      }
      break;

    case REQ_SPEAK_ADJUST:
      if (!isTalking())
      {
        REQUEST_NO = 0;
        Req_SpkDo_adjust();
      }
      break;

    case REQ_SPEAK_BALOON_ADJUST:
      if (!isTalking())
      {
        REQUEST_NO = 0;
        Req_SpkBaloonDo_adjust();
      }
      break;

    case REQ_BALOON_ADJUST:
      REQUEST_NO = 0;
      Req_BaloonDo_adjust();
      break;

    case REQ_BALOON:
      REQUEST_NO = 0;
      Req_BaloonDo();
      break;

    case REQ_SV_MD_ADJUST:
      REQUEST_NO = 0;
      SV_MD = SV_MD_ADJUST;
      break;

    default:
      REQUEST_NO = 0;
      break;
    }
  }

  if (REQ_SHUTDOWN_REBOOT > 0)
  {
    if (REQ_SHUTDOWN_REBOOT == 1)
    {
      // shutdown
      POWER_OFF();
    }
    else if (REQ_SHUTDOWN_REBOOT == 2)
    {
      // reboot
      REBOOT();
    }
    REQ_SHUTDOWN_REBOOT = 0;
  }
}

void Req_BaloonDo()
{
  avatar.setExpression(Expression::Happy);
  avatar.setSpeechText(REQ_MSG.c_str());
  avatar.setExpression(Expression::Neutral);
}

void Req_SpkDo()
{
  avatar.setExpression(expr_table[REQ_AVATAR_EXPR]);
  SPEECH_TEXT = REQ_MSG;
  ttsDo(SPEECH_TEXT);
  SPEECH_TEXT = "";
}

void Req_SpkBaloonDo_adjust()
{
  if (!SV_ADJUST_STATE)
    return;

  avatar.setExpression(Expression::Happy);
  avatar.setSpeechText(REQ_MSG.c_str());
  SPEECH_TEXT = REQ_MSG;
  ttsDo(SPEECH_TEXT);
  SPEECH_TEXT = "";
  avatar.setExpression(Expression::Neutral);
}

void Req_SpkDo_adjust()
{
  if (!SV_ADJUST_STATE)
    return;

  avatar.setExpression(Expression::Happy);
  SPEECH_TEXT = REQ_MSG;
  ttsDo(SPEECH_TEXT);
  SPEECH_TEXT = "";
  avatar.setExpression(Expression::Neutral);
}

void Req_BaloonDo_adjust()
{
  if (!SV_ADJUST_STATE)
    return;

  avatar.setExpression(Expression::Happy);
  avatar.setSpeechText(REQ_MSG.c_str());
  avatar.setExpression(Expression::Neutral);
}

void BaloonClear()
{
  sendReq(REQ_BALOON, "");
}
