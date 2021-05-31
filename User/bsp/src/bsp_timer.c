/*
 * @Description:  配置systick定时器作为系统滴答定时器。缺省定时周期为1ms。
 *                  实现了多个软件定时器供主程序使用(精度1ms)， 可以通过修改 TMR_COUNT 增减定时器个数;
 *                  实现了ms级别延迟函数（精度1ms） 和us级延迟函数;
 *                  实现了系统运行时间函数（1ms单位）bsp_GetRunTime;
 * @Author: Gaven
 * @Date: 2019-10-21 17:46:56
 * @LastEditTime: 2019-11-20 10:22:21
 * @LastEditors: Please set LastEditors
 */

#include "bsp.h"

/**
  * @brief 定于软件定时器结构体变量
  */
static SOFT_TMR s_tTmr[TMR_SUM] = {0};

/**
  * @brief 全局运行时间，单位1ms
  * @details 最长可以表示 24.85天，如果连续运行时间超过这个数，则必须考虑溢出问题
  */
static __IO int32_t g_iRunTime = 0;

/**
  * @brief 启动所有软件定时器
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
  * @brief 配置systick中断，并初始化软件定时器变量
  */
void bsp_InitTimer(void)
{
    uint8_t i;

    /* 清零所有的软件定时器 */
    for (i = 0; i < TMR_SUM; i++)
    {
        s_tTmr[i].Count = 0;
        s_tTmr[i].PreLoad = 0;
        s_tTmr[i].Flag = 0;
        s_tTmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
    }

    /*
    	配置systic中断周期为1ms，并启动systick中断。

    	SystemCoreClock 是固件中定义的系统内核时钟，对于STM32F4XX,一般为 168MHz

    	SysTick_Config() 函数的形参表示内核时钟多少个周期后触发一次Systick定时中断.
        	-- SystemCoreClock / 1000  表示定时频率为 1000Hz， 也就是定时周期为  1ms
        	-- SystemCoreClock / 500   表示定时频率为 500Hz，  也就是定时周期为  2ms
        	-- SystemCoreClock / 2000  表示定时频率为 2000Hz， 也就是定时周期为  500us

    	对于常规的应用，我们一般取定时周期1ms。对于低速CPU或者低功耗应用，可以设置定时周期为 10ms
    */
    SysTick_Config(SystemCoreClock / 1000);
}

/**
  * @brief SysTick中断服务程序，每隔1ms进入1次
  */
void SysTick_ISR(void)
{
    static uint8_t s_count = 0;
    uint8_t i;

    /* 每隔1ms，对软件定时器的计数器进行减一操作 */
    for (i = 0; i < TMR_SUM; i++)
    {
        bsp_SoftTimerDec(&s_tTmr[i]);
    }

    /* 全局运行时间每1ms增1 */
    g_iRunTime++;
    if (g_iRunTime == 0x7FFFFFFF)	/* 这个变量是 int32_t 类型，最大数为 0x7FFFFFFF */
    {
        g_iRunTime = 0;
    }

    bsp_RunPer1ms();		/* 每隔1ms调用一次此函数，此函数在 bsp.c */

    if (++s_count >= 10)
    {
        s_count = 0;

        bsp_RunPer10ms();	/* 每隔10ms调用一次此函数，此函数在 bsp.c */
    }
}

/**
  * @brief 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
  * @param _tmr  定时器变量指针
  */
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
    if (_tmr->Count > 0)
    {
        /* 如果定时器变量减到1则设置定时器到达标志 */
        if (--_tmr->Count == 0)
        {
            _tmr->Flag = 1;

            /* 如果是自动模式，则自动重装计数器 */
            if(_tmr->Mode == TMR_AUTO_MODE)
            {
                _tmr->Count = _tmr->PreLoad;
            }
        }
    }
}

/**
  * @brief ms级延迟，延迟精度为正负1ms
  * @details 使用软件定时器进行延时，阻塞。
  * @param n 延迟长度，单位1 ms。 n 应大于2
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
        bsp_Idle();				/* CPU空闲执行的操作， 见 bsp.c 和 bsp.h 文件 */

        if (bsp_CheckTimer(TMR0_DELAY, 1))
        {
            break;
        }
    }
}

/**
  * @brief us级延迟。 
  * @details systick延时，阻塞。必须在systick定时器启动后才能调用此函数,如果启用
  *          了systick作为softTimer，则此函数可能会失效
  * @param n : 延迟长度，单位1 us
  */
void bsp_DelayUS(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;

    reload = SysTick->LOAD;
    ticks = n * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */

    tcnt = 0;
    told = SysTick->VAL;             /* 刚进入时的计数器值 */

    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            /* SYSTICK是一个递减的计数器 */
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            /* 重新装载递减 */
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;

            /* 时间超过/等于要延迟的时间,则退出 */
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}


/**
  * @brief 启动一个定时器，并设置定时周期。
  * @param _id     : 定时器ID，值域【0,TMR_SUM-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
  * @param _period : 定时周期，单位1ms
  */
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
    _ASSERT(_id < TMR_SUM);

    DISABLE_INT();

    s_tTmr[_id].Count = _period;		/* 实时计数器初值 */
    s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
    s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
    s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

    ENABLE_INT();
}

/**
  * @brief 启动一个自动定时器，并设置定时周期。
  * @param _id     : 定时器ID，值域【0,TMR_SUM-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
  * @param _period : 定时周期，单位10ms
  */
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
    _ASSERT(_id < TMR_SUM);

    DISABLE_INT();
    
    s_tTmr[_id].Count = _period;
    s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
    s_tTmr[_id].Flag = 0;
    s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* 自动工作模式 */

    ENABLE_INT();
}

/**
  * @brief 停止一个定时器
  * @param _id     : 定时器ID，值域【0,TMR_SUM-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
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
  * @brief 检测定时器是否超时
  * @param _id     : 定时器ID，值域【0,TMR_SUM-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
  * @param isNeedStopFlag : 检测到超时后是否需要停止该定时器,如果设置为0，则在调用check后需调用stop手动停止定时器
  * @retval 返回 0 表示定时未到， 1表示定时到
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
  * @brief 获取CPU运行时间，单位1ms。
  * @details 最长可以表示 24.85天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
  * @return CPU运行时间，单位1ms
  */
int32_t bsp_GetRunTime(void)
{
    return g_iRunTime;
}

/**
  * @brief 计算当前运行时间和给定时刻之间的差值。处理了计数器循环。
  * @param _LastTime 上个时刻
  * @return 当前时间和过去时间的差值，单位1ms
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
