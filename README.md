# canary-software

This is the software to operate a Canary.  
Refer to the [Canary Wiki](https://gitlab.cern.ch/guescini/canary/-/wikis/home) for additional information and instructions.

## scripts
- **PCPost.py**: use a PC to read the serial output from the Canary and post the data to an MQTT server. This is useful when the Canary cannot access a wi-fi nework.

## sleepMeasurePostRepeat
This is the Arduino sketch to be flashed on the Canary. To configure the Canary edit `config.h`. Do not edit the rest of the code.
