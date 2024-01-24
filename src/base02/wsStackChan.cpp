// ----------------------------<wsStackChan.cpp>------------------------------------
#include "wsStackChan.h"

// extern void stackchan(String speakStr,int expr =-1, String balloonStr="$$SKIP$$" )

void stackchan(String speakStr, int expr, String balloonStr)
{
  if (expr >= 0 && expr <= 5)
    setAvatarExpr(expr);

  if (balloonStr != "$$SKIP$$")
  {
    if (balloonStr == "" || balloonStr == "void")
      clearAvatarBalloon();
    else
      setAvatarBalloon(balloonStr);
  }

  if(speakStr !="" && speakStr !="$$SKIP$$")
  {
    sendReq(REQ_SPEAK,speakStr);
  }
}
