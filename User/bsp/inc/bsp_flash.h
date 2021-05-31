#ifndef _BSP_FLASH_H_
#define _BSP_FLASH_H_

#include "global_config.h"

#define FLASH_PAGE_SIZE   0x800         /*!< ������ÿҳ2K */
#define FLASH_START_ADDR  0x08030000    /*!< �洢����ʼ��ַ */
#define FLASH_END_ADDR    0x0803FFFF    /*!< �洢��������ַ */

void flash_erase(uint32_t addr);    /*!< ����Flash */
void flash_write(uint32_t addr, void *p, uint16_t len); /*!< ��Flash��д������ */

#endif
