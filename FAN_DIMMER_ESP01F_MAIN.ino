#include <FanDimming.h>
#include "cred8266.h"
//#include "man8266.h"
#include "switch8266.h"

#include <EEPROM.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "ESP8266TimerInterrupt.h"

#define TIMER_INTERRUPT_DEBUG 1


WiFiClientSecure client;
ESP8266Timer ITimer;   // Init ESP8266 timer 0
bool _change = false;
bool update_re=false;
unsigned int speed_val, pot_val;
uint8_t fanSpeedVal_pre = 0;
unsigned int firingDelay_us;
void ICACHE_RAM_ATTR ISRzeroCrossing();
void ICACHE_RAM_ATTR TimerHandler();
void ICACHE_RAM_ATTR wifi_connect();
void modes(int a);
int fan_dimming();
bool dimmable = false;
bool _ssid = false;
String ucode = "";
String token = "";
String device = "";

int _app1, _app2;
int data, data_pre, mode_data, modeonline_datapre,modeoffline_datapre;

Praudyogi Dev;
ESP8266WebServer server;

bool _netStatus = false;
bool gotBack  = false;
bool f_online = false;
bool first_offline = true;

uint8_t e = 1;
int _netStatusLastValue1;
int state1 = 1;
uint8_t light;
int a;
uint8_t b, d = 1;
const char FingerPrint[] PROGMEM = "038f9cdf0d6abcfca94b78cef49f20dd30903121";

bool testWifi(void)
{
  int c = 0;
  while ( c < 2 )//200
  {
    wifi_start();
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    c++;
  }
  return false;
}

void readDeviceName()
{
  if (byte(EEPROM.read(1)) != 0 && byte(EEPROM.read(1)) != 255)
  {
    if (byte(EEPROM.read(2)) != 0 && byte(EEPROM.read(2)) != 255)
    {
      for (int i = 0; i < 11; ++i) // READ 0-11 FOR DEVICE NAME
      {
        device += char(EEPROM.read(i));
      }
      Serial.println();
      Serial.println(device.c_str());
      Serial.println("starting Ap with custom credentials");
      WiFi.softAP(device.c_str(), "Praudyogi");
      delay(30);
    }
  } else
  {
    Serial.println("starting Ap with default credentials");
    WiFi.softAP("Channel2", "Praudyogi");
    delay(30);
  }
}

void netBack()
{
  Serial.println("netBack");
  if (gotBack == true)
  {
    Serial.println("true");
    if (_change == true)
    {
      Serial.println("changed");
//      _retOnline(relay1, _netStatusLastValue1, _app1);/
      _change = false;
    }
  }
}

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(300);
  delay(10);
  pin_init();
  WiFi.mode(WIFI_AP_STA);
  readDeviceName();
  Serial.println();
  Serial.println(WiFi.softAPIP()); delay(1000);
  cred();

  if (testWifi())// test if wifi is connected
  {
    first_offline = false;
    f_online = true;
    Serial.println(ucode);
    Serial.println(token);
    Dev.Connect(token.c_str(), ucode.c_str());
  }
  server.onNotFound(handleNotFound);
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  attachInterrupt(ZCD, ISRzeroCrossing, CHANGE);
  client.setFingerprint(FingerPrint);
}

void loop()
{
  server.handleClient();
  if(update_re==false)
  {
  pot_val = analogRead(A0);
  Serial.print("pot-val: ");
  Serial.println(pot_val);
  mode_data = fan_dimming(pot_val);
  if (WiFi.status() == WL_CONNECTED)
  {
    b = 1; e = 0;
    if (b == 1 && d == 1)
    {
      if(modeoffline_datapre!=modeonline_datapre)
      {
        modeonline_datapre=modeoffline_datapre;
        Dev.set_intensity(_app1, modeonline_datapre);
      }
      _netStatus = true;
      gotBack = true;
      netBack();
      gotBack = false;
      b = 0; d = 0;
    }
    data = Dev.get_intensity(_app1);
    if (modeonline_datapre != mode_data)
    {
      delay(100);
      firingDelay_us = 10000 - ( speed_val * 10);
      modeonline_datapre = mode_data;
      Dev.set_intensity(_app1, mode_data);
      Serial.print("Mode:");
      Serial.println(mode_data);
    }
    else if (data_pre != data)
    {
      modes(data);
      delay(100);
      firingDelay_us = 10000 - ( speed_val * 10);
      data_pre = data;
      Serial.print("Mode:");
      Serial.println(data);
    }
  }
  else
  {
    _netStatus = false;
    b = 0; d = 1;
    if (modeoffline_datapre != mode_data)
    {
      delay(100);
      firingDelay_us = 10000 - ( speed_val * 10);
      modeoffline_datapre = mode_data;
      Serial.print("Mode:");
      Serial.println(mode_data);
    }
    if (first_offline)
    {
      wifi_connect();
      if (WiFi.status() == WL_CONNECTED) {
        first_offline = false;
        gotBack = true;
      }
    }
  }
  }
  else
  {
  }
  delay(50);
}

void ICACHE_RAM_ATTR TimerHandler()
{
  {
    if ((speed_val > 0) && (speed_val < 650))
    {
      ITimer.detachInterrupt();
      digitalWrite(PWM, HIGH);
      delayMicroseconds(speed_val);
      digitalWrite(PWM, LOW);
      delayMicroseconds(speed_val);
    }
    else if (speed_val > 650)
    {
      ITimer.detachInterrupt();
      digitalWrite(PWM, HIGH);
    }
    else
    {
      ITimer.detachInterrupt();
      digitalWrite(PWM, LOW);
    }
  }
}

void ICACHE_RAM_ATTR ISRzeroCrossing()  // In ESP all interrupts should be under ICACHE_RAM_ATTR
{
  ITimer.attachInterruptInterval(firingDelay_us, TimerHandler);
}

void ICACHE_RAM_ATTR wifi_connect()
{
  wifi_start();
  Dev.Connect(token.c_str(), ucode.c_str());
}

void modes(int a)
{
  switch (a)
  {
    case 1:
      speed_val = 400;
      Serial.println(speed_val);
      break;
    case 2:
      speed_val = 500;
      Serial.println(speed_val);
      break;
    case 3:
      speed_val = 550;
      Serial.println(speed_val);
      break;
    case 4:
      speed_val = 649;
      Serial.println(speed_val);
      break;
    case 5:
      speed_val = 1000;
      Serial.println(speed_val);
      break;
    default:
      speed_val = 0;
      Serial.println(speed_val);
      break;
  }
  //  Dev.set_intensity(_app2, a);
}
int fan_dimming(int pot)
{
  int re;
  if (pot < 60)
  {
    modes(0);
    re = 0;
  }
  else if (pot > 60 && pot < 200)
  {
    modes(1);
    re = 1;
  }
  else if ((pot > 200) && (pot < 400))
  {
    modes(2);
    re = 2;
  }
  else if ((pot > 400) && (pot < 600))
  {
    modes(3);
    re = 3;
  }
  else if ((pot > 600) && (pot < 800))
  {
    modes(4);
    re = 4;
  }
  else if (pot > 800)
  {
    modes(5);
    re = 5;
  }
  return re;
}
