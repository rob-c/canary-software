#include "BME280Sensor.h"

#define SEALEVELPRESSUREHPA 1013.25

//******************************************
//BME280 constructor
BME280Sensor::BME280Sensor(uint8_t address) {
  _name = "BME280";
  _address = address;
}

//******************************************
//initialize BME280 sensor
int BME280Sensor::init() {
  if (not _bme.begin(_address)) {
    Serial.println("BME280: begin error");
    return 1;
  }

  _bme.setSampling(Adafruit_BME280::MODE_NORMAL,      //mode
		   Adafruit_BME280::SAMPLING_X16,     //temperature
		   Adafruit_BME280::SAMPLING_X16,     //pressure
		   Adafruit_BME280::SAMPLING_X16,     //humidity
		   Adafruit_BME280::FILTER_OFF,       //filter
		   Adafruit_BME280::STANDBY_MS_0_5 ); //standby period [ms]

  return 0;
}

//******************************************
//read data from the sensor
void BME280Sensor::readData() {

  //check the communication with the sensor
  Wire.beginTransmission(byte(_address));

  //read data
  if (Wire.endTransmission()){
    //Serial.println("failed to read BME280");
    _temp = std::numeric_limits<float>::quiet_NaN();
    _rh = std::numeric_limits<float>::quiet_NaN();
    _pressure = std::numeric_limits<float>::quiet_NaN();
    _altitude = std::numeric_limits<float>::quiet_NaN();
  } else {
    _temp = _bme.readTemperature(); //C
    _rh = _bme.readHumidity(); //%
    _pressure = _bme.readPressure(); //Pa
    _altitude = _bme.readAltitude(SEALEVELPRESSUREHPA); //m
  }
  
  computeDewPoint();
  
  return;
}

//******************************************
//get label string for measurement values
String BME280Sensor::getSensorString() {
  return "t[C]   RH[%]  DP[C]  P[Pa]     alt.[m]  ";
}

//******************************************
//get string with measurement values
String BME280Sensor::getMeasurementsString(void) {
  String s;
  s += String(_temp); //temperature
  s += ("  ");
  s += String(_rh); //humidity
  s += ("  ");
  s += String(_dp); //dew point
  s += ("  ");
  s += String(_pressure); //pressure
  s += ("  ");
  s += String(_altitude); //altitude
  s += ("  ");
  return s;
}

//******************************************
//get JSON doc with measurement values
void BME280Sensor::getJSONDoc(JsonDocument &doc) {

  //------------------------------------------
  //clear
  doc.clear();

  //------------------------------------------
  //temperature
  if ( ! isnan(_temp)) doc["temp"] = _temp;
  else doc["temp"] = "\"NaN\"";

  //------------------------------------------
  //relative humidity
  if ( ! isnan(_rh)) doc["rh"] = _rh;
  else doc["rh"] = "\"NaN\"";

  //------------------------------------------
  //dew point
  if ( ! isnan(_dp)) {
      doc["dewpoint"] = _dp;
      if (_temp > _dp) doc["dewpstatus"] = 1;
      else doc["dewpstatus"] = 0;
  } else {
      doc["dewpoint"] = "\"NaN\"";
      doc["dewpstatus"] = "\"NaN\"";
  }

  //------------------------------------------
  //pressure
  if ( ! isnan(_pressure)) doc["pressure"] = _pressure;
  else doc["pressure"] = "\"NaN\"";

  //------------------------------------------
  //altitude
  if ( ! isnan(_altitude)) doc["altitude"] = _altitude;
  else doc["altitude"] = "\"NaN\"";

  //------------------------------------------
  //sensor name
  doc["sensor"] = _name;
  
  return;
}
