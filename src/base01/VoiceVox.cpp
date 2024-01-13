#include "VoiceVox.h"

TaskHandle_t voicevox_task_handle;

void voicevox_task_loop(void *args) {
    VoiceVox *ptr = static_cast<VoiceVox*>(args);
    for (;;) {
        if (ptr->is_talking && ptr->mp3->isRunning()) {
            float level = abs(*(ptr->out->getBuffer()));
            if (level < 100) { level = 0; }
            if (level > 15000) { level = 15000; }
            // if (ptr->talk_type == "URL") { avatar.setSpeechText(speaker_name.c_str()); }
            avatar.setMouthOpenRatio(level / 15000.0);
            if (!ptr->mp3->loop()) {
                ptr->is_talking = false;
                ptr->mp3->stop();
                delete ptr->mp3; ptr->mp3 = nullptr;
                delete ptr->out; ptr->out = nullptr;
                if (ptr->talk_type == "URL") {
                    delete ptr->file_https; ptr->file_https = nullptr;
                    delete ptr->buff; ptr->buff = nullptr;
                }
                // if (ptr->talk_type == "FILE") {
                //     delete ptr->file_sd; ptr->file_sd = nullptr;
                // }
                avatar.setMouthOpenRatio(0);
                avatar.setSpeechText("");
                // M5.Speaker.end();
                // M5.Mic.begin();
                log_free_size("VOICEVOX：OUT");
                M5.Log.println("発話：終了");
            }
        }
        vTaskDelay(1);
    }
}

VoiceVox::VoiceVox() {
    is_talking = false;
    xTaskCreateUniversal(voicevox_task_loop, "voicevox_task_loop", 4096, this, 1, &voicevox_task_handle, APP_CPU_NUM);
}

VoiceVox::~VoiceVox() {
    vTaskDelete(voicevox_task_handle);
}

String VoiceVox::synthesis(String text) {
    M5.Log.println("VOICEVOX：開始");
    uint32_t start_time = millis();

    HTTPClient https;
    https.setTimeout(https_timeout);
    if (!https.begin(url, root_ca_voicevox)) {
        M5.Log.println("VOICEVOX：接続失敗");
        return "";
    }

    https.addHeader("content-type", "application/x-www-form-urlencoded");

    const String send_data = "key=" + VOICEVOX_API_KEY + "&speaker=" + config_speaker + "&text=" + text;

    int http_code = https.POST(send_data.c_str());
    if (!(http_code == HTTP_CODE_OK)) {
        M5.Log.printf("VOICEVOX：HTTPSエラー(%d %s)\n", http_code, https.errorToString(http_code).c_str());
        avatar.setSpeechText("たいむあうと");
        avatar.setExpression(m5avatar::Expression::Doubt);
        delay(2000);
        return "";
    }

    const String response = https.getString();
    https.end();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        M5.Log.println("VOICEVOX：JSONシリアル化エラー");
        return "";
    }

    const String mp3_url = doc["mp3StreamingUrl"].as<String>();
    doc.clear();

    M5.Log.printf("VOICEVOX：%s(%.1f秒)\n", mp3_url.c_str(), (millis() - start_time) / 1000.0);
    M5.Log.println("VOICEVOX：終了");
    return mp3_url;
}

void VoiceVox::talk_https(String url) {
    M5.Log.println("発話：開始");
    if (is_talking) { M5.Log.println("発話：スキップ"); return; }
    mp3 = new AudioGeneratorMP3();
    out = new AudioOutputM5Speaker(&M5.Speaker);
    file_https = new AudioFileSourceHTTPSStream(url.c_str(), root_ca_voicevox);
    buff = new AudioFileSourceBuffer(file_https, 1024 * 10);
    // M5.Mic.end();
    M5.Speaker.begin();
    is_talking = true;
    talk_type = "URL";
    mp3->begin(buff, out);
}

// void VoiceVox::talk_sd(String path) {
//     M5.Log.println("発話：開始");
//     if (is_talking) { M5.Log.println("発話：スキップ"); return; }
//     mp3 = new AudioGeneratorMP3();
//     out = new AudioOutputM5Speaker(&M5.Speaker);
//     file_sd = new AudioFileSourceSD(path.c_str());
//     M5.Mic.end();
//     M5.Speaker.begin();
//     is_talking = true;
//     talk_type = "FILE";
//     mp3->begin(file_sd, out);
// }
