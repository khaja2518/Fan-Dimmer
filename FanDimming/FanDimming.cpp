#include "FanDimming.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
extern WiFiClientSecure client;
String AuthToken = "";
String _usercode = "";
HTTPClient http;
extern bool dimmable;

void Praudyogi::Connect(String Authentication, String usercode)
{
  AuthToken = Authentication;
  _usercode = usercode;
}

int Praudyogi::getData(int _id)
{
  String url = "https://vidyut.praudyogi.com/api/hardware/v1/getappliancestatus/" + AuthToken + '/' + _usercode + '/' + _id;
  // Serial.println(url);
  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      int _data_length = http.getSize();
      uint8_t buffer[128] = {0};
      WiFiClient *stream = http.getStreamPtr();
      while (http.connected() && (_data_length > 0 || _data_length == -1))
      {
        size_t size = stream->available();
        if (size)
        {
          int _stream = stream->readBytes(buffer, ((size > sizeof(buffer)) ? sizeof(buffer) : size));
          // Serial.write(buffer, _stream);
          String myString = String((char *)buffer);

          StaticJsonDocument<192> doc;

          DeserializationError error = deserializeJson(doc, myString);

          if (error)
          {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
          }
          int status = doc["status"];
          String type = doc["type"];

          int w = doc["power"]; // 0
          if (type == "light")
          {
            dimmable = true;
            return w;
          }
          else
          {
            dimmable = false;
            if (status == 1 || status == 0)
            {
              return status;
            }
          }
          if (_data_length > 0)
          {
            _data_length -= _stream;
          }
        }
        http.end();
        delay(1);
      }
    }
    else
    {
      Serial.println("err");
    }
  }
}

int Praudyogi::get_intensity(int _id)
{
  int id = _id;
  String url = "https://vidyut.praudyogi.com/api/hardware/v1/getappliancestatus/" + AuthToken + '/' + _usercode + '/' + id;
  // Serial.println(url);
  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      int _data_length = http.getSize();
      uint8_t buffer[128] = {0};
      WiFiClient *stream = http.getStreamPtr();
      while (http.connected() && (_data_length > 0 || _data_length == -1))
      {
        size_t size = stream->available();
        if (size)
        {
          int _stream = stream->readBytes(buffer, ((size > sizeof(buffer)) ? sizeof(buffer) : size));
          // Serial.write(buffer, _stream);
          String myString = String((char *)buffer);
          StaticJsonDocument<192> doc;

          DeserializationError error = deserializeJson(doc, myString);

          if (error)
          {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return 99;
          }
          //int w = doc["value"];
          w = doc["power"];
          // Serial.print("intensity:");Serial.println(w);
          return w;
          if (_data_length > 0)
          {
            _data_length -= _stream;
          }
        }
        http.end();
        delay(1);
      }
    }
    else
    {
      Serial.println("err");
    }
  }
}

int Praudyogi::get_stream(int _id)
{
  String url = "https://vidyut.praudyogi.com/api/hardware/v1/getappliancestatus/" + AuthToken + '/' + _usercode + '/' + _id;
  // Serial.println(url);
  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      int _data_length = http.getSize();
      uint8_t buffer[128] = {0};

      WiFiClient *stream = http.getStreamPtr();
      while (http.connected() && (_data_length > 0 || _data_length == -1))
      {
        size_t size = stream->available();
        if (size)
        {
          int _stream = stream->readBytes(buffer, ((size > sizeof(buffer)) ? sizeof(buffer) : size));
          // Serial.write(buffer, _stream);
          String myString = String((char *)buffer);
          // Serial.println(myString);// Stream& input;

          StaticJsonDocument<192> doc;

          DeserializationError error = deserializeJson(doc, myString);

          if (error)
          {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return 99;
          }

          // const char *room_id = doc["room_id"]; // "R001"
          int w = doc["status"]; // 1
          // const char *type = doc["type"];       // "fan"
          // int wattage = doc["wattage"];         // 100
          // int power = doc["power"];             // 50

          // int w = doc["value"];
          // Serial.println(w);
          if (w == 0 || w == 2)
          {
            int _val = 0;
            return _val;
          }
          else if (w == 1)
          {
            int _val = 1;
            return _val;
          }
          if (_data_length > 0)
          {
            _data_length -= _stream;
          }
          _data_length = 0;
          myString = "";
        }
        http.end();
      }
    }
    else
    {
      Serial.println("err");
    }
  }
}

int Praudyogi::set_stream(int _id, int _value)
{
  // client.setFingerprint(FingerPrint);
  _sid = _id;
  _val = _value;

  String url = "https://vidyut.praudyogi.com/api/hardware/v1/setappliancestatus/" + AuthToken + '/' + _usercode + '/' + _sid + '/' + _val;

  http.begin(client, url);
  int httpCode = http.PUT(url);
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      int _data_length = http.getSize();
      uint8_t buffer[128] = {0};
      WiFiClient *stream = http.getStreamPtr();
      while (http.connected() && (_data_length > 0 || _data_length == -1))
      {
        size_t size = stream->available();
        if (size)
        {
          int _stream = stream->readBytes(buffer, ((size > sizeof(buffer)) ? sizeof(buffer) : size));
          // Serial.write(buffer, _stream);
          String myString = String((char *)buffer);
          // Serial.println(myString);
          const size_t capacity = JSON_OBJECT_SIZE(1) + 10;
          DynamicJsonDocument doc(capacity);
          deserializeJson(doc, myString);
          int w = doc["value"];
          //Serial.println(w);
          if (w == 0 || w == 2)
          {
            int _val = 0;
            return _val;
          }
          else if (w == 1)
          {

            int _val = 1;
            return _val;
          }
          if (_data_length > 0)
          {
            _data_length -= _stream;
          }
          _data_length = 0;
          myString = "";
        }
        http.end();
        delay(1);
      }
    }
    else
    {
      Serial.println("err");
    }
  }
}

int Praudyogi::set_intensity(int _id, int _value)
{
  // client.setFingerprint(FingerPrint);
  _sid = _id;
  _val = _value;

  String url = "https://vidyut.praudyogi.com/api/hardware/v1/setapplianceintensity/" + AuthToken + '/' + _usercode + '/' + _sid + '/' + _val;

  http.begin(client, url);
  int httpCode = http.PUT(url);
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      int _data_length = http.getSize();
      uint8_t buffer[128] = {0};
      WiFiClient *stream = http.getStreamPtr();
      while (http.connected() && (_data_length > 0 || _data_length == -1))
      {
        size_t size = stream->available();
        if (size)
        {
          int _stream = stream->readBytes(buffer, ((size > sizeof(buffer)) ? sizeof(buffer) : size));
          // Serial.write(buffer, _stream);
          String myString = String((char *)buffer);
          // Serial.println(myString);
          const size_t capacity = JSON_OBJECT_SIZE(1) + 10;
          DynamicJsonDocument doc(capacity);
          deserializeJson(doc, myString);
          int w = doc["power"];
          //Serial.println(w);
          if (w == 0 )
          {
            int _val = 0;
            return _val;
          }
          else if (w < 6)
          {

            int _val = w;
            return _val;
          }
          if (_data_length > 0)
          {
            _data_length -= _stream;
          }
          _data_length = 0;
          myString = "";
        }
        http.end();
        delay(1);
      }
    }
    else
    {
      Serial.println("err");
    }
  }
}