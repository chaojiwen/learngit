/**
  ******************************************************************************
  * @file              : Mcu_DMA.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015年4月12日 09:55
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
***/
#include "System_Config.h"

#include "Mcu_DMA.h"

#include "Log_Adc1.h"
#include "Log_RF_Card.h"
#include "Log_Sensor.h"


/************************************************************************************************/
/* 函数名	: DMA_Rece_ADC1_Data_Config												 	 		*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置ADC1数据接方式							 										*/
/* 创建日期	: 2015/04/20																		*/
/************************************************************************************************/
void DMA_Rece_ADC1_Data_Config(void)
{
/* DMA接收数据地址 */ 
#define DMA_Rece_ADC1_Data_BaseAddr	((u32)0x4001244C)
	
	DMA_InitTypeDef DMA_InitStructure;

	/* 使能DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	/* 配置ADC1的通道1的DMA模式 */
	DMA_DeInit(DMA1_Channel1);														//复位DMA通道1
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= DMA_Rece_ADC1_Data_BaseAddr;		//设置外设寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (u32)&ADC_DMA_Rece_DR[0];			//设置内存的地址
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC;			//将外设地址做源（MDA_PeripheralBaseAddr）	
	DMA_InitStructure.DMA_BufferSize 			= ADC_Collection_Data_Buffer_Size;	//配置单次传输数据的数量
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;		//不使用DMA的外设地址增量模式
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;				//使用DMA的内存地址增量模式
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord; 	//配置外设数据宽度（半字节，16位）
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_HalfWord;		//配置内存数据宽度（半字节，16位）
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;				//使用DMA的循环模式
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_Medium;				//配置通道的优先级
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;					//不起动寄存器到寄存器模式
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);										//将数据写入寄存器
	DMA_Cmd(DMA1_Channel1,ENABLE);													//使能DMA1
	
#undef DMA_Rece_ADC1_Data_BaseAddr
}

#if (Wireless_RF_Card_Config_Enable == 0x01)
/************************************************************************************************/
/* 函数名	: DMA_Rece_USART2_Data_Config												 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置USART2数据接方式							 									*/
/* 创建日期	: 2015/04/12																		*/
/************************************************************************************************/
void DMA_Rece_USART2_Data_Config(void)
{
/* DMA接收数据地址 */
#define DMA_Rece_USART2_Data_BaseAddr	((u32)0x40004404)
/* DMA单次传输数据的数量 */
#define DMA_Rece_USART2_Data_Num		(32u)	
	
	DMA_InitTypeDef DMA_InitStructure;

	/* 使能DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	/* 复位寄存器 */
	DMA_Cmd(DMA1_Channel6,DISABLE);		
	/* 配置通道6的DMA模式 */
	DMA_DeInit(DMA1_Channel6);														//复位DMA通道6
	
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= DMA_Rece_USART2_Data_BaseAddr;	//设置外设寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (u32)Wireless_RF_Card_Data_Buffer_Pointer;//设置内存的地址
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC;			//将外设地址做源（MDA_PeripheralBaseAddr）	
	DMA_InitStructure.DMA_BufferSize 			= DMA_Rece_USART2_Data_Num;			//配置单次传输数据的数量
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;		//不使用DMA的外设地址增量模式
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;				//使用DMA的内存地址增量模式
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte; 		//配置外设数据宽度（半字节，8位）
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;			//配置内存数据宽度（半字节，8位）
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;				//使用DMA的循环模式
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High;				//配置通道的优先级
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;					//不起动寄存器到寄存器模式
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);										//将数据写入寄存器
	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);									//配置DMA1创数据的中断方式
	DMA_ITConfig(DMA1_Channel6,DMA_IT_HT,ENABLE);									//配置DMA1创数据的中断方式
	
	DMA_Cmd(DMA1_Channel6,ENABLE);													//使能DMA1的通道6功能


#undef DMA_Rece_USART2_Data_BaseAddr
#undef DMA_Rece_USART2_Data_Num	
}
#endif



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

