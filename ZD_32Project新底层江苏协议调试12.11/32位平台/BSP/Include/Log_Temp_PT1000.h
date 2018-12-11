/**
  ******************************************************************************
  * @file              : Log_Temp_PT1000.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 07/01/2016 14:41:56
  * @brief             : c file
  * @description       : PT1000 Device Driver
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
  */
#ifndef __Log_Temp_PT1000_H
#define __Log_Temp_PT1000_H
			  	 
#include "stm32f10x.h"

/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<e>�����´�����PT1000��������
//  <o1.0..2>�����´�����������Χѡ��.
//  <0=> ������Χ-100��+80.
//  <1=> ������Χ0��+300.
//	<i>��ӡ����ѡ��.
#define MACRO_PT1000		(0x01)
#define MACRO_PT1000_Range	(0x01)
//  </e>

//	<<< end of configuration section >>>



/* �������� */
float Temp_PT1000_Get_Sampl_Value		(u16 Adc_DR);


	 
#endif  
	 

