// main.c
// author: Christophe VG <contact@christophe.vg>

// main event loop for the xt0f-004 wireless sensor node
// see http://christophe.vg/embedded/xt0f-004 for more info
// basically this mote consists of:
// - ATMEGA1284p
// - RS-232 connection on USART0
// - XBEE on USART1
// - Light and Temperature sensors
// - LEDs for power-on (red) / operation (green)  / alert (orange)

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "../avr/avr.h"
#include "../avr/bool.h"
#include "../avr/serial.h"
#include "../avr/sleep.h"
#include "../avr/xbee.h"

// forward declarations
void sleep(void);
void wakeup(void);
void send(uint8_t *bytes);
void send_str(const char *string);

// configuration of external components

#define STATUS_LED_PORT    PORTD  // PD3
#define STATUS_LED_PIN     3
#define OPERATION_LED_PORT PORTD  // PD4
#define OPERATION_LED_PIN  4
#define ALERT_LED_PORT     PORTD  // PD5     

#define TEMP_SENSOR_PORT   DDRC   // PC4
#define TEMP_SENSOR_PIN    5
#define LIGHT_SENSOR_PORT  DDRC   // PC5
#define LIGHT_SENSOR_PIN   4

#define VALUES_COUNT 4            // 2 bytes temperature & light

// we use both USARTs
#define SERIAL_ON_USART0
#define XBEE_ON_USART1

// network config
#define DESTINATION XBEE_COORDINATOR

int main(void) {
  uint16_t reading;               // the 16-bit reading from the ADC
  uint8_t  values[VALUES_COUNT];  // the bytes containing the readings

  avr_init();                     // initialise MCU
  avr_set_bit(STATUS_LED_PORT,    // turn on the red status led
              STATUS_LED_PIN);
  avr_adc_init();                 // initialize the ADC for normal readings
  serial_init();                  // initialize use of serial port(s)

  printf("serial initialized\n"); // announce boot process via serial

  sleep_init();                   // we're using power-down-style sleeping

  avr_clear_bit(TEMP_SENSOR_PORT, // prepare sensor pins for input
                TEMP_SENSOR_PIN );
  avr_clear_bit(LIGHT_SENSOR_PORT,
                LIGHT_SENSOR_PIN);

  xbee_wait_for_association();    // wait until the network is available

  printf("xbee initialized\n");   // announce boot process via serial
  send_str("xbee initialized\n"); // announce boot process via xbee

  while(TRUE) {
    wakeup();

    // temperature
    reading = avr_adc_read(TEMP_SENSOR_PIN);
    values[1] = reading & 0x00FF;
    values[2] = (reading >> 8) & 0x00FF;

    // light
    reading = avr_adc_read(LIGHT_SENSOR_PIN);
    values[3] = reading & 0x00FF;
    values[4] = (reading >> 8 ) & 0x00FF;

    // print it to the serial
    send_bytes(values, VALUES_COUNT);

    sleep();
  }

  return(0);
}

void send_bytes(uint8_t *bytes, uint8_t size) {
  xbee_tx_t frame;

  frame.size        = size;
  frame.id          = XB_TX_NO_RESPONSE;
  frame.address     = XB_COORDINATOR;
  frame.nw_address  = XB_NW_ADDR_UNKNOWN;
  frame.radius      = XB_MAX_RADIUS;
  frame.options     = XN_OPT_NONE;
  frame.data        = bytes;

  xbee_send(&frame);
}

void send_str(const char *string) {
  send_bytes((uint8_t*)string, strlen(string));
}

void wakeup(void) {
  avr_set_bit(PORTB, SENSOR_VCC);   // give power to sensors
  _delay_ms(10);                    // time to rise - TODO: how o this CLEANLY?
  avr_adc_init();                   // re-initialize the ADC
                                    // TODO: move this elsewhere once I
                                    // understand enabling ADC better ;-)
  xbee_wakeup();                    // revive the XBee from hibernation
                                    // (this includes waiting for association)
}

void sleep(void) {
  xbee_sleep();                     // put XBee to sleep
  avr_clear_bit(PORTB, SENSOR_VCC); // revoke power to sensors
  sleep_ms(59000L);                 // power-down the MCU for 59 seconds
}
