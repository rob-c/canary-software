/*
 * MQTT handling class.
 */

#ifndef MQTTHANDLER
#define MQTTHANDLER

//******************************************
//libraries
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include "WiFiClientSecure.h"
#include "config.h"
#ifdef ESP8266
#include "ESP8266WiFi.h"
#endif //ESP8266

//******************************************
class MQTTHandler {
  
  //------------------------------------------
  public:
    MQTTHandler(PubSubClient* mqttclient,
		char* server,
		unsigned int port,
		bool tls,
		char* username,
		char* password,
		char* topic,
		unsigned int messagesize,
		const char* cacert="");
    void init();
    void setClient(PubSubClient* mqttclient) {_mqttclient = mqttclient; return;}
    int status(bool verbose=false);
    bool connect(bool verbose=false);
    bool post(JsonDocument &doc,
	      bool post=true,
	      bool verbose=false);
    bool loop() {return _mqttclient->loop();}
    
  //------------------------------------------
  private:
    char* _server;
    unsigned int _port;
    bool _tls;
    char* _username;
    char* _password;
    char* _topic;
    unsigned int _messagesize;
    WiFiClient _wificlient;
    WiFiClientSecure _wificlientsecure;
    PubSubClient* _mqttclient;
    char _clientid[10];
    const char _alphanum[63] = {
      '0','1','2','3','4','5','6','7','8','9',
      'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
      'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
};
#endif //MQTTHANDLER
