#ifndef __Mcu_Beep_H
#define	__Mcu_Beep_H

#include "stm32f10x.h"
#include "ucos_ii.h"       //uC/OS-II 系统函数头文件  

/************************************************************************************************/
/* 蜂鸣器 队列 数据 类型 */
typedef enum {
	/* 无信号 类型 */
	Null_Signal_Beep_EVENT		= 0X00,
	/* 传感器 报警 类型 */
	Sensor_Signal_Beep_EVENT 	= 0X01,	
	/* 掉电 报警 类型 */
	Power_Signal_Beep_EVENT 	= 0X02,	
	/* 电池电量 报警 类型 */
	Battery_Signal_Beep_EVENT 	= 0X03,
	/* 菜单界面的按键声音 */
	Beep_Menu_Key_Sound_EVENT 	= 0X04, 
}BEEP_CMD_TYPE;

typedef struct
{
	u16		Beep_Open_Time;		// 蜂鸣器 开 时间（ms）
	u16		Beep_Close_Time;	// 蜂鸣器 关 时间（ms）
	BOOL	EVENT_Getarg;		// 事件 执行结果(FALSE:信号正在执行，TRUE:型号执行结束)
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
