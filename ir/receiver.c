// receiver.c
// author: Christophe VG

#include <stdbool.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#include "../moose/avr.h"
#include "../moose/serial.h"

// store 100 pulses consisting of 1 off value and 1 on value
#define PULSES 100
volatile uint16_t pulses[PULSES][2];
volatile uint8_t  count = 0;

int main(void) {
  avr_init();

  // set up Timer1
  TCNT1  = 0;             // set timer to zero
  TCCR1A = 0;	            // normal counting up
  TCCR1B = _BV(CS10);     // internal clock, no prescaler

  // configure and enable interrupts on PB0
  // PB0 == PCINT8
  PCMSK1 |= (1<<PCINT8);
  sei();

  uint8_t reported = 0;
  while(true) {
    for(; reported<count; reported++) {
      printf("%d\t%d\r\n", pulses[reported][0], pulses[reported][0]);
    }
  }

  return(0);
}

ISR(PCINT1_vect) {
  if(avr_bit_is_set(PINB, 0)) {
    // pin is high, so last pulse was low
    pulses[count][0] = TCNT1;
  } else {
    // pin is low, so last pulse was high
    pulses[count][1] = TCNT1;
    count++; // move to next pulse pair
  }
  if(count >= PULSES) { cli(); } // stop and avoid pulses array overflow
  TCNT1 = 0; // reset for next measurement
}
