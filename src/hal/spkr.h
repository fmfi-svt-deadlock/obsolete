// HAL for Speaker

#ifndef __SPKR_H
#define __SPKR_H

uint8_t hal_spkr_init();

// duration is in milliseconds, maximum length is 8 seconds
// spkr_beeped_callback will be called when the beep finishes. This method
// will be called from the interrupt handler. Make sure every global
// variable changed by this method is defined as `volatile` and that this
// is fast -- it mustn't block the interrupt handler for too long.
void hal_spkr_beep(uint16_t frequency, uint16_t duration,
                   void (*spkr_beeped_callback)());

#endif
