// main.c
// author: Christophe VG <contact@christophe.vg>

// main event loop for the xt0f-003 wireless sensor node
// see http://christophe.vg/embedded/xt0f-003 for more info

#include <avr/io.h>
#include <util/delay.h>

#include "../avr/avr.h"
#include "../avr/bool.h"
#include "../avr/serial.h"
#include "../avr/xbee.h"


/* configuration of external components */
#define STATUS_LED  0
#define TEMP_SENSOR 4

int main(void) {

  avr_init();
  avr_adc_init();
  
  serial_init();

  // app specific init
  // set pin TEMP_SENSOR on port C to 0 = input
  avr_clear_bit(DDRC, TEMP_SENSOR);

  xbee_wait_for_association();

  // show we're operational
  avr_set_bit(PORTC, STATUS_LED); // visually
  xbee_transmit_string("ONLINE"); // remotely

  uint16_t reading;    // the 16-bit reading from the ADC
  uint8_t  values[2];  // the bytes containing the readings

  // endless loop
  while(TRUE) {
    // get sensor value
    reading = avr_adc_read(TEMP_SENSOR);

    // split the 16-bit reading into 2 bytes
    values[0] = reading & 0x00FF;
    values[1] = (reading >> 8 ) & 0x00FF;

    // print it to the serial
    xbee_transmit(values, 2);

    // sleep
    // TODO implement actual sleep mode to conserve power
    _delay_ms(2000);
  }

  return(0);
}
