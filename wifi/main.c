// A simple example for using the ESP8266 Wifi module, via USART
// author: Christophe VG

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "../moose/avr.h"
#include "../moose/serial.h"
#include "../moose/wifi.h"

#define OK "\r\nOK\r\n"
#define CLOSED "\nCLOSED\r\n"

void show_output_until(const char *expected) {
  uint8_t last = strlen(expected), pos  = 0;
  while(pos < last) {
    uint8_t b = wifi_receive_byte();
    printf("%c", b);
    if(b == expected[pos]) {
      pos++;
    } else if(b == expected[0]) {
      pos = 1;
    } else {
      pos = 0;
    }
  }
}

void execute_cmd(const char *info, const char *cmd, const char *expected) {
  printf("*** %s", info);
  wifi_send_cmd( cmd, strlen(cmd));
  show_output_until(expected);
}

int main(void) {
  avr_init();
  serial_init();
  wifi_init();
  
  printf("boot and init completed.\n");

  _delay_ms(1000); // needed, else the response fails to complete :-(

  execute_cmd("checking version", "AT+GMR", OK );
  execute_cmd("setting client only mode", "AT+CWMODE=1", OK);

  execute_cmd("leaving current access point", "AT+CWQAP", OK);
  
  execute_cmd("listing available access points", "AT+CWLAP", OK);

  execute_cmd("joining Camp2015-open-legacy",
              "AT+CWJAP=\"Camp2015-open-legacy\",\"\"", OK);

  execute_cmd("checking IP address", "AT+CIFSR", OK);
  
  execute_cmd("connect to nerdhero.org",
              "AT+CIPSTART=\"TCP\",\"nerdhero.org\",80", OK);
  execute_cmd("provide content length", "AT+CIPSEND=76", OK);
  execute_cmd("send content", "GET / HTTP/1.1\r\n"
"User-Agent: curl/7.37.0\r\n"
"Host: nerdhero.org\r\n"
"Accept: */*\r\n\r\n", CLOSED);

  printf("*** all done, entering idle loop\n");
  while(1); // idle loop

  return(0);
}
