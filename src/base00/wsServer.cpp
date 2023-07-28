// ----------------------------<wsServer.cpp>------------------------------------
// Extended from
//     ESPAsynch_Server_v1.1 by David Bird 2022
// ------------------------------------------------------------
//  SDにもFileServer機能を対応できるように変更していますが、
//  SDでは、動作が安定しないので SPIFFSだけで使用した方がよい。
//  2023-06-27 by NoRi
// ------------------------------------------------------------
/*
  This software, the ideas and concepts is Copyright (c) David Bird 2022
  All rights to this software are reserved.
  It is prohibited to redistribute or reproduce of any part or all of the software contents in any form other than the following:
  1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
  2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
  3. You may not, except with my express written permission, distribute or commercially exploit the content.
  4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.
  5. You MUST include all of this copyright and permission notice ('as annotated') and this shall be included in all copies or substantial portions of the software
     and where the software use is visible to an end-user.
  6. *** DONT USE THE SOFTWARE IF YOU DONT LIKE THE LICNCE CONDITIONS ***
  THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT.
  FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
  A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OR
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  See more at http://dsbird.org.uk
*/
#include "wsServer.h"

String SERVER_NAME = "stackchan";
AsyncWebServer server(80);
bool isSPIFFS = true;

typedef struct
{
  String filename;
  String ftype;
  String fsize;
} fileinfo;

String webpage, MessageLine;
fileinfo Filenames[200]; // Enough for most purposes!
bool StartupErrors = false;
int start, downloadtime = 1, uploadtime = 1, downloadsize, uploadsize, downloadrate, uploadrate, numfiles;
// float Temperature = 21.34; // for example new page, amend in a sensor function if required
// String Name = "Dave";

void serverSetup()
{
  if (!StartMDNSservice(SERVER_NAME.c_str()))
  {
    Serial.println("Error starting mDNS Service...");
    ;
    StartupErrors = true;
  }
  Serial.println("ServerName = " + SERVER_NAME);

  // ***** M5StackConfig() で、SPIFFSの初期化を行うので削除 **********
  // if (!SPIFFS.begin(true))
  // {
  //   Serial.println("Error preparing Filing System...");
  //   StartupErrors = true;
  // }
  // ***************************************************************
  isSPIFFS = true;

  // ##################### ApiHandler/ UserHandler ##################
  setupApiHandler();
  setupUserHandler();

  // ##################### HOMEPAGE HANDLER ########################
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Home Page...");
    Home(); // Build webpage ready for display
    request->send(200, "text/html", webpage); });
  // ##################### DOWNLOAD HANDLER ##########################
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Downloading file...");
    Select_File_For_Function("[DOWNLOAD]", "downloadhandler"); // Build webpage ready for display
    request->send(200, "text/html", webpage); });

  // ##################### UPLOAD HANDLERS ###########################
  server.on("/upload", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Uploading file...");
    UploadFileSelect(); // Build webpage ready for display
    request->send(200, "text/html", webpage); });

  // Set handler for '/handleupload'
  server.on(
      "/handleupload", HTTP_POST, [](AsyncWebServerRequest *request) {},
      [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
         size_t len, bool final)
      {
        handleFileUpload(request, filename, index, data, len, final);
      });

  // ##################### STREAM HANDLER ############################
  server.on("/stream", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Streaming file...");
    Select_File_For_Function("[STREAM]", "streamhandler"); // Build webpage ready for display
    request->send(200, "text/html", webpage); });

  // ##################### RENAME HANDLER ############################
  server.on("/rename", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Renaming file...");
    File_Rename(); // Build webpage ready for display
    request->send(200, "text/html", webpage); });

  // ##################### DIR HANDLER ###############################
  server.on("/dir", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("File Directory...");
    Dir(request); // Build webpage ready for display
    request->send(200, "text/html", webpage); });

  // ##################### DELETE HANDLER ############################
  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Deleting file...");
    Select_File_For_Function("[DELETE]", "deletehandler"); // Build webpage ready for display
    request->send(200, "text/html", webpage); });

  // ##################### FORMAT HANDLER ############################
  server.on("/format", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Request to Format File System...");
    Format(); // Build webpage ready for display
    request->send(200, "text/html", webpage); });

  // Set handler for '/handleformat'
  server.on("/handleformat", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if(isSPIFFS)
    {
    Serial.println("Processing Format Request of File System...");
    if (request->getParam("format")->value() == "YES") {
      Serial.print("Starting to Format Filing System...");
      SPIFFS.end();
      // *** SD は、format できない **
      bool formatted = SPIFFS.format();
      if (formatted) {
        Serial.println(" Successful Filing System Format...");
      }
      else         {
        Serial.println(" Formatting Failed...");
      }
    }
    }
    request->redirect("/dir"); });

  // ##################### SYSTEM HANDLER ############################
  server.on("/system", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Display_System_Info(); // Build webpage ready for display
    request->send(200, "text/html", webpage); });

  // ##################### IMAGE HANDLER #############################
  server.on("/icon", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/icon.gif", "image/gif"); });
  // { request->send(SPIFFS, "/stack-chan.jpeg", "image/jpeg"); });

  // ##################### NOT FOUND HANDLER #########################
  server.onNotFound(notFound);

  // #################################################################
  server.begin(); // Start the server
  if (!StartupErrors)
    Serial.println("** AsyncWebServer started successfully... **");
  else
    Serial.println("** AsyncWebServer : There were problems starting all services... **");
  Directory(); // Update the file list
}


