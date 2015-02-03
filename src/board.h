// This file is the single source of truth of what is connected to what on the Reader RevA board

#ifndef __BOARD_H
#define __BOARD_H

#include <avr/io.h>

// ---- User Interface ----
#define LED_R_DDR	DDRB
#define LED_R_PORT	PORTB
#define LED_R_PIN	0

#define LED_G_DDR	DDRB
#define LED_G_PORT	PORTB
#define LED_G_PIN	2

#define LED_B_DDR	DDRA
#define LED_B_PORT	PORTA
#define LED_B_PIN	3

#define SPKR_DDR	DDRA
#define SPKR_PORT	PORTA
#define SPKR_PIN	0

// ---- Other interfaces ----
// These are used by the MCU-specific HAL to determine which interface is used in case
// of multiple interfaces being available

#define RDR_SPI_NUM			0
#define CTRL_USART_NUM		0

#endif
