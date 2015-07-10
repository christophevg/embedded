// blink.c
// author: Christophe VG

#include <avr/io.h>
#include <util/delay.h>

#include "../moose/avr.h"
#include "../moose/bool.h"
#include "../moose/serial.h"

#define STATUS_LED 0

void pwm_init(void) {
  TCCR1A |= (1<<COM1A1) | (1<<WGM11); // non-inverting mode for OC1A
  TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11); // Mode 14, Prescaler 8

  ICR1 = 40000; // 320000 / 8 = 40000

  DDRD |= (1<<DDD5); // OC1A set to output
}

int main(void) {

  unsigned int counter = 0;

  avr_init();
  serial_init();

  pwm_init();

  while(TRUE) {
    counter++;
    printf("counter = %i\n", counter);
    
    avr_set_bit(PORTB, STATUS_LED);  _delay_ms(50);

    OCR1A = 425;     _delay_ms(1000);
    OCR1A = 1200;     _delay_ms(1000);
    OCR1A = 1975;     _delay_ms(1000);

    avr_clear_bit(PORTB, STATUS_LED);  _delay_ms(50);
  }
   
  return(0);
}
