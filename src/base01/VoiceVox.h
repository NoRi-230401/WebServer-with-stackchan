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
extern uint8_t config_speaker;
extern uint16_t https_timeout;
extern String speaker_name;
extern void log_free_size(const char *text);

class VoiceVox {
public:
    VoiceVox();
    ~VoiceVox();
    String synthesis(String text);
    void talk_https(String url);
    // void talk_sd(String path);
    AudioGeneratorMP3 *mp3;
    AudioFileSourceHTTPSStream *file_https;
    // AudioFileSourceSD *file_sd;
    AudioFileSourceBuffer *buff;
    AudioOutputM5Speaker *out;
    bool is_talking = false;
    String talk_type;
private:
    const String url = "https://api.tts.quest/v3/voicevox/synthesis";
};

#endif
