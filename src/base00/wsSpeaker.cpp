// ---------------------------< wsSpeaker.cpp >------------------------------------
#include "wsSpeaker.h"

uint8_t m5spk_virtual_channel = 0;
uint8_t VOLUME_VALUE;

bool MUTE_ON_STATE = false;

void M5SpeakerConfig()
{
  auto spk_cfg = M5.Speaker.config();

  // default:64000 (64kHz)  e.g. 48000 , 50000 , 80000 , 96000 , 100000 , 128000 , 144000 , 192000 , 200000
  spk_cfg.sample_rate = 96000;
  spk_cfg.task_pinned_core = APP_CPU_NUM;

  M5.Speaker.config(spk_cfg);
  M5.Speaker.begin();
  M5.Speaker.setVolume(0);
  M5.Speaker.setChannelVolume(m5spk_virtual_channel, 0);
}

uint8_t setVolumeVal(uint8_t volumeVal, int save_flag)
{
  // if (volumeVal > 255)
  //   volumeVal = 255;
  // if (volumeVal <= 0)
  //   volumeVal = 0;

  VOLUME_VALUE = volumeVal;
  M5.Speaker.setVolume(VOLUME_VALUE);
  M5.Speaker.setChannelVolume(m5spk_virtual_channel, VOLUME_VALUE);

  if (save_flag == VAL_NVS_SAVE)
  {
    uint32_t nvs_handle;
    if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
      nvs_set_u32(nvs_handle, "volume", (uint32_t)VOLUME_VALUE);
    nvs_close(nvs_handle);
  }
  return (VOLUME_VALUE);
}

uint8_t getVolumeVal()
{
  return (VOLUME_VALUE);
}

void toneOn()
{
  M5.Speaker.tone(1000, 100);
}

void tone(int mode)
{
  switch (TONE_MODE)
  {
  case 0: // always toneOff
    break;

  case 1: // toneOn when buttons pressed
    if (mode == TONE_MODE)
      toneOn();
    break;

  case 2: // toneOn whenn external command rcv
    if (mode == TONE_MODE)
      toneOn();
    break;

  case 3: // toneOn every time
    toneOn();
    break;

  case 4: // LED only
    // toneOn();
    blueLedOn();
    break;

  case 5: // LED and Tone
    toneOn();
    blueLedOn();
    break;

  default:
    break;
  }
}

void muteOn()
{
  M5.Speaker.setVolume(0);
  M5.Speaker.setChannelVolume(m5spk_virtual_channel, 0);
  MUTE_ON_STATE = true;
}

void muteOff()
{
  M5.Speaker.setVolume(VOLUME_VALUE);
  M5.Speaker.setChannelVolume(m5spk_virtual_channel, VOLUME_VALUE);
  MUTE_ON_STATE = false;
}
