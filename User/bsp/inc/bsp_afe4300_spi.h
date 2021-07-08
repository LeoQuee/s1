/**
  ******************************************************************************
  * @file    bsp_afe4300_spi.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */  
 
#ifndef _BSP_AFE4300_SPI_H_
#define _BSP_AFE4300_SPI_H_		

#include "global_config.h"

#define     I_MODE          (0x0063)
#define     Q_MODE          (0x0065)
#define     FWR_MODE        (0x0000)
#define     IQ_MODE         (0x0800)

#define		TimPrescaler_7		(7)
#define		TimPrescaler_4		(4)
#define		TimPrescaler_3		(3)

/********************* AFE4300 �Ĵ���address ***********************/
#define		DEVICE_CONTROL1					(0x09)
#define		DEVICE_CONTROL2     			(0x0F)

#define		ADC_DATA_RESULT					(0x00)
#define		ADC_CONTROL_REGISTER1			(0x01)
#define		ADC_CONTROL_REGISTER2     		(0x10)

#define		WEIGHT_SCALE_CONTROL   			(0x0D)

#define		ISW_MUX							(0x0A)
#define		VSENSE_MUX						(0x0B)
#define		BCM_DAC_FREQ	   				(0x0E)
#define		IQ_MODE_ENABLE					(0x0C)

#define		MISC_REGISTER1					(0x02)
#define		MISC_REGISTER2					(0x03)
#define		MISC_REGISTER3     		    	(0x1A)

//----------------------------adcת������----------------------------
#define ADC_RATE_8_SPS      (0 << 4)
#define ADC_RATE_16_SPS     (1 << 4)
#define ADC_RATE_32_SPS     (2 << 4)
#define ADC_RATE_64_SPS     (3 << 4)
#define ADC_RATE_128_SPS    (4 << 4)
#define ADC_RATE_250_SPS    (5 << 4)
#define ADC_RATE_475_SPS    (6 << 4)
#define ADC_RATE_860_SPS    (7 << 4)

typedef struct
{
	uint16_t usTimPeriod;
	uint16_t usTimPrescaler;
}TIM_CFG;

static void bsp_AfeHardInit(void);  /*!< AFE4300Ӳ����ʼ����SPI��ʼ����AFE4300����ʱ�ӳ�ʼ���� */
static unsigned char SPI_ReadWriteByte(SPI_TypeDef* SPIx, unsigned char TxData); /*!< SPI��дһ���ֽ� */
static void bsp_AfeRegisterInit(void); /*!< AFE4300�Ĵ�����ʼ�� */
static void bsp_AfeReset(void);    /*!< AFE4300��λ */
static void _bsp_AfeWriteReg(unsigned char spiAddr, unsigned short spiData); /*!< дAFE4300�Ĵ��� */
static void bsp_AfeWriteReg(unsigned char spiAddr, unsigned short spiData); /*!< дAFE4300�Ĵ��� */


void bsp_Afe4300Init(void); /*!< AFE4300��ʼ�� */
short bsp_AfeReadReg(unsigned char spiAddr);    /*!< ��ȡAFE4300�Ĵ��� */
void bsp_AfeSetMode(int32_t mode);  /*!< AFE4300����ģʽ */
void bsp_AfeSetDacAndClk(int32_t dac ,int32_t decode_clk); /*!< ����DACƵ�� */
void bsp_AfeSetIorQMode(uint16_t mode); /*!< ����IQģʽ */
void bsp_AfeSetVESENSE_MUX(uint16_t channel); /*!< ���ò���ͨ�� */
void bsp_AfeSetISW_MUX(uint16_t channel);   /*!< ���ü���ͨ�� */
void bsp_SetAfeTIMClk(uint16_t tim_Period, uint16_t tim_Prescaler); /*!< ����ʱ��Ƶ�� */

#endif
