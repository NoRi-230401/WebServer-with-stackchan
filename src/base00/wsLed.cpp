// ----------------------------<wsLed.cpp>------------------------------------
#include "wsLed.h"

bool LED_OnOff_STATE = true;
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); // 800kHzでNeoPixelを駆動 おまじない行


void led_allOff()
{ // 全てのLEDを消灯

  // 一時的にLEDを制御できるようにする。
  bool tmpState = LED_OnOff_STATE;
  LED_OnOff_STATE = true;

  for (int i = 0; i < NUM_LEDS; i++)
  {
    led_setColor2(i, led_ColorLED3(0, 0, 0));
  }
  led_show();

  LED_OnOff_STATE = tmpState;
  // delay(500); // 0.5秒待機
}

// ----- LED表示用ラッパー関数　-----------
void led_setColor2(uint16_t n, uint32_t c)
{
  if (LED_OnOff_STATE)
    pixels.setPixelColor(n, c);
}

void led_setColor4(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
  if (LED_OnOff_STATE)
    pixels.setPixelColor(n, r, g, b);
}

uint32_t led_ColorLED3(uint8_t r, uint8_t g, uint8_t b)
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
  led_setColor2(2, led_ColorLED3(0, 0, 255));
  led_setColor2(7, led_ColorLED3(0, 0, 255));
  led_show();
  delay(1000);
  led_clear();
  led_show();
}

