#include "cred8266.h"
#include "Arduino.h"
String content;
String esid;
String epass = "";
String esid1;
String epass1 = "";

void handleNotFound()
{ // page not found err for webserver
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void wifi() // save wifi cred in eeprom
{
  content = "<!DOCTYPE HTML>\r\n<html><head><meta name='viewport' content='width=device-width, initial-scale=1'></head><body style='color:blue;text-align:center;'><h1>SET YOUR WIFI HERE</h1><br><br>";
  content += "<form method='post' action='setting'><label>SSID: </label><input name='ssid' length=32 ><br><br><label>PASS: </label><input name='pass' length=64><br><br><label>SSID1: </label><input name='ssid1' length=32 ><br><br><label>PASS1: </label><input name='pass1' length=64><br><br><input type='submit'></form>";
  content += "</body></html>";
  server.send(200, "text/html", content);

  server.on("/setting", []()
            {
              String qsid = server.arg("ssid");
              String qpass = server.arg("pass");
              String qsid1 = server.arg("ssid1");
              String qpass1 = server.arg("pass1");

              if (qsid.length() > 0 && qpass.length() > 0)
              {
                Serial.println(qsid);
                Serial.println(qpass);
                Serial.println(qsid1);
                Serial.println(qpass1);
                for (int i = 0; i < 64; ++i)
                {
                  EEPROM.write(i + 227, 0);
                }
                for (int i = 0; i < 33; ++i)
                {
                  Serial.println(227 + i);
                  Serial.print(qsid[i]);
                  EEPROM.write(227 + i, qsid[i]);
                }
                for (int i = 0; i < 33; ++i)
                {
                  Serial.println(259 + i);
                  Serial.print(qpass[i]);
                  EEPROM.write(259 + i, qpass[i]);
                }
                for (int i = 0; i < 64; ++i)
                {
                  EEPROM.write(i + 291, 0);
                }
                for (int i = 0; i < 33; ++i)
                {
                  Serial.println(291 + i);
                  Serial.print(qsid1[i]);

                  EEPROM.write(291 + i, qsid1[i]);
                }
                for (int i = 0; i < 33; ++i)
                {
                  Serial.println(323 + i);
                  Serial.print(qpass1[i]);
                  EEPROM.write(323 + i, qpass1[i]);
                }

                EEPROM.commit();
                content = "<!DOCTYPE HTML>\r\n<html><head><meta name='viewport' content='width=device-width, initial-scale=1'></head><body style='color:blue;text-align:center;'><h1>SET YOUR WIFI HERE</h1><br><br>";
                content += "{\"Success\":\" Please wait, This device will restart\"}";
                content += "</body></html>";
                statusCode = 200;
                for (int i = 0; i < 2; ++i)
                {
                  delay(1000);
                }
                ESP.restart();
              }
              else
              {
                content = "{\"Error\":\"404 not found\"}";
                statusCode = 404;
              }
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.send(statusCode, "application/json", content);
            });
}

void _config() // save wifi cred in eeprom
{
  content = "<!DOCTYPE HTML>\r\n<html><head><meta name='viewport' content='width=device-width, initial-scale=1'></head><body style='color:blue;text-align:center;'><h1>SET YOUR USER CREDENTALS HERE</h1><br><br>";
  content += "<form method='post' action='config'>";
  content += "<label>DEVICE NAME: </label><input name='NAME' placeholder='MAX 10' length=10 ><br><br>";
  content += "<label>USERCODE: </label><input name='CODE' placeholder='MAX 15' length=15><br><br>";
  content += "<label>TOKEN: </label><input name='TOKEN' placeholder='MAX 170' length=170><br><br>";
  content += "<label>Restart Device? </label><input type='checkbox' id='OFFLINE' name='OFFLINE' value='1'><br><br>";
  content += "<label>NO. OF APPLIANCES</label><select name='COUNT'><br><br>";
  content += "<option value=''></option><br><br>";
  content += "<option value='1'>ONE</option></select><br><br>";
  // content += "<option value='2'>TWO</option></select><br><br>";
  content += "<label>ID1: </label><input name='ID1' placeholder='MAX 4' length=4><br><br>";
  // content += "<label>ID2: </label><input name='ID2' placeholder='MAX 4' length=4><br><br>";
  content += "<input type = 'submit'> </form > ";
  content += "<form method='post' action='_update'>";
  content += "<label>UPDATE: </label><input type='checkbox' id='UPDATE' name='UPDATE' value='1'><br><br>";
  content += "<input type = 'submit'> </form > ";
  content += "</body></html>";
  server.send(200, "text / html", content);

  server.on("/config", []()
            {
              String msg;
              String device = server.arg("NAME");
              String usercode = server.arg("CODE");
              String Token = server.arg("TOKEN");
              String apps = server.arg("COUNT");
              String ID1 = server.arg("ID1");
              // String ID2 = server.arg("ID2");
              //String ID3 = server.arg("ID3");
              //String ID4 = server.arg("ID4");
              String _mode = server.arg("OFFLINE");
              // String _up_date= server.arg("UPDATE");

              Serial.print("_mode----");
              Serial.println(_mode);
              Serial.println();
              //  if(_up_date != 0)
              // {
              //   update_re=true;
              //   server.on("/update", _update);
              // }
              // else{
              //   update_re=false;
              // }
              //    DEVICE NAME STARTS    //
              if (device.length() > 0)
              {
                for (int i = 0; i < 11; ++i)
                {
                  //        Serial.print(i);
                  EEPROM.write(i, 0);
                }
                for (int i = 0; i < 11; ++i)
                {
                  Serial.print(device[i]);
                  EEPROM.write(i, device[i]);
                }
                EEPROM.commit();
              }
              //    DEVICE NAME ENDS    //
              Serial.println();

              //    USER CREDENTIALS STARTS    //
              if (usercode.length() > 0 && Token.length() > 0)
              {
                if (server.hasArg("COUNT"))
                {
                  if (server.arg("COUNT") != 0)
                  {
                    int counts = apps.toInt();
                    if (counts != 0)
                    {
                      Serial.println("WRITING APPS:");

                      /// ERASE ALL USER DATA STARTS///
                      for (int i = 0; i < 16; ++i)
                      {

                        EEPROM.write(i + 41, 0);
                      }

                      for (int i = 0; i < 170; ++i)
                      {
                        EEPROM.write(i + 56, 0);
                      }
                      /// ERASE ALL USER DATA ENDS///
                      for (int i = 0; i < 16; ++i)
                      {
                        Serial.print(usercode[i]);
                        EEPROM.write(41 + i, usercode[i]);
                      }
                      Serial.println();
                      for (int i = 0; i < 170; ++i)
                      {
                        Serial.print(Token[i]);
                        EEPROM.write(56 + i, Token[i]);
                      }
                      Serial.println();
                      int address = 11; //11
                      Serial.println(address);
                      EEPROM.write(address, apps.toInt()); //11
                      address += sizeof(int);
                      Serial.println(address);
                      EEPROM.write(address, ID1.toInt()); //15
                      // address += sizeof(int);
                      // Serial.println(address);
                      // EEPROM.write(address, ID2.toInt()); //19
                      if (_mode != 0)
                      {
                        // Serial.println(address);
                        // Serial.println("offline mode");
                        // EEPROM.write(address, _mode.toInt()); //31

                        server.send(200, "text/plain", "restarting device, APPS UPDATE DONE");
                        EEPROM.commit();
                        delay(3000);
                        ESP.restart();
                      }
                      else
                      {
                        EEPROM.commit();
                        server.send(200, "text/plain", "APPS UPDATE DONE, no restart");
                      }

                    }
                    return;
                  }
                  else
                  {
                    msg = "MINIMUM ONE APPLIANCE ID IS REQUIRED, apps cannot be 0";
                    server.send(200, "text/plain", msg);
                    Serial.println("MINIMUM ONE APPLIANCE ID REQUIRED");
                  }
                }
              }
              else
              {
                content = "{\"Error\":\"404 not found OR No Details Provided\"}";
                statusCode = 404;
              }
              
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.send(statusCode, "application/json", content);
            });
              server.on("/_update",_update);

}

void _update()
{
  update_re=true;
  content="<form method='POST' action='/update_done' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
    //  server.sendHeader("Connection", "close");
      server.send(200, "text/html", content);
    server.on("/update_done", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
        Serial.printf("Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(maxSketchSpace)) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      server.sendHeader("Connection", "close");
      yield();
    });
}
void cred()
{
  server.on("/wifi", wifi);
  server.on("/user", _config);
  // server.on("/update", _update);

  // String ucode = "";
  for (int i = 0; i < 15; i++) // READ 41-55 = 15 BYTES FOR USERCODE
  {
    ucode += char(EEPROM.read(i + 41));
  }
  Serial.println();
  Serial.println(ucode);
  Serial.println();

  //String token = "";
  for (int i = 56; i < 226; i++) // READ 56-226 = 170 BYTES FOR TOKEN
  {
    token += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.println(token);
  Serial.println();
  int address = 11; // SET APPLIANCE ADDRESS
  //  Serial.println(EEPROM.readInt(address)); // READ FROM 11 W.R.T TO INT(4 BYTES) FOR APPLIANCE ID ==> 11= APP COUNT
  _appCount = EEPROM.read(11);
  Serial.print("count == ");
  Serial.println(_appCount);
  address += sizeof(int);
  //  Serial.println(EEPROM.readInt(address)); // READ FROM 15 W.R.T TO INT(4 BYTES) FOR APPLIANCE ID ==> 15= APP1
  _app1 = EEPROM.read(15);
  Serial.print("app1 == ");
  Serial.println(_app1);
  address += sizeof(int);
  //  Serial.println(EEPROM.readInt(address)); // READ FROM 19 W.R.T TO INT(4 BYTES) FOR APPLIANCE ID ==> 19= APP2
  // _app2 = EEPROM.read(19);
  // Serial.print("app2 == ");
  // Serial.println(_app2);

  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(227 + i));
  }
  Serial.print("SSID:");
  Serial.println(esid);
  Serial.println();

  //String pass = "";
  for (int i = 0; i < 32; ++i)
  {
    epass += char(EEPROM.read(i + 259));
  }
  Serial.print("PASS:");
  Serial.println(epass);
  //String wifi1 = "";
  for (int i = 0; i < 32; ++i)
  {
    esid1 += char(EEPROM.read(291 + i));
  }
  Serial.print("SSID1:");
  Serial.println(esid1);
  Serial.println();

  //String pass1 = "";
  for (int i = 0; i < 32; ++i)
  {
    epass1 += char(EEPROM.read(i + 323));
  }
  Serial.print("PASS1:");
  Serial.println(epass1);
  Serial.println();
}

