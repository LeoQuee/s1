/**
  ******************************************************************************
  * @file    bsp_afe4300_spi.c
  * @author  lik
  * @date    2021-7-8
  * @brief   afe4300 Ӳ����ʼ��ģ��
  ******************************************************************************
  */  
 
#include "bsp_afe4300_spi.h"
#include "bsp.h"

#if (defined PCB_V3) || (defined PCB_V4)  
#define SPI_AFE4300 SPI3
#else
#define SPI_AFE4300 SPI1
#endif

/**
  * @brief AFE4300Ӳ����ʼ����SPI��ʼ����AFE4300����ʱ�ӳ�ʼ����
  */
static void bsp_AfeHardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
#if (defined PCB_V3) || (defined PCB_V4)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    RCC_APB2PeriphClockCmd(SPI3_RCC, ENABLE);
    
    RCC_APB2PeriphClockCmd(AFE4300_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // ����JTAG
#else
    RCC_APB2PeriphClockCmd(SPI1_RCC, ENABLE);//SPI1
    RCC_APB2PeriphClockCmd(AFE4300_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // ����JTAG
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); // TIM2CH1 -> PA15 TIM2CH2 -> PB3
#endif    
    
    // AFE4300 RESET_N
    GPIO_InitStructure.GPIO_Pin   = AFE4300_RST_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(AFE4300_RST_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(AFE4300_RST_PORT, AFE4300_RST_PIN);

    // AFE4300 CLK_IN  1MHZ����
    GPIO_InitStructure.GPIO_Pin   = AFE4300_CLK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(AFE4300_CLK_PORT, &GPIO_InitStructure);
    
    // AFE4300 RDY
    GPIO_InitStructure.GPIO_Pin   = AFE4300_RDY_PIN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(AFE4300_RDY_PORT, &GPIO_InitStructure);
    
#if (defined PCB_V3) || (defined PCB_V4)
    //PA15---SPI3 NSS---STE_N
    GPIO_InitStructure.GPIO_Pin   = SPI3_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI3_NSS_PORT, &GPIO_InitStructure);

    //PB3---SPI3 SCK---SCLK
    //PB4---SPI3 MISO--SDIN
    //PB5---SPI3 MOSI---SDOUT
    GPIO_InitStructure.GPIO_Pin   = SPI3_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI3_SCK_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = SPI3_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI3_MOSI_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = SPI3_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI3_MISO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(SPI3_SCK_PORT,  SPI3_SCK_PIN);
    GPIO_SetBits(SPI3_MOSI_PORT, SPI3_MOSI_PIN);
    GPIO_SetBits(SPI3_MISO_PORT, SPI3_MISO_PIN);
#else
    //PA4---SPI1 NSS---STE_N
    GPIO_InitStructure.GPIO_Pin   = SPI1_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_NSS_PORT, &GPIO_InitStructure);

    //PA5---SPI1 SCK---SCLK
    //PA6---SPI1 MISO--SDIN
    //PA7---SPI1 MOSI---SDOUT
    GPIO_InitStructure.GPIO_Pin   = SPI1_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_SCK_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = SPI1_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = SPI1_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_MISO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(SPI1_SCK_PORT,  SPI1_SCK_PIN);
    GPIO_SetBits(SPI1_MOSI_PORT, SPI1_MOSI_PIN);
    GPIO_SetBits(SPI1_MISO_PORT, SPI1_MISO_PIN);
#endif

    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;//SPI_Mode_Master    SPI_Mode_Slave
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;    //SPI_NSS_Hard        SPI_NSS_Soft
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_Init(SPI_AFE4300, &SPI_InitStructure);
    SPI_Cmd(SPI_AFE4300, ENABLE);
    SPI_I2S_ITConfig(SPI_AFE4300, SPI_I2S_IT_RXNE,ENABLE);

    //SPI_ReadWriteByte(SPI_AFE4300, 0xff);//��������
    
    // TIM_Period ����ֵ�� TIM_Period=17   ��Ƶ���� TIM_Prescaler=3 ��0Ϊ����Ƶ����APB2 ʱ����72Mhz��
    // 72Mhz/��17+1��/��3+1��= 1Mhz
    TIM_TimeBaseStructure.TIM_Period            = 17;    //  TIM_Period=70;    TIM_Prescaler=0;    ���1Mhzʱ��
    TIM_TimeBaseStructure.TIM_Prescaler         = 3;
    TIM_TimeBaseStructure.TIM_ClockDivision     = 0x00;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;   //����TIM1-8
    TIM_TimeBaseInit(AFE4300_CLK_TIM, &TIM_TimeBaseStructure);
    
    TIM_OCInitStructure.TIM_OCMode     = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;
    //TIM_Pulse��ֵ���ɴ���TIM_Period
    TIM_OCInitStructure.TIM_Pulse      =9;
