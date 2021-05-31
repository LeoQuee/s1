#ifndef _GLOBAL_CONFIG_H_
#define _GLOBAL_CONFIG_H_

#include <stdlib.h>
#include <stdio.h>

#include "hardware_config.h"

/********************************** �����汾 **********************************/
#define MAIN_VER    "V1.0.0"

#define SOFTWARE_VERSION_MAIN   3
#define SOFTWARE_VERSION_SUB1   6
#define SOFTWARE_VERSION_SUB2   7

/********************************** �������� **********************************/
#define BOOT_CONFIG         0   /*!< �������� */
#define WATCHDOG_CONFIG     0   /*!< ���Ź� */
#define LCD_CONFIG          0   /*!< Һ���� */
#define LCR_CAL_CONFIG      0   /*!< �������ֵ��Ž������У׼ */
#define KEY_CONFIG          1   /*!< ���� */
#define LED_CONFIG          0   /*!< ָʾ�� */
#define DOT_MATRIX_CONFIG   0   /*!< ���� */
#define AFE4300_CONFIG      1   /*!< AFE4300 */

#define BSP_Printf		    printf
#define _PRINT              printf
#define _ASSERT(expr)       ((expr) ? (void)0 : (void)printf("Error:file %s, function %s, line %d\r\n", __FILE__, __FUNCTION__, __LINE__))

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	    __set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	    __set_PRIMASK(1)	/* ��ֹȫ���ж� */


#define _USART_PRINT_DEBUG_
//#define _OLED_ON_
#define DEBUG1

#endif