/*
 * BME280 sensor class.
 * This inherits from the generic TRH sensor class.
 */

#ifndef BME280SENSOR
#define BME280SENSOR

//******************************************
#include "TRHSensor.h"
#include "Adafruit_BME280.h"

//******************************************
class BME280Sensor: public virtual TRHSensor {

  //------------------------------------------
  public:
    BME280Sensor(uint8_t address);
    int init() override;
    void readData() override;
    String getSensorString(void) override;
    String getMeasurementsString(void) override;
    void getJSONDoc(JsonDocument &doc) override;

  //------------------------------------------
  private:
    Adafruit_BME280 _bme;
    uint8_t _address = 0x77;
    float _pressure = std::numeric_limits<float>::quiet_NaN();//pressure [Pa]
    float _altitude = std::numeric_limits<float>::quiet_NaN();//altitude [m]
};
#endif //BME280SENSOR
