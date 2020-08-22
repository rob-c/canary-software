#ifndef ADS1X15SENSOR
#define ADS1X15SENSOR

#include "Sensor.h"
#include "Wire.h"
#include "Adafruit_ADS1015.h"
#include "vector"

class ADS1x15Sensor: public virtual Sensor {
  
  public:
    ADS1x15Sensor(float vdd, float vref);
    int init(void) override;
    void readData() override;
    String getSensorString(void) override;
    String getMeasurementsString(void) override;
    void getJSONDoc(JsonDocument &doc) override;
    //float getTNTC(int adc, float adcrdiv, float ntct0, float ntcb, float ntcr0, float vdd, float vref);
    float getTNTC(uint8_t channel);
    void setADCChannel(uint8_t channel, float rdiv, float t0, float b, float r0);

  private:    
#if defined(ADS1015)
    Adafruit_ADS1015 _ads1x15; //ADS1015 (12 bit)
    uint8_t nbits = 11;//NOTE n-1 ADC bits since one bit is for the sign
#elif defined(ADS1115)
    Adafruit_ADS1115 _ads1x15; //ADS1115 (16 bit)
    uint8_t nbits = 15;//NOTE n-1 ADC bits since one bit is for the sign
#else //ADS1115 as default
    Adafruit_ADS1115 _ads1x15; //ADS1115 (16 bit)
    uint8_t nbits = 15;//NOTE n-1 ADC bits since one bit is for the sign
#endif //ADS1015 or ADS1115

    float _vdd = std::numeric_limits<float>::quiet_NaN();//VDD [V]
    float _vref = std::numeric_limits<float>::quiet_NaN();//VREF [V]

    std::vector<uint16_t> _adc;//ADC []
    std::vector<float> _tntc;//NTC [C]
    std::vector<float> _rdiv;//RDIV [Ohm]
    std::vector<float> _t0;//T0 [K]
    std::vector<float> _b;//B []
    std::vector<float> _r0;//R0 [Ohm]
    
};
#endif //ADS1X15SENSOR
