/*
 * @Description:  main.c
 * @Author: Gaven
 * @Date: 2019-10-21 10:34:45
 * @LastEditTime: 2019-10-29 11:50:09
 * @LastEditors: Gaven
 */
 
#include "main.h"
#include "bsp.h"
#include "task.h"
#include "protocol_task.h"

/**
  * @brief ������
  */
int main(void)
{
    bsp_Init();
    PrintfLogo();
    SerialTaskListInit();
    bsp_StartAllTimer();
    
    while (1)
    {
        bsp_Idle();
        TaskProcess();
    }
}

/**
  * @brief �ϵ���ӡ��Ϣ
  */
static void PrintfLogo(void)
{
    _PRINT("*************************************************************\r\n");
    _PRINT("* Project Name     : MultiPoint Impedance Measure\r\n");
    _PRINT("* Project Version  : %d.%d.%d\r\n", SOFTWARE_VERSION_MAIN, SOFTWARE_VERSION_SUB1, SOFTWARE_VERSION_SUB2);
    _PRINT("* FWLib Version    : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
           __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    _PRINT("* CMSIS Version    : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    _PRINT("* \r\n");
    _PRINT("* Copyright 2021 GuangZhou ZhongKang.\r\n");
    _PRINT("*************************************************************\r\n");
}
