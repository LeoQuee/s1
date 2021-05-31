/*
 * @Description:  �ṩȫ��ͨ�õĽӿڵľ���ʵ��
 * @Author: Gaven
 * @Date: 2019-10-22 10:31:58
 * @LastEditTime: 2019-10-24 16:46:48
 * @LastEditors: Gaven
 */
#include "globle.h"

/**
  * @description: ��ֵ�˲���ȥ�������Сֵ��ȡʣ��ֵ�ľ�ֵ
  * @param array ����
  * @param len ���ݳ�����С��3
  * @return ��ֵ�˲���Ľ��
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
  * @description: ������Ϻ���
  *  ����ΪRƽ���ļ��㹫ʽ��
  *      0~0.3 �����
  *      0.3~0.6  �еȳ̶����
  *      0.6~1  ǿ���
  *
  *  �� RSS = ��(yʵ��ֵ - yԤ��ֵ)^2
  *  �� TSS = ��(yʵ��ֵ - yƽ��ֵ)^2
  *
  * R^2 = 1 - RSS / TSS
  * ԭ�ģ�https://www.zhihu.com/question/24393657
  * @param x ����
  * @param y ����
  * @param length Ԫ�ظ���
  * @return ��Ͻ����б�ʡ��ؾࡢ���ϵ����
  */
LEASQ_COEFFICIENT LeastsqLinearFit(float x[], float y[], int32_t length)
{
    int32_t i;
    float mean_x = 0, mean_y = 0, e = 0, f = 0, q = 0, RSS = 0, TSS = 0;
    LEASQ_COEFFICIENT linearModel = {0};
    
    // �����쳣�ж�
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
