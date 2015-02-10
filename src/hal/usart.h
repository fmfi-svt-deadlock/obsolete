// HAL for USART

#ifndef __USART_H
#define __USART_H

uint8_t hal_usart_init(void (*usart_recv_callback)(uint8_t byte));
void hal_usart_transmit(uint8_t *data);

#endif
