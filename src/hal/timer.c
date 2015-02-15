#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "../bit_manip.h"

static void (*callback)();
static uint8_t centiseconds_remaining = 0;

ISR(TIMER0_COMPA_vect) {
    centiseconds_remaining--;
    if (centiseconds_remaining == 0) {
        // Stop the timer
        TCCR0B &= ~(bit(CS02) | bit(CS00));
        if (callback != NULL) {
            callback();
        }
    }
}

void hal_timer_init() {
    // When running with /1024 prescaler compare approx. every 10ms
    OCR0A = 80;

    // Clear Timer on Compare mode
    TCCR0A |= bit(WGM01);

    // Enable Output Compare Match A Interrupt
    TIMSK0 |= bit(OCIE0A);
}

void hal_timer_start(uint8_t centiseconds, void (*timer_callback)()) {
    centiseconds_remaining = centiseconds;
    callback = timer_callback;
    TCNT0 = 0;

    if (centiseconds_remaining > 0) {
        // Start the timer with the /1024 prescaler
        TCCR0B |= bit(CS02) | bit(CS00);
    }
}

void hal_timer_stop() {
    callback = NULL;
    // Stop the timer
    TCCR0B &= ~(bit(CS02) | bit(CS00));
}
