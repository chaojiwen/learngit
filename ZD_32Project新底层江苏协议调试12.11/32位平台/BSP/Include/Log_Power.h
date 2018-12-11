#ifndef __Log_Power_H
#define	__Log_Power_H

#include "stm32f10x.h"
#include "Mcu_Config.h"


/* 外电状态检测引脚设定 */
#define Pwr_ET_GPIO_PORT              	GPIOE
#define Pwr_ET_GPIO_CLK               	RCC_APB2Periph_GPIOE 
#define Pwr_ET_GPIO_PIN               	GPIO_Pin_13

// 读取GPIO口电平值应使用STM32库函数，而不应直接读取寄存器(某些IO口可以通过直接读取寄存器正确读出，有些不行)
#define Ext_Power_ON_OFF_Check()      GPIO_ReadInputDataBit(Pwr_ET_GPIO_PORT,Pwr_ET_GPIO_PIN)	


/* 函数声明 */
void Power_ET_Link_State_Check	(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
void Power_ET_SET_Alarm_State	(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
u8 	 Power_ET_Alarm_Check			(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
void Power_ET_Para_Init			(INSTRU_POWER *power_state);

#endif /* __Log_Power_H */





