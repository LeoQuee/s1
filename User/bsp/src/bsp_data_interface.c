#include "bsp_data_interface.h"
#include "bsp_flash.h"
#include "bsp_w25q128.h"

#include <string.h>


/**
  * @brief 读取系统标志
  * @param p
  */
void sysflag_read(SYSFLAG *p)
{
    W25QXX_Read(XFLASH_ADDR_SYSFLAG, sizeof(SYSFLAG), (uint8_t *)p);
    
#ifndef _USART_PRINT_DEBUG_    
    if(p->year == 0xFFFF)
#endif    
    {
        p->R1 = 100;
        p->R2 = 510;
        p->R3 = 1100;
        p->year = 2020;
        p->month = 11;
        p->day = 5;
        strcpy(p->sn, "demo001");
    }
}

/**
  * @brief 写入系统标志
  * @param p
  */
void sysflag_write(SYSFLAG *p)
{
    W25QXX_Erase_Sector(XFLASH_ADDR_SYSFLAG);
    W25QXX_Write(XFLASH_ADDR_SYSFLAG, sizeof(SYSFLAG), (uint8_t *)p);
}
