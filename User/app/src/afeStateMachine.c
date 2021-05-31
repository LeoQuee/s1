/*
 * @Description:  afe4300 主要功能实现
 * @Author: Gaven
 * @Date: 2019-10-21 18:12:10
 * @LastEditTime: 2020-03-19 16:25:36
 * @LastEditors: Please set LastEditors
 */
 
#include "afeStateMachine.h"
#include "protocol_task.h"
#include "mid\mmath.h"
#include "bsp_dac.h"

//----------------------------AFE4300激励通道配置----------------------------
#define RN0         (1 << 0)
#define RN1         (1 << 1)
#define IOUT_N0     (1 << 2)
#define IOUT_N1     (1 << 3)
#define IOUT_N2     (1 << 4)
#define IOUT_N3     (1 << 5)
#define IOUT_N4     (1 << 6)
#define IOUT_N5     (1 << 7)
#define RP0         (1 << 8)
#define RP1         (1 << 9)
#define IOUT_P0     (1 << 10)
#define IOUT_P1     (1 << 11)
#define IOUT_P2     (1 << 12)
#define IOUT_P3     (1 << 13)
#define IOUT_P4     (1 << 14)
#define IOUT_P5     (1 << 15)
//----------------------------AFE4300测量通道配置----------------------------
#define VSENSEM_R0  (1 << 0)
#define VSENSEM_R1  (1 << 1)
#define VSENSEN0    (1 << 2)
#define VSENSEN1    (1 << 3)
#define VSENSEN2    (1 << 4)
#define VSENSEN3    (1 << 5)
#define VSENSEN4    (1 << 6)
#define VSENSEN5    (1 << 7)
#define VSENSEP_R0  (1 << 8)
#define VSENSEP_R1  (1 << 9)
#define VSENSEP0    (1 << 10)
#define VSENSEP1    (1 << 11)
#define VSENSEP2    (1 << 12)
#define VSENSEP3    (1 << 13)
#define VSENSEP4    (1 << 14)
#define VSENSEP5    (1 << 15)

#define FREQ_NUM    12      /*!< 测量频率点数 */

/**
  * @brief 频率测试点结构体
  */
typedef struct 
{
    float       clk;           /*!< AFE4300时钟频率(kHz) = 72M/(tim_period+1)/(tim_prescaler+1) */
    uint16_t    tim_period;    /*!< 定时器周期 */
    uint16_t    tim_prescaler; /*!< 定时器分频数 */
    float       freq;          /*!< 激励频率(kHz) */
    uint16_t    IQ_decodeClk;  /*!< 0x2800,0x2000,0x1800,0x1000,0x0800,0x0000 */
    uint16_t    IQ_DAC;        /*!< 8, 16, 32, 64, 128, 256 */
} TestFreq;

/**
  * @brief 所有频率测试点信息
  */
static TestFreq freq_list[FREQ_NUM] = 
{
    {750,  24-1, 4-1, 5.86f,   0x2800, 8},    /*!< 72M/24/4/128 = 5.86K */
    {1000, 18-1, 4-1, 7.81f,   0x2800, 8},    /*!< 72M/18/4/128 = 7.81K */
    {750,  24-1, 4-1, 11.72f,  0x2000, 16},   /*!< 72M/24/4/64 = 11.72K */
    {1000, 18-1, 4-1, 15.63f,  0x2000, 16},   /*!< 72M/18/4/64 = 15.63K */
    {750,  24-1, 4-1, 23.44f,  0x1800, 32},   /*!< 72M/24/4/32 = 23.44K */
    {1000, 18-1, 4-1, 31.25f,  0x1800, 32},   /*!< 72M/18/4/32 = 31.25K */
    {750,  24-1, 4-1, 46.88f,  0x1000, 64},   /*!< 72M/24/4/16 = 46.88K */
    {1000, 18-1, 4-1, 62.5f,   0x1000, 64},   /*!< 72M/18/4/16 = 62.5K */
    {750,  24-1, 4-1, 93.75f,  0x0800, 128},  /*!< 72M/24/4/8 = 93.75K */
    {1000, 18-1, 4-1, 125,     0x0800, 128},  /*!< 72M/18/4/8 = 125K */
    {750,  24-1, 4-1, 187.5f,  0x0000, 256},  /*!< 72M/24/4/4 = 187.5K */
    {1000, 18-1, 4-1, 250,     0x0000, 256},  /*!< 72M/18/4/4 = 250K */
};

