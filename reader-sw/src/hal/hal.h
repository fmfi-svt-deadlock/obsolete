// Contains includes of all HAL components and prototypes for initialization
// routines

#ifndef __HAL_H
#define __HAL_H

#include <avr/io.h>

#include "../bit_manip.h"

#include "leds.h"
#include "spkr.h"
#include "usart.h"
#include "spi.h"
#include "timer.h"

// ------ Hardware Abstraction Layer configuration ------ //

// # LEDs
// LED connections are fully configurable by the defines below:
#define LED_R_DDR   DDRB
#define LED_R_PORT  PORTB
#define LED_R_PIN   2

#define LED_G_DDR   DDRB
#define LED_G_PORT  PORTB
#define LED_G_PIN   0

#define LED_B_DDR   DDRA
#define LED_B_PORT  PORTA
#define LED_B_PIN   0

// # Speaker
// The speaker must be connected to TOCC2 (PORTA 3) pin.
// To change this, rewrite spkr.c
// DO NOT change these defines:
#define SPKR_DDR   DDRA
#define SPKR_PORT  PORTA
#define SPKR_PIN   3
// This module uses peripheral Timer/Counter 1 and 2

// # USART
// This module uses peripheral USART0
// These values will configure USART0 as described here:
// https://github.com/fmfi-svt-gate/server/wiki/Controller-%E2%86%94-Reader-Protocol#hardware-layer
// These values presume that the MCU clock is running at 8MHz

// Sets the baud rate to 19200 bauds. Presumes usage of a Normal speed mode.
#define USART_UBRR  25

// Sets the frame format (1 start, 8 data, even parity, 1 stop bits)
#define USART_FRAME (bit(UPM01) | bit(UCSZ00) | bit (UCSZ01))

// # SPI
// This module uses peripheral SPI
// Correct values for communication with RFID-RC522 are hardcoded in the spi.c
// (If the RFID module changes rewrite would be necessary anyway)
#define SPI_DDR        DDRA
#define SPI_PORT       PORTA
#define SPI_MOSI_PIN   6
#define SPI_SCK_PIN    4
#define SPI_SS_PIN     7

#define SPI_RC522_RST_DDR   DDRB
#define SPI_RC522_RST_PORT  PORTB
#define SPI_RC522_RST_PIN   1

// # Timer
// This module uses peripheral Timer/Counter 0

// ------------------------------------------------------ //

/**
  * This initializes HAL and configures the HW in the following way:
  *
  *  * Prepares LEDs
  *  * Prepares timer for beeping
  *  * Prepares USART for communicating with the Controller
  *  * Prepares SPI for communicating with the RFID-RC552
  *  * Prepares Low-precision timer for th receive timeout measurement
  */
void hal_init(void (*usart_recv_callback)(uint8_t byte));


#endif
