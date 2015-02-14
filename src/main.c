#include <stddef.h>
#include <util/delay.h>
#include "hal/hal.h"

#include "comm.h"
#include "protocol.h"

// __attribute__((OS_main)) tells the compiler that this function never
// returns and saves us precious space
__attribute__((OS_main)) int main(void) {

    hal_init(&comm_byte_received_callback);

    hal_leds_set_status(bit(HAL_FLAG_GREEN_LED));

    packet p;

    while(1) {
        if (comm_wait_for_packet(&p) != 0) {
            hal_leds_set_status(bit(HAL_FLAG_RED_LED));
            _delay_ms(500);
            hal_leds_set_status(bit(HAL_FLAG_GREEN_LED));
        } else {
            hal_leds_set_status(hal_leds_get_status() | bit(HAL_FLAG_BLUE_LED));
            comm_transmit_packet(&p);
            _delay_ms(500);
            hal_leds_set_status(hal_leds_get_status() & ~bit(HAL_FLAG_BLUE_LED));
        }
    }

}