//有几个频率，则有几个系数 100 ~ 1k ~ 10k ~ 200k 分别测6个频率，6*2 = 12
static LEASQ_COEFFICIENT g_CalibLsqCoeff[FREQ_NUM] = {0};

/***********************************************测量部位配置*******************/
const static CHANNEL_CONFIG s_ChannelCfg1[] =
{
    //测量部位           激励通道                测量通道
    
    // 校准通道
    {RESISTANCE1,       RN0 | RP0,          VSENSEP_R0|VSENSEM_R0},
    {RESISTANCE2,       RN1 | RP0,          VSENSEP_R0|VSENSEM_R1},
    {RESISTANCE3,       RN1 | RP1,          VSENSEP_R1|VSENSEM_R1},

    // 左右手4*4矩阵点阵
    {BODY_POSITION_L1,    IOUT_N2|IOUT_P0,     VSENSEP0|VSENSEN1},
    {BODY_POSITION_L2,    IOUT_N2|IOUT_P0,     VSENSEP0|VSENSEN2},
    
    {BODY_POSITION_R1,    IOUT_N1|IOUT_P0,     VSENSEP0|VSENSEN3},
    {BODY_POSITION_R2,    IOUT_N1|IOUT_P0,     VSENSEP0|VSENSEN4},
};

const static CHANNEL_CONFIG s_ChannelCfg2[] =
{
    //测量部位           激励通道                测量通道

    // 校准通道
    {RESISTANCE1,       RN0 | RP0,          VSENSEP_R0|VSENSEM_R0},
    {RESISTANCE2,       RN1 | RP0,          VSENSEP_R0|VSENSEM_R1},
    {RESISTANCE3,       RN1 | RP1,          VSENSEP_R1|VSENSEM_R1},

#ifdef PCB_V3
    // 左手指
    {BODY_POSITION_L1,    IOUT_N1|IOUT_P0,     VSENSEP0|VSENSEN3},
    {BODY_POSITION_L2,    IOUT_N1|IOUT_P0,     VSENSEP0|VSENSEN4},
    
    // 右手指
    {BODY_POSITION_R1,    IOUT_N2|IOUT_P0,     VSENSEP0|VSENSEN1},
    {BODY_POSITION_R2,    IOUT_N2|IOUT_P0,     VSENSEP0|VSENSEN2},
#else
    // 右手指
    {BODY_POSITION_L1,    IOUT_N2|IOUT_P0,     VSENSEP0|VSENSEN1},
    {BODY_POSITION_L2,    IOUT_N2|IOUT_P0,     VSENSEP0|VSENSEN2},
    
    // 左手指
    {BODY_POSITION_R1,    IOUT_N1|IOUT_P0,     VSENSEP0|VSENSEN3},
    {BODY_POSITION_R2,    IOUT_N1|IOUT_P0,     VSENSEP0|VSENSEN4},
#endif    
};

static const CHANNEL_CONFIG *s_ChannelCfg = s_ChannelCfg1;

#define CYCLE_SUM               (5)   //采集5组IQ值进行均值滤波，CYCLE_SUM >= 4

static float s_basePhase = 0;//基准相角
static float s_baseRes = 0; // 校准电阻阻值

////////////////////////////////////////////////////////////////////////////////

#define AFE_CHANGE_MODE_TIME    (500) //切换IQ模式后的延时，单位ms

static AFE4300_WORK_STATE s_afe4300WorkState = AFE_STATE_INIT;

typedef struct
{
    uint8_t status;
    uint8_t freq_id;
    uint8_t freq_num;
    uint8_t point_id;
    uint8_t point_num;
	uint8_t cur_point;
    uint8_t channel_id;
    uint8_t step;
} TestParam;

typedef struct
{
    float freq[FREQ_NUM];
    float R[FREQ_NUM];
    float phase[FREQ_NUM];    
} MEASURE_RESULT;

// 测量
static TestParam s_test = {STATUS_OFF};
static MEASURE_RESULT s_test_res = {0};

