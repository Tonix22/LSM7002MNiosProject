#ifndef SPI_H_
#define SPI_H_


#include <stdint.h>


struct spi_device {
	struct device	dev;
	uint8_t 		id_no;
};

int32_t spi_read(uint8_t *data, uint8_t bytes_number);


int spi_write_then_read(struct spi_device *spi,
                        const unsigned char *txbuf,
                        unsigned n_tx,
                        unsigned char *rxbuf,
                        unsigned n_rx);



#endif /* SPI_H_ */
