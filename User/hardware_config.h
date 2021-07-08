/**
  ******************************************************************************
  * @file    hardware_config.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#ifndef _HARDWARE_CONFIG_H_
#define _HARDWARE_CONFIG_H_

#include "stm32f10x.h"

/*******************************************************************************
矩形点阵版V1.0
单片机：STM32F103RCT6
RAM：48K
ROM：256K
*******************************************************************************/
// #define PCB_V1
// #define RECTANGULAR_lATTICE_V0_1     // 第1版矩阵板

//#define PCB_V2
//#define RECTANGULAR_lATTICE_V0_2      // 第2版矩阵板
//#define PCB_FINGER_V0_1               // 第1版手指侧板

#define PCB_V3                          // 第2版手指侧板
//#define PCB_V4                          // 第3版手指侧板

////////////////////////////////////////////////////////////////////////////////
#if (defined RECTANGULAR_lATTICE_V0_1) || (defined RECTANGULAR_lATTICE_V0_2) || (defined PCB_FINGER_V0_1)

// 多路开关
#define MUX508_RCC          (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB)

#define MUX508_A0_PORT      GPIOA
#define MUX508_A0_PIN       GPIO_Pin_0

#define MUX508_A1_PORT      GPIOA
#define MUX508_A1_PIN       GPIO_Pin_1

#define MUX508_A2_PORT      GPIOA
#define MUX508_A2_PIN       GPIO_Pin_2

#define MUX508_EN1_PORT     GPIOB
#define MUX508_EN1_PIN      GPIO_Pin_15

#define MUX508_EN2_PORT     GPIOB
#define MUX508_EN2_PIN      GPIO_Pin_14

#define MUX508_EN3_PORT     GPIOB
#define MUX508_EN3_PIN      GPIO_Pin_12

#define MUX508_EN4_PORT     GPIOB
#define MUX508_EN4_PIN      GPIO_Pin_13

// AFE4300
#define AFE4300_RCC         (RCC_APB2Periph_GPIOB)

#define AFE4300_CLK_TIM		(TIM2)
#define AFE4300_CLK_CH      (CH2)
#define AFE4300_CLK_PORT    GPIOB
#define AFE4300_CLK_PIN     GPIO_Pin_3

#define AFE4300_RDY_PORT    GPIOB
#define AFE4300_RDY_PIN     GPIO_Pin_4

#define AFE4300_RST_PORT    GPIOB
#define AFE4300_RST_PIN     GPIO_Pin_5

// SPI1
#define SPI1_RCC            (RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA)

#define SPI1_NSS_PORT       GPIOA
#define SPI1_NSS_PIN        GPIO_Pin_4
#define	SPI_AFE4300_CS	    PAout(4)

#define SPI1_SCK_PORT       GPIOA
#define SPI1_SCK_PIN        GPIO_Pin_5

#define SPI1_MISO_PORT      GPIOA
#define SPI1_MISO_PIN       GPIO_Pin_6

#define SPI1_MOSI_PORT      GPIOA
#define SPI1_MOSI_PIN       GPIO_Pin_7

#define USART1_RCC          (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA)

#define USART1_TX_PORT      GPIOA
#define USART1_TX_PIN       GPIO_Pin_9

#define USART1_RX_PORT      GPIOA
#define USART1_RX_PIN       GPIO_Pin_10

// 按键
#define KEY_RCC             (RCC_APB2Periph_GPIOC)

#define KEY1_PORT           GPIOC
#define KEY1_PIN            GPIO_Pin_6

#define KEY2_PORT           GPIOC
#define KEY2_PIN            GPIO_Pin_7

#define KEY3_PORT           GPIOC
#define KEY3_PIN            GPIO_Pin_8

#define KEY4_PORT           GPIOC
#define KEY4_PIN            GPIO_Pin_9

// 指示灯
#ifdef RECTANGULAR_lATTICE_V0_1
#define LED1_RCC             (RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC)

#define LED1_RED_PORT        GPIOC
#define LED1_RED_PIN         GPIO_Pin_5

