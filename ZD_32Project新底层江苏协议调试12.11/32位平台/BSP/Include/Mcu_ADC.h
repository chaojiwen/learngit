/******************** (C) zhou ***************************
 * 文件名  ：
 * 描述    ：       
 * 实验平台：野火STM32开发板
 *
 * 库版本  ：ST3.5.0
 *
 * 作者    ：zhou 
**********************************************************************************/

#ifndef __Mcu_ADC_H
#define __Mcu_ADC_H


#include "stm32f10x.h"
 

/* 仪器启动电压 *///3.5V（由于GSM模块最低的工作电压为3.4+，所以这里设置为3.5V，保证GSM模块能够正常工作。）
#define ADC1_Start_Voltage		(2173u)

/* 仪器最低工作电压 */
//3.35V(这个电压检测电压必须大于3.3V，应为低于3.3V后Vref电压也跟随着低于3.3V，
//这样采样到的ADC值就会不准.)
#define ADC1_Week_Voltage_Low	(2080u)


/*************************************************  函数声明  *********************************************************/
void ADC1_Init(void);




#endif


/******************* (C) ZHOU *****END OF FILE****/
