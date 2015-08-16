#!/usr/bin/python

# this program plots the three axis of the accelerometer, but it's TOO SLOW for
# real-time analysis.

import matplotlib
matplotlib.use('TKAgg')

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import string
from sys import stdin

def getXYZ():
  for line in lines:
    try:
      (t, x, y, z) = string.split(stdin.readline(), "\t")
      return (int(t), int(x), int(y), int(z))
    except: pass

x = np.arange(0, 100)
y = np.zeros(100)

fig, axes = plt.subplots(nrows=3)
styles    = ['r-', 'g-', 'y-' ]

def plot(ax, style):
  ax.axis([0, 100, -100, 100])
  return ax.plot(x, y, style, animated=True)[0]

lines = [plot(ax, style) for ax, style in zip(axes, styles)]

x_data = np.zeros(100)
y_data = np.zeros(100)
z_data = np.zeros(100)

def animate(i):
  global x_data, y_data, z_data
  try:
    (t, xn,yn,zn) = getXYZ()
    x_data = np.append(x_data, xn)[-100:]
    y_data = np.append(y_data, yn)[-100:]
    z_data = np.append(z_data, zn)[-100:]
    lines[0].set_ydata(x_data)
    lines[1].set_ydata(y_data)
    lines[2].set_ydata(z_data)
  except: pass
  return lines
  
ani = animation.FuncAnimation(fig, animate, interval=0, blit=True)
plt.show()
