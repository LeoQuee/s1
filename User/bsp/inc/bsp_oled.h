/**
  ******************************************************************************
  * @file    bsp_oled.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#ifndef _BSP_OLED_H_
#define _BSP_OLED_H_

#include "global_config.h"

void ssd1306_write_reg(uint8_t reg);    /*!< OLED写入寄存器 */
void ssd1306_write_data(uint8_t data);  /*!< OLED写入数据 */
void oled_init(void);   /*!< OLED初始化 */
void oled_set_position(uint8_t x, uint8_t y);   /*!< 设置坐标 */
void oled_clear(void);  /*!< 清屏 */
void oled_show_char(uint8_t x, uint8_t y, uint8_t c);   /*!< 显示字符 */
void oled_show_str(uint8_t x, uint8_t y, char *str); /*!< 显示字符串 */

#endif
