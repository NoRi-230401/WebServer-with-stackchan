// -------------------------<wsLed.h>-----------------------------------
#ifndef WS_LED_H
#define WS_LED_H
// ---------------------------------------------------------------------
#include <Adafruit_NeoPixel.h>
// #include <Arduino.h>

#define LED_PIN 25         // GPIO25でLEDを使用する
#define NUM_LEDS 10        // LEDの数を指定する

// ---------------------------------------------------------------------
void led_allOff();
void led_setColor2(uint16_t n, uint32_t c);
void led_setColor4(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
uint32_t led_ColorLED3(uint8_t r, uint8_t g, uint8_t b);
void led_show();
void led_clear();
// ---------------------------------------------------------------------
#endif
