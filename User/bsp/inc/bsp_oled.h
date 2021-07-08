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

void ssd1306_write_reg(uint8_t reg);    /*!< OLEDд��Ĵ��� */
void ssd1306_write_data(uint8_t data);  /*!< OLEDд������ */
void oled_init(void);   /*!< OLED��ʼ�� */
void oled_set_position(uint8_t x, uint8_t y);   /*!< �������� */
void oled_clear(void);  /*!< ���� */
void oled_show_char(uint8_t x, uint8_t y, uint8_t c);   /*!< ��ʾ�ַ� */
void oled_show_str(uint8_t x, uint8_t y, char *str); /*!< ��ʾ�ַ��� */

#endif
