#include "bsp_spi.h"

void spi2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
#ifdef PCB_V3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(SPI2_RCC, ENABLE);
    
    //PB12---SPI2 NSS
    GPIO_InitStructure.GPIO_Pin   = SPI2_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_NSS_PORT, &GPIO_InitStructure);

    //PB13---SPI2 SCK
    //PB14---SPI2 MISO
    //PB15---SPI2 MOSI
    GPIO_InitStructure.GPIO_Pin   = SPI2_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_SCK_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = SPI2_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_MOSI_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = SPI2_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_MISO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(SPI2_NSS_PORT,  SPI2_NSS_PIN);
#endif

    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;//SPI_Mode_Master    SPI_Mode_Slave
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;    //SPI_NSS_Hard        SPI_NSS_Soft
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    
    SPI_Init(SPI2, &SPI_InitStructure);
    
    SPI_Cmd(SPI2, ENABLE);
    //SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE,ENABLE);

    spi_readwrite_byte(SPI2, 0xff);//启动传输
}

/**
  * @brief SPI读写一个字节
  * @param SPIx
  * @param TxData:要写入的字节
  * @return 读取到的字节
  */
uint8_t spi_readwrite_byte(SPI_TypeDef* SPIx, uint8_t TxData)
{
    uint8_t retry=0;
  
    while((SPIx->SR&1<<1)==0)
    {
        retry++;
        //if(retry>200) return 0;
    }
    
    SPIx->DR=TxData;
    retry=0;
    
    while((SPIx->SR&1<<0)==0)
    {
        retry++;
        //if(retry>200) return 0;
    }
    
    return SPIx->DR;
}
