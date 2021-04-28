/*
 * MQTT handling class.
 */

#ifndef MQTTHANDLER
#define MQTTHANDLER

//******************************************
//libraries
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include "config.h"

//ESP32 or ESP8266
#ifdef ESP8266
#include "ESP8266WiFi.h" //ESP8266
#else
#include "WiFi.h" //ESP32, default
#endif //ESP32 or ESP8266

//******************************************
class MQTTHandler {
  
  //------------------------------------------
  public:
    MQTTHandler(char* server,
		unsigned int port,
		char* username,
		char* password,
		char* topic,
		unsigned int messagesize);
    void init();
    int status(bool verbose=false);
    bool connect(bool verbose=false);
    bool post(JsonDocument &doc,
		  bool post=true,
		  bool verbose=false);
    bool loop() {return _mqttclient.loop();}
  
  //------------------------------------------
  private:
    char* _server;
    unsigned int _port;
    char* _username;
    char* _password;
    char* _topic;
    unsigned int _messagesize;
    WiFiClient _wificlient;
    PubSubClient _mqttclient;
    char _clientid[10];
    const char _alphanum[63] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
};
#endif //MQTTHANDLER
