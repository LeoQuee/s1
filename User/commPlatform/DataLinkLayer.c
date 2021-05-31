#include "global_config.h"
#include "DataLinkLayer.h"
#include "HardwareLayer.h"
#include "AppLayer.h"

#include <string.h>

/**
  * @brief generate Sequence ID
  * @return Sequence ID
  */
static unsigned char SequenceIDMaker(void)                       
{
	static unsigned char squence = 0;
    
	squence++;
	if(0 == squence)
	{
        squence = 1;
    }
    
	return squence;
}

//逆序CRC计算

/**
  * @brief Calculate the check
  * @param pData 数据
  * @param nLength 数据长度
  * @param init 初始值
  * @param ptable CRC表
  * @return CRC校验值
  */
static __inline unsigned short GetRevCrc16(unsigned char * pData, int nLength,
                            unsigned short init,
                            const unsigned short *ptable)
{
    unsigned short cRc_16 = init;
    unsigned char temp;

    while(nLength-- > 0)
    {
        temp = cRc_16 & 0xFF;
        cRc_16 = (cRc_16 >> 8) ^ ptable[(temp ^ *pData++) & 0xFF];
    }

    return cRc_16;
}

/**
  * @brief Calculate the check
  * @param pdata 数据
  * @param len 数据长度
  * @return CRC校验值
  */
static unsigned short CRCGetModbus16(unsigned char *pdata, int len)
{
    //MODBUS CRC-16表 8005 逆序
    static const unsigned short g_McRctable_16[256] =
    {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
    };
    
    return GetRevCrc16(pdata, len, 0xFFFF, g_McRctable_16);
}

/**
  * @brief Escape the frame data 
  * @param pDest After escape data buff 
  * @param pSrc before escape data buff 
  * @param iSrcLen before escape data length
  * @return After escape data  length
  */
static unsigned int EscapeChar(unsigned char* pDest,
                        unsigned char* pSrc,
                        unsigned int iSrcLen)
{
	unsigned int indexSrc = 0,indexDest = 0;
    
	if(0 == iSrcLen)
	{
        return 0;
    }
    
    pDest[indexDest++] = FRAME_END;
	for(indexSrc=0; indexSrc<iSrcLen; indexSrc++)
	{
		if(FRAME_END == pSrc[indexSrc])
		{
			pDest[indexDest++] = TRANS_END_DB;
			pDest[indexDest++] = TRANS_END_DC;
		}
		else if(FRAME_DB == pSrc[indexSrc])
		{
			pDest[indexDest++] = TRANS_DB_DB;
			pDest[indexDest++] = TRANS_DB_DD;
		}
		else
		{
			pDest[indexDest++] = pSrc[indexSrc];
		}
	}

	pDest[indexDest++] = FRAME_END;

	return indexDest;
}

/**
  * @brief revescape data into the frame 
  * @param pDest before escape data buff 
  * @param pSrc After escape data buff 
  * @param iSrcLen After escape data length
  * @return before escape data  length
  */
static unsigned int RevEscapeChar(unsigned char* pDest,
                           unsigned char* pSrc,
                           unsigned int iSrcLen)
{
	unsigned int indexSrc = 0, indexDest = 0;
	unsigned char uclastdata = pSrc[indexSrc];
    
	for(indexSrc=0; indexSrc<iSrcLen; indexSrc++)
	{
		if(TRANS_END_DB == uclastdata)
		{
			if(TRANS_END_DC == pSrc[indexSrc])
			{
				pDest[indexDest++] = FRAME_END;
			}
			else if (TRANS_DB_DD == pSrc[indexSrc])
			{
				pDest[indexDest++] = FRAME_DB;
			}
		}
		else
		{
			if (TRANS_END_DB != pSrc[indexSrc])
			{
				pDest[indexDest++] = pSrc[indexSrc];
			}
		}

		 uclastdata = pSrc[indexSrc];
	}

	return indexDest;
}

/**
  * @brief 将消息添加到应答列表
  * @param pPComCtrl replay structure pointer 
  * @param seqID 流水号
  * @param fucID 功能ID
  * @param len 长度
  * @param buff 缓存
  * @return add success return TRUE,Otherwise return FALSE
  */
