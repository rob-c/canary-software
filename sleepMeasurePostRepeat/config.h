/*
 * Edit this file to configure any option.
 * Do not edit sleepMeasurePostRepeat.ino or other files.
 * To enable/disable a sensor uncomment/comment its definition.
 * To set a parameter uncomment its definition and provide its value.
 */

//******************************************
//setup
#define CAFFEINE true //do not go to sleep; use if not battery operated
#define SLEEPTIME 5 //sleep interval [s]
#define INTEGRATIONTIME 1 //measurements integration time interval [s]
#define PRINTSERIAL true //print measurements to serial output
#define POST true //connect and post measurements to MQTT server
#define VERBOSE false //print connection status and posting details
#define TLS false //use TLS encryption; does not work with ESP8266
//#define ESP8266 //use an ESP8266 microcontroller instead of an ESP32
#define I2CSDA 23 //I2C SDA GPIO pin
#define I2CSCL 22 //I2C SCL GPIO pin

//******************************************
//network
//#define ETHERNET //use ethernet connection instead of wifi

//ethernet settings
//wESP32 revision 7+ uses RTL8201
//https://wesp32.com/software/#arduino-esp32
#define ETHERNETPHYADDR 0
#define ETHERNETPHYPOWER -1
#define ETHERNETPHYMDC 16
#define ETHERNETPHYMDIO 17
#define ETHERNETPHYTYPE ETH_PHY_RTL8201

//wifi settings
#define WIFISSID "eduroam" //wifi SSID
//#define WIFIUSER "username" //wifi username
//#define WIFIPASSWORD "password" //wifi password
#define ENT_WIFI_V1 false //fall back to using V1 of the wifi stack (esp32 <= v1.0.6 board from espressif)
#define WIFIENTENABLE true //use enetrprise login details over username/password
#define WIFIENTID "@institute.ac.uk" //wifi enterprise ID
#define WIFIENTUSER "some_user@institute.ac.uk" //wifi enterprise username
#define WIFIENDPASS "wifi_password" //wifi enterprise username

//******************************************
//MQTT
#define MQTTMESSAGESIZE 1024 //maximum message length [byte]
#define MQTTSERVER "some.server.ac.uk" //MQTT server address
#define MQTTPORT 1883 //MQTT server port
#define MQTTTLSPORT 8883 //MQTT with TLS server port
#define MQTTUSERNAME "mqtt_user" //MQTT username
#define MQTTPASSWORD "mqtt_pass" //MQTT password
#define MQTTTOPIC "mqtt_topic" //MQTT topic
#define INSTITUTE "UoE" //institute of the measuring device (optional)
#define ROOM "52xx" //room of the measuring device (optional)
#define LOCATION "JCMB" //location of the measuring device (optional)
#define NAME "canary1" //name of the measuring device (optional)
//#define MACASNAME //use the MAC address as device name (optional)
#define MQTTTIME 10 //MQTT broker check-in time interval [s]
#define MQTTFAILREBOOT true //reboot the MCU in case of repeated MQTT connection failures

//******************************************
//SHT temperature and humidity sensors
//#define SHT35A //enable SHT35 A (0x44 address)
#define SHT35B //enable SHT35 B (0x45 address)
//#define SHT35 //enable SHT85 (0x44 address)

//******************************************
//MAX31865 RTD sensor
#define MAX31865 //enable MAX31865
#define MAX31865RNOM 1000 //sensor nominal resistance at 0 C (100 Ohm or 1000 Ohm) [Ohm]
#define MAX31865RREF 4020 //reference resistor [Ohm]
#ifdef ESP8266 //ESP8266
#define MAX31865CS 15 //CS pin for ESP8266
#else //ESP32
#define MAX31865CS 33 //CS pin for ESP32
#endif
#define MAX31865RHSOURCE sht35b //RH measurement source for dew point calculation; use instance name from sleepMeasurePostRepeat.ino

//******************************************
//BME280 temperature, humidity and pressure sensor
//#define BME280 //enable BME280
#define BME280ADDRESS 0x77 //BME280 address: 0x77 (default) or 0x76

//******************************************
//BMP3xx temperature and pressure sensors
//#define BMP3XX //enable BMP3xx
#define BMP3XXTYPE "BMP390" //BMP3xx type
#define BMP3XXADDRESS 0x77 //BMP3xx address: 0x77 (default) or 0x76

//******************************************
//SPS30 dust particle counter
#define SPS30 true //enable SPS30
#define SPS30AVERAGE true //average over multiple SPS30 measurements (one per second)
#define SPS30VERBOSE false //print SPS30 information

//******************************************
//ADS1x15 ADCs and NTCs (4 channels)
//#define ADS1015 //enable ADS1015 (12 bit)
//#define ADS1115 //enable ADS1115 (16 bit)
#define ADS1x15VDD 3.3 //VDD [V]
#define ADS1x15VREF 4.096 //ADC reference voltage [V] //NOTE valid for ADC gain set to 1, see ADS1x15GAIN
#define ADS1x15GAIN GAIN_ONE //ADC gain: 1x gain  +/- 4.096V  1 bit = 2mV (ADS1015) or 0.125mV (ADS1115), see ADS1x15VREF

