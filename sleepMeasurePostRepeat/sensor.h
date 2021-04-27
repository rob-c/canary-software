/*
 * Generic sensor class.
 * All sensor classes inherit from this class.
 */
 
#ifndef SENSOR
#define SENSOR

//******************************************
#include "limits.h"
#include "ArduinoJson.h"

//******************************************
class Sensor {

  //------------------------------------------
  public:
    Sensor(){};
    virtual int init(bool verbose=false) {return 0;}
    virtual void readData(bool verbose=false) {return;}
    virtual void integrate(bool verbose=false) {return;}
    virtual String getSensorString(void) {return "";}
    virtual String getMeasurementsString(void) {return "";}
    virtual void getJSONDoc(JsonDocument &doc) {return;}
    virtual String getName(void) {return _name;}

  //------------------------------------------
  protected:
    String _name = "GenericSensor";
};
#endif //SENSOR
