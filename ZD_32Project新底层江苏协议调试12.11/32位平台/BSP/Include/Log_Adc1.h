#ifndef __Log_Adc1_H
#define __Log_Adc1_H
			  	 
#include "stm32f10x.h"	     				
   
/************************************************************************************************/
/* 传感器信号检测		 控制端口 端口时钟 控制引脚编号 */
#define Sen_Pwr_Spe_PORT              	GPIOE
#define Sen_Pwr_Spe_CLK               	RCC_APB2Periph_GPIOE 
#define Sen_Pwr_Spe_PIN               	GPIO_Pin_15

/* PT1000传感器信号检测	 控制端口 端口时钟 控制引脚编号 */
#define PT1000_Pwr_Spe_PORT             (GPIOE)
#define PT1000_Pwr_Spe_CLK              (RCC_APB2Periph_GPIOE) 
#define PT1000_Pwr_Spe_PIN              (GPIO_Pin_12)


/* 仪器电源电压检测	 控制端口 端口时钟 控制引脚编号 */
#define Itu_Pwr_Spe_PORT              	GPIOA
#define Itu_Pwr_Spe_CLK               	RCC_APB2Periph_GPIOA 
#define Itu_Pwr_Spe_PIN               	GPIO_Pin_5

/* 传感器信号检测  :开 */
#define Sen_Pwr_Spe_Open  				{GPIO_ResetBits	(Sen_Pwr_Spe_PORT,Sen_Pwr_Spe_PIN);}
#define Sen_Pwr_Spe_Close 				{GPIO_SetBits	(Sen_Pwr_Spe_PORT,Sen_Pwr_Spe_PIN);}

/* PT1000传感器信号检测  :开 */
#define PT1000_Pwr_Spe_Open  			{GPIO_ResetBits	(PT1000_Pwr_Spe_PORT,PT1000_Pwr_Spe_PIN);}
#define PT1000_Pwr_Spe_Close 			{GPIO_SetBits	(PT1000_Pwr_Spe_PORT,PT1000_Pwr_Spe_PIN);}

/* 仪器电源电压检测:开 */
#define Itu_Pwr_Spe_Open				{GPIO_SetBits	(Itu_Pwr_Spe_PORT,Itu_Pwr_Spe_PIN);}		
#define Itu_Pwr_Spe_Close				{GPIO_ResetBits	(Itu_Pwr_Spe_PORT,Itu_Pwr_Spe_PIN);}


/* ADC采样的数据寄存器大小 */
#define		ADC_Collection_Data_Buffer_Size		(9u)
/* ADC 通过DMA方式传输的数据寄存器 */
extern u16	ADC_DMA_Rece_DR[ADC_Collection_Data_Buffer_Size];

/************************************************************************************************/
/* 函数声明 */
void ADC_Data_Buffer_Init(void);
void ADC_Get_Value(void);
void ADC_Trans_Data(float *data_buffer);

void ADC_AWD_Run_Init(void);
void ADC_Sam_Pwr_Sta(void);

#endif  
	 

