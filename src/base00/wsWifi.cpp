// ----------------------------<wsWifi.cpp>------------------------------------
#include "wsWifi.h"

const String WIFI_SPIFFS = "/wsWifi.json";
const String WIFI_SD = "/wsWifi.json";
const String WIFITXT_SD = "/wifi.txt";


void wsHandleWifiSetting(String initS, String ssidS, String passwdS,String removeS,
                    String ipS, String gatewayS, String subnetS, String dnsS)
{
  DynamicJsonDocument wifiJson(WIFIJSON_SIZE);

  if (initS == "on")
  { // 初期化
    if (!initWifiJson(wifiJson))
    {
      Serial.println("faile to init wifiJson file");
      return;
    }

    if ((ssidS == "") && (passwdS == ""))
    {
      webpage = "WifiSet : init = on";
      return;
    }
  }

  if (!jsonRead(FLTYPE_SPIFFS, wifiJson, WIFI_SPIFFS))
  {
    Serial.println("faile to Read from SPIFFS");
    return;
  }

  if ((ssidS == "") && (passwdS == "") && (removeS == "") && (initS == ""))
  {
    // HTMLデータを出力する
    String html = "";
    serializeJsonPretty(wifiJson, html);
    Serial.println(html);
    webpage = html;
    return;
  }

  if (removeS != "")
  {
    uint8_t ap_no = 0;
    ap_no = removeS.toInt();
    String msg = "remove accesspoint : " + removeS;
    Serial.println(msg);

    JsonArray jsonArray = wifiJson["accesspoint"];
    uint8_t arraySize = jsonArray.size();
    Serial.println("arraySize = " + String(arraySize, DEC));
    // Serial.println(arraySize, DEC);

    if ((ap_no >= 0) && (ap_no < arraySize))
    {
      jsonArray.remove(ap_no); // データ削除

      if (!jsonSave(FLTYPE_SPIFFS, wifiJson, WIFI_SPIFFS))
      {
        Serial.println("faile to Save to SPIFFS");
        return;
      }

      String msgJson = "";
      serializeJsonPretty(wifiJson, msgJson);
      Serial.println(msgJson);
      webpage = "wifi remove = " + removeS;
      return;
    }
    else
    {
      Serial.println("fail Wifi remove : invalid ap_no = " + removeS);
      return;
    }
  }

  if ((ssidS != "") && (passwdS != ""))
  {
    JsonArray jsonArray = wifiJson["accesspoint"];
    JsonObject new_ap = jsonArray.createNestedObject();
    new_ap["ssid"] = ssidS;
    new_ap["passwd"] = passwdS;
    new_ap["ip"] = ipS;
    new_ap["gateway"] = gatewayS;
    new_ap["subnet"] = subnetS;
    new_ap["dns"] = dnsS;

    if (!jsonSave(FLTYPE_SPIFFS, wifiJson, WIFI_SPIFFS))
    {
      Serial.println("faile to Save to SPIFFS");
      return;
    }
    webpage = "wifi : new Access Poitn Add";
    return;
  }
}

void wifiSetup()
{
  // *** wifi setup **********
  bool success = wifiConnect();
  if (!success)
    REBOOT("wifi : cannot connected !!"); // --- Reboot

  // --- Wifi connected !! ---
  IP_ADDR = WiFi.localIP().toString();
  
  M5.Lcd.println("\nConnected");
  Serial.println("IP_ADDR = " + IP_ADDR);
  Serial.println("SSID = " + SSID);
  Serial.println("Go to http://" + IP_ADDR);

  M5.Lcd.print("Go to http://");
  M5.Lcd.println(WiFi.localIP());
}

bool initWifiJson(DynamicJsonDocument &wifiJson)
{
  String wifiJsonInitStr = " { \"timeout\": 10, \"accesspoint\": [ ] }";
  DeserializationError error = deserializeJson(wifiJson, wifiJsonInitStr);
  if (error)
  {
    Serial.println("DeserializationError in initWiFiJson func");
    return false;
  }
  jsonSave(FLTYPE_SPIFFS, wifiJson, WIFI_SPIFFS);
  return true;
}