static unsigned char PutMsgToReplayQueue(PComCtrl pPComCtrl,
                                  unsigned char seqID,
                                  unsigned char fucID,
                                  unsigned char len,
                                  unsigned char buff[])
{
    unsigned int index;

    for(index = 0 ;index < pPComCtrl->m_AppCtrl.m_TxMsgBuffCtrl.m_iLen ;index++)
	{
        if(0 ==  pPComCtrl->m_ReplyQueue[index].m_ucSeqId)
		{
            pPComCtrl->m_ReplyQueue[index].m_iTicks = 0;
            pPComCtrl->m_ReplyQueue[index].m_iResendTime = RESENT_TIME;
            pPComCtrl->m_ReplyQueue[index].m_ucSeqId = seqID;
            pPComCtrl->m_ReplyQueue[index].m_ucFucId = fucID;
            pPComCtrl->m_ReplyQueue[index].m_iReplyDataLen = len;
            memcpy(pPComCtrl->m_ReplyQueue[index].m_iReplyDataBuf,buff,len);
            
			return TRUE;
		}
	}	
	
	return FALSE;
}

/**
  * @brief Send reply to each other
  * @param Pcomctrl Comm platform structure pointer
  * @param seqID 流水号
  * @param funID 功能ID
  */
static void SendReply(PComCtrl pComCtrl,
               unsigned char seqID,
               unsigned char funID)
{
    unsigned char buff[16] = {0};
    StandardFrame frame =  {0};
    unsigned int len = 0;
	unsigned short crc;
    
    frame.Head.m_ucLen = 7;
    frame.Head.m_ucVer = PROTOCOL_VER;
    frame.Head.m_ucFrameType = EProtocolType_Reply;
    frame.Head.m_ucSeqId = seqID;
    frame.Head.m_ucFucId = funID;

    crc = CRCGetModbus16((unsigned char*)&frame, 4);
    frame.FrameData[0] = crc;
    frame.FrameData[1] = crc>>8;

    len = EscapeChar(buff, (unsigned char*)&frame, 6);
    if(!WriteDataIntoCommBuff(&(pComCtrl->m_HardwareCtrl),buff,len))
    {
        //serial send buff full error
        pComCtrl->m_ErrorCtrl[EErrorType_TxComBufferFull].m_ucErrorCount++;
        pComCtrl->m_ErrorCtrl[EErrorType_TxComBufferFull].m_ucDetial[0] = funID;
        pComCtrl->m_ErrorCtrl[EErrorType_TxComBufferFull].m_ucDetial[1] = seqID;
	}
}

/**
  * @brief Wipe out the reply queue member
  * @param pComCtrl replay structure pointer
  * @param seqID 流水号
  * @param fucID 功能号
  * @return Wipe out success return TRUE,Otherwise return FALSE
  */
static unsigned char EraseReplayMachCtrl(PComCtrl pComCtrl,
                                  unsigned char seqID,
                                  unsigned char fucID)
{
    unsigned int index;
    for(index = 0 ;index < pComCtrl->m_AppCtrl.m_TxMsgBuffCtrl.m_iLen;index++)
	{
        if(seqID == pComCtrl->m_ReplyQueue[index].m_ucSeqId && fucID == pComCtrl->m_ReplyQueue[index].m_ucFucId)
		{		
            memset(&(pComCtrl->m_ReplyQueue[index]),0,sizeof(ReplyCtrl));
			return TRUE;
		}
	}
	
	return FALSE;
}

/**
  * @brief judge the reply queue is full
  * @param pComCtrl replay structure pointer
  * @return Not full return TRUE,Otherwise return FALSE
  */
