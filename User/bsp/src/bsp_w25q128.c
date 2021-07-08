/**
  ******************************************************************************
  * @file    bsp_w25q128.c
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#include "bsp_w25q128.h"
#include "bsp_spi.h"

#if (defined PCB_V3) || (defined PCB_V4)

#define W25X_WriteEnable        0x06
#define W25X_ReadSR1            0x05
#define W25X_ReadSR2            0x35
#define W25X_WriteStatusReg     0x01
#define W25X_PageProgram        0x02
#define W25X_ReadData           0x03
#define W25X_SectorErase        0x20
#define W25X_ReadID             0x90

#define W25X_BlockErase         0xD8
#define W25X_ChipErase          0x60

#define EN_FLASH()      GPIO_ResetBits(SPI2_NSS_PORT, SPI2_NSS_PIN)
#define DIS_FLASH()     GPIO_SetBits(SPI2_NSS_PORT, SPI2_NSS_PIN)

void W25QXX_init(void)
{
    spi2_init();
}

uint8_t W25QXX_Read_SR(void)
{
    uint8_t byte;
    
    EN_FLASH();
    spi_readwrite_byte(SPI2, W25X_ReadSR1);
    byte = spi_readwrite_byte(SPI2, 0xff);
    DIS_FLASH();
    
    return byte;
}

void W25QXX_Write_SR(uint8_t sr)
{
    EN_FLASH();
    spi_readwrite_byte(SPI2, W25X_WriteStatusReg);
    spi_readwrite_byte(SPI2, sr);
    DIS_FLASH();
}

void W25QXX_Write_Enable(void)
{
    EN_FLASH();
    spi_readwrite_byte(SPI2, W25X_WriteEnable);
    DIS_FLASH();
}

void W25QXX_Wait_Busy(void)
{
    while((W25QXX_Read_SR()&0x01) == 0x01);
}

void W25QXX_Erase_Sector(uint32_t addr)
{
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    
    EN_FLASH();
    spi_readwrite_byte(SPI2, W25X_SectorErase);
    spi_readwrite_byte(SPI2, (addr>>16)&0xff);
    spi_readwrite_byte(SPI2, (addr>>8)&0xff);
    spi_readwrite_byte(SPI2, addr&0xff);
    DIS_FLASH();
    
    W25QXX_Wait_Busy();
}

void W25QXX_Erase_Bolck(uint32_t addr)
{
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    
    EN_FLASH();
    spi_readwrite_byte(SPI2, W25X_BlockErase);
    spi_readwrite_byte(SPI2, (addr>>16)&0xff);
    spi_readwrite_byte(SPI2, (addr>>8)&0xff);
    spi_readwrite_byte(SPI2, addr&0xff);
    DIS_FLASH();
    
    W25QXX_Wait_Busy();
}

void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    
    EN_FLASH();
    spi_readwrite_byte(SPI2, W25X_ChipErase);
    DIS_FLASH();
    
    W25QXX_Wait_Busy();
}

void W25QXX_Write_Page(uint32_t addr, uint8_t NumByteToWrite, uint8_t *pTxBuff)
{
    uint8_t i;
    
    // 单页写入，不可跨页。写入字节数，不应超过该页的剩余字节数
    
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    
    EN_FLASH();
    spi_readwrite_byte(SPI2, W25X_PageProgram);
    spi_readwrite_byte(SPI2, (addr>>16)&0xff);
    spi_readwrite_byte(SPI2, (addr>>8)&0xff);
    spi_readwrite_byte(SPI2, addr&0xff);
    for(i=0; i<NumByteToWrite; i++)
    {
        spi_readwrite_byte(SPI2, pTxBuff[i]);
    }
    DIS_FLASH();
    
    W25QXX_Wait_Busy();
}

void W25QXX_Write(uint32_t addr, uint32_t NumByteToWrite, uint8_t *pTxBuff)
{
    uint32_t wlen;
    
    while(NumByteToWrite > 0)
    {
        wlen = 256 - (addr&0xff);
        if(NumByteToWrite < wlen)
        {
            wlen = NumByteToWrite;
        }
        
        W25QXX_Write_Page(addr, wlen, pTxBuff);
        addr += wlen;
        pTxBuff += wlen;
        NumByteToWrite -= wlen;
    }
}

void W25QXX_Read(uint32_t addr, uint32_t NumByteToRead, uint8_t *pRxBuff)
{
    uint32_t i;
    
    DISABLE_INT();
    
    EN_FLASH();
    spi_readwrite_byte(SPI2, W25X_ReadData);
    spi_readwrite_byte(SPI2, (addr>>16)&0xff);
    spi_readwrite_byte(SPI2, (addr>>8)&0xff);
    spi_readwrite_byte(SPI2, addr&0xff);
    for(i=0; i<NumByteToRead; i++)
    {
        pRxBuff[i] = spi_readwrite_byte(SPI2, 0xff);
    }
    DIS_FLASH();
    
    ENABLE_INT();
}

uint16_t W25QXX_ReadID(void)
{
    uint8_t d0, d1;
    
    EN_FLASH();
    spi_readwrite_byte(SPI2, W25X_ReadID);
    spi_readwrite_byte(SPI2, 0x00);
    spi_readwrite_byte(SPI2, 0x00);
    spi_readwrite_byte(SPI2, 0x00);
    d0 = spi_readwrite_byte(SPI2, 0xff);
    d1 = spi_readwrite_byte(SPI2, 0xff);
    DIS_FLASH();
    
    return (d0<<8) | d1;
}

#endif
