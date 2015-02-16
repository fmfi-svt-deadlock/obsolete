// Includes communication protocol parsing

#ifndef __COMM_H
#define __COMM_H

#include "protocol.h"

// This function will block until a new packet is received or reception error
// will occur.
// Comm module has no internal buffer. If another packets
// arrives (or starts to arrive) when this function is not being called
// this packet will be lost (due to memory constraints).
// Returns 0 if a packet was received, 1 on reception error
uint8_t comm_wait_for_packet(Packet *p);

void comm_transmit_packet(Packet *p);

// This function should be called by a hardware abstraction layer when
// new byte is received.
void comm_byte_received_callback(uint8_t byte);

#endif
