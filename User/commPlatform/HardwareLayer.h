/*
 * @Description:   hardware communication buffer read and write interface for linker layer
 *                  1�����շ����ж��ھ�����Ҫ�������º�����
 *                      ֱ��ʹ��HardwareCtrl�ṹ�壬���ٿ���
 * @Author: Gaven
 * @Date: 2019-10-21 16:11:01
 * @LastEditTime: 2019-10-24 17:01:47
 * @LastEditors: Gaven
 */
#ifndef __HARDWARE_LAYER_H__
#define __HARDWARE_LAYER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "PlatformDef.h"

/**
  * @brief judge serial port buff is enough to Send one frame data or not
  */
unsigned char IsCommEnoughForOneFrame(PComBuffCtrl pCommBuffCtrl);

/**
  * @brief write data into serial buff
  */
unsigned int WriteDataIntoCommBuff(HardwareCtrl* pHardware,unsigned char* pBuff,unsigned int iDataLen);

/**
  * @brief read data from serial buff
  */
unsigned int ReadDataFromCommBuff(PComBuffCtrl pCommBuffCtrl,unsigned char* pBuff,unsigned int Len);

#ifdef __cplusplus
}
#endif

#endif
