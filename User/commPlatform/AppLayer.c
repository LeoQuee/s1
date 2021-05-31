#include "AppLayer.h"

/**
  * @brief  read one Msg from the Msg buff
  * @param pMsgBuffCtrl Msg buff structure pointer
  * @param Message Msg structure pointer
  * @return Success read return TRUE,Otherwise return FALSE
  */
unsigned char MsgBuffRead(MsgBuffCtrl* pMsgBuffCtrl, Message* pMessage)
{
	if(pMsgBuffCtrl->m_iWriteIndex == pMsgBuffCtrl->m_iReadIndex)
	{
		return FALSE;
	}
	else
	{
		*pMessage = pMsgBuffCtrl->m_pMsgBuff[pMsgBuffCtrl->m_iReadIndex]; // 结构体赋值
		pMsgBuffCtrl->m_iReadIndex = (pMsgBuffCtrl->m_iReadIndex+1) % pMsgBuffCtrl->m_iLen;

		return TRUE;
	}	
}

/**
  * @brief write one Msg to the Msg buff
  * @param pMsgBuffCtrl Msg buff structure pointer
  * @param Message Msg structure pointer
  * @return Success write return TRUE,Otherwise return FALSE
  */
unsigned char MsgBuffWrite(MsgBuffCtrl* pMsgBuffCtrl, Message* pMessage)
{
	if((pMsgBuffCtrl->m_iWriteIndex+1)%pMsgBuffCtrl->m_iLen == pMsgBuffCtrl->m_iReadIndex)
	{
		return FALSE;
	}
	else
	{   
		pMsgBuffCtrl->m_pMsgBuff[pMsgBuffCtrl->m_iWriteIndex] = *pMessage; // 结构体赋值
		pMsgBuffCtrl->m_iWriteIndex = (pMsgBuffCtrl->m_iWriteIndex+1)%pMsgBuffCtrl->m_iLen;

		return TRUE;
	}
}


/**
  * @brief with the FuncID, Type, DataLen, Databuf, to pack these info into message
  * @param PMsgBuffCtrl Msg structure pointer
  * @param FuncID 功能id
  * @param Type 类型
  * @param Datalen 数据长度
  * @param Databuf 数据
  */
void PackMessage(MsgBuffCtrl* PMsgBuffCtrl, unsigned char FuncID, unsigned char Type, unsigned char DataLen, unsigned char Databuf[])
{
    Message message = {0,0,0,{0}};
    unsigned int index = 0;

    message.m_ucLen = DataLen;
    message.m_ucFuctionId = FuncID;
    message.m_ucType = Type;

    for(;index < DataLen;index++)
    {
        message.m_ucParaTable[index] = Databuf[index];
    }

    MsgBuffWrite(PMsgBuffCtrl,&message);
}
