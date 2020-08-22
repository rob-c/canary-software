#ifndef SPS30SENSOR
#define SPS30SENSOR

#include "sensor.h"
#include "sps30.h"

class SPS30Sensor: public virtual Sensor {
  
  public:
    SPS30Sensor(bool average);
    int init(void) override;
    float simplyRead(void);
    void readData(void) override;
    void integrate(void) override;
    float getDustConcentration(void) {return _dustnc;}
    String getSensorString(void) override {return "dust[cm^-3] ";}
    String getMeasurementsString(void) override;
    void getJSONDoc(JsonDocument &doc) override;
    
  private:
    float _dustnc; //dust concentration (0.5-10 µm) [cm^_3]
    int16_t _ret;
    uint8_t _auto_clean_days = 2;
    uint32_t _auto_clean;
    bool _sps30available = false;
    char _serial[SPS30_MAX_SERIAL_LEN];
    struct sps30_measurement _m;
    uint16_t _data_ready;
    bool _average = false;
    float _integral = 0;
    uint32_t _counter = 0;
};
#endif //SPS30SENSOR
