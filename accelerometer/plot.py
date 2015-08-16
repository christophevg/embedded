#!/usr/bin/python

# this program plots the three axis of the accelerometer, but it's TOO SLOW for
# real-time analysis.

import matplotlib
matplotlib.use('TKAgg')

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

import serial
import string

ser = serial.Serial("/dev/tty.usbserial-FTSJ84AI", 9600)

def wait_for_nl():
  ser.readline();

def getXYZ():
  while(True):
    try:
      (t, x, y, z) = string.split(ser.readline(), "\t")
      return (int(t), float(x), float(y), float(z))
    except: pass

wait_for_nl() # to avoid reading a partial line already being sent

x = np.arange(0, 100)
y = np.zeros(100)

fig, axes = plt.subplots(nrows=3)
styles    = ['r-', 'g-', 'y-' ]

def plot(ax, style):
  ax.axis([0, 100, -1, 1])
  return ax.plot(x, y, style, animated=True)[0]

lines = [plot(ax, style) for ax, style in zip(axes, styles)]

x_data = np.zeros(100)
y_data = np.zeros(100)
z_data = np.zeros(100)

def animate(i):
  global x_data, y_data, z_data
  (t, xn,yn,zn) = getXYZ()
  x_data = np.append(x_data, xn)[-100:]
  y_data = np.append(y_data, yn)[-100:]
  z_data = np.append(z_data, zn)[-100:]
  lines[0].set_ydata(x_data)
  lines[1].set_ydata(y_data)
  lines[2].set_ydata(z_data)
  return lines

ani = animation.FuncAnimation(fig, animate, xrange(1, 200), 
                              interval=0, blit=True)
plt.show()
