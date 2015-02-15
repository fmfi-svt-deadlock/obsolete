#include <stddef.h>
#include <util/delay.h>
#include "hal/hal.h"

#include "comm.h"
#include "protocol.h"

// We must store 2 packets: One we have previously sent (in case we need
// to retransmit it) and one newly arriving.
static packet p[2];
// This distinguishes between those two packets
static uint8_t current_packet = 0;

static uint16_t tone_buffer[16];
static uint8_t tone_counter = 0;
static uint8_t tone_max = 0;
static uint8_t loop_forever = 0;

static void beepCallback() {
    if ((tone_counter == tone_max) && loop_forever) {
        tone_counter = 0;
    }
    if (tone_counter < tone_max) {
        hal_spkr_beep(tone_buffer[tone_counter*2],
                      tone_buffer[(tone_counter*2)+1], &beepCallback);
        tone_counter++;
    }
}

static void transmit_and_flip() {
    comm_transmit_packet(&p[current_packet]);
    current_packet ^= 1;
}

// ACK/Status packet is an universal response to most of the packets
static void sendACK() {
    Status *s = (Status*) p[current_packet].data;
    // Bit flags of leds used by the protocol and the HAL are intentionally
    // compatible
    s->led_status = hal_leds_get_status();
    s->sound_status = (loop_forever ? 0xFF : (tone_max - tone_counter));

    p[current_packet].id = packet_ACK;
    p[current_packet].length = 2;
    transmit_and_flip();
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
        if (comm_wait_for_packet(&p[current_packet]) == 0) {
            switch (p[current_packet].id) {
                case packet_GetStatus:
                    sendACK();
                    break;
                case packet_SetLed:
                    // Bit flags are compatible
                    hal_leds_set_status(p[current_packet].data[0]);
                    sendACK();
                    break;
                case packet_Beep:
                    hal_spkr_killbeep();
                    tone_counter = 0;
                    tone_max = (p[current_packet].length-1) / 4;
                    for (uint8_t i = 0; i < (p[current_packet].length-1);
                         i += 2) {
                        tone_buffer[i/2] = p[current_packet].data[i] |
                                           (p[current_packet].data[i+1] << 8);
                    }
                    loop_forever = p[current_packet]
                                   .data[p[current_packet].length-1];
                    beepCallback();
                    sendACK();
                    break;
                case packet_RFIDSend:
                    // We will send data from this packet, and replace them
                    // in-place with what we receive
                    for (uint8_t i = 0; i < p[current_packet].length; i++) {
                        p[current_packet].data[i] = 
                            hal_spi_transfer(p[current_packet].data[i]);
                    }
                    // And suddenly, we are a response packet :)
                    p[current_packet].id = packet_RFIDSendComplete;
                    transmit_and_flip();
                    break;
                case packet_RxError:
                    // Retransmit the last packet
                    comm_transmit_packet(&p[current_packet ^ 1]);
                    break;
                default:
                    // Unacceptable packet was received in this mode
                    p[current_packet].id = packet_InvalidPacket;
                    p[current_packet].length = 0;
                    transmit_and_flip();
                    break;
            }
        } else {
            // Reception error
            p[current_packet].id = packet_RxError;
            p[current_packet].length = 0;
            // Intentionally NOT using transmit_and_flip
            comm_transmit_packet(&p[current_packet]);
        }
    }

}
