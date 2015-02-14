#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hal.h"
#include "usart.h"
#include "../bit_manip.h"

static void (*recv_callback)(uint8_t byte);

ISR(USART0_RX_vect) {

    if (recv_callback != NULL) {
        recv_callback(UDR0);
    }
}

uint8_t hal_usart_init(void (*usart_recv_callback)(uint8_t byte)) {

    // Set baud rate to 19 200 bauds
    UBRR0 = 25;

    // Rx Interrupt Enable, Enable transmitter and receiver
    UCSR0B |= bit(RXCIE0) | bit(RXEN0) | bit(TXEN0);

    // Even parity, 8 bit data size
    UCSR0C |= bit(UPM01) | bit(UCSZ00);

    recv_callback = usart_recv_callback;

    return 0;
}

void hal_usart_transmit(uint8_t data) {

    UDR0 = data;
    while(!is_set(UCSR0A, bit(UDRE0)));
}
