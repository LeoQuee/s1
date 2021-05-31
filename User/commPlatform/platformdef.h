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
!!!!!!!!!!!!!!!!!!!!!!!!!!!!�û����Ը���ʵ������޸����»���������!!!!!!!!!!!!!!
*******************************************************************************/
#define  COM_TX_COMBUFF_SIZE (512)           //serial port send buffer size
#define  COM_RX_COMBUFF_SIZE (512)           //serial port receives buffer size
#define  COM_TX_MSGBUFF_SIZE (32)            //msg send buffer size
#define  COM_RX_MSGBUFF_SIZE (32)            //msg receives buffer size

/*******************************************************************************
@�û���Ҫ�ı�һ��Message�����ݳ��ȣ�ֻҪ���� MESSAGE_DATA_BUFF_SIZE ��
*******************************************************************************/
#define MESSAGE_DATA_BUFF_SIZE (132)   //��Ϣ ���ݻ��������� ����ʱ��Ҫ����255��

/*******************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���¶��岻���޸�!!!!!!!!!!!!!!!!!!!!!!!!!!
*******************************************************************************/
#define FRAME_DATA_BUFF_SIZE            (MESSAGE_DATA_BUFF_SIZE+4)  //֡ ���ݻ���������
#define BEFORE_ESCAPE_DATA_BUFF_SIZE    (FRAME_DATA_BUFF_SIZE*2)    //ת��ǰ ���ݻ��������ȣ����������ɴ��С��
#define AFTER_ESCAPE_DATA_BUFF_SIZE     (FRAME_DATA_BUFF_SIZE*2)    //ת��� ���ݻ���������
#define REPLY_CTRL_DATA_BUFF_SIZE       (FRAME_DATA_BUFF_SIZE*2)    //Ӧ�� ���ݻ���������

/*******************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���º���������·��ʹ��!!!!!!!!!!!!!!!!!!!!
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
#define RESEND_WAIT_TIME            (50*HANDLE_TIME)     /*!< �ط������250ms */

#define PLATFORM_ERROR_FUNC_ID      (0xF0)

/**
  * @brief ��Ϣ��֡������
  */
typedef enum
{
    EProtocolType_Cmd,      /*!< ָ�Ҫ��ͨ�ŷ�����Ӧ�� */
    EProtocolType_Data,     /*!< ���ݣ�Ҫ��ͨ�ŷ�����Ӧ��Ч����ָ��һ�� */
    EProtocolType_Reply,    /*!< Ӧ���ڲ�ʹ�ã��û�����ʹ�ã� */
    EProtocolType_Notice,   /*!< ֪ͨ����Ҫ��ͨ�ŷ�����Ӧ�� */
}EProtocolType;

/**
  * @brief ��������
  */
typedef enum
{
    EErrorType_CheckError         , /*!< У����� The check error */
    EErrorType_LineNoise        ,   /*!< ��·���� Line noise error */
    EErrorType_MsgBufferFull     ,  /*!< ��Ϣ�������� Message buffer full error */
    EErrorType_BackupBufferFull  ,  /*!< ֡������ Frame backup buff full error */
    EErrorType_TxComBufferFull  ,   /*!< ���ͻ������� serial send buff full error */
    EErrorType_Resend,              /*!< �ط�ʧ�� Cmd and data resend three times error */
    EErrorType_Total,
}EErrorType;

/**
  * @brief ���ں�
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
  * @brief ����������շ��ͻ�������16�ֽ�
  */
typedef struct tagComBuffCtrl
{
    unsigned int m_iWriteIndex;
    unsigned int m_iReadIndex;
    const unsigned int m_iLen;
    unsigned char* m_pBuff;
}ComBuffCtrl,*PComBuffCtrl;

/**
  * @brief Ӳ������շ��������ṹ��40�ֽ�
  */
typedef struct tagHardwareComCtrl
{
    ComBuffCtrl m_TxComBuffCtrl;            /*!< ���ڷ��ͻ������ */
    ComBuffCtrl m_RxComBuffCtrl;            /*!< ���ڽ��ջ������ */
    const unsigned char m_ucUartId;         /*!< ���ں� */
    unsigned char m_ucReserve[3];           /*!< Ԥ�� */
    void (*psend)(unsigned char UartId);    /*!< ���ڷ��ͺ���ָ��,����ע�ắ����һ������ʹ�ܷ��� */
}HardwareCtrl;

/**
  * @brief ֡ͷ��4�ֽ�----�ڲ�ʹ��
  */
typedef struct tagFrameHead
{
    unsigned char m_ucLen;      /*!< ֡�� */
    UCHAR_BIT m_ucVer:4;        /*!< �汾�� The low 4 bit */
    UCHAR_BIT m_ucFrameType:4;  /*!< ֡���� ��4λ */
    unsigned char m_ucSeqId;    /*!< ��ˮ�� The sequence ID */
    unsigned char m_ucFucId;    /*!< ���ܺ� The function ID */
}FrameHead;

