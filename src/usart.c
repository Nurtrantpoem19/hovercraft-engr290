#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "usart.h"

#define FOSC 16000000UL // Clock Speed
#define BAUD 9600UL // BAUD Rate
#define UBRR_VAL (FOSC / (16UL * (BAUD)) - 1UL) // Normal asynchronous clock operation
#define BUFFERSIZE 32U // 32 bytes for buffer

// Transceiver Buffer
volatile uint8_t txBuffer[BUFFERSIZE];
volatile uint8_t txHead = 0; // write
volatile uint8_t txTail = 0; // read

ISR(USART_UDRE_vect) {
    if (txTail != txHead) { // if not empty
        UDR0 = txBuffer[txTail];
        txTail = (txTail + 1) % BUFFERSIZE;
    }
    else {
       UCSR0B &= ~(1 << UDRIE0); 
    }
}

void usart_init(uint16_t ubrr_val) {
    SREG |= (1 << SREG_I); // Allows global interrupts --> Move this to the bottom of system_init later


    // Split 12 bits of ubrr into high and low
    UBRR0H = (uint8_t) (ubrr_val >> 8);
    UBRR0L = (uint8_t) (ubrr_val);
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Set Character size: 8-bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_transmit(uint8_t data) {
    if (((txHead + 1) % BUFFERSIZE) != txTail) {
        txBuffer[txHead] = data;
        txHead = ((txHead + 1) % BUFFERSIZE);
        UCSR0B |= (1 << UDRIE0);
    }
    else { // full: discard data
        return;
    }
}