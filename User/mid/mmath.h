#ifndef _MMATH_H_
#define _MMATH_H_

#include "stm32f10x.h"

int16_t get_max(int16_t *data, int len);    /*!< 计算数组最大值 */
int16_t get_min(int16_t *data, int len);    /*!< 计算数组最小值 */

#endif
