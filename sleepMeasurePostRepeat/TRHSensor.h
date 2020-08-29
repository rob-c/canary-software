/*
 * Generic temperature and relative humidity sensor class.
 * This inheriths from the generic sensor class.
 */

#ifndef TRHSENSOR
#define TRHSENSOR

//******************************************
#include "sensor.h"

//******************************************
class TRHSensor: public virtual Sensor {

  //------------------------------------------
  public:
    TRHSensor(){_name = "TRHGenericSensor";}
    virtual String getSensorString(void) override {return "t[C]   RH[%]  DP[C]  ";}
    virtual String getMeasurementsString(void) override;
    virtual void getJSONDoc(JsonDocument &doc) override;
    virtual float getTemp(void) {return _temp;}
    virtual float getRH(void) {return _rh;}
    virtual float getDewPoint(void) {return _dp;}
    virtual void computeDewPoint(void);

  //------------------------------------------
  protected:
    float _temp = std::numeric_limits<float>::quiet_NaN();//temperature [C]
    float _rh = std::numeric_limits<float>::quiet_NaN();//relative humidity [%]
    float _dp = std::numeric_limits<float>::quiet_NaN();//dew point [C]
};
#endif //TRHSENSOR
