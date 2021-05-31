/*
 * @Description:  AFE4300 ��Ҫ����ʵ��ͷ�ļ�
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
  * @brief ����ͨ��
  */
typedef enum {
    RESISTANCE1 = 0,        //У׼����1
    RESISTANCE2 = 1,        //У׼����2
    RESISTANCE3 = 2,        //У׼����3
    BODY_POSITION_L1,      // Left 1
    BODY_POSITION_L2,      // Left 2
    BODY_POSITION_R1,      // Right 1
    BODY_POSITION_R2,      // Right 2
} AFE4300_POSITION;

/**
  * @brief ����ͨ�����ýṹ��
  */
typedef struct
{
    AFE4300_POSITION channelPosition;  /*!< ������λ */
    uint16_t isw_mux;        /*!< ����ͨ�� */
    uint16_t vsense_mux;     /*!< ����ͨ�� */
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
  * @brief 4300�������
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

static uint8_t IsDeviceAdjusted(void);  /*!< �豸�Ƿ���У׼ */
static void SetBasePhase(float phase);  /*!< ���û�׼��� */
static float GetBasePhase(void); /*!< ��ȡ��׼��� */
static uint8_t IsTestChannel(AFE4300_POSITION channel); /*!< �жϵ�ǰͨ���Ƿ�Ϊ����ͨ�� */
static float Afe4300CalVoltageByIQ(float _I, float _Q); /*!< ����I��Qֵ�����ѹ */
static float Afe4300CalPhaseByIQ(float _I, float _Q);   /*!< ����I��Qֵ������� */
static void Afe4300SetIV_MUX(CHANNEL_CONFIG channelCfg);   /*!< �л������Ͳ���ͨ�� */
static void AFE4300_SetExtClk(uint16_t tim_period, uint16_t tim_prescaler); /*!< ����AFE4300ʱ��Ƶ�� */
static int16_t Afe4300_ReadAdc(void);   /*!< ��ȡAFE4300 ADֵ */
/**
  * @brief ����������
  */
static uint8_t IsAfeSampleOver(float curFreq, AFE4300_POSITION channelPosition, uint8_t pointIndex, AFE4300_SAMPLE_RESULT *sampleResult);
/**
  * @brief ����ָ��Ƶ���µĵ����迹�����
  */
static AFE_STATE_ERROR_TYPE Afe4300SetTestPosition2(uint8_t pointIndex, float curFreq, AFE4300_POSITION channelPosition, LEASQ_COEFFICIENT coeffStruct);

void AFE4300_Set_Adjust_On(void);   /*!< ����У׼ */
AFE_STATE_ERROR_TYPE AFE4300_StartAdjust(void); /*!< AFE4300У׼ */
void AFE4300_Set_Test_On(void);     /*!< �������� */
AFE_STATE_ERROR_TYPE AFE4300_StartTest(void);   /*!< AFE4300���� */

AFE4300_WORK_STATE GetAfe4300State(void);   /*!< ��ȡ4300״̬ */
void SetAfe4300State(AFE4300_WORK_STATE mode);  /*!< ����4300״̬ */
void  AFE4300_Stop(void);    /*!< ֹͣУ׼������ */
void AFE4300_Set_SingleTest_On(uint8_t point_id);   /*!< ����������� */
AFE_STATE_ERROR_TYPE AFE4300_SingleTest(void);  /*!< ������� */
void AFE4300_PointInc(void); /*!< ���Թ���-�л������� */  

#endif
