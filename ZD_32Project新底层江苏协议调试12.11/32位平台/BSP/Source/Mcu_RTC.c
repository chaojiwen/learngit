/************************************************************************************************/
/** @file              : Mcu_RTC.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月07日															*/
/** @brief             : c file																	*/
/** @description       : RTC MCU文件														    */
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "Mcu_RTC.h"





/************************************************************************************************/
/* 函数名	: Mcu_RTCInit																		*/
/* 输入  	: *pdata:数据指针;addr:操作地址;	count:操作数量;										*/
/* 输出		: 执行结果。																			*/
/* 作用	 	: 写RTC操作。								 									 	*/
/* 创建日期 	: 2015/8/27																			*/
/************************************************************************************************/
void McuRTCInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RTC_OE_CLK,ENABLE);
	
	/* WP写保护 GPIO */
	GPIO_InitStructure.GPIO_Pin = RTC_OE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	
	GPIO_Init(RTC_OE_PORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(RTC_OE_PORT, RTC_OE_PIN );
	
	 RTC_OE_Enable();//开启RTC功能zwc
	
}




/******************* (C) ZHOU *****END OF FILE****/

