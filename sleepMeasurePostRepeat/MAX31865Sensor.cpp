#include "MAX31865Sensor.h"

//******************************************
//MAX31865 constructor
MAX31865Sensor::MAX31865Sensor(float rnom, float rref, uint8_t cs, TRHSensor* rhsource):
  _max31865(Adafruit_MAX31865(cs))
  {
  _name = "MAX31865";
  _rnom = rnom;
  _rref = rref;
  _cs = cs;
  _rhsource = rhsource;
  return;
}
  
//******************************************
//initialize MAX31865 sensor
int MAX31865Sensor::init() {
  _max31865.begin(MAX31865_2WIRE); //2-wire configuration
  return 0;
}

//******************************************
//read data from the sensor
void MAX31865Sensor::readData() {

  //------------------------------------------
  //read temperature measurement
  _temp = _max31865.temperature(_rnom, _rref);

  //------------------------------------------
  //check and print any faults
  uint8_t fault = _max31865.readFault();
  if (fault) {
    Serial.print("MAX31865 fault 0x");
    Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD high threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD low threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("under/over voltage"); 
    }
    _max31865.clearFault();
  }

  //------------------------------------------
  //get RH measurement from another sensor
  if (_rhsource) {
    _rh = _rhsource->getRH();
    computeDewPoint();
  }

  return;
}

//******************************************
//get label string for measurement values
String MAX31865Sensor::getSensorString() {
  if (_rhsource) return "t[C]   RH[%]  DP[C]  ";
  else return "t[C]   ";
}

//******************************************
//get string with measurement values
String MAX31865Sensor::getMeasurementsString(void) {
  String s;
  s += String(_temp); //temperature
  s += ("  ");
  if (_rhsource) {
    s += String(_rh); //humidity
    s += ("  ");
    s += String(_dp); //dew point
    s += ("  ");
  }
  return s;
}

//******************************************
//get JSON doc with measurement values
void MAX31865Sensor::getJSONDoc(JsonDocument &doc) {

  //------------------------------------------
  //clear
  doc.clear();

  //------------------------------------------
  //temperature
  if ( ! isnan(_temp)) doc["temp"] = _temp;
  else doc["temp"] = "\"NaN\"";

  //------------------------------------------
  //RH source
  if (_rhsource) {

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
    
  }

  //------------------------------------------
  //sensor name
  doc["sensor"] = _name;
  
  return;
}
