// blink.c
// author: Christophe VG

// based on blink_1MHz.c from tutorial on Sparkfun

#include <avr/io.h>
#include <util/delay.h>

#include "../moose/avr.h"
#include "../moose/bool.h"

#define STATUS_LED 3

int main(void) {

  avr_init();

  while(TRUE) {
    avr_set_bit  (PORTD, STATUS_LED);  _delay_ms(500);
    avr_clear_bit(PORTD, STATUS_LED);  _delay_ms(250);
    avr_set_bit  (PORTD, STATUS_LED);  _delay_ms(500);
    avr_clear_bit(PORTD, STATUS_LED);  _delay_ms(1000);
  }
   
  return(0);
}
