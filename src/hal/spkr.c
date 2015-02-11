#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hal.h"
#include "spkr.h"
#include "../bit_manip.h"

static void (*beeped_callback)() = NULL;

uint8_t hal_spkr_init() {

    // -- Beeping timer initialization
    // Set speaker pin as output
    SPKR_DDR |= bit(SPKR_PIN);

    // Set Output Compare Multiplexor to use TOCC2 for OC1B
    TOCPMSA0 |= bit(TOCC2S0);

    // Enable TOCC2 output
    TOCPMCOE |= bit(TOCC2OE);

    // Clear timer on Compare Match
    TCCR1B |= bit(WGM12);

    // Toggle OC1B on Compare Match (Due to Output Mux setting this will
    // toggle pin TOCC2)
    TCCR1A |= bit(COM1B0);

    // -- Duration timer initialization
    // Enable Output Compare Interrupt for Output Compare A
    TIMSK2 |= bit(OCIE2A);

	return 0;
}

// Interrupt Service Routine for Timer2 Compare A
ISR(TIMER2_COMPA_vect) {

    // Stop the duration timer
    TCCR2B &= ~(bit(CS22) | bit(CS20));
    // Reset the duration timer
    TCNT2 = 0;

    // Stop the beeping timer
    TCCR1B &= ~bit(CS11);

    if (beeped_callback != NULL) {
        beeped_callback();
    }
}

void hal_spkr_beep(uint16_t frequency, uint16_t duration,
                   void (*spkr_beeped_callback)()) {

    beeped_callback = spkr_beeped_callback;

    // Disable interrupts. TCNT and OCR2A registers are 16 bit registers,
    // consisting of 2 8-bit registers, updates of which must be atomic.
    cli();

    // Reset Timer/Counter 2
    TCNT2 = 0;

    // Set duration timer running at 8 MHz with 1024 prescaler
    // This is not precise, but we don't need such precision anyway
    OCR2A = duration*8;

    sei();

    if (frequency != 0) {
        cli();

        // Reset Timer 1
        TCNT1 = 0;

        // Set output compare register of Timer1 to the tone period
        OCR1B = 500000 / frequency;
        // Waweform generation mode 4 (Clear Timer on Compare) clears the
        // timer on compare match with OCR1A. But we must use OCR1B if we
        // want to toggle even TOCC (TOCC2). This is a hack so that timer
        // will be cleared.
        OCR1A = 500000 / frequency;

        // Reenable interrupts
        sei();

        // Start the timer with /8 prescaler (1MHz)
        TCCR1B |= bit(CS11);
    }

    // Start duration timer (/1024 prescaler)
    TCCR2B |= bit(CS22) | bit(CS20);
}
