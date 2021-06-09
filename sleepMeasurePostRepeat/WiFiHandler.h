/*
 * Wifi handling class.
 */

#ifndef WIFIHANDLER
#define WIFIHANDLER

//******************************************
//libraries
//NOTE include the MQTTHandler library so that it knows the correct wifi library to use
#include "MQTTHandler.h"
 
//******************************************
class WiFiHandler {

  //------------------------------------------
  public:
    WiFiHandler(char* ssid,
		char* password);
    int status(bool verbose=false);
    bool connect(bool verbose=false);
    String getMACAddress() {return WiFi.macAddress();}
    void disconnect();

  //------------------------------------------
  private:
    const char* _ssid;
    const char* _password;
};
#endif //WIFIHANDLER
