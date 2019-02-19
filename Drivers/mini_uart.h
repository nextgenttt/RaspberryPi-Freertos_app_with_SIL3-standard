#ifndef _MINI_UART_H_
#define _MINI_UART_H_

//this counter is for debug only
volatile char int_rx_count;

void serial_writer_task(void* write_delay);

int mini_uart_write(const char *buf, int count);

int mini_uart_read(char *buf, int count);

void hexstring ( unsigned int d );
void bcm2835_miniuart_sendstr(char *str);

#endif
