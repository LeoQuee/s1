/*
 * @Description:  ������������ģ��
 *                  ɨ�������������������˲����ƣ����а���FIFO�����Լ�������¼���
 *                      (1) ��������
 *				        (2) ��������
 *				        (3) ������
 *				        (4) ����ʱ�Զ�����
 *
 * ��ʵ�ֶ̰���������������⣻��ϼ���˫����δʵ�֡�
 */

#include "bsp.h"

static KEY_T s_tBtn[KEY_NUM];
static KEY_FIFO_T s_tKey;		/* ����FIFO����,�ṹ�� */

/**
 * @brief DOWN�����Ƿ���
 * @return: 1 ��ʾ���£� 0��ʾδ����
 */
static uint8_t IsKey1Down(void)
{
#if (KEY_CONFIG == 1) && (defined KEY1_PORT)
    if(GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == Bit_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/**
 * @brief UP�����Ƿ���
 * @return: 1 ��ʾ���£� 0��ʾδ����
 */
static uint8_t IsKey2Down(void)
{
#if (KEY_CONFIG == 1) && (defined KEY2_PORT)
    if(GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN) == Bit_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/**
 * @brief MENU�����Ƿ���
 * @return: 1 ��ʾ���£� 0��ʾδ����
 */
static uint8_t IsKey3Down(void)
{
#if (KEY_CONFIG == 1) && (defined KEY3_PORT)
    if(GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == Bit_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/**
 * @brief ENTER�����Ƿ���
 * @return: 1 ��ʾ���£� 0��ʾδ����
 */
static uint8_t IsKey4Down(void)
{
#if (KEY_CONFIG == 1) && (defined KEY4_PORT)
    if(GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN) == Bit_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/**
  * @brief ��ʼ���������IO��
  */
static void bsp_InitKeyHard(void)
{
#if KEY_CONFIG == 1
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ��1������GPIOʱ�� */
    RCC_APB2PeriphClockCmd(KEY_RCC, ENABLE);

    /* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* ���븡��ģʽ */

    /* ��3��������ִ�����ú��� */
#ifdef     KEY1_PORT
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);
#endif    

#ifdef     KEY2_PORT
    GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
    GPIO_Init(KEY2_PORT, &GPIO_InitStructure);
#endif
    
#ifdef KEY3_PORT
    GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
    GPIO_Init(KEY3_PORT, &GPIO_InitStructure);
#endif

#ifdef KEY4_PORT
    GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
    GPIO_Init(KEY4_PORT, &GPIO_InitStructure);
#endif    
#endif
}

/**
  * @brief ��ʼ��������ز���
  */
static void bsp_InitKeyVar(void)
{
    uint8_t i;

    /* �԰���FIFO��дָ������ */
    s_tKey.Read = 0;
    s_tKey.Write = 0;

    /* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
    for (i = 0; i < KEY_NUM; i++)
    {
        s_tBtn[i].LongTime = 100;	    // ��ⳤ����1s
        s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
        s_tBtn[i].State = 0;        // δ����
        s_tBtn[i].RepeatSpeed = 10;  // �������
        s_tBtn[i].RepeatCount = 0;	// ����������
    }

    /* �жϰ������µĺ��� */
#ifdef PCB_V3
    s_tBtn[0].IsKeyDownFunc = IsKey4Down;
    s_tBtn[0].keyval = KEY_DOWN;
    
    s_tBtn[1].IsKeyDownFunc = IsKey1Down;
    s_tBtn[1].keyval = KEY_UP;
    
    s_tBtn[2].IsKeyDownFunc = IsKey3Down;
    s_tBtn[2].keyval = KEY_MENU;
    
    s_tBtn[3].IsKeyDownFunc = IsKey2Down;
    s_tBtn[3].keyval = KEY_ENTER;
#else    
    if(s_pcb_verion == PCB_FINGER)
    {
        s_tBtn[0].IsKeyDownFunc = IsKey3Down;
        s_tBtn[0].keyval = KEY_DOWN;
        
        s_tBtn[1].IsKeyDownFunc = IsKey2Down;
        s_tBtn[1].keyval = KEY_UP;
        
        s_tBtn[2].IsKeyDownFunc = IsKey1Down;
        s_tBtn[2].keyval = KEY_MENU;
        
        s_tBtn[3].IsKeyDownFunc = IsKey4Down;
        s_tBtn[3].keyval = KEY_ENTER;

    }
    else
    {
        s_tBtn[0].IsKeyDownFunc = IsKey1Down;
        s_tBtn[0].keyval = KEY_DOWN;
        
        s_tBtn[1].IsKeyDownFunc = IsKey3Down;
        s_tBtn[1].keyval = KEY_UP;
        
        s_tBtn[2].IsKeyDownFunc = IsKey4Down;
        s_tBtn[2].keyval = KEY_MENU;
        
        s_tBtn[3].IsKeyDownFunc = IsKey2Down;
        s_tBtn[3].keyval = KEY_ENTER;
    }
#endif    
}

/**
  * @brief ������ʼ��
  */
void bsp_InitKey(void)
{
    _PRINT("bsp_InitKey()\r\n");
    bsp_InitKeyVar();		/* ��ʼ���������� */
    bsp_InitKeyHard();		/* ��ʼ������Ӳ�� */
}

/**
  * @brief ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
  * @param _KeyCode ��������
  */
void bsp_PutKey(uint8_t _KeyCode)
{
    s_tKey.Buf[s_tKey.Write] = _KeyCode;

    if (++s_tKey.Write  >= KEY_FIFO_SIZE)
    {
        s_tKey.Write = 0;
    }
}

/**
  * @brief �Ӱ���FIFO��������ȡһ����ֵ��
  * @retval ��������
  */
uint8_t bsp_GetKey(void)
{
    uint8_t ret;

    if (s_tKey.Read == s_tKey.Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = s_tKey.Buf[s_tKey.Read];

        if (++s_tKey.Read >= KEY_FIFO_SIZE)
        {
            s_tKey.Read = 0;
        }
        
        return ret;
    }
}

/**
  * @brief ���ð�������
  * @param _ucKeyID     ����ID����0��ʼ
  * @param _LongTime    �����¼�ʱ��
  * @param _RepeatSpeed �����ٶ�
  */
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
    s_tBtn[_ucKeyID].LongTime = _LongTime;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
    s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;	/* �����������ٶȣ�0��ʾ��֧������ */
    s_tBtn[_ucKeyID].RepeatCount = 0;				/* ���������� */
}


/**
  * @brief ��հ���FIFO������
  */
void bsp_ClearKey(void)
{
    s_tKey.Read = s_tKey.Write;
}

/**
  * @brief ���һ��������
  * @details ������״̬�����뱻�����Եĵ��á�
  * @param i �ڼ�������
  */
static void bsp_DetectKey(uint8_t i)
{
    KEY_T *pBtn;

    pBtn = &s_tBtn[i];
    
    if (pBtn->IsKeyDownFunc()) // ��ǰ�����Ƿ���
    {
        if (pBtn->Count < KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if(pBtn->Count < 2 * KEY_FILTER_TIME)
        {
            pBtn->Count++;
        }
        else
        {
            if (pBtn->State == 0)
            {
                pBtn->State = 1;

                /* ���Ͱ�ť���µ���Ϣ */
                bsp_PutKey(pBtn->keyval);
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    /* ���Ͱ�ť�������µ���Ϣ */
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        /* ��ֵ���밴��FIFO */
                        bsp_PutKey(pBtn->keyval | KEY_LONG);
                    }
                }
                else
                {
                    if (pBtn->RepeatSpeed > 0)
                    {
                        if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
                        {
                            pBtn->RepeatCount = 0;
                            /* ��������ÿ��10ms����1������ */
                            bsp_PutKey(pBtn->keyval | KEY_REPEAT);
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(pBtn->Count > KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if(pBtn->Count != 0)
        {
            pBtn->Count--;
        }
        else
        {
            if (pBtn->State == 1)
            {
                pBtn->State = 0;
            }
        }

        pBtn->LongCount = 0;
        pBtn->RepeatCount = 0;
    }
}

/**
  * @brief ɨ�����а���
  * @details systick�ж��е��ã�ÿ10msִ��һ�Ρ�
  */
void bsp_KeyScan(void)
{
    uint8_t i;

    for (i = 0; i < KEY_NUM; i++)
    {
        bsp_DetectKey(i);
    }
}
