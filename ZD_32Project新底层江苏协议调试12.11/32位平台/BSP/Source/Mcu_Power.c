/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/10/26																			 */
/*************************************************************************************************/
#include "Mcu_Power.h" 
/************************************************************************************************/
/* 函数名	: Power_Check_Init																 	*/
/* 输入		: 无 																				*/
/* 输出		: 无 																				*/
/* 作用		: 配置外部电源检测用到的I/O口 														 	*/
/* 创建日期	: 2014/10/08																		*/
/* 修改日期	: 2014/11/11																		*/
/************************************************************************************************/
void Power_Check_Init(void)
{			   
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);
	/* 外电检测和电池检测 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/* 电池电量检测 开关 控制引脚 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

