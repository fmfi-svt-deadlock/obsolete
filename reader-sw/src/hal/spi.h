// HAL for SPI
// Configures SPI so that it is capable of communicating with RFID-RC522

#ifndef __SPI_H
#define __SPI_H

void hal_spi_init();

// This method sends one byte to the RFID-RC522. Every communication on SPI
// consists of exchange of one byte, therefore this method returns received
// byte in turn. This function must be called between
// `hal_spi_begin_transaction()` and `hal_spi_end_transaction()`
uint8_t hal_spi_transfer(uint8_t data);

// Selects and unselects the slave
void hal_spi_begin_transaction();
void hal_spi_end_transaction();

#endif
