/*
 * @Description:  ÖÐ¶ÏÄ£¿é
 * @Author: Gaven
 * @Date: 2019-10-21 10:34:45
 * @LastEditTime: 2019-10-24 16:58:27
 * @LastEditors: Gaven
 */
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#include "stm32f10x.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif
