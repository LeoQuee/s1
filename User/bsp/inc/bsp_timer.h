/*
 * @Description:  ��ʱ��ģ��
 * @Author: Gaven
 * @Date: 2019-10-21 10:34:45
 * @LastEditTime: 2019-11-20 10:22:09
 * @LastEditors: Please set LastEditors
 */
#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include "global_config.h"

/**
  * @brief �ڴ˶������ɸ������ʱ��ȫ�ֱ���
    ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
  */

typedef enum
{
	TMR0_DELAY,				    //ϵͳ��ʱ��ʱ�������+-1ms
	TMR1_COMPLATFORM,		    //ͨ��ƽ̨��ʱ��,30ms
	TMR2_PC_HEART_BEAT,         //pc������ʱ, 10s
    TMR3_SND_M3_HAERT,          //����������1s
    TMR4_ALARM_LED,             //������ʧ�󣬱�����500ms��һ��
    TMR5_AFE_CHANGE_MODE,		//AFE�л�IQģʽ�����ʱ
#ifdef _USART_PRINT_DEBUG_
    //TMR6_ADJUST_TEST,
#endif
	TMR_SUM
}TMR_ID;

/**
  * @brief ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ����������
  */
typedef enum
{
	TMR_ONCE_MODE = 0,		/*!< һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/*!< �Զ���ʱ����ģʽ */
}TMR_MODE_E;

/**
  * @breif ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ����������
  */
typedef struct
{
	volatile uint8_t Mode;		/*!< ������ģʽ��1���� */
	volatile uint8_t Flag;		/*!< ��ʱ�����־  */
	volatile uint32_t Count;	/*!< ������ */
	volatile uint32_t PreLoad;	/*!< ������Ԥװֵ */
}SOFT_TMR;


void bsp_StartAllTimer(void);       /*!< �������������ʱ�� */
void bsp_InitTimer(void);           /*!< ����systick�жϣ�����ʼ�������ʱ������ */
void SysTick_ISR(void);             /*!< SysTick�жϷ������ÿ��1ms����1�� */
static void bsp_SoftTimerDec(SOFT_TMR *_tmr);           /*!< ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á� */
void bsp_DelayMS(uint32_t n);       /*!< ms���ӳ٣��ӳپ���Ϊ����1ms */
void bsp_DelayUS(uint32_t n);       /*!< us���ӳ� */
void bsp_StartTimer(uint8_t _id, uint32_t _period);     /*!< ����һ����ʱ���������ö�ʱ���ڡ� */
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period); /*!< ����һ���Զ���ʱ���������ö�ʱ���ڡ� */
void bsp_StopTimer(uint8_t _id);    /*!< ֹͣһ����ʱ�� */
uint8_t bsp_CheckTimer(uint8_t _id, uint8_t isNeedStopFlag); /*!< ��ⶨʱ���Ƿ�ʱ */
int32_t bsp_GetRunTime(void);       /*!< ��ȡCPU����ʱ�䣬��λ1ms�� */
int32_t bsp_CheckRunTime(int32_t _LastTime);    /*!< ���㵱ǰ����ʱ��͸���ʱ��֮��Ĳ�ֵ�������˼�����ѭ���� */

#endif
