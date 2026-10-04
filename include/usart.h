#ifndef USART_H
#define USART_H

#define FOSC // Clock Speed
#define BAUD
#define UBRR_VAL

void usart_init(uint16_t ubrr_val);
void usart_transmit(uint8_t data);
uint8_t usart_receive(void);

#endif