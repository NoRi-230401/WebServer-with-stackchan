// ---------------------------< wsServo.cpp >------------------------------------
#include "wsServo.h"

ServoEasing servo_x;
ServoEasing servo_y;
const String SERVO_SPIFFS = "/wsServo.json";
extern const String SV_MD_NAME[];
const String SV_MD_NAME[] = {"Moving", "Home", "Random", "Center", "Swing", "Stop", "Adjust"};
const String SV_AXIS_NAME[] = {"X", "Y", "XY"};
const String SERVO_ITEM[] = {"servo", "servoPort", "servoMode", "servoHomeX", "servoHomeY"};
String SV_PORT = "";
const String jsonSERVO = "{\"servo\":[{\"servo\":\"on\",\"servoPort\":\"portC\",\"servoMode\":\"home\",\"servoHomeX\":\"90\",\"servoHomeY\":\"80\"}]}";
bool SV_USE = true;
bool SV_ADJUST_STATE = false;
bool SV_MD_RANDOM_1st = false;
bool SV_MD_MOVING_1st = false;
int SV_MD = SV_MD_MOVING;
int SV_MD_NAME_NO = SV_MD_MOVING;
int SV_HOME_X = 90;
int SV_HOME_Y = 80;
int SV_PT_X = SV_HOME_X;
int SV_PT_Y = SV_HOME_Y;
int SV_PREV_PT_X = SV_HOME_X;
int SV_PREV_PT_Y = SV_HOME_Y;
int SV_NEXT_PT_X;
int SV_NEXT_PT_Y;
int SV_SWING_CNT = 0;
int SV_SWING_AXIS = 2;
int SV_SWING_LEN = 3;
int SV_PIN_X;
int SV_PIN_Y;

void servo(void *args)
{
  float gazeX, gazeY;
  DriveContext *ctx = (DriveContext *)args;
  Avatar *avatar = ctx->getAvatar();

  for (;;)
  {
    if (SV_USE)
    {
      int mode = SV_MD;

      switch (mode)
      {
      case SV_MD_MOVING:
        avatar->getGaze(&gazeY, &gazeX);
        sv_setEaseToX(SV_HOME_X + (int)(15.0 * gazeX));
        if (gazeY < 0)
        {
          int tmp = (int)(10.0 * gazeY);
          if (tmp > 10)
            tmp = 10;
          sv_setEaseToY(SV_HOME_Y + tmp);
        }
        else
        {
          sv_setEaseToY(SV_HOME_Y + (int)(10.0 * gazeY));
        }
        synchronizeAllServosStartAndWaitForAllServosToStop();

        if (SV_MD_MOVING_1st)
        {
          SV_MD_MOVING_1st = false;
          stackchanBalllonAdj(SV_MD_NAME[SV_MD_MOVING]);
        }
        break;

      case SV_MD_HOME:
        SV_NEXT_PT_X = SV_HOME_X;
        SV_NEXT_PT_Y = SV_HOME_Y;
        if ((SV_NEXT_PT_X != SV_PT_X) || (SV_NEXT_PT_Y != SV_PT_Y))
        {
          stackchanBalllonAdj(SV_MD_NAME[SV_MD_HOME]);
        }
        sv_setEaseToXY(SV_NEXT_PT_X, SV_NEXT_PT_Y);
        synchronizeAllServosStartAndWaitForAllServosToStop();
        break;

      case SV_MD_RANDOM: // Random Mode
        if (SV_MD_RANDOM_1st)
        {
          SV_MD_RANDOM_1st = false;
          stackchanBalllonAdj(SV_MD_NAME[SV_MD_RANDOM]);
        }
        SV_random();
        break;

      case SV_MD_CENTER:
        SV_NEXT_PT_X = SV_CENTER_X;
        SV_NEXT_PT_Y = SV_CENTER_Y;
        if ((SV_NEXT_PT_X != SV_PT_X) || (SV_NEXT_PT_Y != SV_PT_Y))
        {
          stackchanBalllonAdj(SV_MD_NAME[SV_MD_CENTER]);
        }
        sv_setEaseToXY(SV_NEXT_PT_X, SV_NEXT_PT_Y);
        synchronizeAllServosStartAndWaitForAllServosToStop();
        break;

      case SV_MD_SWING:
        for (int i = 0; i < SV_SWING_LEN; i++)
        {
          servoSwing(SV_SWING_AXIS, i + 1, SV_SWING_LEN);
        }
        SV_MD = SV_MD_NONE;
        SV_SWING_LEN = 0;
        break;

      case SV_MD_STOP:
        SV_MD = SV_MD_NONE;
        stackchanBalllonAdj(SV_MD_NAME[SV_MD_STOP]);
        break;

      case SV_MD_ADJUST:
        KEYLOCK_STATE = true;
        SV_ADJUST_STATE = true;
        SV_MD = SV_MD_NONE;
        sv_setEaseToXY(SV_CENTER_X, SV_CENTER_Y);
        synchronizeAllServosStartAndWaitForAllServosToStop();
        stackchanSpkBalllonAdj("サーボ調整");
        break;

      case SV_MD_POINT:
        if ((SV_NEXT_PT_X != SV_PT_X) || (SV_NEXT_PT_Y != SV_PT_Y))
        {
          String msg = " X=" + String(SV_NEXT_PT_X, DEC) + " Y=" + String(SV_NEXT_PT_Y, DEC);
          stackchanBalllonAdj(msg);
        }
        sv_setEaseToXY(SV_NEXT_PT_X, SV_NEXT_PT_Y);
        synchronizeAllServosStartAndWaitForAllServosToStop();
        break;

      case SV_MD_DELTA:
        if ((SV_NEXT_PT_X != SV_PT_X) || (SV_NEXT_PT_Y != SV_PT_Y))
        {
          String msg = " X=" + String(SV_NEXT_PT_X, DEC) + " Y=" + String(SV_NEXT_PT_Y, DEC);
          stackchanBalllonAdj(msg);
        }
        sv_setEaseToXY(SV_NEXT_PT_X, SV_NEXT_PT_Y);
        synchronizeAllServosStartAndWaitForAllServosToStop();
        break;

      case SV_MD_NONE:
        break;

      default:
        return;
      }

      delay(50);
    }
  }
}

