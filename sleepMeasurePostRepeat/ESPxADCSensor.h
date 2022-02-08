/*
 * ESP8266 and ESP32 ADC template class.
 * This template class supports ESP8266 and ESP32.
 * This inheriths from the generic sensor class.
 */
 
#ifndef ESPXADCSENSOR
#define ESPXADCSENSOR

//******************************************
#include "sensor.h"
#include "vector"

//******************************************
template <uint8_t N, uint8_t C>
class ESPxADCSensor: public virtual Sensor {

  //------------------------------------------
  public:
  ESPxADCSensor(String name, float vdd, float vref, uint8_t nreadings);
    int init() override;
    void readData() override;
    String getSensorString(void) override;
    String getMeasurementsString(void) override;
    void getJSONDoc(JsonDocument &doc) override;
    uint16_t getADC(uint8_t channel);
    float getTNTC(uint8_t channel);
    void setADCChannel(uint8_t channel, uint8_t pin, bool adc, bool ntc, float rdiv, float t0, float b, float r0);
    void setNTCThreshold(uint8_t channel, float threshold = std::numeric_limits<float>::quiet_NaN());
#ifdef ESP32
    void setAttenuation(adc_attenuation_t attenuation);
#endif //ESP32
    
  //------------------------------------------
  private:
    uint8_t _nbits = N-1;//NOTE n-1 ADC bits since one bit is for the sign
    float _vdd = std::numeric_limits<float>::quiet_NaN();//VDD [V]
    float _vref = std::numeric_limits<float>::quiet_NaN();//VREF [V]
    uint8_t _nreadings = 1;
#ifdef ESP32
    adc_attenuation_t _attenuation;
#endif //ESP32
    
    std::vector<bool> _adcenabled;
    std::vector<bool> _ntcenabled;
    std::vector<uint8_t> _pin;//ADC channel pin []
    std::vector<uint16_t> _adc;//ADC []
    std::vector<float> _tntc;//NTC [C]
    std::vector<float> _rdiv;//RDIV [Ohm]
    std::vector<float> _t0;//T0 [K]
    std::vector<float> _b;//B []
    std::vector<float> _r0;//R0 [Ohm]
    std::vector<float> _tntcthreshold;//NTC lower threshold [C]
    
};

#endif //ESPXADCSENSOR

//******************************************
//ESPxADC constructor
template <uint8_t N, uint8_t C>
  ESPxADCSensor<N, C>::ESPxADCSensor(String name, float vdd, float vref, uint8_t nreadings):
  _adcenabled (C, false),
  _ntcenabled (C, false),
  _pin (C, std::numeric_limits<uint8_t>::quiet_NaN()),
  _adc (C, std::numeric_limits<uint16_t>::quiet_NaN()),
  _tntc (C, std::numeric_limits<float>::quiet_NaN()),
  _rdiv (C, std::numeric_limits<float>::quiet_NaN()),
  _t0 (C, std::numeric_limits<float>::quiet_NaN()),
  _b (C, std::numeric_limits<float>::quiet_NaN()),
  _r0 (C, std::numeric_limits<float>::quiet_NaN()),
  _tntcthreshold (C, std::numeric_limits<float>::quiet_NaN())
{
  _name = name;
  _vdd = vdd;
  _vref = vref;
  _nreadings = nreadings;
  return;
}

//******************************************
//initialize ESPxADC sensor
template <uint8_t N, uint8_t C>
int ESPxADCSensor<N, C>::init() {

#ifdef ESP32
  analogReadResolution(N);
  analogSetWidth(N);
  analogSetAttenuation(_attenuation);
#endif

  return 0;
}

//******************************************
//setup ADC channel parameters
template <uint8_t N, uint8_t C>
void ESPxADCSensor<N, C>::setADCChannel(uint8_t channel, uint8_t pin, bool adc, bool ntc, float rdiv, float t0, float b, float r0) {
  if (channel < _adc.size()) {
    _pin[channel] = pin;
    _adcenabled[channel] = adc;
    _ntcenabled[channel] = ntc;
    _rdiv[channel] = rdiv;
    _t0[channel] = t0;
    _b[channel] = b;
    _r0[channel] = r0;
  }
  return;
}

//******************************************
//setup ADC attenuation
//NOTE this is used by the ESP32 ADC
#ifdef ESP32
template <uint8_t N, uint8_t C>
void ESPxADCSensor<N, C>::setAttenuation(adc_attenuation_t attenuation) {
  _attenuation = attenuation;
  return;
}
#endif //ESP32

