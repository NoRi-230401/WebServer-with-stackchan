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
// extern void log_free_size(const char *text);
// #define EXE_TM_MD0 0      // (default) disp and timer reset
// #define EXE_TM_MD1 1      // disp and timer no reset
// #define EXE_TM_MD2 2      // no disp and timer reset
// #define EXE_TM_MD_START 3 // no disp and timer reset for START 
// extern void showExeTime(String msg, int mode = EXE_TM_MD0);

// --- Wss StaTe (WST) define ------- 
extern uint16_t WST; 
#define WST_TTS_start     0x5000
#define WST_TTS_mp3urlGet 0x5100
#define WST_TTS_talkStart 0x5200
#define WST_TTS_exit      0x5800
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
