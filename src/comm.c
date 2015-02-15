#include <stddef.h>
#include "hal/hal.h"
#include "comm.h"
#include "protocol.h"

// State is named according to what byte we expect to arrive
enum RxStates {
    RxState_ID          = 0,
    RxState_Length      = 1,
    RxState_Data        = 2,
    RxState_Checksum    = 3,
    RxState_Ignore      = 4,
};

enum BufferStates {
    BufferState_Empty   = 0,
    BufferState_Ready   = 1,    // Reception complete
    BufferState_Corrupt = 2,    // There was a reception error
};

// Packet buffer and its status
static volatile packet * volatile rx_packet = NULL;
static volatile enum BufferStates buffer_state = BufferState_Empty;

// Receiver state machine status
static volatile enum RxStates rx_state = RxState_ID;
static volatile uint8_t rx_remaining_data = 0;
static volatile uint8_t *rx_data_pointer = NULL;
static volatile uint8_t rx_checksum = 0;
static volatile uint8_t rx_blocked = 0;

uint8_t comm_wait_for_packet(packet *p) {

    while (rx_blocked) ;

    rx_packet = p;

    // Reset the state machine
    rx_state = RxState_ID;
    rx_remaining_data = 0;
    rx_data_pointer = NULL;
    rx_checksum = 0;

    while (buffer_state == BufferState_Empty) ;

    rx_packet = NULL;

    enum BufferStates state = buffer_state;
    buffer_state = BufferState_Empty;

    return state == BufferState_Ready ? 0 : 1;
}

void comm_transmit_packet(packet *p) {

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
    buffer_state = BufferState_Corrupt;
    rx_state = RxState_Ignore;
}

static void stop_ignoring() {
    rx_state = RxState_ID;
    rx_blocked = 0;
}

void comm_byte_received_callback(uint8_t byte) {

    if (rx_packet == NULL) return;

    switch(rx_state) {
        case RxState_ID:
            hal_timer_start(RECEIVE_TIMEOUT, &receive_timeout);
            rx_packet->id = byte;
            rx_state = RxState_Length;
            break;
        case RxState_Length:
            if (byte > 128) {
                hal_timer_stop();
                buffer_state = BufferState_Corrupt;
                // We are ignoring bytes of a currupt packet
                rx_state = RxState_Ignore;
                rx_blocked = 1;
                hal_timer_start(RECEIVE_TIMEOUT, &stop_ignoring);
            }
            rx_remaining_data = byte;
            rx_packet->length = byte;
            rx_data_pointer = rx_packet->data;
            rx_state = rx_remaining_data != 0 ? RxState_Data : RxState_Checksum;
            break;
        case RxState_Data:
            *rx_data_pointer = byte;
            rx_data_pointer++;
            rx_remaining_data--;
            if (rx_remaining_data == 0) rx_state = RxState_Checksum;
            break;
        case RxState_Checksum:
            hal_timer_stop();
            if (rx_checksum == byte) buffer_state = BufferState_Ready;
            else buffer_state = BufferState_Corrupt;
            break;
        case RxState_Ignore:
            // Ignore
            break;
    }

    rx_checksum ^= byte;
}
