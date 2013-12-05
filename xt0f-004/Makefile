MORE_SRC = ../moose/avr.c    ../moose/clock.c ../moose/sleep.c  \
					 ../moose/serial.c ../moose/xbee.c

MCU=atmega1284p
F_CPU=8000000

AVRDUDE_PROGRAMMER=jtag2

# external hardware composition ... yummie :-)
MORE_CDEFS=-DSERIAL_ON_USART1 # -DXBEE_ON_USART0 == default

#MORE_CDEFS=-DDEBUG -DSERIAL_ON_USART1 # -DXBEE_ON_USART0 == default

include ../moose/Makefile

clean: clean_avr

clean_avr:
	@(cd ../moose; $(MAKE) clean)