#if (defined PCB_V3) || (defined PCB_V4)    
    TIM_OC4Init(AFE4300_CLK_TIM, &TIM_OCInitStructure);
#else
    TIM_OC2Init(AFE4300_CLK_TIM, &TIM_OCInitStructure);
#endif
    
    TIM_Cmd(AFE4300_CLK_TIM, ENABLE);			 //TIMxʹ��
    TIM_CtrlPWMOutputs(AFE4300_CLK_TIM, ENABLE); //TIMxPWM���
}

/**
  * @brief SPI��дһ���ֽ�
  * @param TxData:Ҫд����ֽ�
  * @return ��ȡ�����ֽ�
  */
unsigned char SPI_ReadWriteByte(SPI_TypeDef* SPIx, unsigned char TxData)
{
    unsigned char retry=0;
  
    while((SPIx->SR&1<<1)==0)
    {
        retry++;
        if(retry>200) return 0;
    }
    
    SPIx->DR=TxData;
    retry=0;
    
    while((SPIx->SR&1<<0)==0)
    {
        retry++;
        if(retry>200) return 0;
    }
    
    return SPIx->DR;
}

/**
  * @brief ����ʱ��Ƶ��
  * @param tim_Period
  * @param tim_Prescaler
  */
void bsp_SetAfeTIMClk(uint16_t tim_Period, uint16_t tim_Prescaler)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // TIM_Period ����ֵ�� TIM_Period=17   ��Ƶ���� TIM_Prescaler=3 ��0Ϊ����Ƶ����APB2 ʱ����72Mhz��
    // 72Mhz/��17+1��/��3+1��= 1Mhz
    TIM_TimeBaseStructure.TIM_Period            = tim_Period;
    TIM_TimeBaseStructure.TIM_Prescaler         = tim_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;   //����TIM1-8
    TIM_TimeBaseInit(AFE4300_CLK_TIM, &TIM_TimeBaseStructure);
    
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    //TIM_Pulse��ֵ���ɴ���TIM_Period
    TIM_OCInitStructure.TIM_Pulse       = (tim_Period+1)/2;      // ����ռ�ձȣ���Ӧÿ�����ڵ͵�ƽ����ʱ��
#if (defined PCB_V3) || (defined PCB_V4)
    TIM_OC4Init(AFE4300_CLK_TIM, &TIM_OCInitStructure);
#else    
    TIM_OC2Init(AFE4300_CLK_TIM, &TIM_OCInitStructure);
#endif
}

/**
  * @brief ���ü���ͨ��
  * @param channel
  */
void bsp_AfeSetISW_MUX(uint16_t channel)
{
	//ISW_MUX : ��������ͨ·����
    bsp_AfeWriteReg(ISW_MUX, channel);
}

/**
  * @brief ���ò���ͨ��
  * @param channel
  */
void bsp_AfeSetVESENSE_MUX(uint16_t channel)
{
	//VESENSE_MUX : ��ѹ����ͨ������
    bsp_AfeWriteReg(VSENSE_MUX, channel);		//	�����ͨ��������ͬ			
}

/**
  * @brief ����IQģʽ
  * @mode 
  */
void bsp_AfeSetIorQMode(uint16_t mode)
{
	//ADC_CONTROL_REG2 : Iģʽ��Qģʽѡ��
    bsp_AfeWriteReg(ADC_CONTROL_REGISTER2, mode); ///Iģʽ��0x0063; Qģʽ��0x0065
}

/**
  * @brief ����DACƵ��
  * @param dac
  * @param decode_clk
  */
void bsp_AfeSetDacAndClk(int32_t dac, int32_t decode_clk)
{
    static int s_dac = -1;
    static int s_decode_clk = -1;
    
    if(s_dac == dac && s_decode_clk == decode_clk)
    {
        return;
    }
    else
    {
        s_dac = dac;
        s_decode_clk = decode_clk;
    }
    
    bsp_AfeWriteReg(DEVICE_CONTROL2, 0x0000);
    bsp_AfeWriteReg(DEVICE_CONTROL2, 0x0000);
    
    bsp_AfeWriteReg(BCM_DAC_FREQ, 0x0000);
    bsp_AfeWriteReg(BCM_DAC_FREQ, 0x0000);
    
    DISABLE_INT();
    
    // BCM_DAC_FREQ : DACƵ������
    bsp_AfeWriteReg(BCM_DAC_FREQ, dac);
    bsp_AfeWriteReg(BCM_DAC_FREQ, dac);

    //DEVICE_CONTROL2 : I_MODE/Q�����ʱ��Ƶ������,FWRģʽ��������
    bsp_AfeWriteReg(DEVICE_CONTROL2, decode_clk);
    bsp_AfeWriteReg(DEVICE_CONTROL2, decode_clk);
    
    ENABLE_INT();
}

