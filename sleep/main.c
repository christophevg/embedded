// main.c
// author: Christophe VG <contact@christophe.vg>

// putting the ATMega328 to sleep to conserve power

#include <avr/io.h>
#include <util/delay.h>

#include "../avr/avr.h"
#include "../avr/bool.h"
#include "../avr/serial.h"
#include "../avr/sleep.h"

/* configuration of external components */
#define STATUS_LED 3  // PD3

int main(void) {
  // initialise MCU
  avr_init();
  
  // turn on the red status led ASAP to show boot process
  avr_set_bit(PORTD, STATUS_LED);

  // we're using the XBee module, connected to the UART
  serial_init();

  // turn off the red status led ASAP to show boot process has ended
  avr_clear_bit(PORTD, STATUS_LED);

  // prepare for using sleep_ms
  sleep_init();

  // the endless loop
  while(TRUE) {

    // show we're awake
    avr_set_bit(PORTC, STATUS_LED); // visually
    printf( "Awake for 1 second ...\n");           // remotely
    
    _delay_ms(1000);
    
    printf( "Going to sleep for 10 seconds...\n");

    avr_clear_bit(PORTC, STATUS_LED);

    sleep_ms(10000L);
  }

  return(0);
}
