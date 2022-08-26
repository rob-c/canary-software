/*
 * Generic network handling class.
 * Wifi and Ethernet handling classes inherit from this class.
 */
 
#ifndef NETWORKHANDLER
#define NETWORKHANDLER

//******************************************
#include "WString.h"
//NOTE include the MQTTHandler library so that it knows the correct wifi library to use
#include "MQTTHandler.h"

//******************************************
class NetworkHandler {

  //------------------------------------------
  public:
    NetworkHandler(){};
    virtual int status(bool verbose=false) {return 0;};
    virtual bool connect(bool verbose=false) {return false;}
    virtual String getMACAddress(void) {return "";}
    virtual void disconnect(void) {return;}
    
};
#endif //NETWORKHANDLER
