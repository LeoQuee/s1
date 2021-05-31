/*
 * @Description:  AFE4300 主要功能实现头文件
 * @Author: Gaven
 * @Date: 2019-10-21 18:12:14
 * @LastEditTime: 2019-11-19 14:21:52
 * @LastEditors: Please set LastEditors
 */
 
#ifndef __AFE_STATEMACHINE_H_
#define __AFE_STATEMACHINE_H_

#include "global_config.h"
#include "bsp.h"
#include "globle.h"

/**
  * @brief 测量通道
  */
typedef enum {
    RESISTANCE1 = 0,        //校准电阻1
    RESISTANCE2 = 1,        //校准电阻2
    RESISTANCE3 = 2,        //校准电阻3
    BODY_POSITION_L1,      // Left 1
    BODY_POSITION_L2,      // Left 2
    BODY_POSITION_R1,      // Right 1
    BODY_POSITION_R2,      // Right 2
} AFE4300_POSITION;

/**
  * @brief 测量通道配置结构体
  */
typedef struct
{
    AFE4300_POSITION channelPosition;  /*!< 测量部位 */
    uint16_t isw_mux;        /*!< 激励通道 */
    uint16_t vsense_mux;     /*!< 测量通道 */
} CHANNEL_CONFIG;

/**
  * @brief 
  */
typedef struct
{
    float resistance[3];
    float voltage[3];
} LEASQ_RAW_DATA;

/**
  * @brief 4300采样结果
  */
typedef struct
{
    uint16_t flag;
    short mean_I;
    short mean_Q;
    float mean_V;
    float phase;
}AFE4300_SAMPLE_RESULT;

////////////////////////////////////////////////////////////////////////////////

#define STATUS_ON       1
#define STATUS_OFF      0

static uint8_t IsDeviceAdjusted(void);  /*!< 设备是否已校准 */
static void SetBasePhase(float phase);  /*!< 设置基准相角 */
static float GetBasePhase(void); /*!< 读取基准相角 */
static uint8_t IsTestChannel(AFE4300_POSITION channel); /*!< 判断当前通道是否为测量通道 */
static float Afe4300CalVoltageByIQ(float _I, float _Q); /*!< 根据I、Q值计算电压 */
static float Afe4300CalPhaseByIQ(float _I, float _Q);   /*!< 根据I、Q值计算相角 */
static void Afe4300SetIV_MUX(CHANNEL_CONFIG channelCfg);   /*!< 切换激励和测量通道 */
static void AFE4300_SetExtClk(uint16_t tim_period, uint16_t tim_prescaler); /*!< 设置AFE4300时钟频率 */
static int16_t Afe4300_ReadAdc(void);   /*!< 读取AFE4300 AD值 */
/**
  * @brief 计算采样结果
  */
static uint8_t IsAfeSampleOver(float curFreq, AFE4300_POSITION channelPosition, uint8_t pointIndex, AFE4300_SAMPLE_RESULT *sampleResult);
/**
  * @brief 测量指定频率下的单点阻抗和相角
  */
static AFE_STATE_ERROR_TYPE Afe4300SetTestPosition2(uint8_t pointIndex, float curFreq, AFE4300_POSITION channelPosition, LEASQ_COEFFICIENT coeffStruct);

void AFE4300_Set_Adjust_On(void);   /*!< 启动校准 */
AFE_STATE_ERROR_TYPE AFE4300_StartAdjust(void); /*!< AFE4300校准 */
void AFE4300_Set_Test_On(void);     /*!< 启动测量 */
AFE_STATE_ERROR_TYPE AFE4300_StartTest(void);   /*!< AFE4300测量 */

AFE4300_WORK_STATE GetAfe4300State(void);   /*!< 读取4300状态 */
void SetAfe4300State(AFE4300_WORK_STATE mode);  /*!< 设置4300状态 */
void  AFE4300_Stop(void);    /*!< 停止校准、测量 */
void AFE4300_Set_SingleTest_On(uint8_t point_id);   /*!< 启动单点测量 */
AFE_STATE_ERROR_TYPE AFE4300_SingleTest(void);  /*!< 单点测量 */
void AFE4300_PointInc(void); /*!< 调试功能-切换测量点 */  

#endif
