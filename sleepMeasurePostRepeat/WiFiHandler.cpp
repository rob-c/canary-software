#include "WiFiHandler.h"
//#include <esp_wifi.h>
#include <esp_wpa2.h>
//#include <WiFi.h>

//******************************************
//wifi handler constructor
WiFiHandler::WiFiHandler(const char* ssid,
			 const char* password) {
  _use_eap = false;
  _ssid = ssid;
  _password = password;
  return;
}

//****
// Eduroam/Enterprise constructor
WiFiHandler::WiFiHandler(const char* ssid,
    const char* eap_identity,
    const char* eap_username,
    const char* eap_password) {
  _use_eap = true;
  _ssid = ssid;
  _eap_identity = eap_identity;
  _eap_username = eap_username;
  _eap_password = eap_password;
  return;      
}

//******************************************
//get wifi status
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
//connect to wifi with correct method - just a wrapper
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

  // start disconnected
  WiFi.disconnect(true, true);

  if (_use_eap) { return eap_connect(verbose); }
  else { return basic_connect(verbose); }
}

//******************************************
//connect eap enabled wifi i.e. eduroam
bool WiFiHandler::eap_connect(bool verbose) {

  Serial.printf("MAC >> '%s'\n", WiFi.macAddress());
  Serial.printf("Connecting to WiFi: '%s' \n", _ssid);

  WiFi.mode(WIFI_STA); //init wifi mode

  Serial.print("identity >> ");
  Serial.println(_eap_identity);
  if( esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)_eap_identity, strlen(_eap_identity)) ){
    Serial.println("Failed to set WPA2 Identity");
    return false;
  }
  Serial.print("user >> ");
  if (verbose) {
    Serial.println(_eap_username);
  } else {
    Serial.println("user-xxx-yyy-zzz");
  }
  if( esp_wifi_sta_wpa2_ent_set_username((uint8_t *)_eap_username, strlen(_eap_username)) ){
    Serial.println("Failed to set WPA2 Username");
    return false;
  }
  Serial.print("pass >> ");
  if (verbose) {    
    Serial.println(_eap_password);
  } else {
    Serial.print("pass-xxx-yyy-zzz");
  }
  if( esp_wifi_sta_wpa2_ent_set_password((uint8_t *)_eap_password, strlen(_eap_password)) ){
    Serial.println("Failed to set WPA2 Password");
    return false;
  }

#ifdef ENT_WIFI_V1
  bool err = esp_wifi_sta_wpa2_ent_enable();
#else
  esp_wpa2_config_t wpa2_config = WPA2_CONFIG_INIT_DEFAULT();
  bool err = esp_wifi_sta_wpa2_ent_enable(&wpa2_config);
#endif

  if( err ){
    Serial.println("Failed to set WPA2 Enterprise config");
    return false;
  }

  short counter=0;
  WiFi.setAutoReconnect(true);
  WiFi.begin(_ssid, NULL, 0 , NULL, true);
  WiFi.setTxPower(WIFI_POWER_2dBm);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
    counter++;
    if (counter >= 60)
    { //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }

  Serial.println("");
  Serial.println(F("WiFi is connected!"));
  Serial.println(F("IP address set: "));
  Serial.println(WiFi.localIP()); //print LAN IP

  return true;
}

//******************************************
//connect wifi
bool WiFiHandler::basic_connect(bool verbose) {

  //------------------------------------------
  //wifi disconnect
  //https://github.com/esp8266/Arduino/issues/2702
  WiFi.disconnect();

  //wifi mode
#ifdef ESP8266
  WiFi.mode(WIFI_STA); //ESP8266
#else //ESP32
  WiFi.mode(WIFI_MODE_STA); //ESP32
#endif //ESP8266

  //wifi begin
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
