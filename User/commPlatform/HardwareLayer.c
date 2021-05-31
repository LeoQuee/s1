#include "HardwareLayer.h"

/**
  * @brief judge serial port buff is enough to Send one frame data or not
  * @param pCommBuffCtrl serial port buff structure pointer
  * @return enough return TRUE,Otherwise return FALSE
  */
unsigned char IsCommEnoughForOneFrame(PComBuffCtrl pCommBuffCtrl)
{
	unsigned int len = 0;
	if(pCommBuffCtrl->m_iReadIndex <= pCommBuffCtrl->m_iWriteIndex)
	{
		len = pCommBuffCtrl->m_iLen-(pCommBuffCtrl->m_iWriteIndex-pCommBuffCtrl->m_iReadIndex)-1;
	}
	else
	{
		len = (pCommBuffCtrl->m_iReadIndex-pCommBuffCtrl->m_iWriteIndex)-1;
	}
	
    if(len >= AFTER_ESCAPE_DATA_BUFF_SIZE)
		return TRUE;
	else
		return FALSE;
}

/**
  * @brief read data from serial buff
  * @param pCommBuffCtrl serial buff structure pointer
  * @param pBuff data buffer 
  * @param Len data length
  * @return return the length of the read data
  */
unsigned int ReadDataFromCommBuff(PComBuffCtrl pCommBuffCtrl,unsigned char* pBuff,unsigned int Len)        
{
	unsigned int iDataLen = 0;	
	unsigned int iCanReadLen = 0;
	unsigned int index = 0;
    unsigned int iTempWriteIndex = pCommBuffCtrl->m_iWriteIndex;
	unsigned int iTempReadIndex = pCommBuffCtrl->m_iReadIndex;	
	unsigned int iLen = pCommBuffCtrl->m_iLen;
    
	if(iTempReadIndex <= iTempWriteIndex)
	{
		iDataLen = iTempWriteIndex-iTempReadIndex;
	}
	else
	{
		iDataLen = iLen-(iTempReadIndex-iTempWriteIndex);
	}

	if(Len <= iDataLen)
	{
		iCanReadLen = Len;
	}
	else
	{
		iCanReadLen = iDataLen;
	}	
	
	for(index=0;index<iCanReadLen;index++)
	{	
		pBuff[index] = pCommBuffCtrl->m_pBuff[iTempReadIndex];
		iTempReadIndex = (iTempReadIndex+1)%iLen;		
	}
    pCommBuffCtrl->m_iReadIndex = iTempReadIndex;
	 
	return iCanReadLen;
}

/**
  * @brief write data into serial buff
  * @param pHardware hardware structure pointer
  * @param pBuff data buffer 
  * @param Len data length
  * @return return the length of the written data
  */
unsigned int WriteDataIntoCommBuff(HardwareCtrl* pHardware, unsigned char* pBuff,unsigned int iDataLen)
{
    unsigned int index = 0;
    unsigned int iCanWritesize = 0;

	if(pHardware->m_TxComBuffCtrl.m_iReadIndex <= pHardware->m_TxComBuffCtrl.m_iWriteIndex)
	{
		iCanWritesize = pHardware->m_TxComBuffCtrl.m_iLen-(pHardware->m_TxComBuffCtrl.m_iWriteIndex-pHardware->m_TxComBuffCtrl.m_iReadIndex)-1;
	}
	else
	{
		iCanWritesize = (pHardware->m_TxComBuffCtrl.m_iReadIndex - pHardware->m_TxComBuffCtrl.m_iWriteIndex)-1;
	}
	
    if(iCanWritesize >= iDataLen)
	{
		iCanWritesize = iDataLen;
	}
    else
    {
        return 0;
    }

    for(index = 0;index < iCanWritesize;index++)
	{
		pHardware->m_TxComBuffCtrl.m_pBuff[pHardware->m_TxComBuffCtrl.m_iWriteIndex] = pBuff[index];

		pHardware->m_TxComBuffCtrl.m_iWriteIndex = (pHardware->m_TxComBuffCtrl.m_iWriteIndex+1) % pHardware->m_TxComBuffCtrl.m_iLen;
	}
    
    if(pHardware->psend)
    {
        pHardware->psend(pHardware->m_ucUartId);
    }

	return iCanWritesize;
}
