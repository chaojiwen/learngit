/************************************************************************************************/
/** @file              : Mcu_Fram.h																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月07日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--MCU内部Fram底层驱动文件							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年11月07日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __Mcu_Fram_H_
#define __Mcu_Fram_H_
#include "stm32f10x.h"



#if (1)//存储相关宏定义
/********************************* 外部Fram 端口定义 *********************************/
#define FramWP_GPIO_PORT		(GPIOD)
#define FramWP_GPIO_CLK			(RCC_APB2Periph_GPIOD)
#define FramWP_GPIO_PIN			(GPIO_Pin_12)
/* 外部Fram写保护引脚 */
#define FramWP_GPIO_Enable()		{GPIO_ResetBits(FramWP_GPIO_PORT,FramWP_GPIO_PIN);}
#define FramWP_GPIO_DISABLE()		{GPIO_SetBits(FramWP_GPIO_PORT,FramWP_GPIO_PIN);}

#endif



/* ***************************函数声明*************************** */
void McuFramInit(void);

#endif


/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
