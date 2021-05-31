#ifndef _BSP_FLASH_H_
#define _BSP_FLASH_H_

#include "global_config.h"

#define FLASH_PAGE_SIZE   0x800         /*!< 大容量每页2K */
#define FLASH_START_ADDR  0x08030000    /*!< 存储区起始地址 */
#define FLASH_END_ADDR    0x0803FFFF    /*!< 存储区结束地址 */

void flash_erase(uint32_t addr);    /*!< 擦除Flash */
void flash_write(uint32_t addr, void *p, uint16_t len); /*!< 向Flash中写入数据 */

#endif
