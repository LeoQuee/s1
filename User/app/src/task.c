/**
  ******************************************************************************
  * @file    task.c
  * @author  lik
  * @date    2021-7-8
  * @brief   �������ļ�
  ******************************************************************************
  */

#include "task.h"

#include "protocol_task.h"
#include "HardwareLayer.h"

#include "afeStateMachine.h"
#include "bsp_timer.h"

#include "bsp_dac.h"
#include "bsp_oled.h"
#include "bsp_w25q128.h"

#define ALARM_OFF   0
#define ALARM_ON    1

int8_t g_PcHeartBeatCount = 0;      /*!< ��λ���������� */
static int8_t s_IsAlarmed = ALARM_OFF;      /*!< ��ǰ�Ƿ�Ϊ����״̬��0-�� 1-�� */

/**
  * @brief �����ʱ���������
  */
static void ProcessSoftTimer(void)
{
    if(bsp_CheckTimer(TMR1_COMPLATFORM, TMR_NOT_NEEDSTOP))
    {
        ComDataProcess(&CommToPc);
    }
    
    if(bsp_CheckTimer(TMR3_SND_M3_HAERT, TMR_NOT_NEEDSTOP))
    {
        // ����λ��������������
        PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_EXEC_HEART_BEAT, EProtocolType_Notice, 0, NULL);
    }
    
    if(bsp_CheckTimer(TMR2_PC_HEART_BEAT, TMR_NOT_NEEDSTOP))
    {
        if(g_PcHeartBeatCount >= 3)
        {
            g_PcHeartBeatCount = 0;

            if(s_IsAlarmed == ALARM_ON)
            {
                s_IsAlarmed = ALARM_OFF;
                
                //_PRINT("pc heart recover!\r\n");
                bsp_SetLedStatus(LED_YELLOW, LED_OFF);
            }
        }
        else
        {
            if(s_IsAlarmed == ALARM_OFF)
            {
                s_IsAlarmed = ALARM_ON;
                
                //_PRINT("pc heart lose!\r\n");
                bsp_SetLedStatus(LED_YELLOW, LED_BLINK);
            }
        }
    }
    
    if(bsp_CheckTimer(TMR4_ALARM_LED, TMR_NOT_NEEDSTOP))
    {
        static uint8_t s_flag = 0;
        
        if(s_flag == 0)
        {
            s_flag = 1;
            
            if(bsp_GetLedStatus(LED_RED) == LED_ON)
            {
                LED_RED_ON();
            }
            else
            {
                LED_RED_OFF();
            }
            
            if(bsp_GetLedStatus(LED_GREEN) == LED_ON)
            {
                LED_GREEN_ON();
            }
            else
            {
                LED_GREEN_OFF();
            }
            
            if(bsp_GetLedStatus(LED_YELLOW) == LED_ON)
            {
                LED_YELLOW_ON();
            }
            else
            {
                LED_YELLOW_OFF();
            }
        }
        else
        {
            s_flag = 0;
            
            if(bsp_GetLedStatus(LED_RED) == LED_OFF)
            {
                LED_RED_OFF();
            }
            else
            {
                LED_RED_ON();
            }
            
            if(bsp_GetLedStatus(LED_GREEN) == LED_OFF)
            {
                LED_GREEN_OFF();
            }
            else
            {
                LED_GREEN_ON();
            }
            
            if(bsp_GetLedStatus(LED_YELLOW) == LED_OFF)
            {
                LED_YELLOW_OFF();
            }
            else
            {
                LED_YELLOW_ON();
            }
        }
        
    }
#ifdef _USART_PRINT_DEBUG_    
//    if(bsp_CheckTimer(TMR6_ADJUST_TEST, TMR_NOT_NEEDSTOP))
//    {

//        _PRINT("--TIMER TASK: ADJUST TEST--\r\n");
//    
//        SetAfe4300State(AFE_STATE_ADJUST);
//        AFE4300_Set_Adjust_On();
//    }
#endif
}

/**
 * @brief ����ָ���
 * @details����ǰ�汾ֻ������������¼���
 */
