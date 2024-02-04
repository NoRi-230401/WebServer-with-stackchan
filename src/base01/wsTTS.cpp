// ---------------------------< wsTTS.cpp >------------------------------------
#include "../h/wsTTS.h"

uint8_t TTS_vSpkNo = 3;
VoiceVox *tts_voiceVox = new VoiceVox();
// VoiceText *tts_voiceText = new VoiceText();
String SPEECH_TEXT = "";

void ttsDo(const String &speechText)
{
  WST = WST_TTS_start;
  log_free_size("VOICEVOX：IN");
  Serial.println("------- [ speak to you ] -------");
  Serial.println(speechText);
  Serial.println("--------------------------------");

  String return_string = execute_voicevox(speechText, TTS_vSpkNo);
  if(return_string=="")
  {
    Serial.println("voicevox Err");
    WST = WST_TTS_exit;
    return;
  }
  else
  {
    WST = WST_TTS_talkStart;
    showExeTime("",EXE_TM_MD_START);

    // execute_talk(return_string);
    if ( return_string != "")
        tts_voiceVox->talk_https(return_string);

    showExeTime("VOICEVOX：mp3Url get then start speaking");
  }
}




void wsHandleSpeech(String sayS, String expressionS, String balloonS, String voiceS, String afterExpS )
{
  String speakTxt = "";
  int expr = -1;
  int afterExpr = -1;
  String balloonStr = "$$SKIP$$";

  webpage = "";

  speakTxt = sayS;

  if (expressionS != "")
  { // Avatar の顔の表情
    int tmp_expr = expressionS.toInt();
    if (tmp_expr >= 0 && tmp_expr <= 5)
      expr = tmp_expr;

    webpage += "speech : expression = " + EXPR_STR[expr] ;
    webpage += "　expr = " + String(expr,DEC) + "<br>";
  }

  if (afterExpS != "")
  { // After Avatar の顔の表情
    int tmp_expr = afterExpS.toInt();
    if (tmp_expr >= 0 && tmp_expr <= 5)
      afterExpr = tmp_expr;

    webpage += "speech : afterExpr = " + EXPR_STR[afterExpr] ;
    webpage += "　expr = " + String(afterExpr,DEC) + "<br>";
  }

  if ( balloonS != "$$SKIP$$")
  {
    balloonStr = balloonS;
    webpage += "speech : baloon = " + balloonS + "<br>";
  }

  if (voiceS != "")
  {
    int tmp_vNo = voiceS.toInt();
    if( tmp_vNo>=0 && tmp_vNo<=66)
        TTS_vSpkNo = (uint8_t)tmp_vNo;

    webpage += "speech : voice = " + voiceS + "<br>";
  }


  stackchanReq(speakTxt, expr, balloonStr, afterExpr);
  webpage += "speech : say = " + sayS;
}

bool isTalking()
{
  return ((tts_voiceVox->is_talking) || (SPEECH_TEXT != ""));
}

String execute_voicevox(const String &speechText, uint8_t spk_no)
{
  if (speechText == "")
  {
    return "";
  }

  tts_voiceVox->setSpkNo(spk_no);
  String return_string = tts_voiceVox->synthesis(speechText);
  return return_string;
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
  size_t spkNo = TTS_VSPKNO_INIT;

  if (ESP_OK == nvs_open(SETTING_NVS, NVS_READONLY, &nvs_handle))
  {
    nvs_get_u32(nvs_handle, "vSpkNo", &spkNo);
    if (spkNo > TTS_VSPKNO_MAX)
      spkNo = TTS_VSPKNO_INIT;
  }
  nvs_close(nvs_handle);

  return (uint8_t)spkNo;
}
