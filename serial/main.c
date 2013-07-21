// main.c
// author: Christophe VG <contact@christophe.vg>

// based on basic-in-atmega168.c from tutorial on Sparkfun

#include <stdio.h>
#include <avr/io.h>
#include <string.h>

#include "../avr/avr.h"
#include "../avr/bool.h"
#include "../avr/bits.h"
#include "../avr/serial.h"


#define MAX_CMD_SIZE 64

#define STATUS_LED 0

int main(void) {
  bool    active = TRUE;
  uint8_t key = 0;

  char    buffer[MAX_CMD_SIZE+1];
  buffer[MAX_CMD_SIZE] = '\0';

  uint8_t index = 0;
  
  avr_init();
  serial_init();

  printf("\n\nATMEGA168 at your service...\n");
  sbi(PORTC, STATUS_LED);

  while(active) {
    printf("shell> ");
    
    key   = 0;
    index = 0;
    while(key != 13 && index < MAX_CMD_SIZE) {
      key = serial_getchar();
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
