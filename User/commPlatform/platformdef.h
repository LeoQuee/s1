/*
 * @Description:  
 * @Author: Gaven
 * @Date: 2019-10-21 16:11:01
 * @LastEditTime: 2019-10-24 17:02:03
 * @LastEditors: Gaven
 */
#ifndef __PLATFORMDEF_H__
#define __PLATFORMDEF_H__

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(4)

#if defined(WIN32)||defined(QT)||defined(__ARMCC_VERSION)
#define UCHAR_BIT unsigned char
#else
#define UCHAR_BIT unsigned
#endif

#ifndef NULL
#define NULL 0
#endif

#define PROTOCOL_VER    0x01    /*!< The version number */

/*******************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!用户可以根据实际情况修改以下缓冲区长度!!!!!!!!!!!!!!
*******************************************************************************/
#define  COM_TX_COMBUFF_SIZE (512)           //serial port send buffer size
#define  COM_RX_COMBUFF_SIZE (512)           //serial port receives buffer size
#define  COM_TX_MSGBUFF_SIZE (32)            //msg send buffer size
#define  COM_RX_MSGBUFF_SIZE (32)            //msg receives buffer size

/*******************************************************************************
@用户若要改变一个Message的数据长度，只要更改 MESSAGE_DATA_BUFF_SIZE 宏
*******************************************************************************/
#define MESSAGE_DATA_BUFF_SIZE (132)   //消息 数据缓冲区长度 （暂时不要超过255）

/*******************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!以下定义不能修改!!!!!!!!!!!!!!!!!!!!!!!!!!
*******************************************************************************/
#define FRAME_DATA_BUFF_SIZE            (MESSAGE_DATA_BUFF_SIZE+4)  //帧 数据缓冲区长度
#define BEFORE_ESCAPE_DATA_BUFF_SIZE    (FRAME_DATA_BUFF_SIZE*2)    //转义前 数据缓冲区长度（本缓冲区可大可小）
#define AFTER_ESCAPE_DATA_BUFF_SIZE     (FRAME_DATA_BUFF_SIZE*2)    //转义后 数据缓冲区长度
#define REPLY_CTRL_DATA_BUFF_SIZE       (FRAME_DATA_BUFF_SIZE*2)    //应答 数据缓冲区长度

/*******************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!以下宏在数据链路层使用!!!!!!!!!!!!!!!!!!!!
*******************************************************************************/
#define FRAME_END       (0xC0)
#define FRAME_DB        (0xDB)

#define TRANS_END_DB    (FRAME_DB)
#define TRANS_END_DC    (0XDC)

#define TRANS_DB_DB     (FRAME_DB)
#define TRANS_DB_DD     (0XDD)

#define RESENT_TIME      (3)       //The resend times
#define HANDLE_TIME      (5)       //The cycle time(ms)
//The resend Wait time
#define RESEND_WAIT_TIME            (50*HANDLE_TIME)     /*!< 重发间隔，250ms */

#define PLATFORM_ERROR_FUNC_ID      (0xF0)

/**
  * @brief 消息（帧）类型
  */
typedef enum
{
    EProtocolType_Cmd,      /*!< 指令：要求通信方发送应答 */
    EProtocolType_Data,     /*!< 数据：要求通信方发送应答，效果和指令一样 */
    EProtocolType_Reply,    /*!< 应答：内部使用（用户不能使用） */
    EProtocolType_Notice,   /*!< 通知：不要求通信方发送应答 */
}EProtocolType;

/**
  * @brief 错误类型
  */
typedef enum
{
    EErrorType_CheckError         , /*!< 校验错误 The check error */
    EErrorType_LineNoise        ,   /*!< 线路噪声 Line noise error */
    EErrorType_MsgBufferFull     ,  /*!< 消息缓冲区满 Message buffer full error */
    EErrorType_BackupBufferFull  ,  /*!< 帧缓冲满 Frame backup buff full error */
    EErrorType_TxComBufferFull  ,   /*!< 发送缓冲区满 serial send buff full error */
    EErrorType_Resend,              /*!< 重发失败 Cmd and data resend three times error */
    EErrorType_Total,
}EErrorType;

/**
  * @brief 串口号
  */
typedef enum
{
    EUart0,
    EUart1      = 0x00,
    EUart2,
    EUart3,
    EUart4,
    EUart5,
    EUart6,
}EUart;

/**
  * @brief 数据流层接收发送缓冲区，16字节
  */
typedef struct tagComBuffCtrl
{
    unsigned int m_iWriteIndex;
    unsigned int m_iReadIndex;
    const unsigned int m_iLen;
    unsigned char* m_pBuff;
}ComBuffCtrl,*PComBuffCtrl;

/**
  * @brief 硬件层的收发缓冲区结构，40字节
  */
