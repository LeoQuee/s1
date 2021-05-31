#ifndef _BSP_MUX508_H_
#define _BSP_MUX508_H_

#include "global_config.h"

typedef enum
{
    TP_NONE = 0,
    TP1 = 1,
    TP2,
    TP3,
    TP4,
    TP5,
    TP6,
    TP7,
    TP8,
    TP9,
    TP10,
    TP11,
    TP12,
    TP13,
    TP14,
    TP15,
    TP16,
    TP17,
    TP18,
    TP19,
    TP20,
    TP21,
    TP22,
    TP23,
    TP24,
    TP25,
    TP26,
    TP27,
    TP28,
    TP29,
    TP30,
    TP31,
    TP32,
} RECT_LATTICE;

void mux508_init(void); /*!< 多路开关初始化 */
void mux508_set(RECT_LATTICE point);    /*!< 设置多路开关 */

#endif
