#include "BMP3xxSensor.h"

#define SEALEVELPRESSUREHPA 1013.25

//******************************************
//BMP3xx constructor
BMP3xxSensor::BMP3xxSensor(String name, uint8_t address) {
  _name = name;
  _address = address;
}

//******************************************
//initialize BMP3xx sensor
int BMP3xxSensor::init() {
  if (not _bmp.begin_I2C(_address)) {
    Serial.println("BMP3xx: begin error");
    return 1;
  }

  _bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  _bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  _bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  _bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  //read values a first time
  readData();
  
  return 0;
}

//******************************************
//read data from the sensor
void BMP3xxSensor::readData() {
  if (not _bmp.performReading()) {
    Serial.println("failed to read BMP3xx");
    return;
  }
  _temp = _bmp.temperature; //C
  _pressure = _bmp.pressure; //Pa
  _altitude = _bmp.readAltitude(SEALEVELPRESSUREHPA); //m
}

//******************************************
//get label string for measurement values
String BMP3xxSensor::getSensorString() {
  return "t[C]   P[Pa]    alt.[m]  ";
}

//******************************************
//get string with measurement values
String BMP3xxSensor::getMeasurementsString(void) {
  String s;
  s += String(_temp); //temperature
  s += ("  ");
  s += String(_pressure); //pressure
  s += ("  ");
  s += String(_altitude); //altitude
  s += ("  ");
  return s;
}

//******************************************
//get JSON doc with measurement values
void BMP3xxSensor::getJSONDoc(JsonDocument &doc) {

  //------------------------------------------
  //clear
  doc.clear();

  //------------------------------------------
  //temperature
  if ( ! isnan(_temp)) doc["temp"] = _temp;
  else doc["temp"] = "\"NaN\"";

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
