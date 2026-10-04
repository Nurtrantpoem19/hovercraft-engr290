#include <avr/io.h>
#include <stdint.h>
#include "usart.h"

#define FOSC 16000000UL // Clock Speed
#define BAUD 9600UL // BAUD Rate
#define UBRR_VAL (FOSC / (16UL * (BAUD)) - 1UL) // Normal asynchronous clock operation

void usart_init(uint16_t ubrr_val) {
    // Split 12 bits of ubrr into high and low
    UBRR0H = (uint8_t) (ubrr_val >> 8);
    UBRR0L = (uint8_t) (ubrr_val);
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Set Character size: 8-bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_transmit(uint16_t data) {

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