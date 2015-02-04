#include <avr/io.h>
#include "hal.h"

uint8_t hal_init(void (*usart_recv_callback)(uint8_t byte)) {
	uint8_t error = 0;
	error |= leds_init();
	error |= spkr_init();
	error |= usart_init(usart_recv_callback);
	error |= spi_init();
	return error;
}