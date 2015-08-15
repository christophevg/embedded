MORE_SRC = ../moose/avr.c ../moose/serial.c ../moose/wifi.c

MCU=atmega1284p
F_CPU=8000000

AVRDUDE_PROGRAMMER=jtag2

# external hardware composition ... yummie :-)
MORE_CDEFS=-DSERIAL_ON_USART1

include ../moose/Makefile

serial-digitus:
	screen /dev/tty.usbserial-FTSJ84AI 9600

go: all program serial-digitus
