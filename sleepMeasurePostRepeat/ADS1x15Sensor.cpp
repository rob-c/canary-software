#include "ADS1x15Sensor.h"

//******************************************
//ADS1x15 constructor
ADS1x15Sensor::ADS1x15Sensor(float vdd, float vref):
  _adc (4, std::numeric_limits<float>::quiet_NaN()),
  _tntc (4, std::numeric_limits<float>::quiet_NaN()),
  _rdiv (4, std::numeric_limits<float>::quiet_NaN()),
  _t0 (4, std::numeric_limits<float>::quiet_NaN()),
  _b (4, std::numeric_limits<float>::quiet_NaN()),
  _r0 (4, std::numeric_limits<float>::quiet_NaN())
  {
  _name = "ADS1115";
  _vdd = vdd;
  _vref = vref;

  #ifdef ADS1x150NTC
  setADCChannel(0, ADS1x150RDIV, ADS1x150T0, ADS1x150B, ADS1x150R0);
#endif //ADS1x150NTC

#ifdef ADS1x151NTC
  setADCChannel(1, ADS1x151RDIV, ADS1x151T0, ADS1x151B, ADS1x151R0);
#endif //ADS1x151NTC

#ifdef ADS1x152NTC
  setADCChannel(2, ADS1x152RDIV, ADS1x152T0, ADS1x152B, ADS1x152R0);
#endif //ADS1x152NTC

#ifdef ADS1x153NTC
  setADCChannel(3, ADS1x153RDIV, ADS1x153T0, ADS1x153B, ADS1x153R0);
#endif //ADS1x153NTC
}

//******************************************
//setup ADC channel parameters
void ADS1x15Sensor::setADCChannel(uint8_t channel, float rdiv, float t0, float b, float r0) {
  if (channel < _adc.size()) {
    _rdiv[channel] = rdiv;
    _t0[channel] = t0;
    _b[channel] = b;
    _r0[channel] = r0;
  }
  return;
}

//******************************************
//initialize ADS1x15 sensor
int ADS1x15Sensor::init() {

  //https://github.com/adafruit/Adafruit_ADS1X15/blob/master/examples/singleended/singleended.ino
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  _ads1x15.setGain(GAIN_ONE);
  _ads1x15.begin();
  return 0;
}

//******************************************
//read data from the sensor
void ADS1x15Sensor::readData() {

  //------------------------------------------
  //ADC
#if defined(ADS1x150ADC) or defined(ADS1x150NTC)
  _adc[0] = _ads1x15.readADC_SingleEnded(0); //ADS1x15 ADC0
#endif //ADS1x150ADC or ADS1x150NTC

#if defined(ADS1x151ADC) or defined(ADS1x151NTC)
  _adc[1] = _ads1x15.readADC_SingleEnded(1); //ADS1x15 ADC1
#endif //ADS1x151ADC or ADS1x151NTC

#if defined(ADS1x152ADC) or defined(ADS1x152NTC)
  _adc[2] = _ads1x15.readADC_SingleEnded(2); //ADS1x15 ADC2
#endif //ADS1x152ADC or ADS1x152NTC

#if defined(ADS1x153ADC) or defined(ADS1x153NTC)
  _adc[3] = _ads1x15.readADC_SingleEnded(3); //ADS1x15 ADC3
#endif //ADS1x153ADC or ADS1x153NTC

  //------------------------------------------
  //NTC temperature
#ifdef ADS1x150NTC
  _tntc[0] = getTNTC(0);
#endif //ADS1x150NTC

#ifdef ADS1x151NTC
  _tntc[1] = getTNTC(1);
#endif //ADS1x151NTC

#ifdef ADS1x152NTC
  _tntc[2] = getTNTC(2);
#endif //ADS1x152NTC

#ifdef ADS1x153NTC
  _tntc[3] = getTNTC(3);
#endif //ADS1x153NTC

  return;
}

//******************************************
//compute NTC temperature from ADC value
float ADS1x15Sensor::getTNTC(uint8_t channel) {
  if (! isnan(_adc[channel])) {
    float _rntc = _rdiv[channel] / ((pow(2, nbits) - 1.) / _adc[channel] * _vdd / _vref - 1.); //Ohm
    return (1. / ((1. / _t0[channel]) + (1. / _b[channel]) * log((_rntc / _r0[channel]))) - 273.15); //C
  }
  else return std::numeric_limits<float>::quiet_NaN();
}

