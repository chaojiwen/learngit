#ifndef __Log_Temp_Cal_H
#define __Log_Temp_Cal_H
			  	 
#include "stm32f10x.h"


/* º¯ÊýÉùÃ÷ */
float Temp_Get_Cal_Value			(float Temp_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value);
float Temp_Get_Sampl_Value		(u16 Adc_DR);
float CarbonDioxide_Get_Sampl_Value(u16 Adc_DR);
float DoorContact_Get_Sampl_Value(u16 Adc_DR);
	 
#endif  
	 

