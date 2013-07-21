// main.c
// author: Christophe VG <contact@christophe.vg>

// main event loop for the xt0f-003 wireless sensor node
// see http://christophe.vg/embedded/xt0f-003 for more info

#include <avr/io.h>
#include <util/delay.h>

#include "../avr/avr.h"
#include "../avr/bits.h"
#include "../avr/xbee.h"
#include "../avr/serial.h"


/* configuration of external components */
#define STATUS_LED 0


int main(void) {

  avr_init();
  
  serial_init();

  xbee_wait_for_association();

  sbi(PORTC, STATUS_LED);

  xbee_transmit("READY");

  _delay_ms(1000);

  cbi(PORTC, STATUS_LED);

  return(0);
}
