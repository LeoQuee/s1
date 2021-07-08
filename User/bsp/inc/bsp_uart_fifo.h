/**
  ******************************************************************************
  * @file    bsp_uart_fifo.h
  * @author  lik
  * @date    2021-7-8
  * @brief   串口中断+FIFO驱动模块
  ******************************************************************************
  */  
 
#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

#include "global_config.h"

#define UART_BAUD			115200  /*!< 串口1波特率 */

/**
  * @brief 初始化串口硬件，并对全局变量赋初值.
  */
void bsp_InitUart(void);

/**
  * @brief 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）
  */
static void InitHardUart(void);

/**
  * @brief 配置串口硬件中断
  */
static void ConfigUartNVIC(void);

/**
  * @brief 串口4中断处理函数
  */
static void Usart1IRQ(void);

#endif
