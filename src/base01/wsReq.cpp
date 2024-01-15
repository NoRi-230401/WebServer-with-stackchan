// ---------------------------< wsReq.cpp >------------------------------------
#include "wsReq.h"

int REQUEST_GET = 0; // 0 : no request
String REQ_MSG = "";
bool REQ_chatGPT_GET = false;
int REQ_SHUTDOWN_REBOOT = 0;
// int REQ_SPK_PARMS_NO;
int REQ_SPK_EXPR;

void sendReq(int reqNo, String msg)
{
  REQ_MSG = msg;
  REQUEST_GET = reqNo;
}

void RequestManage()
{

  if (REQUEST_GET != 0)
  {
    int req = REQUEST_GET;

    switch (req)
    {
    case REQ_SPEAK_ADJUST:
      REQUEST_GET = 0;
      Req_SpkDo_adjust();
      break;

    case REQ_BALOON_ADJUST:
      REQUEST_GET = 0;
      Req_BaloonDo_adjust();
      break;

    case REQ_SPEAK_BALOON_ADJUST:
      if (!isTalking() )
      {
        REQUEST_GET = 0;
        Req_SpkBaloonDo_adjust();
      }
      break;

    case REQ_SPEAK:
      if (!isTalking() )
      {
        REQUEST_GET = 0;
        Req_SpkDo();
      }
      break;

    case REQ_BALOON:
      REQUEST_GET = 0;
      Req_BaloonDo();
      break;

    case REQ_SV_MD_ADJUST:
      REQUEST_GET = 0;
      SV_MD = SV_MD_ADJUST;
      break;

    default:
      REQUEST_GET = 0;
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

// void ReqSpkBaloon_adjust(String spkMsg)
// {
//   REQ_MSG = spkMsg;
//   REQUEST_GET = REQ_SPEAK_BALOON_ADJUST;
// }

// void ReqSpkOnly(String spkMsg)
// {
//   REQ_MSG = spkMsg;
//   REQUEST_GET = REQ_SPEAK;
// }

// void ReqBaloonOnly(String msg)
// {
//   REQ_MSG = msg;
//   REQUEST_GET = REQ_BALOON;
// }

void Req_BaloonDo()
{
  avatar.setExpression(Expression::Happy);
  avatar.setSpeechText(REQ_MSG.c_str());
  avatar.setExpression(Expression::Neutral);
}

void Req_SpkDo()
{
  avatar.setExpression(expressions_table[REQ_SPK_EXPR]);
  ttsDo(REQ_MSG);
}

void Req_SpkBaloonDo_adjust()
{
  if (!SV_ADJUST_STATE)
    return;

  avatar.setExpression(Expression::Happy);
  avatar.setSpeechText(REQ_MSG.c_str());
  ttsDo(REQ_MSG);
  avatar.setExpression(Expression::Neutral);
}

void Req_SpkDo_adjust()
{
  if (!SV_ADJUST_STATE)
    return;

  avatar.setExpression(Expression::Happy);
  ttsDo(REQ_MSG);
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

// void Req_Baloon_adjust()
// {
//   if (!SV_ADJUST_STATE)
//     return;

//   REQUEST_GET = REQ_BALOON_ADJUST;
// }

void BaloonClear()
{
  sendReq(REQ_BALOON,"");
  // REQ_MSG = "";
  // avatar.setSpeechText("");
  // avatar.setExpression(Expression::Neutral);
}
