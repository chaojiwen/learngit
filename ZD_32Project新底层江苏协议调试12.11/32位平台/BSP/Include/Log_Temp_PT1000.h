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
//	<e>超低温传感器PT1000参数设置
//  <o1.0..2>超低温传感器测量范围选择.
//  <0=> 测量范围-100到+80.
//  <1=> 测量范围0到+300.
//	<i>打印方案选择.
#define MACRO_PT1000		(0x01)
#define MACRO_PT1000_Range	(0x01)
//  </e>

//	<<< end of configuration section >>>



/* 函数声明 */
float Temp_PT1000_Get_Sampl_Value		(u16 Adc_DR);


	 
#endif  
	 

