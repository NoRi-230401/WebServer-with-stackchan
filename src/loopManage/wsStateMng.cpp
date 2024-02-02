// ----------------------------<wsStateMng.cpp>------------------------------------
#include "../h/wsStateMng.h"
uint16_t WST = WST_INIT;

void stateManage()
{
  if (WST == WST_LOOP || WST == WST_TTS_talking)
    return;

  switch (WST)
  {
  case WST_SETUP_done:
    log_free_size("setup() --- END --- ");
    showExeTime("setup() --- END --- ");
    WST = WST_LOOP;
    return;

  case WST_chatGPT_exit:
    log_free_size("chatGPT Exit : ");
    showExeTime("chatGPT Exi : ");
    WST = WST_LOOP;
    return;

  case WST_chatGPT_done:
    return;

  case WST_TTS_talkStart:
    WST = WST_TTS_talking;
    return;
  
  case WST_TTS_exit:
    log_free_size("TTS exit : ");
    showExeTime("TTS exit : ");
    WST = WST_LOOP;
    return;

  case WST_TTS_talkDone:
    avatar.setMouthOpenRatio(0);
    avatar.setSpeechText("");

    if (REQ_EXPR_AFTER >= 0 && REQ_EXPR_AFTER <= 5)
    {
      avatar.setExpression(expr_table[REQ_EXPR_AFTER]);
      // setAvatarExpr(REQ_EXPR_AFTER);  //コメントアウトするとハングアップする
      // Serial.println("exprAfter = " + String(REQ_EXPR_AFTER, DEC));
    }
    REQ_EXPR_AFTER = -1;

    log_free_size("VOICEVOX : OUT");
    showExeTime("VOICEVOX : end of speaking");
    WST = WST_LOOP;
    return;

  default:
    Serial.println("WST state value get : WST = " + String(WST, HEX));
    // WST = WST_LOOP;
    return;
  }

  return;
}
