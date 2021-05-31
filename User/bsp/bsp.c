/*
 * @Description:  这是硬件底层驱动程序模块的主文件。主要提供 bsp_Init()函数供主程序调用。主程序的每个c文件可以在开
 *			        头添加 #include "bsp.h" 来包含所有的外设驱动模块。
 * @Author: Gaven
 * @Date: 2019-10-21 10:34:45
 * @LastEditTime: 2019-10-30 08:48:01
 * @LastEditors: Gaven
 */
 
#include "bsp.h"
#include "bsp_oled.h"
    
/**
  * @brief 初始化硬件设备。
  * @details 只需要调用一次。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。
  */
void bsp_Init(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    init_version_bit();
    bsp_InitTimer();
    bsp_InitUart();
    bsp_InitKey();
	bsp_Afe4300Init();
    mux508_init();
    led_init();
    i2c_init();
    bsp_DelayMS(200);
    
#ifdef PCB_V3    
    W25QXX_init();
    bsp_dac_init();
#endif
    
#ifdef _OLED_ON_    
    oled_init();
    printf("oled_init\r\n");
    oled_clear();
    printf("oled_clear\r\n");
    oled_show_str(0, 0, "12345");
    oled_show_str(0, 16, "abcd");
    oled_show_str(0, 32, "00000");
    oled_show_str(0, 48, "test");
#endif    
}

/**
  * @brief 该函数每隔10ms被Systick中断调用1次。
  * @details 详见 bsp_timer.c的定时中断服务程序。一些处理时间要求不严格的任务可
  *          以放在此函数。比如：按键扫描、蜂鸣器鸣叫控制等。
  */
void bsp_RunPer10ms(void)
{
	bsp_KeyScan();		/* 每10ms扫描按键一次 */
}

/**
  * @brief 该函数每隔1ms被Systick中断调用1次。
  * @details 详见 bsp_timer.c的定时中断服务程序。一些需要周期性处理的事务可以放
  *          在此函数。比如：触摸坐标扫描。
  */
void bsp_RunPer1ms(void)
{
}

/**
  * @brief 空闲时执行的函数。
  * @details 一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
  *			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
  */
void bsp_Idle(void)
{
	/* --- 喂狗 */

	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();
}
