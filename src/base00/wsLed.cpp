// ----------------------------<wsLed.cpp>------------------------------------
#include "wsLed.h"

bool LED_OnOff_STATE = true;
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); // 800kHzでNeoPixelを駆動 おまじない行

// M5Stack Core2 AWS 
#define rLED_LEN 4
#define lLED_LEN 4
#define LED_LEN (rLED_LEN + lLED_LEN)
const uint16_t rLED[rLED_LEN] ={0,1,2,3};
const uint16_t lLED[lLED_LEN] ={8,7,6,5};
const uint16_t LED_ROLL[LED_LEN] ={0,1,2,3,5,6,7,8 };

void ledSetup()
{
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void led_allOff()
{ // 全てのLEDを消灯
  pixels.clear();
  pixels.show();

  // 一時的にLEDを制御できるようにする。
  // bool tmpState = LED_OnOff_STATE;
  // LED_OnOff_STATE = true;

  // for (int i = 0; i < NUM_LEDS; i++)
  // {
  //   setLedColor2(i, getLedColorNoRGB(0, 0, 0));
  // }
  // led_show();

  // LED_OnOff_STATE = tmpState;
}

// ----- LED表示用ラッパー関数　-----------
void setLedColor2(uint16_t n, uint32_t c)
{
  if (LED_OnOff_STATE)
    pixels.setPixelColor(n, c);
}

void setLedColor4(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
  if (LED_OnOff_STATE)
    pixels.setPixelColor(n, r, g, b);
}

uint32_t getLedColorNoRGB(uint8_t r, uint8_t g, uint8_t b)
{
  if (LED_OnOff_STATE)
    return (pixels.Color(r, g, b));
  else
    return 0;
}

void led_show()
{
  if (LED_OnOff_STATE)
    pixels.show();
}

void led_clear()
{
  if (LED_OnOff_STATE)
    pixels.clear();
}

void blueLedOn()
{
  led_clear();
  led_show();

  // setLedColor2(2, getLedColorNoRGB(0, 0, 255));
  // setLedColor2(7, getLedColorNoRGB(0, 0, 255));

  // --- right blue 
  setLedColor4(rLED[0], 0, 0, 155);
  setLedColor4(rLED[3], 0, 0, 155);
  
  // --- left red 
  setLedColor4(lLED[0], 155, 0, 0);
  setLedColor4(lLED[3], 155, 0, 0);

  led_show();
  delay(1000);
  led_clear();
  led_show();
}

