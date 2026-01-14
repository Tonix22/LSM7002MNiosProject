#ifndef SPI_H_
#define SPI_H_


#include <stdint.h>


struct spi_device {
	struct device	dev;
	uint8_t 		id_no;
};

int32_t spi_read(uint8_t *data, uint8_t bytes_number, uint8_t slave);


int spi_write_then_read(struct spi_device *spi,
                        const unsigned char *txbuf,
                        unsigned n_tx,
                        unsigned char *rxbuf,
                        unsigned n_rx, uint8_t slave);



#endif /* SPI_H_ */
