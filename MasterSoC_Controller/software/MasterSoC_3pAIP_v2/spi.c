#include "stdint.h"
#include "altera_avalon_spi_regs.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"



int32_t spi_read(uint8_t *data, uint8_t bytes_number) {
	uint32_t cnt = 0;

	/* Enable Slave Select mask. */
	IOWR_ALTERA_AVALON_SPI_SLAVE_SEL(SPI_0_BASE, 1);
	/* Set the SSO bit (force chip select). */
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_0_BASE,ALTERA_AVALON_SPI_CONTROL_SSO_MSK);
	/* Discard any stale data, in case previous communication was interrupted. */
	IORD_ALTERA_AVALON_SPI_RXDATA(SPI_0_BASE);

	while (cnt < bytes_number) {
		/* Wait until txdata register is empty. */
		while ((IORD_ALTERA_AVALON_SPI_STATUS(SPI_0_BASE)
				& ALTERA_AVALON_SPI_STATUS_TRDY_MSK) == 0)
			;
		/* Write data to txdata register. */
		IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_0_BASE, data[cnt]);
		/* Wait until rxdata register is full. */
		while ((IORD_ALTERA_AVALON_SPI_STATUS(SPI_0_BASE)
				& ALTERA_AVALON_SPI_STATUS_RRDY_MSK) == 0)
			;
		/* Read data from rxdata register. */
		data[cnt] = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_0_BASE);
		cnt++;
	}

	/* Wait until the interface has finished transmitting. */
	while ((IORD_ALTERA_AVALON_SPI_STATUS(SPI_0_BASE)
			& ALTERA_AVALON_SPI_STATUS_TMT_MSK) == 0)
		;
	/* Clear the SSO bit (release chip select). */
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_0_BASE, 0);

	return 0;
}



int spi_write_then_read(struct spi_device *spi, const unsigned char *txbuf,
		unsigned n_tx, unsigned char *rxbuf, unsigned n_rx) {
	uint8_t buffer[20] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t byte;

	for (byte = 0; byte < n_tx; byte++) {
		buffer[byte] = (unsigned char) txbuf[byte];
	}
	spi_read(buffer, n_tx + n_rx);
	for (byte = n_tx; byte < n_tx + n_rx; byte++) {
	//	rxbuf[byte - n_tx] = buffer[byte];
		rxbuf[byte] = buffer[byte];
	}
	

	return 0;
}
