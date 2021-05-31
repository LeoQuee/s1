/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-19 14:50:40
 * @LastEditTime: 2019-11-20 14:47:52
 * @LastEditors: Please set LastEditors
 */
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

#include "AppLayer.h"
#include "DataLinkLayer.h"
#include "stm32f10x.h"

/* RCV_NOT : ֪ͨ
   RCV_CMD : ָ��
   RCV_DAT : ����
*/
typedef enum
{
	RCV_MSG_TABLE_START				= 0x00,		// ��λ��->��λ���б�ʼ
	
	RCV_NOT_MAIN_HEART_BEAT			= 0x01,		// ֪ͨ��pc����
	RCV_CMD_START_ADJUST		    = 0x02,		// ָ�����У׼
	RCV_CMD_START_TEST              = 0x03,		// ָ���������
    RCV_CMD_GET_VERSION             = 0x04,     // ָ���ѯ��λ������汾
    RCV_NOT_RESET                   = 0x05,     // ֪ͨ��ϵͳ��λ
    RCV_CMD_SET_POINT_HIGH          = 0x06,     // ָ����ߵ���ĳ����
    RCV_CMD_SET_FREQ                = 0x07,     // ָ�����Ƶ��
    RCV_CMD_SINGLE_POINT_TEST       = 0x10,     // ָ��������
    RCV_CMD_SINGLE_FREQ_TEST        = 0x11,     // ָ���Ƶ�ʲ���
    RCV_CMD_STOP                    = 0x12,     // ָ���ֹУ׼\��������
    RCV_CMD_WRITE_PARAM             = 0x13,     // ָ�д��ϵͳ����
    RCV_CMD_READ_PARAM              = 0x14,     // ָ���ȡϵͳ����
    RCV_CMD_WRITE_SN                = 0x15,     // ָ�д���豸��
    RCV_CMD_READ_SN                 = 0x16,     // ָ���ȡ�豸��
    RCV_CMD_WRITE_SOUND             = 0x17,     // ָ�д����ʾ����
	
	RCV_MSG_TABLE_END,							// ��λ��->��λ���б����
    
	SND_MSG_TABLE_START				= 0x40,		// ��λ��->��λ���б�ʼ
	
	SND_NOT_EXEC_HEART_BEAT			= 0x41,		// ֪ͨ��M3����
	SND_DAT_VERSION_INFO			= 0x42,		// ���ݣ���������汾��Ϣ
	SND_DAT_IQ_SAMPLE_AD           	= 0x43,		// ���ݣ�����ʵʱad����,
	                                            //������λ(1byte)+������id(1bytes)+Ƶ��(4bytes)+ad_I(2bytes)+ad_Q(2bytes)
    SND_DAT_IMPEDANCE_PHASE 		= 0x44,		// ���ݣ����͵��迹�����,
                                                //������id(1bytes) + Ƶ��(4bytes) +���迹(4bytes) + ad_I(2bytes) + ad_Q(2bytes) + ���(4bytes) 
    SND_DAT_ADJUST_COEFF            = 0x45,     // ���ݣ�У׼ϵ��, Ƶ��(4bytes)+m(4bytes)+o(4bytes)+R2(4bytes)
    SND_DAT_SN                      = 0x46,     // ���ݣ������豸��
    SND_DAT_SYS_PARAM               = 0x47,     // ���ݣ�����ϵͳ����
    SND_DAT_TEMP_HUMIDITY           = 0x48,     // ���ݣ������¶Ⱥ�ʪ��
    
    SND_NOT_ADJUST_OVER		        = 0x50,		// ֪ͨ��У׼���
    SND_NOT_TEST_OVER       		= 0x51,		// ֪ͨ���������
    SND_NOT_POWER_ON        		= 0x52,		// ֪ͨ������
    
    SND_NOT_LINEAR_REGRESSION_FAIL	= 0x60,		// ֪ͨ��У׼���Զȹ���
    SND_NOT_DEVICE_NOT_ADJUSTED     = 0x61,     // ֪ͨ���豸δУ׼����У׼���Զȹ���
    SND_NOT_DEVICE_IS_BUSY          = 0x62,     // ֪ͨ���豸��æ...����ӦУ׼����ָ��
    
    SND_NOT_ABNORMAL                = 0x70,     // ֪ͨ���쳣
    
	SND_MSG_TABLE_END,							// ��λ��->��λ���б����
    
    RCV_CMD_ERROR_INFO              = PLATFORM_ERROR_FUNC_ID,
}SERIAL_ID_PC;


/**
  * @brief ϵͳ�����ṹ�壬16�ֽ�
  */
typedef struct
{
    float Rref[3]; // 3��У׼�������ֵ����
    uint16_t year;
    uint8_t month;
    uint8_t day;
} SysParam;

/**
  * @brief ������
  */
typedef enum
{
    POINT_LEFT_START,
    POINT_TP1           = POINT_LEFT_START,
    POINT_TP2,
    POINT_TP3,
    POINT_TP4,
    POINT_TP5,
    POINT_TP6,
    POINT_TP7,
    POINT_TP8,
    POINT_TP9,
    POINT_TP10,
    POINT_TP11,
    POINT_TP12,
    POINT_TP13,
    POINT_TP14,
    POINT_TP15,
    POINT_TP16,
    POINT_TP17,
    POINT_TP18,
    POINT_TP19,
    POINT_TP20,
    
    POINT_RIGHT_START,
    POINT_TP27          = POINT_RIGHT_START,
    POINT_TP28,
    POINT_TP29,
    POINT_TP30,
    POINT_TP31,
    POINT_TP32,
    POINT_TP33,
    POINT_TP34,
    POINT_TP35,
    POINT_TP36,
    POINT_TP37,
    POINT_TP38,
    POINT_TP39,
    POINT_TP40,
    POINT_TP41,
    POINT_TP42,
    POINT_TP43,
    POINT_TP44,
    POINT_TP45,
    POINT_TP46,

    POINT_END
}AFE4300_POINT;

/**
  * @brief AFE4300״̬
  */
typedef enum 
{
    AFE_STATE_INIT				= (u8)0x00,
    AFE_STATE_ADJUST			= (u8)0x01,//������Ĭ�Ͻ������У׼,���㲻ͬf�µ�ϵ��m��o
    AFE_STATE_TEST			    = (u8)0x02,//���������ʱ��ֻУ׼����һ����������
    AFE_STATE_SINGLE_TEST,
} AFE4300_WORK_STATE;

typedef void (*Serial_Fun)(unsigned char data_len, unsigned char data[]);

extern ComCtrl CommToPc;

static void EnableUart1TxINT(u8 uartId); /*!< ʹ�ܴ���1�����ж� */
    
void AnalyzeDataFromPC(void);   /*!< ����PC�˵����� */

#endif
