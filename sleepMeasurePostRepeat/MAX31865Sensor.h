/*
 * MAX31865 RTD sensor class.
 * This inherits from the generic TRH sensor class.
 */

#ifndef MAX31865SENSOR
#define MAX31865SENSOR

//******************************************
#include "TRHSensor.h"
#include "Adafruit_MAX31865.h"

//******************************************
class MAX31865Sensor: public virtual TRHSensor {

  //------------------------------------------
  public:
    MAX31865Sensor(float rnom, float rref, uint8_t cs);
    int init() override;
    void readData() override;
    String getSensorString(void) override;
    String getMeasurementsString(void) override;
    void getJSONDoc(JsonDocument &doc) override;
    void setRHSource(TRHSensor* rhsource);

  //------------------------------------------
  private:
    Adafruit_MAX31865 _max31865;
    uint8_t _fault;
    float _rnom;
    float _rref;
    uint8_t _cs;
    TRHSensor* _rhsource = NULL;
};
#endif //MAX31865SENSOR
