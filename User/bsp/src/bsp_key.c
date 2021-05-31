/*
 * @Description:  独立按键驱动模块
 *                  扫描独立按键，具有软件滤波机制，具有按键FIFO。可以检测如下事件：
 *                      (1) 按键按下
 *				        (2) 按键弹起
 *				        (3) 长按键
 *				        (4) 长按时自动连发
 *
 * 仅实现短按，长按，连按检测；组合键、双击等未实现。
 */

#include "bsp.h"

static KEY_T s_tBtn[KEY_NUM];
static KEY_FIFO_T s_tKey;		/* 按键FIFO变量,结构体 */

/**
 * @brief DOWN按键是否按下
 * @return: 1 表示按下， 0表示未按下
 */
static uint8_t IsKey1Down(void)
{
#if (KEY_CONFIG == 1) && (defined KEY1_PORT)
    if(GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == Bit_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/**
 * @brief UP按键是否按下
 * @return: 1 表示按下， 0表示未按下
 */
static uint8_t IsKey2Down(void)
{
#if (KEY_CONFIG == 1) && (defined KEY2_PORT)
    if(GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN) == Bit_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/**
 * @brief MENU按键是否按下
 * @return: 1 表示按下， 0表示未按下
 */
static uint8_t IsKey3Down(void)
{
#if (KEY_CONFIG == 1) && (defined KEY3_PORT)
    if(GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == Bit_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/**
 * @brief ENTER按键是否按下
 * @return: 1 表示按下， 0表示未按下
 */
static uint8_t IsKey4Down(void)
{
#if (KEY_CONFIG == 1) && (defined KEY4_PORT)
    if(GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN) == Bit_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/**
  * @brief 初始化按键相关IO口
  */
static void bsp_InitKeyHard(void)
{
#if KEY_CONFIG == 1
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 第1步：打开GPIO时钟 */
    RCC_APB2PeriphClockCmd(KEY_RCC, ENABLE);

    /* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* 输入浮空模式 */

    /* 第3步：依次执行配置函数 */
#ifdef     KEY1_PORT
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);
#endif    

#ifdef     KEY2_PORT
    GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
    GPIO_Init(KEY2_PORT, &GPIO_InitStructure);
#endif
    
#ifdef KEY3_PORT
    GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
    GPIO_Init(KEY3_PORT, &GPIO_InitStructure);
#endif

#ifdef KEY4_PORT
    GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
    GPIO_Init(KEY4_PORT, &GPIO_InitStructure);
#endif    
#endif
}

/**
  * @brief 初始化按键相关参数
  */
static void bsp_InitKeyVar(void)
{
    uint8_t i;

    /* 对按键FIFO读写指针清零 */
    s_tKey.Read = 0;
    s_tKey.Write = 0;

    /* 给每个按键结构体成员变量赋一组缺省值 */
    for (i = 0; i < KEY_NUM; i++)
    {
        s_tBtn[i].LongTime = 100;	    // 检测长按，1s
        s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
        s_tBtn[i].State = 0;        // 未按下
        s_tBtn[i].RepeatSpeed = 10;  // 检测连按
        s_tBtn[i].RepeatCount = 0;	// 连发计数器
    }

    /* 判断按键按下的函数 */
#ifdef PCB_V3
    s_tBtn[0].IsKeyDownFunc = IsKey4Down;
    s_tBtn[0].keyval = KEY_DOWN;
    
    s_tBtn[1].IsKeyDownFunc = IsKey1Down;
    s_tBtn[1].keyval = KEY_UP;
    
    s_tBtn[2].IsKeyDownFunc = IsKey3Down;
    s_tBtn[2].keyval = KEY_MENU;
    
    s_tBtn[3].IsKeyDownFunc = IsKey2Down;
    s_tBtn[3].keyval = KEY_ENTER;
#else    
    if(s_pcb_verion == PCB_FINGER)
    {
        s_tBtn[0].IsKeyDownFunc = IsKey3Down;
        s_tBtn[0].keyval = KEY_DOWN;
        
        s_tBtn[1].IsKeyDownFunc = IsKey2Down;
        s_tBtn[1].keyval = KEY_UP;
        
        s_tBtn[2].IsKeyDownFunc = IsKey1Down;
        s_tBtn[2].keyval = KEY_MENU;
        
        s_tBtn[3].IsKeyDownFunc = IsKey4Down;
        s_tBtn[3].keyval = KEY_ENTER;

    }
    else
    {
        s_tBtn[0].IsKeyDownFunc = IsKey1Down;
        s_tBtn[0].keyval = KEY_DOWN;
        
        s_tBtn[1].IsKeyDownFunc = IsKey3Down;
        s_tBtn[1].keyval = KEY_UP;
        
        s_tBtn[2].IsKeyDownFunc = IsKey4Down;
        s_tBtn[2].keyval = KEY_MENU;
        
        s_tBtn[3].IsKeyDownFunc = IsKey2Down;
        s_tBtn[3].keyval = KEY_ENTER;
    }
#endif    
}

/**
  * @brief 按键初始化
  */
void bsp_InitKey(void)
{
    _PRINT("bsp_InitKey()\r\n");
    bsp_InitKeyVar();		/* 初始化按键变量 */
    bsp_InitKeyHard();		/* 初始化按键硬件 */
}

/**
  * @brief 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
  * @param _KeyCode 按键代码
  */
void bsp_PutKey(uint8_t _KeyCode)
{
    s_tKey.Buf[s_tKey.Write] = _KeyCode;

    if (++s_tKey.Write  >= KEY_FIFO_SIZE)
    {
        s_tKey.Write = 0;
    }
}

/**
  * @brief 从按键FIFO缓冲区读取一个键值。
  * @retval 按键代码
  */
uint8_t bsp_GetKey(void)
{
    uint8_t ret;

    if (s_tKey.Read == s_tKey.Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = s_tKey.Buf[s_tKey.Read];

        if (++s_tKey.Read >= KEY_FIFO_SIZE)
        {
            s_tKey.Read = 0;
        }
        
        return ret;
    }
}

/**
  * @brief 设置按键参数
  * @param _ucKeyID     按键ID，从0开始
  * @param _LongTime    长按事件时间
  * @param _RepeatSpeed 连发速度
  */
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
    s_tBtn[_ucKeyID].LongTime = _LongTime;			/* 长按时间 0 表示不检测长按键事件 */
    s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;	/* 按键连发的速度，0表示不支持连发 */
    s_tBtn[_ucKeyID].RepeatCount = 0;				/* 连发计数器 */
}


/**
  * @brief 清空按键FIFO缓冲区
  */
void bsp_ClearKey(void)
{
    s_tKey.Read = s_tKey.Write;
}

/**
  * @brief 检测一个按键。
  * @details 非阻塞状态，必须被周期性的调用。
  * @param i 第几个按键
  */
static void bsp_DetectKey(uint8_t i)
{
    KEY_T *pBtn;

    pBtn = &s_tBtn[i];
    
    if (pBtn->IsKeyDownFunc()) // 当前按键是否按下
    {
        if (pBtn->Count < KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if(pBtn->Count < 2 * KEY_FILTER_TIME)
        {
            pBtn->Count++;
        }
        else
        {
            if (pBtn->State == 0)
            {
                pBtn->State = 1;

                /* 发送按钮按下的消息 */
                bsp_PutKey(pBtn->keyval);
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    /* 发送按钮持续按下的消息 */
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        /* 键值放入按键FIFO */
                        bsp_PutKey(pBtn->keyval | KEY_LONG);
                    }
                }
                else
                {
                    if (pBtn->RepeatSpeed > 0)
                    {
                        if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
                        {
                            pBtn->RepeatCount = 0;
                            /* 长按键后，每隔10ms发送1个按键 */
                            bsp_PutKey(pBtn->keyval | KEY_REPEAT);
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(pBtn->Count > KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if(pBtn->Count != 0)
        {
            pBtn->Count--;
        }
        else
        {
            if (pBtn->State == 1)
            {
                pBtn->State = 0;
            }
        }

        pBtn->LongCount = 0;
        pBtn->RepeatCount = 0;
    }
}

/**
  * @brief 扫描所有按键
  * @details systick中断中调用，每10ms执行一次。
  */
void bsp_KeyScan(void)
{
    uint8_t i;

    for (i = 0; i < KEY_NUM; i++)
    {
        bsp_DetectKey(i);
    }
}
