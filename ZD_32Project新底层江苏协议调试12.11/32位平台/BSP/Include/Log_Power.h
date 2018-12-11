#ifndef __Log_Power_H
#define	__Log_Power_H

#include "stm32f10x.h"
#include "Mcu_Config.h"


/* ���״̬��������趨 */
#define Pwr_ET_GPIO_PORT              	GPIOE
#define Pwr_ET_GPIO_CLK               	RCC_APB2Periph_GPIOE 
#define Pwr_ET_GPIO_PIN               	GPIO_Pin_13

// ��ȡGPIO�ڵ�ƽֵӦʹ��STM32�⺯��������Ӧֱ�Ӷ�ȡ�Ĵ���(ĳЩIO�ڿ���ͨ��ֱ�Ӷ�ȡ�Ĵ�����ȷ��������Щ����)
#define Ext_Power_ON_OFF_Check()      GPIO_ReadInputDataBit(Pwr_ET_GPIO_PORT,Pwr_ET_GPIO_PIN)	


/* �������� */
void Power_ET_Link_State_Check	(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
void Power_ET_SET_Alarm_State	(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
u8 	 Power_ET_Alarm_Check			(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
void Power_ET_Para_Init			(INSTRU_POWER *power_state);

#endif /* __Log_Power_H */





