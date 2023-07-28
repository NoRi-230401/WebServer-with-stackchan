// ---------------------------< wsTTS.cpp >------------------------------------
#include "wsTTS.h"

extern const String TTS2_SPEAKER;
extern const String KEYWORDS[];

uint8_t m5spk_virtual_channel = 0;
String SPEECH_TEXT = "";
String SPEECH_TEXT_BUFFER = "";

// *** 感情表現 *****
const String EMOTION_PARAMS[] = {
    "&emotion_level=2&emotion=happiness",
    "&emotion_level=3&emotion=happiness",
    "&emotion_level=2&emotion=sadness",
    "&emotion_level=1&emotion=sadness",
    "&emotion_level=4&emotion=sadness",
    "&emotion_level=4&emotion=anger"};
int TTS_EMOTION_NO = 0;
// TTS0(VoiceText) --感情表現用--
extern const String EXPRESSION_STRING[] ;
int EXPRESSION_INDX = -1;
const String KEYWORDS[] = {"(Neutral)", "(Happy)", "(Sleepy)", "(Doubt)", "(Sad)", "(Angry)"};

// extern const char tts_parms1[];
// extern const char tts_parms2[];
// extern const char tts_parms3[];
// extern const char tts_parms4[];
// extern const char tts_parms5[];
// extern const char tts_parms6[];
// extern const char *tts_parms_table[];
// extern const String tts_parms01;
// extern const String tts_parms02;
// extern const String tts_parms03;
// extern const String tts_parms04;
// extern const String tts_parms05;
// extern const String tts_parms06;
// extern const String TTS_PARMS_STR[];
// const char tts_parms1[] = "&emotion_level=4&emotion=happiness&format=mp3&speaker=takeru&volume=200&speed=100&pitch=130";
// const char tts_parms2[] = "&emotion=happiness&format=mp3&speaker=hikari&volume=200&speed=120&pitch=130";
// const char tts_parms3[] = "&emotion=anger&format=mp3&speaker=bear&volume=200&speed=120&pitch=100";
// const char tts_parms4[] = "&emotion_level=2&emotion=happiness&format=mp3&speaker=haruka&volume=200&speed=80&pitch=70";
// const char tts_parms5[] = "&emotion_level=4&emotion=happiness&format=mp3&speaker=santa&volume=200&speed=120&pitch=90";
// const char tts_parms6[] = "&emotion=happiness&format=mp3&speaker=hikari&volume=150&speed=110&pitch=140";
// const char *tts_parms_table[] = {tts_parms1, tts_parms2, tts_parms3, tts_parms4, tts_parms5, tts_parms6};
// const String tts_parms01 = "&format=mp3&speaker=takeru&volume=200&speed=100&pitch=130";
// const String tts_parms02 = "&format=mp3&speaker=hikari&volume=200&speed=120&pitch=130";
// const String tts_parms03 = "&format=mp3&speaker=bear&volume=200&speed=120&pitch=100";
// const String tts_parms04 = "&format=mp3&speaker=haruka&volume=200&speed=80&pitch=70";
// const String tts_parms05 = "&format=mp3&speaker=santa&volume=200&speed=120&pitch=90";
// const String tts_parms06 = "&format=mp3&speaker=hikari&volume=150&speed=110&pitch=140";
// const String TTS_PARMS_STR[] = {tts_parms01, tts_parms02, tts_parms03, tts_parms04, tts_parms05, tts_parms06};
// int TTS_PARMS_NO = 1;

// -- TTS2(VOICEVOX)　---
String TTS2_SPEAKER_NO = "";
const String TTS2_SPEAKER = "&speaker=";
String TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;

// String separator_tbl[2][10] = {{"。", "？", "！", "、", "　", "♪", "\n", ""}, {":", ",", ".", "?", "!", "\n", ""}};

AudioOutputM5Speaker out(&M5.Speaker, m5spk_virtual_channel);
AudioGeneratorMP3 *mp3;

// for TTS0(VoiceText) and TTS2(VOICEVOX)
// AudioFileSourceVoiceTextStream *file_TTS0 = nullptr;

