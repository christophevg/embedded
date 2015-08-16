// 
// author: Christophe VG <contact@christophe.vg>

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#include "../moose/avr.h"
#include "../moose/bool.h"
#include "../moose/serial.h"
#include "../moose/clock.h"

/* configuration of external components */
#define XYZ_PORT DDRA
#define PIN_X    0  // PA0
#define PIN_Y    1  // PA1
#define PIN_Z    2  // PA2

int main(void) {
  avr_init();
  clock_init();
  serial_init();

  // initialize the ADC for normal readings
  avr_adc_init();

  avr_clear_bit(XYZ_PORT, PIN_X);
  avr_clear_bit(XYZ_PORT, PIN_Y);
  avr_clear_bit(XYZ_PORT, PIN_Z);

  printf("boot sequence completed...\n");

  // the endless loop
  uint16_t  x,  y,  z;
  float    gx, gy, gz;
  while(TRUE) {
    x = avr_adc_read(PIN_X);
    y = avr_adc_read(PIN_Y);
    z = avr_adc_read(PIN_Z);

    // scale to [-1,+1]
    gx = (x / 1024.0 * 2) - 1;
    gy = (y / 1024.0 * 2) - 1;
    gz = (z / 1024.0 * 2) - 1;

    printf("%lu\t%f\t%f\t%f\n", clock_get_millis(), gx, gy, gz);

    _delay_ms(5);
  }

  return(0);
}
