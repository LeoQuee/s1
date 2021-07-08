/**
  ******************************************************************************
  * @file    global_config.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#ifndef _GLOBAL_CONFIG_H_
#define _GLOBAL_CONFIG_H_

#include <stdlib.h>
#include <stdio.h>

#include "hardware_config.h"

/********************************** ����汾 ***********************************/
#define SOFTWARE_VERSION_MAIN   3
#define SOFTWARE_VERSION_SUB1   6
#define SOFTWARE_VERSION_SUB2   8

/********************************** �������� ***********************************/
#define BOOT_CONFIG         0   /*!< �������� */
#define WATCHDOG_CONFIG     0   /*!< ���Ź� */
#define LCD_CONFIG          0   /*!< Һ���� */
#define LCR_CAL_CONFIG      0   /*!< �������ֵ��Ž������У׼ */
#define KEY_CONFIG          1   /*!< ���� */
#define LED_CONFIG          0   /*!< ָʾ�� */
#define DOT_MATRIX_CONFIG   0   /*!< ���� */
#define AFE4300_CONFIG      1   /*!< AFE4300 */
#define _PRE_TESTING_           /*!< Ԥ���Թ��� */

#define BSP_Printf		    printf
#define _PRINT              printf
#define _ASSERT(expr)       ((expr) ? (void)0 : (void)printf("Error:file %s, function %s, line %d\r\n", __FILE__, __FUNCTION__, __LINE__))

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	    __set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	    __set_PRIMASK(1)	/* ��ֹȫ���ж� */


#define _USART_PRINT_DEBUG_
//#define _OLED_ON_
#define DEBUG1  // ���ڴ�ӡIQֵ

//#define _SKIN_ELECTRODE_IMPEDANCE_TEST_  // �缫Ƥ���Ӵ��迹����
#define _AFE4300_IMPROVE_

#endif
