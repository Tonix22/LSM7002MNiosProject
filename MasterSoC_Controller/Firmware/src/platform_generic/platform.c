/***************************************************************************//**
 *   @file   Platform.c
 *   @brief  Implementation of Platform Driver.
 *   @author DBogdan (dragos.bogdan@analog.com)
********************************************************************************
 * Copyright 2013(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "stdint.h"
#include "util.h"
#include "platform.h"
//#include "dupin.h"
#include "config.h"
#include <stdbool.h>
//#include "dupin_small.h"
#define MICRO_DELAY_FACTOR 5
#define MILLI_DELAY_FACTOR  5000


/***************************************************************************//**
 * @brief usleep
*******************************************************************************/
static inline void usleep(unsigned long usleep)
{
	for(uint32_t idx = 0;idx < (usleep * MICRO_DELAY_FACTOR); idx++) asm volatile("");
}

uint32_t spidev_interface_transact(const uint32_t data, const bool readback)
{
	
	unsigned char txbuf[4];
    unsigned char rxbuf[4];
	if(readback) // read
	{	
		rxbuf[0] = (data >> 24) & 0xFF; 
		rxbuf[1] = (data >> 16) & 0xFF;
	
		spi_read_API(rxbuf,2);
		return \
        (((uint32_t)rxbuf[0]) << 24) |
        (((uint32_t)rxbuf[1]) << 16) |
        (((uint32_t)rxbuf[2]) << 8) |
        (((uint32_t)rxbuf[3]) << 0);
	}else // write
	{
		txbuf[0] = (data >> 24);
		txbuf[1] = (data >> 16);
		txbuf[2] = (data >> 8);
		txbuf[3] = (data >> 0);
		spi_write_API(txbuf,4);
	}
	
    return 0;
}

/***************************************************************************//**
 * @brief spi_init
*******************************************************************************/
int32_t spi_init(uint32_t device_id,
				 uint8_t  clk_pha,
				 uint8_t  clk_pol)
{/*
	spi_config_reg spiConfig;
	spiConfig.cpol = clk_pol;
    spiConfig.cpha = clk_pha;
    spiConfig.mode = 0;
    spiConfig.clkDivider = 10;
    spiConfig.ssSetup = 2;
    spiConfig.ssHold = 2;
    spiConfig.ssDisable = 2;
	spi_configure (SPI0_BASE, &spiConfig);*/
	return 0;
}

/***************************************************************************//**
 * @brief spi_read_API
*******************************************************************************/
int32_t spi_read_API(uint8_t *data,
				 uint8_t bytes_number)
{
	unsigned char *rx = data;
	/*for(int i=0; i < bytes_number ; i++,rx++)
	{
		spi_select_device(SPI0_BASE, 0);
		*rx = spi_rxdat(SPI0_BASE);
		spi_deselect_device(SPI0_BASE, 0);
	}*/
    unsigned char tx[2] = {rx[0], rx[1]};
    spi_write_then_read(NULL, tx, 2, rx, bytes_number);

   // printf("Hola, estas en la funcion de spi_read_API\n");
	return 0;
}

int spi_write_API(const unsigned char *txbuf, unsigned n_tx)
{
	const unsigned char *tx = txbuf;
	
	/*for(int i=0; i < n_tx; i++,tx++)
	{
		usleep(10);
		spi_select_device(SPI0_BASE, 0);
		spi_txbyte(SPI0_BASE, *tx);
		spi_deselect_device(SPI0_BASE, 0);
	}*/
    spi_write_then_read(NULL, tx, n_tx, NULL, 0);

	//printf("Hola, estas en la funcion de spi_write_API\n");
	return 0;
}
/***************************************************************************//**
 * @brief spi_write_then_read
*******************************************************************************/
/*int spi_write_then_read(struct spi_device *spi,
		const unsigned char *txbuf, unsigned n_tx,
		unsigned char *rxbuf, unsigned n_rx)
{
	
	const unsigned char *tx = txbuf;
	
	
	for(int i=0; i < n_tx; i++,tx++)
	{
		usleep(10);
		spi_select_device(SPI0_BASE, 0);
		spi_txbyte(SPI0_BASE, *tx);
		spi_deselect_device(SPI0_BASE, 0);
	}
	
	spi_read_API(rxbuf,n_rx);

	return 0;
}
*/
/***************************************************************************//**
 * @brief gpio_init
*******************************************************************************/
void gpio_init(uint32_t device_id)
{
//	GPIO->output_enable = 0x0000FFFF;
}

/***************************************************************************//**
 * @brief gpio_direction
*******************************************************************************/
void gpio_direction(uint8_t pin, uint8_t direction)
{/*
	if(direction == 1) // set as OUTPUT
	{
		GPIO1->OUTPUT_ENABLE |= (1<<pin);
	}
	else // set as input
	{
		GPIO1->OUTPUT_ENABLE &=~(1<<pin);
	}*/
}

/***************************************************************************//**
 * @brief gpio_is_valid
*******************************************************************************/
bool gpio_is_valid(int number)
{
	if(number < 32)
	{
		return true;
	}
	else
	{
		return false;
	}
	return 0;
}

/***************************************************************************//**
 * @brief gpio_data
*******************************************************************************/
void gpio_data(uint8_t pin, uint8_t* data)
{
//	*data = GPIO1->INPUT & (1<<pin);
}

/***************************************************************************//**
 * @brief gpio_set_value
*******************************************************************************/
void gpio_set_value(unsigned gpio, int value)
{/*
	if(value == 1)
	{
		GPIO1->OUTPUT|=(1<<gpio);
	}
	else if(value == 0)
	{
		GPIO1->OUTPUT&=~(1<<gpio);
	}*/
}

/***************************************************************************//**
 * @brief udelay
*******************************************************************************/
void udelay(unsigned long usecs)
{
	for(uint32_t idx = 0;idx < (usecs * MICRO_DELAY_FACTOR); idx++) asm volatile("");
}

/***************************************************************************//**
 * @brief mdelay
*******************************************************************************/
void mdelay(unsigned long msecs)
{
	for(uint32_t idx = 0;idx < (msecs * MILLI_DELAY_FACTOR) ;idx++) asm volatile("");
}

/***************************************************************************//**
 * @brief msleep_interruptible
*******************************************************************************/
unsigned long msleep_interruptible(unsigned int msecs)
{

	return 0;
}
