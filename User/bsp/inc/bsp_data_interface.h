/**
  ******************************************************************************
  * @file    bsp_data_interface.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#ifndef _BSP_DATA_INTERFACE_H_
#define _BSP_DATA_INTERFACE_H_

#include "global_config.h"

#define XFLASH_ADDR_SYSFLAG      0x000000
#define XFLASH_ADDR_SYSFLAG_END  0x000FFF // 4K字节

#define XFLASH_ADDR_RESERVED     0x001000
#define XFLASH_ADDR_RESERVED_END 0x0FFFFF // 1020K字节

#define XFLASH_ADDR_SOUND        0x100000
#define XFLASH_ADDR_SOUND_END    0x1FFFFF // 1M字节

#define XFLASH_ADDR_MUSIC        0x200000
#define XFLASH_ADDR_MUSIC_END    0xFFFFFF // 14M字节

#define XFLASH_ADDR_SOUND1       0x100000 // 测量结束,32K
#define XFLASH_ADDR_SOUND2       0x108000 // 测量开始,160K
#define XFLASH_ADDR_SOUND3       0x130000 // 校准开始,64K
#define XFLASH_ADDR_SOUND4       0x140000 // 校准结束,64K
#define XFLASH_ADDR_SOUND5       0x150000 // 预测，320K
#define XFLASH_ADDR_SOUND6       0x1A0000 // 预留，384K

#define XFLASH_ADDR_MUSIC1       0x200000 // 致爱丽丝 ~ 0x688000，4640K

/**
  * @brief 32字节
  */
typedef struct
{
    float R1;
    float R2;
    float R3;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    char  sn[16];
} SYSFLAG;

void sysflag_read(SYSFLAG *p);  /*!< 读取系统标志 */
void sysflag_write(SYSFLAG *p); /*!< 写入系统标志 */

#endif