void wait_SD()
{
  if (!isSPIFFS)
    delay(20);
}

// #############################################################################################
void Dir(AsyncWebServerRequest *request)
{
  String Fname1, Fname2;
  int index = 0;
  Directory(); // Get a list of the current files on the FS
  webpage = HTML_Header();
  webpage += "<h3>Filing System Content</h3><br>";
  if (numfiles > 0)
  {
    webpage += "<table class='center'>";
    webpage += "<tr><th>Type</th><th>File Name</th><th>File Size</th><th class='sp'></th><th>Type</th><th>File Name</th><th>File Size</th></tr>";
    while (index < numfiles)
    {
      Fname1 = Filenames[index].filename;
      Fname2 = Filenames[index + 1].filename;
      webpage += "<tr>";
      webpage += "<td style = 'width:5%'>" + Filenames[index].ftype + "</td><td style = 'width:25%'>" + Fname1 + "</td><td style = 'width:10%'>" + Filenames[index].fsize + "</td>";
      webpage += "<td class='sp'></td>";
      if (index < numfiles - 1)
      {
        webpage += "<td style = 'width:5%'>" + Filenames[index + 1].ftype + "</td><td style = 'width:25%'>" + Fname2 + "</td><td style = 'width:10%'>" + Filenames[index + 1].fsize + "</td>";
      }
      webpage += "</tr>";
      index = index + 2;
    }
    webpage += "</table>";
    webpage += "<p style='background-color:yellow;'><b>" + MessageLine + "</b></p>";
    MessageLine = "";
  }
  else
  {
    webpage += "<h2>No Files Found</h2>";
  }
  webpage += HTML_Footer();
  request->send(200, "text/html", webpage);
}
// #############################################################################################
void Directory()
{
  numfiles = 0; // Reset number of SPIFFS/SD files counter
  File root;

  if (isSPIFFS)
    root = SPIFFS.open("/");
  else
    root = SD.open("/");

  if (root)
  {
    wait_SD();
    root.rewindDirectory();

    wait_SD();
    File file = root.openNextFile();

    while (file)
    { // Now get all the filenames, file types and sizes
      Filenames[numfiles].filename = (String(file.name()).startsWith("/") ? String(file.name()).substring(1) : file.name());
      Filenames[numfiles].ftype = (file.isDirectory() ? "Dir" : "File");
      Filenames[numfiles].fsize = ConvBinUnits(file.size(), 1);
      wait_SD();
      file = root.openNextFile();
      numfiles++;
    }
    root.close();
  }
}

