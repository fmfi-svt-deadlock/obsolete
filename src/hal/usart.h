// HAL for USART

#ifndef __USART_H
#define __USART_H

// usart_recv_callback will be called for every received byte. This method
// will be called from the interrupt handler. Make sure every global
// variable changed by this method is defined as `volatile` and that this
// is fast -- it mustn't block interrupt handler for too long.
uint8_t hal_usart_init(void (*usart_recv_callback)(uint8_t byte));

// This method transmits array of bytes until it encounters '\0'. This
// method is blocking, so DON'T call it from interrupt handler.
void hal_usart_transmit(uint8_t *data);

#endif
