// ----------------------------<wsLed.cpp>------------------------------------
#include "../h/wsLed.h"

bool LED_OnOff_STATE = true;
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); // 800kHzでNeoPixelを駆動 おまじない行

// M5Stack Core2 AWS
#define rLED_LEN 4
#define lLED_LEN 4
#define LED_LEN (rLED_LEN + lLED_LEN)
extern const uint16_t rLED[rLED_LEN] = {0, 1, 2, 3};
extern const uint16_t lLED[lLED_LEN] = {8, 7, 6, 5};
extern const uint16_t LED_ROLL[LED_LEN] = {0, 1, 2, 3, 5, 6, 7, 8};

void ledSetup()
{
  pixels.begin();
  pixels.clear();
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

#define LED_BRIGHT 155
void ledRolling(int num)
{
  int val = num % 10;
  
  switch (val)
  {
  case 0:
  case 5:
    ledClear();
    ledShow();
    break;

  case 1:
    // led_clear();
    ledSetColor(rLED[0], LED_BRIGHT, 0, 0);
    ledShow();
    break;

  case 2:
    // led_clear();
    ledSetColor(rLED[0], 0, 0, 0);
    ledSetColor(rLED[1], 0, LED_BRIGHT, 0);
    ledShow();
    break;

  case 3:
    // led_clear();
    ledSetColor(rLED[1], 0, 0, 0);
    ledSetColor(rLED[2], 0, 0, LED_BRIGHT);
    ledShow();
    break;

  case 4:
    // led_clear();
    ledSetColor(rLED[2], 0, 0, 0);
    ledSetColor(rLED[3], LED_BRIGHT, LED_BRIGHT, LED_BRIGHT);
    ledShow();
    break;

  case 6:
    // led_clear();
    ledSetColor(lLED[3], LED_BRIGHT, 0, 0);
    ledShow();
    break;

  case 7:
    // led_clear();
    ledSetColor(lLED[3], 0, 0, 0);
    ledSetColor(lLED[2], 0, LED_BRIGHT, 0);
    ledShow();
    break;

  case 8:
    // led_clear();
    ledSetColor(lLED[2], 0, 0, 0);
    ledSetColor(lLED[1], 0, 0, LED_BRIGHT);
    ledShow();
    break;

  case 9:
    // led_clear();
    ledSetColor(lLED[1], 0, 0, 0);
    ledSetColor(lLED[0], LED_BRIGHT, LED_BRIGHT, LED_BRIGHT);
    ledShow();
    break;
  }

}

void blueAndRedLedOn()
{
  ledClear();
  ledShow();

  // ledSetColor2(2, ledGetColorNo(0, 0, 255));
  // ledSetColor2(7, ledGetColorNo(0, 0, 255));

  // --- right blue
  ledSetColor(rLED[0], 0, 0, 155);
  ledSetColor(rLED[3], 0, 0, 155);

  // --- left red
  ledSetColor(lLED[0], 155, 0, 0);
  ledSetColor(lLED[3], 155, 0, 0);

  ledShow();
  delay(1000);
  ledClear();
  ledShow();
}
