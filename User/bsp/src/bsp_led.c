#include "bsp.h"

/**
  * @brief LED初始化
  */
void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    _PRINT("led_init()\r\n");
    
#ifdef PCB_V3
    RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = LED_YELLOW_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_YELLOW_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);
#else    
    if(s_pcb_verion == PCB_FINGER)
    {
        RCC_APB2PeriphClockCmd(LED2_RCC, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = LED2_RED_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(LED2_RED_PORT, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = LED2_YELLOW_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(LED2_YELLOW_PORT, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = LED2_GREEN_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(LED2_GREEN_PORT, &GPIO_InitStructure);
    }
    else
    {
        RCC_APB2PeriphClockCmd(LED1_RCC, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = LED1_RED_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(LED1_RED_PORT, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = LED1_YELLOW_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(LED1_YELLOW_PORT, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = LED1_GREEN_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(LED1_GREEN_PORT, &GPIO_InitStructure);
    }
#endif

    LED_RED_OFF();
    LED_YELLOW_OFF();
    LED_GREEN_ON();    
}


static LED_STATUS s_LedStatus[3] = {LED_OFF, LED_ON, LED_OFF};   /*!< LED当前状态 */

/**
  * @brief 读取LED状态
  * @param led_id
  * @return
  */
LED_STATUS bsp_GetLedStatus(uint8_t led_id)
{
    return s_LedStatus[led_id];
}

/**
  * @brief 设置LED状态
  * @param led_id
  * @param status
  */
void bsp_SetLedStatus(uint8_t led_id, LED_STATUS status)
{
    s_LedStatus[led_id] = status;
}

