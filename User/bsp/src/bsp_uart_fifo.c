/**
  ******************************************************************************
  * @file    bsp_uart_fifo.c
  * @author  lik
  * @date    2021-7-8
  * @brief   采用串口中断+FIFO模式实现多个串口的同时访问
  ******************************************************************************
  */ 
 
#include "bsp.h"
#include "Platformdef.h"
#include "afeStateMachine.h"

/**
  * @brief 初始化串口硬件，并对全局变量赋初值.
  */
void bsp_InitUart(void)
{
    InitHardUart();		/* 配置串口的硬件参数(波特率等) */

    ConfigUartNVIC();	/* 配置串口中断 */
}

/**
  * @brief 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）
  */
static void InitHardUart(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* 第1步：打开GPIO和USART部件的时钟 */
    RCC_APB2PeriphClockCmd(USART1_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    /* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
    GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);

    /* 第3步：将USART Rx的GPIO配置为浮空输入模式
    	由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
    	但是，还是加上便于阅读，并且防止其它地方修改了这个口线的设置参数
    */
    GPIO_InitStructure.GPIO_Pin  = USART1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);

    /* 第4步： 配置串口硬件参数 */
    USART_InitStructure.USART_BaudRate = UART_BAUD;	/* 波特率 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
    USART_Cmd(USART1, ENABLE);		/* 使能串口 */

    /* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
    	如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
}

/**
  * @brief 配置串口硬件中断
  */
static void ConfigUartNVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

    /* 使能串口1中断 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

extern ComCtrl CommToPc;
HardwareCtrl* PHardwareCtrl = &(CommToPc.m_HardwareCtrl);

#define UART_USE_IT_TC       //使能数据bit位全部发送完毕的中断

/**
  * @brief 串口1中断处理函数
  * @param _pUart
  */
static void Usart1IRQ(void)
{
    uint8_t ch;
    
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)/* 处理接收中断 */
    {
        ch = USART_ReceiveData(USART1);
        if((PHardwareCtrl->m_RxComBuffCtrl.m_iWriteIndex + 1) % PHardwareCtrl->m_RxComBuffCtrl.m_iLen != PHardwareCtrl->m_RxComBuffCtrl.m_iReadIndex)
        {
            PHardwareCtrl->m_RxComBuffCtrl.m_pBuff[PHardwareCtrl->m_RxComBuffCtrl.m_iWriteIndex] = ch;	
            PHardwareCtrl->m_RxComBuffCtrl.m_iWriteIndex = (PHardwareCtrl->m_RxComBuffCtrl.m_iWriteIndex+1) % PHardwareCtrl->m_RxComBuffCtrl.m_iLen;		
        }
    }

    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)/* 处理发送缓冲区空中断 */
    {
        if(PHardwareCtrl->m_TxComBuffCtrl.m_iWriteIndex != PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex)
		{
			ch = PHardwareCtrl->m_TxComBuffCtrl.m_pBuff[PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex];
			PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex = (PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex+1)%PHardwareCtrl->m_TxComBuffCtrl.m_iLen;	
            USART_SendData(USART1, ch);
		}
		else
		{
		    /* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
#ifdef UART_USE_IT_TC
            USART_ITConfig(USART1, USART_IT_TC, ENABLE);
#endif
		}
    }
#ifdef UART_USE_IT_TC
    else if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)/* 数据bit位全部发送完毕的中断 */
    {
        if(PHardwareCtrl->m_TxComBuffCtrl.m_iWriteIndex != PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex)
        {
            /* 正常情况下，不会进入此分支 */

            /* 如果发送FIFO的数据还未完毕，则从发送FIFO取1个数据写入发送数据寄存器 */
            ch = PHardwareCtrl->m_TxComBuffCtrl.m_pBuff[PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex];
			PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex = (PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex+1)%PHardwareCtrl->m_TxComBuffCtrl.m_iLen;	
            USART_SendData(USART1, ch);
        }
        else
        {
            USART_ITConfig(USART1, USART_IT_TC, DISABLE);
        }
    }
#endif
}

/**
  * @brief 串口1中断服务程序
  */
void USART1_IRQHandler(void)
{
    Usart1IRQ();
}

/**
  * @brief 重定义putc函数，使用printf函数时用到
  * @param ch
  * @param f
  * @return 
  */
int fputc(int ch, FILE *f)
{
#ifdef _USART_PRINT_DEBUG_	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
    USART_SendData(USART1, (uint8_t) ch);

    /* 等待发送结束 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}

    return ch;
#else
    /*使用ITM方式打印*/
    ITM_SendChar((uint32_t)ch);
    
    return ch;
#endif
}
