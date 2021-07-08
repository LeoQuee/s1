/**
  ******************************************************************************
  * @file    bsp_key.h
  * @author  lik
  * @date    2021-7-8
  * @brief   ��������ģ��
  ******************************************************************************
  */  
 
#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "global_config.h"

#define KEY_NUM     4       /*!< �������� */
#define KEY_LONG    (0x80)  /*!< ������־ */
#define KEY_REPEAT  (0x40)  /*!< ������־ */

/**
  * @brief �����˲�ʱ��50ms, ��λ10ms��
  * ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
  * ��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
  */
#define KEY_FILTER_TIME   5

/**
  * @brief ÿ��������Ӧ1��ȫ�ֵĽṹ�������
  */
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
    uint8_t  keyval;        /*!< ��ֵ */
}KEY_T;

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_UP,
    KEY_DOWN,
    KEY_MENU,
    KEY_ENTER,

} KEY_ENUM;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ�� */
	uint8_t Write;					/* ������дָ�� */
}KEY_FIFO_T;


static uint8_t IsKeyDown_DOWN(void);
static uint8_t IsKeyDown_UP(void);
static uint8_t IsKeyDown_MENU(void);
static uint8_t IsKeyDown_ENTER(void);
static void bsp_InitKeyHard(void);
static void bsp_InitKeyVar(void);
void bsp_InitKey(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);
static void bsp_DetectKey(uint8_t i);
void bsp_KeyScan(void);

#endif
