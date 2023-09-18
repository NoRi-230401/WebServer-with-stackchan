// ----------------------------<wsServer2.cpp>------------------------------------
// 2023-09-18 Add folder funciton for SD (chdir,mkdir,rmdir, etc)  by NoRi
// -------------------------------------------------------------------------------
#include "wsServer2.h"

extern const String FLS_NAME[];
const String FLS_NAME[] = {"SD", "SPIFFS"};
int isSPIFFS = 1;
int numDirs;
String SdPath = "/";
bool StartupErrors = false;

void serverSetup()
{
  setupMDNS();
  
  // ------------------- setup Handler -----------------
  setupApiHandler();
  setupUserHandler();
  serverSetup2();
  serverSetup1();
  // ----------------------------------------------------
  
  server.begin(); // Start the server
  if (!StartupErrors)
    Serial.println("** AsyncWebServer started successfully... **");
  else
    Serial.println("** AsyncWebServer : There were problems starting all services... **");
  
  FilesDirList(); // Update the file list  
}

void setupMDNS()
{
  if (!StartMDNSservice(SERVER_NAME.c_str()))
  {
    Serial.println("Error starting mDNS Service...");
    ;
    StartupErrors = true;
  }
  Serial.println("ServerName = " + SERVER_NAME);
}

void serverSetup2()
{
  // ############### Spiffs/Sd change  #############################
  server.on("/fileSystem", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("change file system SPIFFS <---> SD ...");
    handle_fileSystem(request); // file System change  SPIFFS - SD
    request->send(200, "text/html", webpage); });

  // ######################  ROOT_SD ################################
  server.on("/root_sd", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("change SdPath to Root ...");
    handle_root_sd(); 
    request->send(200, "text/html", webpage); });

  // ##################### CHDIR HANDLER ############################
  server.on("/chdir", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("chdir...");
    Select_Dir_For_Function("[CHDIR]", "chdirhandler");
    request->send(200, "text/html", webpage); });

  // ##################### MKDIR HANDLER ############################
  server.on("/mkdir", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("mkdir ...");
    Dir_Make();
    request->send(200, "text/html", webpage); });

  // ##################### RMDIR HANDLER #############################
  server.on("/rmdir", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("rmdir...");
    Select_Dir_For_Function("[RMDIR]", "rmdirhandler");
    request->send(200, "text/html", webpage); });
}

// #############################################################################################
void handle_fileSystem(AsyncWebServerRequest *request)
{
  String modeS;
  modeS = request->arg("mode");

  if (modeS != "")
  {
    if (modeS == "toggle")
      isSPIFFS ^= 1; // 反転

    else if (modeS == "SPIFFS")
      isSPIFFS = 1;

    else if (modeS == "SD")
    {
      isSPIFFS = 0;
    }

    if (!isSPIFFS)
    {
      SdPath = String("/");
    }
  }

  Serial.println("file System is " + FLS_NAME[isSPIFFS]);

  Home();
}

// #############################################################################################
void handle_root_sd()
{
  SdPath = String("/");
  // Serial.println("change SdPath to Root");
  Home();
}


// #############################################################################################
void Handle_chdir(String filename)
{ // chdri
  webpage = HTML_Header();
  if (!filename.startsWith("/"))
    filename = "/" + filename;

  if (SdPath != "/")
    filename = SdPath + filename;
  Serial.println("filename = " + filename);

  if (SD.exists(filename))
  { // success
    SdPath = filename;
    Serial.println("SdPath = " + SdPath);
    webpage += "<h3>Directory '" + SdPath + "' has been changed</h3>";
    webpage += "<a href='/dir'>[Enter]</a><br><br>";
  }
  else
  {
    webpage += "<h3>Directory [ " + filename + " ] is NOT changed successfully</h3>";
    webpage += "<a href='/dir'>[Enter]</a><br><br>";
  }
  webpage += HTML_Footer();
}

// #############################################################################################
void Handle_rmdir(String filename)
{ // rmdir
  webpage = HTML_Header();
  if (!filename.startsWith("/"))
    filename = "/" + filename;

  if (SdPath != "/")
    filename = SdPath + filename;
  Serial.println("filename = " + filename);

  if (SD.rmdir(filename))
  { // success
    webpage += "<h3>Directory '" + filename.substring(1) + "' has been removed</h3>";
    webpage += "<a href='/dir'>[Enter]</a><br><br>";
  }
  else
  {
    webpage += "<h3>Directory [ " + filename + " ] is NOT removed successfully</h3>";
    webpage += "<a href='/dir'>[Enter]</a><br><br>";
  }
  webpage += HTML_Footer();
}

