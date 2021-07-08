/**
  ******************************************************************************
  * @file    bsp_i2c.h
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */  

#ifndef _BSP_I2C_H_
#define _BSP_I2C_H_

#include "global_config.h"

void i2c_init(void);    /*!< i2c��ʼ�� */
int i2c_read(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len);  /*!< ��ȡ���� */
int i2c_write(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len); /*!< д������ */

void i2c2_init(void);
int i2c2_write(uint8_t addr, uint8_t *data, uint8_t len);

#endif
