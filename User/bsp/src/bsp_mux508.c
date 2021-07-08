/**
  ******************************************************************************
  * @file    bsp_mux508.c
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#include "bsp.h"

/**
  * @brief 多路开关初始化
  */
void mux508_init(void)
{
#ifdef MUX508_RCC  
    GPIO_InitTypeDef GPIO_InitStructure;
    
    _PRINT("mux508_init()\r\n");
    
    RCC_APB2PeriphClockCmd(MUX508_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = MUX508_A0_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MUX508_A0_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MUX508_A1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MUX508_A1_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MUX508_A2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MUX508_A2_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MUX508_EN1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MUX508_EN1_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MUX508_EN2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MUX508_EN2_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MUX508_EN3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MUX508_EN3_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MUX508_EN4_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MUX508_EN4_PORT, &GPIO_InitStructure);
    
    GPIO_ResetBits(MUX508_EN1_PORT, MUX508_EN1_PIN);
    GPIO_ResetBits(MUX508_EN2_PORT, MUX508_EN2_PIN);
    GPIO_ResetBits(MUX508_EN3_PORT, MUX508_EN3_PIN);
    GPIO_ResetBits(MUX508_EN4_PORT, MUX508_EN4_PIN);
    GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
    GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
    GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);  
#endif
}

/**
  * @brief 设置多路开关
  * @param point
  */
