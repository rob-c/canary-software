#include "TRHSensor.h"

//******************************************
//get string with measurement values
String TRHSensor::getMeasurementsString(void) {
  String s;
  s += String(_temp); //temperature
  s += ("  ");
  s += String(_rh); //humidity
  s += ("  ");
  s += String(_dp); //dew point
  s += ("  ");
  return s;
}

//******************************************
//get JSON doc with measurement values
void TRHSensor::getJSONDoc(JsonDocument &doc) {

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
  //sensor name
  doc["sensor"] = _name;

  return;
}

//******************************************
//compute the dew point from the temperature and RH measurements
void TRHSensor::computeDewPoint(void) {
  if (! isnan(_temp) and ! isnan(_rh)) {
    float h = (log10(_rh) - 2) / 0.4343 + (17.62 * _temp) / (243.12 + _temp);
    _dp = 243.12 * h / (17.62 - h);
  }
  else _dp = std::numeric_limits<float>::quiet_NaN();
}
