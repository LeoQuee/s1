#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#include "global_config.h"

void spi2_init(void);

uint8_t spi_readwrite_byte(SPI_TypeDef* SPIx, uint8_t TxData); /*!< SPI¶ÁÐ´Ò»¸ö×Ö½Ú */

#endif
