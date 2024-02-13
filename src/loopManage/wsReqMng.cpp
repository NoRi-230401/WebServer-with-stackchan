// ---------------------------< wsReqMng.cpp >------------------------------------
#include "wsReqMng.h"

String REQ_SPEAK_STR = "";
int REQ_EXPR = -1;
int REQ_EXPR_AFTER = -1;
String REQ_BALLOON_STR = "";
int REQUEST_NO = 0; // 0 : no request
String REQ_MSG = "";

void requestManage()
{
  if (REQUEST_NO == 0)
    return;

  int req = REQUEST_NO;

  switch (req)
  {
  case REQ_STACKCHAN:
    if (!isTalking())
    {
      REQUEST_NO = 0;
      Req_stackchanDo();
    }
    break;

  case REQ_SV_MD_ADJUST:
    if (!isTalking())
    {
      REQUEST_NO = 0;
      SV_MD = SV_MD_ADJUST;
    }
    break;

  case REQ_REBOOT:
    REBOOT();
    break;

  case REQ_SHUTDOWN:
    POWER_OFF();
    break;

  default:
    REQUEST_NO = 0;
    break;
  }
}

void sendReq(int reqNo)
{
  REQUEST_NO = reqNo;
}

void sendReq_stackchan(const String &speakStr, int expr, const String balloonStr, int afterExpr)
{
  REQ_SPEAK_STR = speakStr;
  REQ_EXPR = expr;
  REQ_EXPR_AFTER = afterExpr;
  REQ_BALLOON_STR = balloonStr;
  REQUEST_NO = REQ_STACKCHAN;
}

void Req_stackchanDo()
{
  if (REQ_EXPR >= 0 && REQ_EXPR <= 5)
    setAvatarExpr(REQ_EXPR);

  if (REQ_BALLOON_STR != "$$SKIP$$")
  {
    if (REQ_BALLOON_STR == "" || REQ_BALLOON_STR == "void")
      clearAvatarBalloon();
    else
      setAvatarBalloon(REQ_BALLOON_STR);
  }

  if (REQ_SPEAK_STR != "" && REQ_SPEAK_STR != "$$SKIP$$")
  {
    SPEECH_TEXT = REQ_SPEAK_STR;
    ttsDo(SPEECH_TEXT);
    SPEECH_TEXT = "";
  }
}
