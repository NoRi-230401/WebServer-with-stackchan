// ----------------------------<wsChatGPT.cpp>------------------------------------
#include "wsChatGPT.h"

const String json_ChatString =
    "{\"model\": \"gpt-3.5-turbo-0613\",\"max_tokens\":512,\"messages\": [{\"role\": \"user\", \"content\": \""
    "\"}]}";

const String CHATDOC_SPI = "/data.json"; // chatDoc in SPIFFS
const int MAX_HISTORY = 5;               // 保存する質問と回答の最大数
String INIT_BUFFER = "";
String CHAT_RESPONSE = "";
DynamicJsonDocument CHAT_DOC(1024 * 10);
std::deque<String> chatHistory; // 過去の質問と回答を保存するデータ構造

const String CHARA_SPIFFS = "/wsCharacter.json";
const String charaJsonInitStr = "{\"character\":[{\"name\":\"\",\"vSpkNo\":\"3\",\"role\":\"\"},{\"name\":\"\",\"vSpkNo\":\"3\",\"role\":\"\"},{\"name\":\"\",\"vSpkNo\":\"3\",\"role\":\"\"},{\"name\":\"\",\"vSpkNo\":\"3\",\"role\":\"\"},{\"name\":\"\",\"vSpkNo\":\"3\",\"role\":\"\"},{\"name\":\"\",\"vSpkNo\":\"3\",\"role\":\"\"},{\"name\":\"\",\"vSpkNo\":\"3\",\"role\":\"\"},{\"name\":\"\",\"vSpkNo\":\"3\",\"role\":\"\"},{\"name\":\"\",\"vSpkNo\":\"3\",\"role\":\"\"}]}";

// 独り言
const String random_words[] = {"あなたは誰", "楽しい", "怒った", "可愛い", "悲しい", "眠い", "ジョークを言って", "泣きたい", "怒ったぞ", "こんにちは", "お疲れ様", "詩を書いて", "疲れた", "お腹空いた", "嫌いだ", "苦しい", "俳句を作って", "歌をうたって"};
bool RANDOM_SPEAK_STATE = false; // 独り言モード　true -> on  false -> off
bool RANDOM_SPEAK_ON_GET = false;
bool RANDOM_SPEAK_OFF_GET = false;
int RANDOM_TIME = -1;
static int LASTMS1 = 0;

// 「わかりません」対策
int WK_CNT = 0;
int WK_ERR_NO = 0;
int WK_ERR_CODE = 0;
int WK_LAST_ERR_NO = 0;
int WK_LAST_ERR_CODE = 0;

void chatGptManage()
{
  // --  RandomSpeakManage ---
  if ((RANDOM_SPEAK_ON_GET) && (SPEECH_TEXT_BUFFER == "") && (SPEECH_TEXT == ""))
  {
    RANDOM_SPEAK_ON_GET = false;

    if (SYSINFO_DISP_STATE)
      sysInfoDispEnd();

    timerStop2();
    randomSpeak(true);
  }

  if (RANDOM_SPEAK_OFF_GET && (SPEECH_TEXT_BUFFER == "") && (SPEECH_TEXT == ""))
  {
    RANDOM_SPEAK_OFF_GET = false;
    randomSpeak(false);
  }

  if ((RANDOM_TIME >= 0) && (millis() - LASTMS1 > RANDOM_TIME))
  {
    LASTMS1 = millis();                      // 今回のRandomSpeak起動した時刻
    RANDOM_TIME = 40000 + 1000 * random(30); // 次回のRandomSpeak起動までの時間
    if ((!mp3->isRunning()) && (SPEECH_TEXT == "") && (SPEECH_TEXT_BUFFER == ""))
    {
      exec_chatGPT(random_words[random(18)]);
    }
  }

  // --- chatGPT REQ ----------
  if (REQ_chatGPT_GET)
  {
    if (SYSINFO_DISP_STATE)
      sysInfoDispEnd();

    randomSpeakStop2();
    timerStop2();

    REQ_chatGPT_GET = false;

    exec_chatGPT(REQ_MSG);
  }
}

