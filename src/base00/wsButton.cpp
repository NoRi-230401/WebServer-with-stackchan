// ----------------------------<wsButton.cpp>------------------------------------
#include "wsButton.h"

bool KEYLOCK_STATE = false;
int BTN_REQ = 0;

struct box_t
{
  int x;
  int y;
  int w;
  int h;
  int touch_id = -1;

  void setupBox(int x, int y, int w, int h)
  {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    // Serial.println("x = " + String(x, DEC));
    // Serial.println("y = " + String(y, DEC));
    // Serial.println("wide = " + String(w, DEC));
    // Serial.println("height = " + String(h, DEC) + "\n");
  }

  bool contain(int x, int y)
  {
    return this->x <= x && x < (this->x + this->w) && this->y <= y && y < (this->y + this->h);
  }
};

static box_t BOX_STATUS_LINE_SELECT;
static box_t BOX_STATUS_LINE_ONE;
static box_t BOX_STATUS_LINE_ONOFF;
static box_t BOX_SERVO;
// static box_t BOX_STT;

void wsHandleBtn(String arg)
{
  String arg_mode = arg;
  if (arg_mode == "")
    return;

  arg_mode.toUpperCase();
  if (arg_mode == "BTNA")
  {
    webpage = "self-talk On/Off changed";
    BTN_REQ = BtnREQ_A;
    return;
  }
  else if (arg_mode == "BTNB")
  {
    webpage = "talk to chatGPT";
    BTN_REQ = BtnREQ_B;
    return;
  }
  else if (arg_mode == "BTNC")
  {
    webpage = "timer Start/Stop changed";
    BTN_REQ = BtnREQ_C;
    return;
  }
  else if (arg_mode == "BTNUA")
  {
    webpage = "statusLineSelect";
    BTN_REQ = BtnREQ_UA;
    return;
  }
  else if (arg_mode == "BTNUB")
  {
    webpage = "statusLineOne";
    BTN_REQ = BtnREQ_UB;
    return;
  }
  else if (arg_mode == "BTNUC")
  {
    webpage = "statusLineOnOff";
    BTN_REQ = BtnREQ_UC;
    return;
  }
  else if (arg_mode == "BOX_SERVO")
  {
    webpage = BoxServoDo();
    return;
  }
  // else if (arg_mode == "BOX_STT")
  // {
  //   webpage = "talk-with-chatGpt Start";
  //   BTN_REQ = BtnREQ_BOX_STT;
  //   return;
  // }
  else
  {
    BTN_REQ = 0;
    return;
  }

  webpage = "Button Pushed : " + arg;
  Serial.println(webpage);
  return;
}

void BtnReqGet()
{
  if (BTN_REQ == 0)
    return;

  Serial.print("BTN_REQ = ");
  Serial.println(BTN_REQ, DEC);

  int req = BTN_REQ;
  BTN_REQ = 0;

  switch (req)
  {
  case BtnREQ_A:
    BtnA_Do();
    break;

  case BtnREQ_B:
    BtnB_Do();
    break;

  case BtnREQ_C:
    BtnC_Do();
    break;

  case BtnREQ_UA:
    BtnUA_Do();
    break;

  case BtnREQ_UB:
    BtnUB_Do();
    break;

  case BtnREQ_UC:
    BtnUC_Do();
    break;

  case BtnREQ_BOX_SERVO:
    BoxServoDo();
    break;

    // case BtnREQ_BOX_STT:
    //   BoxSttDo();
    //   break;

  default:
    break;
  }

  return;
}

void ButtonManage()
{
  BtnReqGet();

  M5.update();

// *** 画面タッチボタン ***
#if defined(ARDUINO_M5STACK_Core2) || defined(ARDUINO_M5STACK_CORES3)
  auto count = M5.Touch.getCount();
  if (count)
  {
    if (!KEYLOCK_STATE)
    {
      auto t = M5.Touch.getDetail();
      if (t.wasPressed())
      {
        if (BOX_STATUS_LINE_SELECT.contain(t.x, t.y) && statusLineOnOffState && !statusLineOneState)
          StatusLineDoSelect();
        if (BOX_STATUS_LINE_ONE.contain(t.x, t.y) && !statusLineOnOffState && !statusLineOneState)
          StatusLineDoOne();
        if (BOX_STATUS_LINE_ONOFF.contain(t.x, t.y) && !statusLineOneState)
          StatusLineDoOnOff();
        // if (BOX_STT.contain(t.x, t.y) && (!mp3->isRunning())) BoxSttDo();
        if (BOX_SERVO.contain(t.x, t.y))
          BoxServoDo();
      }
    }
  }
#endif

  // ** (BtnA) self-talk OnOff
  if (M5.BtnA.wasPressed())
  {
    if (!KEYLOCK_STATE)
      BtnA_Do();
  }

  // ** (BtnB) talk to chatGPT --STT
  if (M5.BtnB.wasPressed())
  {
    if (!KEYLOCK_STATE)
      BtnB_Do();
  }

  // ** (BtnC) Timer Start/Stop
  if (M5.BtnC.wasPressed())
  {
    if (!KEYLOCK_STATE)
      BtnC_Do();
  }
}

void BtnA_Do()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  if (!RANDOM_SPEAK_STATE)
    RANDOM_SPEAK_ON_GET = true;
  else
    RANDOM_SPEAK_OFF_GET = true;
}

void BtnUA_Do()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  statusLineSelect();
}

void BtnUB_Do()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();
  
  statusLineOne();
}

void BtnUC_Do()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  statusLineOnOff();
}


// 240113 btn change by NoRi
// ----------------------------
// BtnB --> talk to ChatGPT
// BtnC --> TimerOn/Off
void BtnC_Do()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  if (!TIMER_STARTED)
  { // ---- Timer 開始 ------
    randomSpeakStop2();
    TIMER_GO_GET = true;
    TIMER_STOP_GET = false;
  }
  else
  { // --- Timer 停止 ------
    TIMER_STOP_GET = true;
    TIMER_GO_GET = false;
  }
}

void BtnB_Do()
{
  BoxSttDo();
}

void BoxSttDo()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  SST_ChatGPT();
}
//-----------------------------------------------

void StatusLineDoOnOff()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  statusLineOnOff();
}

void StatusLineDoSelect()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  statusLineSelect();
}

void StatusLineDoOne()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  // Serial.println("batteryIconDoOnce()");
  statusLineOne();
}

void BoxTouchSetup()
{
  int w100 = M5.Display.width();
  int w50 = w100 / 2;
  int w25 = w100 / 4;
  int h100 = M5.Display.height();
  int h50 = h100 / 2;
  int h25 = h100 / 4;

  BOX_STATUS_LINE_SELECT.setupBox(0, 0, w25, h25);              // 上左
  BOX_STATUS_LINE_ONE.setupBox(w50 - w25 / 2 - 1, 0, w25, h25); // 上中
  BOX_STATUS_LINE_ONOFF.setupBox(w100 - w25 - 1, 0, w25, h25);  // 上右

  // BOX_STT.setupBox(0, h50 - (h25 / 2) - 1, w25, h25);             // 中左
  // BOX_SERVO.setupBox(w100-w25-1, h50 - (h25 / 2) - 1 , w25, h25); // 中右
  BOX_SERVO.setupBox(w50 - w25 - 1, h50 - (h25 / 2) - 1, w25, h25); // 中心
}
