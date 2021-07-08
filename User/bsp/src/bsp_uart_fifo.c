/**
  ******************************************************************************
  * @file    bsp_uart_fifo.c
  * @author  lik
  * @date    2021-7-8
  * @brief   ���ô����ж�+FIFOģʽʵ�ֶ�����ڵ�ͬʱ����
  ******************************************************************************
  */ 
 
#include "bsp.h"
#include "Platformdef.h"
#include "afeStateMachine.h"

/**
  * @brief ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
  */
void bsp_InitUart(void)
{
    InitHardUart();		/* ���ô��ڵ�Ӳ������(�����ʵ�) */

    ConfigUartNVIC();	/* ���ô����ж� */
}

/**
  * @brief ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ�
  */
static void InitHardUart(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* ��1������GPIO��USART������ʱ�� */
    RCC_APB2PeriphClockCmd(USART1_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    /* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
    GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);

    /* ��3������USART Rx��GPIO����Ϊ��������ģʽ
    	����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
    	���ǣ����Ǽ��ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
    */
    GPIO_InitStructure.GPIO_Pin  = USART1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);

    /* ��4���� ���ô���Ӳ������ */
    USART_InitStructure.USART_BaudRate = UART_BAUD;	/* ������ */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
    USART_Cmd(USART1, ENABLE);		/* ʹ�ܴ��� */

    /* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
    	�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
    USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
}

/**
  * @brief ���ô���Ӳ���ж�
  */
static void ConfigUartNVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

    /* ʹ�ܴ���1�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

extern ComCtrl CommToPc;
HardwareCtrl* PHardwareCtrl = &(CommToPc.m_HardwareCtrl);

#define UART_USE_IT_TC       //ʹ������bitλȫ��������ϵ��ж�

/**
  * @brief ����1�жϴ�����
  * @param _pUart
  */
static void Usart1IRQ(void)
{
    uint8_t ch;
    
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)/* ��������ж� */
    {
        ch = USART_ReceiveData(USART1);
        if((PHardwareCtrl->m_RxComBuffCtrl.m_iWriteIndex + 1) % PHardwareCtrl->m_RxComBuffCtrl.m_iLen != PHardwareCtrl->m_RxComBuffCtrl.m_iReadIndex)
        {
            PHardwareCtrl->m_RxComBuffCtrl.m_pBuff[PHardwareCtrl->m_RxComBuffCtrl.m_iWriteIndex] = ch;	
            PHardwareCtrl->m_RxComBuffCtrl.m_iWriteIndex = (PHardwareCtrl->m_RxComBuffCtrl.m_iWriteIndex+1) % PHardwareCtrl->m_RxComBuffCtrl.m_iLen;		
        }
    }

    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)/* �����ͻ��������ж� */
    {
        if(PHardwareCtrl->m_TxComBuffCtrl.m_iWriteIndex != PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex)
		{
			ch = PHardwareCtrl->m_TxComBuffCtrl.m_pBuff[PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex];
			PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex = (PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex+1)%PHardwareCtrl->m_TxComBuffCtrl.m_iLen;	
            USART_SendData(USART1, ch);
		}
		else
		{
		    /* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
#ifdef UART_USE_IT_TC
            USART_ITConfig(USART1, USART_IT_TC, ENABLE);
#endif
		}
    }
#ifdef UART_USE_IT_TC
    else if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)/* ����bitλȫ��������ϵ��ж� */
    {
        if(PHardwareCtrl->m_TxComBuffCtrl.m_iWriteIndex != PHardwareCtrl->m_TxComBuffCtrl.m_iReadIndex)
        {
            /* ��������£��������˷�֧ */

            /* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
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
  * @brief ����1�жϷ������
  */
void USART1_IRQHandler(void)
{
    Usart1IRQ();
}

/**
  * @brief �ض���putc������ʹ��printf����ʱ�õ�
  * @param ch
  * @param f
  * @return 
  */
int fputc(int ch, FILE *f)
{
#ifdef _USART_PRINT_DEBUG_	/* ����������ʽ����ÿ���ַ�,�ȴ����ݷ������ */
    USART_SendData(USART1, (uint8_t) ch);

    /* �ȴ����ͽ��� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}

    return ch;
#else
    /*ʹ��ITM��ʽ��ӡ*/
    ITM_SendChar((uint32_t)ch);
    
    return ch;
#endif
}