void wsHandleRandomSpeak(String modeS)
{
  if (modeS == "")
    return;

  String mode = modeS;
  mode.toUpperCase();

  if (mode == "ON")
  {
    if (!RANDOM_SPEAK_STATE)
      RANDOM_SPEAK_ON_GET = true;
  }
  else if (mode == "OFF")
  {
    if (RANDOM_SPEAK_STATE)
      RANDOM_SPEAK_OFF_GET = true;
  }

  webpage = "randomSpeak : mode = " + modeS;
  Serial.println(webpage);
}

void wsHandelChat(String textS, String voiceS)
{
  if (textS == "")
    return;

  if (voiceS != "")
    TTS2_PARMS = TTS2_SPEAKER + voiceS;

  REQ_chatGPT_GET = true;
  REQ_MSG = textS;

  webpage = "chat : voice = " + voiceS;
  webpage += "chat : text = " + textS;
}

void wsHandelChatCharacter(String ch_NoS, String ch_nameS, String ch_voiceS, String ch_roleS)
{
  Serial.println("no = " +ch_NoS );
  Serial.println("name = " +ch_nameS );
  Serial.println("voice = " +ch_voiceS );
  Serial.println("role = " +ch_roleS );
  
  DynamicJsonDocument charaJson(CHARA_SIZE);
  if (!jsonRead(FLTYPE_SPIFFS, charaJson, CHARA_SPIFFS))
  {
    Serial.println("faile to Read from SPIFFS in wsHandleChatGpt func");
    initCharaJson(charaJson);
    return;
  }

  if ((ch_NoS == "") && (ch_nameS == "") && (ch_voiceS == "") && (ch_roleS == ""))
  {
    // HTMLデータを出力する
    String html = "";
    serializeJsonPretty(charaJson, html);
    // Serial.println(html);
    webpage = html;
    return;
  }

  int charaNo = 0;
  String charaName = "";
  String charaVoiceNo = "3";
  String charaRole = "";

  if (ch_NoS == "0" || ch_NoS == "1" || ch_NoS == "2" || ch_NoS == "3" || ch_NoS == "4" || ch_NoS == "5" || ch_NoS == "6" || ch_NoS == "7" || ch_NoS == "8")
  {
    charaNo = ch_NoS.toInt();
  }
  else
    return;

  if (ch_nameS == "")
    charaName = ch_NoS + "　ばん";
  else
    charaName = ch_nameS;

  int tmpVoiceNo = ch_voiceS.toInt();
  if ((tmpVoiceNo >= 0) && (tmpVoiceNo <= 66))
  {
    charaVoiceNo = ch_voiceS;
  }

  if (ch_roleS != "")
    charaRole = ch_roleS;

  JsonArray jsonArray = charaJson["character"];
  JsonObject object = jsonArray[charaNo];
  object["name"] = charaName;
  object["vSpkNo"] = charaVoiceNo;
  object["role"] = charaRole;

  bool success = jsonSave(charaJson, CHARA_SPIFFS);
  if (!success)
  {
    return;
  }

  webpage = "Character No. = " + ch_NoS  + "  modified";
}



bool initCharaJson(DynamicJsonDocument &charaJson)
{
  DeserializationError error = deserializeJson(charaJson, charaJsonInitStr);
  if (error)
  {
    Serial.println("DeserializationError in initCharaJson func");
    return false;
  }
  jsonSave(charaJson, CHARA_SPIFFS);
  return true;
}

int CHARA_NO= -1;

