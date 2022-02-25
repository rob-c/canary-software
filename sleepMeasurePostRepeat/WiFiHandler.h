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
    WiFiHandler(const char* ssid,
		const char* password);
    WiFiHandler(const char* ssid,
                const char* eap_identity,
                const char* eap_username,
                const char* eap_password);
    int status(bool verbose=false) override;
    bool connect(bool verbose=false) override;
    bool basic_connect(bool verbose=false);
    bool eap_connect(bool verbose=false);
    String getMACAddress() override {return WiFi.macAddress();}
    void disconnect() override;

  //------------------------------------------
  private:
    bool _use_eap;
    const char* _ssid;
    const char* _password;
    const char* _eap_identity;
    const char* _eap_username;
    const char* _eap_password;
};
#endif //WIFIHANDLER
