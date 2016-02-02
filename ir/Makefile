TARGET             ?= sender
MCU                ?= atmega1284p
F_CPU              ?= 8000000
MORE_SRC           ?= ../moose/avr.c ../moose/serial.c
MORE_CDEFS         ?= -DSERIAL_ON_USART1
AVRDUDE_PROGRAMMER ?= jtag2

include ../moose/Makefile
