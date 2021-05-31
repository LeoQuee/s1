#include "bsp_oled.h"
#include "bsp_i2c.h"
#include "bsp_oled_font.h"

#define P_X     128
#define P_Y     64

#define _SSD_1306_

#define I2C_SSD1306             I2C2

#define SSD1306_ADDR            0x3C // 7位地址，D/C#接地为0x3C, D/C#引脚接VCC为0x3D

#define CTRL_BYTE_CMD           0x00 // 写命令
#define CTRL_BYTE_DATA          0x40 // 写数据

#define REG_PAGE_COL_L          0x00 // 0x00~0x0F,列低4位（默认为0）
#define REG_PAGE_COL_H          0x10 // 0x10~0x1F,列高4位（默认为0）
#define REG_RAMADDR_MODE        0x20 // 内存地址模式,data[0]:00b-水平地址模式，01b-垂直地址模式，10b-页地址模式（默认）
#define REG_COL                 0x21 // 设置列地址，data[0]:开始地址，0~127,默认为0，data[1]:结束地址，0~127,默认为127
#define REG_PAGE                0x22 // 设置页地址，data[0]:开始地址，0~7,默认为0，data[1]:结束地址，0~7,默认为7
#define REG_ROLL_RIGHT          0x26 // 向右滚动，data[0]:空字节，data[1]:开始页地址，data[2]:时间间隔,data[3]:结束页地址
#define REG_ROLL_LEFT           0x27 // 向左滚动，data[0]:空字节，data[1]:开始页地址，data[2]:时间间隔,data[3]:结束页地址
#define REG_ROLL_HRIGHT         0x29 // 垂直和向右滚动，data[0]:空字节，data[1]:开始页地址，data[2]:时间间隔,data[3]:结束页地址,data[4]:垂直滚屏位移
#define REG_ROLL_HLEFT          0x2A // 垂直和向左滚动，data[0]:空字节，data[1]:开始页地址，data[2]:时间间隔,data[3]:结束页地址,data[4]:垂直滚屏位移
#define REG_ROLL_OFF            0x2E // 关闭滚屏
#define REG_ROLL_ON             0x2F // 开始滚屏
#define REG_START_LINE          0x40 // 0x40~0x7F，设置显示起始行
#define REG_CONTRAST            0x81 // 对比度，data[0]:1~256， 默认为7FH
#define REG_SEG_REMAP_OFF       0xA0 // 0列作为SEG0（默认）
#define REG_SEG_REMAP_ON        0xA1 // 127列作为SEG0（默认）
#define REG_ROLL_HAREA          0xA3 // 设置垂直滚动区域，data[0]:顶层固定的行数，data[1]:滚动区域的行数
#define REG_ENTIRE_DISPLAY_OFF  0xA4 // 整体显示开启状态，输出跟随RAM（默认）
#define REG_ENTIRE_DISPLAY_ON   0xA5 // 整体显示开启状态，输出不跟随RAM
#define REG_INVERSE_OFF         0xA6 // RAM中0不显示，1显示（默认）
#define REG_INVERSE_ON          0xA7 // RAM中0显示，1不显示
#define REG_MUX_RATIO           0xA8 // 复合比率，data[0]:15~63
#define REG_DISPLAY_OFF         0xAE // 显示关（睡眠模式，默认）
#define REG_DISPLAY_ON          0xAF // 显示开
#define REG_PAGE_ADDR           0xB0 // 0xB0~0xB7，选择页地址模式的页地址
#define REG_COM_DIRECTION_OFF   0xC0 // COM0->COM[N-1](默认)
#define REG_COM_DIRECTION_ON    0xC8 // COM[N-1]->COM0
#define REG_DISP_OFFSET         0xD3 // 显示偏移，data[0]:0~63,默认为0
#define REG_DISP_FREQ           0xD5 // 设置显示时钟分频率，振荡器频率，data[0],默认1分频
#define REG_PRE_CHARGE          0xD9 // 设置重充电周期 data[0],
#define REG_COM_CFG             0xDA // COM引脚配置 data[0]
#define REG_V_COMH              0xDB // data[0]
#define REG_NOP                 0xE3

