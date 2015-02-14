#include <stddef.h>
#include <util/delay.h>
#include "hal/hal.h"

#include "comm.h"
#include "protocol.h"

static packet p;

// ACK/Status packet is an universal response to the most of the packets
static void sendACK() {
    Status *s = (Status*) p.data;
    // TODO: Checking periodically for RFID module health.
    s->rfid_status = 0;
    // Bit flags of leds used by the protocol and the HAL are intentionally
    // compatible
    s->led_status = hal_leds_get_status();
    // TODO: Sound playback
    s->sound_status = 0;

    p.id = packet_ACK;
    p.length = 3;
    comm_transmit_packet(&p);
}

// __attribute__((OS_main)) tells the compiler that this function never
// returns and saves us precious space
__attribute__((OS_main)) int main(void) {

    hal_init(&comm_byte_received_callback);

    // If we got here, it means we are out of bootloader. Protcol
    // requires us to send an ACK packet
    sendACK();

    // This is the main handling loop
    // It handles all packets that can arrive in Normal or Degraded mode
    while(1) {
        if (comm_wait_for_packet(&p) == 0) {
            switch (p.id) {
                case packet_GetStatus:
                    sendACK();
                    break;
                case packet_SetLed:
                    // Bit flags are compatible
                    hal_leds_set_status(p.data[0]);
                    sendACK();
                    break;
                case packet_Beep:
                    // TODO
                    sendACK();
                    break;
                case packet_RFIDSend:
                    // We will send data from this packet, and replace them
                    // in-place with what we receive
                    for (uint8_t i = 0; i < p.length; i++) {
                        p.data[i] = hal_spi_transfer(p.data[i]);
                    }
                    // And suddenly, we are a response packet :)
                    p.id = packet_RFIDSendComplete;
                    comm_transmit_packet(&p);
                    break;
                default:
                    // Unacceptable packet was received in this mode
                    p.id = packet_InvalidPacket;
                    p.length = 0;
                    comm_transmit_packet(&p);
                    break;
            }
        } else {
            // Reception error
            p.id = packet_RxError;
            p.length = 0;
            comm_transmit_packet(&p);
        }
    }

}
