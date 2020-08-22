#ifndef SENSOR
#define SENSOR

#include "config.h"
#include "limits.h"
#include "ArduinoJson.h"

class Sensor {
  
  public:
    Sensor(){};
    virtual int init(void) {return 0;}
    virtual void readData(void) {return;}
    virtual void integrate(void) {return;}
    virtual String getSensorString(void) {return "";}
    virtual String getMeasurementsString(void) {return "";}
    virtual void getJSONDoc(JsonDocument &doc) {return;}
    virtual String getName(void) {return _name;}
  
  protected:
    String _name = "GenericSensor";
};
#endif //SENSOR
