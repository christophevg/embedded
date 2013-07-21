// blink.c
// author: Christophe VG

// based on blink_1MHz.c from tutorial on Sparkfun

#include <avr/io.h>
#include <util/delay.h>

#include "../avr/avr.h"
#include "../avr/bool.h"
#include "../avr/bits.h"

#define STATUS_LED 0

int main(void) {

  avr_init();

  while(TRUE) {
    sbi(PORTC, STATUS_LED);  _delay_ms(500);
    cbi(PORTC, STATUS_LED);  _delay_ms(250);
    sbi(PORTC, STATUS_LED);  _delay_ms(500);
    cbi(PORTC, STATUS_LED);  _delay_ms(1000);
  }
   
  return(0);
}