typedef struct
{
    uint8_t status;
    uint8_t freq_id;
    uint8_t freq_num;
    uint8_t channel_id;
    uint8_t channel_num;
    uint8_t step;
    LEASQ_RAW_DATA lsqRawData;
} AdjustParam;

// 校准
static AdjustParam s_adjust = {STATUS_OFF};
static AFE4300_SAMPLE_RESULT sampleResult =  {0};

////////////////////////////////////////////////////////////////////////////////

/**
  * @brief 设备是否已校准
  * @return 0-未校准 1-已校准
  */
static uint8_t IsDeviceAdjusted(void)
{
    uint8_t i = 0;
    
    for(i = 0; i < FREQ_NUM; i++)
    {
        if(g_CalibLsqCoeff[i].R2 < 0.9f)
        {
            return 0;
        }
    }
    
    return 1;
}

/**
  * @brief 设置基准相角
  * @param phase 基准相角，°
  */
static void SetBasePhase(float phase)
{
    s_basePhase = phase;
}

/**
  * @brief 读取基准相角
  * @return 基准相角，°
  */
static float GetBasePhase(void)
{
    return s_basePhase;
}

/**
 * @brief 判断当前通道是否为测量通道
 * @param channel 当前通道id
 * @return: 1：测量通道， 0：校准通道
 */