//ADC0/NTC0 (channel 0)
#define ADS1x150ADC false //NOTE set also the voltage divider resistor value
#define ADS1x150NTC false //NOTE set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x150RDIV 49.9e3 //ADC voltage divider resistor value [Ohm] //49.9e3
#define ADS1x150R0 22e3 //NTC R0 [Ohm] //1e4
#define ADS1x150T0 298.15 //NTC T0 [K] //298.15
#define ADS1x150B 4000 //NTC B //3435
#define ADS1x150NTCTHRESHOLD -75 //NTC lower threshold to accept any temperature as valid [C]

//ADC1/NTC1 (channel 1)
#define ADS1x151ADC false //NOTE set also the voltage divider resistor value
#define ADS1x151NTC false //NOTE set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x151RDIV 49.9e3 //ADC voltage divider resistor value [Ohm] //49.9e3
#define ADS1x151R0 1e4 //NTC R0 [Ohm] //1e4
#define ADS1x151T0 298.15 //NTC T0 [K] //298.15
#define ADS1x151B 3435 //NTC B //3435
#define ADS1x151NTCTHRESHOLD -75 //NTC lower threshold to accept any temperature as valid [C]

//ADC2/NTC2 (channel 2)
#define ADS1x152ADC false //NOTE set also the voltage divider resistor value
#define ADS1x152NTC false //NOTE set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x152RDIV 49.9e3 //ADC voltage divider resistor value [Ohm] //49.9e3
#define ADS1x152R0 1e4 //NTC R0 [Ohm] //1e4
#define ADS1x152T0 298.15 //NTC T0 [K] //298.15
#define ADS1x152B 3435 //NTC B //3435
#define ADS1x152NTCTHRESHOLD -75 //NTC lower threshold to accept any temperature as valid [C]

//ADC3/NTC3 (channel 3)
#define ADS1x153ADC false //NOTE set also the voltage divider resistor value
#define ADS1x153NTC false //NOTE set also the voltage divider resistor value and NTC R0, T0 and B values
#define ADS1x153RDIV 49.9e3 //ADC voltage divider resistor value [Ohm] //49.9e3
#define ADS1x153R0 1e4 //NTC R0 [Ohm] //1e4
#define ADS1x153T0 298.15 //NTC T0 [K] //298.15
#define ADS1x153B 3435 //NTC B //3435
#define ADS1x153NTCTHRESHOLD -75 //NTC lower threshold to accept any temperature as valid [C]

//******************************************
//ESP8266/ESP32 ADCs and NTCs
//NOTE ESP8266 has only one ADC channel (A0)
//NOTE ESP32 has multiple ADC channels (with some conditions) (A2, A3, A4, A7 and A9)
//NOTE here one channel is considered but additional channels can be added
//#define ESPxADC //enable ESP8266/ESP32 ADC(s)
#define ESPxADCVDD 3.3 //VDD [V]
#define ESPxADCNREADINGS 10 //number of ADC readings to average
#ifdef ESP8266 //ESP8266
#define ESPxADCVREF 1.0 //ADC reference voltage [V]
#else //ESP32
#define ESPxADCVREF 1.1//ADC reference voltage [V] //NOTE for 0 dB attenuation (0 - 1.1 V input range)
#define ESPxADCATTENUATION ADC_0db //ADC input attenuation: 0 dB gives no attenuation
#endif //ESP8266 or ESP32

//ADC0/NTC0 (channel 0)
#define ESPxADC0ADC true //NOTE set also the voltage divider resistor value
#define ESPxADC0NTC true //NOTE set also the voltage divider resistor value and NTC R0, T0 and B values
#define ESPxADC0RDIV 84.5e3 //ADC voltage divider resistor value [Ohm] //84.5e3
#define ESPxADC0R0 1e4 //NTC R0 [Ohm] //1e4
#define ESPxADC0T0 298.15 //NTC T0 [K] //298.15
#define ESPxADC0B 3435 //NTC B //3435
#define ESPxADC0NTCTHRESHOLD -75 //NTC lower threshold to accept any temperature as valid [C]
#ifdef ESP8266 //ESP8266
#define ESPxADC0PIN A0 //ADC channel 0 pin //only A0
#else //ESP32
#define ESPxADC0PIN A2 //ADC channel 0 pin //A2, A3, A4, A7 or A9
#endif //ESP8266 or ESP32

//******************************************
//certification authority's ROOT certificate
//TrustID X3 Root
#define CACERT						               \
  "-----BEGIN CERTIFICATE-----\n"				       \
  "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
  "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
  "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
  "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
  "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
  "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
  "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
  "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
  "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
  "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
  "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
  "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
  "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
  "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
  "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
  "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
  "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
  "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n"                         \
  "-----END CERTIFICATE-----\n"
