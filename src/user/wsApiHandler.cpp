// ----------------------------<wsApiHandler.cpp>------------------------------------
#include "wsApiHandler.h"

void setupApiHandler()
{
  // ##################### setting ############################
  server.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_setting(request); serverSend(request); });

  // ##################### setting2 ############################
  server.on("/setting2", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_setting2(request); serverSend(request); });

  // ##################### sysInfo ############################
  server.on("/sysInfo", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_sysInfo(request);   serverSend(request); });

  // ##################### button ############################
  server.on("/button", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_button(request); serverSend(request); });

  // ##################### shutdown ############################
  server.on("/shutdown", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_shutdown(request);  serverSend(request); });

  // ##################### startup ############################
  server.on("/startup", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_startup(request); serverSend(request); });

  // ##################### wifiSelect ############################
  server.on("/wifiSelect", HTTP_GET, [](AsyncWebServerRequest *request)
            {  handle_WifiSetting(request); serverSend(request); });

  // ##################### apiKey ############################
  server.on("/apikey", HTTP_GET, [](AsyncWebServerRequest *request)
            {  handle_apikey(request); serverSend(request); });

  // ##################### apikey_set ############################
  // server.on("/apikey_set", HTTP_GET, [](AsyncWebServerRequest *request)
  //           {  handle_apikey_set(request); serverSend(request); });

  // ##################### servo ############################
  server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request)
            {  handle_servo(request);  serverSend(request); });

  // ##################### exServo ############################
  server.on("/servoSetting", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_servoSetting(request); serverSend(request); });

  // ##################### speech ############################
  server.on("/speech", HTTP_GET, [](AsyncWebServerRequest *request)
            {    handle_speech(request);    serverSend(request); });

  // ##################### face ############################
  server.on("/face", HTTP_GET, [](AsyncWebServerRequest *request)
            {    handle_face(request);    serverSend(request); });

  // ##################### chat ############################
  server.on("/chat", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_chat(request); request->send(200, "text/plain", "OK"); });

  // ##################### randomSpeak ############################
  server.on("/randomSpeak", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_randomSpeak(request);   serverSend(request); });

  // ##################### role_set ############################
  server.on("/role_set", HTTP_POST, [](AsyncWebServerRequest *request)
            { handle_role_set(request);  request->send(200, "text/html", webpage); });

  // ##################### role_get ############################
  server.on("/role_get", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_role_get(request);  request->send(200, "text/html", webpage); });

  // ##################### timer ############################
  server.on("/timer", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_timer(request);    serverSend(request); });

}


void handle_button(AsyncWebServerRequest *request)
{
  if (TONE_MODE != 3)
    tone(2);

  webpage = "NG";
  String arg_str = request->arg("push");
  wsHandleBtn(arg_str);
}

void handle_sysInfo(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String txS = request->arg("tx");
  String dispS = request->arg("disp");
  String modeS = request->arg("mode");
  wsHandleSysInfo(txS, dispS, modeS);
}

void handle_servo(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String swingXYS = request->arg("swingXY");
  String swingXS = request->arg("swingX");
  String swingYS = request->arg("swingY");
  String pointXS = request->arg("pointX");
  String pointYS = request->arg("pointY");
  String deltaXS = request->arg("deltaX");
  String deltaYS = request->arg("deltaY");
  String txS = request->arg("tx");
  String modeS = request->arg("mode");
  wsHandleServo(swingXYS, swingXS, swingYS,
                pointXS, pointYS, deltaXS, deltaYS, txS, modeS);
}

void handle_setting(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String volumeS = request->arg("volume");
  String volumeDS = request->arg("volumeDelta");
  String speakerS = request->arg("speaker");
  String ledS = request->arg("led");
  wsHandleSetting(volumeS, volumeDS, speakerS, ledS);
}

