/**
  ******************************************************************************
  * @file    bsp_dac.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#ifndef _BSP_DAC_H_
#define _BSP_DAC_H_

#include "global_config.h"

typedef enum
{
    DAC_ID_TEST_OVER = 0,
    DAC_ID_TEST_START = 1,
    DAC_ID_ADJUST_START = 2,
    DAC_ID_ADJUST_OVER = 3,
    DAC_ID_MUSIC = 4,
    DAC_ID_PRE_TEST_START = 5,
} DAC_ID;

void bsp_dac_init(void);
void bsp_dac_open(uint8_t _id);
void bsp_dac_close(void);
void bsp_dac_volume_dec(void);
void bsp_dac_volume_inc(void);


void TIM6_IRQHandler(void);

#endif
