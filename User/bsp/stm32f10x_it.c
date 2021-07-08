/**
  ******************************************************************************
  * @file    stm32f10x_it.c
  * @author  lik
  * @date    2021-7-8
  * @brief   中断模块。为了便于他人了解程序用到的中断，本文件存放所有的中断服务函数。
  *          我们只需要添加需要的中断函数即可。一般中断函数名是固定的，除非修改了启动
  *          文件：Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm\
  *          startup_stm32f10x_hd.s
  *          启动文件是汇编语言文件，定了每个中断的服务函数，这些函数使用了WEAK 关键
  *          字，表示弱定义，因此如果我们在c文件中重定义了该服务函数（必须和它同名），
  *          那么启动文件的中断函数将自动无效。这也就函数重定义的概念，这和C++中的函
  *          数重载的意义类似。
  *
  ******************************************************************************
  */ 

#include "stm32f10x_it.h"

#define ERR_INFO "\r\nEnter HardFault_Handler, System Halt.\r\n"

/*
********************************************************************************
*	Cortex-M3 内核异常中断服务程序
********************************************************************************
*/

/*
********************************************************************************
*	函 数 名: NMI_Handler
*	功能说明: 不可屏蔽中断服务程序。
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/  
void NMI_Handler(void)
{
}

/*
********************************************************************************
*	函 数 名: HardFault_Handler
*	功能说明: 硬件失效中断服务程序。
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/ 
void HardFault_Handler(void)
{
#if 0
  const char *pError = ERR_INFO;
  uint8_t i;

  for (i = 0; i < sizeof(ERR_INFO); i++)
  {
     USART1->DR = pError[i];
     /* 等待发送结束 */
     while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
  }
#endif	
  
  /* 当硬件失效异常发生时进入死循环 */
  while (1)
  {
  }
}

/*
********************************************************************************
*	函 数 名: MemManage_Handler
*	功能说明: 内存管理异常中断服务程序。
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/   
void MemManage_Handler(void)
{
  /* 当内存管理异常发生时进入死循环 */
  while (1)
  {
  }
}

/*
********************************************************************************
*	函 数 名: BusFault_Handler
*	功能说明: 总线访问异常中断服务程序。
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/    
void BusFault_Handler(void)
{
  /* 当总线异常时进入死循环 */
  while (1)
  {
  }
}

/*
********************************************************************************
*	函 数 名: UsageFault_Handler
*	功能说明: 未定义的指令或非法状态中断服务程序。
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/   
void UsageFault_Handler(void)
{
  /* 当用法异常时进入死循环 */
  while (1)
  {
  }
}

/*
********************************************************************************
*	函 数 名: SVC_Handler
*	功能说明: 通过SWI指令的系统服务调用中断服务程序。
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/   
void SVC_Handler(void)
{
}

/*
********************************************************************************
*	函 数 名: DebugMon_Handler
*	功能说明: 调试监视器中断服务程序。
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/   
void DebugMon_Handler(void)
{
}

/*
********************************************************************************
*	函 数 名: PendSV_Handler
*	功能说明: 可挂起的系统服务调用中断服务程序。
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/     
void PendSV_Handler(void)
{
}


/*
********************************************************************************
*	STM32F10x内部外设中断服务程序
*	用户在此添加用到外设中断服务函数。有效的中断服务函数名请参考启动文件(startup_stm32f10x_xx.s)
********************************************************************************
*/

/*
********************************************************************************
*	函 数 名: PPP_IRQHandler
*	功能说明: 外设中断服务程序。
*	形    参：无
*	返 回 值: 无
********************************************************************************
*/    
/* 
	因为中断服务程序往往和具体的应用有关，会用到用户功能模块的变量、函数。如果在本文件展开，会增加大量的
	外部变量声明或者include语句。
	
	因此，这个地方只写一个调用语句，中断服务函数的本体放到对应的用户功能模块中。
	增加一层调用会降低代码的执行效率，不过宁愿损失这个效率，从而增强程序的模块化特性。
	
	增加extern关键字，直接引用用到的外部函数，避免在文件头include其他模块的头文件
extern void ppp_ISR(void);	
void PPP_IRQHandler(void)
{
	ppp_ISR();
}
*/

extern void SysTick_ISR(void);
void SysTick_Handler(void)
{
	SysTick_ISR();
}
