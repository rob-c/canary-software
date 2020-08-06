//******************************************
//setup
//#define CAFFEINE //do not go to sleep
//#define SLEEPTIME (5)//s
//#define PRINTSERIAL //print measurements to serial output
//#define POST //connect and post measurements
//#define VERBOSE //print connection status and posting details
//#define ESP8266 //use an ESP8266 board instead of an ESP32 board (default)

//******************************************
//SHT temperature and humidity sensors
//#define SHT35A //SHT35A (0x44 address)
#define SHT35B //SHT35B (0x45 address)
//#define SHT85 //SHT85 (0x44 address)

//******************************************
//MAX31865 RTD sensor
#define MAX31865 //MAX31865
#define MAX31865RNOM (1000) //sensor nominal resistance at 0 C (100 Ohm or 1000 Ohm) [Ohm]
#define MAX31865RREF (4300) //reference resistor [Ohm]
#ifdef ESP8266 //ESP8266
#define MAX31865CS (15) //CS pin for ESP8266
#else //ESP32
#define MAX31865CS (33) //CS pin for ESP32
#endif
//#define MAX31865RHSOURCE ("SHT35B") //RH source

//******************************************
//SPS30 dust particle counter
//#define SPS30
//#define SPS30AVERAGE //instead of sleeping, average over multiple SPS30 measurements
