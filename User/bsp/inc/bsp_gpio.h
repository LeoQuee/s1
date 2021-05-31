#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#include "global_config.h"

#define PCB_UNKNOWN                 -1
#define PCB_RECTANGULAR_lATTICE     0
#define PCB_FINGER                  1
#define PCB_BA_GUA                  2
#define PCB_RESERVED                3

extern int8_t s_pcb_verion;

void init_version_bit(void);    /*!< 初始化硬件版本控制引脚 */

#endif
