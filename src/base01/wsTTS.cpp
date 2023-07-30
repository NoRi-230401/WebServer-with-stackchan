// ---------------------------< wsTTS.cpp >------------------------------------
#include "wsTTS.h"

String SPEECH_TEXT = "";
String SPEECH_TEXT_BUFFER = "";

// -- TTS2(VOICEVOX)　---
String TTS2_SPEAKER_NO = "";
extern const String TTS2_SPEAKER;
const String TTS2_SPEAKER = "&speaker=";
String TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;
int TTS_mp3buffSize = 25 * 1024;
uint8_t *TTS_mp3buff;
uint8_t m5spk_virtual_channel = 0;

AudioOutputM5Speaker out(&M5.Speaker, m5spk_virtual_channel);
AudioGeneratorMP3 *mp3;
AudioFileSourceHTTPSStream *file_TTS2 = nullptr;
AudioFileSourceBuffer *BUFF = nullptr;

uint8_t TTS_TYPE = 2; // default "VOICEVOX"
String LANG_CODE = "";
const char *TTS_NAME[] = {"VoiceText", "GoogleTTS", "VOICEVOX"};
const char *LANG_CODE_JP = "ja-JP";
const char *LANG_CODE_EN = "en-US";

void playMP3(AudioFileSourceBuffer *buff)
{
  mp3->begin(buff, &out);
}

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
    TTS2_PARMS = TTS2_SPEAKER + voiceS;
    webpage += "\nspeech : voice = " + voiceS;
  }

  REQUEST_GET = REQ_SPEAK_MSG2;
  REQ_MSG = sayS;
  REQ_SPK_EXPR = expr;
  webpage += "\nspeech : say = " + sayS;
}

void ttsSetup()
{
  // *** TTS用 mp3Buffer確保 ******
  TTS_mp3buff = (uint8_t *)malloc(TTS_mp3buffSize);
  if (!TTS_mp3buff)
  {
    String msg = "\nFATAL ERROR:  Unable to preallocate " + String(TTS_mp3buffSize, DEC) + "bytes for app\n";
    Serial.println(msg);
    M5.Display.println(msg);
    errSTOP();
    // --- Stop
  }

  audioLogger = &Serial;
  mp3 = new AudioGeneratorMP3();
}

void SpeechManage()
{
  // *** SPEECH_TEXT 1st ****
  if (SPEECH_TEXT != "")
    SpeechText1st();

  // ** mp3 Running and SPEECH Next **
  if (mp3->isRunning())
  { // -- mp3 loop終了 ------
    if (!mp3->loop())
    {
      SpeechTextNext();
    }
    // delay(1);
  }
}

void SpeechText1st()
{
  SPEECH_TEXT_BUFFER = SPEECH_TEXT;
  SPEECH_TEXT = "";
  avatar.setExpression(Expression::Happy);
  ttsDo(SPEECH_TEXT_BUFFER);
}

void SpeechTextNext()
{
  mp3->stop();
  Serial.println("mp3 stop");

  if (file_TTS2 != nullptr)
  {
    delete file_TTS2;
    file_TTS2 = nullptr;
  }

  avatar.setExpression(Expression::Neutral);
  SPEECH_TEXT_BUFFER = "";
  Serial.println("***** End of Speaking *****");
}

void ttsDo( const String& speechText )
{
  Serial.println("\nSpeech Text = " + speechText);
  Voicevox_tts((char *)speechText.c_str(), (char *)TTS2_PARMS.c_str());
}



bool isJP()
{
  return true;
}

