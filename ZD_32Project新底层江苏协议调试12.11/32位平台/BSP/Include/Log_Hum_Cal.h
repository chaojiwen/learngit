#ifndef __Log_Hum_Cal_H
#define __Log_Hum_Cal_H
			  	 
#include "stm32f10x.h"	     



/* 计算温湿卡片 湿度数据的算法 */
#define RF_Humiture_Card_Data_to_Hum_DATA(Hum)		((((Hum*125.0)/65536)-6)*10)


/* 函数声明 */
void 	Humi_Get_Temp_Comp_Value	(void);
float 	Humi_Get_Sampl_Value		(u16 Adc_DR);
float 	Humi_Get_Cal_Value			(float Hum_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value);
#endif  
	 

