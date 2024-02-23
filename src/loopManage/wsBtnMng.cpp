// ----------------------------<wsBtnMng.cpp>------------------------------------
#include "wsBtnMng.h"

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
  }

  bool contain(int x, int y)
  {
    return this->x <= x && x < (this->x + this->w) && this->y <= y && y < (this->y + this->h);
  }
};
static box_t BOX_STATUS_NEXT;
static box_t BOX_STATUS_MODE_SELECT;
static box_t BOX_STATUS_PREV;
static box_t BOX_SERVO;
// static box_t BOX_SYSINFO;

void buttonManage()
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
        if (BOX_STATUS_NEXT.contain(t.x, t.y) && statusMode != STM0X_NONE)
          // next
          BtnUA_Do();

        if (BOX_STATUS_MODE_SELECT.contain(t.x, t.y))
          // statusModeSelect
          BtnUB_Do();

        if (BOX_STATUS_PREV.contain(t.x, t.y) && statusMode != STM0X_NONE)
          // prev
          BtnUC_Do();

        if (BOX_SERVO.contain(t.x, t.y) && statusMode != STM2X_SYSINFO)
          // BoxServo
          BtnMB_Do();
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
  else if (arg_mode == "BTNB" || arg_mode == "BOX_STT")
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
    webpage = "StatusPrev";
    BTN_REQ = BtnREQ_UA;
    return;
  }
  else if (arg_mode == "BTNUC")
  {
    webpage = "StatusNext";
    BTN_REQ = BtnREQ_UC;
    return;
  }
  else if (arg_mode == "BTNUB")
  {
    webpage = "StatusModeSelect";
    BTN_REQ = BtnREQ_UB;
    return;
  }
  else if (arg_mode == "BOX_SERVO" || arg_mode == "BTNMB")
  {
    BTN_REQ = BtnREQ_MB;
    webpage = "BtnMB: BoxServo";
    return;
  }
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

  case BtnREQ_MB:
    BtnMB_Do();
    break;

  default:
    break;
  }
  return;
}

void BtnUA_Do()
{
  tone(1);
  Serial.println("StatusPrev");
  statusPrev();
}

void BtnUB_Do()
{
  tone(1);
  Serial.println("StatusModeSelect");
  statusModeSelect();
}

void BtnUC_Do()
{
  tone(1);
  Serial.println("StatusNext");
  statusNext();
}

void BtnMB_Do()
{
  tone(1);
  BoxServoDo();
}

void BtnA_Do()
{
  tone(1);

  switch (statusMode)
  {
  case STM0X_NONE:
  case STM1X_LINE:
    if (!RANDOM_SPEAK_STATE)
      RANDOM_SPEAK_ON_GET = true;
    else
      RANDOM_SPEAK_OFF_GET = true;
    break;

  case STM3X_SETTING:
    Serial.println("Will go to lobby screen");
    SDU_lobby();
    SDU_disp();
    delay(50);
    break;

  default:
    break;
  }
}

void BtnB_Do()
{
  tone(1);
  switch (statusMode)
  {
  case STM0X_NONE:
  case STM1X_LINE:
    SST_ChatGPT();
    break;

  case STM3X_SETTING:
    Serial.println("Will Load menu binary");
    SDU_fromSD();
    // updateFromFS(SD);
    ESP.restart();
    break;

  default:
    break;
  }
}


void BtnC_Do()
{
  tone(1);

  switch (statusMode)
  {
  case STM0X_NONE:
  case STM1X_LINE:
    if (!TM_STARTED)
    { // ---- Timer 開始 ------
      randomSpeakStop2();
      TM_GO_GET = true;
      TM_STOP_GET = false;
    }
    else
    { // --- Timer 停止 ------
      TM_STOP_GET = true;
      TM_GO_GET = false;
    }
    break;

  case STM3X_SETTING:
    avatarStop();
    avatarStop2();
    Serial.println("Will store BIN_FILE to SD");
    SDU_saveBin(WSS_SD_BIN);
    SDU_disp();
    delay(50);
    break;

  default:
    break;
  }
}

void BoxTouchSetup()
{
  int w100 = M5.Display.width();
  int w50 = w100 / 2;
  int w25 = w100 / 4;
  int h100 = M5.Display.height();
  int h50 = h100 / 2;
  int h25 = h100 / 4;

  BOX_STATUS_NEXT.setupBox(0, 0, w25, h25);                          // 上左
  BOX_STATUS_MODE_SELECT.setupBox(w50 - (w25 / 2) - 1, 0, w25, h25); // 上中
  BOX_STATUS_PREV.setupBox(w100 - w25 - 1, 0, w25, h25);             // 上右

  BOX_SERVO.setupBox(w50 - (w25 / 2) - 1, h50 - (h25 / 2) - 1, w25, h25); // 中中
}
