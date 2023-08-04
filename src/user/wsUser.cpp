// ---------------------------< wsUser.cpp >------------------------------------
#include "wsUser.h"

const String WSS1_HTML = "/wss1.html";
const String WSS2_HTML = "/wss2.html";
const String WSS3_HTML = "/wss3.html";
const String WSS4_HTML = "/wss4.html";
const String WSS5_HTML = "/wss5.html";
const String NAME_WSS1 = "Main";
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
  server.on("/icon", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/icon.gif", "image/gif"); });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "application/javascript"); });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // ###########################################################################
  server.on("/fsMode", HTTP_GET, [](AsyncWebServerRequest *request)
            { handle_fsMode(request);  serverSend(request); });

  // ###########################################################################
}

// extern void wait_SD();
extern bool isSPIFFS;
typedef struct
{
  String filename;
  String ftype;
  String fsize;
} fileinfo;
extern fileinfo Filenames[]; // Enough for most purposes!
extern String ConvBinUnits(int bytes, int resolution);
void Directory2();
void Directory3();
void printDirectory(File dir, int numTabs);


void handle_fsMode(AsyncWebServerRequest *request)
{
  webpage = "NG";
  isSPIFFS = true;

  String argS = request->arg("mode");
  Serial.println("test?mode = " + argS);

  if (argS == "SD")
  {
    webpage = "fsMode = SD";
    isSPIFFS = false;
  }
  else if (argS == "SPIFFS")
  {
    webpage = "fsMOde = SPIFFS";
    isSPIFFS = true;
  }
  else
    return;

  // Directory2();
  // Directory3();
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




void wait_SD()
{
  if (!isSPIFFS)
      delay(1);
}


void Directory2()
{
  int numfiles = 0; // Reset number of SPIFFS/SD files counter
  File root;

  if (isSPIFFS)
    root = SPIFFS.open("/");
  else
    root = SD.open("/");

  if (root)
  {
    wait_SD();
    root.rewindDirectory();

    // wait_SD();
    File file = root.openNextFile();
    webpage += "\n\n";
    while (file)
    { // Now get all the filenames, file types and sizes
      Filenames[numfiles].filename = (String(file.name()).startsWith("/") ? String(file.name()).substring(1) : file.name());
      Filenames[numfiles].ftype = (file.isDirectory() ? "Dir" : "File");
      Filenames[numfiles].fsize = ConvBinUnits(file.size(), 1);

      String flname = Filenames[numfiles].filename;
      String fltype = Filenames[numfiles].ftype;
      String flsize = Filenames[numfiles].fsize;

      webpage += flname + "\n";
      webpage += fltype + "\n";
      webpage += flsize + "\n\n";

      wait_SD();
      file = root.openNextFile();
      numfiles++;
    }
  }

  root.close();
}

void Directory3()
{
  int numfiles = 0; // Reset number of SPIFFS/SD files counter
  File root;

  if (isSPIFFS)
    root = SPIFFS.open("/", "r");
  else
    root = SD.open("/", "r");

  webpage = "";
  printDirectory(root, 0);
  root.close();
}

void printDirectory(File dir, int numTabs)
{
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
    {
      dir.rewindDirectory();
      break;
    }

    for (uint8_t i = 0; i < numTabs; i++)
    {
      webpage += "        ";
      Serial.print("        ");
    }

    webpage += entry.name();
    Serial.print(entry.name());

    if (entry.isDirectory())
    {
      // M5.Lcd.println("/");
      webpage += "/\n";
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    else
    {
      // M5.Lcd.println("\t\t");
      webpage += "                \n";
      Serial.println("                ");
    }
  }
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
  page += "<a href='/wss1'>" + NAME_WSS1 + "</a>";
  page += "<a href='/wss2'>" + NAME_WSS2 + "</a>";
  page += "<a href='/wss3'>" + NAME_WSS3 + "</a>";
  page += "<a href='/wss4'>" + NAME_WSS4 + "</a>";
  page += "<a href='/wss5'>" + NAME_WSS5 + "</a>";
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
