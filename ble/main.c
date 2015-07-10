// This is a simple program that augments a counter and prints this value to a
// terminal via its USART port.
//
// author: Christophe VG

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../moose/avr.h"
#include "../moose/pins.h"
#include "../moose/serial.h"

#include "ble.h"

void ble_init(void) {
  // make RX pin input pin by clearing it
  avr_clear_bit(BLE_RX_PORT, BLE_RX_PIN);

  // compute baud rate setting
  // thanks: http://www.appelsiini.net/2011/simple-usart-with-avr-libc
  #undef BAUD
  #define BAUD 9600
  #include <util/setbaud.h>

  UBRRxH = UBRRH_VALUE;
  UBRRxL = UBRRL_VALUE;

#if USE_2X
  UCSRxA |= _BV(U2Xx);
#else
  UCSRxA &= ~(_BV(U2Xx));
#endif

  UCSRxC = _BV(UCSZx1) | _BV(UCSZx0); // 8-bit data
  UCSRxB = _BV(RXENx)  | _BV(TXENx);  // Enable RX and TX
}

void ble_putchar(char c) {
  loop_until_bit_is_set(UCSRxA, UDREx);
  UDRx = c;
}

uint8_t ble_getchar(void) {
  while( !(UCSRxA & (1<<RXCx)) );
  return(UDRx);
}

int main(void) {
  avr_init();
  serial_init();
  ble_init();
  
  uint8_t count = 0;
  while(1) {
    uint8_t c = ble_getchar();
    count += 16;
    if(count == 0) {
      printf("%d\n", c);
    }
  }

  return 0;
}
