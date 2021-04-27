/*
 * Wifi handling class.
 */

#ifndef WIFIHANDLER
#define WIFIHANDLER


//******************************************
//libraries
#include "config.h"
//ESP8266 or ESP32
#ifdef ESP8266
#include "ESP8266WiFi.h" //ESP8266
#else
#include "WiFi.h" //ESP32
#endif //ESP8266 or ESP32

//******************************************
class WiFiHandler {

  //------------------------------------------
  public:
    WiFiHandler(char* ssid,
		char* password);
    int status(bool verbose=false);
    bool connect(bool verbose=false);
    bool isConnected() {return (WiFi.status() == WL_CONNECTED);}
    WiFiClient getClient() {return _client;}
    String getMACAddress() {return WiFi.macAddress();}
    void disconnect();

  //------------------------------------------
  private:
    const char* _ssid;
    const char* _password;
    WiFiClient _client;    
};
#endif //WIFIHANDLER
