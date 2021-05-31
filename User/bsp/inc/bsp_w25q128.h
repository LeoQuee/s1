#ifndef _BSP_W25Q128_H_
#define _BSP_W25Q128_H_

#include "global_config.h"

#ifdef PCB_V3

#define W25QXX_SIZE     0x1000000 // 16M×Ö½Ú

void W25QXX_init(void);
uint8_t W25QXX_Read_SR(void);
void W25QXX_Write_SR(uint8_t sr);
void W25QXX_Write_Enable(void);
void W25QXX_Wait_Busy(void);
void W25QXX_Erase_Sector(uint32_t addr);
void W25QXX_Erase_Bolck(uint32_t addr);
void W25QXX_Erase_Chip(void);
void W25QXX_Write_Page(uint32_t addr, uint8_t NumByteToWrite, uint8_t *pTxBuff);
void W25QXX_Write(uint32_t addr, uint32_t NumByteToWrite, uint8_t *pTxBuff);
void W25QXX_Read(uint32_t addr, uint32_t NumByteToRead, uint8_t *pRxBuff);
uint16_t W25QXX_ReadID(void);

#endif

#endif
