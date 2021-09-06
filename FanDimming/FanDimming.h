#ifndef FanDimming_h
#define FanDimming_h
#include "Arduino.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define Host "vidyut.praudyogi.com"
extern const char FingerPrint[];

class Praudyogi
{
public:
  void Connect(String Authentication, String usercode);
  int get_stream(int _id);
  int set_stream(int _id, int _value);
  int set_intensity(int _id, int _value);
  int get_intensity(int _id);
  int getData(int _id);
  

private:
  int _sid;
  int _val; //, _v;
  int _Appliance_id;
  int portNumber = 443;
  int w;
};

#endif
