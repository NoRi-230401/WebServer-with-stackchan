#ifndef _VOICEVOX_H
#define _VOICEVOX_H

#include <M5Unified.h>
#include <Avatar.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>

#include "../lib/AudioOutputM5Speaker.h"
#include "../lib/AudioFileSourceHTTPSStream.h"


// #include "AudioOutputM5Speaker.h"
// #include "AudioFileSourceHTTPSStream.h"

// #include <AudioFileSourceSD.h>
// #include "MyFunction.h"
#include "RootCA.h"

extern m5avatar::Avatar avatar;
// extern String voicevox_apikey;
extern String VOICEVOX_API_KEY;
// extern uint8_t config_speaker;
extern uint16_t https_timeout;
// extern String speaker_name;
extern void log_free_size(const char *text);

class VoiceVox {
public:
    VoiceVox();
    ~VoiceVox();
    // String synthesis(String text);
    String synthesis(const String &text);
    void talk_https(String url);
    // void talk_sd(String path);
    AudioGeneratorMP3 *mp3;
    AudioFileSourceHTTPSStream *file_https;
    // AudioFileSourceSD *file_sd;
    AudioFileSourceBuffer *buff;
    AudioOutputM5Speaker *out;
    bool is_talking = false;
    String talk_type;
    uint32_t getStartTime();
    void setStartTime();
    uint8_t getSpkNo();
    void setSpkNo(uint8_t spk_no);

private:
    const String url = "https://api.tts.quest/v3/voicevox/synthesis";
    uint32_t voicevox_start_time = 0;
    uint8_t spkNo = 3;
};

#endif
