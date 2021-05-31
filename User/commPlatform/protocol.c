/*
 * @Description:  
 * @Author: Gaven
 * @Date: 2019-10-21 16:11:01
 * @LastEditTime: 2019-11-20 15:06:31
 * @LastEditors: Please set LastEditors
 */
 
#include "protocol.h"

/**
  * @brief ��Ϣ���ͻ��棬68*32=2176�ֽ�
  */
static Message TxMsgBuff[COM_TX_MSGBUFF_SIZE] = {0};
/**
  * @brief ��Ϣ���ջ��棬68*32=2176�ֽ�
  */
static Message RxMsgBuff[COM_RX_MSGBUFF_SIZE] = {0};
/**
  * @brief ���ڷ��ͻ��棬512�ֽ�
  */
static unsigned char TxComBuff[COM_TX_COMBUFF_SIZE] = {0};
/**
  * @brief ���ڽ��ջ��棬512�ֽ�
  */
static unsigned char RxComBuff[COM_RX_COMBUFF_SIZE] = {0};
/**
  * @brief Ӧ����ƻ��棬144*32=4608�ֽ�
  */
static ReplyCtrl     TxReplyFrameBuff[COM_TX_MSGBUFF_SIZE] = {0};
/**
  * @brief ת���140�ֽ�
  */
static AfterEscape   RevBackupFrame = {0,{0}}; 
/**
  * @brief ת��ǰ��144�ֽ�
  */
static BeforeEscape  RevBeforeFrame = {0,0,{0}};
/**
  * @brief ������ƻ��棬4*6=24�ֽ�
  */
static ErrorCtrl     ErrorRecordCtrl[6] = 
{
	{EErrorType_CheckError,         0},
	{EErrorType_LineNoise,          0},
	{EErrorType_MsgBufferFull,      0},	
    {EErrorType_BackupBufferFull,   0},
	{EErrorType_TxComBufferFull,    0},	
    0
};

/**
  * @brief ���ں���ָ�룬4*7=28�ֽ�
  */
Serial_Fun SerialFunHandler[RCV_MSG_TABLE_END - RCV_MSG_TABLE_START] = {NULL};

/**
  * @brief ʹ�ܴ���1�����ж�
  * @param uartId ���ں�
  */
static void EnableUart1TxINT(u8 uartId)
{
	if(EUart1 == uartId)
	{
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
}

/**
  * @brief ͨѶ���ƣ�88�ֽ�
  */
ComCtrl CommToPc = 
{
    // Ӳ�������
	{
        // ���ڷ��ͻ������
		{
			0,
			0,
			COM_TX_COMBUFF_SIZE,
			TxComBuff
		},
        // ���ڽ��ջ������
		{
			0,
			0,
			COM_RX_COMBUFF_SIZE,
			RxComBuff
		},
        // ���ں�
        EUart1, 
        // Ԥ��
        {0},
        // ���ڷ��ͺ���
        EnableUart1TxINT,
	},
    // Ӧ�ò����
	{
        // ��Ϣ���ͻ������
		{
			0,
			0,
			COM_TX_MSGBUFF_SIZE,
			TxMsgBuff
		},
        // ��Ϣ���ջ������
		{
			0,
			0,
			COM_RX_MSGBUFF_SIZE,
			RxMsgBuff	
		}
	},
    // Ӧ�����
	TxReplyFrameBuff,
    // ֡ ת��ǰ
    &RevBackupFrame,
    // ֡ ת���
    &RevBeforeFrame,
    ErrorRecordCtrl,
};


/**
  * @brief ����PC�˵�����
  */
void AnalyzeDataFromPC(void)
{
    Message message = {0};

	while(MsgBuffRead(&(CommToPc.m_AppCtrl.m_RxMsgBuffCtrl), &message))
	{
		if(RCV_MSG_TABLE_START < message.m_ucFuctionId && message.m_ucFuctionId < RCV_MSG_TABLE_END)
        {
            if(NULL != SerialFunHandler[message.m_ucFuctionId])
            {
                SerialFunHandler[message.m_ucFuctionId](message.m_ucLen, message.m_ucParaTable);
            }
        }
	}
}
