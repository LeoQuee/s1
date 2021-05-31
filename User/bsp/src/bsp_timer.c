/*
 * @Description:  ����systick��ʱ����Ϊϵͳ�δ�ʱ����ȱʡ��ʱ����Ϊ1ms��
 *                  ʵ���˶�������ʱ����������ʹ��(����1ms)�� ����ͨ���޸� TMR_COUNT ������ʱ������;
 *                  ʵ����ms�����ӳٺ���������1ms�� ��us���ӳٺ���;
 *                  ʵ����ϵͳ����ʱ�亯����1ms��λ��bsp_GetRunTime;
 * @Author: Gaven
 * @Date: 2019-10-21 17:46:56
 * @LastEditTime: 2019-11-20 10:22:21
 * @LastEditors: Please set LastEditors
 */

#include "bsp.h"

/**
  * @brief ���������ʱ���ṹ�����
  */
static SOFT_TMR s_tTmr[TMR_SUM] = {0};

/**
  * @brief ȫ������ʱ�䣬��λ1ms
  * @details ����Ա�ʾ 24.85�죬�����������ʱ�䳬�������������뿼���������
  */
static __IO int32_t g_iRunTime = 0;

/**
  * @brief �������������ʱ��
  */
void bsp_StartAllTimer(void)
{
    bsp_StartAutoTimer(TMR1_COMPLATFORM,    30);
    bsp_StartAutoTimer(TMR2_PC_HEART_BEAT,  10000);
    bsp_StartAutoTimer(TMR3_SND_M3_HAERT,   1000);
    bsp_StartAutoTimer(TMR4_ALARM_LED, 500);
#ifdef _USART_PRINT_DEBUG_
    //bsp_StartAutoTimer(TMR6_ADJUST_TEST, 60*1000);
#endif
}

/**
  * @brief ����systick�жϣ�����ʼ�������ʱ������
  */
void bsp_InitTimer(void)
{
    uint8_t i;

    /* �������е������ʱ�� */
    for (i = 0; i < TMR_SUM; i++)
    {
        s_tTmr[i].Count = 0;
        s_tTmr[i].PreLoad = 0;
        s_tTmr[i].Flag = 0;
        s_tTmr[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
    }

    /*
    	����systic�ж�����Ϊ1ms��������systick�жϡ�

    	SystemCoreClock �ǹ̼��ж����ϵͳ�ں�ʱ�ӣ�����STM32F4XX,һ��Ϊ 168MHz

    	SysTick_Config() �������βα�ʾ�ں�ʱ�Ӷ��ٸ����ں󴥷�һ��Systick��ʱ�ж�.
        	-- SystemCoreClock / 1000  ��ʾ��ʱƵ��Ϊ 1000Hz�� Ҳ���Ƕ�ʱ����Ϊ  1ms
        	-- SystemCoreClock / 500   ��ʾ��ʱƵ��Ϊ 500Hz��  Ҳ���Ƕ�ʱ����Ϊ  2ms
        	-- SystemCoreClock / 2000  ��ʾ��ʱƵ��Ϊ 2000Hz�� Ҳ���Ƕ�ʱ����Ϊ  500us

    	���ڳ����Ӧ�ã�����һ��ȡ��ʱ����1ms�����ڵ���CPU���ߵ͹���Ӧ�ã��������ö�ʱ����Ϊ 10ms
    */
    SysTick_Config(SystemCoreClock / 1000);
}

/**
  * @brief SysTick�жϷ������ÿ��1ms����1��
  */
void SysTick_ISR(void)
{
    static uint8_t s_count = 0;
    uint8_t i;

    /* ÿ��1ms���������ʱ���ļ��������м�һ���� */
    for (i = 0; i < TMR_SUM; i++)
    {
        bsp_SoftTimerDec(&s_tTmr[i]);
    }

    /* ȫ������ʱ��ÿ1ms��1 */
    g_iRunTime++;
    if (g_iRunTime == 0x7FFFFFFF)	/* ��������� int32_t ���ͣ������Ϊ 0x7FFFFFFF */
    {
        g_iRunTime = 0;
    }

    bsp_RunPer1ms();		/* ÿ��1ms����һ�δ˺������˺����� bsp.c */

    if (++s_count >= 10)
    {
        s_count = 0;

        bsp_RunPer10ms();	/* ÿ��10ms����һ�δ˺������˺����� bsp.c */
    }
}

/**
  * @brief ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
  * @param _tmr  ��ʱ������ָ��
  */
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
    if (_tmr->Count > 0)
    {
        /* �����ʱ����������1�����ö�ʱ�������־ */
        if (--_tmr->Count == 0)
        {
            _tmr->Flag = 1;

            /* ������Զ�ģʽ�����Զ���װ������ */
            if(_tmr->Mode == TMR_AUTO_MODE)
            {
                _tmr->Count = _tmr->PreLoad;
            }
        }
    }
}

