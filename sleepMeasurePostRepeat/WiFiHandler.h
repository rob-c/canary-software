/*
 * Wifi handling class.
 * This inherits from the network handler class.
 */

#ifndef WIFIHANDLER
#define WIFIHANDLER

//******************************************
//libraries
#include "NetworkHandler.h"
 
//******************************************
class WiFiHandler: public virtual NetworkHandler {

  //------------------------------------------
  public:
    WiFiHandler(char* ssid,
		char* password);
    int status(bool verbose=false) override;
    bool connect(bool verbose=false) override;
    String getMACAddress() override {return WiFi.macAddress();}
    void disconnect() override;

  //------------------------------------------
  private:
    const char* _ssid;
    const char* _password;
};
#endif //WIFIHANDLER
