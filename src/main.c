#include <stddef.h>
#include <util/delay.h>
#include "hal/hal.h"

// __attribute__((OS_main)) tells the compiler that this function never returns and
// saves us precious space
__attribute__((OS_main)) int main(void) {

	hal_init(NULL);

	uint8_t recv = 0;
    hal_spi_transfer(0b10000010);   // Send read h01 reg request
    recv = hal_spi_transfer(0x00);  // Dummy byte to receive data

    if (recv == 0x20) {
        hal_leds_set_status(bit(HAL_FLAG_RED_LED));
    } else {
        hal_leds_set_status(bit(HAL_FLAG_GREEN_LED));
    }

}