//******************************************
//setup NTC channel threshold
template <uint8_t N, uint8_t C>
void ESPxADCSensor<N, C>::setNTCThreshold(uint8_t channel, float threshold) {
  if (channel < _adc.size())
    _tntcthreshold[channel] = threshold;
  return;
}

//******************************************
//read data from the sensor
template <uint8_t N, uint8_t C>
void ESPxADCSensor<N, C>::readData() {

  //loop over ADC channels
  for (int ii = 0; ii < _adc.size(); ii++) {

    //ADC
    if (_adcenabled[ii] || _ntcenabled[ii]) {
      _adc[ii] = getADC(ii);
    }

    //NTC
    if (_ntcenabled[ii]) {

      float tntc = getTNTC(ii);

      //check if the temperature is above the threshold
      if ( ! isnan(_tntcthreshold[ii])) {
	if ( ! isnan(tntc) && tntc > _tntcthreshold[ii]) _tntc[ii] = tntc;
	else _tntc[ii] = std::numeric_limits<float>::quiet_NaN();
      } else {
	_tntc[ii] = tntc;
      }

    }
    
  }
  
  return;
}

//******************************************
//get ADC value
template <uint8_t N, uint8_t C>
uint16_t ESPxADCSensor<N, C>::getADC(uint8_t channel) {

  uint16_t value = std::numeric_limits<uint16_t>::quiet_NaN();

  //read ADC value multiple times
  for (int ii = 0; ii < _nreadings; ii++) {
    value += analogRead(_pin[channel]);
    delay(10);//ms
  }

  //return average
  if (! isnan(value)) return value / _nreadings;
  else return std::numeric_limits<uint16_t>::quiet_NaN();
}
  
//******************************************
//compute NTC temperature from ADC value
template <uint8_t N, uint8_t C>
float ESPxADCSensor<N, C>::getTNTC(uint8_t channel) {
  if (! isnan(_adc[channel])) {
    float _rntc = _rdiv[channel] / ((pow(2, _nbits) - 1.) / _adc[channel] * _vdd / _vref - 1.); //Ohm
    return (1. / ((1. / _t0[channel]) + (1. / _b[channel]) * log((_rntc / _r0[channel]))) - 273.15); //C
  }
  else return std::numeric_limits<float>::quiet_NaN();
}

//******************************************
//get label string for measurement values
template <uint8_t N, uint8_t C>
String ESPxADCSensor<N, C>::getSensorString(void) {
  String s;

  //loop over ADC channels
  for (int ii = 0; ii < _adc.size(); ii++) {

    //ADC
    if (_adcenabled[ii]) {
      s += ("ADC");
      s += String(ii);
      s += ("  ");
    }

    //NTC
    if (_ntcenabled[ii]) {
      s += ("NTC");
      s += String(ii);
      s += ("[C]  ");
    }

  }
  
  return s;
}

//******************************************
//get string with measurement values
template <uint8_t N, uint8_t C>
String ESPxADCSensor<N, C>::getMeasurementsString(void) {
  String s;

  //loop over ADC channels
  for (int ii = 0; ii < _adc.size(); ii++) {

    //ADC
    if (_adcenabled[ii]) {
      s += String(_adc[ii]);
      s += ("  ");
    }

    //NTC
    if (_ntcenabled[ii]) {
      s += String(_tntc[ii]);
      s += ("  ");
    }

  }

  return s;
}

//******************************************
//get JSON doc with measurement values
template <uint8_t N, uint8_t C>
void ESPxADCSensor<N, C>::getJSONDoc(JsonDocument &doc) {

  //------------------------------------------
  //clear
  doc.clear();

  //------------------------------------------
  //ADC
  for (int ii = 0; ii < _adc.size(); ii++) {
    if (_adcenabled[ii]) {
      if ( ! isnan(_adc[ii])) doc["ADC"+String(ii)] = _adc[ii];
      else doc["ADC"+String(ii)] = "\"NaN\"";
    }
  }
  
  //------------------------------------------
  //NTC
  for (int ii = 0; ii < _adc.size(); ii++) {
    if (_ntcenabled[ii]) {

      if ( ! isnan(_tntcthreshold[ii])) {
	
	if ( ! isnan(_tntc[ii]) && _tntc[ii] > _tntcthreshold[ii])
	  doc["NTC"+String(ii)] = _tntc[ii];
	else doc["NTC"+String(ii)] = "\"NaN\"";

      } else{

	if ( ! isnan(_tntc[ii]))
	  doc["NTC"+String(ii)] = _tntc[ii];
	else doc["NTC"+String(ii)] = "\"NaN\"";
	
      }
    }
  }
  
  //------------------------------------------
  //sensor name
  doc["sensor"] = _name;
  
  return;
}
