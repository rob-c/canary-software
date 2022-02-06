/*
 * Wifi handling class.
 * This inheriths from the network handler class.
 */

#ifndef WIFIHANDLER
#define WIFIHANDLER

//******************************************
//libraries
#include "NetworkHandler.h"
//NOTE include the MQTTHandler library so that it knows the correct wifi library to use
#include "MQTTHandler.h"
 
//******************************************
class WiFiHandler: public virtual NetworkHandler {

  //------------------------------------------
  public:
    WiFiHandler(char* ssid,
		char* password);
    int status(bool verbose=false);
    bool connect(bool verbose=false) override;
    String getMACAddress() override {return WiFi.macAddress();}
    void disconnect() override;

  //------------------------------------------
  private:
    const char* _ssid;
    const char* _password;
};
#endif //WIFIHANDLER
