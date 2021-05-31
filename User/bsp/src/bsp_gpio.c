#include "bsp_gpio.h"

int8_t s_pcb_verion = PCB_UNKNOWN;

/**
  * @brief ��ʼ��Ӳ���汾��������
  */
void init_version_bit(void)
{
#ifdef     VERSION_BIT_RCC
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ��1������GPIOʱ�� */
    RCC_APB2PeriphClockCmd(VERSION_BIT_RCC, ENABLE);

    /* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* ���븡��ģʽ */

    /* ��3��������ִ�����ú��� */
    GPIO_InitStructure.GPIO_Pin = VERSION_BIT0_PIN;
    GPIO_Init(VERSION_BIT0_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = VERSION_BIT1_PIN;
    GPIO_Init(VERSION_BIT1_PORT, &GPIO_InitStructure);

    if(GPIO_ReadInputDataBit(VERSION_BIT0_PORT, VERSION_BIT0_PIN) == Bit_RESET
        && GPIO_ReadInputDataBit(VERSION_BIT1_PORT, VERSION_BIT1_PIN) == Bit_RESET)
    {
        s_pcb_verion = PCB_RECTANGULAR_lATTICE;
    }
    else if(GPIO_ReadInputDataBit(VERSION_BIT0_PORT, VERSION_BIT0_PIN) == Bit_SET
        && GPIO_ReadInputDataBit(VERSION_BIT1_PORT, VERSION_BIT1_PIN) == Bit_RESET)
    {
        s_pcb_verion = PCB_FINGER;
    }
    else if(GPIO_ReadInputDataBit(VERSION_BIT0_PORT, VERSION_BIT0_PIN) == Bit_RESET
        && GPIO_ReadInputDataBit(VERSION_BIT1_PORT, VERSION_BIT1_PIN) == Bit_SET)
    {
        s_pcb_verion = PCB_BA_GUA;
    }
    else
    {
        s_pcb_verion = PCB_RESERVED;
    }
#endif    
}
