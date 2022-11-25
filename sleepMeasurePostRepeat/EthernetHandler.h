/*
 * Ethernet handling class.
 * This inherits from the network handler class.
 */

#ifndef ESP8266
#ifndef ETHERNETHANDLER
#define ETHERNETHANDLER

//******************************************
//libraries
#include "NetworkHandler.h"
#include "ETH.h"
#include "config.h"

//******************************************
class EthernetHandler: public virtual NetworkHandler {

  //------------------------------------------
  public:
    EthernetHandler();
    int status(bool verbose=false) override;
    bool connect(bool verbose=false) override;
    String getMACAddress() override {return ETH.macAddress();}
    void disconnect() override {return;}
    static void handleEvent(WiFiEvent_t event);

  //------------------------------------------
  private:
    static bool _connected;
};

#endif //ETHERNETHANDLER
#endif //not ESP8266
