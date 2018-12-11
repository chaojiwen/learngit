/************************************************************************************************/
/** @file              : Mcu_DMA2.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月15日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--STM32F103-DMA2逻辑层文件						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0      ZRDang      2016年11月15日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/

#include "Mcu_DMA2.h"




/************************************************************************************************/
/* 函数名	: DMA2_Init																 	 		*/
/* 输入  	: PBA:目标地址，MBA：内存地址。			 											*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化DMA2功能									 										*/
/* 创建日期	: 2016/11/15																		*/
/************************************************************************************************/
void DMA2_Init(void)
{

	/* 配置DMA2的通道5的模式 */
	DMA_DeInit(DMA2_Channel5);		
	/* 使能DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);

}

/************************************************************************************************/
/* 函数名	: DMA2_SendData														 	 			*/
/* 输入  	: PBA:目标地址，MBA：内存地址。			 											*/
/* 输出		: 无 																				*/
/* 作用	 	: 通过DMA2发送指定数量的数据到指定的外设去							 					*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/*   注释	: DMA2使用单次传输方式																*/
/************************************************************************************************/
void DMA2_SendData(uint32_t pba,uint32_t mba,uint16_t len)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	
	/* 初始化DMA数据结构 */
	DMA_StructInit(&DMA_InitStructure);
	/* 配置DMA2的通道5的模式 */
	DMA_DeInit(DMA2_Channel5);	
	
	/* 配置运行参数 */
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= pba;								//设置外设寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr 		= mba;								//设置内存的地址
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralDST;			//将外设地址做源（MDA_PeripheralBaseAddr）	
	DMA_InitStructure.DMA_BufferSize 			= len;								//配置单次传输数据的数量
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;		//不使用DMA的外设地址增量模式
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;				//使用DMA的内存地址增量模式
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte; 		//配置外设数据宽度（字节，8位）
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;			//配置内存数据宽度（字节，8位）
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;					//使用DMA的单次传输模式
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_Medium;				//配置通道的优先级
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;					//不起动寄存器到寄存器模式
	
	/* 将参数写入寄存器 */
	DMA_Init(DMA2_Channel5,&DMA_InitStructure);		
	
	/* 数据传输 */
	DMA_Cmd(DMA2_Channel5,ENABLE);	
}




/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

