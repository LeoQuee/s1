/*
 * @Description:  
 * @Author: Gaven
 * @Date: 2019-10-21 16:14:20
 * @LastEditTime: 2019-10-29 11:46:59
 * @LastEditors: Gaven
 */
#ifndef _TASK_H_
#define _TASK_H_

#include "global_config.h"

#include "afeStateMachine.h"
#include "bsp.h"
#include "globle.h"

/**
  * @brief 任务处理
  */
void TaskProcess(void);

static void ProcessSoftTimer(void);
static void ProcessKeyEvent(void);
static void ProcessAfeStateMonitor(void);

#endif
