/*
 * Edit this file to configure any option.
 * Do not edit sleepMeasurePostRepeat.ino or other files.
 * To enable/disable a sensor uncomment/comment its definition.
 * To set a parameter uncomment its definition and provide its value.
 */

//******************************************
//setup
#define CAFFEINE (true) //do not go to sleep; use if not battery operated
#define SLEEPTIME (60) //sleep interval [s]
#define INTEGRATIONTIME (1) //measurements integration time interval [s]
#define PRINTSERIAL (false) //print measurements to serial output
#define POST (true) //connect and post measurements to MQTT server
#define VERBOSE (true) //print connection status and posting details
//#define ESP8266 //use an ESP8266 MCU instead of an ESP32 MCU (default)

//******************************************
//wifi
//#define WIFISSID ("ssid") //wifi SSID
//#define WIFIPASSWORD ("password") //wifi password

//******************************************
//MQTT
#define MQTTMESSAGESIZE (1024) //maximum message length [byte]
//#define MQTTSERVER ("127.0.0.1") //MQTT server address
//#define MQTTSPORT (1883) //MQTT server port
//#define MQTTUSERNAME ("username") //MQTT username
//#define MQTTPASSWORD ("password") //MQTT password
//#define MQTTTOPIC ("topic") //MQTT topic
//#define INSTITUTE ("institute") //institute of the measuring device (optional)
//#define ROOM ("room") //room of the measuring device (optional)
//#define LOCATION ("location") //location of the measuring device (optional)
//#define NAME ("name") //name of the measuring device (optional)
//#define MACASNAME //use the MAC address as device name (optional)
#define MQTTTIME (10) //MQTT broker check-in time interval [s]

//******************************************
//SHT temperature and humidity sensors
//#define SHT35A //SHT35 A (0x44 address)
#define SHT35B //SHT35 B (0x45 address)
//#define SHT85 //SHT85 (0x44 address)

//******************************************
//MAX31865 RTD sensor
//#define MAX31865 //MAX31865
#define MAX31865RNOM (1000) //sensor nominal resistance at 0 C (100 Ohm or 1000 Ohm) [Ohm]
#define MAX31865RREF (4020) //reference resistor [Ohm]
#ifdef ESP8266 //ESP8266
#define MAX31865CS (15) //CS pin for ESP8266
#else //ESP32
#define MAX31865CS (33) //CS pin for ESP32
#endif
//#define MAX31865RHSOURCE (sht35b) //RH measurement source for dew point calculation; use instance name from sleepMeasurePostRepeat.ino

//******************************************
//SPS30 dust particle counter
//#define SPS30
#define SPS30AVERAGE (true)//average over multiple SPS30 measurements (one per second)

//******************************************
//ADS1x15 ADCs and NTCs (4 channels)
//#define ADS1015 //ADS1015 (12 bit)
//#define ADS1115 //ADS1115 (16 bit)
#define ADS1x15VDD (3.3)//VDD [V]
#define ADS1x15VREF (4.096)//ADC reference voltage [V] //NOTE valid for ADC gain set to 1, see ADS1x15GAIN
#define ADS1x15GAIN (GAIN_ONE) //ADC gain: 1x gain  +/- 4.096V  1 bit = 2mV (ADS1015) or 0.125mV (ADS1115), see ADS1x15VREF

//ADC0/NTC0 (channel 0)
//#define ADS1x150ADC //NOTE set also the voltage divider resistor value
#define ADS1x150NTC //NOTE set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x150RDIV (49.9e3) //ADC voltage divider resistor value [Ohm] //49.9e3
#define ADS1x150R0 (4.7e3) //NTC R0 [Ohm] //1e4
#define ADS1x150T0 (298.15) //NTC T0 [K] //298.15
#define ADS1x150B (3650) //NTC B //3435

//ADC1/NTC1 (channel 1)
//#define ADS1x151ADC //NOTE set also the voltage divider resistor value
#define ADS1x151NTC //NOTE set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x151RDIV (49.9e3) //ADC voltage divider resistor value [Ohm] //49.9e3
#define ADS1x151R0 (4.7e3) //NTC R0 [Ohm] //1e4
#define ADS1x151T0 (298.15) //NTC T0 [K] //298.15
#define ADS1x151B (3650) //NTC B //3435

//ADC2/NTC2 (channel 2)
//#define ADS1x152ADC //NOTE set also the voltage divider resistor value
#define ADS1x152NTC //NOTE set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x152RDIV (49.9e3) //ADC voltage divider resistor value [Ohm] //49.9e3
#define ADS1x152R0 (4.7e3) //NTC R0 [Ohm] //1e4
#define ADS1x152T0 (298.15) //NTC T0 [K] //298.15
#define ADS1x152B (3650) //NTC B //3435

//ADC3/NTC3 (channel 3)
#define ADS1x153ADC //NOTE set also the voltage divider resistor value
#define ADS1x153NTC //NOTE set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x153RDIV (49.9e3) //ADC voltage divider resistor value [Ohm] //49.9e3
#define ADS1x153R0 (4.7e3) //NTC R0 [Ohm] //1e4/22e3/4.7e3
#define ADS1x153T0 (298.15) //NTC T0 [K] //298.15
#define ADS1x153B (3650) //NTC B //3435/4000/3650
