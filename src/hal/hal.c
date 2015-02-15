#include <avr/io.h>
#include <avr/interrupt.h>
#include "hal.h"

void hal_init(void (*usart_recv_callback)(uint8_t byte)) {
    // HAL Initialization should be atomic; disable interrupts
    cli();
    hal_leds_init();
    hal_spkr_init();
    hal_usart_init(usart_recv_callback);
    hal_spi_init();
    // Reenable interrupts
    sei();
}
