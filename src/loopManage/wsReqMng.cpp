// ---------------------------< wsReqMng.cpp >------------------------------------
#include "../h/wsReqMng.h"

int REQUEST_NO2 = 0; // 0 : no request
String REQ_SPEAK_STR = "";
int REQ_EXPR = -1;
int REQ_EXPR_AFTER = -1;
String REQ_BALOON_STR = "";

int REQUEST_NO = 0; // 0 : no request
String REQ_MSG = "";
bool REQ_chatGPT_GET = false;
int REQ_SHUTDOWN_REBOOT = 0;

// void sendReq2(int reqNo, const String &speakStr, int expr, const String balloonStr, int afterExpr)
// {
//   REQ_SPEAK_STR = speakStr;
//   REQ_EXPR = expr;
//   REQ_EXPR_AFTER = afterExpr;
//   REQ_BALOON_STR = balloonStr;
//   // REQUEST_NO2 = reqNo;
//   REQUEST_NO = reqNo;
// }

void sendReq_stackchan(const String &speakStr, int expr, const String balloonStr, int afterExpr)
{
  REQ_SPEAK_STR = speakStr;
  REQ_EXPR = expr;
  REQ_EXPR_AFTER = afterExpr;
  REQ_BALOON_STR = balloonStr;
  REQUEST_NO = REQ_STACKCHAN;
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

// void sendReq(int reqNo, String msg)
// {
//   REQ_MSG = msg;
//   REQUEST_NO = reqNo;
// }

void requestManage()
{
  // if (REQUEST_NO2 != 0)
  // {
  //   if ((REQUEST_NO2 == REQ_STACKCHAN) && !isTalking())
  //   {
  //     REQUEST_NO2 = 0;
  //     Req_stackchanDo();
  //   }
  // }

  if (REQUEST_NO != 0)
  {
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
