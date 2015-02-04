// Contains includes of all HAL components and prototypes for initialization routines

#ifndef __HAL_H
#define __HAL_H

#include "../board.h"

#include "leds.h"
#include "spkr.h"
#include "usart.h"
#include "spi.h"

// TODO: Come up with a way to specify MCU-independent Reader <-> Controller USART settings
//       somewhere.

uint8_t hal_init(void (*usart_recv_callback)(uint8_t byte));

#endif