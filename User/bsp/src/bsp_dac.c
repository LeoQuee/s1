#include "bsp_dac.h"
#include "bsp_data_interface.h"
#include "bsp_w25q128.h"
#include "bsp_oled.h"

#ifdef PCB_V3

#define DAC_DHR8R1_Address      0x40007410

static int cnt = 0;
static int flag = 0;
static int id = 0;
static uint8_t s_dac_volume = 16;
static uint8_t s_music_on = 0;

/**
  * @brief DAC初始化
  */
void bsp_dac_init(void)
{
    // DAC IO口初始化
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = DAC_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(DAC_PORT, &GPIO_InitStructure);
    }
    
    // DAC 外设初始化
    {
        DAC_InitTypeDef DAC_InitStructure;
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
        
        DAC_StructInit(&DAC_InitStructure);
        DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
        DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
        DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
        DAC_Init(DAC_Channel_1, &DAC_InitStructure);
        
        DAC_Cmd(DAC_Channel_1, ENABLE);
    }
    
    // DAC TIMER6中断初始化
    {
        NVIC_InitTypeDef NVIC_InitStructure;
    
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    
    // DAC TIMER6初始化
    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
        
        // 72M / 4500 / 1 = 16K
        TIM_DeInit(TIM6);
        TIM_TimeBaseStructure.TIM_Period = 4500 - 1;
        TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
        TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
        
        TIM_Cmd(TIM6, ENABLE);
    }
}

/**
  * @brief 关闭DAC输出
  */
void bsp_dac_close(void)
{
    flag = 0;
}

/**
  * @brief DAC音频地址
  */
const int c_addr[] = {
XFLASH_ADDR_SOUND1, 
XFLASH_ADDR_SOUND2, 
XFLASH_ADDR_SOUND3, 
XFLASH_ADDR_SOUND4, 
XFLASH_ADDR_MUSIC1,
XFLASH_ADDR_SOUND5};

/**
  * @brief 打开DAC输出
  */
void bsp_dac_open(uint8_t _id)
{   
    cnt = 0;
    flag = 1;
    id = _id;
}

/**
  * @brief DAC音量+
  */
void bsp_dac_volume_inc(void)
{
    s_music_on = 1;
    
    if(s_dac_volume < 16)
    {
        s_dac_volume ++;
        
#ifdef _OLED_ON_   
        oled_show_str(0, 0, "volume:");
        oled_show_char(56, 0, (s_dac_volume/10) + '0');
        oled_show_char(64, 0, (s_dac_volume%10) + '0');
#endif        
    }
}

/**
  * @brief DAC音量-
  */
void bsp_dac_volume_dec(void)
{
    s_music_on = 0;
    
    if(s_dac_volume > 0)
    {
        s_dac_volume --;

#ifdef _OLED_ON_        
        oled_show_str(0, 0, "volume:");
        oled_show_char(56, 0, (s_dac_volume/10) + '0');
        oled_show_char(64, 0, (s_dac_volume%10) + '0');
#endif        
    }
}

/**
  * @brief DAC 定时器中断
  */
void TIM6_IRQHandler(void)
{
    uint16_t data;
    
    TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
    
    if(flag == 1)
    {
        W25QXX_Read(c_addr[id]+cnt, sizeof(uint16_t), (uint8_t *)&data);
        
        if(data == 0xFFFF)
        {
            if((id == 1 && s_music_on == 1) || id == 4) // 测量开始提示语音结束后，继续播放音乐
            {
                id = 4;
                cnt = 0;
            }
            else
            {
                flag = 0;
            }
        }
        else
        {
            data = (data * s_dac_volume) >> 4;
            DAC_SetChannel1Data(DAC_Align_12b_R, data);
            DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
            cnt += 2;
        }
    }
}

#endif