static uint8_t IsTestChannel(AFE4300_POSITION channel)
{
    if (channel <= RESISTANCE3)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief 根据I、Q值计算电压
 * @details 用sqrtf代替sqrt,运算效率会提高很多
 * @param _I I模式读取的16位ad
 * @param _Q Q模式读取的16位ad
 * @return 电压, 单位(mv)
 */
static float Afe4300CalVoltageByIQ(float _I, float _Q)
{
    return sqrtf(_I * _I +_Q * _Q) * 1000 * 1.7f / 32767;
}

/**
  * @brief 根据I、Q值计算相角
  * @details 用atanf代替atan,运算效率会提高很多,因为atan返回double型
  * @param _I I模式读取的ad
  * @param _Q Q模式读取的ad
  * @return: 相角，单位 (°)
  */
static float Afe4300CalPhaseByIQ(float _I, float _Q)
{
    return atanf(_Q / _I) * 180.0f / PI;
}

/**
  * @brief 切换激励和测量通道
  * @param channelCfg 通道配置结构体
  */
static void Afe4300SetIV_MUX(CHANNEL_CONFIG channelCfg)
{
    bsp_AfeSetISW_MUX(channelCfg.isw_mux);
    bsp_AfeSetVESENSE_MUX(channelCfg.vsense_mux);
}

/**
  * @brief 设置AFE4300时钟频率
  * @param tim_period 
  * @param tim_prescaler
  */
static void AFE4300_SetExtClk(uint16_t tim_period, uint16_t tim_prescaler)
{
    static uint16_t s_tim_period = 0;
    static uint16_t s_tim_prescaler = 0;

    if(s_tim_period == tim_period && s_tim_prescaler == tim_prescaler)
    {
        return;
    }
    
    bsp_SetAfeTIMClk(tim_period, tim_prescaler);
    
    s_tim_period = tim_period;
    s_tim_prescaler = tim_prescaler;
}

/**
  * @brief 读取AFE4300 AD值
  * @retval AFE4300 AD值
  */
static int16_t Afe4300_ReadAdc(void)
{
    while(GPIO_ReadInputDataBit(AFE4300_RDY_PORT, AFE4300_RDY_PIN) == Bit_SET); // 等待RDY_N低电平
    while(GPIO_ReadInputDataBit(AFE4300_RDY_PORT, AFE4300_RDY_PIN) == Bit_RESET); // 等待RDY_N高电平
    
    return bsp_AfeReadReg(ADC_DATA_RESULT);
}

/**
 * @brief 计算采样结果
 * @param curFreq DAC激励频率
 * @param channelPosition 测量位置
 * @param pointIndex 测量点
 * @param sampleResult 采样结果
 * @return 返回当前采样状态（进行中，完成）
 */
static uint8_t IsAfeSampleOver(float curFreq, AFE4300_POSITION channelPosition, uint8_t pointIndex, AFE4300_SAMPLE_RESULT *sampleResult)
{
    uint32_t index = 0;
    static int16_t ad_value_I[CYCLE_SUM] = {0};
    int16_t ad_value_Q[CYCLE_SUM] = {0};
    uint8_t dataBuff[10] = {0};
    char *positionName = NULL;
    
    switch(channelPosition)
    {
    case RESISTANCE1:
        positionName = "Res1";
        break;
    case RESISTANCE2:
        positionName = "Res2";
        break;
    case RESISTANCE3:
        positionName = "Res3";
        break;
    default:
        positionName = "Human";
        break;
    }
    
    if(sampleResult->flag == 0)
    {
        bsp_AfeSetIorQMode(I_MODE);
        if(s_adjust.step == 1)
        {
            bsp_StartTimer(TMR5_AFE_CHANGE_MODE, AFE_CHANGE_MODE_TIME*2);
        }
        else
        {
            bsp_StartTimer(TMR5_AFE_CHANGE_MODE, AFE_CHANGE_MODE_TIME*2);
        }
        sampleResult->flag = 1;
#ifdef DEBUG1
        _PRINT("\r\n\r\nI=");
#endif        
    }
#ifdef DEBUG1
    else if(sampleResult->flag == 1 && bsp_CheckTimer(TMR5_AFE_CHANGE_MODE, 0) == 0)
    {
        _PRINT("%d,", Afe4300_ReadAdc());
    }
#endif    
    else if(sampleResult->flag == 1 && bsp_CheckTimer(TMR5_AFE_CHANGE_MODE, 0) == 1)
    {
        for(index = 0; index < CYCLE_SUM; index++)
        {
            ad_value_I[index] = Afe4300_ReadAdc();
        }
        
        sampleResult->flag = 2;
    }
    else if(sampleResult->flag == 2)
    {
        bsp_AfeSetIorQMode(Q_MODE);
        if(s_adjust.step == 1)
        {
            bsp_StartTimer(TMR5_AFE_CHANGE_MODE, AFE_CHANGE_MODE_TIME);
        }
        else
        {
            bsp_StartTimer(TMR5_AFE_CHANGE_MODE, AFE_CHANGE_MODE_TIME);
        }
        sampleResult->flag = 3;
#ifdef DEBUG1
        _PRINT("\r\n\r\nQ=");
#endif 
    }
#ifdef DEBUG1
    else if(sampleResult->flag == 3 && bsp_CheckTimer(TMR5_AFE_CHANGE_MODE, 0) == 0)
    {
        _PRINT("%d,", Afe4300_ReadAdc());
    }
#endif
    else if(sampleResult->flag == 3 && bsp_CheckTimer(TMR5_AFE_CHANGE_MODE, 0) == 1)
    {
        for(index = 0; index < CYCLE_SUM; index++)
        {
            ad_value_Q[index] = Afe4300_ReadAdc();
            
            if(s_test.step == 3 || s_adjust.step == 1)
            {
                dataBuff[0] = channelPosition;
                dataBuff[1] = pointIndex;   //当测量部位是板置电阻时,该字节无效.
                memcpy(&dataBuff[2], &curFreq, 4);
                memcpy(&dataBuff[2+4], &ad_value_I[index], 2);
                memcpy(&dataBuff[2+4+2], &ad_value_Q[index], 2);
                
                PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_DAT_IQ_SAMPLE_AD, EProtocolType_Data, sizeof(dataBuff), dataBuff);
            }
        }
        
        _PRINT("\r\n%s:%d,freq:%.2f,", positionName, pointIndex, curFreq);
        _PRINT("I:%d,%d,%d,%d,%d,", ad_value_I[0], ad_value_I[1], ad_value_I[2], ad_value_I[3], ad_value_I[4]);
        _PRINT("Q:%d,%d,%d,%d,%d,", ad_value_Q[0], ad_value_Q[1], ad_value_Q[2], ad_value_Q[3], ad_value_Q[4]);
        
        sampleResult->mean_I = (short)MeanFiltering(ad_value_I, sizeof(ad_value_I)/sizeof(ad_value_I[0]));
        sampleResult->mean_Q = (short)MeanFiltering(ad_value_Q, sizeof(ad_value_Q)/sizeof(ad_value_Q[0]));
        sampleResult->mean_V = Afe4300CalVoltageByIQ(sampleResult->mean_I, sampleResult->mean_Q);
        sampleResult->phase = Afe4300CalPhaseByIQ(sampleResult->mean_I, sampleResult->mean_Q);
        
        _PRINT("meanI:%d,meanQ:%d,meanV:%.2f,phase:%.2f,",
            sampleResult->mean_I, 
            sampleResult->mean_Q, 
            sampleResult->mean_V,
            sampleResult->phase);
        
        sampleResult->flag = 4;
        
        return AFE_SUCCESS;
    }
    
    return AFE_EXCUTING;
}

