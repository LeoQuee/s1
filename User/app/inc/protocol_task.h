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
  * @brief ���������б��ʼ��
  */
void SerialTaskListInit(void);
/**
  * @brief �����쳣֪ͨ
  */
static void SendAbnormalInfo(char *str);
/**
  * @brief ��ȡ�豸��
  */
static void SF_Read_SN(unsigned char data_len, unsigned char data[]);
/**
  * @brief д���豸��
  */
static void SF_Write_SN(unsigned char data_len, unsigned char data[]);
/**
  * @brief ��ȡϵͳ����
  */
static void SF_Read_Param(unsigned char data_len, unsigned char data[]);
/**
  * @brief д��ϵͳ����
  */
static void SF_Write_Param(unsigned char data_len, unsigned char data[]);
/**
  * @brief ֹͣ����/У׼
  */
static void SF_Stop(unsigned char data_len, unsigned char data[]);
/**
  * @brief (��ʵ��)��Ƶ�ʲ���
  */
static void SF_Single_Freq_Test(unsigned char data_len, unsigned char data[]);
/**
  * @brief �������
  */
static void SF_Single_Point_Test(unsigned char data_len, unsigned char data[]);
/**
  * @brief (��ʵ��)����Ƶ��
  */
static void SF_Set_Freq(unsigned char data_len, unsigned char data[]);
/**
  * @brief ���������ߴ�����
  */
static void SET_POINT_HIGH(unsigned char data_len, unsigned char data[]);
/**
  * @brief ϵͳ��λ������
  */
static void RESET_Handler(unsigned char data_len, unsigned char data[]);
/**
  * @brief ��ȡ����汾�Ŵ�����
  */
static void GET_SOFTWARE_VERSION_Handler(unsigned char data_len, unsigned char data[]);
/**
  * @brief ��ʼ���Դ�����
  */
static void START_TEST_Handler(unsigned char data_len, unsigned char data[]);
/**
  * @brief ��ʼУ׼������
  */
static void START_ADJUST_Handler(unsigned char data_len, unsigned char data[]);
/**
  * @brief ��λ������������
  */
static void MAIN_HEART_BEAT_Handler(unsigned char data_len, unsigned char data[]);



#endif
