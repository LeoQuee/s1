/**
  ******************************************************************************
  * @file    protocol_task.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */

#ifndef _PROTOCOL_TASK_H_
#define _PROTOCOL_TASK_H_

/**
  * @brief 串口任务列表初始化
  */
void SerialTaskListInit(void);
/**
  * @brief 发送异常通知
  */
static void SendAbnormalInfo(char *str);
/**
  * @brief 读取设备号
  */
static void SF_Read_SN(unsigned char data_len, unsigned char data[]);
/**
  * @brief 写入设备号
  */
static void SF_Write_SN(unsigned char data_len, unsigned char data[]);
/**
  * @brief 读取系统参数
  */
static void SF_Read_Param(unsigned char data_len, unsigned char data[]);
/**
  * @brief 写入系统参数
  */
static void SF_Write_Param(unsigned char data_len, unsigned char data[]);
/**
  * @brief 停止测试/校准
  */
static void SF_Stop(unsigned char data_len, unsigned char data[]);
/**
  * @brief (待实现)单频率测量
  */
static void SF_Single_Freq_Test(unsigned char data_len, unsigned char data[]);
/**
  * @brief 单点测量
  */
static void SF_Single_Point_Test(unsigned char data_len, unsigned char data[]);
/**
  * @brief (待实现)设置频率
  */
static void SF_Set_Freq(unsigned char data_len, unsigned char data[]);
/**
  * @brief 测量点拉高处理函数
  */
static void SET_POINT_HIGH(unsigned char data_len, unsigned char data[]);
/**
  * @brief 系统复位处理函数
  */
static void RESET_Handler(unsigned char data_len, unsigned char data[]);
/**
  * @brief 获取软件版本号处理函数
  */
static void GET_SOFTWARE_VERSION_Handler(unsigned char data_len, unsigned char data[]);
/**
  * @brief 开始测试处理函数
  */
static void START_TEST_Handler(unsigned char data_len, unsigned char data[]);
/**
  * @brief 开始校准处理函数
  */
static void START_ADJUST_Handler(unsigned char data_len, unsigned char data[]);
/**
  * @brief 上位机心跳处理函数
  */
static void MAIN_HEART_BEAT_Handler(unsigned char data_len, unsigned char data[]);



#endif
