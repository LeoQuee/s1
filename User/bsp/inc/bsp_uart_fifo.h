/**
  ******************************************************************************
  * @file    bsp_uart_fifo.h
  * @author  lik
  * @date    2021-7-8
  * @brief   �����ж�+FIFO����ģ��
  ******************************************************************************
  */  
 
#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

#include "global_config.h"

#define UART_BAUD			115200  /*!< ����1������ */

/**
  * @brief ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
  */
void bsp_InitUart(void);

/**
  * @brief ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ�
  */
static void InitHardUart(void);

/**
  * @brief ���ô���Ӳ���ж�
  */
static void ConfigUartNVIC(void);

/**
  * @brief ����4�жϴ�����
  */
static void Usart1IRQ(void);

#endif