#define LED1_YELLOW_PORT     GPIOB
#define LED1_YELLOW_PIN      GPIO_Pin_0

#define LED1_GREEN_PORT      GPIOB
#define LED1_GREEN_PIN       GPIO_Pin_1
#else
#define LED1_RCC             (RCC_APB2Periph_GPIOA)

#define LED1_RED_PORT        GPIOA
#define LED1_RED_PIN         GPIO_Pin_8

#define LED1_YELLOW_PORT     GPIOA
#define LED1_YELLOW_PIN      GPIO_Pin_11

#define LED1_GREEN_PORT      GPIOA
#define LED1_GREEN_PIN       GPIO_Pin_12
#endif

#define LED2_RCC             (RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC)

#define LED2_RED_PORT        GPIOC
#define LED2_RED_PIN         GPIO_Pin_5

#define LED2_YELLOW_PORT     GPIOB
#define LED2_YELLOW_PIN      GPIO_Pin_0

#define LED2_GREEN_PORT      GPIOB
#define LED2_GREEN_PIN       GPIO_Pin_1


// I2C1-温湿度计
#define I2C1_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB)
#define I2C1_REMAP

#define I2C1_SCL_PORT       GPIOB
#define I2C1_SCL_PIN        GPIO_Pin_8

#define I2C1_SDA_PORT       GPIOB
#define I2C1_SDA_PIN        GPIO_Pin_9

// I2C2-液晶屏
#define I2C2_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB)

#define I2C2_SCL_PORT       GPIOB
#define I2C2_SCL_PIN        GPIO_Pin_10

#define I2C2_SDA_PORT       GPIOB
#define I2C2_SDA_PIN        GPIO_Pin_11

// 硬件版本控制引脚
#define VERSION_BIT_RCC     (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)

#define VERSION_BIT0_PORT   GPIOC
#define VERSION_BIT0_PIN    GPIO_Pin_12

#define VERSION_BIT1_PORT   GPIOD
#define VERSION_BIT1_PIN    GPIO_Pin_2

////////////////////////////////////////////////////////////////////////////////
#elif (defined PCB_V3)

// 多路开关
#define MUX508_RCC          (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC)

#define MUX508_A0_PORT      GPIOA
#define MUX508_A0_PIN       GPIO_Pin_0

#define MUX508_A1_PORT      GPIOA
#define MUX508_A1_PIN       GPIO_Pin_1

#define MUX508_A2_PORT      GPIOA
#define MUX508_A2_PIN       GPIO_Pin_2

#define MUX508_EN1_PORT     GPIOC
#define MUX508_EN1_PIN      GPIO_Pin_5

#define MUX508_EN2_PORT     GPIOC
#define MUX508_EN2_PIN      GPIO_Pin_4

#define MUX508_EN3_PORT     GPIOA
#define MUX508_EN3_PIN      GPIO_Pin_8

#define MUX508_EN4_PORT     GPIOA
#define MUX508_EN4_PIN      GPIO_Pin_12

// AFE4300
#define AFE4300_RCC         (RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO)

#define AFE4300_CLK_TIM		(TIM3)
#define AFE4300_CLK_CH      (CH4)
#define AFE4300_CLK_PORT    GPIOB
#define AFE4300_CLK_PIN     GPIO_Pin_1

#define AFE4300_RDY_PORT    GPIOB
#define AFE4300_RDY_PIN     GPIO_Pin_6

#define AFE4300_RST_PORT    GPIOB
#define AFE4300_RST_PIN     GPIO_Pin_7

// SPI3
#define SPI3_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB)

#define SPI3_NSS_PORT       GPIOA
#define SPI3_NSS_PIN        GPIO_Pin_15
#define	SPI_AFE4300_CS	    PAout(15)

#define SPI3_SCK_PORT       GPIOB
#define SPI3_SCK_PIN        GPIO_Pin_3

#define SPI3_MISO_PORT      GPIOB
#define SPI3_MISO_PIN       GPIO_Pin_4

