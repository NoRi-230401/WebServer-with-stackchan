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
#include "RootCA.h"

extern m5avatar::Avatar avatar;
extern String VOICEVOX_API_KEY;
extern void log_free_size(const char *text);
extern void showExeTime(String msg , bool resetTm );

class VoiceVox {
public:
    VoiceVox();
    ~VoiceVox();
    String synthesis(const String &text);
    void talk_https(String url);
    AudioGeneratorMP3 *mp3;
    AudioFileSourceHTTPSStream *file_https;
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
