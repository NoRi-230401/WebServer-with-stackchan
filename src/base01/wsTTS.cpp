// ---------------------------< wsTTS.cpp >------------------------------------
#include "wsTTS.h"

String SPEECH_TEXT = "";
String SPEECH_TEXT_BUFFER = "";

// -- TTS (VOICEVOX)　---
String TTS_SPEAKER_NO = "";
extern const String TTS_SPEAKER;
const String TTS_SPEAKER = "&speaker=";
String TTS_PARMS = TTS_SPEAKER + TTS_SPEAKER_NO;


void wsHandleSpeech(String sayS, String expressionS, String voiceS)
{
  if (sayS == "")
    return;

  webpage = "";

  int expr = 0;
  if (expressionS != "")
  {
    expr = expressionS.toInt();
    if (expr < 0)
      expr = 0;
    if (expr > 5)
      expr = 5;
    webpage += "speech : expression = " + String(expr, DEC);
  }

  if (voiceS != "")
  {
    TTS_PARMS = TTS_SPEAKER + voiceS;
    webpage += "\nspeech : voice = " + voiceS;
  }

  sendReq(REQ_SPEAK,sayS);

  REQ_SPK_EXPR = expr;
  webpage += "\nspeech : say = " + sayS;
}


VoiceVox *tts = new VoiceVox();
// uint8_t config_speaker;           // 声
uint16_t https_timeout = 60000;   // HTTPタイムアウト時間

void ttsDo(const String &speechText)
{
  Serial.println("~~~~~~~ [ speak to you ] ~~~~~~~");
  Serial.println(speechText);
  // Serial.println("--------------------------------");

  // config_speaker = (uint8_t)TTS_SPEAKER_NO.toInt();  
  // speaker_name = get_speaker_name(String(config_speaker));

  String return_string = execute_voicevox(speechText, (uint8_t)TTS_SPEAKER_NO.toInt());
  SPEECH_TEXT_BUFFER="";
  execute_talk(return_string);
}



bool isTalking()
{
  return( tts->is_talking);
}

void setSpeaker(uint8_t spk_no)
{
  tts ->setSpkNo(spk_no);
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
  if (url == "")
  {
    return;
  }
  tts->talk_https(url);
}