AudioFileSourceHTTPSStream *file_TTS2 = nullptr;
int TTS_mp3buffSize = 25 * 1024;
uint8_t *TTS_mp3buff;
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
  int expr = 0;
  // int parms_no = 1;
  
  if( sayS =="" )
      return;
  
  webpage = "";
    
  if (expressionS != "")
  {
    expr = expressionS.toInt();
    if (expr < 0)
      expr = 0;
    if (expr > 5)
      expr = 5;

    webpage += "speech : expression = " + String(expr,DEC); 
  }

  if (voiceS != "")
  {
    // if (TTS_TYPE == 0)
    // { // voiceText
    //   parms_no = voiceS.toInt();
    //   if (parms_no < 0)
    //     parms_no = 0;
    //   if (parms_no > 4)
    //     parms_no = 4;
    // }

    if (TTS_TYPE == 2)
    { // voicevox
      TTS2_PARMS = TTS2_SPEAKER + voiceS;
    }

    webpage += "\nspeech : voice = " + voiceS;
  }
  
  REQUEST_GET = REQ_SPEAK_MSG2;
  REQ_MSG = sayS;
  REQ_SPK_EXPR = expr;
  // REQ_SPK_PARMS_NO = parms_no;
  
  webpage += "\nspeech : say = " + sayS ;
}



