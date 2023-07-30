// ---------------------------< wsUser.cpp >------------------------------------
#include "wsUser.h"


const String U01_HTML_SPIFFS = "/u01.html";
const String U02_HTML_SPIFFS = "/u02.html";
const String U03_HTML_SPIFFS = "/u03.html";
const String U04_HTML_SPIFFS = "/u04.html";
const String U05_HTML_SPIFFS = "/u05.html";
const String NAME_U01 = "Main";
const String NAME_U02 = "Servo";
const String NAME_U03 = "Remote";
const String NAME_U04 = "u04.html";
const String NAME_U05 = "u05.html";

void setupUserHandler()
{
  // #########################################################################
  //  --- Html -----
  server.on("/u01", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_s01();  request->send(200, "text/html", webpage); });

  server.on("/u02", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_s02();  request->send(200, "text/html", webpage); });

  server.on("/u03", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_s03();  request->send(200, "text/html", webpage); });

  server.on("/u04", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_s04();  request->send(200, "text/html", webpage); });

  server.on("/u05", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/u05.html", String(), false, processor05); });
  
  // #########################################################################
  //  --- StyleSheet -----
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // style-u0x.css --- styleSheet -for user ----
  server.on("/style-u01.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style-u01.css", "text/css"); });

  server.on("/style-u02.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style-u02.css", "text/css"); });

  server.on("/style-u03.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style-u03.css", "text/css"); });
    
  server.on("/style-u04.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style-u04.css", "text/css"); });
  
  server.on("/style-u05.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style-u05.css", "text/css"); });
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

void handle_s01()
{
  htmlConv(U01_HTML_SPIFFS);
}

void handle_s02()
{
  htmlConv(U02_HTML_SPIFFS);
}

void handle_s03()
{
  htmlConv(U03_HTML_SPIFFS);
}

void handle_s04()
{
  htmlConv(U04_HTML_SPIFFS);
}

