/**
  ******************************************************************************
  * @file    task.h
  * @author  lik
  * @date    2021-7-8
  * @brief   任务
  ******************************************************************************
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