bool jsonSERVOinit(DynamicJsonDocument &jsonDoc)
{
  return (jsonStrSave(jsonDoc, jsonSERVO, SERVO_SPIFFS));
}

void BoxServoDo()
{
  if (SV_USE)
  {
    int svMode = SV_MD;
    if (svMode > SV_MD_CENTER)
      SV_MD = SV_MD_MOVING;

    SV_MD = (svMode + 1) % (SV_MD_CENTER + 1);
    Serial.println("Button Pushed : BOX_SERVO");
    String msg = "ServoMode = " + SV_MD_NAME[SV_MD];
    Serial.println(msg);
    webpage = msg;
  }
}

void wsHandleServo(String swingXYS, String swingXS, String swingYS,
  String pointXS, String pointYS, String deltaXS, String deltaYS,
  String txS, String modeS)
{
  // ---- swingXY -------
  if (swingXYS != "")
  { // swingXY
    SV_SWING_CNT = 0;
    SV_SWING_LEN = 1;
    SV_SWING_AXIS = SV_SWING_AXIS_XY;

    int tmp_repeat = swingXYS.toInt();
    if (tmp_repeat > 10)
      SV_SWING_LEN = 10;
    else if (tmp_repeat <= 0)
      SV_SWING_LEN = 1;
    else
      SV_SWING_LEN = tmp_repeat;

    SV_MD = SV_MD_SWING;
    servoSetup2();
    webpage = "SERVO : SwingXY " + String(SV_SWING_LEN, DEC) + " repeat";
    return;
  }

  // ---- swingX -------
  if (swingXS != "")
  { // swingX
    SV_SWING_CNT = 0;
    SV_SWING_LEN = 1;
    SV_SWING_AXIS = SV_SWING_AXIS_X;

    int tmp_repeat = swingXS.toInt();
    if (tmp_repeat > 10)
      SV_SWING_LEN = 10;
    else if (tmp_repeat <= 0)
      SV_SWING_LEN = 1;
    else
      SV_SWING_LEN = tmp_repeat;

    SV_MD = SV_MD_SWING;
    servoSetup2();
    webpage = "SERVO : SwingX " + String(SV_SWING_LEN, DEC) + " repeat";
    return;
  }

  // ---- swingY -------
  if (swingYS != "")
  { // swingY
    SV_SWING_CNT = 0;
    SV_SWING_LEN = 1;
    SV_SWING_AXIS = SV_SWING_AXIS_Y;

    int tmp_repeat = swingYS.toInt();
    if (tmp_repeat > 10)
      SV_SWING_LEN = 10;
    else if (tmp_repeat <= 0)
      SV_SWING_LEN = 1;
    else
      SV_SWING_LEN = tmp_repeat;

    SV_MD = SV_MD_SWING;
    servoSetup2();
    webpage = "SERVO : SwingY " + String(SV_SWING_LEN, DEC) + " repeat";
    return;
  }

  // ---- pointX, pointY -------
  if ((pointXS != "") || (pointYS != ""))
  { // pointX
    // --------
    int x_val = SV_PT_X; // default

    if (pointXS != "")
      x_val = pointXS.toInt();

    SV_NEXT_PT_X = x_val;

    if (SV_NEXT_PT_X < SV_X_MIN)
      SV_NEXT_PT_X = SV_X_MIN;

    if (SV_NEXT_PT_X > SV_X_MAX)
      SV_NEXT_PT_X = SV_X_MAX;

    // y-value
    // --------
    int y_val = SV_PT_Y; // default

    if (pointYS != "")
      y_val = pointYS.toInt();

    SV_NEXT_PT_Y = y_val;

    if (SV_NEXT_PT_Y < SV_Y_MIN) // 50 to 100
      SV_NEXT_PT_Y = SV_Y_MIN;

    if (SV_NEXT_PT_Y > SV_Y_MAX)
      SV_NEXT_PT_Y = SV_Y_MAX;

    SV_MD = SV_MD_POINT;
    servoSetup2();
    webpage = "SERVO : Point X = " + String(SV_NEXT_PT_X, DEC) + " Y = " + String(SV_NEXT_PT_Y);
    Serial.println(webpage);
    return;
  }

  // ---- deltaX, deltaY -------
  if ((deltaXS != "") || (deltaYS != ""))
  {
    // deltaX
    // -------
    int x_delta = 0;
    SV_NEXT_PT_X = SV_PT_X;

    if (deltaXS != "")
      x_delta = deltaXS.toInt();

    SV_NEXT_PT_X += x_delta;

    if (SV_NEXT_PT_X < SV_X_MIN) // 0 to 180
      SV_NEXT_PT_X = SV_X_MIN;

    if (SV_NEXT_PT_X > SV_X_MAX)
      SV_NEXT_PT_X = SV_X_MAX;

    // deltaY
    // --------
    int y_delta = 0;
    SV_NEXT_PT_Y = SV_PT_Y;

    if (deltaYS != "")
      y_delta = deltaYS.toInt();

    SV_NEXT_PT_Y += y_delta;

    if (SV_NEXT_PT_Y < SV_Y_MIN) // 50 to 100
      SV_NEXT_PT_Y = SV_Y_MIN;

    if (SV_NEXT_PT_Y > SV_Y_MAX)
      SV_NEXT_PT_Y = SV_Y_MAX;

    SV_MD = SV_MD_DELTA;
    servoSetup2();

    webpage = "SERVO : Delta X = " + String(SV_NEXT_PT_X, DEC) + " Y = " + String(SV_NEXT_PT_Y);
    Serial.println(webpage);
    return;
  }

  // ---- TX -------
  txS.toUpperCase();
  if (txS != "")
  { // TX
    if (txS == "XY")
      webpage = "SERVO TX=XY : X  = " + String(SV_PT_X, DEC) + " Y = " + String(SV_PT_Y, DEC);

    else if (txS == "X")
      webpage = "SERVO TX=X  : X  = " + String(SV_PT_X, DEC);

    else if (txS == "Y")
      webpage = "SERVO TX=Y  : Y  = " + String(SV_PT_Y, DEC);

    return;
  }

  // ---- MODE -------
  modeS.toUpperCase();
  if (modeS != "")
  {
    webpage = "servo?mode=" + modeS;
    Serial.println(webpage);

    if (modeS == "MOVING")
    { // moving
      SV_MD = SV_MD_MOVING;
      SV_MD_MOVING_1st = true;
      stackchanNow(-1, "");
      servoSetup2();
      return;
    }

    else if (modeS == "HOME")
    { // home
      SV_MD = SV_MD_HOME;
      servoSetup2();
      return;
    }

    else if (modeS == "RANDOM")
    { // random
      SV_MD = SV_MD_RANDOM;
      SV_MD_RANDOM_1st = true;
      return;
    }

    else if (modeS == "CENTER")
    { // center
      SV_MD = SV_MD_CENTER;
      servoSetup2();
      return;
    }

    else if (modeS == "STOP")
    { // stop
      SV_MD = SV_MD_STOP;
      servoSetup2();
      return;
    }

    webpage = "NG";
    return;
  }

  webpage = "NG";
}

