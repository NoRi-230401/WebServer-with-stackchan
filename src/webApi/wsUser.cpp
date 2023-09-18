// ---------------------------< wsUser.cpp >------------------------------------
#include "wsUser.h"

const String WSS1_HTML = "/wss1.html";
const String WSS2_HTML = "/wss2.html";
const String WSS3_HTML = "/wss3.html";
const String WSS4_HTML = "/wss4.html";
const String WSS5_HTML = "/wss5.html";
const String NAME_WSS1 = "Setting";
const String NAME_WSS2 = "Servo";
const String NAME_WSS3 = "Remote";
const String NAME_WSS4 = "wss4";
const String NAME_WSS5 = "wss5";

void setupUserHandler()
{
  // #########################################################################
  //  --- Html -----
  server.on("/wss1", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_wss1();  request->send(200, "text/html", webpage); });

  server.on("/wss2", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_wss2();  request->send(200, "text/html", webpage); });

  server.on("/wss3", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_wss3();  request->send(200, "text/html", webpage); });

  server.on("/wss4", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_wss4();  request->send(200, "text/html", webpage); });

  server.on("/wss5", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/wss5.html", String(), false, processor05); });

  // #########################################################################
  server.on("/icon.gif", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/icon.gif", "image/gif"); });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "application/javascript"); });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // server.on("/smart-style.css", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send(SPIFFS, "/smart-style.css", "text/css"); });

  // server.on("/pc-style.css", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send(SPIFFS, "/pc-style.css", "text/css"); });

  // ###########################################################################
}

void serverSend(AsyncWebServerRequest *request)
{
  if (webpage == "NG")
  {
    webpage = HTML_Header2Ng() + webpage + HTML_Footer2();
    request->send(400, "text/html", webpage);
  }
  else
  {
    String tmpPage;
    tmpPage = HTML_Header2() + webpage + HTML_Footer2();
    webpage = tmpPage;
    request->send(200, "text/html", webpage);
  }
}

void handle_wss1()
{
  htmlConv(WSS1_HTML);
}

void handle_wss2()
{
  htmlConv(WSS2_HTML);
}

void handle_wss3()
{
  htmlConv(WSS3_HTML);
}

void handle_wss4()
{
  htmlConv(WSS4_HTML);
}

void handle_wss5()
{
  htmlConv(WSS5_HTML);
}

// #define DEBUG_INDEX_HTML
bool htmlConv(const String flname)
{
  // *************************************************************
  // htmlファイル中の  "http://stackchan/"  を実際のIPアドレスに変換
  const char *findStr = "http://stackchan/";
  // *************************************************************
  webpage = "";

  File fl = SPIFFS.open(flname.c_str(), "r");
  if (!fl)
  {
    fl.close();
    String msg = "Error handleRoot : cannot open " + flname;
    webpage = msg;
    Serial.println(msg);
    return false;
  }

  // *** Buffer確保 ******
  size_t sz = fl.size();
  Serial.println(flname + " :  file size = " + String(sz, DEC));

  char *buff;
  buff = (char *)malloc(sz + 1);
  if (!buff)
  {
    String msg = "ERROR:  Unable to malloc " + String(sz, DEC) + " bytes for app";
    webpage = String(msg);
    Serial.println(msg);
    fl.close();
    return false;
  }

  fl.read((uint8_t *)buff, sz);
  buff[sz] = 0;
  fl.close();

  webpage = String(buff);
  free(buff);

#ifndef DEBUG_INDEX_HTML
  // ** 本体のIP_ADDRに変換 **
  String replacedStr = "http://" + IP_ADDR + "/";
  webpage.replace(findStr, (const char *)replacedStr.c_str());
#endif
  return true;
}

String processor05(const String &var)
{
  // ********************* SAMPLE : 本体の内容をWEBに表示　********************
  // Htmlファイル内の %IP_ADDR% , %SERVER_NAME%, %VOLUME_VALUE% に指定された箇所
  // を書き換える。本体の状態をWEB上で表示することができます。
  // **************************************************************************
  Serial.println(var);
  if (var == "IP_ADDR")
  {
    Serial.println(IP_ADDR);
    return IP_ADDR;
  }
  else if (var == "SERVER_NAME")
  {
    Serial.println(SERVER_NAME);
    return SERVER_NAME;
  }
  if (var == "VOLUME_VALUE")
  {
    int vol = (int)VOLUME_VALUE;
    String vol_str = String(vol, DEC);
    return vol_str;
  }

  return String();
}

// #############################################################################################
void Home()
{
  webpage = HTML_Header();
  webpage += "<br>";
  webpage += "<img src='icon.gif' alt='icon'>";
  webpage += "<h3>[&nbsp;Home&nbsp;]　" + SERVER_NAME + "　IP=" + IP_ADDR + "</h3>";
  // webpage += "<br>";
  webpage += HTML_Footer();
}

// #############################################################################################
String HTML_Header2()
{
  String page;
  page = "<!DOCTYPE html>";
  page += "<html lang = 'ja'>";
  page += "<head>";
  page += "<meta charset='UTF-8'>";
  page += "<meta name='viewport' content='width=device-width,initial-scale=1.0'>";
  page += "<title>StackChan</title>";
  page += "<base target='StackChanSub'>";
  page += "<style>";
  page += "html {font-size: 62.5%;}";
  page += "body {font-size:1.6rem;background-color:#fffde7;text-align:left;}";
  page += "div {font-size:1.6rem;text-align:center;}";
  page += "@media screen and (max-width: 480px) {body{font-size:1.4rem;} img{width:100%;height:auto;}}";
  page += "</style>";
  page += "</head>";
  page += "<body><pre>";
  return page;
}

// #############################################################################################
String HTML_Header2Ng()
{
  String page;
  page = "<!DOCTYPE html>";
  page += "<html lang = 'ja'>";
  page += "<head>";
  page += "<meta charset='UTF-8'>";
  page += "<title>StackChan</title>";
  page += "<base target='StackChanSub'>";
  page += "<meta name='viewport' content='width=device-width,initial-scale=1.0'>";
  page += "<style>";
  page += "html {font-size: 62.5%;}";
  page += "body {font-size:1.6rem;background-color:#ffccff;text-align:left;}";
  page += "div {font-size:1.6rem;text-align:center;}";
  page += "@media screen and (max-width: 480px) {body{font-size:1.4rem;} img{width:100%;height:auto;}}";
  page += "</style>";
  page += "</head>";
  page += "<body><pre>";
  return page;
}

// #############################################################################################
String HTML_Footer2()
{
  String page;
  // page += "<footer>";
  // page += "</footer>";
  page += "</pre><br><br>";
  page += "<div><form><input type='button' name='button' value='このウィンドウを閉じる' onclick='window.close();'></form><div>";
  page += "</body></html>";
  return page;
}

// #############################################################################################
String HTML_Header()
{
  String page;
  page = "<!DOCTYPE html>";
  page += "<html lang = 'ja'>";
  page += "<head>";
  page += "<title>Home</title>";
  page += "<base target='_self'>";
  page += "<meta charset='UTF-8'>";
  page += "<meta name='viewport' content='width=device-width,initial-scale=1.0'>";
  page += "<style>";
  page += "@media screen and (max-width: 480px) {img{width:100%;height:auto;}}";
  page += "html {font-size: 62.5%;}";
  page += "body {width:100%;margin-left:auto;margin-right:auto;font-family:Arial,Helvetica,sans-serif;font-size:1.4rem;color:#2f4f4f;background-color:#fffacd;text-align:center;}";
  page += "footer {padding:1.0rem;background-color:cyan;font-size:1.4rem;}";
  page += "table {font-family:arial,sans-serif;border-collapse:collapse;width:80%;}";
  page += "table.center {margin-left:auto;margin-right:auto;}";
  page += "td, th {border:1px solid #dddddd;text-align:left;padding:0.8rem;}";
  page += "tr:nth-child(even) {background-color:#dddddd;}";
  page += "h3 {color:#6ecf12;font-size:1.8rem;font-style:normal;text-align:center;}";
  page += "h4 {color:slateblue;font-size:1.5rem;text-align:left;font-style:oblique;text-align:center;}";
  page += ".center {margin-left:auto;margin-right:auto;}";

  // TOPNAV
  page += ".topnav {overflow: visible;background-color:lightPink;}";
  page += ".topnav a {float:center;color:blue;text-align:center;padding:1.0rem 1.0rem;text-decoration:none;font-size:1.5rem;}";
  page += ".topnav a:hover {background-color:deepskyblue;color:white;}";
  page += ".topnav a.active {background-color:lightblue;color:blue;}";

  // TOPNAV2
  page += ".topnav2 {overflow: visible;background-color:lightcyan;}";
  page += ".topnav2 a {float:center;color:blue;text-align:center;padding:1.0rem 1.0rem;text-decoration:none;font-size:1.5rem;}";
  page += ".topnav2 a:hover {background-color:deepskyblue;color:white;}";
  page += ".topnav2 a.active {background-color:lightblue;color:blue;}";

  page += ".notfound {padding:0.8rem;text-align:center;font-size:1.3rem;}";
  page += ".left {text-align:left;}";
  page += ".medium {font-size:1.9rem;padding:0;margin:0}";
  page += ".ps {font-size:1.4rem;padding:0;margin:0}";
  page += ".sp {background-color:silver;white-space:nowrap;width:2%;}";
  page += "</style>";
  page += "</head>";
  page += "<body>";

  page += "<div class = 'topnav'>";
  page += "<a href='/dir'>Dir</a> ";
  page += "<a href='/upload'>Upload</a> ";
  page += "<a href='/download'>Download</a> ";
  page += "<a href='/stream'>Stream</a> ";
  page += "<a href='/delete'>Delete</a> ";
  page += "<a href='/rename'>Rename</a> ";
  page += "<a href='/fileSystem?mode=toggle'>Spiffs/Sd</a>:" + FLS_NAME[isSPIFFS];
  page += "</div>";

  if (!isSPIFFS)
  { // directory func to SD --- by NoRi -----
    page += "<div class = 'topnav'>";
    page += "<a href='/root_sd'>Root</a>CurrentDir = " + SdPath;
    page += "<a href='/chdir'>Chdir</a> ";
    page += "<a href='/mkdir'>Mkdir</a> ";
    page += "<a href='/rmdir'>Rmdir</a>";
    page += "</div>";
    // page += "<br>";
  }

  page += "<br>";
  page += "<div class = 'topnav2'>";
  page += "<a href='/' target='Home'>Home</a>";
  page += "<a href='/wss1' target='" + NAME_WSS1 + "'>" + NAME_WSS1 + "</a> ";
  page += "<a href='/wss2' target='" + NAME_WSS2 + "'>" + NAME_WSS2 + "</a> ";
  page += "<a href='/wss3' target='" + NAME_WSS3 + "'>" + NAME_WSS3 + "</a> ";
  // page += "<a href='/wss4'>" + NAME_WSS4 + "</a> ";
  // page += "<a href='/wss5'>" + NAME_WSS5 + "</a> ";
  page += "<a href='/system'>Status</a>";
  page += "<a href='https://nori.rdy.jp/wss/' target='WSS-Support'>Support</a>";
  page += "</div>";

  return page;
}
// #############################################################################################
String HTML_Footer()
{
  String page;
  page += "<br>";
  page += "<footer>";
  page += "<p class='medium'>" + SERVER_NAME + " is a super-kawaii robot.</p>";
  page += "<p class='ps'><i> " + WS_VERSION + "</i></p>";
  page += "</footer>";
  page += "<br>";
  page += "</body>";
  page += "</html>";
  return page;
}

void checkWebReq(AsyncWebServerRequest *request)
{
  // List all parameters
  int params = request->params();
  for (int i = 0; i < params; i++)
  {
    AsyncWebParameter *p = request->getParam(i);
    if (p->isFile())
    { // p->isPost() is also true
      Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
    }
    else if (p->isPost())
    {
      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
    else
    {
      Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
  }

  // List all parameters (Compatibility)
  int args = request->args();
  for (int i = 0; i < args; i++)
  {
    Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
  }
}

void handle_test(AsyncWebServerRequest *request)
{
  // List all collected headers (Compatibility)
  int headers = request->headers();
  int i;
  for (i = 0; i < headers; i++)
  {
    Serial.printf("HEADER[%s]: %s\n", request->headerName(i).c_str(), request->header(i).c_str());
  }

  // List all parameters
  int params = request->params();
  for (int i = 0; i < params; i++)
  {
    AsyncWebParameter *p = request->getParam(i);
    if (p->isFile())
    { // p->isPost() is also true
      Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
    }
    else if (p->isPost())
    {
      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
    else
    {
      Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
  }

  // List all parameters (Compatibility)
  int args = request->args();
  for (int i = 0; i < args; i++)
  {
    Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
  }

  // tone(2);
  webpage = "NG";
}

// void Directory3()
// {
//   int numfiles = 0; // Reset number of SPIFFS/SD files counter
//   File root;

//   if (isSPIFFS)
//     root = SPIFFS.open("/", "r");
//   else
//     root = SD.open("/", "r");

//   webpage = "";
//   printDirectory(root, 0);
//   root.close();
// }

// void printDirectory(File dir, int numTabs)
// {
//   while (true)
//   {
//     File entry = dir.openNextFile();
//     if (!entry)
//     {
//       dir.rewindDirectory();
//       break;
//     }

//     for (uint8_t i = 0; i < numTabs; i++)
//     {
//       webpage += "        ";
//       Serial.print("        ");
//     }

//     webpage += entry.name();
//     Serial.print(entry.name());

//     if (entry.isDirectory())
//     {
//       // M5.Lcd.println("/");
//       webpage += "/\n";
//       Serial.println("/");
//       printDirectory(entry, numTabs + 1);
//     }
//     else
//     {
//       // M5.Lcd.println("\t\t");
//       webpage += "                \n";
//       Serial.println("                ");
//     }
//   }
// }
