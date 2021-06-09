#!/usr/bin/env python

#Read the serial output and print it to screen.
#Author: Francesco Guescini.

#******************************************
#import stuff
import argparse, serial, time, sys

#******************************************
def readSerial(args):
    """Read the serial output and print it to screen."""

    #------------------------------------------
    #serial setup
    ser = serial.Serial(
        port = args.port,
        baudrate = args.baudrate,
        parity = serial.PARITY_NONE,
        stopbits = serial.STOPBITS_ONE,
        bytesize = serial.EIGHTBITS,
        timeout = 60)
    ser.flushInput()

    #------------------------------------------
    #loop
    while True:
        try:
            print(time.strftime("%Y%m%d%H%M%S"), ser.readline().decode("utf-8", errors="ignore").strip("\n"))
        except KeyboardInterrupt:
            print()
            sys.exit(0)

#******************************************
if __name__ == "__main__":

    #------------------------------------------
    #parse input arguments
    parser = argparse.ArgumentParser(description="%prog [options]")
    parser.add_argument("-p", "--port", dest="port", default="/dev/ttyUSB0", help="serial port, usually \"/dev/cu.SLAB_USBtoUART\" or \"/dev/ttyUSB0\"")
    parser.add_argument("-b", "--baudrate", dest="baudrate", default=115200, help="baud rate")
    args = parser.parse_args()
    
    #------------------------------------------
    #read serial
    readSerial(args)
    print()