// #############################################################################################
void Handle_mkdir(AsyncWebServerRequest *request)
{ // Dir Make

  webpage = HTML_Header();
  String filename = request->arg("filename");

  if (!filename.startsWith("/"))
    filename = "/" + filename;

  if (SdPath != "/")
    filename = SdPath + filename;
  Serial.println("filename = " + filename);

  if ( SD.mkdir(filename))
  { // success
    webpage += "<h3>Directory '" + filename + "' has been created</h3>";
    webpage += "<a href='/dir'>[Enter]</a><br><br>";
  }
  else
  {
    webpage += "<h3>Directory [ " + filename + " ] is NOT created successfully</h3>";
    webpage += "<a href='/dir'>[Enter]</a><br><br>";
  }
  webpage += HTML_Footer();
}

// #############################################################################################
void Dir_Make()
{
  SelectInputDirName("Make New Directory", "mkdirhandler", "filename");
}

// #############################################################################################
void SelectInputDirName(String Heading, String Command, String Arg_name)
{
  webpage = HTML_Header();
  webpage += "<h3>" + Heading + "</h3>";
  webpage += "<form  action='/" + Command + "'>";
  webpage += "DirName: <input type='text' name='" + Arg_name + "'><br><br>";
  webpage += "<input type='submit' value='Enter'>";
  webpage += "</form>";
  webpage += HTML_Footer();
}

// #############################################################################################
void Select_Dir_For_Function(String title, String function)
{
  String Fname1, Fname2;
  int index = 0;
  DirsList(); // Get a Dir list
  webpage = HTML_Header();
  webpage += "<h3>Select a Directory to " + title + " from this device</h3>";
  webpage += "<table class='center'>";
  webpage += "<tr> <th>Directory Name</th> <th>Directory Name</th> </tr>";

  while (index < numDirs)
  {
    Fname1 = Filenames[index].filename;
    Fname2 = Filenames[index + 1].filename;
    if (Fname1.startsWith("/"))
      Fname1 = Fname1.substring(1);
    if (Fname2.startsWith("/"))
      Fname2 = Fname2.substring(1);

    webpage += "<tr>";
    webpage += "<td style='width:25%'><button><a href='" + function + "~/" + Fname1 + "'>" + Fname1 + "</a></button></td>";

    if (index < numDirs - 1)
    {
      webpage += "<td style='width:25%'><button><a href='" + function + "~/" + Fname2 + "'>" + Fname2 + "</a></button></td>";
    }
    webpage += "</tr>";
    index = index + 2;
  }
  webpage += "</table>";
  webpage += HTML_Footer();
}

// #############################################################################################
//  File含まない。Dirのみの表示 for SD Only  .. by NoRi
void DirsList()
{
  numDirs = 0; // Reset number of dirs counter
  File root = SD.open(SdPath, "r");

  if (root)
  {
    root.rewindDirectory();
    File file = root.openNextFile();

    while (file)
    {
      String tmp_filename = (String(file.name()).startsWith("/") ? String(file.name()).substring(1) : file.name());

      if (!file.isDirectory() || tmp_filename == "System Volume Information")
      {
        file = root.openNextFile();
      }
      else
      {
        Filenames[numDirs].filename = tmp_filename;
        Filenames[numDirs].ftype = (file.isDirectory() ? "Dir" : "File");
        Filenames[numDirs].fsize = ConvBinUnits(file.size(), 1);
        file = root.openNextFile();
        numDirs++;
      }
    }
    root.close();
  }
}

// void checkWebReq(AsyncWebServerRequest *request)
// {
//   // List all parameters
//   int params = request->params();
//   for (int i = 0; i < params; i++)
//   {
//     AsyncWebParameter *p = request->getParam(i);
//     if (p->isFile())
//     { // p->isPost() is also true
//       Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
//     }
//     else if (p->isPost())
//     {
//       Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//     }
//     else
//     {
//       Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
//     }
//   }

//   // List all parameters (Compatibility)
//   int args = request->args();
//   for (int i = 0; i < args; i++)
//   {
//     Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
//   }
// }

// void handle_test(AsyncWebServerRequest *request)
// {
//   // List all collected headers (Compatibility)
//   int headers = request->headers();
//   int i;
//   for (i = 0; i < headers; i++)
//   {
//     Serial.printf("HEADER[%s]: %s\n", request->headerName(i).c_str(), request->header(i).c_str());
//   }

//   // List all parameters
//   int params = request->params();
//   for (int i = 0; i < params; i++)
//   {
//     AsyncWebParameter *p = request->getParam(i);
//     if (p->isFile())
//     { // p->isPost() is also true
//       Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
//     }
//     else if (p->isPost())
//     {
//       Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//     }
//     else
//     {
//       Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
//     }
//   }

//   // List all parameters (Compatibility)
//   int args = request->args();
//   for (int i = 0; i < args; i++)
//   {
//     Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
//   }

//   // tone(2);
//   webpage = "NG";
// }

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