static unsigned char IsReplayMachNotFull(PComCtrl pComCtrl)
{
    unsigned int index;
    for(index = 0 ;index < pComCtrl->m_AppCtrl.m_TxMsgBuffCtrl.m_iLen;index++)
	{
        if(0 == pComCtrl->m_ReplyQueue[index].m_ucSeqId)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

/**
  * @brief message into the frame 
  * @param frame frame structure pointer
  * @param message message structure pointer
  */
static void MsgToFrame(StandardFrame* frame, Message* message)
{
    unsigned int index = 0;
    unsigned short crc;
    
    frame->Head.m_ucLen = message->m_ucLen + 7;
    frame->Head.m_ucVer = PROTOCOL_VER;
    frame->Head.m_ucFrameType = message->m_ucType;
    frame->Head.m_ucSeqId = SequenceIDMaker();
    frame->Head.m_ucFucId = message->m_ucFuctionId;
    for(;index < message->m_ucLen;index++)
    {
        frame->FrameData[index] = message->m_ucParaTable[index];
    }
    crc = CRCGetModbus16((unsigned char*)frame,frame->Head.m_ucLen-3);
    frame->FrameData[index] = crc;
    frame->FrameData[index+1] = crc>>8;
}

/**
  * @brief reply timeout detection  
  * @param pComCtrl Comm platform structure pointer
  */
static __inline void ReplayMachTicksOverTime(PComCtrl pComCtrl)
{
    unsigned int index;
    
    for(index = 0 ;index < pComCtrl->m_AppCtrl.m_TxMsgBuffCtrl.m_iLen;index++)
	{
        if(0 != pComCtrl->m_ReplyQueue[index].m_ucSeqId)
		{
            pComCtrl->m_ReplyQueue[index].m_iTicks++;
			
            if(pComCtrl->m_ReplyQueue[index].m_iTicks >= RESEND_WAIT_TIME)
			{
                pComCtrl->m_ReplyQueue[index].m_iTicks = 0;
                pComCtrl->m_ReplyQueue[index].m_iResendTime--;
                if(!WriteDataIntoCommBuff(&(pComCtrl->m_HardwareCtrl),
                                          pComCtrl->m_ReplyQueue[index].m_iReplyDataBuf,
                                          pComCtrl->m_ReplyQueue[index].m_iReplyDataLen))
                {
                    //serial send buff full error
                    pComCtrl->m_ErrorCtrl[EErrorType_TxComBufferFull].m_ucErrorCount++;
                    pComCtrl->m_ErrorCtrl[EErrorType_TxComBufferFull].m_ucDetial[0] = pComCtrl->m_ReplyQueue[index].m_ucFucId;
                    pComCtrl->m_ErrorCtrl[EErrorType_TxComBufferFull].m_ucDetial[1] = pComCtrl->m_ReplyQueue[index].m_ucSeqId;
                    break;
                }
                if(pComCtrl->m_ReplyQueue[index].m_iResendTime <= 0)
                {
                    pComCtrl->m_ErrorCtrl[EErrorType_Resend].m_ucErrorCount++;
                    pComCtrl->m_ErrorCtrl[EErrorType_Resend].m_ucDetial[0] = pComCtrl->m_ReplyQueue[index].m_ucFucId;
                    pComCtrl->m_ErrorCtrl[EErrorType_Resend].m_ucDetial[1] = pComCtrl->m_ReplyQueue[index].m_ucSeqId;
                    memset(&(pComCtrl->m_ReplyQueue[index]),0,sizeof(ReplyCtrl));
                }
			}
		}
	}	
}

/**
  * @brief Receive process 
  * @param pComCtrl Comm platform structure pointer
  */
static __inline void Receive(PComCtrl pComCtrl)
{
    unsigned int iAfterRevEscapeLen;

    if(pComCtrl->m_RevBeforeFrame->m_iPraseIndex == pComCtrl->m_RevBeforeFrame->m_iLen)
    {
        pComCtrl->m_RevBeforeFrame->m_iLen = \
                ReadDataFromCommBuff(&(pComCtrl->m_HardwareCtrl.m_RxComBuffCtrl),
                                     pComCtrl->m_RevBeforeFrame->m_ucDataTable,
                                     sizeof(pComCtrl->m_RevBeforeFrame->m_ucDataTable));
        pComCtrl->m_RevBeforeFrame->m_iPraseIndex = 0;
    }

    for(;pComCtrl->m_RevBeforeFrame->m_iPraseIndex < pComCtrl->m_RevBeforeFrame->m_iLen;pComCtrl->m_RevBeforeFrame->m_iPraseIndex++)
    {
        if (FRAME_END == pComCtrl->m_RevBeforeFrame->m_ucDataTable[pComCtrl->m_RevBeforeFrame->m_iPraseIndex])
        {
            if(pComCtrl->m_RevBackupFrame->m_iLen == 0)
            {
                continue;
            }

            iAfterRevEscapeLen =\
                    RevEscapeChar(pComCtrl->m_RevBackupFrame->m_ucDataTable,
                                  pComCtrl->m_RevBackupFrame->m_ucDataTable,
                                  pComCtrl->m_RevBackupFrame->m_iLen);

            if(iAfterRevEscapeLen < 6)
            {
                //Line noise error
                pComCtrl->m_ErrorCtrl[EErrorType_LineNoise].m_ucErrorCount++;
            }
            else if(pComCtrl->m_RevBackupFrame->m_ucDataTable[0] == iAfterRevEscapeLen + 1)
            {
                {//frame
                    StandardFrame frame = {0};
                    unsigned short  crc;
                    memcpy((unsigned char *)&frame,pComCtrl->m_RevBackupFrame->m_ucDataTable,iAfterRevEscapeLen);
                    crc = (frame.FrameData[iAfterRevEscapeLen - 5] << 8)|frame.FrameData[iAfterRevEscapeLen - 6];
                    if(crc == CRCGetModbus16((unsigned char*)&frame,iAfterRevEscapeLen-2))
                    {
                        if(EProtocolType_Reply == frame.Head.m_ucFrameType)
                        {
                            EraseReplayMachCtrl(pComCtrl,frame.Head.m_ucSeqId,frame.Head.m_ucFucId);
                        }
                        else
                        {
                            if(EProtocolType_Notice != frame.Head.m_ucFrameType)
                            {
                                SendReply(pComCtrl,frame.Head.m_ucSeqId,frame.Head.m_ucFucId);
                            }

                            {//message
                                Message message = {0,0,0,{0}};
                                message.m_ucLen = frame.Head.m_ucLen - 7;
                                message.m_ucType = frame.Head.m_ucFrameType;
                                message.m_ucFuctionId = frame.Head.m_ucFucId;
                                memcpy(message.m_ucParaTable,frame.FrameData,frame.Head.m_ucLen - 7);

                                if(!MsgBuffWrite(&(pComCtrl->m_AppCtrl.m_RxMsgBuffCtrl), &message))
                                {
                                    //Message buffer full error
                                    pComCtrl->m_RevBackupFrame->m_iLen = 0;
                                    pComCtrl->m_ErrorCtrl[EErrorType_MsgBufferFull].m_ucErrorCount++;
                                    pComCtrl->m_ErrorCtrl[EErrorType_MsgBufferFull].m_ucDetial[0] = frame.Head.m_ucFucId;
                                    pComCtrl->m_ErrorCtrl[EErrorType_MsgBufferFull].m_ucDetial[1] = frame.Head.m_ucSeqId;
                                    break;
                                }
                            }//end message
                        }
                    }
                    else
                    {
                        //The checksum error
                        pComCtrl->m_ErrorCtrl[EErrorType_CheckError].m_ucErrorCount++;
                    }
                }// end frame
            }

            pComCtrl->m_RevBackupFrame->m_iLen = 0;
        }
        else
        {
            pComCtrl->m_RevBackupFrame->m_ucDataTable[pComCtrl->m_RevBackupFrame->m_iLen++] \
                    = pComCtrl->m_RevBeforeFrame->m_ucDataTable[pComCtrl->m_RevBeforeFrame->m_iPraseIndex];
            if (pComCtrl->m_RevBackupFrame->m_iLen >= sizeof(pComCtrl->m_RevBackupFrame->m_ucDataTable))
            {
                //Frame backup buff full error
                pComCtrl->m_RevBackupFrame->m_iLen = 0;
                pComCtrl->m_ErrorCtrl[EErrorType_BackupBufferFull].m_ucErrorCount++;
            }
        }
    }

    ReplayMachTicksOverTime(pComCtrl);
}

/**
  * @brief Send process 
  * @param pComCtrl Comm platform structure pointer
  */
static __inline void Send(PComCtrl pComCtrl)
{
    unsigned int len;

    while(IsCommEnoughForOneFrame(&(pComCtrl->m_HardwareCtrl.m_TxComBuffCtrl)))
	{
        if(IsReplayMachNotFull(pComCtrl))
		{
            {//frame,message
                StandardFrame frame = {0};
                Message message = {0,0,0,{0}};
                if(MsgBuffRead(&(pComCtrl->m_AppCtrl.m_TxMsgBuffCtrl),&message))
                {
                    MsgToFrame(&frame,&message);

                    {//table
                        unsigned char table[AFTER_ESCAPE_DATA_BUFF_SIZE] = {0};
                        len = EscapeChar(table,(unsigned char*)&frame,frame.Head.m_ucLen - 1);

                        if(!WriteDataIntoCommBuff(&(pComCtrl->m_HardwareCtrl),table,len))
                        {
                            //serial send buff full error
                            pComCtrl->m_ErrorCtrl[EErrorType_TxComBufferFull].m_ucErrorCount++;
                            pComCtrl->m_ErrorCtrl[EErrorType_TxComBufferFull].m_ucDetial[0] = frame.Head.m_ucFucId;
                            pComCtrl->m_ErrorCtrl[EErrorType_TxComBufferFull].m_ucDetial[1] = frame.Head.m_ucSeqId;

                            MsgBuffWrite(&(pComCtrl->m_AppCtrl.m_TxMsgBuffCtrl),&message);
                            break;
                        }

                        if(EProtocolType_Notice != frame.Head.m_ucFrameType)
                        {
                            //put message into the reply queue
                            PutMsgToReplayQueue(pComCtrl,
                                                frame.Head.m_ucSeqId,
                                                frame.Head.m_ucFucId,
                                                len,
                                                table);
                        }
                    }//table end
                }
                else
                {
                    break;
                }
            }//frame,message  end
        }
        else
        {
            break;
        }
	}
}

/**
  * @brief 异常处理
  * @param pComCtrl 
  */
static __inline void Exception(PComCtrl pComCtrl)
{
    Message message = {0,0,0,{0}};
    unsigned char ucErrorIndex = EErrorType_CheckError;

    message.m_ucLen = 4;
    message.m_ucFuctionId = PLATFORM_ERROR_FUNC_ID;
    message.m_ucType = EProtocolType_Cmd;

    for( ;ucErrorIndex < EErrorType_Total;ucErrorIndex++)
    {
        if(pComCtrl->m_ErrorCtrl[ucErrorIndex].m_ucErrorCount > 0)
        {
            pComCtrl->m_ErrorCtrl[ucErrorIndex].m_ucErrorCount = 0;
            message.m_ucParaTable[0] = ucErrorIndex;
            message.m_ucParaTable[1] = pComCtrl->m_ErrorCtrl[ucErrorIndex].m_ucDetial[0];
            message.m_ucParaTable[2] = pComCtrl->m_ErrorCtrl[ucErrorIndex].m_ucDetial[1];
            message.m_ucParaTable[3] = pComCtrl->m_ErrorCtrl[ucErrorIndex].m_ucDetial[2];

            if(!MsgBuffWrite(&(pComCtrl->m_AppCtrl.m_RxMsgBuffCtrl), &message)) // 向消息接收缓存发送，通知本设备进行处理
            {
                pComCtrl->m_ErrorCtrl[EErrorType_MsgBufferFull].m_ucErrorCount++;
            }
        }
    }
}

/**
  * @brief 串口数据处理过程
  */
void ComDataProcess(PComCtrl pComCtrl)
{
#ifndef _USART_PRINT_DEBUG_
    Receive(pComCtrl);
    Send(pComCtrl);
    Exception(pComCtrl);
#endif
}