void wsHandelChatGpt(String historyS, String charaS)
{
  if (historyS == "" && charaS == "")
    return;

  if (charaS == "0" || charaS == "1" || charaS == "2" || charaS == "3" || charaS == "4" || charaS == "5" || charaS == "6" || charaS == "7" || charaS == "8")
  {
    DynamicJsonDocument charaJson(CHARA_SIZE);

    if (!jsonRead(FLTYPE_SPIFFS, charaJson, CHARA_SPIFFS))
    {
      Serial.println("faile to Read from SPIFFS in wsHandleChatGpt func");
      initCharaJson(charaJson);
      return;
    }

    // --- New Character Set -------------------------
    JsonArray jsonArray = charaJson["character"];
    CHARA_NO = charaS.toInt();
    JsonObject chara = jsonArray[CHARA_NO];
    String chara_name = chara["name"];
    String chara_vSpeakerNoS = chara["vSpkNo"];
    String chara_role = chara["role"];

    Serial.println("charaNo = " + String(CHARA_NO, DEC));
    Serial.println("name = " + chara_name);
    Serial.println("vSpeakerNo = " + chara_vSpeakerNoS);
    Serial.println("role = " + chara_role);

    init_chat_doc(json_ChatString.c_str());
    JsonArray messages = CHAT_DOC["messages"];
    JsonObject systemMessage1 = messages.createNestedObject();
    systemMessage1["role"] = "system";
    systemMessage1["content"] = chara_role;

    // 会話履歴をクリア
    chatHistory.clear();
    INIT_BUFFER = "";
    serializeJson(CHAT_DOC, INIT_BUFFER);
    Serial.println("INIT_BUFFER = " + INIT_BUFFER);
    // JSONデータをspiffsへ保存
    save_json();

    // ---- Speaker -------
    size_t speaker_no;
    if (chara_vSpeakerNoS != "")
    {
      uint32_t nvs_handle;

      speaker_no = chara_vSpeakerNoS.toInt();
      if (speaker_no > 66)
      {
        speaker_no = 3;
      }
      TTS2_SPEAKER_NO = String(speaker_no);
      TTS2_PARMS = TTS2_SPEAKER + TTS2_SPEAKER_NO;

      if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
      {
        nvs_set_u32(nvs_handle, "vSpeakerNo", speaker_no);
        Serial.println("NVS Write : vSpeakerNo = " + String(speaker_no, DEC));
      }
      nvs_close(nvs_handle);
    }

    webpage = "character changed<br><br>";
    webpage += "character No. = " + String(CHARA_NO, DEC) + "<br>";
    webpage += "name = " + chara_name + "<br>";
    webpage += "vSpeakerNo = " + chara_vSpeakerNoS + "<br>";
    webpage += "role = " + chara_role + "<br><br>";

    if (chara_name != "")
    {
      String spkMsg = chara_name + " です。";
      Serial.println(spkMsg);
      ReqSpkMsg2(spkMsg);
    }
    return;
  }

  if (historyS.equalsIgnoreCase("all"))
  {
    webpage = "<br>";
    int max_history = chatHistory.size();
    for (int i = 0; i < max_history; i++)
    {
      if ((i % 2) == 0)
      {
        webpage += "Q - " + chatHistory[i] + "<br>";
      }
      else
      {
        webpage += "A - " + chatHistory[i] + "<br><br>";
      }
    }
  }
}

void wsHandleRoleSet(String roleS)
{
  String role = roleS;

  if (role != "")
  {
    init_chat_doc(json_ChatString.c_str());
    JsonArray messages = CHAT_DOC["messages"];
    JsonObject systemMessage1 = messages.createNestedObject();
    systemMessage1["role"] = "system";
    systemMessage1["content"] = role;
    webpage = "chatGPT : set role data ";
  }
  else
  {
    init_chat_doc(json_ChatString.c_str());
    webpage = "chatGPT : clear role data ";
  }
  // 会話履歴をクリア
  chatHistory.clear();

  INIT_BUFFER = "";
  serializeJson(CHAT_DOC, INIT_BUFFER);
  Serial.println("INIT_BUFFER = " + INIT_BUFFER);

  // JSONデータをspiffsへ保存
  save_json();
}

