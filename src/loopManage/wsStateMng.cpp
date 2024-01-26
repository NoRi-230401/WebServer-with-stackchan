// ----------------------------<wsStateMng.cpp>------------------------------------
#include "wsStateMng.h"
uint16_t WST = 0x0000;

void stateManage()
{
  if (WST == WST_loop)
    return;

  else if (WST == WST_setupDone)
  {
    log_free_size("初期化終了：");
    showExeTime("setup()  --- End --- ");
    WST = WST_loop;
    return;
  }

  else if (WST == WST_talkDone)
  {
    avatar.setMouthOpenRatio(0);
    avatar.setSpeechText("");

    if (REQ_EXPR_AFTER >= 0 && REQ_EXPR_AFTER <= 5)
    {
      avatar.setExpression(expr_table[REQ_EXPR_AFTER]);
      
      // setAvatarExpr(REQ_EXPR_AFTER);  // なぜかこれをコメントアウトするとハングアップする
      // Serial.println("exprAfter = " + String(REQ_EXPR_AFTER, DEC));
    }
    REQ_EXPR_AFTER = -1;

    log_free_size("VOICEVOX：OUT");
    showExeTime("VOICEVOX：end of speaking");
    WST = WST_loop;
    return;
  }

  else if (WST == WST_ttsExit)
  {
    WST = WST_loop;
    return;
  }

  return;

  // if (WST != WST_loop)
  // {
  //   switch (WST)
  //   {
  //   case WST_setupDone:
  //     // log_free_size("初期化終了：");
  //     // showExeTime("setup()  --- End --- ");
  //     WST = WST_loop;
  //     break;

  //   case WST_talkDone:
  //     if (REQ_AVATAR_EXPR >= 0 && REQ_AVATAR_EXPR <= 5)
  //       setAvatarExpr(REQ_AVATAR_EXPR);
  //     REQ_AVATAR_EXPR = -1;

  //     showExeTime("VOICEVOX：end of speaking");
  //     log_free_size("VOICEVOX：OUT");
  //     WST = WST_loop;
  //     break;

  //   default:
  //     break;
  //   }
  // }
}
