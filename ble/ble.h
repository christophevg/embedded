// xbee.h
// author: Christophe VG <contact@christophe.vg>

#ifndef __BLE_H
#define __BLE_H

#include <avr/io.h>

#include "../moose/bool.h"
#include "../moose/avr.h"
#include "../moose/pins.h"

// BLE is controled via USART, some AVR devices have multiple USARTs
// the identifying 0 or 1 is replace by an 'x'

#define BLE_ON_USART1_NAME STR(BLE_ON_USART1)

// USART0 (default)
// interrupts
#ifdef BLE_ON_USART1    // USART1 (e.g. on the ATMEGA1284p)
// interrupts
#define USARTx_RX_vect USART1_RX_vect
#define USARTx_TX_vect USART1_TX_vect
// port + pin
#define BLE_RX_PORT RX1_PORT
#define BLE_RX_PIN  RX1_PIN
// registers
#define UBRRxH UBRR1H
#define UBRRxL UBRR1L  
#define UCSRxA UCSR1A
#define U2Xx   U2X1
#define UCSRxC UCSR1C
#define UCSRxB UCSR1B
#define UCSZx1 UCSZ11
#define UCSZx0 UCSZ10
#define RXENx  RXEN1
#define TXENx  TXEN1
#define UCSRxA UCSR1A
#define UDREx  UDRE1
#define RXCx   RXC1
#define UDRx   UDR1
#define TXCx   TXC1
#define RXCIEx RXCIE1
#define TXCIEx TXCIE1
#else                     // USART0 (default)
// interruptss
#define USARTx_RX_vect USART0_RX_vect
#define USARTx_TX_vect USART0_TX_vect
// port + pin
#define BLE_RX_PORT RX0_PORT
#define BLE_RX_PIN  RX0_PIN
// registers
#define UBRRxH UBRR0H
#define UBRRxL UBRR0L  
#define UCSRxA UCSR0A
#define U2Xx   U2X0
#define UCSRxC UCSR0C
#define UCSRxB UCSR0B
#define UCSZx1 UCSZ01
#define UCSZx0 UCSZ00
#define RXENx  RXEN0
#define TXENx  TXEN0
#define UCSRxA UCSR0A
#define UDREx  UDRE0
#define RXCx   RXC0
#define UDRx   UDR0
#define TXCx   TXC0
#define RXCIEx RXCIE0
#define TXCIEx TXCIE0
#endif


#endif