typedef struct tagHardwareComCtrl
{
    ComBuffCtrl m_TxComBuffCtrl;            /*!< 串口发送缓存控制 */
    ComBuffCtrl m_RxComBuffCtrl;            /*!< 串口接收缓存控制 */
    const unsigned char m_ucUartId;         /*!< 串口号 */
    unsigned char m_ucReserve[3];           /*!< 预留 */
    void (*psend)(unsigned char UartId);    /*!< 串口发送函数指针,可以注册函数，一般用于使能发送 */
}HardwareCtrl;

/**
  * @brief 帧头，4字节----内部使用
  */
typedef struct tagFrameHead
{
    unsigned char m_ucLen;      /*!< 帧长 */
    UCHAR_BIT m_ucVer:4;        /*!< 版本号 The low 4 bit */
    UCHAR_BIT m_ucFrameType:4;  /*!< 帧类型 高4位 */
    unsigned char m_ucSeqId;    /*!< 流水号 The sequence ID */
    unsigned char m_ucFucId;    /*!< 功能号 The function ID */
}FrameHead;

/**
  * @brief 标准帧，72字节-----内部使用
  */
typedef struct tagStandardFrame
{
    FrameHead Head; /*!< 帧头 */
    unsigned char FrameData[FRAME_DATA_BUFF_SIZE];  /*!< 帧缓冲区 Include the checksum */
}StandardFrame;

/**
  * @brief 帧转义后缓冲区，140字节-----内部使用
  */
typedef struct tagAfterEscape
{
    unsigned int  m_iLen;   /*!< 长度 */
    unsigned char m_ucDataTable[AFTER_ESCAPE_DATA_BUFF_SIZE];   /*!< 缓冲区 */
}AfterEscape;

/** 
  * @brief 帧转义前缓冲区,144字节-----内部使用
  */
typedef struct tagBeforeEscape
{
    unsigned int    m_iPraseIndex;                                  /*!< 上次解析的位置 Record the position of the current analytical */
    unsigned int    m_iLen;                                         /*!< 缓冲区长度 The length of the data before escape */
    unsigned char   m_ucDataTable[BEFORE_ESCAPE_DATA_BUFF_SIZE];    /*!< 缓冲区 the data before escape */
}BeforeEscape;

/**
  * @brief 应答控制，144字节-----内部使用
  */
typedef struct tagReplyCtrl
{
    unsigned  m_iResendTime:4;                               /*!< 剩余重发次数 The remaining resend times */
    unsigned  m_iTicks:12;                                   /*!< 超时时间 the timeout ticks */
    unsigned char m_ucSeqId;                                 /*!< 流水号 The sequence ID */
    unsigned char m_ucFucId;                                 /*!< 功能号 The function ID */
    unsigned int  m_iReplyDataLen;                           /*!< 应答长度 The length of the data after escape */
    unsigned char m_iReplyDataBuf[REPLY_CTRL_DATA_BUFF_SIZE];/*!< 应答缓冲区 the data after escape */
}ReplyCtrl;

/**
  * @brief 错误记录，4字节-----内部使用
  */
typedef struct tagErrorCtrl
{
    unsigned char m_ucErrorCount;   /*!< 错误发生次数 error count */
    unsigned char m_ucDetial[3];    /*!< 错误类型 func id,seq id */
}ErrorCtrl;

/**
  * @brief 消息结构体，68字节
  */
typedef struct tagMessage
{
    unsigned char m_ucLen;  /*!< 消息的数据长度 the data length of the m_ucParaTable */
    unsigned char m_ucType; /*!< 消息类型 */
    unsigned char m_ucFuctionId; /*!< 功能号 */
    unsigned char m_ucParaTable[MESSAGE_DATA_BUFF_SIZE+1]; /*!< 数据缓冲区 */
}Message;

/**
  * @brief 消息缓冲区，16字节
  */
typedef struct  tagMsgBuffCtrl
{
    unsigned int m_iWriteIndex; /*!< 写指针 */
    unsigned int m_iReadIndex;  /*!< 读指针 */
    const unsigned int m_iLen;  /*!< 缓冲区大小 */
    Message* m_pMsgBuff;        /*!< 消息缓冲区 */
}MsgBuffCtrl;

/**
  * @brief 应用层消息缓冲区，32字节
  */
typedef struct tagApplicationCtrl
{
    MsgBuffCtrl m_TxMsgBuffCtrl;    /*!< Message send buffer */
    MsgBuffCtrl m_RxMsgBuffCtrl;    /*!< Message receive buffer */
}AppCtrl;

/**
  * @brief 通信控制结构，88字节
  */
typedef struct tagComCtrl
{
    HardwareCtrl   m_HardwareCtrl;      /*!< 硬件层功能结构 */
    AppCtrl        m_AppCtrl;           /*!< 应用层控制结构 */
    ReplyCtrl*     m_ReplyQueue;        /*!< 应答控制结构(请确保与消息发送缓冲区大小一致) */
    AfterEscape*   m_RevBackupFrame;    /*!< 转义后缓冲区控制 */
    BeforeEscape*  m_RevBeforeFrame;    /*!< 转义前缓冲区控制 */
    ErrorCtrl*     m_ErrorCtrl;         /*!< 错误记录 */
}ComCtrl,*PComCtrl;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
