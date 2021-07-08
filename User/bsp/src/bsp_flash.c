/**
  ******************************************************************************
  * @file    bsp_flash.c
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#include "bsp_flash.h"

/**
  * @brief 擦除Flash
  * @param addr 擦除页地址
  */
void flash_erase(uint32_t addr)
{
    _ASSERT(addr >= FLASH_START_ADDR && addr <= FLASH_END_ADDR);
    _ASSERT((addr&(FLASH_PAGE_SIZE-1)) == 0);
    
    FLASH_Unlock();
    FLASH_ErasePage(addr);
    FLASH_Lock();
}

/**
  * @brief 向Flash中写入数据
  * @details 写入Flash时需要提前擦除
  * @param addr 写入地址（必须为4的倍数）
  * @param p 数据指针
  * @param len 数据长度（必须为4的倍数）
  */
void flash_write(uint32_t addr, void *p, uint16_t len)
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;
    uint32_t *pdata = (uint32_t *)p;
    
    _ASSERT(addr >= FLASH_START_ADDR && addr <= FLASH_END_ADDR);
    _ASSERT((addr&0x3) == 0 && (len&0x3) == 0 && pdata != NULL);
    
    __set_PRIMASK(1); // 关闭总中断
    
    FLASH_Unlock();
    
    while(len > 0)
    {
        if(FLASHStatus == FLASH_COMPLETE)
        {
            FLASHStatus = FLASH_ProgramWord(addr, *pdata);
        }
        
        addr += sizeof(uint32_t);
        pdata ++;
        len -= sizeof(uint32_t);
    }
    
    FLASH_Lock();
    
    __set_PRIMASK(0); // 打开总中断
}