////////////////////////////////////////////////////////////////////////////////


/**
  * @brief 启动校准
  */
void AFE4300_Set_Adjust_On(void)
{
    SYSFLAG s;
    
    sysflag_read(&s);
    
    s_adjust.lsqRawData.resistance[0] = s.R1;
    s_adjust.lsqRawData.resistance[1] = s.R2;
    s_adjust.lsqRawData.resistance[2] = s.R3;
    s_adjust.lsqRawData.voltage[0] = 0;
    s_adjust.lsqRawData.voltage[1] = 0;
    s_adjust.lsqRawData.voltage[2] = 0;
    
    s_adjust.status = STATUS_ON;
    s_adjust.freq_id = 0;
    s_adjust.freq_num = FREQ_NUM;
    s_adjust.channel_id = 0;
    s_adjust.channel_num = 3;
    s_adjust.step = 0;
    
    if(s_pcb_verion == PCB_FINGER)
    {
        s_ChannelCfg = s_ChannelCfg2;
    }
    else
    {
        s_ChannelCfg = s_ChannelCfg1;
    }
}

/**
  * @brief AFE4300校准
  * @return 失败，成功，进行中
  */
AFE_STATE_ERROR_TYPE AFE4300_StartAdjust(void)
{
    LEASQ_COEFFICIENT *pLsqCoeff;
    
    if(s_adjust.status == STATUS_OFF)
    {
        return AFE_FAILED;
    }
    
    if(s_adjust.step == 0)
    {
        // 设置时钟频率
        AFE4300_SetExtClk(freq_list[s_adjust.freq_id].tim_period, freq_list[s_adjust.freq_id].tim_prescaler);
        // 设置激励频率
        bsp_AfeSetMode(IQ_MODE);
        bsp_AfeSetDacAndClk(freq_list[s_adjust.freq_id].IQ_DAC, freq_list[s_adjust.freq_id].IQ_decodeClk);
        // 设置测量通道
        Afe4300SetIV_MUX(s_ChannelCfg[s_adjust.channel_id]);
        memset(&sampleResult, 0, sizeof(sampleResult));
        s_adjust.step = 1;
    }
    else if(s_adjust.step == 1 && AFE_SUCCESS == IsAfeSampleOver(freq_list[s_adjust.freq_id].freq, s_ChannelCfg[s_adjust.channel_id].channelPosition, TP_NONE, &sampleResult))
    {
        s_adjust.step = 0;
        s_adjust.lsqRawData.voltage[s_adjust.channel_id] = sampleResult.mean_V;
        
        s_adjust.channel_id ++;
        if(s_adjust.channel_id == s_adjust.channel_num)
        {
            uint8_t dataBuff[16];
            
            s_adjust.channel_id = 0;
            
            pLsqCoeff = &g_CalibLsqCoeff[s_adjust.freq_id];
            *pLsqCoeff = LeastsqLinearFit(s_adjust.lsqRawData.voltage, s_adjust.lsqRawData.resistance, 3);
            //上传m、o、R2等参数
            _PRINT("\r\n\r\nFreq=%.2f, m=%f, o=%f, R2=%f\r\n", freq_list[s_adjust.freq_id].freq, pLsqCoeff->coeff_m, pLsqCoeff->coeff_o, pLsqCoeff->R2);
            
            memcpy(&dataBuff[0], &freq_list[s_adjust.freq_id].freq, 4);
            memcpy(&dataBuff[0+4], &pLsqCoeff->coeff_m, 4);
            memcpy(&dataBuff[0+4+4], &pLsqCoeff->coeff_o, 4);
            memcpy(&dataBuff[0+4+4+4], &pLsqCoeff->R2, 4);
            PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_DAT_ADJUST_COEFF, EProtocolType_Data, 16, dataBuff);
            
            if(pLsqCoeff->R2 < 0.9f)
            {
                _PRINT("adjust failed:R2 < 0.9!\r\n");
                
                PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_LINEAR_REGRESSION_FAIL, EProtocolType_Notice, 0, NULL);
                
                s_adjust.status = STATUS_OFF;
                
                return AFE_FAILED;
            }
            
            s_adjust.freq_id ++;
            if(s_adjust.freq_id == s_adjust.freq_num)
            {
                s_adjust.status = STATUS_OFF;
                
                return AFE_SUCCESS;
            }
        }
    }
    
    return AFE_EXCUTING;
}

