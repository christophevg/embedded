#if ! defined F_CPU     // should come in from the Makefile
#define F_CPU  18000000
#endif

#define BAUD   9600
#define MYUBRR (F_CPU/16/BAUD-1)



void       ioinit(void);
static int uart_putchar(char c, FILE *stream);
uint8_t    uart_getchar(void);