void mux508_set(RECT_LATTICE point)
{
#ifdef MUX508_RCC
    GPIO_ResetBits(MUX508_EN1_PORT, MUX508_EN1_PIN);
    GPIO_ResetBits(MUX508_EN2_PORT, MUX508_EN2_PIN);
    GPIO_ResetBits(MUX508_EN3_PORT, MUX508_EN3_PIN);
    GPIO_ResetBits(MUX508_EN4_PORT, MUX508_EN4_PIN);

#ifdef PCB_V3
    switch(point)
    {
    case TP1:// S1
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
        break;
    case TP2:// S2
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
        break;
    case TP3:
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
        break;
    case TP4:
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
        break;     
    case TP5:// S5
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
        break; 
    case TP6:// S6
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
        break; 
    case TP7:// S7
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
        break; 
    case TP8:// S8
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
        break; 
    
    case TP9:// S1
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
        break;
    case TP10:// S2
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
        break;      
    case TP11:
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
        break;
    case TP12:
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
        break;
    case TP13: // S8
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
        break;
    case TP14: // S7
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
        break;
    case TP15: // S6
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
        break;
    case TP16: // S5
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
        break;
    
    case TP17: // S1
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
        break;
    case TP18: // S2
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
        break;
    case TP19: // S3
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
        break;
    case TP20: // S4
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
        break;
    case TP21: // S8
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
        break;
    case TP22: // S7
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
        break;
    case TP23: // S6
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
        break;
    case TP24: // S5
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
        break;
    
    case TP25: // S4
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
        break; 
    case TP26: // S3
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
        break;
    case TP27:// S2
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
        break;  
    case TP28:// S1
        GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
        break;   
    case TP29: // S8
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
        break;
    case TP30: // S7
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
        break;
    case TP31: // S6
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
        break;
    case TP32: // S5
        GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
        GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
        GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
        GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
        break;
    
    default:
        break;
    }
#else    
    if(s_pcb_verion == PCB_FINGER)
    {
        switch(point)
        {
        case TP1: // S3 -> MUX1_S1 -> 1
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP2: // S4 -> MUX1_S2 -> 2
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP3: // S8 -> MUX1_S3 -> 3
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP4:// S7 -> MUX1_S4 -> 4
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP5:// S6 -> MUX1_S5 -> 5
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        
        case TP6:// S3 -> MUX2_S5 -> 6
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP7:// S4 -> MUX2_S4 -> 7
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP8:// S8 -> MUX2_S3 -> 8
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP9:// S7 -> MUX2_S2-> 9
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP10:// S6 -> MUX2_S1 -> 10
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        
        case TP11:// S1 -> MUX3_S1 -> 11
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP12:// S2 -> MUX3_S2 -> 12
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP13:// S3 -> MUX3_S3 -> 13
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP14:// S4 -> MUX3_S4 -> 14
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP15:// S8 -> MUX3_S5 -> 15
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        
        case TP16:// S1 -> MUX4_S5 -> 16
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP17:// S2 -> MUX4_S4 -> 17
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP18:// S3 -> MUX4_S3 -> 18
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP19:// S7 -> MUX4_S2 -> 19
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP20:// S8 -> MUX4_S1 -> 20
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
            
        default:
            break;
        }
    }
    else
    {      
        switch(point)
        {
#ifdef RECTANGULAR_lATTICE_V0_1
        case TP4: // S1 -> MUX1_S4 -> PAD4 -> 3
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP3: // S2 -> MUX1_S3 -> PAD3 -> 4
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
#else
        case TP3: // S1 -> MUX1_S4 -> PAD4 -> 3
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP4: // S2 -> MUX1_S3 -> PAD3 -> 4
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
#endif
        case TP2:
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP1:
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
#ifdef RECTANGULAR_lATTICE_V0_1
        case TP8: // S5 -> MUX1_S5 -> PAD5 -> 7
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP7: // S6 -> MUX1_S6 -> PAD6 -> 8
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
#else        
        case TP7: // S5 -> MUX1_S5 -> PAD5 -> 7
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP8: // S6 -> MUX1_S6 -> PAD6 -> 8
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
#endif        
        case TP6:
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
        case TP5:
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN1_PORT,  MUX508_EN1_PIN);
            break;
#ifdef RECTANGULAR_lATTICE_V0_1
        case TP12:// S1 -> MUX2_S4 -> PAD12 -> 11
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP11:// S2 -> MUX2_S3 -> PAD11 -> 12
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
#else        
        case TP11:// S1 -> MUX2_S4 -> PAD12 -> 11
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP12:// S2 -> MUX2_S3 -> PAD11 -> 12
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
#endif        
        case TP10:
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP9:
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
#ifdef RECTANGULAR_lATTICE_V0_1
        case TP16:// S5 -> MUX2_S5 -> PAD13 -> 15
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP15:// S6 -> MUX2_S6 -> PAD14 -> 16
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
#else        
        case TP15:// S5 -> MUX2_S5 -> PAD13 -> 15
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP16:// S6 -> MUX2_S6 -> PAD14 -> 16
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
#endif
        case TP14:
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
        case TP13:
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN2_PORT,  MUX508_EN2_PIN);
            break;
#ifdef RECTANGULAR_lATTICE_V0_1
        case TP21:// S1 -> MUX3_S1 -> PAD17 -> 22
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP22:// S2 -> MUX3_S2 -> PAD18 -> 21
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
#else        
        case TP22:// S1 -> MUX3_S1 -> PAD17 -> 22
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP21:// S2 -> MUX3_S2 -> PAD18 -> 21
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
#endif
        case TP23:
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP24:
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
#ifdef RECTANGULAR_lATTICE_V0_1
        case TP17:// S5 -> MUX3_S5 -> PAD21 -> 18
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP18:// S6 -> MUX3_S6 -> PAD22 -> 17
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
#else        
        case TP18:// S5 -> MUX3_S5 -> PAD21 -> 18
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP17:// S6 -> MUX3_S6 -> PAD22 -> 17
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
#endif
        case TP19:
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
        case TP20:
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN3_PORT,  MUX508_EN3_PIN);
            break;
#ifdef RECTANGULAR_lATTICE_V0_1        
        case TP29:// S1 -> MUX4_S1 -> PAD25 -> 30
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP30:// S2 -> MUX4_S2 -> PAD26 -> 29
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
#else
        case TP30:// S1 -> MUX4_S1 -> PAD25 -> 30
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP29:// S2 -> MUX4_S2 -> PAD26 -> 29
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
#endif        
        case TP31:
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP32:
            GPIO_ResetBits(MUX508_A2_PORT, MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
#ifdef RECTANGULAR_lATTICE_V0_1
        case TP25:// S5 -> MUX4_S5 -> PAD29 -> 26
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP26:// S6 -> MUX4_S6 -> PAD30 -> 25
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
#else        
        case TP26:// S5 -> MUX4_S5 -> PAD29 -> 26
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP25:// S6 -> MUX4_S6 -> PAD30 -> 25
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_ResetBits(MUX508_A1_PORT, MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
#endif        
        case TP27:
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_ResetBits(MUX508_A0_PORT, MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        case TP28:
            GPIO_SetBits(MUX508_A2_PORT,   MUX508_A2_PIN);
            GPIO_SetBits(MUX508_A1_PORT,   MUX508_A1_PIN);
            GPIO_SetBits(MUX508_A0_PORT,   MUX508_A0_PIN);
            GPIO_SetBits(MUX508_EN4_PORT,  MUX508_EN4_PIN);
            break;
        
        default:
            break;
        }
    }
#endif
#endif
}