bool wifiSelect(int flType)
{
  DynamicJsonDocument wifiJson(WIFIJSON_SIZE);

  if (flType == FLTYPE_SPIFFS)
  {
    if (!jsonRead(FLTYPE_SPIFFS, wifiJson, WIFI_SPIFFS))
    {
      Serial.println("Fail : wsWife.json in SPIFFS");
      return false;
    }
  }
  else if (flType == FLTYPE_SD)
  {
    if (!jsonRead(FLTYPE_SD, wifiJson, WIFI_SPIFFS))
    {
      Serial.println("Fail : wsWife.json in SD");
      return false;
    }
  }
  // Serial.println("### wsWife.json file read Success! ### ");

  // ---------------------------------------
  int timeOut = wifiJson["timeout"];

  JsonArray jsonArray = wifiJson["accesspoint"];
  if (jsonArray.size() < 1)
  {
    Serial.println("no AP information in wsWifi.json file!");
    return false;
  }

  for (int index = 0; index < jsonArray.size(); ++index)
  {
    JsonObject object = jsonArray[index];
    String ssid = object["ssid"];
    String passWord = object["passwd"];

    // --- fix IP mode -------
    String ip_str = object["ip"];
    String gateway_str = object["gateway"];
    String subnet_str = object["subnet"];
    String dns_str = object["dns"];

    M5.Lcd.print("\nConnecting ");
    M5.Lcd.print(ssid);

    if (ip_str != "")
    { // 固定IPモードの処理 ----------------------------------------------
      int ip[4], gateway[4], subnet[4], dns[4];

      if (IPstrToIntArray(ip_str, ip) && IPstrToIntArray(gateway_str, gateway) && IPstrToIntArray(subnet_str, subnet))
      {
        IPAddress fix_ip(ip[0], ip[1], ip[2], ip[3]);
        IPAddress fix_gateway(gateway[0], gateway[1], gateway[2], gateway[3]);
        IPAddress fix_subnet(subnet[0], subnet[1], subnet[2], subnet[3]);

        if (IPstrToIntArray(dns_str, dns))
        { // DNS情報が有効ならば、４つの情報で接続
          IPAddress fix_dns(dns[0], dns[1], dns[2], dns[3]);

          if (!WiFi.config(fix_ip, fix_gateway, fix_subnet, fix_dns))
          {
            Serial.println("Failed to FixIP with DNS configure!");
            return false;
          }
          else
          {
            Serial.println("Try to connect fixIP4 : ip-gateway-subnet-dns ");
          }
        }
        else
        { // DNS情報が無効ならば、３つの情報で接続
          if (!WiFi.config(fix_ip, fix_gateway, fix_subnet))
          {
            Serial.println("Failed to Fix-WiFi no DNS configure!");
            return false;
          }
          else
          {
            Serial.println("Try to connect fixIP3 : ip-gateway-subnet ");
          }
        }
      }
    } // -------------------------------------------------------------------------

    WiFi.begin(ssid.c_str(), passWord.c_str());
    int loopCount = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      M5.Display.print(".");
      Serial.print(".");
      delay(500);
      // 設定されたタイムアウト秒接続できなかったら抜ける
      if (loopCount++ > timeOut * 2)
      {
        break;
      }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      SSID = ssid;
      SSID_PASSWD = passWord;
      return true;
    }
  }
  return false;
}

bool wifiNoSetupFileConnect()
{
  // 設定ファイルが無効の場合は、前回接続情報での接続
  WiFi.begin();

  // 待機
  int loopCount10sec = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    M5.Display.print(".");
    Serial.print(".");
    delay(500);
    // 10秒以上接続できなかったら false
    if (loopCount10sec++ > 10 * 2)
    {
      return false;
    }
  }
  return true;
}

bool wifiSmartConfigConnect()
{
  // ---------------------------------------
  // SmartConfig接続
  M5.Display.println("");
  Serial.println("");
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  M5.Display.println("Waiting for SmartConfig");
  Serial.println("Waiting for SmartConfig");

  // ---------------------------------------
  int loopCount30sec = 0;
  while (!WiFi.smartConfigDone())
  {
    delay(500);
    M5.Display.print("#");
    Serial.print("#");
    // 30秒以上 smartConfigDoneできなかったら false
    if (loopCount30sec++ > 30 * 2)
    {
      return false;
    }
  }

  // ---------------------------------------
  // Wi-fi接続待ち
  M5.Display.println("");
  Serial.println("");
  M5.Display.println("Waiting for WiFi");
  Serial.println("Waiting for WiFi");

  int loopCount60sec = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    M5.Display.print(".");
    Serial.print(".");
    // 60秒以上接続できなかったら false
    if (loopCount60sec++ > 60 * 2)
    {
      return false;
    }
  }

  return true;
}

