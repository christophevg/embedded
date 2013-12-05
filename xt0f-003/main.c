// main.c
// author: Christophe VG <contact@christophe.vg>

// main event loop for the xt0f-003 wireless sensor node
// see http://christophe.vg/embedded/xt0f-003 for more info

#include <avr/io.h>
#include <util/delay.h>

#include "../moose/avr.h"
#include "../moose/bool.h"
#include "../moose/serial.h"
#include "../moose/sleep.h"
#include "../moose/xbee.h"

// forward declarations
void sleep(void);
void wakeup(void);

/* configuration of external components */
#define SENSOR_VCC   1  // PB1
#define STATUS_LED   3  // PD3
#define TEMP_SENSOR  5  // PC4
#define LIGHT_SENSOR 4  // PC5

#define VALUES_COUNT 6  // 2 bytes for VCC, temperature & light

// network config
#define DESTINATION XBEE_COORDINATOR

int main(void) {

  uint16_t reading;               // the 16-bit reading from the ADC
  uint8_t  values[VALUES_COUNT];  // the bytes containing the readings
  // initialise MCU
  avr_init();
  
  // turn on the red status led ASAP to show boot process
  avr_set_bit(PORTD, STATUS_LED);

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
  avr_clear_bit(PORTD, STATUS_LED); // visually by removing the boot indicator
  send_str("HELLO");                // remotely to announce we're booted

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
    send_bytes(values, VALUES_COUNT);

    sleep();
  }

  return(0);
}

void send_str(const char *string) {
  send_bytes((uint8_t*)string, strlen(string));
}

void send_bytes(uint8_t *bytes, uint8_t size) {
  xbee_tx_t frame;

  frame.size        = size;
  frame.id          = XB_TX_NO_RESPONSE;
  frame.address     = XB_COORDINATOR;
  frame.nw_address  = XB_NW_ADDR_UNKNOWN;
  frame.radius      = XB_MAX_RADIUS;
  frame.options     = XB_OPT_NONE;
  frame.data        = bytes;

  xbee_send(&frame);
}

void wakeup(void) {
  // give power to sensors
  avr_set_bit(PORTB, SENSOR_VCC);

  // give it some time to rise ...
  // TODO: how to do this CLEANLY ?
  _delay_ms(10);

  // re-initialize the ADC for normal readings
  // TODO: move this elsewhere once I understand enabling ADC better ;-)
  avr_adc_init();

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
