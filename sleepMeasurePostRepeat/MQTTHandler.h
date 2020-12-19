/*
 * MQTT handling.
 */

#ifndef MQTTHANDLER
#define MQTTHANDLER

//******************************************
//libraries
#include "config.h"
#include "PubSubClient.h"

#ifdef ESP32
#include "WiFi.h" //ESP32
#else
#include "ESP8266WiFi.h" //ESP8266
#endif //ESP32

//******************************************
//random client ID for posting
char clientID[10];
static const char alphanum[] = "0123456789"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";

//******************************************
//connect to MQTT broker
void MQTTConnect(PubSubClient &mqttclient, const char* mqttserver, unsigned int mqttport, const char* mqttusername, const char* mqttpassword) {

  //------------------------------------------
  //check if already conncted to MQTT server
  if (mqttclient.connected()) {
#ifdef VERBOSE
    Serial.print("already connected to MQTT server ");
    Serial.println(mqttserver);
#endif //VERBOSE
    return;
  }

  //------------------------------------------
  //print status
#ifdef VERBOSE
  Serial.print("connecting to MQTT server ");
  Serial.print(mqttserver);
  Serial.println("...");
#endif //VERBOSE

  //------------------------------------------
  //try connecting for 10 times with 1 s intervals
  for (int ii = 0; ii < 10; ii++) {

    //------------------------------------------
    //generate random clientID
    for (int i = 0; i < 10; i++) {
      clientID[i] = alphanum[random(51)];
    }

    //https://community.thingspeak.com/forum/esp8266-wi-fi/problem-rc-4-using-library-pubsub/
    clientID[10] = '\0';

#ifdef VERBOSE
    Serial.print("client ID: ");
    Serial.println(clientID);
#endif //VERBOSE

    //------------------------------------------
    //connect
    mqttclient.connect(clientID, mqttusername, mqttpassword);

    //------------------------------------------
    //connection status
    //print to know why the connection failed
    //see http://pubsubclient.knolleary.net/api.html#state for the failure code explanation
#ifdef VERBOSE
    Serial.print("status: ");

    switch (mqttclient.state()) {
      case -4:
        Serial.println("timeout");
        break;
      case -3:
        Serial.println("lost");
        break;
      case -2:
        Serial.println("failed");
        break;
      case -1:
        Serial.println("disconnected");
        break;
      case 0:
        Serial.println("connected");
        break;
      case 1:
        Serial.println("bad protocol");
        break;
      case 2:
        Serial.println("bad client ID");
        break;
      case 3:
        Serial.println("unavailable");
        break;
      case 4:
        Serial.println("bad credentials");
        break;
      case 5:
        Serial.println("unauthorized");
        break;
      default:
        Serial.println("unknown");
        break;
    }
#endif //VERBOSE

    //------------------------------------------
    //print connection info
    if (mqttclient.connected()) {
#ifdef VERBOSE
      Serial.print("MQTT connected to ");
      Serial.println(mqttserver);
#endif //VERBOSE
      break;
    }

    //------------------------------------------
    //delay between trials
    delay(1000);//ms
  }//END connecting loop
  return;
}

//******************************************
//post values from enbled sensors
void postValues(PubSubClient &mqttclient, const char* mqttserver, unsigned int mqttport, const char* mqttusername, const char* mqttpassword, JsonDocument &doc, String mqtttopic) {

  //------------------------------------------
  //convert message JSON to char array
  char message[measureJson(doc) + 1];
  serializeJson(doc, message, measureJson(doc) + 1);

  //------------------------------------------
  //convert topic string to char array
  char topic[mqtttopic.length()];
  mqtttopic.toCharArray(topic, mqtttopic.length() + 1);

  //------------------------------------------
  //print info
#ifdef VERBOSE
  Serial.println();
  Serial.println("JSON: ");
  serializeJsonPretty(doc, Serial);
  Serial.println();
  Serial.print("message: ");
  Serial.println(message);
  Serial.print("message length: ");
  Serial.println(measureJson(doc));
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.print("server: ");
  Serial.println(mqttserver);
  Serial.print("port: ");
  Serial.println(mqttport);
  Serial.print("user: ");
  Serial.println(mqttusername);
  Serial.print("auth: ");
  Serial.println(mqttpassword);
#endif //VERBOSE

#ifdef POST
  //------------------------------------------
  //post data
  wifiConnect();
  if (WiFi.status() == WL_CONNECTED) {

    //------------------------------------------
    //connect to MQTT broker
    MQTTConnect(mqttclient, mqttserver, mqttport, mqttusername, mqttpassword);

    //------------------------------------------
    //publish
    if (mqttclient.connected()) {
      if (mqttclient.publish(topic, message)) {
#ifdef VERBOSE
        Serial.println("success\n");
#endif //VERBOSE
      } else {
#ifdef VERBOSE
        Serial.println("fail\n");
#endif //VERBOSE
      }
    }
  }

  //------------------------------------------
  //disconnect before leaving
#ifndef CAFFEINE
  WiFi.disconnect();
#endif //CAFFEINE

#endif //POST
  return;
}

#endif //MQTTHANDLER
