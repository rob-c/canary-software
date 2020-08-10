//******************************************
//setup
//#define CAFFEINE //do not go to sleep
#define SLEEPTIME (5)//s
//#define PRINTSERIAL //print measurements to serial output
#define POST //connect and post measurements
#define VERBOSE //print connection status and posting details
//#define ESP8266 //use an ESP8266 board instead of an ESP32 board (default)

//******************************************
//wifi
#define WIFISSID ("ssid")
#define WIFIPASSWORD ("password")

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

//******************************************
//ADS1x15 ADCs and NTCs
//#define ADS1015 //ADS1015 (12 bit)
#define ADS1115 //ADS1115 (16 bit)
#define ADS1x15VDD (3.3)//[V]
#define ADS1x15VREF (4.096)//[V] //NOTE valid for ADC gain set to 1 //see ADS1x15GAIN
#define ADS1x15GAIN (GAIN_ONE) //1x gain  +/- 4.096V  1 bit = 2mV (ADS1015) or 0.125mV (ADS1115) //see ADS1x15VREF

//ADC0/NTC0
#define ADS1x150ADC //NOTE: set also the voltage divider resistor value
#define ADS1x150NTC //NOTE: set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x150RDIV (50e3) //ADC voltage divider resistor value [Ohm] //50e3
#define ADS1x150R0 (1e4) //NTC R0 [Ohm] //1e4
#define ADS1x150T0 (298.15) //NTC T0 [K] //298.15
#define ADS1x150B (3435) //NTC B //3435

//ADC1/NTC1
#define ADS1x151ADC //NOTE: set also the voltage divider resistor value
#define ADS1x151NTC //NOTE: set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x151RDIV (50e3) //ADC voltage divider resistor value [Ohm] //50e3
#define ADS1x151R0 (1e4) //NTC R0 [Ohm] //1e4
#define ADS1x151T0 (298.15) //NTC T0 [K] //298.15
#define ADS1x151B (3435) //NTC B //3435

//ADC2/NTC2
#define ADS1x152ADC //NOTE: set also the voltage divider resistor value
#define ADS1x152NTC //NOTE: set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x152RDIV (50e3) //ADC voltage divider resistor value [Ohm] //50e3
#define ADS1x152R0 (1e4) //NTC R0 [Ohm] //1e4
#define ADS1x152T0 (298.15) //NTC T0 [K] //298.15
#define ADS1x152B (3435) //NTC B //3435

//ADC3/NTC3
#define ADS1x153ADC //NOTE: set also the voltage divider resistor value
#define ADS1x153NTC //NOTE: set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x153RDIV (50e3) //ADC voltage divider resistor value [Ohm] //50e3
#define ADS1x153R0 (22e3) //NTC R0 [Ohm] //1e4/22e3/4.7e3
#define ADS1x153T0 (298.15) //NTC T0 [K] //298.15
#define ADS1x153B (4000) //NTC B //3435/4000/3650
