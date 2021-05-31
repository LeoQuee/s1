#include "bsp_oled.h"
#include "bsp_i2c.h"
#include "bsp_oled_font.h"

#define P_X     128
#define P_Y     64

#define _SSD_1306_

#define I2C_SSD1306             I2C2

#define SSD1306_ADDR            0x3C // 7λ��ַ��D/C#�ӵ�Ϊ0x3C, D/C#���Ž�VCCΪ0x3D

#define CTRL_BYTE_CMD           0x00 // д����
#define CTRL_BYTE_DATA          0x40 // д����

#define REG_PAGE_COL_L          0x00 // 0x00~0x0F,�е�4λ��Ĭ��Ϊ0��
#define REG_PAGE_COL_H          0x10 // 0x10~0x1F,�и�4λ��Ĭ��Ϊ0��
#define REG_RAMADDR_MODE        0x20 // �ڴ��ַģʽ,data[0]:00b-ˮƽ��ַģʽ��01b-��ֱ��ַģʽ��10b-ҳ��ַģʽ��Ĭ�ϣ�
#define REG_COL                 0x21 // �����е�ַ��data[0]:��ʼ��ַ��0~127,Ĭ��Ϊ0��data[1]:������ַ��0~127,Ĭ��Ϊ127
#define REG_PAGE                0x22 // ����ҳ��ַ��data[0]:��ʼ��ַ��0~7,Ĭ��Ϊ0��data[1]:������ַ��0~7,Ĭ��Ϊ7
#define REG_ROLL_RIGHT          0x26 // ���ҹ�����data[0]:���ֽڣ�data[1]:��ʼҳ��ַ��data[2]:ʱ����,data[3]:����ҳ��ַ
#define REG_ROLL_LEFT           0x27 // ���������data[0]:���ֽڣ�data[1]:��ʼҳ��ַ��data[2]:ʱ����,data[3]:����ҳ��ַ
#define REG_ROLL_HRIGHT         0x29 // ��ֱ�����ҹ�����data[0]:���ֽڣ�data[1]:��ʼҳ��ַ��data[2]:ʱ����,data[3]:����ҳ��ַ,data[4]:��ֱ����λ��
#define REG_ROLL_HLEFT          0x2A // ��ֱ�����������data[0]:���ֽڣ�data[1]:��ʼҳ��ַ��data[2]:ʱ����,data[3]:����ҳ��ַ,data[4]:��ֱ����λ��
#define REG_ROLL_OFF            0x2E // �رչ���
#define REG_ROLL_ON             0x2F // ��ʼ����
#define REG_START_LINE          0x40 // 0x40~0x7F��������ʾ��ʼ��
#define REG_CONTRAST            0x81 // �Աȶȣ�data[0]:1~256�� Ĭ��Ϊ7FH
#define REG_SEG_REMAP_OFF       0xA0 // 0����ΪSEG0��Ĭ�ϣ�
#define REG_SEG_REMAP_ON        0xA1 // 127����ΪSEG0��Ĭ�ϣ�
#define REG_ROLL_HAREA          0xA3 // ���ô�ֱ��������data[0]:����̶���������data[1]:�������������
#define REG_ENTIRE_DISPLAY_OFF  0xA4 // ������ʾ����״̬���������RAM��Ĭ�ϣ�
#define REG_ENTIRE_DISPLAY_ON   0xA5 // ������ʾ����״̬�����������RAM
#define REG_INVERSE_OFF         0xA6 // RAM��0����ʾ��1��ʾ��Ĭ�ϣ�
#define REG_INVERSE_ON          0xA7 // RAM��0��ʾ��1����ʾ
#define REG_MUX_RATIO           0xA8 // ���ϱ��ʣ�data[0]:15~63
#define REG_DISPLAY_OFF         0xAE // ��ʾ�أ�˯��ģʽ��Ĭ�ϣ�
#define REG_DISPLAY_ON          0xAF // ��ʾ��
#define REG_PAGE_ADDR           0xB0 // 0xB0~0xB7��ѡ��ҳ��ַģʽ��ҳ��ַ
#define REG_COM_DIRECTION_OFF   0xC0 // COM0->COM[N-1](Ĭ��)
#define REG_COM_DIRECTION_ON    0xC8 // COM[N-1]->COM0
#define REG_DISP_OFFSET         0xD3 // ��ʾƫ�ƣ�data[0]:0~63,Ĭ��Ϊ0
#define REG_DISP_FREQ           0xD5 // ������ʾʱ�ӷ�Ƶ�ʣ�����Ƶ�ʣ�data[0],Ĭ��1��Ƶ
#define REG_PRE_CHARGE          0xD9 // �����س������ data[0],
#define REG_COM_CFG             0xDA // COM�������� data[0]
#define REG_V_COMH              0xDB // data[0]
#define REG_NOP                 0xE3

/**
  * @brief OLEDд��Ĵ���
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
  * @brief OLEDд������
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
  * @brief OLED��ʼ��
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
    
    ssd1306_write_reg(REG_DISPLAY_ON); // 0xAF ����ʾ
}

/**
  * @brief ��������
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
  * @brief ����
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
  * @brief ��ʾ�ַ�
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
  * @brief ��ʾ�ַ���
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