/**
  * @brief OLED写入寄存器
  * @param reg
  */
void ssd1306_write_reg(uint8_t reg)
{
    uint8_t temp[2];
    
    temp[0] = CTRL_BYTE_CMD;
    temp[1] = reg;
    
    i2c2_write(SSD1306_ADDR, temp, 2);
}

/**
  * @brief OLED写入数据
  * @param data 
  */
void ssd1306_write_data(uint8_t data)
{
    uint8_t temp[2];
    
    temp[0] = CTRL_BYTE_DATA;
    temp[1] = data;
    
    i2c2_write(SSD1306_ADDR, temp, 2);
}

/**
  * @brief OLED初始化
  */
void oled_init(void)
{
    i2c2_init();
    
    ssd1306_write_reg(REG_DISPLAY_OFF); // 0xAE
    ssd1306_write_reg(REG_PAGE_COL_L); // 0x00
    ssd1306_write_reg(REG_PAGE_COL_H); // 0x10
    ssd1306_write_reg(REG_START_LINE); // 0x40
    ssd1306_write_reg(REG_PAGE_ADDR);  // 0xB0
    
    ssd1306_write_reg(REG_CONTRAST); // 0x81
    ssd1306_write_reg(0x66);
    
    ssd1306_write_reg(REG_SEG_REMAP_ON); // 0xA1
    ssd1306_write_reg(REG_INVERSE_OFF); // 0xA6
    
    ssd1306_write_reg(REG_MUX_RATIO); // 0xA8 
    ssd1306_write_reg(0x3F);
    
    ssd1306_write_reg(REG_COM_DIRECTION_ON); // 0xC8
    
    ssd1306_write_reg(REG_DISP_OFFSET); // 0xD3
    ssd1306_write_reg(0x00);
    
    ssd1306_write_reg(REG_DISP_FREQ); // 0xD5
    ssd1306_write_reg(0x80);
    
    ssd1306_write_reg(REG_PRE_CHARGE); // 0xD9
    ssd1306_write_reg(0x1f);
    
    ssd1306_write_reg(REG_COM_CFG); // 0xDA
    ssd1306_write_reg(0x12);
    
    ssd1306_write_reg(REG_V_COMH); // 0xDB
    ssd1306_write_reg(0x30);
    
    ssd1306_write_reg(0x8D);
    ssd1306_write_reg(0x14);
    
    ssd1306_write_reg(REG_DISPLAY_ON); // 0xAF 打开显示
}

/**
  * @brief 设置坐标
  * @param x
  * @param y
  */
void oled_set_position(uint8_t x, uint8_t y)
{
    ssd1306_write_reg(REG_PAGE_ADDR+y/8);
    ssd1306_write_reg(REG_PAGE_COL_L+(x&0x0F));
    ssd1306_write_reg(REG_PAGE_COL_H+((x&0xF0)>>4));
}

/**
  * @brief 清屏
  */
void oled_clear(void)
{
    uint8_t i, j;
    
    for(i=0; i<P_Y; i+=8)
    {
        oled_set_position(0, i);
        for(j=0; j<P_X; j++)
        {
            ssd1306_write_data(0x00);
        }
    }
}

/**
  * @brief 显示字符
  * @param x
  * @param y
  * @param c
  */
void oled_show_char(uint8_t x, uint8_t y, uint8_t c)
{
    int i;
    
    oled_set_position(x, y);
    for(i=0; i<8; i++)
    {
        ssd1306_write_data(code_F8x16[(c-' ')*16+i]);
    }
    oled_set_position(x, y+8);
    for(i=0; i<8; i++)
    {
        ssd1306_write_data(code_F8x16[(c-' ')*16+i+8]);
    }
}

/**
  * @brief 显示字符串
  * @param x
  * @param y
  * @param str
  */
void oled_show_str(uint8_t x, uint8_t y, char *str)
{
    while(*str != '\0')
    {
        oled_show_char(x, y, *str);
        
        str++;
        x+=8;
        
        if(x > 120)
        {
            x = 0;
            y += 16;
        }
    }
}
