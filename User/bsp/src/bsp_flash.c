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
  * @brief ����Flash
  * @param addr ����ҳ��ַ
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
  * @brief ��Flash��д������
  * @details д��Flashʱ��Ҫ��ǰ����
  * @param addr д���ַ������Ϊ4�ı�����
  * @param p ����ָ��
  * @param len ���ݳ��ȣ�����Ϊ4�ı�����
  */
void flash_write(uint32_t addr, void *p, uint16_t len)
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;
    uint32_t *pdata = (uint32_t *)p;
    
    _ASSERT(addr >= FLASH_START_ADDR && addr <= FLASH_END_ADDR);
    _ASSERT((addr&0x3) == 0 && (len&0x3) == 0 && pdata != NULL);
    
    __set_PRIMASK(1); // �ر����ж�
    
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
    
    __set_PRIMASK(0); // �����ж�
}
