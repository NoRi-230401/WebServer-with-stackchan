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

static box_t BOX_STATUS_LINE_NEXT;
static box_t BOX_STATUS_LINE_ONOFF;
static box_t BOX_STATUS_LINE_PREV;
static box_t BOX_SERVO;
static box_t BOX_SYSINFO;
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
    webpage = "statusLine Next";
    BTN_REQ = BtnREQ_UA;
    return;
  }
  else if (arg_mode == "BTNUC")
  {
    webpage = "statusLine Prev";
    BTN_REQ = BtnREQ_UC;
    return;
  }
  else if (arg_mode == "BTNUB")
  {
    webpage = "statusLine OnOff";
    BTN_REQ = BtnREQ_UB;
    return;
  }
  else if (arg_mode == "BOX_SERVO" || arg_mode == "BTNMA" )
  {
    BTN_REQ = BtnREQ_MA;
    webpage = "BtnMA: BoxServo";
    return;
  }
  else if (arg_mode == "BOX_SYSINFO" || arg_mode == "BTNMC")
  {
    BTN_REQ = BtnREQ_MC;
    webpage = "BtnMC: SysInfo Disp";
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

  case BtnREQ_MA:
    BtnMA_Do();
    break;

  case BtnREQ_MC:
    BtnMC_Do();
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
        if (BOX_STATUS_LINE_NEXT.contain(t.x, t.y) && statusLineOnOffState)
          // StatusLineDoNext();
          BtnUA_Do();

        if (BOX_STATUS_LINE_ONOFF.contain(t.x, t.y) )
          // StatusLineDoOnOff();
          BtnUB_Do();

        if (BOX_STATUS_LINE_PREV.contain(t.x, t.y) && statusLineOnOffState)
          // StatusLineDoPrev();
          BtnUC_Do();

        if (BOX_SERVO.contain(t.x, t.y))
          // BoxServoDo();
          BtnMA_Do();

        if (BOX_SYSINFO.contain(t.x, t.y))
          // sysInfoDispOnOff();
          BtnMC_Do();

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

  Serial.println("StatusLineNext");
  statusLineNext();
}

void BtnUB_Do()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();
  
  Serial.println("StatusLineOnOff");
  statusLineOnOff();  
}

void BtnUC_Do()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  Serial.println("StatusLinePrev");
  statusLinePrev();
}

void BtnMC_Do()
{
  tone(1);

  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();
  else
    sysInfoDispStart(0);

  // sysInfoDispOnOff();
}


void BtnMA_Do()
{
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  BoxServoDo();
}



// void sysInfoDispOnOff()
// {
//   if (SYSINFO_DISP_STATE)
//     sysInfoDispEnd();
//   else
//     sysInfoDispStart(0);
// }

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
  tone(1);
  if (SYSINFO_DISP_STATE)
    sysInfoDispEnd();

  SST_ChatGPT();
  // BoxSttDo();
}

// void BoxSttDo()
// {
//   SST_ChatGPT();
// }
//-----------------------------------------------

// void StatusLineOnOff()
// {
//   // tone(1);
//   if (SYSINFO_DISP_STATE)
//     sysInfoDispEnd();

//   Serial.println("StatusLineOnOff");
//   statusLineOnOff();
// }

// void StatusLineNext()
// {
//   // tone(1);
//   if (SYSINFO_DISP_STATE)
//     sysInfoDispEnd();

//   Serial.println("StatusLineNext");
//   statusLineNext();
// }

// void StatusLinePrev()
// {
//   // tone(1);
//   if (SYSINFO_DISP_STATE)
//     sysInfoDispEnd();

//   Serial.println("StatusLinePrev");
//   statusLinePrev();
// }

void BoxTouchSetup()
{
  int w100 = M5.Display.width();
  int w50 = w100 / 2;
  int w25 = w100 / 4;
  int h100 = M5.Display.height();
  int h50 = h100 / 2;
  int h25 = h100 / 4;

  BOX_STATUS_LINE_NEXT.setupBox(0, 0, w25, h25);                    // 上左
  BOX_STATUS_LINE_ONOFF.setupBox(w50 - w25 / 2 - 1, 0, w25, h25);   // 上中
  BOX_STATUS_LINE_PREV.setupBox(w100 - w25 - 1, 0, w25, h25);       // 上右

  // BOX_SERVO.setupBox(w50 - w25 - 1, h50 - (h25 / 2) - 1, w25, h25); // 中心
  BOX_SERVO.setupBox(0, h50 - (h25 / 2) - 1, w25, h25);               // 中左
  BOX_SYSINFO.setupBox(w100-w25-1, h50 - (h25 / 2) - 1 , w25, h25);   // 中右
}
