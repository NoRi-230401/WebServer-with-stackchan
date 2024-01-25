// ----------------------------<wsStackChan.cpp>------------------------------------
#ifndef WS_STACKCHAN_H
#define WS_STACKCHAN_H
// ---------------------------
#include <Arduino.h>

#define EXPR_NEUTRAL 0
#define EXPR_HAPPY 1
#define EXPR_SLEEPY 2
#define EXPR_DOUBT 3
#define EXPR_SAD 4
#define EXPR_ANGRY 5

extern bool setAvatarExpr(int expr);
extern void setAvatarBalloon(String msg);
extern void clearAvatarBalloon();

// -- Request ---------
#define REQ_MSG_CLS 0
#define REQ_SPEAK_ADJUST 1
#define REQ_BALOON_ADJUST 2
#define REQ_SPEAK_BALOON_ADJUST 3
#define REQ_SPEAK 4
#define REQ_BALOON 5
#define REQ_SV_MD_ADJUST 9
// -------------------------
// void sendReq(int reqNo,String msg);


// -----------------------------------
// void stackchan(String speakStr,int expr, String balloonStr );

// -----------------------------------
#endif
