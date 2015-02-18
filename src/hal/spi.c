#include <avr/io.h>

#include "hal.h"
#include "spi.h"

void hal_spi_init() {

    // MOSI, SCK and SS as outputs
    SPI_DDR  |= bit(SPI_MOSI_PIN) | bit(SPI_SCK_PIN) | bit(SPI_SS_PIN);

    // Hold SS high (SS is inverted, this means unselect slave)
    SPI_PORT |= bit(SPI_SS_PIN);

    // Enable SPI, Master, set clock rate fck / 16
    SPCR = bit(SPE) | bit(MSTR) | bit(SPR0);

    SPI_RC522_RST_DDR |= bit(SPI_RC522_RST_PIN);
    SPI_RC522_RST_PORT |= bit(SPI_RC522_RST_PIN);
}

void hal_spi_begin_transaction() {
    // Select slave
    SPI_PORT &= ~bit(SPI_SS_PIN);
}

void hal_spi_end_transaction() {
    // Unselect slave
    SPI_PORT |= bit(SPI_SS_PIN);
}

uint8_t hal_spi_transfer(uint8_t data) {

    // Transfer
    SPDR = data;

    // Wait
    while(!is_set(SPSR, bit(SPIF))) ;

    // Return received data
    return SPDR;
}
