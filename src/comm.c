#include <stddef.h>
#include "hal/hal.h"
#include "comm.h"
#include "protocol.h"

// State is named according to what byte we expect to arrive
enum RxStates {
    RxState_ID          = 0,
    RxState_LENGTH      = 1,
    RxState_DATA        = 2,
    RxState_CHECKSUM    = 3,
    RxState_IGNORE      = 4,
};

enum BufferStates {
    BufferState_EMPTY   = 0,
    BufferState_READY   = 1,    // Reception complete
    BufferState_CORRUPT = 2,    // There was a reception error
};

// Packet buffer and its status
static volatile Packet * volatile rx_packet = NULL;
static volatile enum BufferStates buffer_state = BufferState_EMPTY;

// Receiver state machine status
static volatile enum RxStates rx_state;
static volatile uint8_t rx_remaining_data;
static volatile uint8_t *rx_data_pointer;
static volatile uint8_t rx_checksum;
static volatile uint8_t rx_blocked = 0;

uint8_t comm_wait_for_packet(Packet *p) {

    while (rx_blocked) ;

    rx_packet = p;

    // Reset the state machine
    rx_state = RxState_ID;
    rx_remaining_data = 0;
    rx_data_pointer = NULL;
    rx_checksum = 0;

    while (buffer_state == BufferState_EMPTY) ;

    rx_packet = NULL;

    enum BufferStates state = buffer_state;
    buffer_state = BufferState_EMPTY;

    return state == BufferState_READY ? 0 : 1;
}

void comm_transmit_packet(Packet *p) {

    uint8_t checksum = 0;

    checksum ^= p->id;
    hal_usart_transmit(p->id);

    checksum ^= p->length;
    hal_usart_transmit(p->length);

    uint8_t length = p->length;
    uint8_t *data = p->data;
    while (length > 0) {
        checksum ^= *data;
        hal_usart_transmit(*data);
        data++;
        length--;
    }

    hal_usart_transmit(checksum);
}

static void receive_timeout() {
    buffer_state = BufferState_CORRUPT;
    rx_state = RxState_IGNORE;
}

static void stop_ignoring() {
    rx_state = RxState_ID;
    rx_blocked = 0;
}

void comm_byte_received_callback(uint8_t byte) {

    // Wait until we stop ignoring incoming bytes (in case we have
    // received a corrupted part of a packet and are ignoring the rest
    // of the packet)
    if (rx_packet == NULL) return;

    switch (rx_state) {
        case RxState_ID:
            hal_timer_start(RECEIVE_TIMEOUT, &receive_timeout);
            rx_packet->id = byte;
            rx_state = RxState_LENGTH;
            break;
        case RxState_LENGTH:
            if (byte > 128) {
                hal_timer_stop();
                buffer_state = BufferState_CORRUPT;
                // We are ignoring bytes of a currupt packet
                rx_state = RxState_IGNORE;
                rx_blocked = 1;
                hal_timer_start(RECEIVE_TIMEOUT, &stop_ignoring);
            }
            rx_remaining_data = byte;
            rx_packet->length = byte;
            rx_data_pointer = rx_packet->data;
            rx_state = rx_remaining_data != 0 ? RxState_DATA : RxState_CHECKSUM;
            break;
        case RxState_DATA:
            *rx_data_pointer = byte;
            rx_data_pointer++;
            rx_remaining_data--;
            if (rx_remaining_data == 0) rx_state = RxState_CHECKSUM;
            break;
        case RxState_CHECKSUM:
            hal_timer_stop();
            if (rx_checksum == byte) buffer_state = BufferState_READY;
            else buffer_state = BufferState_CORRUPT;
            break;
        case RxState_IGNORE:
            // Ignore
            break;
    }

    rx_checksum ^= byte;
}
