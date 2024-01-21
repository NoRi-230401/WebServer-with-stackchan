// ---------------------------< wsSpeaker.h >------------------------------------
#ifndef WS_SPEAKER_H
#define WS_SPEAKER_H
// ---------------------------
#include <Arduino.h>
#include <nvs.h>
#include <M5Unified.h>

extern void blueLedOn();
extern const char *SETTING_NVS;

//-------------------------------------------------------------------------
#define TONE_MODE_INIT 4
#define TONE_MODE_MAX 5
uint8_t TONE_MODE = TONE_MODE_INIT; // 0:allOff(default) 1:buttonOn 2:extCommOn 3:allOn 4.LedOnly. 5.tone&LED

#define VAL_NVS_NOSAVE 0
#define VAL_NVS_SAVE 1

void M5SpeakerConfig();
uint8_t setVolumeVal(uint8_t volumeVal, int save_flag=VAL_NVS_SAVE);
void setVolume(int vol);
uint8_t getVolumeValfmNVM();
uint8_t getVolumeVal();
void toneOn();
void tone(int mode);
void muteOn();
void muteOff();
//-------------------------------------------------------------------------

#endif
