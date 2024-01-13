// ---------------------------< wsTTS.cpp >------------------------------------
#include "wsTTS.h"

String SPEECH_TEXT = "";
String SPEECH_TEXT_BUFFER = "";

// -- TTS2(VOICEVOX)　---
String TTS2_SPEAKER_NO = "";
extern const String TTS2_SPEAKER;
const String TTS2_SPEAKER = "&speaker=";
String TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;

// int TTS_mp3buffSize = 25 * 1024;
// uint8_t *TTS_mp3buff;
// uint8_t m5spk_virtual_channel = 0;

// AudioOutputM5Speaker out(&M5.Speaker, m5spk_virtual_channel);
// AudioGeneratorMP3 *mp3;
// AudioFileSourceHTTPSStream *file_TTS2 = nullptr;
// AudioFileSourceBuffer *BUFF = nullptr;

// void playMP3(AudioFileSourceBuffer *buff)
// {
//   mp3->begin(buff, &out);
// }

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

  // REQUEST_GET = REQ_SPEAK;
  // REQ_MSG = sayS;
  sendReq(REQ_SPEAK,sayS);

  REQ_SPK_EXPR = expr;
  webpage += "\nspeech : say = " + sayS;
}


void ttsSetup()
{

  // // *** TTS用 mp3Buffer確保 ******
  // TTS_mp3buff = (uint8_t *)malloc(TTS_mp3buffSize);
  // if (!TTS_mp3buff)
  // {
  //   String msg = "\nFATAL ERROR:  Unable to preallocate " + String(TTS_mp3buffSize, DEC) + "bytes for app\n";
  //   Serial.println(msg);
  //   M5.Display.println(msg);
  //   errSTOP();
  //   // --- Stop
  // }

  // audioLogger = &Serial;
  // mp3 = new AudioGeneratorMP3();

}

void SpeechManage()
{
  // *** SPEECH_TEXT 1st ****
  // if ( !isTalking() && SPEECH_TEXT != "" )
  // {
  //   SpeechText1st();
  // }

  // // ** mp3 Running and SPEECH Next **
  // if (mp3->isRunning())
  // { // -- mp3 loop終了 ------
  //   if (!mp3->loop())
  //   {
  //     SpeechTextNext();
  //   }
  //   delay(1);
  // }
}

void SpeechText1st()
{
  avatar.setExpression(Expression::Happy);
  SPEECH_TEXT_BUFFER = SPEECH_TEXT;
  SPEECH_TEXT = "";
  // ttsDo(SPEECH_TEXT_BUFFER);
  sendReq(REQ_SPEAK,SPEECH_TEXT_BUFFER);
}

void SpeechTextNext()
{
  // mp3->stop();
  // // Serial.println("mp3 stop");

  // if (file_TTS2 != nullptr)
  // {
  //   delete file_TTS2;
  //   file_TTS2 = nullptr;
  // }

  // avatar.setExpression(Expression::Neutral);
  // SPEECH_TEXT_BUFFER = "";
  // Serial.println("--- end of speaking ---");
}


VoiceVox *tts = new VoiceVox();
uint8_t config_speaker;           // 声
String speaker_name;
uint16_t https_timeout = 60000;   // HTTPタイムアウト時間


void ttsDo(const String &speechText)
{
  Serial.println("\n~~~~~ Speech-Text ~~~~~");
  Serial.println(speechText);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~");

  config_speaker = (uint8_t)TTS2_SPEAKER_NO.toInt();  
  speaker_name = get_speaker_name(String(config_speaker));

  String return_string = execute_voicevox(speechText);
  SPEECH_TEXT_BUFFER="";
  execute_talk(return_string);
}



bool isTalking()
{
  return( tts->is_talking);
}

// void ttsDoTp(const String &speechText)
// { // つゆきぱぱさんのコード実行開始。

//   config_speaker = (uint8_t)TTS2_SPEAKER_NO.toInt();  
//   speaker_name = get_speaker_name(String(config_speaker));

//   String return_string = execute_voicevox(speechText);
//   SPEECH_TEXT_BUFFER="";
//   execute_talk(return_string);
// }


String execute_voicevox(String text)
{
  if (text == "")
  {
    return "";
  }
  log_free_size("VOICEVOX：IN");
  // avatar.setSpeechText("すぅー …");
  
  String return_string = tts->synthesis(text);
  return return_string;
}

void execute_talk(String url)
{
  if (url == "")
  {
    return;
  }
  // avatar.setSpeechText("VOICEVOX：");
  tts->talk_https(url);
}

String get_speaker_name(String speaker)
{
  String name_list = R"({"2": "四国めたん", "0": "四国めたん", "6": "四国めたん", "4": "四国めたん", "36": "四国めたん", "37": "四国めたん", "3": "ずんだもん", "1": "ずんだもん", "7": "ずんだもん", "5": "ずんだもん", "22": "ずんだもん", "38": "ずんだもん", "8": "春日部つむぎ", "10": "雨晴はう", "9": "波音リツ", "65": "波音リツ", "11": "玄野武宏", "39": "玄野武宏", "40": "玄野武宏", "41": "玄野武宏", "12": "白上虎太郎", "32": "白上虎太郎", "33": "白上虎太郎", "34": "白上虎太郎", "35": "白上虎太郎", "13": "青山龍星", "14": "冥鳴ひまり", "16": "九州そら", "15": "九州そら", "18": "九州そら", "17": "九州そら", "19": "九州そら", "20": "もち子", "66": "もち子", "21": "剣崎雌雄", "23": "WhiteCUL", "24": "WhiteCUL", "25": "WhiteCUL", "26": "WhiteCUL", "27": "後鬼", "28": "後鬼", "29": "No.7", "30": "No.7", "31": "No.7", "42": "ちび式じい", "43": "櫻歌ミコ", "44": "櫻歌ミコ", "45": "櫻歌ミコ", "46": "小夜/SAYO", "47": "ナースロボ＿タイプＴ", "48": "ナースロボ＿タイプＴ", "49": "ナースロボ＿タイプＴ", "50": "ナースロボ＿タイプＴ", "51": "†聖騎士紅桜†", "52": "雀松朱司", "53": "麒ヶ島宗麟", "54": "春歌ナナ", "55": "猫使アル", "56": "猫使アル", "57": "猫使アル", "58": "猫使ビィ", "59": "猫使ビィ", "60": "猫使ビィ", "61": "中国ウサギ", "62": "中国ウサギ", "63": "中国ウサギ", "64": "中国ウサギ", "67": "栗田まろん", "68": "あいえるたん", "69": "満別花丸", "70": "満別花丸", "71": "満別花丸", "72": "満別花丸", "73": "満別花丸", "74": "琴詠ニア"})";
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, name_list);
  String speaker_name = doc[speaker];
  doc.clear();
  return speaker_name;
}
