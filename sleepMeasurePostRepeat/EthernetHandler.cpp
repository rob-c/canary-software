#ifndef ESP8266
#include "EthernetHandler.h"

//******************************************
//using _connected from the EthernetHandler class
bool EthernetHandler::_connected;

//******************************************
//ethernet handler constructor
EthernetHandler::EthernetHandler() {

  _connected = false;

  //handle ethernet events
  //NOTE this is done through the WiFi class
  WiFi.onEvent(EthernetHandler::handleEvent);

  return;
}

//******************************************
//get ethernet status
int EthernetHandler::status(bool verbose) {
  
  if (verbose) {
    if (_connected)
      Serial.println("status: connected");
    else
      Serial.println("status: disconnected");
  
    Serial.print("MAC: ");
    Serial.println(ETH.macAddress());

    Serial.print("IP: ");
    Serial.println(ETH.localIP());

    Serial.print("IPv6: ");
    Serial.println(ETH.localIPv6());

    Serial.print("subnet mask: ");
    Serial.println(ETH.subnetMask());

    Serial.print("gateway IP: ");
    Serial.println(ETH.gatewayIP());

    Serial.print("DNS IP: ");
    Serial.println(ETH.dnsIP());

    Serial.print("broadcast IP: ");
    Serial.println(ETH.broadcastIP());

    Serial.print("network ID: ");
    Serial.println(ETH.networkID());

    Serial.print("subnet CIDR: ");
    Serial.println(ETH.subnetCIDR());
  
    Serial.print("speeed: ");
    Serial.print(ETH.linkSpeed());
    Serial.println(" Mbps");

  }

  return _connected;
}

//******************************************
//connect ethernet
bool EthernetHandler::connect(bool verbose) {

  //------------------------------------------
  //check if already connected to the network
  if (_connected) {
    if (verbose) {
      Serial.println("already connected to the network");
    }
    return true;
  }

  //------------------------------------------
  //ethernet begin
  if (verbose) Serial.println("\nconnecting to the network...");
  ETH.begin(ETHERNETPHYADDR,
	    ETHERNETPHYPOWER,
	    ETHERNETPHYMDC,
	    ETHERNETPHYMDIO,
	    ETHERNETPHYTYPE);

  //------------------------------------------
  //wait up to 20 s until connected
  for (int ii = 0; ii < 20; ii++) {

    if (_connected) {
      status(verbose);
      return true;
    }

    Serial.print(".");
    
    //delay
    delay(1000);//ms
  }
  Serial.println("");
    
  //------------------------------------------
  //print connection error
  if (verbose) {
    Serial.println("could not connect to the network");
    status(true);
  }
  
  return false;
}

//******************************************
//handle ethernet events
//NOTE this is done through the WiFi class
void EthernetHandler::handleEvent(WiFiEvent_t event) {
  
  switch (event) {
  case ARDUINO_EVENT_ETH_START:
    Serial.println("ETH started");
    //ETH.setHostname(NAME);
    break;
  case ARDUINO_EVENT_ETH_CONNECTED:
    Serial.println("ETH connected");
    EthernetHandler::_connected = true;
    break;
  case ARDUINO_EVENT_ETH_GOT_IP:
    Serial.print("ETH IP: ");
    Serial.print(ETH.localIP());
    Serial.print(", speed: ");
    Serial.print(ETH.linkSpeed());
    Serial.println(" Mbps");
    //status(true);
    EthernetHandler::_connected = true;
    break;
  case ARDUINO_EVENT_ETH_DISCONNECTED:
    Serial.println("ETH disconnected");
    EthernetHandler::_connected = false;
    break;
  case ARDUINO_EVENT_ETH_STOP:
    Serial.println("ETH stopped");
    EthernetHandler::_connected = false;
    break;
  default:
    break;
  }

  return;
}

#endif //not ESP8266