// #define EX_SMART_CONFIG_TEST
bool wifiConnect()
{
  Serial.println("### Connect WiFi Start ###");
  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  M5.Lcd.print("Connecting");

#ifndef EX_SMART_CONFIG_TEST
  Serial.println("#1. CONNECTING : wsWifi.json in SPIFFS");
  if (wifiSelect(FLTYPE_SPIFFS))
  {
    Serial.println("\nCONNECTED : wsWifi.json in SPIFFS");
    return true;
  }

  Serial.println("#2. CONNECTING : wsWifi.json in SD ");
  if (wifiSelect(FLTYPE_SD))
  {
    Serial.println("\nCONNECTED : wsWifi.json in SD");
    return true;
  }

  Serial.println("#3. CONNECTING : wifi.txt in SD");
  // "wifi.txt" の接続
  if (wifiTxtConnect())
  {
    Serial.println("\nCONNECTED : wifi.txt in SD");
    return true;
  }

  Serial.println("#4. CONNECTING : privious wifi settings");
  // 前回接続情報での接続
  if (wifiNoSetupFileConnect())
  {
    Serial.println("\nCONNECTED : privious Setup wifi settings");
    return true;
  }
#endif

  // SmartConfigでの接続
  Serial.println("#5. CONNECTING : SmartConfig");
  if (wifiSmartConfigConnect())
  {
    Serial.println("\nCONNECTED : SmartConfigConnect wifi");
    return true;
  }

  // 全てに失敗した場合
  Serial.println("\n*** Fail : All Wife Connect Settings Done ***");
  return false;
}



bool wifiTxtRead()
{
  File file = fileOpen(FLTYPE_SD, WIFITXT_SD, FILE_READ);
  if (!file)
  {
    // SD.end();
    Serial.println("Fail : wifi.txt not open ");
    return false;
  }

  size_t sz = file.size();
  char buf[sz + 1];
  file.read((uint8_t *)buf, sz);
  buf[sz] = 0;
  file.close();
  // SD.end();

  int y = 0;
  for (int x = 0; x < sz; x++)
  {
    if (buf[x] == 0x0a || buf[x] == 0x0d)
      buf[x] = 0;
    else if (!y && x > 0 && !buf[x - 1] && buf[x])
      y = x;
  }

  SSID = buf;
  SSID_PASSWD = &buf[y];

  if ((SSID == "") || (SSID_PASSWD == ""))
  {
    Serial.println("Fail : ssid or passwd is void ");
    return false;
  }

  return true;
}




bool wifiTxtConnect()
{
  Serial.println("connecting wifi.txt");

  // "wifi.txt"ファイル
  if (!wifiTxtRead())
  {
    Serial.println("Fail : reading wifi.txt in SD");
    return false;
  }

  // "wifi.txt" 接続情報
  WiFi.begin(SSID.c_str(), SSID_PASSWD.c_str());

  // 待機
  int loopCount10sec = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    M5.Display.print(".");
    Serial.print(".");
    delay(500);
    // 10秒以上接続できなかったら false
    if (loopCount10sec++ > 10 * 2)
    {
      Serial.println(" faile to connect  wifi.txt");
      return false;
    }
  }
  return true;
}

bool IPstrToIntArray(String strIPaddr, int *iAddr)
{ // in  : String strIPaddr --> "192.168.0.1"
  // out : int iAddr[4]     --> iAddr[0]=192,iAddr[1]=168,iAddr[2]=0,iAddr[3]=1
  // ret : true(success), false(error)
  // ---------------------------------------------------------------------------

  long addr = 0;
  struct in_addr tag;
  unsigned char szIp[4];

  // IPアドレス文字列を変換
  addr = inet_addr(strIPaddr.c_str());
  if (addr == -1)
  {
    Serial.print("IP addr Conver Error : ");
    Serial.println(strIPaddr);
    return false;
  }

  // バイナリ形式からunsigned charに変換
  memcpy(szIp, &addr, sizeof(long));

  // int型にキャスト
  iAddr[0] = (int)szIp[0];
  iAddr[1] = (int)szIp[1];
  iAddr[2] = (int)szIp[2];
  iAddr[3] = (int)szIp[3];

  // char tmp_str[40];
  // sprintf(tmp_str, "Convert [%d].[%d].[%d].[%d]", iAddr[0], iAddr[1], iAddr[2], iAddr[3]);
  // Serial.println(tmp_str);
  return true;
}