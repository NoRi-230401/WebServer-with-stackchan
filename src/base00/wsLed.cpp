// ----------------------------<wsLed.cpp>------------------------------------
#include "wsLed.h"

#define LED_BRIGHT 155

bool LED_OnOff_STATE = true;
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); // 800kHzでNeoPixelを駆動 おまじない行

// M5Stack Core2 AWS
const uint16_t rLED[rLED_LEN] = {0, 1, 2, 3};
const uint16_t lLED[lLED_LEN] = {8, 7, 6, 5};
const uint16_t LED_ROLL[LED_LEN] = {0, 1, 2, 3, 5, 6, 7, 8};

void M5LedBegin()
{
  pixels.begin();
  pixels.clear();
  pixels.show();
  LED_OnOff_STATE = true;
}


void ledRed()
{
  pixels.clear();
  pixels.setPixelColor(rLED[0], LED_BRIGHT, 0, 0);
  pixels.setPixelColor(lLED[0], LED_BRIGHT, 0, 0);
  pixels.show();
}

void ledGreen()
{
  pixels.clear();
  pixels.setPixelColor(rLED[0], 0,LED_BRIGHT,  0);
  pixels.setPixelColor(lLED[0], 0,LED_BRIGHT,  0);
  pixels.show();
}

void ledBlue()
{
  pixels.clear();
  pixels.setPixelColor(rLED[0], 0,0,LED_BRIGHT);
  pixels.setPixelColor(lLED[0], 0,0,LED_BRIGHT);
  pixels.show();
}


void ledClearAll()
{ // 全てのLEDを消灯
  pixels.clear();
  pixels.show();
}

// ----- LED表示用ラッパー関数　-----------
void ledSetColor2(uint16_t n, uint32_t c)
{
  if (LED_OnOff_STATE)
    pixels.setPixelColor(n, c);
}

void ledSetColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
  if (LED_OnOff_STATE)
    pixels.setPixelColor(n, r, g, b);
}

uint32_t ledGetColorNo(uint8_t r, uint8_t g, uint8_t b)
{
  if (LED_OnOff_STATE)
    return (pixels.Color(r, g, b));
  else
    return 0;
}

void ledShow()
{
  if (LED_OnOff_STATE)
    pixels.show();
}

void ledClear()
{
  if (LED_OnOff_STATE)
    pixels.clear();
}


void ledMoveSec(uint16_t sec)
{
  if (!LED_OnOff_STATE)
    return;

  int val = sec % 10;
  // Serial.println( "sec = " + String(sec,DEC) + "  val = " + String(val,DEC));
  // return;

  switch (val)
  {
  case 0: // 0-red
    pixels.clear();
    pixels.setPixelColor(rLED[0], LED_BRIGHT, 0, 0);
    pixels.setPixelColor(lLED[0], LED_BRIGHT, 0, 0);
    pixels.show();
    break;

  case 1: // 0-green
  case 9:
    pixels.clear();
    pixels.setPixelColor(rLED[0], 0, LED_BRIGHT, 0);
    pixels.setPixelColor(lLED[0], 0, LED_BRIGHT, 0);
    pixels.show();
    break;

  case 2: // 1-green
  case 8:
    pixels.clear();
    pixels.setPixelColor(rLED[1], 0, LED_BRIGHT, 0);
    pixels.setPixelColor(lLED[1], 0, LED_BRIGHT, 0);
    pixels.show();
    break;

  case 3: // 2-green
  case 7:
    pixels.clear();
    pixels.setPixelColor(rLED[2], 0, LED_BRIGHT, 0);
    pixels.setPixelColor(lLED[2], 0, LED_BRIGHT, 0);
    pixels.show();
    break;

  case 4: // 3-green
  case 6:
    pixels.clear();
    pixels.setPixelColor(rLED[3], 0, LED_BRIGHT, 0);
    pixels.setPixelColor(lLED[3], 0, LED_BRIGHT, 0);
    pixels.show();
    break;

  case 5: // 3-blue
    pixels.clear();
    pixels.setPixelColor(rLED[3], 0, 0, LED_BRIGHT);
    pixels.setPixelColor(lLED[3], 0, 0, LED_BRIGHT);
    pixels.show();
    break;

  default:
    break;
  }
}

void blueAndRedLedOn()
{
  if (!LED_OnOff_STATE)
    return;

  // --- right blue
  pixels.setPixelColor(rLED[0], 0, 0, LED_BRIGHT);
  pixels.setPixelColor(rLED[3], 0, 0, LED_BRIGHT);
  // --- left red
  pixels.setPixelColor(lLED[0], LED_BRIGHT, 0, 0);
  pixels.setPixelColor(lLED[3], LED_BRIGHT, 0, 0);
  pixels.show();
  delay(1000);

  ledClearAll();
}