void handle_setting2(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String langS = request->arg("lang");
  String ttsNameS = request->arg("ttsSelect");
  String muteS = request->arg("mute");
  String keyLockS = request->arg("keyLock");
  String toneModeS = request->arg("toneMode");
  wsHandleSetting2(langS, ttsNameS, muteS, keyLockS, toneModeS);
}

void handle_shutdown(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String reboot_get_str = request->arg("reboot");
  String time_get_str = request->arg("time");
  wsHandleShutdown(reboot_get_str, time_get_str);
}

void handle_timer(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String TmSecS = request->arg("setTimeSec");
  String TmMinS = request->arg("setTimeMin");
  String timerModeS = request->arg("mode");
  wsHandleTimer(TmSecS, TmMinS, timerModeS);
}

void handle_face(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String expression = request->arg("expression");
  wsHandleFace(expression);
}

void handle_speech(AsyncWebServerRequest *request)
{
  // tone(2);
  webpage = "NG";
  String sayS = request->arg("say");
  String expressionS = request->arg("expression");
  String voiceS = request->arg("voice");
  wsHandleSpeech(sayS, expressionS, voiceS);
}

void handle_chat(AsyncWebServerRequest *request)
{
  // tone(2);
  webpage = "NG";
  String textS = request->arg("text");
  String voiceS = request->arg("voice");
  wsHandelChat(textS, voiceS);
}

void handle_role_set(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String roleS = request->arg("textarea");
  wsHandleRoleSet(roleS);
  wsHandleRoleGet();
}

void handle_role_get(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  wsHandleRoleGet();
}

void handle_randomSpeak(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String modeS = request->arg("mode");
  wsHandleRandomSpeak(modeS);
}

void handle_WifiSetting(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String initS = request->arg("init");
  String ssidS = request->arg("ssid");
  String passwdS = request->arg("passwd");
  String removeS = request->arg("remove");
  String ipS = request->arg("ip");
  String gatewayS = request->arg("gateway");
  String subnetS = request->arg("subnet");
  String dnsS = request->arg("dns");
  wsHandleWifiSetting(initS, ssidS, passwdS, removeS, ipS, gatewayS, subnetS, dnsS);
}

void handle_apikey(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String openAiS = request->arg("openAiApiKey");
  String voicevoxS = request->arg("voicevoxApiKey");
  String voiceTextS = request->arg("voiceTextApiKey");
  String txS = request->arg("tx");
  wsHandleApikeySetting(openAiS,voicevoxS,voiceTextS,txS);
}

// void handle_apikey_set(AsyncWebServerRequest *request)
// {
//   tone(2);
//   webpage = "NG";
//   String openai = request->arg("openai");
//   String voicevox = request->arg("voicevox");
//   String voicetext = request->arg("voicetext");
//   String sttapikey = request->arg("sttapikey");
//  wsHandleApikeySet(openai, voicevox, voicetext, sttapikey);
// }


void handle_startup(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String ttsSelectS = request->arg("ttsSelect");
  String vvoxSpeakerNoS = request->arg("voicevoxSpeakerNo");
  String langS = request->arg("lang");
  String volumeS = request->arg("volume");
  String ledS = request->arg("led");
  String randomSpeakS = request->arg("randomSpeak");
  String toneModeS = request->arg("toneMode");
  String muteS = request->arg("mute");
  String keyLockS = request->arg("keyLock");
  String timerS = request->arg("timer");
  String txS = request->arg("tx");
  wsHandleStartup(ttsSelectS, vvoxSpeakerNoS,langS,volumeS, ledS,
      randomSpeakS, toneModeS, muteS, keyLockS, timerS, txS);
}

void handle_servoSetting(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String txS = request->arg("tx");
  String servoS = request->arg("servo");
  String servoPortS= request->arg("servoPort");
  String servoModeS = request->arg("servoMode");
  String servoHomeXS = request->arg("servoHomeX");
  String servoHomeYS = request->arg("servoHomeY");
    wsServoSetting( txS, servoS, servoPortS,servoModeS,servoHomeXS, servoHomeYS);
}
