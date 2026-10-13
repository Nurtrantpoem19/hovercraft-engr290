#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdint.h>

void uart_init(uint32_t f_cpu, uint32_t baud);
void uart_transmit_char(char c);
void uart_transmit_string(const char *str);
void uart_transmit_uint16(uint16_t num);

void transmit_IR();
void transmit_US();
#endif // UART_DRIVER_Hinclude <avr/io.h>
