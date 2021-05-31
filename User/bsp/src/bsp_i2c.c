#include "bsp_i2c.h"

static int s_timeout = 0;

/**
  * @brief i2c初始化
  */
void i2c_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    
    //_PRINT("i2c_init()\r\n");
    
    RCC_APB2PeriphClockCmd(I2C1_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
#ifdef I2C1_REMAP
    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
#endif
    
    // 先设置为高电平，解决总线忙的问题，再设置为复用开漏输出
    GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStructure);
    GPIO_SetBits(I2C1_SDA_PORT, I2C1_SDA_PIN);
    
    GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStructure);
    GPIO_SetBits(I2C1_SCL_PORT, I2C1_SCL_PIN);
    
    GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStructure);
    
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    
    I2C_DeInit(I2C1);
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
    I2C_AcknowledgeConfig(I2C1, ENABLE);   
}

/**
  * @brief 读取数据
  * @param I2Cx
  * @param addr
  * @param data
  * @param len
  * @return 
  */
int i2c_read(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len)
{
    s_timeout = 0;
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    {
        if(s_timeout ++ > 10000)
        {
            return -1;
        }
    }
    
    I2C_GenerateSTART(I2Cx, ENABLE); // 发送一个开始位
    s_timeout = 0;
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) // 等待EV5
    {
        if(s_timeout ++ > 10000)
        {
            return -1;
        }
    }
    
    I2C_Send7bitAddress(I2Cx, addr<<1, I2C_Direction_Receiver); // 发送从地址
    s_timeout = 0;
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) // 等待EV6
    {
        if(s_timeout ++ > 10000)
        {
            return -1;
        }
    }
    
    while(len != 0)
    {
        if(len == 1)
        {
            I2C_AcknowledgeConfig(I2Cx, DISABLE);
            
            I2C_GenerateSTOP(I2Cx, ENABLE);
        }
        s_timeout = 0;
        while(!I2C_CheckEvent(I2Cx, 0x00000040)) // 等待EV7
        {
            if(s_timeout ++ > 10000)
            {
                return -1;
            }
        }
        *data = I2C_ReceiveData(I2Cx);
        
        data++;
        len--;
    }
    
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    
    return 0;
}

/**
  * @brief 写入数据
  * @param I2Cx
  * @param addr
  * @param data
  * @param len
  * @return 
  */
int i2c_write(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len)
{
    for(s_timeout=0; s_timeout<1000; s_timeout++);
    
    s_timeout = 0;
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    {
        if(s_timeout ++ > 10000)
        {
            return -1;
        }
    }
    
    I2C_GenerateSTART(I2Cx, ENABLE); // 发送一个开始位
    s_timeout = 0;
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) // 等待EV5
    {
        if(s_timeout ++ > 10000)
        {
            return -1;
        }
    }
    
    for(s_timeout=0; s_timeout<1000; s_timeout++);
    
    I2C_Send7bitAddress(I2Cx, addr<<1, I2C_Direction_Transmitter); // 发送从地址
    s_timeout = 0;
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) // 等待EV6
    {
        if(s_timeout ++ > 10000)
        {
            return -1;
        }
    }
    
    while(len != 0)
    {
        I2C_SendData(I2Cx, *data);
        s_timeout = 0;
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) // 等待EV8
        {
            if(s_timeout ++ > 10000)
            {
                return -1;
            }
        }
        
        data++;
        len--;
    }
    
    I2C_GenerateSTOP(I2Cx, ENABLE);
    
    return 0;
}

/**
  * @brief i2c初始化
  */
void i2c2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    
    //_PRINT("i2c2_init()\r\n");
    
    RCC_APB2PeriphClockCmd(I2C2_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    
    // 先设置为高电平，解决总线忙的问题，再设置为复用开漏输出
    GPIO_InitStructure.GPIO_Pin = I2C2_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C2_SCL_PORT, &GPIO_InitStructure);
    GPIO_SetBits(I2C2_SCL_PORT, I2C2_SCL_PIN);
    
    GPIO_InitStructure.GPIO_Pin = I2C2_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C2_SDA_PORT, &GPIO_InitStructure);
    GPIO_SetBits(I2C2_SDA_PORT, I2C2_SDA_PIN);
    
    GPIO_InitStructure.GPIO_Pin = I2C2_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C2_SCL_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = I2C2_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C2_SDA_PORT, &GPIO_InitStructure);
    
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    
    I2C_DeInit(I2C2);
    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);
    I2C_AcknowledgeConfig(I2C2, ENABLE);   
}

/**
  * @brief 写入数据
  * @param I2Cx
  * @param addr
  * @param data
  * @param len
  * @return 
  */
int i2c2_write(uint8_t addr, uint8_t *data, uint8_t len)
{
    s_timeout = 0;
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
    {
        if(s_timeout ++ > 10000)
        {
            return -1;
        }
    }
    
    I2C_GenerateSTART(I2C2, ENABLE); // 发送一个开始位
    s_timeout = 0;
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) // 等待EV5
    {
        if(s_timeout ++ > 10000)
        {
            return -1;
        }
    }
    
    I2C_Send7bitAddress(I2C2, addr<<1, I2C_Direction_Transmitter); // 发送从地址
    s_timeout = 0;
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) // 等待EV6
    {
        if(s_timeout ++ > 10000)
        {
            return -1;
        }
    }
    
    while(len != 0)
    {
        I2C_SendData(I2C2, *data);
        s_timeout = 0;
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) // 等待EV8
        {
            if(s_timeout ++ > 10000)
            {
                return -1;
            }
        }
        
        data++;
        len--;
    }
    
    I2C_GenerateSTOP(I2C2, ENABLE);
    
    return 0;
}
