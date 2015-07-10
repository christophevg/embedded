// main.c
// author: Christophe VG <contact@christophe.vg>

// experiment to determine costly code constructions and alternatives

#include <avr/io.h>
#include <util/delay.h>

#include "../moose/bool.h"

#include "../moose/avr.h"
#include "../moose/serial.h"
#include "../moose/clock.h"

#define _log printf("[%06lu] ", clock_get_millis()); printf

#define INTERVAL 1000UL      // in milliseconds
#define DURATION 1UL         // in milliseconds
// this should add 1 microsecond, more is overhead of function and scheduling

// test function
void step(void) {
  static time_t next = INTERVAL;
  if(clock_get_millis() >= next) {
    _delay_ms(DURATION);
    next += INTERVAL;
  }
}

#define METRICS_REPORT_INTERVAL 1000UL

static inline void reporting_step(void) {
  static unsigned long cycles = 0;
  static time_t next_report = METRICS_REPORT_INTERVAL;

  cycles++;

  if(clock_get_millis() >= next_report) {
    _log("cycles: %lu (ev:%0.8f ms)\n", 
         cycles, (double)(clock_get_millis()) / cycles);
    next_report += METRICS_REPORT_INTERVAL;
  }
}

int main(void) {
  avr_init();
  serial_init();                          _log("serial link ready...\n");
  clock_init();                           _log("clock initialized...\n");
                                          _log("starting event loop...\n");
  while(TRUE) {
    // step();
    reporting_step();
  }

  return(0);
}
