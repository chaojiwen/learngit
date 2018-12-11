#ifndef __Log_Adc1_H
#define __Log_Adc1_H
			  	 
#include "stm32f10x.h"	     				
   
/************************************************************************************************/
/* �������źż��		 ���ƶ˿� �˿�ʱ�� �������ű�� */
#define Sen_Pwr_Spe_PORT              	GPIOE
#define Sen_Pwr_Spe_CLK               	RCC_APB2Periph_GPIOE 
#define Sen_Pwr_Spe_PIN               	GPIO_Pin_15

/* PT1000�������źż��	 ���ƶ˿� �˿�ʱ�� �������ű�� */
#define PT1000_Pwr_Spe_PORT             (GPIOE)
#define PT1000_Pwr_Spe_CLK              (RCC_APB2Periph_GPIOE) 
#define PT1000_Pwr_Spe_PIN              (GPIO_Pin_12)


/* ������Դ��ѹ���	 ���ƶ˿� �˿�ʱ�� �������ű�� */
#define Itu_Pwr_Spe_PORT              	GPIOA
#define Itu_Pwr_Spe_CLK               	RCC_APB2Periph_GPIOA 
#define Itu_Pwr_Spe_PIN               	GPIO_Pin_5

/* �������źż��  :�� */
#define Sen_Pwr_Spe_Open  				{GPIO_ResetBits	(Sen_Pwr_Spe_PORT,Sen_Pwr_Spe_PIN);}
#define Sen_Pwr_Spe_Close 				{GPIO_SetBits	(Sen_Pwr_Spe_PORT,Sen_Pwr_Spe_PIN);}

/* PT1000�������źż��  :�� */
#define PT1000_Pwr_Spe_Open  			{GPIO_ResetBits	(PT1000_Pwr_Spe_PORT,PT1000_Pwr_Spe_PIN);}
#define PT1000_Pwr_Spe_Close 			{GPIO_SetBits	(PT1000_Pwr_Spe_PORT,PT1000_Pwr_Spe_PIN);}

/* ������Դ��ѹ���:�� */
#define Itu_Pwr_Spe_Open				{GPIO_SetBits	(Itu_Pwr_Spe_PORT,Itu_Pwr_Spe_PIN);}		
#define Itu_Pwr_Spe_Close				{GPIO_ResetBits	(Itu_Pwr_Spe_PORT,Itu_Pwr_Spe_PIN);}


/* ADC���������ݼĴ�����С */
#define		ADC_Collection_Data_Buffer_Size		(9u)
/* ADC ͨ��DMA��ʽ��������ݼĴ��� */
extern u16	ADC_DMA_Rece_DR[ADC_Collection_Data_Buffer_Size];

/************************************************************************************************/
/* �������� */
void ADC_Data_Buffer_Init(void);
void ADC_Get_Value(void);
void ADC_Trans_Data(float *data_buffer);

void ADC_AWD_Run_Init(void);
void ADC_Sam_Pwr_Sta(void);

#endif  
	 

