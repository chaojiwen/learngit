/******************** (C) zhou ***************************
 * �ļ���  ��
 * ����    ��       
 * ʵ��ƽ̨��Ұ��STM32������
 *
 * ��汾  ��ST3.5.0
 *
 * ����    ��zhou 
**********************************************************************************/

#ifndef __Mcu_ADC_H
#define __Mcu_ADC_H


#include "stm32f10x.h"
//#include "math.h" 


//#define	Adc_Precision	(4096-1)
//#define	NTC_B			(3950)
//#define	NTC_T0			(273.15)
//#define	NTC_T1			(273.15+25.0)
//#define	NTC_R0			(10000)
//
//
//#define Adc_to_NTC_Rt(Adc_DR)		((unsigned int)(NTC_R0*Adc_DR)/(Adc_Precision-Adc_DR))
//#define Adc_to_Temperature(Adc_DR)	(((NTC_B*NTC_T1) /(NTC_T1*ln(Adc_to_NTC_Rt(Adc_DR))+NTC_B))-NTC_T0)







/*************************************************  ��������  *********************************************************/
void ADC_Config(void);




#endif


/******************* (C) ZHOU *****END OF FILE****/
