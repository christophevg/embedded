// blink.c
// based on blink_1MHz.c from tutorial on Sparkfun

#include <avr/io.h>
#include <util/delay.h>

#include "bits.h";

#define STATUS_LED 0

void ioinit(void);

int main(void) {
    ioinit();

    while(1) {
      sbi(PORTC, STATUS_LED);  _delay_ms(500);
      cbi(PORTC, STATUS_LED);  _delay_ms(250);
      sbi(PORTC, STATUS_LED);  _delay_ms(500);
      cbi(PORTC, STATUS_LED);  _delay_ms(1000);
    }
   
    return(0);
}

void ioinit(void) {
  // 1 = output, 0 = input
  DDRB = 0b11111111; // all outputs
  DDRC = 0b11111111; // all outputs
  DDRD = 0b11111110; // PORTD (RX on PD0)
}
