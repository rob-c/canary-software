//******************************************
//libraries
#include "config.h"
#include "sensor.h"
#include "vector"
#include "memory"
#include "AsyncDelay.h"

//wifi and MQTT
#if defined(POST) or defined(VERBOSE)
#include "WiFiHandler.h"
#include "MQTTHandler.h"
#include "PubSubClient.h"
#ifdef ESP32
#include "WiFi.h" //ESP32
#else
#include "ESP8266WiFi.h" //ESP8266
#endif //ESP32
#endif //POST or VERBOSE

//SHTxx
#if defined(SHT35A) or defined(SHT35B) or defined(SHT85)
#include "SHTxxSensor.h"
#endif //SHTxx

//MAX31865
#ifdef MAX31865
#include "MAX31865Sensor.h"
#endif //MAX31865

//SPS30
#ifdef SPS30
#include "SPS30Sensor.h"
#endif //SPS30

//ADS1x15
#if defined(ADS1115) or defined(ADS1015)
#include "ADS1x15Sensor.h"
#endif //ADS1015 or ADS1115

//******************************************
//MQTT
#if defined(POST) or defined(VERBOSE)
WiFiClient wificlient;
PubSubClient mqttclient(wificlient);
const char* mqttserver = MQTTSERVER;
long mqttport = MQTTPORT;
const char* mqttusername = MQTTUSERNAME;
const char* mqttpassword = MQTTPASSWORD;
String topicString = MQTTTOPIC;
#endif //POST or VERBOSE

//******************************************
//create a vector of sensors
std::vector<std::unique_ptr<Sensor>> sensors;

//******************************************
//JSON documents
StaticJsonDocument<MQTTMESSAGESIZE> masterdoc;
StaticJsonDocument<160> sensordoc;

//******************************************
//loop time intervals
AsyncDelay sleepTime; //data measurement and posting
AsyncDelay integrationTime; //measurement average
AsyncDelay MQTTTime; //MQTT broker check-in

//******************************************
//setup
void setup() {

  //------------------------------------------
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("\nCanary firmware v. 0.0\n");

  //------------------------------------------
  //MQTT setup
#ifdef POST
  wifiConnect();
  mqttclient.setServer(mqttserver, mqttport);
  mqttclient.setBufferSize(MQTTMESSAGESIZE);
#endif //POST
#ifdef INSTITUTE
  topicString += "/";
  topicString += INSTITUTE;
#endif //INSTITUTE

  //------------------------------------------
  //add sensors to the vector

  //SHT35A
#ifdef SHT35A
  sensors.emplace_back(new SHTxxSensor(SHTxxSensor::sht35a));
#endif //SHT35A

  //SHT35B
#ifdef SHT35B
  sensors.emplace_back(new SHTxxSensor(SHTxxSensor::sht35b));
#endif //SHT35B

  //SHT85
#ifdef SHT85
  sensors.emplace_back(new SHTxxSensor(SHTxxSensor::sht85));
#endif //SHT85

  //MAX31865
#ifdef MAX31865
  sensors.emplace_back(new MAX31865Sensor(MAX31865RNOM, MAX31865RREF, MAX31865CS));
#endif //MAX31865

  //SPS30
#ifdef SPS30
  sensors.emplace_back(new SPS30Sensor(SPS30AVERAGE));
#endif //SPS30

  //ADS1015 or ADS1115
#if defined(ADS1115) or defined(ADS1015)
  sensors.emplace_back(new ADS1x15Sensor(ADS1x15VDD, ADS1x15VREF));
#endif //ADS1015 or ADS1115
    
  //------------------------------------------
  //initialize sensors
  for (auto&& sensor : sensors) {
      sensor->init();
  }

  //------------------------------------------
  //print list of sensors
  Serial.println("available sensors:");
  for (auto&& sensor : sensors) {
      Serial.print("\t");
      Serial.println(sensor->getName());
  }
  Serial.println();

  //------------------------------------------
  //print measurement names and units
#ifdef PRINTSERIAL
  for (auto&& sensor : sensors) {
      Serial.print(sensor->getSensorString());
  }
  Serial.println();
#endif //PRINTSERIAL
  
  //------------------------------------------
  //timing
  sleepTime.start(SLEEPTIME*1e3, AsyncDelay::MILLIS);
  integrationTime.start(INTEGRATIONTIME*1e3, AsyncDelay::MILLIS);
  MQTTTime.start(MQTTTIME*1e3, AsyncDelay::MILLIS);
  
} //setup()

//******************************************
//loop
void loop() {

  //------------------------------------------
  ////integrate sensor measurements (if integration is enabled)
  if (integrationTime.isExpired()) {

    for (auto&& sensor : sensors) {
      sensor->integrate();
    }
    
    integrationTime.repeat();
  } //integration loop

  //------------------------------------------
  //data measurement and posting
  if (sleepTime.isExpired()) {

    //------------------------------------------
    //read data
    for (auto&& sensor : sensors) {
      sensor->readData();
    }
    
    //------------------------------------------
    //print measurements
#ifdef PRINTSERIAL
    for (auto&& sensor : sensors) {
      Serial.print(sensor->getMeasurementsString());
    }
    Serial.println();
#endif //PRINTSERIAL
    
    //------------------------------------------
    //measurements JSON
    masterdoc.clear();
    for (auto&& sensor : sensors) {
      sensor->getJSONDoc(sensordoc);
      masterdoc.add(sensordoc);
    }

    //------------------------------------------
    //post values
#if defined(POST) or defined(VERBOSE)
    postValues(mqttclient, mqttserver, mqttport, mqttusername, mqttpassword, masterdoc, topicString);
#endif //POST or VERBOSE

    //------------------------------------------
    //restart delay from when it expired
    sleepTime.repeat();
  } //data measurement and posting

  //------------------------------------------
  //MQTT check-in loop
#ifdef POST
  if (MQTTTime.isExpired()) {
    mqttclient.loop();
    sleepTime.repeat();
  } //MQTT check-in loop
#endif //POST

} //loop()
