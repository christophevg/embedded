// serial.c
// based on basic-in-atmega168.c from tutorial on Sparkfun

#include <stdio.h>
#include <avr/io.h>
#include <string.h>

#include "bool.h"
#include "bits.h"

#include "serial.h"

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

#define MAX_CMD_SIZE 64

#define STATUS_LED 0

int main(void) {
  bool    active = TRUE;
  uint8_t key = 0;

  char    buffer[MAX_CMD_SIZE+1];
  buffer[MAX_CMD_SIZE] = '\0';

  uint8_t index = 0;
  
  ioinit();

  printf("\n\nATMEGA168 at your service...\n");
  sbi(PORTC, STATUS_LED);

  while(active) {
    printf("shell> ");
    
    key   = 0;
    index = 0;
    while(key != 13 && index < MAX_CMD_SIZE) {
      key = uart_getchar();
      if(key == 13) {
        buffer[index] = '\0';
      } else {
        buffer[index] = key;
        printf("%c", key);  // echo
      }
      index++;
    }
    printf("\n       %s\n", buffer);
    active = strcmp(buffer, "quit\0"); // equal == 0 == FALSE ;-)
  }

  printf("\n\nsession terminated.\n");
  cbi(PORTC, STATUS_LED);

  return(0);
}

void ioinit(void) {
    // 1 = output, 0 = input
    DDRB = 0b11101111; // PB4 = MISO 
    DDRC = 0b11111111; //
    DDRD = 0b11111110; // PORTD (RX on PD0)

    // USART Baud rate: 9600
    UBRR0H = MYUBRR >> 8;
    UBRR0L = MYUBRR;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    
    stdout = &mystdout; // required for printf init
}

static int uart_putchar(char c, FILE *stream) {
  if (c == '\n') uart_putchar('\r', stream); // add a CR before the LF

  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;

  return 0;
}

uint8_t uart_getchar(void) {
  while( !(UCSR0A & (1<<RXC0)) );
  return(UDR0);
}
