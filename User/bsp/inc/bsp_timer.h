/*
 * @Description:  定时器模块
 * @Author: Gaven
 * @Date: 2019-10-21 10:34:45
 * @LastEditTime: 2019-11-20 10:22:09
 * @LastEditors: Please set LastEditors
 */
#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include "global_config.h"

/**
  * @brief 在此定义若干个软件定时器全局变量
    注意，必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
  */

typedef enum
{
	TMR0_DELAY,				    //系统延时定时器，误差+-1ms
	TMR1_COMPLATFORM,		    //通信平台定时器,30ms
	TMR2_PC_HEART_BEAT,         //pc心跳超时, 10s
    TMR3_SND_M3_HAERT,          //发送心跳，1s
    TMR4_ALARM_LED,             //心跳丢失后，报警灯500ms闪一次
    TMR5_AFE_CHANGE_MODE,		//AFE切换IQ模式后的延时
#ifdef _USART_PRINT_DEBUG_
    //TMR6_ADJUST_TEST,
#endif
	TMR_SUM
}TMR_ID;

/**
  * @brief 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题
  */
typedef enum
{
	TMR_ONCE_MODE = 0,		/*!< 一次工作模式 */
	TMR_AUTO_MODE = 1		/*!< 自动定时工作模式 */
}TMR_MODE_E;

/**
  * @breif 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题
  */
typedef struct
{
	volatile uint8_t Mode;		/*!< 计数器模式，1次性 */
	volatile uint8_t Flag;		/*!< 定时到达标志  */
	volatile uint32_t Count;	/*!< 计数器 */
	volatile uint32_t PreLoad;	/*!< 计数器预装值 */
}SOFT_TMR;


void bsp_StartAllTimer(void);       /*!< 启动所有软件定时器 */
void bsp_InitTimer(void);           /*!< 配置systick中断，并初始化软件定时器变量 */
void SysTick_ISR(void);             /*!< SysTick中断服务程序，每隔1ms进入1次 */
static void bsp_SoftTimerDec(SOFT_TMR *_tmr);           /*!< 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。 */
void bsp_DelayMS(uint32_t n);       /*!< ms级延迟，延迟精度为正负1ms */
void bsp_DelayUS(uint32_t n);       /*!< us级延迟 */
void bsp_StartTimer(uint8_t _id, uint32_t _period);     /*!< 启动一个定时器，并设置定时周期。 */
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period); /*!< 启动一个自动定时器，并设置定时周期。 */
void bsp_StopTimer(uint8_t _id);    /*!< 停止一个定时器 */
uint8_t bsp_CheckTimer(uint8_t _id, uint8_t isNeedStopFlag); /*!< 检测定时器是否超时 */
int32_t bsp_GetRunTime(void);       /*!< 获取CPU运行时间，单位1ms。 */
int32_t bsp_CheckRunTime(int32_t _LastTime);    /*!< 计算当前运行时间和给定时刻之间的差值。处理了计数器循环。 */

#endif