// void wsHandleRoleGet()
// {
//   String chatDocGet = "";
//   serializeJsonPretty(CHAT_DOC, chatDocGet);

//   Serial.println("\n****** role_get ******");
//   Serial.println(chatDocGet);
//   Serial.println("**********************\n");

//   webpage = "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'>";
//   webpage += "<title>StackChanSub</title></head><body><pre>";
//   webpage += chatDocGet;
//   webpage += "</pre></body></html>";
// }

void wsHandleRoleGet()
{
  webpage = "";
  serializeJsonPretty(CHAT_DOC, webpage);
  Serial.println("\n****** role_get ******");
  Serial.println(webpage);
  Serial.println("**********************\n");
}

bool chatDocInit()
{
  File fl_SPIFFS = SPIFFS.open(CHATDOC_SPI, "r");
  if (!fl_SPIFFS)
  {
    fl_SPIFFS.close();
    String errorMsg1 = "*** Failed to open file for reading *** ";
    String errorMsg2 = "*** FATAL ERROR : cannot READ CHAT_DOC FILE !!!! ***";
    Serial.println(errorMsg1);
    Serial.println(errorMsg2);
    M5.Lcd.print(errorMsg1);
    M5.Lcd.print(errorMsg2);

    init_chat_doc(json_ChatString.c_str());
    return false;
  }

  DeserializationError error = deserializeJson(CHAT_DOC, fl_SPIFFS);
  fl_SPIFFS.close();

  if (error)
  { // ファイルの中身が壊れていた時の処理 ---------
    Serial.println("Failed to deserialize JSON");
    if (!init_chat_doc(json_ChatString.c_str()))
    {
      String errorMsg1 = "*** Failed to init chat_doc JSON in SPIFFS *** ";
      String errorMsg2 = "*** FATAL ERROR : cannot READ/WRITE CHAT_DOC FILE !!!! ***";
      Serial.println(errorMsg1);
      Serial.println(errorMsg2);
      M5.Lcd.print(errorMsg1);
      M5.Lcd.print(errorMsg2);
      return false;
    }
    else
    { // JSONファイルをSPIFF に保存
      fl_SPIFFS = SPIFFS.open(CHATDOC_SPI, "w");
      if (!fl_SPIFFS)
      {
        fl_SPIFFS.close();
        String errorMsg1 = "*** Failed to open file for writing *** ";
        String errorMsg2 = "*** FATAL ERROR : cannot WRITE CHAT_DOC FILE !!!! ***";
        Serial.println(errorMsg1);
        Serial.println(errorMsg2);
        M5.Lcd.print(errorMsg1);
        M5.Lcd.print(errorMsg2);
        return false;
      }
      serializeJson(CHAT_DOC, fl_SPIFFS);
      fl_SPIFFS.close();
      Serial.println("initial chat_doc data store in SPIFFS");
    }
  }

  serializeJson(CHAT_DOC, INIT_BUFFER);
  // Role_JSON = INIT_BUFFER;
  String json_str;
  serializeJsonPretty(CHAT_DOC, json_str); // 文字列をシリアルポートに出力する
  Serial.println(json_str);
  return true;
}

void randomSpeakStop2()
{
  RANDOM_TIME = -1;
  RANDOM_SPEAK_STATE = false;
  RANDOM_SPEAK_ON_GET = false;
}

void randomSpeak(bool mode)
{
  String speakMsg;

  if (mode)
  {
    speakMsg = "独り言始めます。";
    SPEECH_TEXT_BUFFER = "";
    LASTMS1 = millis();
    RANDOM_TIME = 40000 + 1000 * random(30);
    RANDOM_SPEAK_STATE = true;
  }
  else
  {
    speakMsg = "独り言やめます。";
    SPEECH_TEXT_BUFFER = "";
    RANDOM_TIME = -1;
    RANDOM_SPEAK_STATE = false;
  }
  RANDOM_SPEAK_ON_GET = false;
  avatar.setExpression(Expression::Happy);
  ttsDo(speakMsg);
  avatar.setExpression(Expression::Neutral);
  Serial.println("mp3 begin");
}

