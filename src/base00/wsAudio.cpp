// ---------------------------< wsAudio.cpp >------------------------------------
#include "wsAudio.h"

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

void setVolume(int vol)
{
  if ((vol < 0) || (vol > 255))
    vol = 180;

  VOLUME_VALUE = (uint8_t)vol;
  
  M5.Speaker.setVolume(VOLUME_VALUE);
  M5.Speaker.setChannelVolume(m5spk_virtual_channel, VOLUME_VALUE);

  uint32_t nvs_handle;
  if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    nvs_set_u32(nvs_handle, "volume", (size_t)VOLUME_VALUE);
  nvs_close(nvs_handle);
}

// void volumeReset()
// {
//   M5.Speaker.setVolume(VOLUME_VALUE);
//   M5.Speaker.setChannelVolume(m5spk_virtual_channel, VOLUME_VALUE);
// }

uint8_t getVolumeVal()
{
  return (VOLUME_VALUE);
}

uint8_t getVolumeValfmNVM()
{
  uint32_t nvs_handle;
  size_t getVol = 180;

  if (ESP_OK == nvs_open(SETTING_NVS, NVS_READONLY, &nvs_handle))
    nvs_get_u32(nvs_handle, "volume", &getVol);
  nvs_close(nvs_handle);

  if (getVol > 255)
    getVol = 180;

  return (uint8_t)getVol;
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
    blueAndRedLedOn();
    break;

  case 5: // LED and Tone
    toneOn();
    blueAndRedLedOn();
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
