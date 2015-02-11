#include <avr/io.h>
#include <avr/interrupt.h>
#include "hal.h"

uint8_t hal_init(void (*usart_recv_callback)(uint8_t byte)) {
	uint8_t error = 0;
    // HAL Initialization should be atomic; disable interrupts
    cli();
	error |= hal_leds_init();
	error |= hal_spkr_init();
	error |= hal_usart_init(usart_recv_callback);
	error |= hal_spi_init();
    // Reenable interrupts
    sei();
	return error;
}
