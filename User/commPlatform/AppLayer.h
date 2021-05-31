/*
 * @Description:  app layer buffer read and write interface
 * @Author: Gaven
 * @Date: 2019-10-21 16:11:01
 * @LastEditTime: 2019-10-24 16:59:53
 * @LastEditors: Gaven
 */
#ifndef _APPLICATION_LAYER_H_
#define _APPLICATION_LAYER_H_
 

#ifdef __cplusplus
extern "C" {
#endif

#include "PlatformDef.h"

/**
  * @brief read one Msg from the Msg buff
  */
unsigned char MsgBuffRead(MsgBuffCtrl* pMsgBuffCtrl,Message* pMessage);
/**
  * @brief write one Msg to the Msg buff
  */
unsigned char MsgBuffWrite(MsgBuffCtrl* pMsgBuffCtrl,Message* pMessage);
/**
  * @brief with the FuncID, Type, DataLen, Databuf, to pack these info into message
  */
void PackMessage(MsgBuffCtrl* PMsgBuffCtrl, unsigned char FuncID,
                 unsigned char Type, unsigned char DataLen, unsigned char Databuf[]);

#ifdef __cplusplus
}
#endif

#endif