#define SPI3_MOSI_PORT      GPIOB
#define SPI3_MOSI_PIN       GPIO_Pin_5

// SPI2 --- Flash
#define SPI2_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB)

#define SPI2_NSS_PORT       GPIOB
#define SPI2_NSS_PIN        GPIO_Pin_12

#define SPI2_SCK_PORT       GPIOB
#define SPI2_SCK_PIN        GPIO_Pin_13

#define SPI2_MISO_PORT      GPIOB
#define SPI2_MISO_PIN       GPIO_Pin_14

#define SPI2_MOSI_PORT      GPIOB
#define SPI2_MOSI_PIN       GPIO_Pin_15

#define USART1_RCC          (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA)

#define USART1_TX_PORT      GPIOA
#define USART1_TX_PIN       GPIO_Pin_9

#define USART1_RX_PORT      GPIOA
#define USART1_RX_PIN       GPIO_Pin_10

// 按键
#define KEY_RCC             (RCC_APB2Periph_GPIOC)

#define KEY1_PORT           GPIOC
#define KEY1_PIN            GPIO_Pin_6

#define KEY2_PORT           GPIOC
#define KEY2_PIN            GPIO_Pin_9

#define KEY3_PORT           GPIOC
#define KEY3_PIN            GPIO_Pin_8

#define KEY4_PORT           GPIOC
#define KEY4_PIN            GPIO_Pin_7

// 指示灯
#define LED_RCC             (RCC_APB2Periph_GPIOA)

#define LED_RED_PORT        GPIOA
#define LED_RED_PIN         GPIO_Pin_5

#define LED_YELLOW_PORT     GPIOA
#define LED_YELLOW_PIN      GPIO_Pin_6

#define LED_GREEN_PORT      GPIOA
#define LED_GREEN_PIN       GPIO_Pin_7

// I2C1-温湿度计
#define I2C1_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB)
#define I2C1_REMAP

#define I2C1_SCL_PORT       GPIOB
#define I2C1_SCL_PIN        GPIO_Pin_8

#define I2C1_SDA_PORT       GPIOB
#define I2C1_SDA_PIN        GPIO_Pin_9

// I2C2-液晶屏
#define I2C2_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB)

#define I2C2_SCL_PORT       GPIOB
#define I2C2_SCL_PIN        GPIO_Pin_10

#define I2C2_SDA_PORT       GPIOB
#define I2C2_SDA_PIN        GPIO_Pin_11

// 硬件版本控制引脚
#define VERSION_BIT_RCC     (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)

#define VERSION_BIT0_PORT   GPIOC
#define VERSION_BIT0_PIN    GPIO_Pin_12

#define VERSION_BIT1_PORT   GPIOD
#define VERSION_BIT1_PIN    GPIO_Pin_2

#define DAC_PORT            GPIOA
#define DAC_PIN             GPIO_Pin_4

////////////////////////////////////////////////////////////////////////////////
#elif (defined PCB_V4)

// 多路开关
#define ADG708_RCC          (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC)

#define ADG708_A0_PORT      GPIOA
#define ADG708_A0_PIN       GPIO_Pin_0

#define ADG708_A1_PORT      GPIOA
#define ADG708_A1_PIN       GPIO_Pin_1

#define ADG708_A2_PORT      GPIOA
#define ADG708_A2_PIN       GPIO_Pin_2

#define ADG708_I1_EN_PORT   GPIOC
#define ADG708_I1_EN_PIN    GPIO_Pin_5

#define ADG708_I2_EN_PORT   GPIOC
#define ADG708_I2_EN_PIN    GPIO_Pin_4

#define ADG708_I3_EN_PORT   GPIOC
#define ADG708_I3_EN_PIN    GPIO_Pin_3

#define ADG708_V1_EN_PORT   GPIOC
#define ADG708_V1_EN_PIN    GPIO_Pin_2

#define ADG708_V2_EN_PORT   GPIOC
#define ADG708_V2_EN_PIN    GPIO_Pin_1

