#!/usr/bin/python

import serial
import sys
import time

ser = serial.Serial("/dev/tty.usbserial-FTSJ84AI", 9600)

ser.readline() # to avoid reading a partial line already being sent

samples = 0
start   = time.time()
while(True):
  try:
    print ser.readline().rstrip()
    samples += 1
  except KeyboardInterrupt:
    sys.stderr.write("RPS: {}\n".format(samples/(time.time()-start)))
    sys.exit(0)
