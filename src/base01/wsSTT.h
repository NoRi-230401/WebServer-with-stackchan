// ---------------------------< wsTts.h >------------------------------------
#ifndef WS_STT_H
#define WS_STT_H
// ---------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Avatar.h>
#include <AudioOutput.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>
#include <AudioFileSourcePROGMEM.h>
#include <M5Unified.h>
#include "../lib/AudioWhisper.h"
#include "../lib/Whisper.h"
#include "../lib/Audio.h"

// -- Servo Mode define ----
#define SV_MD_MOVING 0
#define SV_MD_HOME 1
#define SV_MD_RANDOM 2
#define SV_MD_CENTER 3
#define SV_MD_SWING 4
#define SV_MD_STOP 5
#define SV_MD_ADJUST 6
#define SV_MD_POINT 7
#define SV_MD_DELTA 8
#define SV_MD_NONE 99

extern String STT_API_KEY;
extern String OPENAI_API_KEY;
extern bool RANDOM_SPEAK_STATE;
extern AudioGeneratorMP3 *mp3;
extern String SPEECH_TEXT;
extern String SPEECH_TEXT_BUFFER;
extern const char* root_ca_openai;
extern int SV_MD;
extern bool SV_USE;
extern int RANDOM_TIME;
using namespace m5avatar;
extern Avatar avatar;
extern void randomSpeakStop2();
extern void tone(int toneMode);

//--------------------------------------------
String SpeechToText();
void SST_ChatGPT();
void exec_chatGPT(String text);
//--------------------------------------------

#endif
