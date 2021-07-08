/**
  ******************************************************************************
  * @file    bsp_shtc3.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#ifndef _BSP_SHTC3_H_
#define _BSP_SHTC3_H_

#include "global_config.h"

void shtc3_readdata(float *p_temp, float *p_hum);   /*!< ��ȡ��ʪ�� */
void shtc3_readid(int16_t *p_id);   /*!< ��ȡid */

#endif