/**
 * @brief 测量指定频率下的单点阻抗和相角
 * @param pointIndex 测量点
 * @param curFreq 当前频率，kHz
 * @param channelPosition 测量位置
 * @param coeffStruct 校准系数
 * @return 如果当前测量点的I、Q模式下的AD都已经测量完，则返回1，否则返回0
 */
static AFE_STATE_ERROR_TYPE Afe4300SetTestPosition2(uint8_t pointIndex, float curFreq, AFE4300_POSITION channelPosition, LEASQ_COEFFICIENT coeffStruct)
{
    float predictResistance = 0;
    uint8_t dataBuff[18] = {0};
    
    if(AFE_SUCCESS == IsAfeSampleOver(curFreq, channelPosition, pointIndex,  &sampleResult))
    {
        predictResistance = sampleResult.mean_V * coeffStruct.coeff_m + coeffStruct.coeff_o;
        if (!IsTestChannel(channelPosition))
        {
            if(channelPosition == RESISTANCE2)
            {
                s_baseRes = predictResistance;
                SetBasePhase(sampleResult.phase);
                
                _PRINT("R:%.2f, phase:%.2f", predictResistance, sampleResult.phase);
            }
        }
        else
        {
            _PRINT("R:%.2f, phase:%.2f", predictResistance, sampleResult.phase);
            
            sampleResult.phase -= GetBasePhase();
            predictResistance = predictResistance * 510/s_baseRes;
            
#ifdef _USART_PRINT_DEBUG_
            s_test_res.freq[s_test.freq_id] = curFreq;
            s_test_res.R[s_test.freq_id] = predictResistance;
            s_test_res.phase[s_test.freq_id] = sampleResult.phase;
#else
            if(s_test.step == 3)
            {
                dataBuff[0] = pointIndex;
                memcpy(&dataBuff[1], &curFreq, 4);
                memcpy(&dataBuff[1+4], &predictResistance, 4);
                memcpy(&dataBuff[1+4+4], &sampleResult.mean_I, 2);
                memcpy(&dataBuff[1+4+4+2], &sampleResult.mean_Q, 2);
                memcpy(&dataBuff[1+4+4+2+2], &sampleResult.phase, 4);
                dataBuff[1+4+4+2+2+4] = GetAfe4300State();
                PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_DAT_IMPEDANCE_PHASE, EProtocolType_Data, 18, dataBuff);
            }
#endif
        }
        return AFE_SUCCESS;
    }

    return AFE_EXCUTING;
}

////////////////////////////////////////////////////////////////////////////////

/**
  * @brief 启动测量
  */
void AFE4300_Set_Test_On(void)
{
    if(!IsDeviceAdjusted())
    {
        _PRINT("adjust coeff R2 is error, stop test!\r\n");
        
        PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_DEVICE_NOT_ADJUSTED, EProtocolType_Notice, 0, NULL);
        
        s_test.status = STATUS_OFF;
    }
    else
    {
        s_test.status = STATUS_ON;
        s_test.freq_id = 0;
        s_test.freq_num = FREQ_NUM;
        s_test.point_id = 0;
        if(s_pcb_verion == PCB_FINGER)
        {
            s_test.point_num = 20 + 1;
        }
        else
        {
            s_test.point_num = 32 + 1;
        }
        s_test.channel_id = 0;
        s_test.step = 0;
        
        // 控制提示音
        bsp_dac_open(5);
    }
}

/**
  * @brief AFE4300测量
  * @return 失败，成功，进行中
  */
