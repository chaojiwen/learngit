#ifndef __Mcu_Beep_H
#define	__Mcu_Beep_H

#include "stm32f10x.h"
#include "ucos_ii.h"       //uC/OS-II ϵͳ����ͷ�ļ�  

/************************************************************************************************/
/* ������ ���� ���� ���� */
typedef enum {
	/* ���ź� ���� */
	Null_Signal_Beep_EVENT		= 0X00,
	/* ������ ���� ���� */
	Sensor_Signal_Beep_EVENT 	= 0X01,	
	/* ���� ���� ���� */
	Power_Signal_Beep_EVENT 	= 0X02,	
	/* ��ص��� ���� ���� */
	Battery_Signal_Beep_EVENT 	= 0X03,
	/* �˵�����İ������� */
	Beep_Menu_Key_Sound_EVENT 	= 0X04, 
}BEEP_CMD_TYPE;

typedef struct
{
	u16		Beep_Open_Time;		// ������ �� ʱ�䣨ms��
	u16		Beep_Close_Time;	// ������ �� ʱ�䣨ms��
	BOOL	EVENT_Getarg;		// �¼� ִ�н��(FALSE:�ź�����ִ�У�TRUE:�ͺ�ִ�н���)
}Beep_QUE;
/************************************************************************************************/
#define Beep_ON  1
#define Beep_OFF 0

#define Beep_PORT         GPIOA
#define Beep_CLK          RCC_APB2Periph_GPIOA
#define Beep_PIN          GPIO_Pin_15

#define Beep(a)	if (a)	\
					GPIO_SetBits(Beep_PORT,Beep_PIN);\
					else		\
					GPIO_ResetBits(Beep_PORT,Beep_PIN)				



void Beep_Init(void);		
void Alarm_Audible(void);
BOOL Beep_Send_Q(OS_EVENT* pevent,BEEP_CMD_TYPE EVENT_Type);
					
void Sensor_Alarm_Sound(void);
void Power_Alarm_Sound(void);
void Battery_Alarm_Sound(void);
void Beep_Menu_Key_Sound(void);

#endif /* __Beep_H */