static void ProcessKeyEvent(void)
{
    uint8_t ucKeyCode;
    static int s_point_id = 1;

    ucKeyCode = bsp_GetKey();
    if(ucKeyCode <= 0)
    {
        return;
    }
    
    switch (ucKeyCode)
    {
    case KEY_MENU:
    
#ifdef _USART_PRINT_DEBUG_
        _PRINT("\r\n--KEY MENU--\r\n");
    
        AFE4300_Stop();    

        {
            float temp, hum;
            
            shtc3_readdata(&temp, &hum); // ��ȡ�¶Ⱥ�ʪ��
            
            _PRINT("T=%f, H=%f", temp, hum);
        }
        {
            _PRINT("Flash:%X", W25QXX_ReadID());
            _PRINT("Flash:%X", W25QXX_Read_SR());
            
            bsp_dac_close();
        }
#endif
        break;

    case KEY_DOWN:
    
#ifdef    _USART_PRINT_DEBUG_
        _PRINT("\r\n--KEY DOWN--\r\n");
    
        AFE4300_Set_SingleTest_On(s_point_id);
#else
        bsp_dac_volume_dec();    
#endif        
		break;
    
    case KEY_ENTER:
#ifdef    _USART_PRINT_DEBUG_
        {
            //static int id = 0;
            
            _PRINT("\r\n--KEY ENTER--\r\n");
        
            //AFE4300_PointInc();
            s_point_id++;
            if(s_point_id > 20) s_point_id = 1;
            _PRINT("\r\nPoint:%d\r\n", s_point_id);
        }
#endif
        break;
    
    case KEY_UP:
    
#ifdef    _USART_PRINT_DEBUG_
        _PRINT("\r\n--KEY UP--\r\n");
    
        SetAfe4300State(AFE_STATE_ADJUST);
        AFE4300_Set_Adjust_On();  
#else
        bsp_dac_volume_inc(); 
#endif
        break;

    default:
        break;
    }
}

/**
  * @brief AFE4300״̬����
  */
static void ProcessAfeStateMonitor(void)
{
    AFE4300_WORK_STATE state = GetAfe4300State();
    AFE_STATE_ERROR_TYPE curState;
    
    switch (state)
    {
	case AFE_STATE_ADJUST:
		curState = AFE4300_StartAdjust();
		if(AFE_SUCCESS == curState)
		{
            bsp_dac_open(DAC_ID_ADJUST_OVER);
            
			_PRINT("\r\nadjust over!");
            
			PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_ADJUST_OVER, EProtocolType_Notice, 0, NULL);
			SetAfe4300State(AFE_STATE_INIT);
		}
		else if(AFE_FAILED == curState)
		{
			SetAfe4300State(AFE_STATE_INIT);
		}
        break;
        
	case AFE_STATE_TEST:
		curState = AFE4300_StartTest();
		if(AFE_SUCCESS == curState)
		{
            // ������ʾ��
            bsp_dac_open(DAC_ID_TEST_OVER);
            
            // ����ָʾ��
            bsp_SetLedStatus(LED_RED, LED_OFF);
            
			_PRINT("\r\n\r\ntest over!");
            
			PackMessage(&CommToPc.m_AppCtrl.m_TxMsgBuffCtrl, SND_NOT_TEST_OVER, EProtocolType_Notice, 0, NULL);
            
			SetAfe4300State(AFE_STATE_INIT);
		}
		else if(AFE_FAILED == curState)
		{
            // ����ָʾ��
            bsp_SetLedStatus(LED_RED, LED_OFF);
			SetAfe4300State(AFE_STATE_INIT);
		}
        break;
        
    case AFE_STATE_SINGLE_TEST:
        curState = AFE4300_SingleTest();
        if(curState == AFE_SUCCESS || curState == AFE_FAILED)
        {
            SetAfe4300State(AFE_STATE_INIT);
        }
        break;

	default:
		break;
    }  
}

/**
  * @brief ������
  */
void TaskProcess(void)
{
    ProcessSoftTimer();
    ProcessKeyEvent();
    AnalyzeDataFromPC();
    ProcessAfeStateMonitor();
}
