/*
 * @Description:  �����ж�+FIFO����ģ��
 * @Author: Gaven
 * @Date: 2019-10-21 10:34:45
 * @LastEditTime: 2019-10-24 16:48:21
 * @LastEditors: Gaven
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
