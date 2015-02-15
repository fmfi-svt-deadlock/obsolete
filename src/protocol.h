// Contains definitions of structures and packet IDs for the protocol

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

typedef struct {
    uint8_t id;
    uint8_t length;
    uint8_t data[128];
} packet;

typedef struct {
    uint8_t led_status;
    uint8_t sound_status;
} Status;

#define PROTOCOL_PACKET_ID_MAX  0x0D

enum protocol_PacketIds {
    packet_InvalidPacket        = 0x00,
    packet_AnswerToReset        = 0x01,
    packet_ContinueBoot         = 0x02,
    packet_FirmwareUpgrade      = 0x03,
    packet_FWUpgradeReady       = 0x04,
    packet_FWWrite              = 0x05,
    packet_FWWriteAck           = 0x06,
    packet_FWUpdateFinish       = 0x07,
    packet_GetStatus            = 0x08,
    packet_SetLed               = 0x09,
    packet_ACK                  = 0x0A,
    packet_Beep                 = 0x0B,
    packet_RFIDSend             = 0x0C,
    packet_RFIDSendComplete     = 0x0D,
    packet_RxError              = 0xFF
};

// Receive timeout is 90ms
#define RECEIVE_TIMEOUT         9

#endif
