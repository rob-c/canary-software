#include "SPS30Sensor.h"

//******************************************
//SPS30 constructor
SPS30Sensor::SPS30Sensor(bool average) {
  _name = "SPS30";
  _average = average;
  _integral = 0;
  _counter = 0;
  return;
}

//******************************************
//initialize SPS30 sensor
int SPS30Sensor::init(void) {

  //initialize I2C communication
  sensirion_i2c_init();
    
    //probe SPS30
  for (int ii = 0; ii < 10; ii++) {
    if (sps30_probe() != 0) {
#ifdef VERBOSE
      Serial.println("SPS30 sensor probing failed");
#endif //VERBOSE
    } else {
      _sps30available = true;
      break;
    }
    //wait 500 ms between trials
    delay(500); //ms
  }

  //SPS30 serial number
#ifdef VERBOSE
  if (_sps30available) {
    _ret = sps30_get_serial(_serial);
    if (_ret) {
      Serial.println("could not retrieve SPS30 serial number");
    } else {
      Serial.print("SPS30 serial number: ");
      Serial.println(_serial);
    }
  }
#endif //VERBOSE

  //set auto-cleaning
  if (_sps30available) {
    _ret = sps30_set_fan_auto_cleaning_interval_days(_auto_clean_days);
    if (_ret) {
#ifdef VERBOSE
      Serial.print("SPS30 error setting the auto-clean interval: ");
      Serial.println(_ret);
#endif //VERBOSE
    }
  }

  //start measurement
  if (_sps30available) {
    _ret = sps30_start_measurement();
    if (_ret < 0) {
#ifdef VERBOSE
      Serial.print("SPS30 error starting measurement\n");
#endif //verbose
      _sps30available = false;
    }
  }

  //I2C buffer size
#ifdef SPS30_LIMITED_I2C_BUFFER_SIZE
  Serial.println("The hardware has a limitation that only");
  Serial.println("  allows reading the SPS30 mass concentrations.");
  Serial.println("  For more information, please check:");
  Serial.println("  https://github.com/Sensirion/arduino-sps#esp8266-partial-legacy-support\n");
#endif //SPS30_LIMITED_I2C_BUFFER_SIZE

  _dustnc = std::numeric_limits<float>::quiet_NaN();
  return 0;
}

//******************************************
//simply read data from the sensor
float SPS30Sensor::simplyRead(void) {
  
  if (!_sps30available) return std::numeric_limits<float>::quiet_NaN();

  //try to read data from SPS30
  for (int ii = 0; ii < 10; ii++) {
    _ret = sps30_read_data_ready(&_data_ready);
    if (_ret < 0) {
#ifdef VERBOSE
      Serial.print("SPS30 error reading data-ready flag: ");
      Serial.println(_ret);
#endif //VERBOSE
    } else if (!_data_ready) {
#ifdef VERBOSE
      Serial.println("SPS30 data not ready, no new measurement available");
#endif //VERBOSE
    } else {
      break;
    }
    //wait 100 ms between trials
    delay(100);//ms
  };

  //read SPS30 measurement
  _ret = sps30_read_measurement(&_m);
  if (_ret < 0) {
#ifdef VERBOSE
    Serial.println("SPS30 error reading measurement");
#endif //VERBOSE
    return std::numeric_limits<float>::quiet_NaN();
  } else {
    return (_m.nc_10p0 - _m.nc_0p5); //particles/cm^3
  }
}

//******************************************
//integrate measurements to get an average measurement
void SPS30Sensor::integrate(void) {
  if (_average) {
    float value = simplyRead();
    if (!isnan(value)) {
      _integral += value;
      _counter++;
      /*
      Serial.print("value: ");
      Serial.print(value);
      Serial.print("\tintegral: ");
      Serial.print(_integral);
      Serial.print("\tcounter = ");
      Serial.print(_counter);
      Serial.println();
      */
    }
  }
  return;
}

//******************************************
//read data from the sensor
void SPS30Sensor::readData(void) {
  if (_average) {
    _dustnc = _integral / _counter;
    _integral = 0;
    _counter = 0;
  } else {
    _dustnc = simplyRead();
  }
}

//******************************************
//get string with measurement values
String SPS30Sensor::getMeasurementsString(void) {
  String s;
  s += String(_dustnc, 3); //dust concentration (0.5-10 µm)
  s += ("       ");
  return s;
}

//******************************************
//get JSON doc with measurement values
void SPS30Sensor::getJSONDoc(JsonDocument &doc) {

  //clear
  doc.clear();

  //dust concentration
  if ( ! isnan(_dustnc)) doc["dustnc"] = _dustnc;
  else doc["dustnc"] = "\"NaN\"";

  //sensor name
  doc["sensor"] = _name;
  
  return;
}
