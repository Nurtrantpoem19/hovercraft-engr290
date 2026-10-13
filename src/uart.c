#include "../include/uart.h"
#include "../include/sensors.h"


void uart_init(uint32_t f_cpu, uint32_t baud) {
    uint16_t ubrr = (uint16_t)((f_cpu / (16UL * baud)) - 1);
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr & 0xFF);

    UCSR0B = (1 << TXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit_char(char c) {
    while (!(UCSR0A & (1 << UDRE0)));

    UDR0 = c;
}

void uart_transmit_string(const char *str) {
    while (*str) {
        uart_transmit_char(*str++);
    }
}

void uart_transmit_uint16(uint16_t num) {
    char buffer[6];
    int i = 0;

    if (num == 0) {
        uart_transmit_char('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i > 0) {
        uart_transmit_char(buffer[--i]);
    }
}


void transmit_IR()
{
        uint16_t raw_val = sensorsIR_read();
        uint16_t mv_val  = sensorsIR_raw_to_mv(raw_val);

        uart_transmit_string("IR Raw: ");
        uart_transmit_uint16(raw_val);
        uart_transmit_string(" | Voltage: ");
        uart_transmit_uint16(mv_val);
        uart_transmit_string(" mV\r\n");


}

void transmit_US()
{
    uint16_t distance = sensorsUS_read();
    
    uart_transmit_string("US Dist: ");
    uart_transmit_uint16(distance);
    uart_transmit_string(" cm\r\n");




}
