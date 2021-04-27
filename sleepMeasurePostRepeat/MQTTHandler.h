/*
 * MQTT handling class.
 */

#ifndef MQTTHANDLER
#define MQTTHANDLER

//******************************************
//libraries
//NOTE include the WiFiHandler library so that it knows the correct wifi library to use
#include "WiFiHandler.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"

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
		  bool disconnect=false,
		  bool verbose=false);
    bool loop() {return _client.loop();}
  
  //------------------------------------------
  private:
    char* _server;
    unsigned int _port;
    char* _username;
    char* _password;
    char* _topic;
    unsigned int _messagesize;
    WiFiClient _wificlient;
    PubSubClient _client;
    char _clientid[10];
    char _alphanum[63] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
};
#endif //MQTTHANDLER
