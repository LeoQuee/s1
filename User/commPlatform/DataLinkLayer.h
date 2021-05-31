/*
 * @Description:  DataLinkLayer.h
 * @Author: Gaven
 * @Date: 2019-10-21 16:11:01
 * @LastEditTime: 2019-10-24 17:00:34
 * @LastEditors: Gaven
 */
#ifndef __LINK_LAYER_H_
#define __LINK_LAYER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PlatformDef.h"

/**
  * @brief generate Sequence ID
  */
static unsigned char SequenceIDMaker(void);

/**
  * @brief Calculate the check
  */
static __inline unsigned short GetRevCrc16(unsigned char * pData, int nLength,
                            unsigned short init,
                            const unsigned short *ptable);
/**
  * @brief Calculate the check
  */
static unsigned short CRCGetModbus16(unsigned char *pdata, int len);

/**
  * @brief Escape the frame data
  */
static unsigned int EscapeChar(unsigned char* pDest,
                        unsigned char* pSrc,
                        unsigned int iSrcLen);

/**
  * @brief revescape data into the frame
  */
static unsigned int RevEscapeChar(unsigned char* pDest,
                           unsigned char* pSrc,
                           unsigned int iSrcLen);

/**
  * @brief 将消息添加到应答列表
  */
static unsigned char PutMsgToReplayQueue(PComCtrl pPComCtrl,
                                  unsigned char seqID,
                                  unsigned char fucID,
                                  unsigned char len,
                                  unsigned char buff[]);

/**
  * @brief Send reply to each other
  */
static void SendReply(PComCtrl pComCtrl,
               unsigned char seqID,
               unsigned char funID);

/**
  * @brief Wipe out the reply queue member
  */
static unsigned char EraseReplayMachCtrl(PComCtrl pComCtrl,
                                  unsigned char seqID,
                                  unsigned char fucID);

/**
  * @brief judge the reply queue is full
  */
static unsigned char IsReplayMachNotFull(PComCtrl pComCtrl);

/**
  * @brief message into the frame
  */
static void MsgToFrame(StandardFrame* frame, Message* message);

/**
  * @brief reply timeout detection
  */
static __inline void ReplayMachTicksOverTime(PComCtrl pComCtrl);

/**
  * @brief Receive process
  */
static __inline void Receive(PComCtrl pComCtrl);

/**
  * @brief Send process 
  */
static __inline void Send(PComCtrl pComCtrl);

/**
  * @brief 异常处理
  */
static __inline void Exception(PComCtrl pComCtrl);

/**
  * @brief 串口数据处理过程
  */
void ComDataProcess(PComCtrl pcomctrl);

#ifdef __cplusplus
}
#endif

#endif