// #define DEBUG_INDEX_HTML
bool htmlConv(const String flname)
{
  // *************************************************************
  //   WEB Service :  SPIFFS直下に "flname"　を設置
  //  -----------------------------------------------------------
  //   "192.168.0.100"  を接続した実際のIPアドレスに変換
  const char *findStr = "192.168.0.100";
  // *************************************************************
  webpage = "";

  File fl = SPIFFS.open(flname.c_str(), "r");
  if (!fl)
  {
    fl.close();
    // webpage = "Error handleRoot : cannot open file in SPIFFS";
    Serial.println("Error htmlConv func : cannot open " + flname + " in SPIFFS");
    return false;
  }

  // *** Buffer確保 ******
  size_t sz = fl.size();
  // Serial.print(flname + " file size = ");
  // Serial.println(sz, DEC);
  Serial.println(flname + " :  file size = " + String(sz, DEC));

  char *buff;
  buff = (char *)malloc(sz + 1);
  if (!buff)
  {
    char msg[100];
    sprintf(msg, "ERROR:  Unable to malloc %d bytes for app\n", sz);
    webpage = String(msg);
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
  webpage.replace(findStr, (const char *)IP_ADDR.c_str());
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

// String processorS00(const String &var)
// {
//   return String();
// }

// String processor01(const String &var)
// {
//   return String();
// }

// String processor02(const String &var)
// {
//   return String();
// }

// String processor03(const String &var)
// {
//   return String();
// }

// String processor04(const String &var)
// {
//   return String();
// }



// #############################################################################################
void Home()
{
  webpage = HTML_Header();
  webpage += "<h3><pre>[ Home ] <b>" + SERVER_NAME + "  ip=" + IP_ADDR + "</b></pre></h3>";
  webpage += "<img src = 'icon' alt='icon'>";
  webpage += "<h4>- Files, Upload, Download, Stream , Delete , Rename File in SPIFFS -</h4>";
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
  page += "<title>StackChan</title>";
  page += "<base target='StackChanSub'>";
  
  page += "<style>";
  page += "div {text-align:center;}";
  page += "</style>";
  
  page += "</head>";

  page += "<body bgcolor='#ccff99' text='#2f4f4f' leftmargin='10' topmargin='0'>";
  page += "<basefont size='6'>";
  page += "<pre><b>";
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
  
  page += "<style>";
  page += "div {text-align:center;}";
  page += "</style>";
  
  page += "</head>";

  page += "<body bgcolor='#ffccff' text='white' leftmargin='10' topmargin='0'>";
  page += "<basefont size='6'>";
  page += "<pre><b>";
  return page;
}


// #############################################################################################
String HTML_Footer2()
{
  String page;
  // page += "<footer>";
  // page += "</footer>";
  page += "</b></pre><br><br>";
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
  page += "<meta charset='UTF-8'>";
  page += "<style>";
  page += "body {width:50em;margin-left:auto;margin-right:auto;font-family:Arial,Helvetica,sans-serif;font-size:14px;color:#2f4f4f;background-color:#fffacd;text-align:center;}";
  page += "footer {padding:0.08em;background-color:cyan;font-size:0.9em;}";
  page += "table {font-family:arial,sans-serif;border-collapse:collapse;width:70%;}"; // 70% of 75em!
  page += "table.center {margin-left:auto;margin-right:auto;}";
  page += "td, th {border:1px solid #dddddd;text-align:left;padding:8px;}";
  page += "tr:nth-child(even) {background-color:#dddddd;}";
  page += "h3 {color:green;font:1.4em;font-style:normal;text-align:center;}";
  page += "h4 {color:slateblue;font:0.8em;text-align:left;font-style:oblique;text-align:center;}";
  page += ".center {margin-left:auto;margin-right:auto;}";

  // TOPNAV
  page += ".topnav {overflow: hidden;background-color:lightPink;}";
  page += ".topnav a {float:center;color:blue;text-align:center;padding:0.6em 0.6em;text-decoration:none;font-size:1.1em;}";
  page += ".topnav a:hover {background-color:deepskyblue;color:white;}";
  page += ".topnav a.active {background-color:lightblue;color:blue;}";

  // TOPNAV2
  page += ".topnav2 {overflow: hidden;background-color:lightcyan;}";
  page += ".topnav2 a {float:center;color:blue;text-align:center;padding:0.6em 0.6em;text-decoration:none;font-size:1.2em;}";
  page += ".topnav2 a:hover {background-color:deepskyblue;color:white;}";
  page += ".topnav2 a.active {background-color:lightblue;color:blue;}";

  page += ".notfound {padding:0.8em;text-align:center;font-size:1.3em;}";
  page += ".left {text-align:left;}";
  page += ".medium {font-size:1.4em;padding:0;margin:0}";
  page += ".ps {font-size:0.9em;padding:0;margin:0}";
  page += ".sp {background-color:silver;white-space:nowrap;width:2%;}";
  page += "</style>";
  page += "</head>";
  page += "<body>";

  page += "<div class = 'topnav'>";
  page += "<a href='/dir'>Files</a>";
  page += "<a href='/upload'>Upload</a> ";
  page += "<a href='/download'>Download</a>";
  page += "<a href='/stream'>Stream</a>";
  page += "<a href='/delete'>Delete</a>";
  page += "<a href='/rename'>Rename</a>";
  // page += "<a href='/format'>Format FS</a>";
  page += "</div>";

  page += "<div class = 'topnav2'>";
  page += "<a href='/'>Home</a>";
  page += "<a href='/u01'>" + NAME_U01 + "</a>";
  page += "<a href='/u02'>" + NAME_U02 + "</a>";
  page += "<a href='/u03'>" + NAME_U03 + "</a>";
  page += "<a href='/u04'>" + NAME_U04 + "</a>";
  page += "<a href='/u05'>" + NAME_U05 + "</a>";
  page += "<a href='/system'>Status</a><br>";
  page += "</div>";

  return page;
}
// #############################################################################################
String HTML_Footer()
{
  String page;
  page += "<br><footer>";
  page += "<p class='medium'>" + SERVER_NAME + " is a super-kawaii robot.</p>";
  page += "<p class='ps'><i> " + WS_VERSION + "</i></p>";
  page += "</footer>";
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

  char msg[100];

  String arg_str = request->arg("mode");
  sprintf(msg, "test?mode=%s", arg_str.c_str());
  Serial.println(msg);

  if (arg_str == "SD")
    Serial.println("SD mode");
  else if (arg_str == "SPIFFS")
    Serial.println("SPIFFS mode");
  else
    return;

  webpage = "OK";
}
