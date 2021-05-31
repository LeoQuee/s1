#ifndef _BSP_DAC_H_
#define _BSP_DAC_H_

#include "global_config.h"

void bsp_dac_init(void);
void bsp_dac_open(uint8_t _id);
void bsp_dac_close(void);
void bsp_dac_volume_dec(void);
void bsp_dac_volume_inc(void);


void TIM6_IRQHandler(void);

#endif
