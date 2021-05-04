#include "SHTxxSensor.h"

//******************************************
//SHTxx constructor
SHTxxSensor::SHTxxSensor(model model) {
  _model = model;
  switch (_model) {
    case sht35a:
      _name = "SHT35A";
      _address = 0x44;
      break;
    case sht35b:
      _name = "SHT35B";
      _address = 0x45;
      break;
    case sht85:
      _name = "SHT85";
      _address = 0x44;
      break;
    default:
      Serial.print("SHTxx: sensor model \"");
      Serial.print(_model);
      Serial.println("\" not supported");
  }
}

//******************************************
//initialize SHTxx sensor
int SHTxxSensor::init() {
  _sht = Adafruit_SHT31();
  if (not _sht.begin(_address)) {
    Serial.println("SHTxx: begin error");
    return 1;
  }  
  return 0;
}

//******************************************
//read data from the sensor
void SHTxxSensor::readData() {
  _temp = _sht.readTemperature();
  _rh = _sht.readHumidity();
  computeDewPoint();
}