/**
  * @brief ��׼֡��72�ֽ�-----�ڲ�ʹ��
  */
typedef struct tagStandardFrame
{
    FrameHead Head; /*!< ֡ͷ */
    unsigned char FrameData[FRAME_DATA_BUFF_SIZE];  /*!< ֡������ Include the checksum */
}StandardFrame;

/**
  * @brief ֡ת��󻺳�����140�ֽ�-----�ڲ�ʹ��
  */
typedef struct tagAfterEscape
{
    unsigned int  m_iLen;   /*!< ���� */
    unsigned char m_ucDataTable[AFTER_ESCAPE_DATA_BUFF_SIZE];   /*!< ������ */
}AfterEscape;

/** 
  * @brief ֡ת��ǰ������,144�ֽ�-----�ڲ�ʹ��
  */
typedef struct tagBeforeEscape
{
    unsigned int    m_iPraseIndex;                                  /*!< �ϴν�����λ�� Record the position of the current analytical */
    unsigned int    m_iLen;                                         /*!< ���������� The length of the data before escape */
    unsigned char   m_ucDataTable[BEFORE_ESCAPE_DATA_BUFF_SIZE];    /*!< ������ the data before escape */
}BeforeEscape;

/**
  * @brief Ӧ����ƣ�144�ֽ�-----�ڲ�ʹ��
  */
typedef struct tagReplyCtrl
{
    unsigned  m_iResendTime:4;                               /*!< ʣ���ط����� The remaining resend times */
    unsigned  m_iTicks:12;                                   /*!< ��ʱʱ�� the timeout ticks */
    unsigned char m_ucSeqId;                                 /*!< ��ˮ�� The sequence ID */
    unsigned char m_ucFucId;                                 /*!< ���ܺ� The function ID */
    unsigned int  m_iReplyDataLen;                           /*!< Ӧ�𳤶� The length of the data after escape */
    unsigned char m_iReplyDataBuf[REPLY_CTRL_DATA_BUFF_SIZE];/*!< Ӧ�𻺳��� the data after escape */
}ReplyCtrl;

/**
  * @brief �����¼��4�ֽ�-----�ڲ�ʹ��
  */
typedef struct tagErrorCtrl
{
    unsigned char m_ucErrorCount;   /*!< ���������� error count */
    unsigned char m_ucDetial[3];    /*!< �������� func id,seq id */
}ErrorCtrl;

/**
  * @brief ��Ϣ�ṹ�壬68�ֽ�
  */
typedef struct tagMessage
{
    unsigned char m_ucLen;  /*!< ��Ϣ�����ݳ��� the data length of the m_ucParaTable */
    unsigned char m_ucType; /*!< ��Ϣ���� */
    unsigned char m_ucFuctionId; /*!< ���ܺ� */
    unsigned char m_ucParaTable[MESSAGE_DATA_BUFF_SIZE+1]; /*!< ���ݻ����� */
}Message;

/**
  * @brief ��Ϣ��������16�ֽ�
  */
typedef struct  tagMsgBuffCtrl
{
    unsigned int m_iWriteIndex; /*!< дָ�� */
    unsigned int m_iReadIndex;  /*!< ��ָ�� */
    const unsigned int m_iLen;  /*!< ��������С */
    Message* m_pMsgBuff;        /*!< ��Ϣ������ */
}MsgBuffCtrl;

/**
  * @brief Ӧ�ò���Ϣ��������32�ֽ�
  */
typedef struct tagApplicationCtrl
{
    MsgBuffCtrl m_TxMsgBuffCtrl;    /*!< Message send buffer */
    MsgBuffCtrl m_RxMsgBuffCtrl;    /*!< Message receive buffer */
}AppCtrl;

/**
  * @brief ͨ�ſ��ƽṹ��88�ֽ�
  */
typedef struct tagComCtrl
{
    HardwareCtrl   m_HardwareCtrl;      /*!< Ӳ���㹦�ܽṹ */
    AppCtrl        m_AppCtrl;           /*!< Ӧ�ò���ƽṹ */
    ReplyCtrl*     m_ReplyQueue;        /*!< Ӧ����ƽṹ(��ȷ������Ϣ���ͻ�������Сһ��) */
    AfterEscape*   m_RevBackupFrame;    /*!< ת��󻺳������� */
    BeforeEscape*  m_RevBeforeFrame;    /*!< ת��ǰ���������� */
    ErrorCtrl*     m_ErrorCtrl;         /*!< �����¼ */
}ComCtrl,*PComCtrl;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
