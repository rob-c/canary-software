//******************************************
//libraries
#include "config.h"

#ifdef ESP8266
#include "ESP8266WiFi.h" //ESP8266
#else
#include "WiFi.h" //ESP32
#endif //ESP8266 or ESP32

//******************************************
//authentication
const char* ssid     = WIFISSID;
const char* password = WIFIPASSWORD;

//******************************************
//connect
void wifiConnect() {

  //check if already conncted to wifi
  if (WiFi.status() == WL_CONNECTED) {
#ifdef VERBOSE
    Serial.print("already connected to wifi ");
    Serial.println(ssid);
#endif //VERBOSE
    return;
  }

  //https://github.com/esp8266/Arduino/issues/2702
  WiFi.disconnect();
  WiFi.begin(ssid, password);

#ifdef VERBOSE
  Serial.println();
  Serial.print("wifi connecting to ");
  Serial.println(ssid);
#endif //VERBOSE

  //try connecting for 20 times with 1 s intervals
  for (int ii = 0; ii < 20; ii++) {

    //status
#ifdef VERBOSE
    Serial.print("status: ");
    switch (WiFi.status()) {
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
        delay(1000);//ms
        break;
      default:
        Serial.println("unknown");
        break;
    }
#endif //VERBOSE

    if (WiFi.status() == WL_CONNECTED) {
#ifdef VERBOSE
      Serial.println();
      Serial.print("wifi connected to ");
      Serial.println(ssid);
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("RSSI: ");
      Serial.println(WiFi.RSSI());
      Serial.println();
#endif //VERBOSE
      break;
    }

    //delay between trials
    delay(1000);//ms
  }

#ifdef VERBOSE
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("could not connect to ");
    Serial.println(ssid);
    Serial.println("abort");
    Serial.println();
  }
#endif //VERBOSE
}
