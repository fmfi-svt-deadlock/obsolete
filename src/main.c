#include <stddef.h>
#include <util/delay.h>
#include "hal/hal.h"

void recv(uint8_t data) {
    uint8_t data_[] = {0, '\0'};
    data_[0] = data;
    hal_usart_transmit((uint8_t*)data_);
}

// __attribute__((OS_main)) tells the compiler that this function never
// returns and saves us precious space
__attribute__((OS_main)) int main(void) {

	hal_init(&recv);

	uint8_t recv = 0;
    hal_spi_transfer(0x82);         // Send read h01 reg request
    recv = hal_spi_transfer(0x00);  // Dummy byte to receive data

    if (recv == 0x20) {
        hal_leds_set_status(hal_leds_get_status() | bit(HAL_FLAG_GREEN_LED));
    } else {
        hal_leds_set_status(hal_leds_get_status() | bit(HAL_FLAG_RED_LED));
    }

    hal_usart_transmit((uint8_t*)"Hello world, I'm a Reader!\n");

    while(1) ;

}
