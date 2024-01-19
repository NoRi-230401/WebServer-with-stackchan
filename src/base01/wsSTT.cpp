// ---------------------------< wsSTT.cpp >------------------------------------
#include "wsSTT.h"

// bool wakeword_is_enable = false;

String SpeechToText()
{
  Serial.println("\r\nRecord start!\r\n");
  String ret = "";

  AudioWhisper *audio = new AudioWhisper();
  audio->Record();
  Serial.println("Record end\r\n");
  Serial.println("音声認識開始");
  avatar.setSpeechText("わかりました");
  Whisper *cloudSpeechClient = new Whisper(root_ca_openai, OPENAI_API_KEY.c_str());
  ret = cloudSpeechClient->Transcribe(audio);
  delete cloudSpeechClient;
  delete audio;
  return ret;
}


void SST_ChatGPT()
{
  delay(200);
  M5.Speaker.end();
  
  int prev_SV_MD = SV_MD;
  randomSpeakStop2();

  if (SV_USE)
    SV_MD = SV_MD_HOME;

  avatar.setExpression(Expression::Happy);
  avatar.setSpeechText("御用でしょうか？");
  
  // ---- Whisper STT USE -------------
  String transAudioText;
  transAudioText = SpeechToText();

  if (SV_USE)
    SV_MD = prev_SV_MD;

  Serial.println("音声認識終了");
  Serial.println("音声認識結果");
  
  if (transAudioText != "")
  {
    Serial.println(transAudioText);
    // if (!mp3->isRunning() && SPEECH_TEXT == "" && SPEECH_TEXT_BUFFER == "")
    // if ( SPEECH_TEXT == "" && SPEECH_TEXT_BUFFER == "")
    if ( !isTalking() )
    {
      exec_chatGPT(transAudioText);
      // WAKEWORD_MODE = 0;
    }
  }
  else
  {
    Serial.println("音声認識失敗");
    avatar.setExpression(Expression::Sad);
    avatar.setSpeechText("聞き取れませんでした");
    delay(2000);
    avatar.setSpeechText("");
    avatar.setExpression(Expression::Neutral);
  }
  M5.Speaker.begin();
}


