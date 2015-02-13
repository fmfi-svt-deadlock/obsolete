// Includes communication protocol parsing

#ifndef __COMM_H
#define __COMM_H

#include "protocol.h"

// This function will block until a new packet is received or reception error
// will occur.
// Comm module has an internal buffer for one packet only. If another packets
// arrives (or starts to arrive) the original unread packet will be lost.
// Returns 0 if a packet was received, 1 on reception error
uint8_t comm_wait_for_packet(packet *p);

void comm_transmit_packet(packet *p);

// This function should be called by a hardware abstraction layer when
// new byte is received.
void comm_byte_received_callback(uint8_t byte);

#endif
