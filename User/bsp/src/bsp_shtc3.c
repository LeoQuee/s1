/**
  ******************************************************************************
  * @file    bsp_shtc3.c
  * @author  lik
  * @date    2021-7-8
  * @brief   
  ******************************************************************************
  */ 

#include "bsp_shtc3.h"
#include "bsp_i2c.h"

#include "bsp_timer.h"

#define I2C_SHTC3   I2C1

#define GENERAL_RESET_CMD   0x0006

#define SHTC3_ADDR  0x70        /*!< 7位地址 */

#define SLEEP_CMD   0xB098
#define WAKEUP_CMD  0x3517
#define RESET_CMD   0x805D
#define READID_CMD  0xEFC8

#ifdef CLOCK_STRETCH_ENABLE
#define TEMP_CMD    0x7CA2
#define HUM_CMD     0x5C24
#else
#define TEMP_CMD    0x7866
#define HUM_CMD     0x58E0
#endif

/**
  * @brief 读取温湿度
  * @param p_temp
  * @param p_hum
  */
void shtc3_readdata(float *p_temp, float *p_hum)
{
    uint8_t data[10];
    
    data[0] = (WAKEUP_CMD>>8)&0xFF;
    data[1] = WAKEUP_CMD&0xFF;
    i2c_write(I2C_SHTC3, SHTC3_ADDR, data, 2);
    
    data[0] = (TEMP_CMD>>8)&0xFF;
    data[1] = TEMP_CMD&0xFF;
    i2c_write(I2C_SHTC3, SHTC3_ADDR, data, 2);
    
    bsp_DelayMS(20);
    
    // T_H T_L CRC H_H H_L CRC
    // CRC8, Polynomial=0x31, Reflect input=false, Reflect output=false
    // Initialization=0xFF, Final XOR=0x00
    // CRC(0x00) = 0xAC
    // CRC(0xBEEF) = 0x92
    i2c_read(I2C_SHTC3, SHTC3_ADDR, data, 6);
    *p_temp = ((data[0]<<8) | data[1]);
    *p_temp = -45 + 175 * (*p_temp) / 65536;
    *p_hum  = (data[3]<<8) | data[4];
    *p_hum = (*p_hum) * 100 / 65536;
    
    data[0] = (SLEEP_CMD>>8)&0xFF;
    data[1] = SLEEP_CMD&0xFF;
    i2c_write(I2C_SHTC3, SHTC3_ADDR, data, 2);
}

/**
  * @brief 读取id
  * @param p_id
  */
void shtc3_readid(int16_t *p_id)
{
    uint8_t data[10];
    
    data[0] = (WAKEUP_CMD>>8)&0xFF;
    data[1] = WAKEUP_CMD&0xFF;
    i2c_write(I2C_SHTC3, SHTC3_ADDR, data, 2);
    
    data[0] = (READID_CMD>>8)&0xFF;
    data[1] = READID_CMD&0xFF;
    i2c_write(I2C_SHTC3, SHTC3_ADDR, data, 2);
    
    i2c_read(I2C_SHTC3, SHTC3_ADDR, data, 2);
    *p_id = (data[0]<<8) | data[1];
    
    data[0] = (SLEEP_CMD>>8)&0xFF;
    data[1] = SLEEP_CMD&0xFF;
    i2c_write(I2C_SHTC3, SHTC3_ADDR, data, 2);
}
