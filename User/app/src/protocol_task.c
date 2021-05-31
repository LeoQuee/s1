#include "bsp.h"
#include "protocol_task.h"
#include "protocol.h"
#include "AppLayer.h"
#include "afeStateMachine.h"
#include "bsp_dac.h"

/**
  * @brief ���ں����б�
  */
extern Serial_Fun SerialFunHandler[RCV_MSG_TABLE_END - RCV_MSG_TABLE_START];

extern int8_t g_PcHeartBeatCount; /*!< ��λ���������� */

/**
  * @brief ��λ������������
  * @param data_len
  * @param data
  */
static void MAIN_HEART_BEAT_Handler(unsigned char data_len, unsigned char data[])
{
    if(g_PcHeartBeatCount < 100)
    {
        g_PcHeartBeatCount++;
    }
}

/**
  * @brief ��ʼУ׼������
  * @param data_len
  * @param data
  */
static void START_ADJUST_Handler(unsigned char data_len, unsigned char data[])
{
    AFE4300_WORK_STATE state = GetAfe4300State();
    if(AFE_STATE_INIT == state)
    {
        bsp_dac_open(2);
        
        SetAfe4300State(AFE_STATE_ADJUST);
        
        AFE4300_Set_Adjust_On();
    }
    else
    {
        uint8_t dataBuff = state;
        PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_DEVICE_IS_BUSY, EProtocolType_Notice, sizeof(dataBuff), &dataBuff);
    }
}

/**
  * @brief ��ʼ���Դ�����
  * @param data_len
  * @param data
  */
static void START_TEST_Handler(unsigned char data_len, unsigned char data[])
{
    AFE4300_WORK_STATE state = GetAfe4300State();
    if(AFE_STATE_INIT == state)
    {
        SetAfe4300State(AFE_STATE_TEST);
        
        AFE4300_Set_Test_On();
        
        // ����ָʾ��
        bsp_SetLedStatus(LED_RED, LED_BLINK);
        
        // ���Ͱ汾��
        GET_SOFTWARE_VERSION_Handler(data_len, data);
        
        // �����豸��
        SF_Read_SN(data_len, data);
        
        // ������ʪ��
#if (defined I2C1_RCC) &&  (defined I2C2_RCC)
        {
            float temp, hum;
            uint8_t dataBuff[8];
            
            shtc3_readdata(&temp, &hum);
            
            ((float *)dataBuff)[0] = temp;
            ((float *)dataBuff)[1] = hum;
            
            PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_DAT_TEMP_HUMIDITY, EProtocolType_Data, sizeof(dataBuff), dataBuff);
        }  
#endif
    }
    else
    {
        uint8_t dataBuff = state;
        PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_DEVICE_IS_BUSY, EProtocolType_Notice, sizeof(dataBuff), &dataBuff);
    }
}

/**
  * @brief ��ȡ����汾�Ŵ�����
  * @param data_len
  * @param data
  */
static void GET_SOFTWARE_VERSION_Handler(unsigned char data_len, unsigned char data[])
{
    uint8_t dataBuff[3];
    
    dataBuff[0] = SOFTWARE_VERSION_MAIN;
    dataBuff[1] = SOFTWARE_VERSION_SUB1;
    dataBuff[2] = SOFTWARE_VERSION_SUB2;
    PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_DAT_VERSION_INFO, EProtocolType_Data, sizeof(dataBuff), dataBuff);
}

/**
  * @brief ϵͳ��λ������
  * @param data_len
  * @param data
  */
static void RESET_Handler(unsigned char data_len, unsigned char data[])
{
    DISABLE_INT();
    NVIC_SystemReset();
}

/**
  * @brief ���������ߴ�����
  * @param data_len
  * @param data
  */
static void SET_POINT_HIGH(unsigned char data_len, unsigned char data[])
{
    uint8_t point_index = data[0];
    
    mux508_set(point_index); 
    
    _PRINT("RCV_CMD_SET_POINT_HIGH:%d\r\n", point_index);
}

/**
  * @brief (��ʵ��)����Ƶ��
  * @param data_len
  * @param data
  */
static void SF_Set_Freq(unsigned char data_len, unsigned char data[])
{
    _PRINT("Set Freq:%d\r\n", data[0]);
}

/**
  * @brief �������
  * @param data_len
  * @param data
  */
static void SF_Single_Point_Test(unsigned char data_len, unsigned char data[])
{
    uint8_t point_id = data[0];
    
    AFE4300_Set_SingleTest_On(point_id);
    
    _PRINT("Single Point Test:%d\r\n", point_id);
}

/**
  * @brief (��ʵ��)��Ƶ�ʲ���
  * @param data_len
  * @param data
  */
static void SF_Single_Freq_Test(unsigned char data_len, unsigned char data[])
{
    _PRINT("Single Freq Test:%d\r\n", data[0]);
}

/**
  * @brief ֹͣ����/У׼
  * @param data_len
  * @param data
  */
static void SF_Stop(unsigned char data_len, unsigned char data[])
{
    AFE4300_Stop();
    
    _PRINT("Stop\r\n");
}

/**
  * @brief д��ϵͳ����
  * @param data_len
  * @param data
  */
