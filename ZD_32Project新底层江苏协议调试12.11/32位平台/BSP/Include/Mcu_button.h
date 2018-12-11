/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��������ģ��	���ο�armfly�����壩	 
*	�ļ����� : Mcu_button.h
*	��    �� : V1.0
*	˵    �� : ʵ�ְ����ļ�⣬��������˲����ƣ����Լ�������¼���
*			  (1) ��������
*			  (2) ��������
*			  (3) ������
*			  (4) ����ʱ�Զ�����
*			  (5) ��ϼ�
*
*	�޸ļ�¼ :
*	    �汾��    ����          ����                 ˵��
*		v1.0    2011-12-07   WildFire Team  ST�̼���汾 V3.5.0�汾��
*
*	Copyright (C), 2012-2013, WildFire Team
*
*********************************************************************************************************
*/

#ifndef __Mcu_button_H
#define __Mcu_button_H

#include "stm32f10x.h"
#include "ucos_ii.h"       //uC/OS-II ϵͳ����ͷ�ļ�  

/********************************* ���� �˿ڶ��� *********************************/
#ifndef Hardware_Version_V131

#define Btn1_PORT              GPIOC
#define Btn1_CLK               RCC_APB2Periph_GPIOC
#define Btn1_PIN               GPIO_Pin_6

#define Btn2_PORT              GPIOC
#define Btn2_CLK               RCC_APB2Periph_GPIOC 
#define Btn2_PIN               GPIO_Pin_8

#define Btn3_PORT              GPIOC
#define Btn3_CLK               RCC_APB2Periph_GPIOC
#define Btn3_PIN               GPIO_Pin_7

#define Btn4_PORT              GPIOC
#define Btn4_CLK               RCC_APB2Periph_GPIOC 
#define Btn4_PIN               GPIO_Pin_9

#define KEY_Power_PORT         GPIOE
#define KEY_Power_CLK          RCC_APB2Periph_GPIOE
#define KEY_Power_PIN          GPIO_Pin_5

#else


#define Btn1_PORT              GPIOC
#define Btn1_CLK               RCC_APB2Periph_GPIOC
#define Btn1_PIN               GPIO_Pin_9

#define Btn2_PORT              GPIOC
#define Btn2_CLK               RCC_APB2Periph_GPIOC 
#define Btn2_PIN               GPIO_Pin_11

#define Btn3_PORT              GPIOC
#define Btn3_CLK               RCC_APB2Periph_GPIOC
#define Btn3_PIN               GPIO_Pin_10

#define Btn4_PORT              GPIOC
#define Btn4_CLK               RCC_APB2Periph_GPIOC 
#define Btn4_PIN               GPIO_Pin_12

#define KEY_Power_PORT         GPIOE
#define KEY_Power_CLK          RCC_APB2Periph_GPIOE
#define KEY_Power_PIN          GPIO_Pin_5

#endif

/* ������Դ �������� */
#define KEY_Power_Open  	{GPIO_ResetBits(KEY_Power_PORT,KEY_Power_PIN);}
#define KEY_Power_Close 	{GPIO_SetBits(KEY_Power_PORT,KEY_Power_PIN);}

/* 
�����˲�ʱ��50ms, ��λ10ms
ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�

ע�⣺������ϵͳ��ʱ����10msִ��һ�Σ����ڲ���ϵͳ�ˣ�20msִ��һ��
*/
#define BUTTON_FILTER_TIME 	2		/* 2*10 = 20ms */
#define BUTTON_LONG_TIME 	100		/* 100*20 = 2s ,��Ϊ�����¼� */
#define BUTTON_Repeat_TIME 	30		/* 30*20 = 600ms ��Ϊ�����¼� */

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
	���Ա������ʵ���˲��Ͷ��ְ���״̬�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t Count;			/* �˲��������� */
	uint8_t FilterTime;		/* �˲�ʱ��(���255,��ʾ2550ms) */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t KeyCodeUp;		/* ��������ļ�ֵ����, 0��ʾ����ⰴ������ */
	uint8_t KeyCodeDown;	/* �������µļ�ֵ����, 0��ʾ����ⰴ������ */
	uint8_t KeyCodeLong;	/* ���������ļ�ֵ����, 0��ʾ����ⳤ�� */
	uint8_t RepeatSpeed;	/* ������������ */
	uint8_t RepeatCount;	/* �������������� */
}BUTTON_T;

/* �����ֵ����
	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2)	�������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_DOWN_Sure,			/* ȷ�� ������ */
	KEY_UP_Sure,			/* ȷ�� ������ */
	KEY_LONG_Sure,			/* ȷ�� ������ */
	
	KEY_DOWN_Forward,		/* ��ǰ ������ */
	KEY_UP_Forward,			/* ��ǰ ������ */	
	KEY_LONG_Forward,		/* ��ǰ ������ */
	
	KEY_DOWN_Backward,		/* ��� ������ */
	KEY_UP_Backward,		/* ��� ������ */	
	KEY_LONG_Backward,		/* ��� ������ */	

	KEY_DOWN_Cancel,		/* ���� ������ */
	KEY_UP_Cancel,			/* ���� ������ */
	KEY_LONG_Cancel,		/* ���� ������ */

// 	KEY_DOWN_USER_TAMPER	/* ��ϼ���USER����WAKEUP��ͬʱ���� */
}KEY_ENUM;

/* ����ID */
enum
{
	KID_Sure = 0,
	KID_Forward,
	KID_Backward,
	KID_Cancel	
};

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	20
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ�� */
	uint8_t Write;					/* ������дָ�� */
}KEY_FIFO_T;


/* ����һ�����䣬    ��ֻ��һ������ָ�룬  OSMboxCreate�����ᴴ������������Դ */
extern OS_EVENT *App_KEY_Mbox;
/* ���ڲ�ѯ����ֵʱ���浱ǰ�ź������ݵ����ݽṹ */
extern OS_MBOX_DATA KEY_Mbox_DATA;

/* ���ⲿ���õĺ������� */
void 	Button_Init			(void);
void 	bsp_PutKey			(uint8_t _KeyCode);
uint8_t Get_Key_State		(void);
void 	Key_State_Check_All	(void);
uint8_t bsp_KeyState		(uint8_t _ucKeyID);


//void 	Circulate_Data_Value_Deal(u32* data_p,u32 up_limit,u32 down_limit,s32 offset);

void bsp_InitButtonHard(void);

#endif



