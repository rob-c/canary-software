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
  SHTxxSensor* sht35a = new SHTxxSensor(SHTxxSensor::sht35a);
  sensors.emplace_back(sht35a);
#endif //SHT35A

  //SHT35B
#ifdef SHT35B
  SHTxxSensor* sht35a = new SHTxxSensor(SHTxxSensor::sht35a);
  sensors.emplace_back(sht35a);
#endif //SHT35B

  //SHT85
#ifdef SHT85
  SHTxxSensor* sht85 = new SHTxxSensor(SHTxxSensor::sht85);
  sensors.emplace_back(sht85);
#endif //SHT85

  //MAX31865
#ifdef MAX31865
#ifdef MAX31865RHSOURCE
  MAX31865Sensor* max31865 = new MAX31865Sensor(MAX31865RNOM, MAX31865RREF, MAX31865CS, MAX31865RHSOURCE);
#else
  MAX31865Sensor* max31865 = new MAX31865Sensor(MAX31865RNOM, MAX31865RREF, MAX31865CS);
#endif //MAX31865RHSOURCE
  sensors.emplace_back(max31865);
#endif //MAX31865

  //SPS30
#ifdef SPS30
  SPS30Sensor* sps30 = new SPS30Sensor(SPS30AVERAGE);
  sensors.emplace_back(sps30);
#endif //SPS30

  //ADS1015 or ADS1115
#if defined(ADS1115) or defined(ADS1015)
  ADS1x15Sensor* ads1x15 = new ADS1x15Sensor(ADS1x15VDD, ADS1x15VREF);
  sensors.emplace_back(ads1x15);
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
  //read, print and post values before going to sleep
  //NOTE this is meant for battery operation (no caffeine)
#ifndef CAFFEINE
  for (auto&& sensor : sensors) {
    sensor->integrate();
  }
  readPrintPost();
  Serial.println("now going to sleep zzz...");
  ESP.deepSleep(SLEEPTIME * 1e6); //µs
#endif //CAFFEINE
  
  //------------------------------------------
  //timing
#ifdef CAFFEINE
  sleepTime.start(SLEEPTIME*1e3, AsyncDelay::MILLIS);
  integrationTime.start(INTEGRATIONTIME*1e3, AsyncDelay::MILLIS);
  MQTTTime.start(MQTTTIME*1e3, AsyncDelay::MILLIS);
#endif //CAFFEINE
  
} //setup()

//******************************************
//loop
void loop() {

  //------------------------------------------
  ////integrate sensor measurements (if averaging is enabled)
  if (integrationTime.isExpired()) {
    for (auto&& sensor : sensors) {
      sensor->integrate();
    }
    integrationTime.repeat();
  }

  //------------------------------------------
  //read, print and post measurements
  if (sleepTime.isExpired()) {
    readPrintPost();
    sleepTime.repeat();
  }

  //------------------------------------------
  //MQTT check-in loop
#ifdef POST
  if (MQTTTime.isExpired()) {
    mqttclient.loop();
    MQTTTime.repeat();
  }
#endif //POST

} //loop()

//******************************************
//read, print and post measurements
//NOTE integration and averaging is done separately
void readPrintPost() {

    //------------------------------------------
    //read measurements
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
    //post measurements
    masterdoc.clear();
    for (auto&& sensor : sensors) {
      sensor->getJSONDoc(sensordoc);
      masterdoc.add(sensordoc);
    }
#if defined(POST) or defined(VERBOSE)
    postValues(mqttclient, mqttserver, mqttport, mqttusername, mqttpassword, masterdoc, topicString);
#endif //POST or VERBOSE
}