//******************************************
//get string with measurement values
String ADS1x15Sensor::getSensorString(void) {
  String s;

  //------------------------------------------
#ifdef ADS1x150ADC
  s += String("ADC0  ");
#endif //ADS1x150ADC
  
#ifdef ADS1x151ADC
  s += String("ADC1  ");
#endif //ADS1x151ADC

#ifdef ADS1x152ADC
  s += String("ADC2  ");
#endif //ADS1x152ADC

#ifdef ADS1x153ADC
  s += String("ADC3  ");
#endif //ADS1x153ADC

  //------------------------------------------
#ifdef ADS1x150NTC
  s += String("NTC0[C]  ");
#endif //ADS1x150NTC

#ifdef ADS1x151NTC
  s += String("NTC1[C]  ");
#endif //ADS1x151NTC

#ifdef ADS1x152NTC
  s += String("NTC2[C]  ");
#endif //ADS1x152NTC

#ifdef ADS1x153NTC
  s += String("NTC3[C]  ");
#endif //ADS1x153NTC

  return s;
}  

//******************************************
//get string with measurement values
String ADS1x15Sensor::getMeasurementsString(void) {
  String s;

  //------------------------------------------
#ifdef ADS1x150ADC
  s += String(_adc[0]);//ADS1x150ADC
  s += ("  ");
#endif //ADS1x150ADC

#ifdef ADS1x151ADC
  s += String(_adc[1]);//ADS1x151ADC
  s += ("  ");
#endif //ADS1x151ADC

#ifdef ADS1x152ADC
  s += String(_adc[2]);//ADS1x152ADC
  s += ("  ");
#endif //ADS1x152ADC

#ifdef ADS1x153ADC
  s += String(_adc[3]);//ADS1x153ADC
  s += ("  ");
#endif //ADS1x153ADC

  //------------------------------------------
#ifdef ADS1x150NTC
  s += String(_tntc[0]);//ADS1x150ADC
  s += ("  ");
#endif //ADS1x150NTC

#ifdef ADS1x151NTC
  s += String(_tntc[1]);//ADS1x151ADC
  s += ("  ");
#endif //ADS1x151NTC

#ifdef ADS1x152NTC
  s += String(_tntc[2]);//ADS1x152ADC
  s += ("  ");
#endif //ADS1x152NTC

#ifdef ADS1x153NTC
  s += String(_tntc[3]);//ADS1x153ADC
  s += ("  ");
#endif //ADS1x153NTC

  return s;
}

//******************************************
//get JSON doc with measurement values
void ADS1x15Sensor::getJSONDoc(JsonDocument &doc) {

  //------------------------------------------
  //clear
  doc.clear();

  //------------------------------------------
  //channel 0
#ifdef ADS1x150ADC
  if ( ! isnan(_adc[0])) doc["ADC0"] = _adc[0];
  else doc["ADC0"] = "\"NaN\"";
#endif //ADS1x150ADC
#ifdef ADS1x150NTC
  if ( ! isnan(_tntc[0])) doc["NTC0"] = _tntc[0];
  else doc["NTC0"] = "\"NaN\"";
#endif //ADS1x150NTC

  //------------------------------------------
  //channel 1
#ifdef ADS1x151ADC
  if ( ! isnan(_adc[1])) doc["ADC1"] = _adc[1];
  else doc["ADC1"] = "\"NaN\"";
#endif //ADS1x151ADC
#ifdef ADS1x151NTC
  if ( ! isnan(_tntc[1])) doc["NTC1"] = _tntc[1];
  else doc["NTC1"] = "\"NaN\"";
#endif //ADS1x151NTC

  //------------------------------------------
  //channel 2
#ifdef ADS1x152ADC
  if ( ! isnan(_adc[2])) doc["ADC2"] = _adc[2];
  else doc["ADC2"] = "\"NaN\"";
#endif //ADS1x152ADC
#ifdef ADS1x152NTC
  if ( ! isnan(_tntc[2])) doc["NTC2"] = _tntc[2];
  else doc["NTC2"] = "\"NaN\"";
#endif //ADS1x152NTC

  //------------------------------------------
  //channel 3
#ifdef ADS1x150ADC
  if ( ! isnan(_adc[3])) doc["ADC3"] = _adc[3];
  else doc["ADC3"] = "\"NaN\"";
#endif //ADS1x153ADC
#ifdef ADS1x153NTC
  if ( ! isnan(_tntc[3])) doc["NTC3"] = _tntc[3];
  else doc["NTC3"] = "\"NaN\"";
#endif //ADS1x153NTC

  //------------------------------------------
  //sensor name
  doc["sensor"] = _name;
  
  return;
}
