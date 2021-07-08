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

void i2c_init(void);    /*!< i2c初始化 */
int i2c_read(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len);  /*!< 读取数据 */
int i2c_write(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len); /*!< 写入数据 */

void i2c2_init(void);
int i2c2_write(uint8_t addr, uint8_t *data, uint8_t len);

#endif
