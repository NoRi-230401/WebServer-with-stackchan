#include "VoiceVox.h"

#define HTTPS_TIMEOUT   60000U      // HTTPタイムアウト時間

TaskHandle_t voicevox_task_handle;

void voicevox_task_loop(void *args)
{
    VoiceVox *ptr = static_cast<VoiceVox *>(args);
    for (;;)
    {
        if (ptr->is_talking && ptr->mp3->isRunning())
        {
            float level = abs(*(ptr->out->getBuffer()));
            if (level < 100)
                level = 0;
            else if (level > 15000)
                level = 15000;
            avatar.setMouthOpenRatio(level / 15000.0);

            if (!ptr->mp3->loop())
            {
                ptr->is_talking = false;
                ptr->mp3->stop();
                delete ptr->mp3;
                ptr->mp3 = nullptr;
                delete ptr->out;
                ptr->out = nullptr;
                if (ptr->talk_type == "URL")
                {
                    delete ptr->file_https;
                    ptr->file_https = nullptr;
                    delete ptr->buff;
                    ptr->buff = nullptr;
                }
                avatar.setMouthOpenRatio(0);
                avatar.setSpeechText("");
                M5.Log.printf("VOICEVOX：speak_end(%.1fsec)\n", (millis() - ptr->getStartTime()) / 1000.0);
                // showExeTime("VOICEVOX：elapsed time", true);
                log_free_size("VOICEVOX：OUT");
            }
        }
        vTaskDelay(1);
    }
}

VoiceVox::VoiceVox()
{
    is_talking = false;
    xTaskCreateUniversal(voicevox_task_loop, "voicevox_task_loop", 4096, this, 1, &voicevox_task_handle, APP_CPU_NUM);
}

VoiceVox::~VoiceVox()
{
    vTaskDelete(voicevox_task_handle);
}

String VoiceVox::synthesis(const String &speechText)
{
    setStartTime();
    M5.Log.println("VOICEVOX：START");

    HTTPClient https;
    // https.setTimeout(HTTPS_TIMEOUT);
    https.setTimeout(UINT16_MAX);   // 最大値の約65秒にタイムアウトを設定
    if (!https.begin(url, root_ca_voicevox))
    {
        M5.Log.println("VOICEVOX：接続失敗");
        return "";
    }

    https.addHeader("content-type", "application/x-www-form-urlencoded");
    const String send_data = "key=" + VOICEVOX_API_KEY + "&speaker=" + spkNo + "&text=" + speechText;

    int http_code = https.POST(send_data.c_str());
    if (!(http_code == HTTP_CODE_OK))
    {
        M5.Log.printf("VOICEVOX：HTTPSエラー(%d %s)\n", http_code, https.errorToString(http_code).c_str());
        avatar.setSpeechText("TimeOut Error");
        avatar.setExpression(m5avatar::Expression::Doubt);
        delay(2000);
        return "";
    }

    const String response = https.getString();
    https.end();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);
    if (error)
    {
        M5.Log.println("VOICEVOX：JSONシリアル化エラー");
        return "";
    }

    const String mp3_url = doc["mp3StreamingUrl"].as<String>();
    doc.clear();

    M5.Log.printf("VOICEVOX：mp3_URL_Get(%.1fsec)\n", (millis() - getStartTime()) / 1000.0);
    return mp3_url;
}

void VoiceVox::talk_https(String url)
{
    M5.Log.printf("VOICEVOX：speak_start(%.1fsec)\n", (millis() - getStartTime()) / 1000.0);
    // showExeTime("VOICEVOX：SpeakStart:elapsed time", false);

    if (is_talking)
    {
        M5.Log.println("SPEAK：スキップ");
        return;
    }
    mp3 = new AudioGeneratorMP3();
    out = new AudioOutputM5Speaker(&M5.Speaker);
    file_https = new AudioFileSourceHTTPSStream(url.c_str(), root_ca_voicevox);
    buff = new AudioFileSourceBuffer(file_https, 1024 * 10);
    // buff = new AudioFileSourceBuffer(file_https, 1024 * 20);

    // M5.Mic.end();
    // M5.Speaker.begin();
    is_talking = true;
    talk_type = "URL";
    mp3->begin(buff, out);
}

uint8_t VoiceVox::getSpkNo()
{
    return spkNo;
}

void VoiceVox::setSpkNo(uint8_t spk_no)
{
    spkNo = spk_no;
}

uint32_t VoiceVox::getStartTime()
{
    return voicevox_start_time;
}

void VoiceVox::setStartTime()
{
    voicevox_start_time = millis();
}