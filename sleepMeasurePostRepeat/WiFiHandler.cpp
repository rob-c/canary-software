#include "WiFiHandler.h"

//******************************************
//wifi handler constructor
WiFiHandler::WiFiHandler(char* ssid,
			 char* password) {
  _ssid = ssid;
  _password = password;
  return;
}

//******************************************
//connect wifi
int WiFiHandler::status(bool verbose) {

  int status = WiFi.status();
    
  if (verbose) {
    Serial.print("status: ");
    switch (status) {
    case 0:
      Serial.println("idle");
      break;
    case 1:
      Serial.println("no SSID");
      break;
    case 2:
      Serial.println("scan compl.");
      break;
    case 3:
      Serial.println("connected");
      break;
    case 4:
      Serial.println("connection failed");
      break;
    case 5:
      Serial.println("connection lost");
      break;
    case 6:
      Serial.println("disconnected");
      break;
    default:
      Serial.println("unknown");
      break;
    }
  }
  
  return status;
}

//******************************************
//connect wifi
bool WiFiHandler::connect(bool verbose) {

  //------------------------------------------
  //check if already connected to the wifi
  if (WiFi.status() == WL_CONNECTED) {
    if (verbose) {
      Serial.print("already connected to wifi ");
      Serial.println(_ssid);
    }
    return true;
  }

  //------------------------------------------
  //wifi begin
  //https://github.com/esp8266/Arduino/issues/2702
  WiFi.disconnect();
  WiFi.begin(_ssid, _password);

  if (verbose) {
    Serial.print("\nwifi connecting to ");
    Serial.print(_ssid);
    Serial.println("...");
  }

  //------------------------------------------
  //try connecting for 20 times with 1 s intervals
  for (int ii = 0; ii < 20; ii++) {

    //------------------------------------------
    //status
    if (verbose) {
      status(verbose);
    }

    //------------------------------------------
    //print connection info
    if (WiFi.status() == WL_CONNECTED) {
      if (verbose) {
	Serial.print("wifi connected to ");
	Serial.println(_ssid);
	Serial.print("IP: ");
	Serial.println(WiFi.localIP());
	Serial.print("RSSI: ");
	Serial.println(WiFi.RSSI());
	Serial.println();
      }
      return true;
    }

    //------------------------------------------
    //delay between trials
    delay(1000);//ms
  }

  //------------------------------------------
  //print connection error
  if (verbose) {
    Serial.print("could not connect to ");
    Serial.println(_ssid);
    Serial.println();
  }
  
  return false;
}

//******************************************
//disconnect wifi
void WiFiHandler::disconnect() {
  WiFi.disconnect();
  return;
}
