#ifndef __Log_Battery_H
#define __Log_Battery_H

#include "stm32f10x.h"
#include "Mcu_Config.h"

/************************************************************************************************/
/* 电池电量类型 */
#define	Power_Null				(0x00)	
#define	Power_One				  (0x01)	
#define	Power_Two				  (0x02)	
#define	Power_There  			(0x03)	
#define	Power_Four				(0x04)	
#define	Power_Full  			(0x05)

/************************************************************************************************/
void Power_BT_Quantity_Check		  (INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
void Power_BT_Charge_State_Check	(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
void Power_BT_SET_Alarm_State		  (INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
u8 	 Power_BT_Alarm_Check			    (INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
//BOOL Power_Record_Function_Check	(u16 pwr_vle);

void Battery_Power_Display(void);

void Battery_Para_Init(INSTRU_POWER *bt_state);



#endif /* __Log_Battery_H */
