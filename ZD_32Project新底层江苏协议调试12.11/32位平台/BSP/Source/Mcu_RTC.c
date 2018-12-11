/************************************************************************************************/
/** @file              : Mcu_RTC.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��07��															*/
/** @brief             : c file																	*/
/** @description       : RTC MCU�ļ�														    */
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
/* ������	: Mcu_RTCInit																		*/
/* ����  	: *pdata:����ָ��;addr:������ַ;	count:��������;										*/
/* ���		: ִ�н����																			*/
/* ����	 	: дRTC������								 									 	*/
/* �������� 	: 2015/8/27																			*/
/************************************************************************************************/
void McuRTCInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RTC_OE_CLK,ENABLE);
	
	/* WPд���� GPIO */
	GPIO_InitStructure.GPIO_Pin = RTC_OE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	
	GPIO_Init(RTC_OE_PORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(RTC_OE_PORT, RTC_OE_PIN );
	
	 RTC_OE_Enable();//����RTC����zwc
	
}




/******************* (C) ZHOU *****END OF FILE****/

