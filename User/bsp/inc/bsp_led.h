/**
  ******************************************************************************
  * @file    bsp_led.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */  

#ifndef _BSP_LED_H_
#define _BSP_LED_H_

#include "global_config.h"

#if (defined PCB_V3) || (defined PCB_V4)
#define LED_RED_OFF()       GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN)
#define LED_RED_ON()        GPIO_SetBits(LED_RED_PORT, LED_RED_PIN)
#define LED_YELLOW_OFF()    GPIO_ResetBits(LED_YELLOW_PORT, LED_YELLOW_PIN)
#define LED_YELLOW_ON()     GPIO_SetBits(LED_YELLOW_PORT, LED_YELLOW_PIN)
#define LED_GREEN_OFF()     GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN)
#define LED_GREEN_ON()      GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN)
#else
#define LED_RED_OFF()       (s_pcb_verion == PCB_FINGER)?GPIO_ResetBits(LED2_RED_PORT, LED2_RED_PIN):GPIO_ResetBits(LED1_RED_PORT, LED1_RED_PIN)
#define LED_RED_ON()        (s_pcb_verion == PCB_FINGER)?GPIO_SetBits(LED2_RED_PORT, LED2_RED_PIN):GPIO_SetBits(LED1_RED_PORT, LED1_RED_PIN)
#define LED_YELLOW_OFF()    (s_pcb_verion == PCB_FINGER)?GPIO_ResetBits(LED2_YELLOW_PORT, LED2_YELLOW_PIN):GPIO_ResetBits(LED1_YELLOW_PORT, LED1_YELLOW_PIN)
#define LED_YELLOW_ON()     (s_pcb_verion == PCB_FINGER)?GPIO_SetBits(LED2_YELLOW_PORT, LED2_YELLOW_PIN):GPIO_SetBits(LED1_YELLOW_PORT, LED1_YELLOW_PIN)
#define LED_GREEN_OFF()     (s_pcb_verion == PCB_FINGER)?GPIO_ResetBits(LED2_GREEN_PORT, LED2_GREEN_PIN):GPIO_ResetBits(LED1_GREEN_PORT, LED1_GREEN_PIN)
#define LED_GREEN_ON()      (s_pcb_verion == PCB_FINGER)?GPIO_SetBits(LED2_GREEN_PORT, LED2_GREEN_PIN):GPIO_SetBits(LED1_GREEN_PORT, LED1_GREEN_PIN)
#endif

#define LED_RED         0
#define LED_GREEN       1
#define LED_YELLOW      2

typedef enum{
    LED_OFF,
    LED_ON,
    LED_BLINK,
}LED_STATUS;

void led_init(void);    /*!< LED³õÊ¼»¯ */
LED_STATUS bsp_GetLedStatus(uint8_t led_id);    /*!< ¶ÁÈ¡LED×´Ì¬ */
void bsp_SetLedStatus(uint8_t led_id, LED_STATUS status);   /*!< ÉèÖÃLED×´Ì¬ */

#endif
