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

/********************************** 软件版本 ***********************************/
#define SOFTWARE_VERSION_MAIN   3
#define SOFTWARE_VERSION_SUB1   6
#define SOFTWARE_VERSION_SUB2   8

/********************************** 功能配置 ***********************************/
#define BOOT_CONFIG         0   /*!< 引导程序 */
#define WATCHDOG_CONFIG     0   /*!< 看门狗 */
#define LCD_CONFIG          0   /*!< 液晶屏 */
#define LCR_CAL_CONFIG      0   /*!< 根据数字电桥结果进行校准 */
#define KEY_CONFIG          1   /*!< 按键 */
#define LED_CONFIG          0   /*!< 指示灯 */
#define DOT_MATRIX_CONFIG   0   /*!< 点阵 */
#define AFE4300_CONFIG      1   /*!< AFE4300 */
#define _PRE_TESTING_           /*!< 预测试功能 */

#define BSP_Printf		    printf
#define _PRINT              printf
#define _ASSERT(expr)       ((expr) ? (void)0 : (void)printf("Error:file %s, function %s, line %d\r\n", __FILE__, __FUNCTION__, __LINE__))

/* 开关全局中断的宏 */
#define ENABLE_INT()	    __set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	    __set_PRIMASK(1)	/* 禁止全局中断 */


#define _USART_PRINT_DEBUG_
//#define _OLED_ON_
#define DEBUG1  // 串口打印IQ值

//#define _SKIN_ELECTRODE_IMPEDANCE_TEST_  // 电极皮肤接触阻抗测试
#define _AFE4300_IMPROVE_

#endif
