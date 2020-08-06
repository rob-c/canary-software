//******************************************
//libraries
#include "config.h"
#include "sensor.h"
#include "vector"
#include "memory"
#include "AsyncDelay.h"

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

//******************************************
//create a vector of sensors
std::vector<std::unique_ptr<Sensor>> sensors;

//******************************************
//JSON documents
StaticJsonDocument<500> masterdoc;
StaticJsonDocument<160> sensordoc;

//******************************************
//timing
AsyncDelay sleepTime;

//******************************************
//setup
void setup() {

  //------------------------------------------
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println();

  //------------------------------------------
  //declare sensors and add them to the vector

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
  sensors.emplace_back(new SPS30Sensor());
#endif //SPS30
    
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
  for (auto&& sensor : sensors) {
      Serial.print(sensor->getSensorString());
  }
  Serial.println();

  //------------------------------------------
  //timing
  sleepTime.start(SLEEPTIME*1e3, AsyncDelay::MILLIS);
  
} //setup()

//******************************************
//loop
void loop() {

  if (sleepTime.isExpired()) {

    //------------------------------------------
    //read data
    for (auto&& sensor : sensors) {
      sensor->readData();
    }
    
    //------------------------------------------
    //print measurements
    for (auto&& sensor : sensors) {
      Serial.print(sensor->getMeasurementsString());
    }
    Serial.println();
  
    //------------------------------------------
    //print JSON
    /*
    masterdoc.clear();
    for (auto&& sensor : sensors) {
      sensor->getJSONDoc(sensordoc);
      masterdoc.add(sensordoc);
    }
    //serializeJsonPretty(masterdoc, Serial);
    //Serial.println();
    */

    //------------------------------------------
    //restart delay from when it exired
    sleepTime.repeat();
    
  } //sleep time expired
  
} //loop()
