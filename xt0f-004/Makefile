MORE_SRC = ../avr/avr.c ../avr/serial.c ../avr/sleep.c ../avr/xbee.c

MCU=atmega1284p
F_CPU=8000000

AVRDUDE_PROGRAMMER=jtag2

# external hardware composition ... yummie :-)
MORE_CDEFS=-DSERIAL_ON_USART1 # -DXBEE_ON_USART0

include ../avr/Makefile
