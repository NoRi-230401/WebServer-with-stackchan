// ---------------------------< wsTTS.cpp >------------------------------------
#include "wsTTS.h"

uint8_t TTS_vSpkNo = 3;
VoiceVox *tts = new VoiceVox();
String SPEECH_TEXT = "";

void ttsDo(const String &speechText)
{
  Serial.println("------- [ speak to you ] -------");
  Serial.println(speechText);
  Serial.println("--------------------------------");

  String return_string = execute_voicevox(speechText, TTS_vSpkNo);
  execute_talk(return_string);
}

void wsHandleSpeech(String sayS, String expressionS, String voiceS)
{
  if (sayS == "")
    return;

  webpage = "";

  int expr = 0;
  if (expressionS != "")
  { // Avatar の顔の表情
    expr = expressionS.toInt();
    if (expr < 0)
      expr = 0;
    if (expr > 5)
      expr = 5;
    webpage += "speech : expression = " + String(expr, DEC);
  }

  if (voiceS != "")
  {
    // TTS_PARMS = TTS_SPEAKER + voiceS;
    TTS_vSpkNo = (uint8_t)voiceS.toInt();
    webpage += "speech : voice = " + voiceS;
  }

  sendReq(REQ_SPEAK, sayS);
  REQ_SPK_EXPR = expr; // Avatar の顔の表情

  webpage += "\nspeech : say = " + sayS;
}

bool isTalking()
{
  return ((tts->is_talking) || (SPEECH_TEXT != ""));
}

void setSpeaker(uint8_t spk_no)
{
  tts->setSpkNo(spk_no);
}

String execute_voicevox(const String &speechText, uint8_t spk_no)
{
  if (speechText == "")
  {
    return "";
  }
  log_free_size("VOICEVOX：IN");

  tts->setSpkNo(spk_no);
  String return_string = tts->synthesis(speechText);
  return return_string;
}

void execute_talk(String url)
{
  if (url != "")
    tts->talk_https(url);
}

void setTTSvSpkNo(int spkNo)
{
  if (spkNo > TTS_VSPKNO_MAX)
    spkNo = TTS_VSPKNO_INIT;

  TTS_vSpkNo = (uint8_t)spkNo;

  uint32_t nvs_handle;
  if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    nvs_set_u32(nvs_handle, "vSpkNo", (size_t)TTS_vSpkNo);
  nvs_close(nvs_handle);
}

uint8_t getTTSvSpkNofmNVS()
{
  uint32_t nvs_handle;
  size_t spkNo;

  if (ESP_OK == nvs_open(SETTING_NVS, NVS_READONLY, &nvs_handle))
  {
    nvs_get_u32(nvs_handle, "vSpkNo", &spkNo);
    if (spkNo > TTS_VSPKNO_MAX)
      spkNo = TTS_VSPKNO_INIT;

    TTS_vSpkNo = (uint8_t)spkNo;
  }
  nvs_close(nvs_handle);

  return TTS_vSpkNo;
}
