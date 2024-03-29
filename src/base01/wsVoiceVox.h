#ifndef _WS_VOICEVOX_H
#define _WS_VOICEVOX_H

#include <M5Unified.h>
#include <Avatar.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>
#include "../lib/AudioOutputM5Speaker.h"
#include "../lib/AudioFileSourceHTTPSStream.h"
#include "wsRootCA.h"

extern m5avatar::Avatar avatar;
extern String VOICEVOX_API_KEY;

// --- Wss StaTe (WST) define ------- 
extern uint16_t WST; 
#define WST_TTS_talkDone  0x5900

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
    uint8_t getSpkNo();
    void setSpkNo(uint8_t spk_no);

private:
    const String url = "https://api.tts.quest/v3/voicevox/synthesis";
    uint8_t spkNo = 3;
};

#endif
