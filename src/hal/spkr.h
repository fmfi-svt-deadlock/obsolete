// HAL for Speaker

#ifndef __SPKR_H
#define __SPKR_H

uint8_t hal_spkr_init();
void hal_spkr_beep(uint16_t period, uint16_t duration, void (*hal_spkr_beeped_callback)());

#endif
