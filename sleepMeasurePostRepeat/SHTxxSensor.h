#ifndef SHTXXSENSOR
#define SHTXXSENSOR

#include "TRHSensor.h"
#include "Adafruit_SHT31.h"

class SHTxxSensor: public virtual TRHSensor {
  
  public:
    enum model {sht35a, sht35b, sht85};
    SHTxxSensor(model model);
    int init(void) override;
    void readData(void) override;
  
  private:
    Adafruit_SHT31 _sht;
    model _model;
    uint8_t _address;
};
#endif //SHTXXSENSOR