bool init_chat_doc(const char *data)
{

  DeserializationError error = deserializeJson(CHAT_DOC, data);
  if (error)
  {
    Serial.println("DeserializationError");
    return false;
  }
  String json_str;                         //= JSON.stringify(chat_doc);
  serializeJsonPretty(CHAT_DOC, json_str); // 文字列をシリアルポートに出力する
  Serial.println(json_str);
  return true;
}

String https_post_json(const char *url, const char *json_string, const char *root_ca)
{
  WK_ERR_NO = 0;
  WK_ERR_CODE = 0;

  String payload = "";
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client)
  {
    client->setCACert(root_ca);
    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
      HTTPClient https;
      https.setTimeout(UINT16_MAX); // 最大値の約65秒にタイムアウトを設定

      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, url))
      { // HTTPS
        Serial.print("[HTTPS] POST...\n");
        // start connection and send HTTP header
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Authorization", String("Bearer ") + OPENAI_API_KEY);
        int httpCode = https.POST((uint8_t *)json_string, strlen(json_string));

        WK_ERR_CODE = httpCode;
        Serial.print(" httpCode = ");
        Serial.println(httpCode, DEC);

        // httpCode will be negative on error
        if (httpCode > 0)
        {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
          {
            payload = https.getString();
            Serial.println("/////payload-start/////");
            Serial.print(payload);
            Serial.println("/////payload-end/////");

            if (payload == "")
            {
              Serial.println("CODE_OK or CODE_MOVED_PERMANENTLY and payload is void ");
              WK_ERR_NO = 1;
            }
          }
          else
          { // エラーコードを取得できた場合
            Serial.println("httpCode other error code number get ");
            WK_ERR_NO = 2;
          }
        }
        else
        {
          Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
          WK_ERR_NO = 3;
        }
        https.end();
      }
      else
      {
        Serial.printf("[HTTPS] Unable to connect\n");
        WK_ERR_NO = 4;
      }
      // End extra scoping block
    }
    delete client;
  }
  else
  {
    Serial.println("Unable to create client");
    WK_ERR_NO = 5;
  }
  return payload;
}

String chatGpt(String json_string)
{
  String response = "";
  avatar.setExpression(Expression::Doubt);

  avatar.setSpeechText("考え中…");

  // LED 3番と7番を黄色に光らせる
  led_setColor4(2, 255, 255, 255); // 白色
  led_setColor4(7, 255, 255, 255); // 白色
  led_show();

  String ret = https_post_json("https://api.openai.com/v1/chat/completions", json_string.c_str(), root_ca_openai);
  avatar.setExpression(Expression::Neutral);
  avatar.setSpeechText("");
  // Serial.println(ret);

  // 音声が再生された後にLEDを消灯
  led_setColor4(2, 0, 0, 0); // 黒（消灯）
  led_setColor4(7, 0, 0, 0); // 黒（消灯）
  led_show();

  if (ret != "")
  {
    WK_CNT = 0;
    DynamicJsonDocument doc(DOC_SIZE);
    DeserializationError error = deserializeJson(doc, ret.c_str());

    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      avatar.setExpression(Expression::Sad);

      avatar.setSpeechText("エラーです");
      response = "エラーです";

      delay(1000);
      avatar.setSpeechText("");
      avatar.setExpression(Expression::Neutral);
    }
    else
    {
      const char *data = doc["choices"][0]["message"]["content"];
      // Serial.println(data);
      response = String(data);
      std::replace(response.begin(), response.end(), '\n', ' ');
    }
  }
  else
  {
    // 音声が再生された後にLEDを消灯
    led_setColor4(2, 0, 0, 0); // 黒（消灯）
    led_setColor4(7, 0, 0, 0); // 黒（消灯）
    led_show();

    // ---「わかりません」エラー番号とコード情報の発声 ---
    char msg1[200];

    if (WK_ERR_NO != 0)
    {
      if (WK_ERR_CODE < 0)
        sprintf(msg1, "わかりません、番号 %d、コード・マイナス %d です。", WK_ERR_NO, abs(WK_ERR_CODE));
      else
        sprintf(msg1, "わかりません、番号 %d、コード %d です。", WK_ERR_NO, abs(WK_ERR_CODE));
    }
    else
    {
      sprintf(msg1, "わかりません、番号 %d です。", WK_ERR_NO);
    }

    WK_LAST_ERR_NO = WK_ERR_NO;
    WK_LAST_ERR_CODE = WK_ERR_CODE;
    WK_ERR_NO = 0;
    WK_ERR_CODE = 0;

    WK_CNT++;
    Serial.print("WK_CNT = ");
    Serial.println(WK_CNT, DEC);
    char msg2[200] = "";

    char msg[200];
    sprintf(msg, "%s %s", msg1, msg2);
    avatar.setExpression(Expression::Sad);
    avatar.setSpeechText(msg);
    response = msg;
    Serial.println(msg);
    // delay(1000);
    delay(2000); // *** [わかりません対策01] ***
    avatar.setSpeechText("");
    avatar.setExpression(Expression::Neutral);

    // 音声が再生された後にLEDを消灯
    led_setColor4(2, 0, 0, 0); // 黒（消灯）
    led_setColor4(7, 0, 0, 0); // 黒（消灯）
    led_show();
  }
  return response;
}

