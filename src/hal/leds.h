// HAL for LEDs

#ifndef __LEDS_H
#define __LEDS_H

uint8_t hal_leds_init();

// Return value and status value of these functions are a bitmasks where 1 means LED ON.
uint8_t hal_leds_get_status();
void hal_leds_set_status(uint8_t status);

enum hal_LedFlags {
	HAL_FLAG_RED_LED	= 0,
	HAL_FLAG_GREEN_LED	= 1,
	HAL_FLAG_BLUE_LED 	= 2
};

#endif