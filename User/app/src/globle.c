/*
 * @Description:  提供全局通用的接口的具体实现
 * @Author: Gaven
 * @Date: 2019-10-22 10:31:58
 * @LastEditTime: 2019-10-24 16:46:48
 * @LastEditors: Gaven
 */
#include "globle.h"

/**
  * @description: 均值滤波，去除最大最小值后，取剩余值的均值
  * @param array 数组
  * @param len 数据长不能小于3
  * @return 均值滤波后的结果
  */
float MeanFiltering(int16_t array[], int32_t len)
{
    int32_t i;
    float sum, max, min, res;

    if(len < 3)
    {
        return 0;
    }
    
    sum = 0;
    max = min = array[0];
    for(i=0; i < len; i++)
    {
        sum += array[i];
        if(array[i] > max) max = array[i];
        if(array[i] < min) min = array[i];
    }
    
    res = (sum - max - min) / (len - 2);
    
    return res;
}

/**
  * @description: 线性拟合函数
  *  下列为R平方的计算公式：
  *      0~0.3 弱相关
  *      0.3~0.6  中等程度相关
  *      0.6~1  强相关
  *
  *  ● RSS = Σ(y实际值 - y预测值)^2
  *  ● TSS = Σ(y实际值 - y平均值)^2
  *
  * R^2 = 1 - RSS / TSS
  * 原文：https://www.zhihu.com/question/24393657
  * @param x 数组
  * @param y 数组
  * @param length 元素个数
  * @return 拟合结果（斜率、截距、相关系数）
  */
LEASQ_COEFFICIENT LeastsqLinearFit(float x[], float y[], int32_t length)
{
    int32_t i;
    float mean_x = 0, mean_y = 0, e = 0, f = 0, q = 0, RSS = 0, TSS = 0;
    LEASQ_COEFFICIENT linearModel = {0};
    
    // 增加异常判断
    if(length == 0)
    {
        return linearModel;
    }

    for(i = 0; i < length; i++)
    {
        mean_x += x[i];
        mean_y += y[i];
    }
    mean_x /= length;
    mean_y /= length;

    for (i = 0;  i < length; i++)
    {
        q = x[i] - mean_x;
        e = e + q * q;
        f = f + q * (y[i] -mean_y);
    }

    linearModel.coeff_m = f / e;
    linearModel.coeff_o = mean_y -linearModel.coeff_m * mean_x;

    for(i = 0; i < length; i++)
    {
        f = y[i] - linearModel.coeff_m * x[i] - linearModel.coeff_o;
        RSS += f * f;
        TSS += (y[i] - mean_y) * (y[i] - mean_y);
    }

    linearModel.R2 = 1 - RSS / TSS;

    return linearModel;
}