void ttsSetup()
{
  // *** TTS用 mp3Buffer確保 ******
  TTS_mp3buff = (uint8_t *)malloc(TTS_mp3buffSize);
  if (!TTS_mp3buff)
  {
    errSTOP("FATAL ERROR:  Unable to preallocate " + String(TTS_mp3buffSize,DEC) + "bytes for app\n" );
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
  // ---------------------------
  // 最初のSpeechText処理
  // ---------------------------
  // String sentence;
  // int dotIndex;

  // switch (TTS_TYPE)
  // {
  // case 0: // VoiceText
  //   SPEECH_TEXT_BUFFER = SPEECH_TEXT;
  //   SPEECH_TEXT = "";

  //   // 感情表現keyWordの前に、「。」を挿入
  //   addPeriodBeforeKeyword(SPEECH_TEXT_BUFFER, KEYWORDS, 6);

  //   Serial.println("----- Before sentence ---------");
  //   Serial.println(SPEECH_TEXT_BUFFER);

  //   sentence = SPEECH_TEXT_BUFFER;
  //   dotIndex = SPEECH_TEXT_BUFFER.indexOf("。");

  //   if (dotIndex != -1)
  //   {
  //     dotIndex += 3;
  //     sentence = SPEECH_TEXT_BUFFER.substring(0, dotIndex);
  //     // Serial.println(sentence);
  //     SPEECH_TEXT_BUFFER = SPEECH_TEXT_BUFFER.substring(dotIndex);
  //   }
  //   else
  //   {
  //     SPEECH_TEXT_BUFFER = "";
  //   }

  //   //--------------------------------------------------
  //   Serial.println("----- After sentence ---------");
  //   Serial.println("sentence = " + sentence);
  //   getExpression(sentence, EXPRESSION_INDX);
  //   //--------------------------------------------------
  //   if (EXPRESSION_INDX < 0)
  //   {
  //     avatar.setExpression(Expression::Happy);
  //     ttsDo((char *)sentence.c_str(), tts_parms_table[TTS_PARMS_NO]);
  //     avatar.setExpression(Expression::Neutral);
  //   }
  //   else
  //   { // 音声に感情表現
  //     String tmp = EMOTION_PARAMS[EXPRESSION_INDX] + TTS_PARMS_STR[TTS_PARMS_NO];
  //     ttsDo((char *)sentence.c_str(), (char *)tmp.c_str());
  //   }
  //   break;
  //   // ------------------------------------------------------------------------

  // case 1: // GoogleTTS
  //   SPEECH_TEXT_BUFFER = SPEECH_TEXT;
  //   SPEECH_TEXT = "";

  //   // addPeriodBeforeKeyword(SPEECH_TEXT_BUFFER, KEYWORDS, 6);

  //   Serial.println("----- Before sentence ---------");
  //   Serial.println(SPEECH_TEXT_BUFFER);
  //   sentence = SPEECH_TEXT_BUFFER;

  //   if (isJP())
  //     dotIndex = search_separator(SPEECH_TEXT_BUFFER, 0);
  //   else
  //     dotIndex = search_separator(SPEECH_TEXT_BUFFER, 1);

  //   if (dotIndex != -1)
  //   {
  //     if (isJP())
  //       dotIndex += 3;
  //     else
  //       dotIndex += 2; // ** Global版では、(+= 1) by NoRi ***

  //     sentence = SPEECH_TEXT_BUFFER.substring(0, dotIndex);
  //     Serial.println(sentence);
  //     SPEECH_TEXT_BUFFER = SPEECH_TEXT_BUFFER.substring(dotIndex);
  //   }
  //   else
  //   {
  //     SPEECH_TEXT_BUFFER = "";
  //   }
  //   avatar.setExpression(Expression::Happy);
  //   ttsDo((char *)sentence.c_str(), (char *)LANG_CODE.c_str());
  //   avatar.setExpression(Expression::Neutral);
  //   break;
  //   // ------------------------------------------------------------------------

  // case 2: // VOICEVOX
  //  --------------------------------------------------------------------------
    SPEECH_TEXT_BUFFER = SPEECH_TEXT;
    SPEECH_TEXT = "";
    // Serial.println("----- SPEECH_TEXT_BUFFER ---------");
    // Serial.println(SPEECH_TEXT_BUFFER);
    avatar.setExpression(Expression::Happy);
    ttsDo((char *)SPEECH_TEXT_BUFFER.c_str(), (char *)TTS2_PARMS.c_str());
  //  --------------------------------------------------------------------------
  //   break;
  // }

}


void SpeechTextNext()
{
  // ---------------------------
  // ２回目以降のSpeechText処理
  // ---------------------------

  // **********************
  // ***** mp3　STOP　*****
  // **********************
  mp3->stop();
  Serial.println("mp3 stop");

  // switch (TTS_TYPE)
  // {

  // case 0: // voiceText
  //   if (file_TTS0 != nullptr)
  //   {
  //     delete file_TTS0;
  //     file_TTS0 = nullptr;
  //   }

    // // *******************************************
    // // ------- sentence切出し(TTS0 VoiceText) ----
    // // *******************************************
    // if (SPEECH_TEXT_BUFFER != "")
    // {
    //   String sentence = SPEECH_TEXT_BUFFER;
    //   int dotIndex;
    //   dotIndex = SPEECH_TEXT_BUFFER.indexOf("。");

    //   if (dotIndex != -1)
    //   {
    //     dotIndex += 3;
    //     sentence = SPEECH_TEXT_BUFFER.substring(0, dotIndex);
    //     // Serial.println(sentence);
    //     SPEECH_TEXT_BUFFER = SPEECH_TEXT_BUFFER.substring(dotIndex);
    //   }
    //   else
    //   {
    //     SPEECH_TEXT_BUFFER = "";
    //   }

    //   //--------------------------------------------------
    //   Serial.println("----- After sentence ---------");
    //   Serial.println("sentence = " + sentence);
    //   getExpression(sentence, EXPRESSION_INDX);
    //   //--------------------------------------------------
    //   if (EXPRESSION_INDX < 0)
    //   {
    //     avatar.setExpression(Expression::Happy);
    //     ttsDo((char *)sentence.c_str(), tts_parms_table[TTS_PARMS_NO]);
    //     avatar.setExpression(Expression::Neutral);
    //   }
    //   else
    //   { // 音声の感情表現
    //     String tmp = EMOTION_PARAMS[EXPRESSION_INDX] + TTS_PARMS_STR[TTS_PARMS_NO];
    //     ttsDo((char *)sentence.c_str(), (char *)tmp.c_str());
    //   }
    // }
    // else
    // { // 最終回
    //   avatar.setExpression(Expression::Neutral);
    //   EXPRESSION_INDX = -1;
    //   Serial.println("***** End of Speaking *****");
    // }
    // // --- end of TTS0 ---
    // break;
    // // ------------------------------------------------------------------------

    // case 1: // GoogleTTS
    //   if (file_TTS1 != nullptr)
    //   {
    //     delete file_TTS1;
    //     file_TTS1 = nullptr;
    //   }

    //   // *******************************************
    //   // ------- sentence切出し(TTS1 GoogleTTS) ----
    //   // *******************************************
    //   if (SPEECH_TEXT_BUFFER != "")
    //   {
    //     String sentence = SPEECH_TEXT_BUFFER;
    //     int dotIndex;
    //     if (isJP())
    //       dotIndex = search_separator(SPEECH_TEXT_BUFFER, 0);
    //     else
    //       dotIndex = search_separator(SPEECH_TEXT_BUFFER, 1);

    //     if (dotIndex != -1)
    //     {
    //       if (isJP())
    //         dotIndex += 3;
    //       else
    //         dotIndex += 2;

    //       sentence = SPEECH_TEXT_BUFFER.substring(0, dotIndex);
    //       Serial.println(sentence);
    //       SPEECH_TEXT_BUFFER = SPEECH_TEXT_BUFFER.substring(dotIndex);
    //     }
    //     else
    //     {
    //       SPEECH_TEXT_BUFFER = "";
    //     }

    //     // 感情表現の検出を削除 ----------------------------------------
    //     avatar.setExpression(Expression::Happy);
    //     ttsDo((char *)sentence.c_str(), (char *)LANG_CODE.c_str());
    //     avatar.setExpression(Expression::Neutral);
    //   }
    //   else
    //   { // 最終回
    //     avatar.setExpression(Expression::Neutral);
    //     EXPRESSION_INDX = -1;
    //     Serial.println("*** Speak SPEECH_TEXT END ***");
    //   } // --- end of TTS1 ---
    //   break;
    //   // ------------------------------------------------------------------------

  // case 2: // voicevox
    if (file_TTS2 != nullptr)
    {
      delete file_TTS2;
      file_TTS2 = nullptr;
    }

    // 最終回
    avatar.setExpression(Expression::Neutral);
    SPEECH_TEXT_BUFFER = "";
    Serial.println("***** End of Speaking *****");

  //   break;
  //   // ------------------------------------------------------------------------
  // } // end of case
  
}


void ttsDo(char *text, const char *tts_parms)
{
  Serial.print("ttsDo func speaking text = ");
  Serial.println(text);
  
  // switch (TTS_TYPE)
  // {
  // case 0: // VoiceText
  //   VoiceText_tts(text, (char *)tts_parms);
  //   break;

  // case 1: // GoogleTTS
  //   google_tts(text, (char *)LANG_CODE.c_str());
  //   break;

  // case 2: // VOICEVOX
  //   Voicevox_tts(text, (char *)TTS2_PARMS.c_str());
  //   break;

  // default:
    Voicevox_tts(text, (char *)TTS2_PARMS.c_str());
  //   break;
  // }

}

bool isJP()
{
  return true;

  // if (LANG_CODE == String(LANG_CODE_JP) || (TTS_TYPE == 0) || (TTS_TYPE == 2))
  //   return true;
  // else
  //   return false;
}

// int search_separator(String text, int tbl)
// {
//   int i = 0;
//   int dotIndex_min = 1000;
//   int dotIndex;
//   while (separator_tbl[tbl][i] != "")
//   {
//     dotIndex = text.indexOf(separator_tbl[tbl][i++]);
//     if ((dotIndex != -1) && (dotIndex < dotIndex_min))
//       dotIndex_min = dotIndex;
//   }
//   if (dotIndex_min == 1000)
//     return -1;
//   else
//     return dotIndex_min;
// }

// void google_tts(char *text, char *lang)
// {
//   Serial.println("tts Start");
//   String link = "http" + tts.getSpeechUrl(text, lang).substring(5);
//   Serial.println(link);

//   http.begin(client, link);
//   http.setReuse(true);
//   int code = http.GET();
//   if (code != HTTP_CODE_OK)
//   {
//     http.end();
//     //    cb.st(STATUS_HTTPFAIL, PSTR("Can't open HTTP request"));
//     return;
//   }
//   WiFiClient *ttsclient = http.getStreamPtr();
//   ttsclient->setTimeout(10000);
//   if (ttsclient->available() > 0)
//   {
//     int i = 0;
//     int len = sizeof(TTS1_mp3buff);
//     int count = 0;

//     bool data_end = false;
//     while (!data_end)
//     {
//       if (ttsclient->available() > 0)
//       {

//         int bytesread = ttsclient->read(&TTS1_mp3buff[i], len);
//         // Serial.printf("%d Bytes Read\n",bytesread);
//         i = i + bytesread;
//         if (i > sizeof(TTS1_mp3buff))
//         {
//           break;
//         }
//         else
//         {
//           len = len - bytesread;
//           if (len <= 0)
//             break;
//         }
//       }
//       {
//         Serial.printf(" %d Bytes Read\n", i);
//         int lastms = millis();
//         data_end = true;
//         while (millis() - lastms < 600)
//         { // データ終わりか待ってみる
//           if (ttsclient->available() > 0)
//           {
//             data_end = false;
//             break;
//           }
//           yield();
//         }
//       }
//     }

//     Serial.printf("Total %d Bytes Read\n", i);
//     ttsclient->stop();
//     http.end();

//     file_TTS1 = new AudioFileSourcePROGMEM(TTS1_mp3buff, i);
//     mp3->begin(file_TTS1, &out);
//   }
// }

// void VoiceText_tts(char *text, char *tts_parms)
// {
//   file_TTS0 = new AudioFileSourceVoiceTextStream(text, tts_parms);
//   BUFF = new AudioFileSourceBuffer(file_TTS0, TTS_mp3buff, TTS_mp3buffSize);
//   mp3->begin(BUFF, &out);
// }

// void addPeriodBeforeKeyword(String &input, const String keywords[], int numKeywords);

// void addPeriodBeforeKeyword(String &input, const String keywords[], int numKeywords)
// { // keyword の前に「。」または、「.」を挿入する。
//   // String keywords[] = {"(Neutral)", "(Happy)", "(Sleepy)", "(Doubt)", "(Sad)", "(Angry)"};

//   int prevIndex = 0;
//   for (int i = 0; i < numKeywords; i++)
//   {
//     int index = input.indexOf(keywords[i]);
//     while (index != -1)
//     {
//       if (isJP())
//       {
//         // *** [warning対策03] ***
//         // if (index > 0 && input.charAt(index - 1) != '。')
//         //  { input = input.substring(0, index) + "。" + input.substring(index); }
//         // ---------------------------------------------------------------------
//         if (index > 0)
//         {
//           String strLast = input.charAt(index - 1) + "";
//           if (strLast != "。")
//           {
//             input = input.substring(0, index) + "。" + input.substring(index);
//           }
//         }
//       }
//       else
//       {
//         if (index > 0 && input.charAt(index - 1) != '.')
//         {
//           input = input.substring(0, index) + "." + input.substring(index);
//         }
//       }
//       prevIndex = index + keywords[i].length() + 1; // update prevIndex to after the keyword and period
//       index = input.indexOf(keywords[i], prevIndex);
//     }
//   }
//   //  Serial.println(input);
// }



// void getExpression(String &sentence, int &expressionIndx)
// {
// // ----------------------------------------------------------------------------------
// // sentence に　(Happy)などの感情表現が含まれているとその位置を expressionIndx　で返す。
// // 　見つからなかったら、 expressionIndx = -1 を返す
//   // Serial.println("sentence = " + sentence);
//   int startIndex = sentence.indexOf("(");
//   if (startIndex >= 0)
//   {
//     int endIndex = sentence.indexOf(")", startIndex);
//     if (endIndex > 0)
//     {
//       String extractedString = sentence.substring(startIndex + 1, endIndex); // 括弧を含まない部分文字列を抽出
//                                                                              //        Serial.println("extractedString="+extractedString);
//       sentence.remove(startIndex, endIndex - startIndex + 1);                // 括弧を含む部分文字列を削除
//                                                                              //        Serial.println("sentence="+sentence);
//       if (extractedString != "")
//       {
//         expressionIndx = 0;
//         while (1)
//         {
//           if (EXPRESSION_STRING[expressionIndx] == extractedString)
//           {
//             avatar.setExpression(expressions_table[expressionIndx]);
//             break;
//           }
//           if (EXPRESSION_STRING[expressionIndx] == "")
//           {
//             expressionIndx = -1;
//             break;
//           }
//           expressionIndx++;
//         }
//       }
//       else
//       {
//         expressionIndx = -1;
//       }
//     }
//   }
// }
