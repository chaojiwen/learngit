/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/10/28																			 */
/*************************************************************************************************/
#include "Mcu_GPIO.h"
#include "common.h"



///* *********************************************************************************************************
// * 函数名：ADC1_GPIO_Config
// * 描述  ：使能ADC1和DMA1的时钟，初始化PC.01
// * 输入  : 无
// * 输出  ：无
// * 调用  ：内部调用
// * *********************************************************************************************************/
//static void ADC1_GPIO_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	/* 使能DMA时钟 */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
//
//	/* 使能GPIOA\GPIOB\GPIOC和ADC1的时钟 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |\
//						   RCC_APB2Periph_GPIOB |\
//						   RCC_APB2Periph_GPIOC |\
//						   RCC_APB2Periph_ADC1,ENABLE);
//
//	/* 设置IO的工作方式 */		 //    CH0 			CH1			CH2
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;			  													//设置PC1为ADC的电压采样引脚
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;															//IO为模拟输入
//	GPIO_Init(GPIOA,&GPIO_InitStructure);//将数据写入寄存器
//
//	/* 设置IO的工作方式 */		 //		CH3			CH4
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;			  													//设置PC1为ADC的电压采样引脚
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;															//IO为模拟输入
//	GPIO_Init(GPIOB,&GPIO_InitStructure);//将数据写入寄存器
//
//	/* 设置IO的工作方式 */	   //	CH5				CH6			CH7			CH8
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;			  													//设置PC1为ADC的电压采样引脚
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;															//IO为模拟输入
//	GPIO_Init(GPIOC,&GPIO_InitStructure);//将数据写入寄存器
//
//}


/******************* (C) ZHOU *****END OF FILE****/
