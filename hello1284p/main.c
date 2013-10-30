// hello1284p is a simple program that augments a counter and blinks the amount
// of that counter. It also writes the memory address of the counter to the
// USART. It was written to experiment with avarice/avr-gdb to try and access
// the data memory of a running system.
//
// author: Christophe VG

#include <avr/io.h>
#include <util/delay.h>

#include "../avr/avr.h"
#include "../avr/bool.h"

#define STATUS_LED 6

unsigned int counter = 0;

int main(void) {
  avr_init();
  serial_init();

  printf("counter is at %x\n", &counter);

  while(TRUE) {
    counter++;

    printf("counter = %i\n", counter);

    for(int i=counter;i>0;i--) {
      avr_set_bit  (PORTD, STATUS_LED);  _delay_ms(200);
      avr_clear_bit(PORTD, STATUS_LED);  _delay_ms(200);
    }
    
    _delay_ms(2000);
  }

  return(0);
}
