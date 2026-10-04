#include <avr/io.h>
#include <stdint.h>
#include "usart.h"

#define FOSC 16000000UL // Clock Speed
#define BAUD 9600UL
#define MYUBRR (FOSC / (16 * (BAUD)) - 1) // Normal asynchronous clock operation
// usart_init(MYUBRR) to run it
void usart_init(uint16_t ubrr) {
    UBRR0H = (uint8_t) (ubrr >> 8);
    UBRR0L = (uint8_t) ubrr;
    UCSR0B = (1 << RXEN0) |  (1 << TXEN0);
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

// Transmit and Receive Data Buffer Registers share same I/O address UDR0
// UDR0: Data Buffer
/*  ============== UCSR0A =================
    RXC0: USART Receive Complete
    TXC0: USART Transmit Complete
    UDRE0: USART Data Register Empty, ready to receive, set after a reset
    FE0: Frame Error
    DOR0: Data OverRun
    UPE0: USART Parity Error

    ============== UCSR0B =================
    RXEN0: Receiver Enable on the RxDn pin
    TXEN0: Transmitter Enable on the TxD0 pin







*/