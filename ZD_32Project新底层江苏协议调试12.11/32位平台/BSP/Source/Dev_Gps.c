/************************************************************************************************/
/** @file              : Dev_GPS.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年8月7日															*/
/** @brief             : c file																	*/
/** @description       : GPS定位驱动文件														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Dev_GPS.h"


#include "mcu_usart.h"




/************************************************************************************************/
/* 定义GPS电源控制引脚的 端口、时钟源、引脚 */
#define GPS_Power_PORT       			(GPIOC)
#define GPS_Power_CLK        			(RCC_APB2Periph_GPIOC)
#define GPS_Power_PIN        			(GPIO_Pin_15)

#ifdef Hardware_Version_V131
/* GPS模块电源:开 */
#define Open_GPS_Power					{GPIO_ResetBits(GPS_Power_PORT,GPS_Power_PIN);}
/* GPS模块电源:关 */
#define Close_GPS_Power					{GPIO_SetBits(GPS_Power_PORT,GPS_Power_PIN);}
#else
/* GPS模块电源:开 */
#define Open_GPS_Power					{GPIO_SetBits(GPS_Power_PORT,GPS_Power_PIN);}
/* GPS模块电源:关 */
#define Close_GPS_Power					{GPIO_ResetBits(GPS_Power_PORT,GPS_Power_PIN);}
#endif



/************************************************************************************************/
/* GPRMC命令数据长度 */
#define GPS_CMD_RMC_Data_Len			(16u)
/* GPGSA命令数据长度 */
#define GPS_CMD_GSA_Data_Len			(16u)
/* GPGGA命令数据长度 */
#define GPS_CMD_GGA_Data_Len			(16u)
/* GPGLL命令数据长度 */
#define GPS_CMD_GLL_Data_Len			(16u)
/* GPGSV命令数据长度 */
#define GPS_CMD_GSV_Data_Len			(16u)
/* GPVTG命令数据长度 */
#define GPS_CMD_VTG_Data_Len			(16u)
/* 冷启动数据长度 */
#define GPS_CMD_Cold_Start_Data_Len		(12u)


/* 关闭RMC消息输出 */
const unsigned char 	GPS_CMD_TurnOff_RMC[GPS_CMD_RMC_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x40};
/* 关闭GSA消息输出 */
const unsigned char 	GPS_CMD_TurnOff_GSA[GPS_CMD_GSA_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32};
/* 关闭GGA消息输出 */
const unsigned char 	GPS_CMD_TurnOff_GGA[GPS_CMD_GGA_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x24};
/* 关闭GLL消息输出 */
const unsigned char 	GPS_CMD_TurnOff_GLL[GPS_CMD_GLL_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B};
/* 关闭GSV消息输出 */
const unsigned char 	GPS_CMD_TurnOff_GSV[GPS_CMD_GSV_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39};
/* 关闭VTG消息输出 */
const unsigned char 	GPS_CMD_TurnOff_VTG[GPS_CMD_VTG_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05, 0x47};

/* 冷启动 */
const unsigned char		GPS_CMD_ColdStart[GPS_CMD_Cold_Start_Data_Len] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0xFF, 0xFF, 0x02, 0x00, 0x0E, 0x61};


/************************************************************************************************/





/************************************************************************************************/
/* 函数名	: GPS_Power_Control_Pin_Config												 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: GPS电源控制引脚配置。							 									*/
/* 创建日期	: 2015/08/07																		*/
/************************************************************************************************/
static void GPS_Power_Control_Pin_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;


	/* 设置 GPS 电源控制引脚 的时钟 */
	RCC_APB2PeriphClockCmd(GPS_Power_CLK,ENABLE);
	/* 设置 GPS 电源控制引脚的初始状态(关)  */
	GPIO_SetBits(GPS_Power_PORT,GPS_Power_PIN);
	
	/* 配置 电源控制引脚 */
	GPIO_InitStructure.GPIO_Pin 	= GPS_Power_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	/*
		PC15引脚通过电源开关进行供电，而这个电源开关只能够吸收有限的电流(3mA)。
		因此这三个引脚作为输出引脚时有以下限制：在同一时间只有一个引脚能作为输出，
		作为输出脚时只能工作在2MHz模式下，最大驱动负载为30pF，并且不能作为电流源(如驱动LED)。
	*/	
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Init(GPS_Power_PORT, &GPIO_InitStructure);
}

/************************************************************************************************/
/* 函数名	: GPS_CMD_Config															 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 选择GPS模块输出消息内容。						 									*/
/* 创建日期	: 2015/08/07																		*/
/************************************************************************************************/
void GPS_CMD_Config(void)
{
	
#if (GPS_CMD_RMC_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_RMC,GPS_CMD_RMC_Data_Len);
#endif
	
#if (GPS_CMD_GSA_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_GSA,GPS_CMD_GSA_Data_Len);
#endif
	
#if (GPS_CMD_GGA_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_GGA,GPS_CMD_GGA_Data_Len);
#endif
	
#if (GPS_CMD_GSV_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_GSV,GPS_CMD_GSV_Data_Len);
#endif
	
#if (GPS_CMD_GLL_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_GLL,GPS_CMD_GLL_Data_Len);
#endif
	
#if (GPS_CMD_VTG_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_VTG,GPS_CMD_VTG_Data_Len);
#endif

}


/************************************************************************************************/
/* 函数名	: GPS_Dev_Init																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 初始化GPS_Dev																		*/
/* 创建日期	: 2015/08/07																		*/
/************************************************************************************************/
void GPS_Dev_Init(void)
{	
	/* 上电后初始化GPS时将GPS强行断电以便确保上电时GPS处于明确的电源管理状态. */
	/* GPS电源控制引脚配置。 */
	GPS_Power_Control_Pin_Config();
	
	/* 关GPS模块电源 */
	Close_GPS_Power;

	__NOP();

	/* GPS模块电源:开 */
	Open_GPS_Power;

}












