// HAL for the low precision timer

#ifndef __TIMER_H
#define __TIMER_H

void hal_timer_init();

// Measures a time interval. Granularity is 10ms.
void hal_timer_start(uint8_t centiseconds, void (*timer_callback)());
void hal_timer_stop();

#endif
