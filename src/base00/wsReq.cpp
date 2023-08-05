// ---------------------------< wsReq.cpp >------------------------------------
#include "wsReq.h"

int REQUEST_GET = 0; // 0 : no request
String REQ_MSG = "";
bool REQ_chatGPT_GET = false;
int REQ_SHUTDOWN_REBOOT = 0;
int REQ_SPK_PARMS_NO;
int REQ_SPK_EXPR;


void RequestManage()
{

  if (REQUEST_GET != 0)
  {
    int req = REQUEST_GET;

    switch (req)
    {
    case REQ_SPEAK:
      Req_SpkDo();
      break;

    case REQ_MSG_ONLY:
      Req_MsgOnlyDo();
      break;

    case REQ_SPEAK_MSG:
      Req_SpkMsgDo();
      break;

    case REQ_SPEAK_MSG2:
      ReqSpkMsgDo2();
      break;

    case REQ_SV_MD_ADJUST:
      SV_MD = SV_MD_ADJUST;
      break;

    default:
      break;
    }

    REQUEST_GET = 0;
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


void ReqSpkMsgDo2()
{
  avatar.setExpression(expressions_table[REQ_SPK_EXPR]);
  ttsDo(REQ_MSG);
}


void servoReqSpkMsg()
{
  REQUEST_GET = REQ_SPEAK_MSG;
}

void Req_SpkMsgDo()
{
  if (!SV_ADJUST_STATE)
    return;

  avatar.setExpression(Expression::Happy);
  avatar.setSpeechText(REQ_MSG.c_str());
  ttsDo(REQ_MSG);
  avatar.setExpression(Expression::Neutral);
}

void Req_SpkDo()
{
  if (!SV_ADJUST_STATE)
    return;

  avatar.setExpression(Expression::Happy);
  ttsDo(REQ_MSG);
  avatar.setExpression(Expression::Neutral);
}

void Req_MsgOnlyDo()
{
  if (!SV_ADJUST_STATE)
    return;

  avatar.setExpression(Expression::Happy);
  avatar.setSpeechText(REQ_MSG.c_str());
  avatar.setExpression(Expression::Neutral);
}

void Req_MsgOnly()
{
  if (!SV_ADJUST_STATE)
    return;

  REQUEST_GET = REQ_MSG_ONLY;
}

void Req_MsgCls()
{
  REQ_MSG = "";
  avatar.setSpeechText("");
  avatar.setExpression(Expression::Neutral);
}
