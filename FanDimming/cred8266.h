#ifndef cred8266_H
#define cred8266_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
extern ESP8266WebServer server;
#include <EEPROM.h>

static int statusCode;
static int _appCount;
extern int _app1,_app2;
extern String ucode;
extern String token;
extern String device;
extern bool _ssid;
extern bool update_re;
// const char* serverIndex1 = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";


void wifi();
void _config();
void _update();
void handleNotFound();
void cred();
void wifi_start();
#endif