/**
  * @brief ms���ӳ٣��ӳپ���Ϊ����1ms
  * @details ʹ�������ʱ��������ʱ��������
  * @param n �ӳٳ��ȣ���λ1 ms�� n Ӧ����2
  */
void bsp_DelayMS(uint32_t n)
{
    if (n == 0)
    {
        return;
    }
    else if (n == 1)
    {
        n = 2;
    }

    bsp_StartTimer(TMR0_DELAY, n);

    while (1)
    {
        bsp_Idle();				/* CPU����ִ�еĲ����� �� bsp.c �� bsp.h �ļ� */

        if (bsp_CheckTimer(TMR0_DELAY, 1))
        {
            break;
        }
    }
}

/**
  * @brief us���ӳ١� 
  * @details systick��ʱ��������������systick��ʱ����������ܵ��ô˺���,�������
  *          ��systick��ΪsoftTimer����˺������ܻ�ʧЧ
  * @param n : �ӳٳ��ȣ���λ1 us
  */
void bsp_DelayUS(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;

    reload = SysTick->LOAD;
    ticks = n * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */

    tcnt = 0;
    told = SysTick->VAL;             /* �ս���ʱ�ļ�����ֵ */

    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            /* SYSTICK��һ���ݼ��ļ����� */
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            /* ����װ�صݼ� */
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;

            /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}


/**
  * @brief ����һ����ʱ���������ö�ʱ���ڡ�
  * @param _id     : ��ʱ��ID��ֵ��0,TMR_SUM-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
  * @param _period : ��ʱ���ڣ���λ1ms
  */
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
    _ASSERT(_id < TMR_SUM);

    DISABLE_INT();

    s_tTmr[_id].Count = _period;		/* ʵʱ��������ֵ */
    s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
    s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
    s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

    ENABLE_INT();
}

/**
  * @brief ����һ���Զ���ʱ���������ö�ʱ���ڡ�
  * @param _id     : ��ʱ��ID��ֵ��0,TMR_SUM-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
  * @param _period : ��ʱ���ڣ���λ10ms
  */
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
    _ASSERT(_id < TMR_SUM);

    DISABLE_INT();
    
    s_tTmr[_id].Count = _period;
    s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
    s_tTmr[_id].Flag = 0;
    s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */

    ENABLE_INT();
}

/**
  * @brief ֹͣһ����ʱ��
  * @param _id     : ��ʱ��ID��ֵ��0,TMR_SUM-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
  */
void bsp_StopTimer(uint8_t _id)
{
    _ASSERT(_id < TMR_SUM);

    DISABLE_INT();

    s_tTmr[_id].Count = 0;
    s_tTmr[_id].Flag = 0;
    s_tTmr[_id].Mode = TMR_ONCE_MODE;

    ENABLE_INT();
}

/**
  * @brief ��ⶨʱ���Ƿ�ʱ
  * @param _id     : ��ʱ��ID��ֵ��0,TMR_SUM-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
  * @param isNeedStopFlag : ��⵽��ʱ���Ƿ���Ҫֹͣ�ö�ʱ��,�������Ϊ0�����ڵ���check�������stop�ֶ�ֹͣ��ʱ��
  * @retval ���� 0 ��ʾ��ʱδ���� 1��ʾ��ʱ��
  */
uint8_t bsp_CheckTimer(uint8_t _id, uint8_t isNeedStopFlag)
{
    _ASSERT(_id < TMR_SUM);

    if (s_tTmr[_id].Flag == 1)
    {
        if(isNeedStopFlag)
        {
            s_tTmr[_id].Flag = 0;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
  * @brief ��ȡCPU����ʱ�䣬��λ1ms��
  * @details ����Ա�ʾ 24.85�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
  * @return CPU����ʱ�䣬��λ1ms
  */
int32_t bsp_GetRunTime(void)
{
    return g_iRunTime;
}

/**
  * @brief ���㵱ǰ����ʱ��͸���ʱ��֮��Ĳ�ֵ�������˼�����ѭ����
  * @param _LastTime �ϸ�ʱ��
  * @return ��ǰʱ��͹�ȥʱ��Ĳ�ֵ����λ1ms
  */
int32_t bsp_CheckRunTime(int32_t _LastTime)
{
    int32_t now_time;
    int32_t time_diff;
    
    now_time = g_iRunTime;
    
    if (now_time >= _LastTime)
    {
        time_diff = now_time - _LastTime;
    }
    else
    {
        time_diff = 0x7FFFFFFF - _LastTime + now_time;
    }

    return time_diff;
}
