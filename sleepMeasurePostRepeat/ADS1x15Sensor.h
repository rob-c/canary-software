/*
 * ADS1x15 sensor template class.
 * This template class supports Adafruit_ADS1015 and Adafruit_ADS1115.
 * This inheriths from the generic sensor class.
 */
 
#ifndef ADS1X15SENSOR
#define ADS1X15SENSOR

//******************************************
#include "sensor.h"
#include "Wire.h"
#include "Adafruit_ADS1X15.h"
#include "vector"

//******************************************
template <class T, int N>
class ADS1x15Sensor: public virtual Sensor {

  //------------------------------------------
  public:
    ADS1x15Sensor(String name, float vdd, float vref, adsGain_t gain);
    int init() override;
    void readData() override;
    String getSensorString(void) override;
    String getMeasurementsString(void) override;
    void getJSONDoc(JsonDocument &doc) override;
    float getTNTC(uint8_t channel);
    void setADCChannel(uint8_t channel, bool adc, bool ntc, float rdiv, float t0, float b, float r0);
    void setNTCThreshold(uint8_t channel, float threshold = std::numeric_limits<float>::quiet_NaN());
    String getName(void) override {return _name;}
    
  //------------------------------------------
  private:
    T _ads1x15;
    uint8_t nbits = N-1;//NOTE n-1 ADC bits since one bit is for the sign
    String _name = "ADS1x15";//ADS1x15 name
    float _vdd = std::numeric_limits<float>::quiet_NaN();//VDD [V]
    float _vref = std::numeric_limits<float>::quiet_NaN();//VREF [V]
    adsGain_t _gain;//ADS1x15 gain
    
    std::vector<bool> _adcenabled;
    std::vector<bool> _ntcenabled;
    std::vector<uint16_t> _adc;//ADC []
    std::vector<float> _tntc;//NTC [C]
    std::vector<float> _rdiv;//RDIV [Ohm]
    std::vector<float> _t0;//T0 [K]
    std::vector<float> _b;//B []
    std::vector<float> _r0;//R0 [Ohm]
    std::vector<float> _tntcthreshold;//NTC lower threshold [C]
    
};

#endif //ADS1X15SENSOR

//******************************************
//ADS1x15 constructor
template <class T, int N>
ADS1x15Sensor<T,N>::ADS1x15Sensor(String name, float vdd, float vref, adsGain_t gain):
  _adcenabled (4, false),
  _ntcenabled (4, false),
  _adc (4, std::numeric_limits<uint16_t>::quiet_NaN()),
  _tntc (4, std::numeric_limits<float>::quiet_NaN()),
  _rdiv (4, std::numeric_limits<float>::quiet_NaN()),
  _t0 (4, std::numeric_limits<float>::quiet_NaN()),
  _b (4, std::numeric_limits<float>::quiet_NaN()),
  _r0 (4, std::numeric_limits<float>::quiet_NaN()),
  _tntcthreshold (4, std::numeric_limits<float>::quiet_NaN())
{
  _name = name;
  _vdd = vdd;
  _vref = vref;
  _gain = gain;
  return;
}

//******************************************
//setup ADC channel parameters
template <class T, int N>
void ADS1x15Sensor<T,N>::setADCChannel(uint8_t channel, bool adc, bool ntc, float rdiv, float t0, float b, float r0) {
  if (channel < _adc.size()) {
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
//setup NTC channel threshold
template <class T, int N>
void ADS1x15Sensor<T,N>::setNTCThreshold(uint8_t channel, float threshold) {
  if (channel < _adc.size())
    _tntcthreshold[channel] = threshold;
  return;
}

//******************************************
//initialize ADS1x15 sensor
template <class T, int N>
int ADS1x15Sensor<T,N>::init() {

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

  _ads1x15.setGain(_gain);
  _ads1x15.begin();
  return 0;
}

//******************************************
//read data from the sensor
template <class T, int N>
void ADS1x15Sensor<T,N>::readData() {

  //loop over ADC channels
  for (int ii = 0; ii < _adc.size(); ii++) {

    //ADC
    if (_adcenabled[ii] || _ntcenabled[ii]) {
      _adc[ii] = _ads1x15.readADC_SingleEnded(ii);
    }

    //NTC
    if (_ntcenabled[ii]) {

      float tntc = getTNTC(ii);

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
//compute NTC temperature from ADC value
template <class T, int N>
float ADS1x15Sensor<T,N>::getTNTC(uint8_t channel) {
  if (! isnan(_adc[channel])) {
    float _rntc = _rdiv[channel] / ((pow(2, nbits) - 1.) / _adc[channel] * _vdd / _vref - 1.); //Ohm
    return (1. / ((1. / _t0[channel]) + (1. / _b[channel]) * log((_rntc / _r0[channel]))) - 273.15); //C
  }
  else return std::numeric_limits<float>::quiet_NaN();
}

//******************************************
//get label string for measurement values
template <class T, int N>
String ADS1x15Sensor<T,N>::getSensorString(void) {
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
template <class T, int N>
String ADS1x15Sensor<T,N>::getMeasurementsString(void) {
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
template <class T, int N>
void ADS1x15Sensor<T,N>::getJSONDoc(JsonDocument &doc) {

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