// #############################################################################################
void UploadFileSelect()
{
  webpage = HTML_Header();
  webpage += "<h3>Select a File to [UPLOAD] to this device</h3>";
  webpage += "<form method = 'POST' action = '/handleupload' enctype='multipart/form-data'>";
  webpage += "<input type='file' name='filename'><br><br>";
  webpage += "<input type='submit' value='Upload'>";
  webpage += "</form>";
  webpage += HTML_Footer();
}
// #############################################################################################
void Format()
{
  webpage = HTML_Header();
  webpage += "<h3>***  Format Filing System on this device ***</h3>";
  webpage += "<form action='/handleformat'>";
  webpage += "<input type='radio' id='YES' name='format' value = 'YES'><label for='YES'>YES</label><br><br>";
  webpage += "<input type='radio' id='NO'  name='format' value = 'NO' checked><label for='NO'>NO</label><br><br>";
  webpage += "<input type='submit' value='Format?'>";
  webpage += "</form>";
  webpage += HTML_Footer();
}
// #############################################################################################
void handleFileUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    String file = filename;
    if (!filename.startsWith("/"))
      file = "/" + filename;

    if (isSPIFFS)
    {
      request->_tempFile = SPIFFS.open(file, "w");
    }
    else
    {
      request->_tempFile = SD.open(file, "w");
    }

    wait_SD();
    if (!request->_tempFile)
      Serial.println("Error creating file for upload...");
    start = millis();
  }

  if (request->_tempFile)
  {
    if (len)
    {
      request->_tempFile.write(data, len); // Chunked data
      Serial.println("Transferred : " + String(len) + " Bytes");
    }

    if (final)
    {
      wait_SD();
      uploadsize = request->_tempFile.size();
      request->_tempFile.close();
      uploadtime = millis() - start;
      request->redirect("/dir");
    }
  }
}
// #############################################################################################
void File_Stream()
{
  SelectInput("Select a File to Stream", "handlestream", "filename");
}
// #############################################################################################
void File_Delete()
{
  SelectInput("Select a File to Delete", "handledelete", "filename");
}
// #############################################################################################
void Handle_File_Delete(String filename)
{ // Delete the file
  webpage = HTML_Header();
  if (!filename.startsWith("/"))
    filename = "/" + filename;
  File dataFile;
  if (isSPIFFS)
    dataFile = SPIFFS.open(filename, "r"); // Now read SPIFFS to see if file exists
  else
    dataFile = SD.open(filename, "r"); // Now read SD to see if file exists

  wait_SD();
  if (dataFile)
  { // It does so delete it
    if (isSPIFFS)
      SPIFFS.remove(filename);
    else
      SD.remove(filename);

    webpage += "<h3>File '" + filename.substring(1) + "' has been deleted</h3>";
    webpage += "<a href='/dir'>[Enter]</a><br><br>";
  }
  else
  {
    webpage += "<h3>File [ " + filename + " ] does not exist</h3>";
    webpage += "<a href='/dir'>[Enter]</a><br><br>";
  }
  webpage += HTML_Footer();
}
// #############################################################################################
void File_Rename()
{ // Rename the file
  Directory();
  webpage = HTML_Header();
  webpage += "<h3>Select a File to [RENAME] on this device</h3>";
  webpage += "<FORM action='/renamehandler'>";
  webpage += "<table class='center'>";
  webpage += "<tr><th>File name</th><th>New Filename</th><th>Select</th></tr>";
  int index = 0;
  while (index < numfiles)
  {
    webpage += "<tr><td><input type='text' name='oldfile' style='color:blue;' value = '" + Filenames[index].filename + "' readonly></td>";
    webpage += "<td><input type='text' name='newfile'></td><td><input type='radio' name='choice'></tr>";
    index++;
  }
  webpage += "</table><br>";
  webpage += "<input type='submit' value='Enter'>";
  webpage += "</form>";
  webpage += HTML_Footer();
}
// #############################################################################################
void Handle_File_Rename(AsyncWebServerRequest *request, String filename, int Args)
{ // Rename the file
  String newfilename;
  // int Args = request->args();
  webpage = HTML_Header();
  for (int i = 0; i < Args; i++)
  {
    if (request->arg(i) != "" && request->arg(i + 1) == "on")
    {
      filename = request->arg(i - 1);
      newfilename = request->arg(i);
    }
  }
  if (!filename.startsWith("/"))
    filename = "/" + filename;
  if (!newfilename.startsWith("/"))
    newfilename = "/" + newfilename;

  File CurrentFile;

  if (isSPIFFS)
    CurrentFile = SPIFFS.open(filename, "r"); // Now read SPIFFS to see if file exists
  else
    CurrentFile = SD.open(filename, "r"); // Now read SD to see if file exists

  if (CurrentFile && filename != "/" && newfilename != "/" && (filename != newfilename))
  { // It does so rename it, ignore if no entry made, or Newfile name exists already
    wait_SD();

    if (isSPIFFS && SPIFFS.rename(filename, newfilename))
    {
      filename = filename.substring(1);
      newfilename = newfilename.substring(1);
      webpage += "<h3>File '" + filename + "' has been renamed to '" + newfilename + "'</h3>";
      webpage += "<a href='/dir'>[Enter]</a><br><br>";
    }

    if (!isSPIFFS && SD.rename(filename, newfilename))
    {
      filename = filename.substring(1);
      newfilename = newfilename.substring(1);
      webpage += "<h3>File '" + filename + "' has been renamed to '" + newfilename + "'</h3>";
      webpage += "<a href='/dir'>[Enter]</a><br><br>";
    }
  }
  else
  {
    if (filename == "/" && newfilename == "/")
      webpage += "<h3>File was not renamed</h3>";
    else
      webpage += "<h3>New filename exists, cannot rename</h3>";
    webpage += "<a href='/rename'>[Enter]</a><br><br>";
  }
  CurrentFile.close();
  webpage += HTML_Footer();
}

