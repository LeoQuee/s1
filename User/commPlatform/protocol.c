/*
 * @Description:  
 * @Author: Gaven
 * @Date: 2019-10-21 16:11:01
 * @LastEditTime: 2019-11-20 15:06:31
 * @LastEditors: Please set LastEditors
 */
 
#include "protocol.h"

/**
  * @brief 消息发送缓存，68*32=2176字节
  */
static Message TxMsgBuff[COM_TX_MSGBUFF_SIZE] = {0};
/**
  * @brief 消息接收缓存，68*32=2176字节
  */
static Message RxMsgBuff[COM_RX_MSGBUFF_SIZE] = {0};
/**
  * @brief 串口发送缓存，512字节
  */
static unsigned char TxComBuff[COM_TX_COMBUFF_SIZE] = {0};
/**
  * @brief 串口接收缓存，512字节
  */
static unsigned char RxComBuff[COM_RX_COMBUFF_SIZE] = {0};
/**
  * @brief 应答控制缓存，144*32=4608字节
  */
static ReplyCtrl     TxReplyFrameBuff[COM_TX_MSGBUFF_SIZE] = {0};
/**
  * @brief 转义后，140字节
  */
static AfterEscape   RevBackupFrame = {0,{0}}; 
/**
  * @brief 转义前，144字节
  */
static BeforeEscape  RevBeforeFrame = {0,0,{0}};
/**
  * @brief 错误控制缓存，4*6=24字节
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
  * @brief 串口函数指针，4*7=28字节
  */
Serial_Fun SerialFunHandler[RCV_MSG_TABLE_END - RCV_MSG_TABLE_START] = {NULL};

/**
  * @brief 使能串口1发送中断
  * @param uartId 串口号
  */
static void EnableUart1TxINT(u8 uartId)
{
	if(EUart1 == uartId)
	{
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
}

/**
  * @brief 通讯控制，88字节
  */
ComCtrl CommToPc = 
{
    // 硬件层控制
	{
        // 串口发送缓存控制
		{
			0,
			0,
			COM_TX_COMBUFF_SIZE,
			TxComBuff
		},
        // 串口接收缓存控制
		{
			0,
			0,
			COM_RX_COMBUFF_SIZE,
			RxComBuff
		},
        // 串口号
        EUart1, 
        // 预留
        {0},
        // 串口发送函数
        EnableUart1TxINT,
	},
    // 应用层控制
	{
        // 消息发送缓存控制
		{
			0,
			0,
			COM_TX_MSGBUFF_SIZE,
			TxMsgBuff
		},
        // 消息接收缓存控制
		{
			0,
			0,
			COM_RX_MSGBUFF_SIZE,
			RxMsgBuff	
		}
	},
    // 应答控制
	TxReplyFrameBuff,
    // 帧 转义前
    &RevBackupFrame,
    // 帧 转义后
    &RevBeforeFrame,
    ErrorRecordCtrl,
};


/**
  * @brief 解析PC端的数据
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
