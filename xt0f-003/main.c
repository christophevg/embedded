// main.c
// author: Christophe VG <contact@christophe.vg>

// main event loop for the xt0f-003 wireless sensor node
// see http://christophe.vg/embedded/xt0f-003 for more info

#include <avr/io.h>
#include <util/delay.h>

#include "../avr/avr.h"
#include "../avr/bool.h"
#include "../avr/serial.h"
#include "../avr/sleep.h"
#include "../avr/xbee.h"

// forward declarations
void sleep(void);
void wakeup(void);

/* configuration of external components */
#define SENSOR_VCC   1  // PB1
#define STATUS_LED   0  // PC0
#define TEMP_SENSOR  5  // PC4
#define LIGHT_SENSOR 4  // PC5

#define VALUES_COUNT 6  // 2 bytes for temperature and light, 1 bytes for VCC

int main(void) {

  uint16_t reading;               // the 16-bit reading from the ADC
  uint8_t  values[VALUES_COUNT];  // the bytes containing the readings
  // initialise MCU
  avr_init();
  
  // turn on the red status led ASAP to show boot process
  avr_set_bit(PORTC, STATUS_LED);

  // initialize the ADC for normal readings
  avr_adc_init();

  // we're using the XBee module, connected to the UART
  serial_init();

  // we're using power-down-style sleeping
  sleep_init();

  // app specific init
  // set pin TEMP_SENSOR on port C to 0 = input
  avr_clear_bit(DDRC, TEMP_SENSOR);
  // set pin LIGHT_SENSOR on port C to 0 = input
  avr_clear_bit(DDRC, LIGHT_SENSOR);

  // wait until the network is available
  xbee_wait_for_association();

  // show we're operational
  avr_clear_bit(PORTC, STATUS_LED); // visually by removing the boot indicator
  xbee_transmit_string("HELLO");    // remotely to announce we're booted

  // the endless loop
  while(TRUE) {
    // make sure that we're awake
    wakeup();

    // TEMPERATURE
    // get sensor value
    reading = avr_adc_read(TEMP_SENSOR);

    // split the 16-bit reading into 2 bytes
    values[2] = reading & 0x00FF;
    values[3] = (reading >> 8 ) & 0x00FF;

    // LIGHT
    reading = avr_adc_read(LIGHT_SENSOR);

    // split the 16-bit reading into 2 bytes
    values[4] = reading & 0x00FF;
    values[5] = (reading >> 8 ) & 0x00FF;

    // VCC
    // get power level
    // NOTE: this must be read last, the sleep period gives the ADC time to
    //       recover it seems
    // TODO: investigate
    reading = avr_get_vcc();
    values[0] = reading & 0x00FF;
    values[1] = (reading >> 8 ) & 0x00FF;

    // print it to the serial
    xbee_transmit(values, VALUES_COUNT);

    sleep();
  }

  return(0);
}

void wakeup(void) {
  // give power to sensors
  avr_set_bit(PORTB, SENSOR_VCC);

  // give it some time to rise ...
  // TODO: how to do this CLEANLY ?
  _delay_ms(10);

  // revive the XBee from hibernation (this includes waiting for association)
  xbee_wakeup();
}

void sleep(void) {
  // put XBee to sleep
  xbee_sleep();

  // revoke power to sensors
  avr_clear_bit(PORTB, SENSOR_VCC);

  // power-down the MCU
  sleep_ms(59000L);
}
