#include "bsp_gpio.h"

int8_t s_pcb_verion = PCB_UNKNOWN;

/**
  * @brief 初始化硬件版本控制引脚
  */
void init_version_bit(void)
{
#ifdef     VERSION_BIT_RCC
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 第1步：打开GPIO时钟 */
    RCC_APB2PeriphClockCmd(VERSION_BIT_RCC, ENABLE);

    /* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* 输入浮空模式 */

    /* 第3步：依次执行配置函数 */
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
