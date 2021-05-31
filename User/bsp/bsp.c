/*
 * @Description:  ����Ӳ���ײ���������ģ������ļ�����Ҫ�ṩ bsp_Init()��������������á��������ÿ��c�ļ������ڿ�
 *			        ͷ��� #include "bsp.h" ���������е���������ģ�顣
 * @Author: Gaven
 * @Date: 2019-10-21 10:34:45
 * @LastEditTime: 2019-10-30 08:48:01
 * @LastEditors: Gaven
 */
 
#include "bsp.h"
#include "bsp_oled.h"
    
/**
  * @brief ��ʼ��Ӳ���豸��
  * @details ֻ��Ҫ����һ�Ρ��ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����
  */
void bsp_Init(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    init_version_bit();
    bsp_InitTimer();
    bsp_InitUart();
    bsp_InitKey();
	bsp_Afe4300Init();
    mux508_init();
    led_init();
    i2c_init();
    bsp_DelayMS(200);
    
#ifdef PCB_V3    
    W25QXX_init();
    bsp_dac_init();
#endif
    
#ifdef _OLED_ON_    
    oled_init();
    printf("oled_init\r\n");
    oled_clear();
    printf("oled_clear\r\n");
    oled_show_str(0, 0, "12345");
    oled_show_str(0, 16, "abcd");
    oled_show_str(0, 32, "00000");
    oled_show_str(0, 48, "test");
#endif    
}

/**
  * @brief �ú���ÿ��10ms��Systick�жϵ���1�Ρ�
  * @details ��� bsp_timer.c�Ķ�ʱ�жϷ������һЩ����ʱ��Ҫ���ϸ�������
  *          �Է��ڴ˺��������磺����ɨ�衢���������п��Ƶȡ�
  */
void bsp_RunPer10ms(void)
{
	bsp_KeyScan();		/* ÿ10msɨ�谴��һ�� */
}

/**
  * @brief �ú���ÿ��1ms��Systick�жϵ���1�Ρ�
  * @details ��� bsp_timer.c�Ķ�ʱ�жϷ������һЩ��Ҫ�����Դ����������Է�
  *          �ڴ˺��������磺��������ɨ�衣
  */
void bsp_RunPer1ms(void)
{
}

/**
  * @brief ����ʱִ�еĺ�����
  * @details һ����������for��whileѭ������������Ҫ���� CPU_IDLE() �������ñ�������
  *			 ������ȱʡΪ�ղ������û��������ι��������CPU��������ģʽ�Ĺ��ܡ�
  */
void bsp_Idle(void)
{
	/* --- ι�� */

	/* --- ��CPU�������ߣ���Systick��ʱ�жϻ��ѻ��������жϻ��� */

	/* ���� emWin ͼ�ο⣬���Բ���ͼ�ο���Ҫ����ѯ���� */
	//GUI_Exec();
}