void exec_chatGPT(String text)
{
  Serial.println("\n~~ Send Msg to chatGPT ~~");
  Serial.println(text);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~\n");

  CHAT_RESPONSE = "";
  init_chat_doc(INIT_BUFFER.c_str());
  chatHistory.push_back(text);
  // チャット履歴が最大数を超えた場合、古い質問と回答を削除
  if (chatHistory.size() > MAX_HISTORY * 2)
  {
    chatHistory.pop_front();
    chatHistory.pop_front();
  }

  for (int i = 0; i < chatHistory.size(); i++)
  {
    JsonArray messages = CHAT_DOC["messages"];
    JsonObject systemMessage1 = messages.createNestedObject();
    if (i % 2 == 0)
    {
      systemMessage1["role"] = "user";
    }
    else
    {
      systemMessage1["role"] = "assistant";
    }
    systemMessage1["content"] = chatHistory[i];
  }

  String json_string;
  serializeJson(CHAT_DOC, json_string);
  if (SPEECH_TEXT == "" && SPEECH_TEXT_BUFFER == "")
  {
    CHAT_RESPONSE = chatGpt(json_string);
    SPEECH_TEXT = CHAT_RESPONSE;
    // 返答をチャット履歴に追加
    chatHistory.push_back(CHAT_RESPONSE);
  }
  else
  {
    CHAT_RESPONSE = "busy";
  }

  serializeJsonPretty(CHAT_DOC, json_string);
}

bool save_json()
{
  File fl_SPIFFS = SPIFFS.open(CHATDOC_SPI, "w");
  if (!fl_SPIFFS)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }

  serializeJson(CHAT_DOC, fl_SPIFFS);
  fl_SPIFFS.close();
  return true;
}

void chatGptSetup()
{
  // ***  chat_doc initialize  ****
  bool success = chatDocInit();
  if (!success)
  {
    String msg = "cannnot init chat_doc! ";
    Serial.println(msg);
    M5.Display.println();
    M5.Display.println(msg);
    Serial.println(" *** Reboot ***");
    REBOOT();
  }

  // *** final msg in setup() ****
  Serial.println("HTTP server started");
  M5.Lcd.println("HTTP server started");
  Serial.println("_  to control the chatGpt Server.");
  M5.Lcd.println("_  to control the chatGpt Server.");
}

void chatHistoryCls()
{
  // 会話履歴をクリア
  chatHistory.clear();
}