void stackchanBalllonAdj(String balloonMsg)
{
  if (!SV_ADJUST_STATE)
    return;

  stackchanNow(-1, balloonMsg);
}

void stackchanSpkBalllonAdj(String msg)
{
  if (!SV_ADJUST_STATE)
    return;
  stackchanReq(msg, EXPR_HAPPY, msg, EXPR_NEUTRAL);
}

void servoSetup2()
{ // 起動中の変更
  if (SV_USE)
  {
    if (SV_MD == SV_MD_MOVING)
    {
      servo_x.setEasingType(EASE_QUADRATIC_IN_OUT);
      servo_y.setEasingType(EASE_QUADRATIC_IN_OUT);
      setSpeedForAllServos(30);
    }
    else
    {
      servo_x.setEasingType(EASE_LINEAR);
      servo_y.setEasingType(EASE_LINEAR);
      setSpeedForAllServos(60);
    }
  }
}

void servoSetup()
{ // 最初の設定
  if (SV_USE)
  {
    char msg[100];
    uint8_t ret;

    ret = servo_x.attach(SV_PIN_X, SV_HOME_X, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
    if (ret == 0)
      sprintf(msg, "Error attaching servo X PIN=%d HOM=%d", SV_PIN_X, SV_HOME_X);
    else
      sprintf(msg, "Success attaching servo X PIN=%d HOME=%d RET=%d", SV_PIN_X, SV_HOME_X, ret);

    Serial.println(msg);

    ret = servo_y.attach(SV_PIN_Y, SV_HOME_Y, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
    if (ret == 0)
      sprintf(msg, "Error attaching servo Y PIN=%d HOME=%d", SV_PIN_Y, SV_HOME_Y);
    else
      sprintf(msg, "Success attaching servo Y PIN=%d HOME=%d RET=%d", SV_PIN_Y, SV_HOME_Y, ret);

    Serial.println(msg);

    if (SV_ADJUST_STATE)
    { // サーボ調整モード
      servo_x.setEasingType(EASE_LINEAR);
      servo_y.setEasingType(EASE_LINEAR);
      setSpeedForAllServos(60);
      sv_setEaseToX(SV_CENTER_X);
      sv_setEaseToY(SV_CENTER_Y);
    }
    else
    {
      servo_x.setEasingType(EASE_QUADRATIC_IN_OUT);
      servo_y.setEasingType(EASE_QUADRATIC_IN_OUT);
      setSpeedForAllServos(30);
      sv_setEaseToX(SV_HOME_X);
      sv_setEaseToY(SV_HOME_Y);
    }
    synchronizeAllServosStartAndWaitForAllServosToStop();
  }
}

void servoSwing(int sw_mode, int repeatNum, int len)
{
  String msg;

  if (repeatNum == 1)
  {
    msg = " Swing" + SV_AXIS_NAME[sw_mode] + String(len, DEC);
    stackchanBalllonAdj(msg);
    sv_setEaseToXY(SV_CENTER_X, SV_CENTER_Y);
    synchronizeAllServosStartAndWaitForAllServosToStop();
    delay(1000);
  }

  int mode = sw_mode;

  switch (mode)
  {
  case SV_SWING_AXIS_XY:
    msg = "X 90 -> 0  ";
    stackchanBalllonAdj(msg);
    sv_easeToX(0);

    msg = "X 0 -> 180  ";
    stackchanBalllonAdj(msg);
    sv_easeToX(180);

    msg = "X 180 -> 90  ";
    stackchanBalllonAdj(msg);
    sv_easeToX(90);

    msg = "Y 90 -> 50  ";
    stackchanBalllonAdj(msg);
    sv_easeToY(50);

    msg = "Y 50 -> 90  ";
    stackchanBalllonAdj(msg);
    sv_easeToY(90);
    break;

  case SV_SWING_AXIS_X:
    msg = "X 90 -> 0  ";
    stackchanBalllonAdj(msg);
    sv_easeToX(0);

    msg = "X 0 -> 180  ";
    stackchanBalllonAdj(msg);
    sv_easeToX(180);

    msg = "X 180 -> 90  ";
    stackchanBalllonAdj(msg);
    sv_easeToX(90);
    break;

  case SV_SWING_AXIS_Y:
    msg = "Y 90 -> 50  ";
    stackchanBalllonAdj(msg);
    sv_easeToY(50);

    msg = "Y 50 -> 90  ";
    stackchanBalllonAdj(msg);
    sv_easeToY(90);
    break;
  }

  msg = "";
  stackchanBalllonAdj(msg);
}

bool setServo(String item, String setData, DynamicJsonDocument &servoJson)
{
  return (setJsonItem(SERVO_SPIFFS, item, setData, servoJson, "servo"));
}

bool getServo(String item, String &getData, DynamicJsonDocument &servoJson)
{
  return (getJsonItem(SERVO_SPIFFS, item, getData, servoJson, "servo"));
}

void wsServoSetting(String txS, String servoS, String servoPortS,
                    String servoModeS, String servoHomeXS, String servoHomeYS)
{
  DynamicJsonDocument servoJson(SERVOJSON_SIZE);
  // -------------------------------------------------------
  if (txS != "")
  {
    String getStr = "";
    if (getServo(txS, getStr, servoJson))
    {
      webpage = "wsServo.json : " + txS + " = " + getStr;
      Serial.println(webpage);
    }
    return;
  }

  if (servoS != "")
  {
    if (setServo("servo", servoS, servoJson))
    {
      webpage = "wsServo.json : servo = " + servoS;
      Serial.println(webpage);
    }
    return;
  }

  if (servoPortS != "")
  {
    if (setServo("servoPort", servoPortS, servoJson))
    {
      webpage = "wsServo.json : servoPort = " + servoPortS;
      Serial.println(webpage);
    }
    return;
  }

  if (servoModeS != "")
  {
    if (setServo("servoMode", servoModeS, servoJson))
    {
      webpage = "wsServo.json : servoMode = " + servoModeS;
      Serial.println(webpage);
    }
    return;
  }

  if (servoHomeXS != "")
  {
    if (setServo("servoHomeX", servoHomeXS, servoJson))
    {
      webpage = "wsServo.json : servoHomeX = " + servoHomeXS;
      Serial.println(webpage);
    }
    return;
  }

  if (servoHomeYS != "")
  {
    if (setServo("servoHomeY", servoHomeYS, servoJson))
    {
      webpage = "wsServo.json : servoHomeY = " + servoHomeYS;
      Serial.println(webpage);
    }
    return;
  }

  // -------------------------------------------------------
  // HTML形式でファイルを表示する
  {
    if (!jsonRead(FLTYPE_SPIFFS, servoJson, SERVO_SPIFFS))
    {
      Serial.println("faile to Read wsServo.json from SPIFFS");
      return;
    }

    String html = "";
    serializeJsonPretty(servoJson, html);
    Serial.println(html);
    webpage = html;
    return;
  }
}

void servoSetting()
{
  servoInit();
  servoFileRead();
}

void servoInit()
{
  // ****** 初期値設定　**********
  SV_USE = true;
  SV_PORT = "portA";
  SV_PIN_X = SV_PIN_X_CORE2_PA;
  SV_PIN_Y = SV_PIN_Y_CORE2_PA;
  SV_MD = SV_MD_MOVING; // moving
  SV_ADJUST_STATE = false;
  REQUEST_NO = 0; // req none
  SV_MD_NAME_NO = SV_MD_MOVING;
  SV_HOME_X = 90;
  SV_HOME_Y = 80;
}

void servoFileRead()
{
  //----------------------------------
  DynamicJsonDocument servoJson(SERVOJSON_SIZE);

  if (!jsonRead(FLTYPE_SPIFFS, servoJson, SERVO_SPIFFS))
  {
    Serial.println("DeserializationError in wsServo.json in SPIFFS");

    Serial.println("initialize wsServo.json in SPIFFS");
    jsonSERVOinit(servoJson);
  }

  JsonArray jsonArray = servoJson["servo"];
  JsonObject object = jsonArray[0];
  int cnt = 0;

  // servo
  String getStr10 = object[SERVO_ITEM[0]];
  if (getStr10 != "" && (getStr10 != "null"))
  {
    String getData = getStr10;
    getData.toLowerCase();
    if (getData == "off")
      SV_USE = false;

    Serial.println("Servo : " + SERVO_ITEM[0] + " = " + getStr10);
    cnt++;
  }

  // servoPort
  String getStr11 = object[SERVO_ITEM[1]];
  if (getStr11 != "" && (getStr11 != "null"))
  {
    String getData = getStr11;
    getData.toUpperCase();
    if (getData == "PORTC")
    {
      SV_PORT = "portC";
      SV_PIN_X = SV_PIN_X_CORE2_PC;
      SV_PIN_Y = SV_PIN_Y_CORE2_PC;
    }

    Serial.println("Servo : " + SERVO_ITEM[1] + " = " + getStr11);
    cnt++;
  }

  // servoMode
  String getStr12 = object[SERVO_ITEM[2]];
  if (getStr12 != "" && (getStr12 != "null"))
  {
    String getData = getStr12;
    getData.toUpperCase();
    if (getData == "HOME")
    {
      SV_MD = SV_MD_HOME;
      SV_MD_NAME_NO = SV_MD_HOME;
    }
    else if (getData == "ADJUST")
    {
      SV_MD = SV_MD_NONE;
      SV_ADJUST_STATE = true;
      SV_MD_NAME_NO = SV_MD_ADJUST;
      REQUEST_NO = REQ_SV_MD_ADJUST;
    }

    Serial.println("Servo : " + SERVO_ITEM[2] + " = " + getStr12);
    cnt++;
  }

  // servoHomeX
  String getStr13 = object[SERVO_ITEM[3]];
  if (getStr13 != "" && (getStr13 != "null"))
  {
    int getVal = getStr13.toInt(); // 60 <= HomeX <=120
    if (getVal <= 60)
      getVal = 60;

    if (getVal >= 120)
      getVal = 120;

    SV_HOME_X = getVal;

    Serial.println("Servo : " + SERVO_ITEM[3] + " = " + getStr13);
    cnt++;
  }

  // servoHomeY
  String getStr14 = object[SERVO_ITEM[4]];
  if (getStr14 != "" && (getStr14 != "null"))
  {
    int getVal = getStr14.toInt(); // 75 <= HomeY <= 100
    if (getVal <= 75)
      getVal = 75;

    if (getVal >= 100)
      getVal = 100;

    SV_HOME_Y = getVal;

    Serial.println("Servo : " + SERVO_ITEM[4] + " = " + getStr14);
    cnt++;
  }

  Serial.println("** wsServo.json total " + String(cnt, DEC) + " item read **");
}

// 　後にsynchronizeを呼び出す
void sv_setEaseToX(int x)
{
  SV_PREV_PT_X = SV_PT_X;
  SV_PT_X = x;
  servo_x.setEaseTo(x);
}

void sv_setEaseToY(int y)
{
  SV_PREV_PT_Y = SV_PT_Y;
  SV_PT_Y = y;
  servo_y.setEaseTo(y);
}

void sv_setEaseToXY(int x, int y)
{
  sv_setEaseToX(x);
  sv_setEaseToY(y);
}

void sv_setEaseToD_XY(int x, int y, uint32_t millis_for_move)
{
  SV_PREV_PT_X = SV_PT_X;
  SV_PT_X = x;

  SV_PREV_PT_Y = SV_PT_Y;
  SV_PT_Y = y;

  servo_x.setEaseToD(SV_PT_X, millis_for_move);
  servo_y.setEaseToD(SV_PT_Y, millis_for_move);
}

// --- そのままセット ----
void sv_easeToX(int x)
{
  SV_PREV_PT_X = SV_PT_X;
  SV_PT_X = x;
  servo_x.easeTo(x);
}

void sv_easeToY(int y)
{
  SV_PREV_PT_Y = SV_PT_Y;
  SV_PT_Y = y;
  servo_y.easeTo(y);
}

void sv_easeToXY(int x, int y)
{
  sv_easeToY(y);
  sv_easeToX(x);
}

void SV_random()
{
  int random_x = random(45, 135); // 45〜135° でランダム
  int random_y = random(60, 90);  // 50〜90° でランダム
  uint32_t delayTm = random(10);

  sv_setEaseToXY(random_x, random_y);
  synchronizeAllServosStartAndWaitForAllServosToStop();
  delay(500 + 100 * delayTm); // max = 30000mSec delay
}
