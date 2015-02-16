// Contains definitions of structures and packet IDs for the protocol

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

typedef struct {
    uint8_t id;
    uint8_t length;
    uint8_t data[128];
} Packet;

typedef struct {
    uint8_t led_status;
    uint8_t sound_status;
} Status;

#define PROTOCOL_PACKET_ID_MAX  0x0D

enum protocol_PacketIds {
    packet_INVALID_PACKET        = 0x00,
    packet_ANSWER_TO_RESET       = 0x01,
    packet_CONTINUE_BOOT         = 0x02,
    packet_FIRMWARE_UPGRADE      = 0x03,
    packet_FW_UPGRADE_READY      = 0x04,
    packet_FW_WRITE              = 0x05,
    packet_FW_WRITE_ACK          = 0x06,
    packet_FW_UPDATE_FINISH      = 0x07,
    packet_GET_STATUS            = 0x08,
    packet_SET_LED               = 0x09,
    packet_ACK                   = 0x0A,
    packet_BEEP                  = 0x0B,
    packet_RFID_SEND             = 0x0C,
    packet_RFID_SEND_COMPLETE    = 0x0D,
    packet_RX_ERROR              = 0xFF,
};

// Receive timeout is 90ms
#define RECEIVE_TIMEOUT         9

#endif