AFE_STATE_ERROR_TYPE AFE4300_StartTest(void)
{
    if(s_test.status == STATUS_OFF)
    {
        return AFE_FAILED;
    }
    
    // 预测过程，半分钟
    if(s_test.step == 0)
    {
        // 设置时钟频率
        AFE4300_SetExtClk(freq_list[s_test.freq_id].tim_period, freq_list[s_test.freq_id].tim_prescaler);
        // 设置激励频率
        bsp_AfeSetMode(IQ_MODE);
        bsp_AfeSetDacAndClk(freq_list[s_test.freq_id].IQ_DAC, freq_list[s_test.freq_id].IQ_decodeClk);
        // 设置测量通道
        if(s_test.point_id == 0)
        {
            s_test.channel_id = 1; // 校准电阻2相角作为参考
        }
        else
        {
            mux508_set((RECT_LATTICE)s_test.point_id); // 设置测量点
            if(s_pcb_verion == PCB_FINGER)
            {
                s_test.channel_id = 3 + (s_test.point_id-1)/5;
            }
            else           
            {
                s_test.channel_id = 3 + (s_test.point_id-1)/8;   
            }
        }
        Afe4300SetIV_MUX(s_ChannelCfg[s_test.channel_id]);
        memset(&sampleResult, 0, sizeof(sampleResult));
        s_test.step = 1;
    }
    else if(s_test.step == 1 && AFE_SUCCESS == Afe4300SetTestPosition2(s_test.point_id, freq_list[s_test.freq_id].freq, s_ChannelCfg[s_test.channel_id].channelPosition, g_CalibLsqCoeff[s_test.freq_id]))
    {
        s_test.step = 0;
        
        s_test.point_id ++;
        if(s_test.point_id == s_test.point_num)
        {
            s_test.point_id = 0;
            s_test.step = 2;
            
            // 控制提示音
            bsp_dac_open(1);
        }
    }
    
    // 测量过程，5分钟
    if(s_test.step == 2)
    {
        // 设置时钟频率
        AFE4300_SetExtClk(freq_list[s_test.freq_id].tim_period, freq_list[s_test.freq_id].tim_prescaler);
        // 设置激励频率
        bsp_AfeSetMode(IQ_MODE);
        bsp_AfeSetDacAndClk(freq_list[s_test.freq_id].IQ_DAC, freq_list[s_test.freq_id].IQ_decodeClk);
        // 设置测量通道
        if(s_test.point_id == 0)
        {
            s_test.channel_id = 1; // 校准电阻2相角作为参考
        }
        else
        {
            mux508_set((RECT_LATTICE)s_test.point_id); // 设置测量点
            if(s_pcb_verion == PCB_FINGER)
                s_test.channel_id = 3 + (s_test.point_id-1)/5;
            else           
                s_test.channel_id = 3 + (s_test.point_id-1)/8;   
        }
        Afe4300SetIV_MUX(s_ChannelCfg[s_test.channel_id]);
        memset(&sampleResult, 0, sizeof(sampleResult));
        s_test.step = 3;
    }
    else if(s_test.step == 3 && AFE_SUCCESS == Afe4300SetTestPosition2(s_test.point_id, freq_list[s_test.freq_id].freq, s_ChannelCfg[s_test.channel_id].channelPosition, g_CalibLsqCoeff[s_test.freq_id]))
    {
        s_test.step = 2;
        
        s_test.point_id ++;
        if(s_test.point_id == s_test.point_num)
        {
            s_test.point_id = 0;
            
            s_test.freq_id ++;
            if(s_test.freq_id == s_test.freq_num)
            {
                s_test.status = STATUS_OFF;
                
                return AFE_SUCCESS;
            }
        }
    }
    
    return AFE_EXCUTING;
}

/**
 * @brief 读取4300状态
 * @return 4300状态
 */
AFE4300_WORK_STATE GetAfe4300State(void)
{
    return s_afe4300WorkState;
}

/**
 * @brief 设置4300状态
 * @param mode 4300状态
 */
void SetAfe4300State(AFE4300_WORK_STATE mode)
{
    s_afe4300WorkState = mode;
}

/**
  * @brief 停止校准/测量
  */
