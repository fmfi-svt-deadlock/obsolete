#include <stddef.h>
#include <util/delay.h>
#include "hal/hal.h"

#include "comm.h"
#include "protocol.h"

static packet p;

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

// ACK/Status packet is an universal response to most of the packets
static void sendACK() {
    Status *s = (Status*) p.data;
    // TODO: Checking periodically for RFID module health.
    s->rfid_status = 0;
    // Bit flags of leds used by the protocol and the HAL are intentionally
    // compatible
    s->led_status = hal_leds_get_status();
    s->sound_status = (loop_forever ? 0xFF : (tone_max - tone_counter));

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
                    hal_spkr_killbeep();
                    tone_counter = 0;
                    tone_max = (p.length-1) / 4;
                    for (uint8_t i = 0; i < (p.length-1); i += 2) {
                        tone_buffer[i/2] = p.data[i] | (p.data[i+1] << 8);
                    }
                    loop_forever = p.data[p.length-1];
                    beepCallback();
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
