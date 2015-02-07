// HAL for SPI
// Configures SPI so that it is capable of communicating with RFID-RC522

#ifndef __SPI_H
#define __SPI_H

uint8_t hal_spi_init();
uint8_t hal_spi_transfer(uint8_t data);

#endif