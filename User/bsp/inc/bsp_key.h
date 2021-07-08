/**
  ******************************************************************************
  * @file    bsp_key.h
  * @author  lik
  * @date    2021-7-8
  * @brief   按键驱动模块
  ******************************************************************************
  */  
 
#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "global_config.h"

#define KEY_NUM     4       /*!< 按键数量 */
#define KEY_LONG    (0x80)  /*!< 长按标志 */
#define KEY_REPEAT  (0x40)  /*!< 连按标志 */

/**
  * @brief 按键滤波时间50ms, 单位10ms。
  * 只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
  * 即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
  */
#define KEY_FILTER_TIME   5

/**
  * @brief 每个按键对应1个全局的结构体变量。
  */
typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t  Count;			/* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
	uint8_t  RepeatSpeed;	/* 连续按键周期 */
	uint8_t  RepeatCount;	/* 连续按键计数器 */
    uint8_t  keyval;        /*!< 键值 */
}KEY_T;

/*
	定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件

	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2) 编译器可帮我们避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_UP,
    KEY_DOWN,
    KEY_MENU,
    KEY_ENTER,

} KEY_ENUM;

/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针 */
	uint8_t Write;					/* 缓冲区写指针 */
}KEY_FIFO_T;


static uint8_t IsKeyDown_DOWN(void);
static uint8_t IsKeyDown_UP(void);
static uint8_t IsKeyDown_MENU(void);
static uint8_t IsKeyDown_ENTER(void);
static void bsp_InitKeyHard(void);
static void bsp_InitKeyVar(void);
void bsp_InitKey(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);
static void bsp_DetectKey(uint8_t i);
void bsp_KeyScan(void);

#endif
