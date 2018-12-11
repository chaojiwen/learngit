/************************************************************************************************/
/** @file              : Dev_Gateway_NJSY.h															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月08日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--南京盛亿网关底层驱动文件						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0      ZRDang      2016年11月08日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __Dev_Gateway_NJSY_h
#define	__Dev_Gateway_NJSY_h

#include "System_Config.h"

#if (Gateway_NJSY_Enable == 0x01)


/********************************* 端口定义 *********************************/

///* 电源控制GPIO */
//#define DevGW_PowerPORT		(GPIOE)
//#define DevGW_PowerCLK		(RCC_APB2Periph_GPIOE)
//#define DevGW_PowerPIN		(GPIO_Pin_15)


/* 电源控制GPIO */
#define DevGW_PowerPORT		(GPIOE)
#define DevGW_PowerCLK		(RCC_APB2Periph_GPIOE)
#define DevGW_PowerPIN		(GPIO_Pin_12)

///* 按键电源 控制引脚 */
//#define DevGW_PowerOpen()	{GPIO_ResetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
//#define DevGW_PowerClose() 	{GPIO_SetBits(DevGW_PowerPORT,DevGW_PowerPIN);}

/* 按键电源 控制引脚 */
#define DevGW_PowerOpen()	{GPIO_SetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
#define DevGW_PowerClose() 	{GPIO_ResetBits(DevGW_PowerPORT,DevGW_PowerPIN);}






/* ******************* Gateway_NJSY 函数声明 ******************* */
void DevGW_Init(uint32_t baudrate,void (*funtp)(uint8_t));
void DevGW_OpenPower(void);
void DevGW_ClosePower(void);
void DevGW_SendData(uint8_t* datap,uint16_t len);
void DevGW_ReceData(uint8_t* datap,uint8_t len);

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