void wifi_start()
{
  int n = WiFi.scanNetworks();
  //Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");

    for (int i = 0; i < n; ++i)
    {
      //Serial.println("  ");
      //Serial.print(i + 1);
      //Serial.print(": ");
      //Serial.print(WiFi.SSID(i));
      //Serial.println(esid.c_str());
      //Serial.println(esid1.c_str());
      if ((WiFi.SSID(i) == esid.c_str()))
      {
        //Serial.println(esid.c_str());
        //WiFi.disconnect();delay(100);
        Serial.print("connecting");
        _ssid = true;
        WiFi.begin(esid.c_str(), epass.c_str()); //delay(5000);
        int p = 0;
        while ((WiFi.status() != WL_CONNECTED) && (p < 200))
        {

          //Serial.print("connecting");
          delay(100);
          Serial.print(".");
          p++;
        }
        p = 0;
        //k=i;
        Serial.println();
        Serial.print("Connected to: ");
        Serial.println(WiFi.SSID(i));
        Serial.println("");
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("ESP Mac Address: ");
        Serial.println(WiFi.macAddress());
        Serial.print("Subnet Mask: ");
        Serial.println(WiFi.subnetMask());
        Serial.print("Gateway IP: ");
        Serial.println(WiFi.gatewayIP());
      }

      else if ((WiFi.SSID(i) == esid1.c_str()))
      {
        //WiFi.disconnect();
        Serial.println("WIFI CONNECTING");
        Serial.println(esid1.c_str());
        Serial.println(epass1.c_str());
        WiFi.begin(esid1.c_str(), epass1.c_str());
        //delay(5000);
        Serial.print("connecting");
        int p = 0;
        _ssid = true;
        while ((WiFi.status() != WL_CONNECTED) && (p < 200))
        {
          delay(100);
          Serial.print(".");
          p++;
        }
        p = 0;
        Serial.println();
        Serial.print("Connected to: ");
        Serial.println(WiFi.SSID(i));
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("ESP Mac Address: ");
        Serial.println(WiFi.macAddress());
        Serial.print("Subnet Mask: ");
        Serial.println(WiFi.subnetMask());
        Serial.print("Gateway IP: ");
        Serial.println(WiFi.gatewayIP());
      }
      //delay(1000);
    }
    //WiFi.begin(esid.c_str(), epass.c_str());
  }
}
