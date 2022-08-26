/*
 * BMP3xx sensor class.
 * This inherits from the generic TRH sensor class.
 */

#ifndef BMP3XXSENSOR
#define BMP3XXSENSOR

//******************************************
#include "TRHSensor.h"
#include "Adafruit_BMP3XX.h"

//******************************************
class BMP3xxSensor: public virtual TRHSensor {

  //------------------------------------------
  public:
    BMP3xxSensor(String name, uint8_t address);
    int init() override;
    void readData() override;
    String getSensorString(void) override;
    String getMeasurementsString(void) override;
    void getJSONDoc(JsonDocument &doc) override;

  //------------------------------------------
  private:
    Adafruit_BMP3XX _bmp;
    uint8_t _address = 0x77;
    float _pressure = std::numeric_limits<float>::quiet_NaN();//pressure [Pa]
    float _altitude = std::numeric_limits<float>::quiet_NaN();//altitude [m]
};
#endif //BMP3XXSENSOR
