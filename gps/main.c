// This is a simple program that augments a counter and prints this value to a
// terminal via its USART port.
//
// author: Christophe VG

#include <avr/io.h>
#include <util/delay.h>

#include "../moose/avr.h"
#include "../moose/serial.h"
#include "../moose/gps.h"
#include "../moose/nmea.h"

unsigned int counter = 0;

void gps_position_handler(nmea_position pos) {
  printf(
    "UTC=%02d:%02d:%06.3f | latt: %02d %07.4f\" %c | long: %03d %07.4f\" %c\n",
    pos.time.hour,     pos.time.min,      pos.time.sec,
    pos.lattitude.deg, pos.lattitude.min, pos.lattitude.ns,
    pos.longitude.deg, pos.longitude.min, pos.longitude.ew    
  );
}

int main(void) {
  avr_init();
  serial_init();
  gps_init();
  //  gps_on_receive_position(new_position_handler);
  
  printf("boot and init completed.\nentering tracking loop...\n");

  while(1) { gps_receive(); }

  return(0);
}
