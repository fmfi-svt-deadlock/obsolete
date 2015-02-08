#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hal.h"
#include "spkr.h"
#include "../bit_manip.h"

static void (*beeped_callback)() = NULL;

uint8_t hal_spkr_init() {
	
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

	return 0;
}

void hal_spkr_beep(uint16_t period, uint16_t duration, void (*hal_spkr_beeped_callback)()) {

    // Operations on these registers must be atomic, disable interrupts
    cli();

    // Reset Timer 1
    TCNT1 = 0;

    beeped_callback = hal_spkr_beeped_callback;

    // Set output compare register of Timer1 to the tone period
    OCR1B = period / 2;
    // Waweform generation mode 4 (Clear Timer on Compare) clears the timer
    // on compare match with OCR1A. But we must use OCR1B if we want to
    // toggle even TOCC (TOCC2). This is a hack so that timer will be cleared.
    OCR1A = period / 2;

    // Reenable interrupts
    sei();

    // Start the timer with /8 prescaler (1MHz)
    TCCR1B |= bit(CS11);

}
