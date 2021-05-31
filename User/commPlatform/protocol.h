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

/* RCV_NOT : 通知
   RCV_CMD : 指令
   RCV_DAT : 数据
*/
typedef enum
{
	RCV_MSG_TABLE_START				= 0x00,		// 上位机->下位机列表开始
	
	RCV_NOT_MAIN_HEART_BEAT			= 0x01,		// 通知：pc心跳
	RCV_CMD_START_ADJUST		    = 0x02,		// 指令：启动校准
	RCV_CMD_START_TEST              = 0x03,		// 指令：启动测量
    RCV_CMD_GET_VERSION             = 0x04,     // 指令：查询下位机软件版本
    RCV_NOT_RESET                   = 0x05,     // 通知：系统复位
    RCV_CMD_SET_POINT_HIGH          = 0x06,     // 指令：拉高单独某个点
    RCV_CMD_SET_FREQ                = 0x07,     // 指令：设置频率
    RCV_CMD_SINGLE_POINT_TEST       = 0x10,     // 指令：单点测量
    RCV_CMD_SINGLE_FREQ_TEST        = 0x11,     // 指令：单频率测量
    RCV_CMD_STOP                    = 0x12,     // 指令：中止校准\测量过程
    RCV_CMD_WRITE_PARAM             = 0x13,     // 指令：写入系统参数
    RCV_CMD_READ_PARAM              = 0x14,     // 指令：读取系统参数
    RCV_CMD_WRITE_SN                = 0x15,     // 指令：写入设备号
    RCV_CMD_READ_SN                 = 0x16,     // 指令：读取设备号
    RCV_CMD_WRITE_SOUND             = 0x17,     // 指令：写入提示语音
	
	RCV_MSG_TABLE_END,							// 上位机->下位机列表结束
    
	SND_MSG_TABLE_START				= 0x40,		// 下位机->上位机列表开始
	
	SND_NOT_EXEC_HEART_BEAT			= 0x41,		// 通知：M3心跳
	SND_DAT_VERSION_INFO			= 0x42,		// 数据：发送软件版本信息
	SND_DAT_IQ_SAMPLE_AD           	= 0x43,		// 数据：发送实时ad采样,
	                                            //测量部位(1byte)+测量点id(1bytes)+频率(4bytes)+ad_I(2bytes)+ad_Q(2bytes)
    SND_DAT_IMPEDANCE_PHASE 		= 0x44,		// 数据：发送电阻抗、相角,
                                                //测量点id(1bytes) + 频率(4bytes) +电阻抗(4bytes) + ad_I(2bytes) + ad_Q(2bytes) + 相角(4bytes) 
    SND_DAT_ADJUST_COEFF            = 0x45,     // 数据：校准系数, 频率(4bytes)+m(4bytes)+o(4bytes)+R2(4bytes)
    SND_DAT_SN                      = 0x46,     // 数据，发送设备号
    SND_DAT_SYS_PARAM               = 0x47,     // 数据，发送系统参数
    SND_DAT_TEMP_HUMIDITY           = 0x48,     // 数据，发送温度和湿度
    
    SND_NOT_ADJUST_OVER		        = 0x50,		// 通知：校准完成
    SND_NOT_TEST_OVER       		= 0x51,		// 通知：测试完成
    SND_NOT_POWER_ON        		= 0x52,		// 通知：开机
    
    SND_NOT_LINEAR_REGRESSION_FAIL	= 0x60,		// 通知：校准线性度过低
    SND_NOT_DEVICE_NOT_ADJUSTED     = 0x61,     // 通知：设备未校准或者校准线性度过差
    SND_NOT_DEVICE_IS_BUSY          = 0x62,     // 通知：设备正忙...不响应校准测试指令
    
    SND_NOT_ABNORMAL                = 0x70,     // 通知，异常
    
	SND_MSG_TABLE_END,							// 下位机->上位机列表结束
    
    RCV_CMD_ERROR_INFO              = PLATFORM_ERROR_FUNC_ID,
}SERIAL_ID_PC;


/**
  * @brief 系统参数结构体，16字节
  */
typedef struct
{
    float Rref[3]; // 3个校准电阻的阻值，Ω
    uint16_t year;
    uint8_t month;
    uint8_t day;
} SysParam;

/**
  * @brief 测量点
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
  * @brief AFE4300状态
  */
typedef enum 
{
    AFE_STATE_INIT				= (u8)0x00,
    AFE_STATE_ADJUST			= (u8)0x01,//开机后默认进入电阻校准,计算不同f下的系数m、o
    AFE_STATE_TEST			    = (u8)0x02,//在人体测量时，只校准其中一个电阻的相角
    AFE_STATE_SINGLE_TEST,
} AFE4300_WORK_STATE;

typedef void (*Serial_Fun)(unsigned char data_len, unsigned char data[]);

extern ComCtrl CommToPc;

static void EnableUart1TxINT(u8 uartId); /*!< 使能串口1发送中断 */
    
void AnalyzeDataFromPC(void);   /*!< 解析PC端的数据 */

#endif