// String processor(const String &var)
// {
//   if (var == "HELLO_FROM_TEMPLATE")
//     return F("Hello world!");
//   return String();
// }

// #############################################################################################
//  Not found handler is also the handler for 'delete', 'download' and 'stream' functions
void notFound(AsyncWebServerRequest *request)
{ // Process selected file types
  String filename;
  if (request->url().startsWith("/downloadhandler") ||
      request->url().startsWith("/streamhandler") ||
      request->url().startsWith("/deletehandler") ||
      request->url().startsWith("/renamehandler"))
  {
    // Now get the filename and handle the request for 'delete' or 'download' or 'stream' functions
    if (!request->url().startsWith("/renamehandler"))
      filename = request->url().substring(request->url().indexOf("~/") + 1);
    start = millis();
    if (request->url().startsWith("/downloadhandler"))
    {
      Serial.println("Download handler started...");
      MessageLine = "";

      File file;
      if (isSPIFFS)
        file = SPIFFS.open(filename, "r");
      else
        file = SD.open(filename, "r");

      String contentType = getContentType("download");
      AsyncWebServerResponse *response = request->beginResponse(contentType, file.size(), [file](uint8_t *buffer, size_t maxLen, size_t total) mutable -> size_t
                                                                { return file.read(buffer, maxLen); });
      response->addHeader("Server", "ESP Async Web Server");
      request->send(response);
      downloadtime = millis() - start;
      downloadsize = GetFileSize(filename);
      request->redirect("/dir");
    }
    if (request->url().startsWith("/streamhandler"))
    {
      Serial.println("Stream handler started...");
      String ContentType = getContentType(filename);
      AsyncWebServerResponse *response;
      if (isSPIFFS)
        response = request->beginResponse(SPIFFS, filename, ContentType);
      else
        response = request->beginResponse(SD, filename, ContentType);

      request->send(response);
      downloadsize = GetFileSize(filename);
      downloadtime = millis() - start;
      request->redirect("/dir");
    }
    if (request->url().startsWith("/deletehandler"))
    {
      Serial.println("Delete handler started...");
      Handle_File_Delete(filename); // Build webpage ready for display
      request->send(200, "text/html", webpage);
    }
    if (request->url().startsWith("/renamehandler"))
    {
      Handle_File_Rename(request, filename, request->args()); // Build webpage ready for display
      request->send(200, "text/html", webpage);
    }
  }
  else
  {
    Page_Not_Found();
    request->send(200, "text/html", webpage);
  }
}
// #############################################################################################
void Handle_File_Download()
{
  String filename = "";
  int index = 0;
  Directory(); // Get a list of files on the SPI
  webpage = HTML_Header();
  webpage += "<h3>Select a File to Download</h3>";
  webpage += "<table>";
  webpage += "<tr><th>File Name</th><th>File Size</th></tr>";
  while (index < numfiles)
  {
    webpage += "<tr><td><a href='" + Filenames[index].filename + "'></a><td>" + Filenames[index].fsize + "</td></tr>";
    index++;
  }
  webpage += "</table>";
  webpage += "<p>" + MessageLine + "</p>";
  webpage += HTML_Footer();
}
// #############################################################################################
String getContentType(String filenametype)
{ // Tell the browser what file type is being sent
  if (filenametype == "download")
  {
    return "application/octet-stream";
  }
  else if (filenametype.endsWith(".txt"))
  {
    return "text/plain";
  }
  else if (filenametype.endsWith(".htm"))
  {
    return "text/html";
  }
  else if (filenametype.endsWith(".html"))
  {
    return "text/html";
  }
  else if (filenametype.endsWith(".css"))
  {
    return "text/css";
  }
  else if (filenametype.endsWith(".js"))
  {
    return "application/javascript";
  }
  else if (filenametype.endsWith(".png"))
  {
    return "image/png";
  }
  else if (filenametype.endsWith(".gif"))
  {
    return "image/gif";
  }
  else if (filenametype.endsWith(".jpg"))
  {
    return "image/jpeg";
  }
  else if (filenametype.endsWith(".ico"))
  {
    return "image/x-icon";
  }
  else if (filenametype.endsWith(".xml"))
  {
    return "text/xml";
  }
  else if (filenametype.endsWith(".pdf"))
  {
    return "application/x-pdf";
  }
  else if (filenametype.endsWith(".zip"))
  {
    return "application/x-zip";
  }
  else if (filenametype.endsWith(".gz"))
  {
    return "application/x-gzip";
  }
  // ----- Add by NoRi 2023-06-26 ---------------------------------
  else if (filenametype.endsWith(".csv"))
  {
    return "text/csv";
  }
  else if (filenametype.endsWith(".json"))
  {
    return "application/json";
  }
  else if (filenametype.endsWith(".mp3"))
  {
    return "audio/mpeg";
  }
  else if (filenametype.endsWith(".mp4"))
  {
    return "video/mp4";
  }

  return "text/plain";
}
// #############################################################################################
void Select_File_For_Function(String title, String function)
{
  String Fname1, Fname2;
  int index = 0;
  Directory(); // Get a list of files on the SPI
  webpage = HTML_Header();
  webpage += "<h3>Select a File to " + title + " from this device</h3>";
  webpage += "<table class='center'>";
  webpage += "<tr><th>File Name</th><th>File Size</th><th class='sp'></th><th>File Name</th><th>File Size</th></tr>";
  while (index < numfiles)
  {
    Fname1 = Filenames[index].filename;
    Fname2 = Filenames[index + 1].filename;
    if (Fname1.startsWith("/"))
      Fname1 = Fname1.substring(1);
    if (Fname2.startsWith("/"))
      Fname1 = Fname2.substring(1);
    webpage += "<tr>";
    webpage += "<td style='width:25%'><button><a href='" + function + "~/" + Fname1 + "'>" + Fname1 + "</a></button></td><td style = 'width:10%'>" + Filenames[index].fsize + "</td>";
    webpage += "<td class='sp'></td>";
    if (index < numfiles - 1)
    {
      webpage += "<td style='width:25%'><button><a href='" + function + "~/" + Fname2 + "'>" + Fname2 + "</a></button></td><td style = 'width:10%'>" + Filenames[index + 1].fsize + "</td>";
    }
    webpage += "</tr>";
    index = index + 2;
  }
  webpage += "</table>";
  webpage += HTML_Footer();
}
// #############################################################################################
void SelectInput(String Heading, String Command, String Arg_name)
{
  webpage = HTML_Header();
  webpage += "<h3>" + Heading + "</h3>";
  webpage += "<form  action='/" + Command + "'>";
  webpage += "Filename: <input type='text' name='" + Arg_name + "'><br><br>";
  webpage += "<input type='submit' value='Enter'>";
  webpage += "</form>";
  webpage += HTML_Footer();
}
// #############################################################################################
int GetFileSize(String filename)
{
  int filesize;
  File CheckFile;
  if (isSPIFFS)
    CheckFile = SPIFFS.open(filename, "r");
  else
    CheckFile = SD.open(filename, "r");

  filesize = CheckFile.size();
  CheckFile.close();
  return filesize;
}
// #############################################################################################
void LogOut()
{
  webpage = HTML_Header();
  webpage += "<h1>Log Out</h1>";
  webpage += "<h4>You have now been logged out...</h4>";
  webpage += "<h4>NOTE: On most browsers you must close all windows for this to take effect...</h4>";
  webpage += HTML_Footer();
}
// #############################################################################################
// void Display_New_Page()
// {
//   webpage = HTML_Header();                            // Add HTML headers, note '=' for new page assignment
//   webpage += "<h1>My New Web Page</h1>";              // Give it a heading
//   webpage += "<h3>Example entries for the page</h3>"; // Set the scene
//   webpage += "<p>NOTE:</p>";
//   // webpage += "<p>You can display global variables like this: " + String(Temperature, 2) + "属C</p>"; // Convert numeric variables to a string, use 2-decimal places
//   // webpage += "<p>Or, like this, my name is: " + Name + "</p>";                                       // no need to convert string variables
//   webpage += HTML_Footer(); // Add HTML footers, don't forget to add a menu name in header!
// }
// #############################################################################################
void Page_Not_Found()
{
  webpage = HTML_Header();
  webpage += "<div class='notfound'>";
  webpage += "<h1>Sorry</h1>";
  webpage += "<p>Error 404 - Page Not Found</p>";
  webpage += "</div><div class='left'>";
  webpage += "<p>The page you were looking for was not found, it may have been moved or is currently unavailable.</p>";
  webpage += "<p>Please check the address is spelt correctly and try again.</p>";
  webpage += "<p>Or click <b><a href='/'>[Here]</a></b> for the home page.</p></div>";
  webpage += HTML_Footer();
}
// #############################################################################################
void Display_System_Info()
{
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  if (WiFi.scanComplete() == -2)
    WiFi.scanNetworks(true, false); // Scan parameters are (async, show_hidden) if async = true, don't wait for the result
  webpage = HTML_Header();
  webpage += "<h3>System Information</h3>";
  webpage += "<h4>Transfer Statistics</h4>";
  webpage += "<table class='center'>";
  webpage += "<tr><th>Last Upload</th><th>Last Download/Stream</th><th>Units</th></tr>";
  webpage += "<tr><td>" + ConvBinUnits(uploadsize, 1) + "</td><td>" + ConvBinUnits(downloadsize, 1) + "</td><td>File Size</td></tr> ";
  webpage += "<tr><td>" + ConvBinUnits((float)uploadsize / uploadtime * 1024.0, 1) + "/Sec</td>";
  webpage += "<td>" + ConvBinUnits((float)downloadsize / downloadtime * 1024.0, 1) + "/Sec</td><td>Transfer Rate</td></tr>";
  webpage += "</table>";
  webpage += "<h4>Filing System</h4>";
  webpage += "<table class='center'>";
  webpage += "<tr><th>Total Space</th><th>Used Space</th><th>Free Space</th><th>Number of Files</th></tr>";
  webpage += "<tr><td>" + ConvBinUnits(SPIFFS.totalBytes(), 1) + "</td>";
  webpage += "<td>" + ConvBinUnits(SPIFFS.usedBytes(), 1) + "</td>";
  webpage += "<td>" + ConvBinUnits(SPIFFS.totalBytes() - SPIFFS.usedBytes(), 1) + "</td>";
  webpage += "<td>" + (numfiles == 0 ? "Pending Dir or Empty" : String(numfiles)) + "</td></tr>";
  webpage += "</table>";
  webpage += "<h4>CPU Information</h4>";
  webpage += "<table class='center'>";
  webpage += "<tr><th>Parameter</th><th>Value</th></tr>";
  webpage += "<tr><td>Number of Cores</td><td>" + String(chip_info.cores) + "</td></tr>";
  webpage += "<tr><td>Chip revision</td><td>" + String(chip_info.revision) + "</td></tr>";
  webpage += "<tr><td>Internal or External Flash Memory</td><td>" + String(((chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "Embedded" : "External")) + "</td></tr>";
  webpage += "<tr><td>Flash Memory Size</td><td>" + String((spi_flash_get_chip_size() / (1024 * 1024))) + " MB</td></tr>";
  webpage += "<tr><td>Current Free RAM</td><td>" + ConvBinUnits(ESP.getFreeHeap(), 1) + "</td></tr>";
  webpage += "</table>";
  webpage += "<h4>Network Information</h4>";
  webpage += "<table class='center'>";
  webpage += "<tr><th>Parameter</th><th>Value</th></tr>";
  webpage += "<tr><td>LAN IP Address</td><td>" + String(WiFi.localIP().toString()) + "</td></tr>";
  webpage += "<tr><td>Network Adapter MAC Address</td><td>" + String(WiFi.BSSIDstr()) + "</td></tr>";
  webpage += "<tr><td>WiFi SSID</td><td>" + String(WiFi.SSID()) + "</td></tr>";
  webpage += "<tr><td>WiFi RSSI</td><td>" + String(WiFi.RSSI()) + " dB</td></tr>";
  webpage += "<tr><td>WiFi Channel</td><td>" + String(WiFi.channel()) + "</td></tr>";
  webpage += "<tr><td>WiFi Encryption Type</td><td>" + String(EncryptionType(WiFi.encryptionType(0))) + "</td></tr>";
  webpage += "</table> ";
  webpage += HTML_Footer();
}
// #############################################################################################
String ConvBinUnits(int bytes, int resolution)
{
  if (bytes < 1024)
  {
    return String(bytes) + " B";
  }
  else if (bytes < 1024 * 1024)
  {
    return String((bytes / 1024.0), resolution) + " KB";
  }
  else if (bytes < (1024 * 1024 * 1024))
  {
    return String((bytes / 1024.0 / 1024.0), resolution) + " MB";
  }
  else
    return "";
}
// #############################################################################################
String EncryptionType(wifi_auth_mode_t encryptionType)
{
  switch (encryptionType)
  {
  case (WIFI_AUTH_OPEN):
    return "OPEN";
  case (WIFI_AUTH_WEP):
    return "WEP";
  case (WIFI_AUTH_WPA_PSK):
    return "WPA PSK";
  case (WIFI_AUTH_WPA2_PSK):
    return "WPA2 PSK";
  case (WIFI_AUTH_WPA_WPA2_PSK):
    return "WPA WPA2 PSK";
  case (WIFI_AUTH_WPA2_ENTERPRISE):
    return "WPA2 ENTERPRISE";
  case (WIFI_AUTH_MAX):
    return "WPA2 MAX";
  default:
    return "";
  }
}
// #############################################################################################
bool StartMDNSservice(const char *Name)
{
  esp_err_t err = mdns_init(); // Initialise mDNS service
  if (err)
  {
    printf("MDNS Init failed: %d\n", err); // Return if error detected
    return false;
  }
  mdns_hostname_set(Name); // Set hostname
  return true;
}
// #############################################################################################