#define ADG708_V3_EN_PORT   GPIOC
#define ADG708_V3_EN_PIN    GPIO_Pin_0

// AFE4300
#define AFE4300_RCC         (RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO)

#define AFE4300_CLK_TIM		(TIM3)
#define AFE4300_CLK_CH      (CH4)
#define AFE4300_CLK_PORT    GPIOB
#define AFE4300_CLK_PIN     GPIO_Pin_1

#define AFE4300_RDY_PORT    GPIOB
#define AFE4300_RDY_PIN     GPIO_Pin_6

#define AFE4300_RST_PORT    GPIOB
#define AFE4300_RST_PIN     GPIO_Pin_7

// SPI3
#define SPI3_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB)

#define SPI3_NSS_PORT       GPIOA
#define SPI3_NSS_PIN        GPIO_Pin_15
#define	SPI_AFE4300_CS	    PAout(15)

#define SPI3_SCK_PORT       GPIOB
#define SPI3_SCK_PIN        GPIO_Pin_3

#define SPI3_MISO_PORT      GPIOB
#define SPI3_MISO_PIN       GPIO_Pin_4

#define SPI3_MOSI_PORT      GPIOB
#define SPI3_MOSI_PIN       GPIO_Pin_5

// SPI2 --- Flash
#define SPI2_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB)

#define SPI2_NSS_PORT       GPIOB
#define SPI2_NSS_PIN        GPIO_Pin_12

#define SPI2_SCK_PORT       GPIOB
#define SPI2_SCK_PIN        GPIO_Pin_13

#define SPI2_MISO_PORT      GPIOB
#define SPI2_MISO_PIN       GPIO_Pin_14

#define SPI2_MOSI_PORT      GPIOB
#define SPI2_MOSI_PIN       GPIO_Pin_15

// 串口
#define USART1_RCC          (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA)

#define USART1_TX_PORT      GPIOA
#define USART1_TX_PIN       GPIO_Pin_9

#define USART1_RX_PORT      GPIOA
#define USART1_RX_PIN       GPIO_Pin_10

// 按键
#define KEY_RCC             (RCC_APB2Periph_GPIOC)

#define KEY1_PORT           GPIOC
#define KEY1_PIN            GPIO_Pin_6

#define KEY2_PORT           GPIOC
#define KEY2_PIN            GPIO_Pin_9

#define KEY3_PORT           GPIOC
#define KEY3_PIN            GPIO_Pin_8

#define KEY4_PORT           GPIOC
#define KEY4_PIN            GPIO_Pin_7

// 指示灯
#define LED_RCC             (RCC_APB2Periph_GPIOA)

#define LED_RED_PORT        GPIOA
#define LED_RED_PIN         GPIO_Pin_5

#define LED_YELLOW_PORT     GPIOA
#define LED_YELLOW_PIN      GPIO_Pin_6

#define LED_GREEN_PORT      GPIOA
#define LED_GREEN_PIN       GPIO_Pin_7

// I2C1-温湿度计
#define I2C1_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB)
#define I2C1_REMAP

#define I2C1_SCL_PORT       GPIOB
#define I2C1_SCL_PIN        GPIO_Pin_8

#define I2C1_SDA_PORT       GPIOB
#define I2C1_SDA_PIN        GPIO_Pin_9

// I2C2-液晶屏
#define I2C2_RCC            (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB)

#define I2C2_SCL_PORT       GPIOB
#define I2C2_SCL_PIN        GPIO_Pin_10

#define I2C2_SDA_PORT       GPIOB
#define I2C2_SDA_PIN        GPIO_Pin_11

// 硬件版本控制引脚
#define VERSION_BIT_RCC     (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)

#define VERSION_BIT0_PORT   GPIOC
#define VERSION_BIT0_PIN    GPIO_Pin_12

#define VERSION_BIT1_PORT   GPIOD
#define VERSION_BIT1_PIN    GPIO_Pin_2

// DAC
#define DAC_PORT            GPIOA
#define DAC_PIN             GPIO_Pin_4

#else
#error pcb version undefined!
#endif

#endif