/**
  * @brief 多路开关初始化
  */
void ADG708_init(void)
{
#ifdef ADG708_RCC  
    GPIO_InitTypeDef GPIO_InitStructure;
    
    _PRINT("ADG708_init()\r\n");
    
    RCC_APB2PeriphClockCmd(ADG708_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = ADG708_A0_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ADG708_A0_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ADG708_A1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ADG708_A1_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ADG708_A2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ADG708_A2_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ADG708_I1_EN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ADG708_I1_EN_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ADG708_I2_EN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ADG708_I2_EN_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ADG708_I3_EN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ADG708_I3_EN_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ADG708_V1_EN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ADG708_V1_EN_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ADG708_V2_EN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ADG708_V2_EN_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ADG708_V3_EN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ADG708_V3_EN_PORT, &GPIO_InitStructure);
    
    GPIO_ResetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
    GPIO_ResetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
    GPIO_ResetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
    GPIO_ResetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
    GPIO_ResetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
    GPIO_ResetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);
    GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
    GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
    GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);  
#endif
}

/**
  * @brief 设置多路开关
  * @param point
  */
void ADG708_set(RECT_LATTICE point)
{
#ifdef ADG708_RCC
    GPIO_ResetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
    GPIO_ResetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
    GPIO_ResetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
    GPIO_ResetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
    GPIO_ResetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
    GPIO_ResetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);

    if(s_pcb_verion == PCB_FINGER)
    {
        switch(point)
        {
        case TP1: // S1
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
            GPIO_SetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
            break;
        case TP2: // S2
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
            GPIO_SetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
            break;
        case TP3: // S3
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
            GPIO_SetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
            break;
        case TP4: // S4
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
            GPIO_SetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
            break;
        case TP5: // S5
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
            GPIO_SetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
            break; 
        case TP6:// S6
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
            GPIO_SetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
            break;
        case TP7:// S7
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
            GPIO_SetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
            break;        
        case TP8: // S8
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V1_EN_PORT, ADG708_V1_EN_PIN);
            GPIO_SetBits(ADG708_I1_EN_PORT, ADG708_I1_EN_PIN);
            break;
        
        case TP9: // S1
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
            GPIO_SetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
            break;      
        case TP10: // S2
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
            GPIO_SetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
            break;
        
        case TP11:// S3
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
            GPIO_SetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
            break;
        case TP12:// S4
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
            GPIO_SetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
            break;
        case TP13: // S5
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
            GPIO_SetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
            break;
        case TP14: // S6
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
            GPIO_SetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
            break;     
        case TP15:// S7
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
            GPIO_SetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
            break;        
        case TP16:// S8
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V2_EN_PORT, ADG708_V2_EN_PIN);
            GPIO_SetBits(ADG708_I2_EN_PORT, ADG708_I2_EN_PIN);
            break;
        
        case TP17:// S1
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
            GPIO_SetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);
            break;      
        case TP18: // S2
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
            GPIO_SetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);
            break;
        case TP19: // S3
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
            GPIO_SetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);
            break;
        case TP20: // S4
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_ResetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
            GPIO_SetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);
            break;
        case TP21: // S5
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
            GPIO_SetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);
            break;
        case TP22: // S6
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_ResetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
            GPIO_SetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);
            break;
        case TP23: // S7
            GPIO_ResetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
            GPIO_SetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);
            break;
        case TP24: // S8
            GPIO_SetBits(ADG708_A0_PORT, ADG708_A0_PIN);
            GPIO_SetBits(ADG708_A1_PORT, ADG708_A1_PIN);
            GPIO_SetBits(ADG708_A2_PORT, ADG708_A2_PIN);
            GPIO_SetBits(ADG708_V3_EN_PORT, ADG708_V3_EN_PIN);
            GPIO_SetBits(ADG708_I3_EN_PORT, ADG708_I3_EN_PIN);
            break;
        default:
            break;
        }
    }
#endif
}
