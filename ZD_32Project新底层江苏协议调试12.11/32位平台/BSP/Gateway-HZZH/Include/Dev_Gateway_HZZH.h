/************************************************************************************************/
/** @file              : Dev_Gateway_HZZH.h															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月08日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--杭州创辉网关底层驱动文件						*/
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
#ifndef __Dev_Gateway_HZZH_h
#define	__Dev_Gateway_HZZH_h

#include "System_Config.h"

#if (Gateway_HZZH_Enable == 0x01)


/********************************* 端口定义 *********************************/

/* 电源控制GPIO */
#define DevGW_PowerPORT		(GPIOC)
#define DevGW_PowerCLK		(RCC_APB2Periph_GPIOC)
#define DevGW_PowerPIN		(GPIO_Pin_15)


#ifdef Hardware_Version_V131
	/* 按键电源 控制引脚 */
	#define DevGW_PowerOpen()		{GPIO_ResetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
	#define DevGW_PowerClose() 	{GPIO_SetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
#else
/* 按键电源 控制引脚 */
	#define DevGW_PowerOpen()		{GPIO_SetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
	#define DevGW_PowerClose() 	{GPIO_ResetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
#endif

/********************************* 网关API结构 *********************************/
typedef struct
{	
	void	  (*Init)			(uint32_t baudrate,void (*funtp)(uint8_t));
	void 	  (*OpenPower)		(void);
	void	  (*ClosePower)		(void);
	void 	  (*SendData)		(uint8_t* datap,uint16_t len);
	void 	  (*ReceData)		(uint8_t* datap,uint8_t len);
}DevGatewayHZZHAPIStruct;


/* ******************* Gateway_HZZH 全局变量声明 ******************* */
extern DevGatewayHZZHAPIStruct DevGWHZZHAPI;

/* ******************* Gateway_HZZH 函数声明 ******************* */
void DevGW_InitAPI(void);


#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