/**
  * @brief AFE4300�Ĵ�����ʼ��
  */
void bsp_AfeRegisterInit(void)
{
    bsp_AfeReset();
    bsp_AfeWriteReg(ADC_CONTROL_REGISTER1,  0x4100 | ADC_RATE_128_SPS); // ����ģʽ��������룬�����ٶ�128SPS=��������8ms@1MHz
    bsp_AfeWriteReg(DEVICE_CONTROL1,        0x6006); // DAC����Դ��BCMȫ������
}

/**
  * @brief AFE4300���ù���ģʽ
  * @param mode ����ģʽ
  */
void bsp_AfeSetMode(int32_t mode)
{
    //IQ_MODE_ENABLE: IQ_MODE 0x0800 ��ȫ������0x00
    bsp_AfeWriteReg(IQ_MODE_ENABLE, mode);
    
    if(mode == FWR_MODE)
    {
        bsp_AfeWriteReg(ADC_CONTROL_REGISTER2, 0x63); ///Iģʽ��0x0063; Qģʽ��0x0065
    }
}

/**
  * @brief AFE4300��λ
  */
void bsp_AfeReset(void)
{
    GPIO_ResetBits(AFE4300_RST_PORT, AFE4300_RST_PIN);
    bsp_DelayMS(5); // ���������ֲᣬ��������10us
    GPIO_SetBits(AFE4300_RST_PORT, AFE4300_RST_PIN);
    bsp_DelayMS(5); // ���������ֲᣬ��������1ms

    //Write REGISTERS after power up and/or reset
    bsp_AfeWriteReg(MISC_REGISTER1, 0x0000);
    bsp_AfeWriteReg(MISC_REGISTER2, 0xFFFF);
    bsp_AfeWriteReg(MISC_REGISTER3, 0x0030);
}

/**
  * @brief AFE4300��ʼ�� 
  * @details ��������Ż�AFE4300 GPIO��ʱ�ӵĲ���ϸ��
  *          PA4: SIP1_NSS
  *          PA5: SIP1_SCK
  *          PB6: SIP1_MISO
  *          PB7: SIP1_MOSI
  */
void bsp_Afe4300Init(void)
{
    _PRINT("bsp_Afe4300Init()\r\n");
    
    bsp_AfeHardInit();
    bsp_AfeRegisterInit();
}

/**
  * @brief  дAFE4300�Ĵ���
  * @param spiAddr���Ĵ�����ַ
  * @param spiData��16λ������
  */
void bsp_AfeWriteReg(unsigned char spiAddr, unsigned short spiData)
{
    //�ȷ��͵�ַ���ٷ������ݸ�8λ�͵�8λ
    SPI_AFE4300_CS = 1;
    SPI_AFE4300_CS = 0;
    SPI_ReadWriteByte(SPI_AFE4300, spiAddr);
    SPI_ReadWriteByte(SPI_AFE4300, spiData >> 8);
    SPI_ReadWriteByte(SPI_AFE4300, spiData);
    SPI_AFE4300_CS = 1;
}

/**
  * @brief ��ȡAFE4300�Ĵ���
  * @param spiAddr �Ĵ�����ַ
  * @return �Ĵ�������
  */
short bsp_AfeReadReg(unsigned char spiAddr)
{
    short spiData = 0;
    
    SPI_AFE4300_CS = 1;
    SPI_AFE4300_CS = 0;
    spiData = SPI_ReadWriteByte(SPI_AFE4300, 0x20 | spiAddr);      //���Ͷ�ȡ״̬�Ĵ�������,���ض�ȡ����ֵ unsigned
    spiData = (SPI_ReadWriteByte(SPI_AFE4300, 0x00)) << 8;	       // Read MSB data
    spiData |= SPI_ReadWriteByte(SPI_AFE4300, 0x00);               // Read LSB data
    SPI_AFE4300_CS = 1;
    if(spiAddr != ADC_DATA_RESULT) bsp_AfeWriteReg(spiAddr, spiData); // Writeback read data due to feature bug on the BCM device
    
    return spiData;             // Return SPI read data
}