static void SF_Write_Param(unsigned char data_len, unsigned char data[])
{
    SysParam param = {100.0f, 200.0f, 300.0f, 2020, 9, 16};
    SysParam *p = (SysParam *)data;
    SYSFLAG s;
    
    _PRINT("Write: %.2f ohm, %.2f ohm, %.2f ohm, %d-%d-%d\r\n", p->Rref[0], p->Rref[1], p->Rref[2], p->year, p->month, p->day);
    
    sysflag_read(&s);
    s.R1 = p->Rref[0];
    s.R2 = p->Rref[1];
    s.R3 = p->Rref[2];
    s.year = p->year;
    s.month = p->month;
    s.day = p->day;
    sysflag_write(&s);
    sysflag_read(&s);
    param.Rref[0] = s.R1;
    param.Rref[1] = s.R2;
    param.Rref[2] = s.R3;
    param.year    = s.year;
    param.month   = s.month;
    param.day     = s.day;
    
    PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_DAT_SYS_PARAM, EProtocolType_Data, sizeof(SysParam), (uint8_t *)&param);
}

/**
  * @brief ��ȡϵͳ����
  * @param data_len
  * @param data
  */
static void SF_Read_Param(unsigned char data_len, unsigned char data[])
{
    SysParam param = {100.0f, 200.0f, 300.0f, 2020, 9, 16};
    SYSFLAG s;
    
    sysflag_read(&s);
    param.Rref[0] = s.R1;
    param.Rref[1] = s.R2;
    param.Rref[2] = s.R3;
    param.year    = s.year;
    param.month   = s.month;
    param.day     = s.day;
    
    _PRINT("Read: %.2f ohm, %.2f ohm, %.2f ohm, %d-%d-%d\r\n", param.Rref[0], param.Rref[1], param.Rref[2], param.year, param.month, param.day);
    
    PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_DAT_SYS_PARAM, EProtocolType_Data, sizeof(SysParam), (uint8_t *)&param);
}

/**
  * @brief д���豸��
  * @param data_len
  * @param data
  */
static void SF_Write_SN(unsigned char data_len, unsigned char data[])
{
    uint8_t dataBuff[16] = "demo-001";
    SYSFLAG s;
    
    _PRINT("Write:%s\r\n", data);
    
    sysflag_read(&s);
    strcpy(s.sn, (char *)data);
    sysflag_write(&s);
    sysflag_read(&s);
    strcpy((char *)dataBuff, s.sn);
    
    PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_DAT_SN, EProtocolType_Data, sizeof(dataBuff), dataBuff);
}

/**
  * @brief ��ȡ�豸��
  * @param data_len
  * @param data
  */
static void SF_Read_SN(unsigned char data_len, unsigned char data[])
{
    uint8_t dataBuff[16] = "demo-001";
    SYSFLAG s;
    
    _PRINT("Read:%s\r\n", dataBuff);
    sysflag_read(&s);
    strcpy((char *)dataBuff, s.sn);
    
    PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_DAT_SN, EProtocolType_Data, sizeof(dataBuff), dataBuff);
}

/**
  * @brief д����ʾ����
  * @param data_len
  * @param data
  */
static void SF_Write_Sound(unsigned char data_len, unsigned char data[])
{
    uint32_t addr = *(uint32_t *)data;
    //SYSFLAG s;
    char sbuf[20];
    
    sprintf(sbuf, "-%d-\r\n", addr);
    
    if((addr&0xFFF) == 0) // sector��ʼλ��
    {
        W25QXX_Erase_Sector(addr);
    }
    W25QXX_Write(addr, data_len-4, data+4);
    
    SendAbnormalInfo(sbuf);
}

/**
  * @brief ���������б��ʼ��
  */
void SerialTaskListInit(void)
{
    SerialFunHandler[RCV_NOT_MAIN_HEART_BEAT]       = MAIN_HEART_BEAT_Handler;
    SerialFunHandler[RCV_CMD_START_ADJUST]          = START_ADJUST_Handler;
    SerialFunHandler[RCV_CMD_START_TEST]            = START_TEST_Handler;
    SerialFunHandler[RCV_CMD_GET_VERSION]           = GET_SOFTWARE_VERSION_Handler;
    SerialFunHandler[RCV_NOT_RESET]                 = RESET_Handler;
    SerialFunHandler[RCV_CMD_SET_POINT_HIGH]        = SET_POINT_HIGH;
    SerialFunHandler[RCV_CMD_SET_FREQ]              = SF_Set_Freq;
    SerialFunHandler[RCV_CMD_SINGLE_POINT_TEST]     = SF_Single_Point_Test;
    SerialFunHandler[RCV_CMD_SINGLE_FREQ_TEST]      = SF_Single_Freq_Test;
    SerialFunHandler[RCV_CMD_STOP]                  = SF_Stop;
    SerialFunHandler[RCV_CMD_WRITE_PARAM]           = SF_Write_Param;
    SerialFunHandler[RCV_CMD_READ_PARAM]            = SF_Read_Param;
    SerialFunHandler[RCV_CMD_WRITE_SN]              = SF_Write_SN;
    SerialFunHandler[RCV_CMD_READ_SN]               = SF_Read_SN;
    SerialFunHandler[RCV_CMD_WRITE_SOUND]           = SF_Write_Sound;
    
    PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_POWER_ON, EProtocolType_Notice, 0, NULL);
}

/**
  * @brief �����쳣֪ͨ
  * @param str �쳣֪ͨ
  */
static void SendAbnormalInfo(char *str)
{
    PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_ABNORMAL, EProtocolType_Notice, strlen(str) + 1, (uint8_t *)str);
}
