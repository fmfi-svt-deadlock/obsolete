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
}

uint8_t hal_spi_transfer(uint8_t data) {

    // Select slave
    SPI_PORT &= ~bit(SPI_SS_PIN);

    // Transfer
    SPDR = data;
    // Wait
    while(!is_set(SPSR, bit(SPIF))) ;

    // Unselect slave
    SPI_PORT |= bit(SPI_SS_PIN);

    // Return received data
    return SPDR;
}
