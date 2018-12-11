/************************************************************************************************/
/** @file              : BSP.c																		*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																	*/
/** @date              : 2015年6月17日														*/
/** @brief             : c file																	*/
/** @description       : 板级配置																*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "BSP.h"
#include "Mcu_ADC.h"
#include "Mcu_Fram.h"
#include "Mcu_led.h"
#include "Mcu_Power.h"
#include "Mcu_RTC.h"
#include "Mcu_SPI.h"
#include "Mcu_Usart.h"
#include "Dev_RTC.h"
#include "Log_Adc1.h"
#include "Log_Alarm.h"
#include "Log_ConfigService.h"
#include "Log_Download.h"
#include "Log_INF.h"
#include "Log_RF_Card.h"
#include "App_Save.h"
#include "Log_Sensor.h"
#include "Log_Time.h"
#include "Mcu_Beep.h"
#include "Mcu_button.h"
#include "Ks0108.h"
#include "Menu.h"
#include "Psl_AI2C.h"
#include "Sve_Cfg_Tab.h"
#include "Sve_Fix_Cfg_Tab.h"
#include <string.h>
/************************************************************************************************/
//由于sprintf在Keil+Ucos-II使用%f存在字节对齐问题，因此需要现将float通过其他函数转成字符串形式，
//再将其他数据通过sprintf整合成。
static int n_tu(int number, int count)
{
	int result=1;
	
	
	while(count-- > 0)
	result *=number;  

	return result;
}

/***Convert float to string***/
void float_to_string(float f, char r[])
{
	long long int length, length2, i, number, position, sign;
	float number2;
	
	sign=-1;   // -1 == positive number
	if (f <0)
	{
		sign='-';
		f *= -1;    
	}   


	number2=f;  
	number=f;
	length=0;  // size of decimal part
	length2=0; //  size of tenth


	/* calculate length2 decimal part */
	while( (number2 - (float)number) != 0.0 && !((number2 - (float)number) < 0.0) )
	{
		number2= f * (n_tu(10.0,length2+1));
		number=number2; 

		length2++;
	}

	/* calculate length integer part*/
	for(length=(f >= 1) ? 0 : 1; f >= 1; length++) 
		f /= 10;

	if(length2 != 0)
	{
		position=length;
		length=length+1+length2;
	}else
	{
		/* 没有小数的情况，设置一个不存在的值，去掉小数点 */
		position = length+2;
	}
	
	 number=number2;
	 if(sign=='-')
	 {
		 length++;
		 position++;
	 }

	 for(i=length; i >= 0 ; i--)
	 {
		if(i== (length))
		  r[i]='\0';
		else if(i==(position))
		  r[i]='.';
		else if(sign=='-' && i==0)
		  r[i]='-';
		else  
		{
		  r[i]= (number % 10)+'0';
		  number /=10;
		}
	}
}

/************************************************************************************************/
/* 函数名	: BSP_Init																			*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 板级配置(涉及一些 物理层 相关的配置)													*/
/* 创建日期 	: 2014/09/11																		*/
/* 修改日期 	: 2015/11/11																		*/
/************************************************************************************************/
void BSP_Init(void)
{    

	/* LED 初始化 */
	LED_GPIO_Init();

	/* USART1 初始化 */
	USART1_Init();
	
	/* ADC1 初始化 */
	ADC1_Init();
	
#if (Wireless_RF_Card_Config_Enable == 0x01)
	/* 无线RF卡片 初始化 */
	RF_Card_Init();
#endif
	/* SPI1 初始化 */ //其实 SPI1 是用在 外设FLASH 中的，这个应该写在SPI1的物理层当中 
	Mcu_SPI1_Init();

	#ifndef Hardware_Version_V131
	/* 初始化Fram WP 写保护GPIO */
	McuFramInit();
	#endif
	/* 初始化存储探头数据功能接口 */
	AppSave_InitAPI();
	/* 初始化通信控制使能引脚OE */
	McuRTCInit();
	/* 初始化锁住(这个I2C是用IO模拟的) */
	Psl_AI2C_Bus_Unlock();
	/* 外设 OLED 初始化  */	//这个函数这样写是不合理的，后面有时间可更改 
	GUI_Init();
	/* Beep 初始化 */
	Beep_Init();
	/* 电源检测 初始化 */
	Power_Check_Init();
	/* 按键 初始化 */
	Button_Init();
}

