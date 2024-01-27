// ----------------------------<wsApi.cpp>------------------------------------
#include "../h/wsApi.h"

void setupApiHandler()
{
  // ##################### setting ############################
  server.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_setting(request); serverSend(request); });

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
            { handle_startupSetting(request); serverSend(request); });

  // ##################### apiKey ############################
  server.on("/apikey", HTTP_GET, [](AsyncWebServerRequest *request)
            {  handle_apikeySetting(request); serverSend(request); });

  // ##################### wifiSelect ############################
  server.on("/wifiSelect", HTTP_GET, [](AsyncWebServerRequest *request)
            {  handle_wifiSetting(request); serverSend(request); });

  // ##################### exServo ############################
  server.on("/servoSetting", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_servoSetting(request); serverSend(request); });

  // ##################### servo ############################
  server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request)
            {  handle_servo(request);  serverSend(request); });

  // ##################### speech ############################
  server.on("/speech", HTTP_GET, [](AsyncWebServerRequest *request)
            {    handle_speech(request);  serverSend(request); });

  // ##################### face ############################
  server.on("/face", HTTP_GET, [](AsyncWebServerRequest *request)
            {    handle_face(request); serverSend(request); });

  // ##################### face ############################
  server.on("/balloon", HTTP_GET, [](AsyncWebServerRequest *request)
            {    handle_balloon(request); serverSend(request); });

  // ##################### chatGpt ############################
  server.on("/chatGpt", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_chatGpt(request); serverSend3(request); });
  
  // ##################### chat ############################
  server.on("/chat", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_chat(request); serverSend(request); });
  
  // ##################### chat ############################
  server.on("/chatChara", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_chatCharacter(request); serverSend(request); });
  
  // ##################### role_set ############################
  server.on("/role_set", HTTP_POST, [](AsyncWebServerRequest *request)
            { handle_role_set(request);  serverSend(request); });

  // ##################### role_get ############################
    server.on("/role_get", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_role_get(request);  serverSend(request); });

  // ##################### randomSpeak ############################
  server.on("/randomSpeak", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_randomSpeak(request);   serverSend(request); });

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
  String vSpkNoS = request->arg("vSpkNo");
  String ledS = request->arg("led");
  String muteS = request->arg("mute");
  String keyLockS = request->arg("keyLock");
  String toneModeS = request->arg("toneMode");
  wsHandleSetting(volumeS, volumeDS, vSpkNoS, ledS, muteS, keyLockS, toneModeS);
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

void handle_balloon(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String text = request->arg("text");
  wsHandleBalloon(text);
}

void handle_speech(AsyncWebServerRequest *request)
{
  // tone(2);
  webpage = "NG";
  String sayS = request->arg("say");
  String expressionS = request->arg("expression");
  String balloonS = request->arg("balloon");
  String voiceS = request->arg("voice");
  String afterExpS = request->arg("afterExp");
  wsHandleSpeech(sayS, expressionS, balloonS, voiceS, afterExpS);
}

// chatHistory etc, utility for chatGpt -----------
void handle_chatGpt(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String historyS = request->arg("history");
  String charaS = request->arg("chara");
  
  wsHandelChatGpt(historyS,charaS);
}

void handle_chat(AsyncWebServerRequest *request)
{
  // tone(2);
  webpage = "NG";
  String textS = request->arg("text");
  String voiceS = request->arg("voice");
  wsHandelChat(textS, voiceS);
}

void handle_chatCharacter(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String ch_NoS = request->arg("no");
  String ch_nameS = request->arg("name");
  String ch_voiceS = request->arg("voice");
  String ch_roleS = request->arg("role");
  wsHandelChatCharacter( ch_NoS, ch_nameS, ch_voiceS, ch_roleS);
}


void handle_role_set(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String roleS = request->arg("textarea");
  wsHandleRoleSet(roleS);
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

void handle_wifiSetting(AsyncWebServerRequest *request)
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

void handle_apikeySetting(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String openAiS = request->arg("openAiApiKey");
  String voicevoxS = request->arg("voicevoxApiKey");
  String txS = request->arg("tx");
  wsHandleApikeySetting(openAiS,voicevoxS,txS);
}

void handle_startupSetting(AsyncWebServerRequest *request)
{
  tone(2);
  webpage = "NG";
  String serverNameS = request->arg("serverName");
  String vSpkNoS = request->arg("vSpkNo");
  String volumeS = request->arg("volume");
  String ledS = request->arg("led");
  String randomSpeakS = request->arg("randomSpeak");
  String toneModeS = request->arg("toneMode");
  String muteS = request->arg("mute");
  String keyLockS = request->arg("keyLock");
  String timerS = request->arg("timer");
  String txS = request->arg("tx");
  wsHandleStartup(serverNameS,volumeS, ledS, toneModeS,muteS,
   keyLockS, vSpkNoS, randomSpeakS, timerS, txS );
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

