#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Recieves test data from MSP432 over Serial
Designed to be run from SSH into this Raspberry Pi
"""
from serial import Serial, SerialException  # serial interfacing library
from time import sleep                      # for timing
from os import system                       # for clearing screen
import json                                 # javascript interfacing

system("clear")
serialport1 = '/dev/serial/by-path/platform-3f980000.usb-usb-0:1.2:1.0'
serialport2 = '/dev/serial/by-path/platform-3f980000.usb-usb-0:1.3:1.0'


# Initialize serial connection:
try:
    ser = Serial(port=serialport1, baudrate=9600, timeout=10)
except SerialException:
    try:
        ser = Serial(port=serialport2, baudrate=9600, timeout=10)
    except SerialException:
        print "Fatal Serial Error. Closing program"
        quit()

f = open("/home/pi/AeroHacks/data.txt", "w")

# Reading data stream and converting to JSON
#     2-minute data read   => 120s
#     sampling freq = .5Hz => 60 samples
for i in tuple(range(60)):  # convert to tuple for speed
    try:
        obj = {}  # dict for JSON conversion
        ser.write("r".encode('utf-8'))  # request data cmd
        ser.flush()
        sleep(.08)  # wait for data to buffer
        gas_volt = ser.readline()
        print gas_volt
        lArr = gas_volt.split(':')
        print lArr
        obj[lArr[0]] = float(lArr[1])
        pressure = ser.readline()
        print pressure
        lArr = pressure.split(':')
        print lArr
        obj[lArr[0]] = float(lArr[1])
        humidity = ser.readline()
        print humidity
        lArr = humidity.split(':')
        print lArr
        obj[lArr[0]] = float(lArr[1])
        temp = ser.readline()
        print lArr
        lArr = temp.split(':')
        obj[lArr[0]] = float(lArr[1])/4
        print obj
        json.dumps(obj)
        f.write(gas_volt)
        f.write(pressure)
        f.write(humidity)
        f.write(lArr[0])
        f.write(str(float(lArr[1]) / 4))
        sleep(1.92) 
    except SerialException:
        print "Serial Exception! Maybe port disconnected?"
        break
f.close()
quit()