/************************************************************************************************/
/* 函数名	: Serial_Number_Init																*/
/* 输入		: 仪器配置SN号																		*/
/* 输出		: 无 																				*/
/* 作用		: 设置 仪器的SN号																		*/
/* 创建日期	: 2015/06/01																		*/
/* 创建日期	: 2015/11/12																		*/
/************************************************************************************************/
void Serial_Number_Init(SERIAL_NUMBER *serial_number)
{
	
	/* 判断 仪器编号 是否一致 */
	if((serial_number->Serial_Number[0] != (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+0)))||
	   (serial_number->Serial_Number[1] != (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+1)))||
	   (serial_number->Serial_Number[2] != (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)))||
	   (serial_number->Serial_Number[3] != (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+3))))
	{
		/* 判断当前仪器SN配置情况. */
		if(((*(u32*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START))) == 0XFFFFFFFF)
		{
			/* MCU的内部FLASH存储空间中的SN没有配置过 */
			
			/* 判断当前仪器是 “重新烧写程序” 还是 “首次烧写程序” */
			if((serial_number->Serial_Number[0] == 0X00) &&
			   (serial_number->Serial_Number[1] == 0X00) &&
			   (serial_number->Serial_Number[2] == 0X00) &&
			   (serial_number->Serial_Number[3] == 0X00))
			{
				/* 判断当前仪器是 “首次烧写程序”,需要配置工具 配置SN号. */
				__nop();
			}else
			{
				/* 判断当前仪器是 “重新烧写程序”,可以根据配置表中留存的SN自动配置. */
				Flash_Write_SN_NO((unsigned char*)serial_number->Serial_Number);
				
			}
			
		}else
		{
//////			/* MCU的内部FLASH存储空间中的SN号与配置表中的SN号不一致，将使用MCU的内部FLASH存储空间中的SN号. */
//////			serial_number->Serial_Number[0] = (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+0));
//////			serial_number->Serial_Number[1] = (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+1));
//////			serial_number->Serial_Number[2] = (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2));
//////			serial_number->Serial_Number[3] = (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+3));
			Flash_Write_SN_NO((unsigned char*)serial_number->Serial_Number);
			/* 修改 仪器 信息表 设置标志 */
			Write_Config_Tab((u8*)serial_number,sizeof(SERIAL_NUMBER));
		}		
		
	}else
	{
		/* 仪器SN号与配置表中的SN一致. */
		__nop();
	}
	
	
	/* 设置仪器编号字符串 */
	sprintf((char*)Serial_Number,"%04X%05d",
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8)|
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8)|
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
	
	
	
	
	
}

/************************************************************************************************/
/* 函数名	: Ste_Vsn_NO_Init																	*/
/* 输入		: 软件版本号																			*/
/* 输出		: 无 																				*/
/* 作用		: 设置 软件SN号																		*/
/* 创建日期	: 2015/11/12																		*/
/* 创建日期	: 2015/11/12																		*/
/************************************************************************************************/
static void Ste_Vsn_NO_Init(VERSION *vsn_no)
{
	/* 判断软件版本号是否一致 */
	if((vsn_no->Serial_Number_Code.Code_A != Software_Version_NO_Major)||
	   (vsn_no->Serial_Number_Code.Code_B != Software_Version_NO_Child)||
	   (vsn_no->Serial_Number_Code.Code_C != Software_Version_NO_Stage))
	{
		/* 数据不一致，将使用仪器默认的软件版本信息 */
		vsn_no->Serial_Number_Code.Code_A = Software_Version_NO_Major;
		vsn_no->Serial_Number_Code.Code_B = Software_Version_NO_Child;
		vsn_no->Serial_Number_Code.Code_C = Software_Version_NO_Stage;
		
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)&vsn_no,sizeof(*vsn_no));
	}

	/* 设置中文软件版本字符串 */
	sprintf((char*)Software_Version_CN,"软件版本:%d.%d.%d",
	Software_Version_NO_Major,
	Software_Version_NO_Child,
	Software_Version_NO_Stage);
	
	/* 设置英文软件版本字符串 */
	sprintf((char*)Software_Version_EN,"SV:%d.%d.%d",
	Software_Version_NO_Major,
	Software_Version_NO_Child,
	Software_Version_NO_Stage);
	
}

