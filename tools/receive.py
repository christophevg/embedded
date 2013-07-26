# transmission receiver for XBee
# Christophe VG <contact@christophe.vg>

# example: 7e 00 0d 90 00 13 a2 00 40 8b 1a 6b 01 c3 01 52 53
#          frame    
#             MSB
#                LSB
#                   frame type
#                      64-bit address
#                                             16-bit address
#                                                   options
#                                                       data (R)
#                                                          checksum

import serial
import sys
import time

ser = serial.Serial('/dev/tty.usbserial-AD025LL3', 9600)

while(True):
  try:
    # wait for start of frame
    while(0x7e != ord(ser.read())): pass

    # get length MSB (normally zero)
    length_msb = ord(ser.read())
    #            LSB (= actual length + 12 bytes of "overhead")
    length     = ord(ser.read())
    actual     = int(length)-12;
  
    # frame type
    frame = ser.read()
  
    # 64-bit source address = 8 bytes
    source = []
    for i in range(8):
      source.append(ord(ser.read()))
  
    # 16-bit source network address
    source2 = []
    for i in range(2):
      source2.append(ord(ser.read()))
  
    # options
    options = ser.read()

    # data
    data = []
    for i in range(length-12):
      data.append(ord(ser.read()))

    # checksum
    checksum = ser.read()

    # print out frame in a "nice" format ;-)
    print "-" * 80
    print "time    : ", time.strftime("%a, %d %b %Y %H:%M:%S +0000", time.gmtime())
    print "frame   : ", frame.encode("hex")
    print "length  : ", length, " / actual = ", actual
    print "source  : ", ' '.join('%02x'%i for i in source), "/", \
                        ' '.join('%02x'%i for i in source2)
    print "options : ", options.encode("hex")

    # data
    print "data    :", ' '.join(map(str,data))

    if data == [72, 69, 76, 76, 79 ]:
      print " -> node joining"
    elif len(data) == 6:
      # decoding of VCC = first byte
      power = data[0] + data[1] * 256
      power_vcc = (power / 255.0) * 1.1
      power_pct = power_vcc / 1.1 * 100

      # decoding of temperature = second and third byte
      temp = data[2] + data[3] * 256
      temp_volt = (temp / 1024.0) * 3.3
      temp_degr = (temp_volt - 0.25) / 0.028

      # decoding of light intensity = fourth and fifth byte
      light = data[4] + data[5] * 256
      light_pct = light / 10.24

      print " -> readings:"
      print "   temp :", temp,  '=>', temp_volt, "V =", temp_degr, "C"
      print "   light:", light, '=>', light_pct, "%"
      print "   power:", power, '=>', power_vcc, "V =", power_pct, "%"
    else:
      print " -> unknown packet"

    print "checksum: ", checksum.encode("hex")
    print "-" * 80

  except KeyboardInterrupt:
    sys.exit(0)
