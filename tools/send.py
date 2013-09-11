# send/receive a packet from XBee, connected through USB explorer
# Christophe VG <contact@christophe.vg>

import serial
import sys

ser = serial.Serial('/dev/tty.usbserial-AD025LL3', 9600)

# get hex from command line (skip first/script)
message = map(lambda a: int(a, 16), sys.argv[1:])

# take complement of last value/checksum
message[-1] = 0xff - message[-1]

# STEP 1: send string as hex
print 'send   : ', ' '.join('%02x'%i for i in message)
ser.write( bytearray(message) )

# STEP 2: print out the response packet

# wait for start of the frame
while(0x7e != ord(ser.read())): pass

# get length MSB (normally zero)
length_msb = ord(ser.read())
# get lenght LSB (normally the actual length)
length     = ord(ser.read())

# construct a response list
response = [ 0x7e, length_msb, length ]
for i in range(length+1):
   response.append(ord(ser.read()))

print 'receive: ', ' '.join('%02x'%i for i in response)
