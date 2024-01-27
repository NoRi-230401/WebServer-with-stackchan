// -------------------------<wsLed.h>-----------------------------------
#ifndef WS_LED_H
#define WS_LED_H
// ---------------------------------------------------------------------
#include <Adafruit_NeoPixel.h>
// #include <Arduino.h>

#define LED_PIN 25         // GPIO25でLEDを使用する
#define NUM_LEDS 10        // LEDの数を指定する

// ---------------------------------------------------------------------
void ledSetup();
void ledClearAll();
void ledSetColor2(uint16_t n, uint32_t c);
void ledSetColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
uint32_t ledGetColorNo(uint8_t r, uint8_t g, uint8_t b);
void ledShow();
void ledClear();
void blueAndRedLedOn();
void ledRolling(int num);
// ---------------------------------------------------------------------
#endif
