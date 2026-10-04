#ifndef USART_H
#define USART_H

#define FOSC // Clock Speed
#define BAUD
#define UBRR_VAL

void usart_init(uint16_t ubrr_val);
void usart_transmit(uint16_t data);

#endif