/*
 * @Description:  全局通用的头文件
 * @Author: Gaven
 * @Date: 2019-10-22 10:34:00
 * @LastEditTime: 2019-11-20 10:09:48
 * @LastEditors: Please set LastEditors
 */
#ifndef _GLOBLE_H_
#define _GLOBLE_H_

/**************标准库**************/
#include <math.h>
#include <stdlib.h>
#include <string.h>

/**********通信协议***************/
#include "AppLayer.h"
#include "protocol.h"

typedef enum
{
    AFE_SUCCESS         = 0,    //成功
    AFE_FAILED          = 1,    //失败
    AFE_EXCUTING        = 2     //正在执行中
}AFE_STATE_ERROR_TYPE;

#define		PI				(3.14159265f)

// mag = coeff_m * voltage + coeff_o
typedef struct
{
    float coeff_m;
    float coeff_o;
    float R2;
} LEASQ_COEFFICIENT;

float MeanFiltering(int16_t array[], int32_t max_len);
LEASQ_COEFFICIENT LeastsqLinearFit(float x[], float y[], int32_t length);

#endif
