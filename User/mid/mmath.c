/**
  ******************************************************************************
  * @file    mmath.c
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */

#include "mid\mmath.h"

/**
  * @brief �����������ֵ
  * @param data ����ָ��
  * @param len ���鳤��
  * @return �������ֵ
  */
int16_t get_max(int16_t *data, int len)
{
    int i;
    int16_t max_data;
    
    if(len < 1) return 0;
    
    for(max_data=data[0], i=1; i<len; i++)
    {
        if(data[i] > max_data)
        {
            max_data = data[i];
        }
    }
    
    return max_data;
}

/**
  * @brief ����������Сֵ
  * @param data ����ָ��
  * @param len ���鳤��
  * @return ������Сֵ
  */
int16_t get_min(int16_t *data, int len)
{
    int i;
    int16_t min_data;
    
    if(len < 1) return 0;
    
    for(min_data=data[0], i=1; i<len; i++)
    {
        if(data[i] < min_data)
        {
            min_data = data[i];
        }
    }
    
    return min_data;
}