/************************************************************************************************/
/* 函数名	: Hde_Vsn_NO_Init																	*/
/* 输入		: 硬件版本号																			*/
/* 输出		: 无 																				*/
/* 作用		: 设置 硬件SN号																		*/
/* 创建日期	: 2015/11/12																		*/
/* 创建日期	: 2015/11/12																		*/
/************************************************************************************************/
static void Hde_Vsn_NO_Init(VERSION *vsn_no)
{

	/* 设置中文硬件版本字符串 */
	sprintf((char*)Hardware_Version_CN,"硬件版本:%d.%d.%d",
	vsn_no->Serial_Number_Code.Code_A,
	vsn_no->Serial_Number_Code.Code_B,
	vsn_no->Serial_Number_Code.Code_C);

	/* 设置英文硬件版本字符串 */
	sprintf((char*)Hardware_Version_EN,"HV:%d.%d.%d",
	vsn_no->Serial_Number_Code.Code_A,
	vsn_no->Serial_Number_Code.Code_B,
	vsn_no->Serial_Number_Code.Code_C);

}

/************************************************************************************************/
/* 函数名	: System_Serial_Init																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 系统序列号初始化																	*/
/* 创建日期	: 2015/11/17																		*/
/* 创建日期	: 2015/11/17																		*/
/************************************************************************************************/
void System_Serial_Init(void)
{

	/* 仪器SN号 初始化 */
	Serial_Number_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number);
	
	/* 仪器软件版本号 初始化 */
	Ste_Vsn_NO_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Software_Version);
	
	/* 仪器硬件版本号 初始化 */
	Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number_Code.Code_A=Hardware_Version_NO_Major;
	Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number_Code.Code_B=Hardware_Version_NO_Child;
	Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number_Code.Code_C=Hardware_Version_NO_Stage;
	Hde_Vsn_NO_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Hardware_Version);

}

/************************************************************************************************/
/* 函数名	: System_Para_Init																	*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置系统参数																		*/
/* 创建日期 	: 2014/11/11																		*/
/************************************************************************************************/
void System_Para_Init(void)
{
	/* 配置表 初始化 */
	Sve_Cfg_Tab_Init();
	/* 固定配置表 初始化 */
	Sve_Fix_Cfg_Tab_Init();
	/* 系统序列号 初始化 */
	System_Serial_Init();
	/********************* IAP 程序升级 数据存储 相关参数(内部flash) ***************************************/
	//	Mcu_IAP_Flash_Data.Data_Buffer		= IAP_Data_Buffer;		//IAP数据缓存
	//	Mcu_IAP_Flash_Data.Write_Pointer	= BLOCK_CODESTORE_START;//IAP写指针
	//	Mcu_IAP_Flash_Data.Data_Buffer_Size = IAP_PageSize;			//接收数据缓冲区大小
	//	Mcu_IAP_Flash_Data.Read_Pointer		= BLOCK_CODESTORE_START;//IAP读指针
	//	Mcu_IAP_Flash_Data.Space_Size		= BLOCK_CODESTORE_SPACE;//芯片容量：250K字节
	//	Mcu_IAP_Flash_Data.Data_Block_Total	= 1;					//数据块数量	
	//	Mcu_IAP_Flash_Data.Data_Block_No	= 1;					//数据块编号
	//	Mcu_IAP_Flash_Data.Data_Block_Check	= 0;					//数据校验
	//	Mcu_IAP_Flash_Data.RX_COM_State		= FALSE;				//数据读取状态
	/* 传感器菜单项目 初始化 */
	DispSensorNumConfig(Sensor_CH_Config_Num);
	/* 数据库 初始化 */
	Mcu_Pgm_Ude_State_Check();	
#if ((AppGPRSComProtocolEnable == 0x01)&& \
	   ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04)))
	/* 初始化配置服务器参数 */
	ConfigService_InitPara(&Instru_Config_Inf_Tab);
#endif
	/* 复位系统时间计数器 */
	OSTimeSet(0);
}
/******************* (C) ZHOU *****END OF FILE****/
 