void  AFE4300_Stop(void)
{
    s_adjust.status = STATUS_OFF;
    s_test.status = STATUS_OFF;
    
    bsp_dac_close();
}

/**
  * @brief 启动单点测量
  * @param point_id 测量点id
  */
void AFE4300_Set_SingleTest_On(uint8_t point_id)
{
    if(s_pcb_verion == PCB_FINGER && (point_id < 1 || point_id > 20))
    {
        return;
    }
    
    if(s_pcb_verion == PCB_RECTANGULAR_lATTICE && (point_id < 1 || point_id > 32))
    {
        return;
    }
    
    s_test.status = STATUS_ON;
    s_test.freq_id = 0;
    s_test.freq_num = FREQ_NUM;
    s_test.point_id = point_id;
    s_test.point_num = 32 + 1;
    s_test.cur_point = 0;
    s_test.channel_id = 0;
    s_test.step = 0;
    
    if(!IsDeviceAdjusted())
    {
        _PRINT("adjust coeff R2 is error, stop test!\r\n");
        
        PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_DEVICE_NOT_ADJUSTED, EProtocolType_Notice, 0, NULL);
        
        s_test.status = STATUS_OFF;
    }
    
    SetAfe4300State(AFE_STATE_SINGLE_TEST);
}

/**
  * @brief 单点测量
  * @return 成功，失败，进行中
  */
AFE_STATE_ERROR_TYPE AFE4300_SingleTest(void)
{    
    if(s_test.status == STATUS_OFF)
    {
        return AFE_FAILED;
    }
    
    if(s_test.step == 0)
    {
        // 设置时钟频率
        AFE4300_SetExtClk(freq_list[s_test.freq_id].tim_period, freq_list[s_test.freq_id].tim_prescaler);
        // 设置激励频率
        bsp_AfeSetMode(IQ_MODE);
        bsp_AfeSetDacAndClk(freq_list[s_test.freq_id].IQ_DAC, freq_list[s_test.freq_id].IQ_decodeClk);
        // 设置测量通道
        if(s_test.cur_point == 0)
        {
            s_test.channel_id = 1; // 校准电阻2相角作为参考
        }
        else
        {
            mux508_set((RECT_LATTICE)s_test.cur_point); // 设置测量点
            if(s_pcb_verion == PCB_FINGER)
            {
                s_test.channel_id = (s_test.cur_point+14)/5;
            }
            else            
            {
                s_test.channel_id = (s_test.cur_point+23)/8;  
            }
        }
        Afe4300SetIV_MUX(s_ChannelCfg[s_test.channel_id]);
        memset(&sampleResult, 0, sizeof(sampleResult));
        s_test.step = 1;
    }
    else if(s_test.step == 1 && AFE_SUCCESS == Afe4300SetTestPosition2(s_test.cur_point, freq_list[s_test.freq_id].freq, s_ChannelCfg[s_test.channel_id].channelPosition, g_CalibLsqCoeff[s_test.freq_id]))
    {
        s_test.step = 0;
        
//        if(s_test.cur_point < s_test.point_id)
//        {
//            s_test.cur_point ++;
//        }
        if(s_test.cur_point == 0)
        {
            s_test.cur_point = s_test.point_id;
        }
        else
        {
            s_test.cur_point = 0;
            
            s_test.freq_id ++;
            if(s_test.freq_id == s_test.freq_num)
            {
                int i = 0;
                
                _PRINT("\r\n\r\nfreq\tR\tphase");
                for(i=0; i<FREQ_NUM; i++)
                {
                    _PRINT("\r\n%.2f\t%.2f\t%.2f", s_test_res.freq[i], s_test_res.R[i], s_test_res.phase[i]);
                }
                s_test.freq_id = 0;
                s_test.status = STATUS_OFF;
                
                return AFE_SUCCESS;
            }
        }
    }
    
    return AFE_EXCUTING;
}


//////////////////////////////////////////////////////////////////////////////// 调试功能

/**
  * @brief 调试功能-切换测量点
  */
void AFE4300_PointInc(void)
{
    s_test.point_id ++;
    if(s_test.point_id >= s_test.point_num)
    {
        s_test.point_id = 1;
    }
    
    _PRINT("Point:%d\r\n", s_test.point_id);
}
