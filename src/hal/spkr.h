// HAL for Speaker

#ifndef __SPKR_H
#define __SPKR_H

uint8_t hal_spkr_init();

// duration is in milliseconds, maximum length is 8 seconds
void hal_spkr_beep(uint16_t frequency, uint16_t duration, void (*hal_spkr_beeped_callback)());

#endif
