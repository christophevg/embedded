// sender.c
// author: Christophe VG

#include <avr/io.h>
#include <util/delay.h>

#include <stdbool.h>

#include "../moose/avr.h"
#include "../moose/bool.h"

#define IR_LED 0

// 38KHz ~= 26us per cycle
void pulse(void) {
  for(uint8_t i=0; i<10; i++) {
    avr_set_bit  (PORTB, IR_LED);
    _delay_us(13);
    avr_clear_bit(PORTB, IR_LED);
    _delay_us(13);  
  }
}

void no_pulse(void) {
  _delay_us(200);
}

void send_one(void) {
  pulse();
  no_pulse();
  no_pulse();
  no_pulse();
}

void send_zero(void) {
  pulse();
  no_pulse();
}

void send_byte(uint8_t b) {
  for(uint8_t i=0; i<8; i++) {
    b & 0x01 ? send_one() : send_zero();
    b >>= 1;
  }
}

int main(void) {

  avr_init();

  _delay_ms(5000);
  
  send_byte(0b10101010);

  return(0);
}
