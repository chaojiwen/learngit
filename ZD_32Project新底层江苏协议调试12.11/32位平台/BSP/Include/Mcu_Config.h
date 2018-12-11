#ifndef __Mcu_Config_H
#define __Mcu_Config_H

#include "stm32f10x.h"



/*************************************************************************************************
									用户定义参数
**************************************************************************************************/
/* 软件版本编号: 主版本.子版本.阶段版本 */
/* 主版本编号 */
#define Software_Version_NO_Major		   (2u)
/* 子版本编号 */
#define Software_Version_NO_Child		   (1u)
/* 阶段版本编号 */
#define Software_Version_NO_Stage		   (5u)

#ifndef Hardware_Version_V131
/* 硬件版本编号: 主版本.子版本.阶段版本 */
/* 主版本编号 */
#define Hardware_Version_NO_Major		   (2u)
/* 子版本编号 */
#define Hardware_Version_NO_Child		   (0u)
/* 阶段版本编号 */
#define Hardware_Version_NO_Stage		   (1u)
#else
/* 硬件版本编号: 主版本.子版本.阶段版本 */
/* 主版本编号 */
#define Hardware_Version_NO_Major		   (1u)
/* 子版本编号 */
#define Hardware_Version_NO_Child		   (3u)
/* 阶段版本编号 */
#define Hardware_Version_NO_Stage		   (1u)
#endif


/* 仪器编号数据缓存大小 */
#define Serial_Number_Menu_Buffer_Size 	(12u)
/* 版本信息的数据缓存大小 */
#define Version_Inf_Buffer_Size 				(18u)

/* 温湿度 传感器 的数据表大小 */
#define INSTRU_SENSOR_Data_Tab_Size			(8u)

/* 打印车牌号的数据长度 */
#define Print_Card_NO_Size							(12u)
/* 打印公司名称 */
#define Print_Company_Name_Size					(32u)
/* 记录卸货开始地址数组大小 */
#define Print_Start_upload_Adder_Size   (100u)
/* 记录卸货结束地址数组大小 */
#define Print_Stop_upload_Adder_Size    (100u)

//仪器配置接收表的大小
#define Instru_Config_Tab_Size	        (7u)


/* IAP 数据相关参数 */

#define FLASH_ACCESS_WIDTH				      (2)				// Flash读写宽度(2字节，注意Nand Flash不允许覆盖写)
// STM32 Flash的页大小(擦除Flash时须以页为单位)
#define FLASH_PAGE_SIZE					        (2048)			//2K	
// 系统默认的起始ROM和RAM地址
#define BASE_OF_ROM						          (0x08000000)
#define BASE_OF_RAM						          (0x20000000)

#define SIZE_OF_ROM						          (0x80000)	 	// 512K bytes
#define SIZE_OF_RAM						          (0x10000)		// 64K bytes

////////#define STARTUP_FROM_BOOTLOADER     (0x00)

#ifdef STARTUP_FROM_BOOTLOADER				// 应用程序自行启动，无需bootloader引导
	// | 250K App1 | 250K app2| 10K BOOT | 2k env |

	// APP代码区域 250K
	#define BLOCK_APPLICATION_START			(BASE_OF_ROM)
	#define BLOCK_APPLICATION_NUM			    (125)									// 125页
	#define BLCOK_APPLICATION_SPACE			(FLASH_PAGE_SIZE*BLOCK_APPLICATION_NUM)	// 250K空间
	
	// 新APP代码存储区域 250K
	#define BLOCK_CODESTORE_START			(BLOCK_APPLICATION_START+BLCOK_APPLICATION_SPACE)
	#define BLOCK_CODESTORE_NUM				    (125)									// 125页
	#define BLOCK_CODESTORE_SPACE			(FLASH_PAGE_SIZE*BLOCK_CODESTORE_NUM)	// 250K空间
	
	// Bootloader代码区域
	#define BLOCK_BOOTLOADER_START			(BLOCK_CODESTORE_START+BLOCK_CODESTORE_SPACE)
	#define BLOCK_BOOTLOADER_NUM			    (5)										// 5页
	#define BLOCK_BOOTLOADER_SPACE			(FLASH_PAGE_SIZE*BLOCK_BOOTLOADER_NUM)	// 10K空间

	// 系统参数区域	
	#define BLCOK_SYS_PARAM_START			(BLOCK_BOOTLOADER_START+BLOCK_BOOTLOADER_SPACE)
	#define BLCOK_SYS_PARAM_NUM				    (1)										// 1页
	#define BLCOK_SYS_PARAM_SPACE			(FLASH_PAGE_SIZE*BLCOK_SYS_PARAM_NUM)	// 2K空间

#else										// 应用程序由bootloader引导再启动
	// | 10K boot  | 2k env | 250K app1 | 250K app2|

	// Bootloader代码区域 10K
	#define BLOCK_BOOTLOADER_START			(BASE_OF_ROM)
	#define BLOCK_BOOTLOADER_NUM			    (5)										// 5页
	#define BLOCK_BOOTLOADER_SPACE			(FLASH_PAGE_SIZE*BLOCK_BOOTLOADER_NUM)	// 10K空间
	
	// 系统参数区域	2K
	#define BLCOK_SYS_PARAM_START			(BLOCK_BOOTLOADER_START+BLOCK_BOOTLOADER_SPACE)
	#define BLCOK_SYS_PARAM_NUM				    (1)										// 1页
	#define BLCOK_SYS_PARAM_SPACE			(FLASH_PAGE_SIZE*BLCOK_SYS_PARAM_NUM)	// 2K空间

	// APP代码区域 250K
	#define BLOCK_APPLICATION_START			(BLCOK_SYS_PARAM_START+BLCOK_SYS_PARAM_SPACE)
	#define BLOCK_APPLICATION_NUM			    (125)									// 125页
	#define BLCOK_APPLICATION_SPACE			(FLASH_PAGE_SIZE*BLOCK_APPLICATION_NUM)	// 250K空间

	// 新APP代码存储区域 250K
	#define BLOCK_CODESTORE_START			(BLOCK_APPLICATION_START+BLCOK_APPLICATION_SPACE)
	#define BLOCK_CODESTORE_NUM				    (125)									// 125页
	#define BLOCK_CODESTORE_SPACE			(FLASH_PAGE_SIZE*BLOCK_CODESTORE_NUM)	// 250K空间

#endif

// flash各个区块用途定义





// 应用程序是否被更新的标志(0x00: 未更新，0x01: 已更新)
#define ENV_APP_UPDATED_START			      (0u)		// 是否有新版程序被下载的标志在环境变量区域的相对起始位置
#define ENV_APP_UPDATED_SIZE			      (2u)		// 是否有新版程序被下载的标志的存储长度

/******************************************** 系统版本号 *******************************************/
// 系统SN号
#define ENV_SN_NO_START					(ENV_APP_UPDATED_START+ENV_APP_UPDATED_SIZE)
#define ENV_SN_NO_SIZE					        (6u)


//// 系统软件版本号
//#define ENV_VERSION_SOFTWARE_START			(ENV_ERR_DETECT_MOTION_START+ENV_ERR_DETECT_MOTION_SIZE)
//#define ENV_VERSION_SOFTWARE_SIZE			16		
//// 系统硬件版本号			
//#define ENV_VERSION_HARDWARE_START			(ENV_VERSION_SOFTWARE_START+ENV_VERSION_SOFTWARE_SIZE)		
//#define ENV_VERSION_HARDWARE_SIZE			16	 	

/* 仪器 系统 信息 空间大小 */
#define Instru_System_Inf_Space_Size						  (20u)
/* 仪器 电源 状态 信息 空间大小 */
#define Instru_Power_Inf_State_Space_Size					(12u)
/* 仪器 实时时钟RTC 信息 空间大小 */
#define Instru_RTC_Inf_State_Space_Size						(12u)
/* 仪器 GSM模块 状态信息 空间大小 */
#define Instru_GSM_Inf_State_Space_Size						(160u)
/* 仪器 传感器 状态信息 空间大小 */
#define Instru_Sensor_Inf_State_Space_Size				(768u)
/* 仪器 菜单 状态信息 空间大小 */
#define Instru_Menu_Inf_State_Space_Size					(48u)
/* 仪器 校准表 信息 空间大小 */
#define Instru_Cal_Tab_Space_Size							    (1024u)
/* 仪器 配置表 空间大小(2k字节) */
#define Instru_Inf_State_Space_Size							  (0X0800)
/* 仪器 传感器 状态信息 剩余空间大小 */
#define Instru_Inf_State_Surplus_Space_Size	(Instru_Inf_State_Space_Size		-\
											(Instru_System_Inf_Space_Size		+\
											 Instru_Power_Inf_State_Space_Size 	+\
											 Instru_RTC_Inf_State_Space_Size	+\
											 Instru_GSM_Inf_State_Space_Size	+\
											 Instru_Sensor_Inf_State_Space_Size	+\
											 Instru_Menu_Inf_State_Space_Size	+\
											 Instru_Cal_Tab_Space_Size))
									


/*Wifi模块参数设置*/
#define Wifi_Name_Size					                  (32u)
#define Wifi_Passward_Size					              (32u)
    /*Wifi模块Mac地址大小*/
#define Wifi_Mac_Address_Size                      (32u)
    /*Wifi模块本地IP大小*/
#define Wifi_Local_IP_Size					              (32u)
		/*Wifi模块子网掩码大小*/
#define Wifi_Subnet_Mask_Size					            (32u)
    /*Wifi模块网关IP地址大小*/
#define Wifi_Gateway_IP_Size					            (32u)
 /* 仪器 Wifi固定信息 空间大小 */                                                 
#define Instru_Wifi_Inf_Space_Size                (100u)





/* 仪器 系统固定信息 空间大小 */
#define Instru_System_Fix_Inf_Space_Size					 (16u)
/* 仪器 电源固定信息 空间大小 */                    
#define Instru_Power_Fix_Inf_Space_Size						 (16u)
/* 仪器 传感器固定信息 空间大小 */                  
#define Instru_Sensor_Fix_Inf_Space_Size					 (768u)
/* 仪器 Fram固定信息 空间大小 */                    
#define Instru_Fram_Fix_Inf_Space_Size						 (32u)
/* 仪器 网关固定信息 空间大小 */                    
#define Instru_Gateway_Fix_Inf_Space_Size					 (16u)
/* 仪器 Flash固定信息 空间大小 */                   
#define Instru_Flash_Fix_Inf_Space_Size						 (48u)
/* 仪器 通信1口固定信息 空间大小 */                 
#define Instru_Com1_Fix_Inf_Space_Size						 (48u)
/* 仪器 打印信息 空间大小 */                        
#define Instru_Print_Fix_Inf_Space_Size						 (888u)
/* 仪器 Wifi信息 空间大小 */  
#define Instru_Wifi_Fix_Inf_Space_Size					   (68u)
                                                   
                                                   
/* 仪器 固定信息 空间大小 */				                 
#define Instru_Fix_Inf_Tab_Space_Size						   (0X076C)//增加了Wifi网络名称及密码
/* 仪器 固定信息 剩余空间大小 */
#define Instru_Fix_Inf_Tab_Surplus_Space_Size	(Instru_Fix_Inf_Tab_Space_Size		-\
												(Instru_System_Fix_Inf_Space_Size	+\
												 Instru_Power_Fix_Inf_Space_Size	+\
												 Instru_Sensor_Fix_Inf_Space_Size	+\
												 Instru_Fram_Fix_Inf_Space_Size		+\
												 Instru_Gateway_Fix_Inf_Space_Size	+\
												 Instru_Flash_Fix_Inf_Space_Size	+\
												 Instru_Com1_Fix_Inf_Space_Size		+\
												 Instru_Print_Fix_Inf_Space_Size))
									
/* 仪器 序列号 空间大小 */ 
#define Serial_Number_Buffer_Size			             (8u)
/* 仪器 版本号 空间大小 */
#define Version_Buffer_Size					               (4u)
/* 传感器SN号空间大小 */
#define SENSOR_SN_NO_Size					                 (4u)	
/* 手机号码 空间大小 */
#define Mobile_No_Buffer_Size				               (8u)
/* 仪器 传感器 识别号 空间大小 */
#define SENSOR_ID_Buffer_Size				               (4u)
/* 报警时间段 空间大小 */
#define Workaday_Data_Buffer_Size			             (6u)
/* 配置表 信息 空间大小 */
#define Config_Tab_Chane_Size				               (5u)



/* 仪器 信息 配置表 在FRAM中的存储参数 */
/* 仪器 信息配置表存储在FRAM的 起始地址 */
#define Instru_Inf_Tab_Mem_Star 			             (0X1000)
/* 仪器 信息配置表存储在FRAM的 空间大小（这个应该跟配置表的大小一致，但先在考虑FRAM空间不足，先单独配置一个小的地方） */
#define Instru_Inf_Tab_Mem_Size 			             (0X0800)
/* 仪器 信息配置表存储在FRAM的 尾地址 */
#define Instru_Inf_Tab_Mem_End 				    (Instru_Inf_Tab_Mem_Star+Instru_Inf_Tab_Mem_Size)
/* 仪器 信息配置表存储在FRAM的 系统信息地址 */
#define Instru_Inf_Tab_Mem_System_adder		(Instru_Inf_Tab_Mem_Star)
/* 仪器 信息配置表存储在FRAM的 电源状态信息地址 */
#define Instru_Inf_Tab_Mem_Power_adder		(Instru_Inf_Tab_Mem_System_adder+Instru_System_Inf_Space_Size)
/* 仪器 信息配置表存储在FRAM的 实时时钟信息地址 */
#define Instru_Inf_Tab_Mem_RTC_adder		  (Instru_Inf_Tab_Mem_Power_adder+Instru_Power_Inf_State_Space_Size)	
/* 仪器 信息配置表存储在FRAM的 GSM模块信息地址 */
#define Instru_Inf_Tab_Mem_GSM_adder		  (Instru_Inf_Tab_Mem_RTC_adder+Instru_RTC_Inf_State_Space_Size)	
/* 仪器 信息配置表存储在FRAM的 传感器信息地址 */
#define Instru_Inf_Tab_Mem_Sensor_adder		(Instru_Inf_Tab_Mem_GSM_adder+Instru_GSM_Inf_State_Space_Size)	
/* 仪器 信息配置表存储在FRAM的 报警总开关信息地址 */
#define Instru_Inf_Tab_Mem_Menu_adder 		(Instru_Inf_Tab_Mem_Sensor_adder+Instru_Sensor_Inf_State_Space_Size)
/* 仪器 信息配置表存储在FRAM的 校准表信息地址 */
#define Instru_Inf_Tab_Mem_Cal_adder		  (Instru_Inf_Tab_Mem_Menu_adder+Instru_Menu_Inf_State_Space_Size)


/* 仪器 固定信息表 在FRAM中的存储参数 */
/* 仪器 固定信息表 存储在FRAM的 起始地址 */
#define Instru_Fix_Inf_Tab_Mem_Star 			    (0X1800)
/* 仪器 固定信息表 存储在FRAM的 空间大小 */     
#define Instru_Fix_Inf_Tab_Mem_Size 			    (0X076C)//增加了Wifi网络名称及密码
/* 仪器 固定信息表 存储在FRAM的 尾地址 */      
#define Instru_Fix_Inf_Tab_Mem_End 				    (Instru_Fix_Inf_Tab_Mem_Star+Instru_Fix_Inf_Tab_Mem_Size)
/* 仪器 固定信息表 存储在FRAM的 系统信息地址 */
#define Instru_Fix_Inf_Tab_Mem_System_adder		(Instru_Fix_Inf_Tab_Mem_Star)
/* 仪器 固定信息表 存储在FRAM的 电源状态信息地址 */			
#define Instru_Fix_Inf_Tab_Mem_Power_adder		(Instru_Fix_Inf_Tab_Mem_System_adder+		  Instru_System_Fix_Inf_Space_Size)
/* 仪器 固定信息表 存储在FRAM的 传感器信息地址 */                                         
#define Instru_Fix_Inf_Tab_Mem_Sensor_adder		(Instru_Fix_Inf_Tab_Mem_Power_adder+		  Instru_Power_Fix_Inf_Space_Size)
/* 仪器 信息配置表存储在FRAM的 存储数据块FRAM信息地址 */                                  
#define Instru_Fix_Inf_Tab_Mem_Fram_adder		  (Instru_Fix_Inf_Tab_Mem_Sensor_adder+		  Instru_Sensor_Fix_Inf_Space_Size)
/* 仪器 信息配置表存储在FRAM的 存储数据块FRAM信息地址 */                                  
#define Instru_Fix_Inf_Tab_Mem_Gateway_adder	(Instru_Fix_Inf_Tab_Mem_Fram_adder+			  Instru_Fram_Fix_Inf_Space_Size)
/* 仪器 信息配置表存储在FRAM的 存储数据块Flash信息地址 */
#define Instru_Fix_Inf_Tab_Mem_Flash_adder		(Instru_Fix_Inf_Tab_Mem_Gateway_adder+		Instru_Gateway_Fix_Inf_Space_Size)
/* 仪器 信息配置表存储在FRAM的 存储数据块Com1信息地址 */
#define Instru_Fix_Inf_Tab_Mem_Com1_adder		  (Instru_Fix_Inf_Tab_Mem_Flash_adder+		  Instru_Flash_Fix_Inf_Space_Size)
/* 仪器 信息配置表存储在FRAM的 存储数据块打印信息地址 */
#define Instru_Fix_Inf_Tab_Mem_Print_adder		(Instru_Fix_Inf_Tab_Mem_Com1_adder+			  Instru_Com1_Fix_Inf_Space_Size)
/* 仪器 信息配置表存储在FRAM的 存储数据块Wifi模块网络连接名称、密码及加密方式地址 */
#define Instru_Fix_Inf_Tab_Mem_Wifi_adder		  (Instru_Fix_Inf_Tab_Mem_Print_adder+			Instru_Wifi_Fix_Inf_Space_Size)




/*************************************************************************************************/
/************************ 注意: 以下参数不宜擅自修改，以免造成系统运行异常 ***********************/
/*************************************************************************************************/
/* 传感器 序列号 数据结构 */
typedef union 
{
	struct 
	{	
		/* 第1个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_1			:4;
		/* 第2个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_2			:4;
		/* 第3个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_3			:4;
		/* 第4个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_4			:4;
		/* 第5个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_5			:4;
		/* 第6个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_6			:4;
		/* 第7个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_7			:4;
		/* 第8个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_8			:4;
		/* 第9个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_9			:4;
		/* 第10个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_10			:4;
		/* 第11个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_11			:4;
		/* 第12个 SN 编码 */	//	程序运行后自行设置。（该选项不可更改）
		u32 Code_12			:4;
	}Serial_Number_Code;
	u8 Serial_Number[Serial_Number_Buffer_Size];
}SERIAL_NUMBER;

/* 传感器 版本号 数据结构 */
typedef union 
{
	struct 
	{
		/* 版本号 低位 *///	程序运行后自行设置。（该选项不可更改）
		u8 Code_C;
		/* 版本号 中位 *///	程序运行后自行设置。（该选项不可更改）
		u8 Code_B;
		/* 版本号 高位 *///	程序运行后自行设置。（该选项不可更改）
		u8 Code_A;
	}Serial_Number_Code;
	u8 Serial_Number[Version_Buffer_Size];
}VERSION;


/* 系统 信息 */
typedef union 
{
	struct 
	{
		/* 序列号 */										//	0 0 0 0 0 0 0 0 0 0 0 0	:未配置过的序列号（默认配置），出产前需要配置该项内容。
		SERIAL_NUMBER 	Serial_Number;					//	SN只能在配置工具上面修该，平台软件无法修改。
//		/* 序列号 保留 */
//		u32 			Serial_Number_Reserve		:16;//	00:默认配置
		/* ******************************************* */
		/* 软件版本 */									//	00(高) 00（中） 00（低）：版本号只能在配置工具上面修该，平台软件无法修改。
														//	软件版本号是有程序决定的，因此不需要设置。
		VERSION 		Software_Version;
//		/* 软件版本 保留 */
//		u32 			Software_Version_Reserve	:8; //	00:默认配置
		/* ******************************************* */
		/* 硬件版本 */									//	00(高) 00（中） 00（低） :未配置过的版本号，出产前需要配置该项内容。
														//	依据修改的重要性 依次 向曾加
		VERSION 		Hardware_Version;
//		/* 硬件版本 保留 */
//		u32 			Hardware_Version_Reserve	:8; //  00:默认配置
		/* ******************************************* */
		/* 远程升级 功能项 */								//	00:未配置（默认配置）	01:远程升级（升级提醒）	02:远程升级(立即升级)
		u8 				UpdateFlag;
		/* 仪器类型 */									//	00:默认配置
		u8				Instru_Type;
	}Inf;
	u8 Inf_Space[Instru_System_Inf_Space_Size];
}INSTRU_SYSTEM;

/* 电源 状态 信息*/
typedef union 
{
	struct 
	{
		/* 外接电源 状态 */			//	TRUE:外电连接（默认配置）	FALSE:外电未连接
		BOOL ET_Link_State;
		/* 外接电源 声报警开关 */		//	TRUE:报警开（默认配置）	FALSE:报警关
		BOOL ET_Sound_Alarm_Switch;
		/* 外接电源 光报警开关 */		//	TRUE:报警开（默认配置）	FALSE:报警关
		BOOL ET_Light_Alarm_Switch;
		/* 外接电源 短信报警开关 */	//	TRUE:报警开				FALSE:报警关（默认配置）
		BOOL ET_SMS_Alarm_Switch;
		/* 外接电源 报警状态 */		//	TRUE:已报警				FALSE:未报警（默认配置）
		BOOL ET_Alarm_State;


		/* 电池 充电状态 */			//	TRUE:电池正在充电		FALSE:电池未充电（默认配置）
		BOOL BT_Charge_State;
		/* 电池 电量状态 */			//	0:电池有0格电(没电，GSM无法工作)
									//	1:电池有1格电	
									//	2:电池有2格电	
									//	3:电池有3格电	
									//	4:电池有4格电（默认配置）	
									//	5:状态保留		
									//	6:电池未接(暂时没有这个功能)
		u8	 BT_Quantity_State;
		/* 电池 声报警开关 */		//	TRUE:报警开				FALSE:报警关（默认配置）
		BOOL BT_Sound_Alarm_Switch;
		/* 电池 光报警开关 */		//	TRUE:报警开				FALSE:报警关（默认配置）
		BOOL BT_Light_Alarm_Switch;
		/* 电池 短信报警开关 */		//	TRUE:报警开				FALSE:报警关（默认配置）
		BOOL BT_SMS_Alarm_Switch;
		/* 电池 报警状态 */			//	TRUE:已报警				FALSE:未报警（默认配置）
		BOOL BT_Alarm_State;

	}Inf_State;
	u8 Inf_State_Space[Instru_Power_Inf_State_Space_Size];
}INSTRU_POWER; 

/* 实时时钟 信息 */
typedef union
{
	struct 
	{
		/* 实时时钟 秒 参数 */	//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项 可更改）（BCD码格式）		
		u8 Sec					;  
		/* 实时时钟 分 参数 */	//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项 可更改）（BCD码格式）
		u8 Min					; 	
		/* 实时时钟 时 参数 */	//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项 可更改）（BCD码格式）
		u8 Hour					;  	
		/* 实时时钟 星期 参数 */	//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项 可更改）（BCD码格式）
		u8 Week					;  
		/* 实时时钟 日期 参数 */	//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项 可更改）（BCD码格式）
		u8 Day					;  
		/* 实时时钟 月份 参数 */	//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项 可更改）（BCD码格式）
		u8 Month				;  	
		/* 实时时钟 年份 参数 */	//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项 可更改）（BCD码格式）
		u16 Year				;
		/* 实时时钟 时区 参数 */	//	08:默认配置（东8区），出厂前需要配置该项内容。(该选项 可更改)（BCD码格式）
		int	Zone				;

	}Inf_State;
	u8 Inf_State_Space[Instru_RTC_Inf_State_Space_Size];
}INSTRU_RTC;


/* 服务器 地址 */
typedef union 
{
	struct 
	{	/* 格式:A.B.C.D(255.255.255.255) */
		/* 地址 A */			//	00:未配置（默认配置）。(该选项可更改)
		u8 Address_A;
		/* 地址 B */			//	00:未配置（默认配置）。(该选项可更改)
		u8 Address_B;
		/* 地址 C */			//	00:未配置（默认配置）。(该选项可更改)
		u8 Address_C;
		/* 地址 D */			//	00:未配置（默认配置）。(该选项可更改)
		u8 Address_D;
	}Serial_Number_Number;
	u32 Server_Address;
}SERVER_IP_ADDRESS;

/* 手机 号码 结构 */
typedef union 
{
	struct 
	{	
		/* 第1个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_1			:4;
		/* 第2个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_2			:4;
		/* 第3个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_3			:4;
		/* 第4个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_4			:4;
		/* 第5个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_5			:4;
		/* 第6个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_6			:4;
		/* 第7个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_7			:4;
		/* 第8个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_8			:4;
		/* 第9个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_9			:4;
		/* 第10个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_10			:4;
		/* 第11个 SN 编码 */	//	00:未配置（默认配置）。(该选项可更改)
		u32 Number_11			:4;
	}Serial_Number_Number;
	u8 Mobile_No[Mobile_No_Buffer_Size];
}MOBILE_NO;

/* GSM模块 状态信息 */
typedef union
{
	struct
	{
		/* GSM模块 对应服务器1 域名 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		u8  Domain_Name_ID1[32];
		/* GSM模块 对应服务器1 IP地址 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		SERVER_IP_ADDRESS  IP_Address_ID1	;
		/* GSM模块 对应服务器1 端口 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		u16 Port_ID1						;
		/* GSM模块 服务器1保留 */			//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
		u16 GSM_Inf_ID1_Reserve				;
		
		/* GSM模块 对应服务器2 域名 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		u8 	Domain_Name_ID2[32];
		/* GSM模块 对应服务器2 IP地址 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		SERVER_IP_ADDRESS 	IP_Address_ID2;
		/* GSM模块 对应服务器2 端口 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		u16 Port_ID2						;
		/* GSM模块 保留 */				//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
		u16 GSM_Inf_ID2_Reserve				;
		
		/* GSM模块 对应服务器3 域名 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		u8 	Domain_Name_ID3[32];
		/* GSM模块 对应服务器3 IP地址 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		SERVER_IP_ADDRESS 	IP_Address_ID3	;
		/* GSM模块 对应服务器3 端口 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		u16 Port_ID3						;
		/* GSM模块 保留 */				//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
		u16 GSM_Inf_ID3_Reserve				;

		/* GSM模块 报警短信号码_1 */		//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		MOBILE_NO 	Alarm_SMS_NO1;
		/* GSM模块 报警短信号码_2 */		//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		MOBILE_NO	Alarm_SMS_NO2;
		/* GSM模块 报警短信号码_3 */		//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		MOBILE_NO 	Alarm_SMS_NO3;
		/* GSM模块 报警短信 备用4 */		//	00:未配置（默认配置）。(该选项 可更改，预留)
		MOBILE_NO 	Alarm_SMS_NO_Reserve4;
		/* GSM模块 报警短信 备用5 */		//	00:未配置（默认配置）。(该选项 可更改，预留)
		MOBILE_NO 	Alarm_SMS_NO_Reserve5;
		

	}Inf_State;
	u8 Inf_State_Space[Instru_GSM_Inf_State_Space_Size];
}INSTRU_GSM;

    			  		  
/* 仪器 传感器 通道信息结构 */
typedef struct {
	/* 卡片ID号 */							//	0:未配置(默认配置)
	u32				SENSOR_RF_Card_ID_NO;
	/* 传感器 SN号 */						//	0:未配置(默认配置)
	u8				SENSOR_SN_NO[SENSOR_SN_NO_Size];
	/* 传感器 报警上限 */						//	00:未配置(默认配置)，出厂前需要配置该项内容。(该选项 可更改)
	float			SENSOR_Up_Limit;
	/* 传感器 报警下限 */						//	00:未配置(默认配置)，出厂前需要配置该项内容。(该选项 可更改)
	float			SENSOR_Low_Limit;
	/* 传感器 类型 */						
	u8		Type;							//	0:没有设定的传感器类型	
											//	1:温度传感器		
											//	2:湿度传感器		
											//	3:单温卡片	
											//	4:温湿卡片->温度
											//	5:温湿卡片->湿度	
											//	6:经度	
											//	7:纬度
											//	8:速度
											//	其他:无效
	
	/* 传感器 报警开关 */						//	TRUE:报警开(默认配置)		FALSE:报警关
	BOOL	SENSOR_Alarm_Switch;
	/* 传感器 声音报警开关 */					//	TRUE:报警开				FALSE:报警关(默认配置)
	BOOL	SENSOR_Sound_Alarm_Switch;
	/* 传感器 光报警开关 */					//	TRUE:报警开				FALSE:报警关(默认配置)
	BOOL	SENSOR_Light_Alarm_Switch;
	/* 传感器 短信报警开关 */					//	TRUE:报警开				FALSE:报警关(默认配置)
	BOOL	SENSOR_SMS_Alarm_Switch;
	/* 传感器 报警状态 */						//	0x0:传感器未报警(默认配置)	0x1:上限报警 0x02:下限报警			
	u8		SENSOR_Alarm_State;
	
}INSTRU_SENSOR_CHANNEL;

/* 传感器 状态信息 */
typedef union 
{
	struct
	{
		/* 传感器 通道1 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH1;
		/* 传感器 通道2 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH2;
		/* 传感器 通道3 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH3;
		/* 传感器 通道4 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH4;
		/* 传感器 通道5 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH5;
		/* 传感器 通道6 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH6;
		/* 传感器 通道7 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH7;
		/* 传感器 通道8*/			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH8;
		/* 传感器 通道9 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH9;
		/* 传感器 通道10 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH10;
		/* 传感器 通道11 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH11;
		/* 传感器 通道12 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH12;
		/* 传感器 通道13 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH13;
		/* 传感器 通道14 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH14;
		/* 传感器 通道15 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH15;
		/* 传感器 通道16 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH16;
		/* 传感器 通道17 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH17;
		/* 传感器 通道18 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH18;
		/* 传感器 通道19 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH19;
		/* 传感器 通道20 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH20;
		/* 传感器 通道21 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH21;
		/* 传感器 通道22 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH22;
		/* 传感器 通道23 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH23;
		/* 传感器 通道24 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH24;
		/* 传感器 通道25 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH25;
		/* 传感器 通道26 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH26;
		/* 传感器 通道27 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH27;
		/* 传感器 通道28 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH28;
		/* 传感器 通道29 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH29;
		/* 传感器 通道30 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH30;
		/* 传感器 通道31 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH31;
		/* 传感器 通道32 */			//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
		INSTRU_SENSOR_CHANNEL		CH32;

	}Inf_State;
	u8 Inf_State_Space[Instru_Sensor_Inf_State_Space_Size];
}INSTRU_SENSOR;

/* 报警时间段 数据结构 */
typedef union 
{
	struct 
	{	
		/* 报警时间段 模式设置 */			//  00:关（默认配置）01:上班时间段	02:非上班时间段	03:自定义时间段 04:全部时间段
		u32  Alarm_Mode			:8;
		/* 周一 报警 设置 */				//	00:报警关（默认配置）01:报警开（自定义时间模式下启动）
		u32	 Alarm_Week_1		:1;
		/* 周二 报警 设置 */				//	00:报警关（默认配置）01:报警开（自定义时间模式下启动）
		u32	 Alarm_Week_2		:1;
		/* 周三 报警 设置 */				//	00:报警关（默认配置）01:报警开（自定义时间模式下启动）
		u32	 Alarm_Week_3		:1;
		/* 周四 报警 设置 */				//	00:报警关（默认配置）01:报警开（自定义时间模式下启动）
		u32	 Alarm_Week_4		:1;
		/* 周五 报警 设置 */				//	00:报警关（默认配置）01:报警开（自定义时间模式下启动）
		u32	 Alarm_Week_5		:1;
		/* 周六 报警 设置 */				//	00:报警关（默认配置）01:报警开（自定义时间模式下启动）
		u32	 Alarm_Week_6		:1;
		/* 周日 报警 设置 */				//	00:报警关（默认配置）01:报警开（自定义时间模式下启动）
		u32	 Alarm_Week_7		:1;
		/* 周 功能 保留 */				//	00:未配置（默认配置）。(该选项不可更改)
		u32	 Alarm_Reserve		:1;
		/* 报警时间段 上班时间 时 */		//	08:未配置（默认配置）(该选项可更改)（自定义时间模式下启动）
		u32  OnWork_Time_Hour	:8;			
		/* 报警时间段 上班时间 分 */		//	00:未配置（默认配置）(该选项可更改)（自定义时间模式下启动）
		u32  OnWork_Time_Min	:8;		
		/* 报警时间段 下班时间 时 */		//	17:未配置（默认配置）(该选项可更改)（自定义时间模式下启动）
		u32  OffWork_Time_Hour	:8;			
		/* 报警时间段 下班时间 分 */		//	00:未配置（默认配置）(该选项可更改)（自定义时间模式下启动）
		u32  OffWork_Time_Min	:8;
	}Inf;
	u8 Workaday_Data[Workaday_Data_Buffer_Size];
}WORKADAY;

/* 菜单控制 信息*/
typedef union 
{
	struct 
	{
		/* 菜单控制 声音报警总开关 */		//	FALSE:关 声音 报警（默认配置），	TRUE:开 声音 报警
		BOOL Sound_Alarm_Master_Switch	;
		/* 菜单控制 光  报警总开关 */		//	FALSE:关 光 报警（默认配置），		TRUE:开 光 报警
		BOOL Light_Alarm_Master_Switch	;
		/* 菜单控制 短信报警总开关 */		//	FALSE:关 短信 报警（默认配置），	TRUE:开 短信 报警
		BOOL SMS_Alarm_Master_Switch	;
		/* 菜单控制 报警正常 短信提示 */	//	FALSE:关（默认配置），			TRUE:开 
		BOOL Alarm_Normal_SMS			;
		/* 声音 报警的 延时 时间 */		//	默认配置:300s 范围:0-4294967295（单位：秒）
		u32 Sound_Alarm_Delay_Time		;
		/* 光   报警的 延时 时间 */		//	默认配置:300s 范围:0-4294967295（单位：秒）
		u32 Light_Alarm_Delay_Time		;
		/* 短信 报警的 延时 时间 */		//	默认配置:300s 范围:0-4294967295（单位：秒）
		u32 SMS_Alarm_Delay_Time		;
		/* 采集数据间隔 */				//	默认配置:1s 范围:0-4294967295（单位：秒）
		u32 Collect_Data_Interval		;
		/* 报警数据记录间隔 */			//	默认配置:120s 范围:0-4294967295（单位：秒）
		u32 Alarm_Save_Data_Interval	;
		/* 正常数据记录间隔 */			//	默认配置:300s 范围:0-4294967295（单位：秒）
		u32 Normal_Save_Data_Interval	;
		/* 上传数据间隔 */				//	默认配置:60s 范围:0-4294967295（单位：秒）
		u32 Upload_Data_Interval		;
		/* 短信重复报警时间间隔 */			//	默认配置:0s 范围:0-4294967295（单位：秒）
		u32 SMS_Repeat_Alarm_Interval	;
		/* 实时数据存储开关 */			//	FALSE:关 存储功能，	TRUE:开 （默认配置）.
		FunctionalState Menu_Save_Data_Switch;
	}Inf_State;
	u8 Inf_State_Space[Instru_Menu_Inf_State_Space_Size];
}INSTRU_MENU;

/*传感器 数据表 结构 */
typedef union 
{
	struct
	{
		/* 通道1 真实值 */
		u16 CH1_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道2 真实值 */
		u16 CH2_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道3 真实值 */
		u16 CH3_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道4 真实值 */
		u16 CH4_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道5 真实值 */
		u16 CH5_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道6 真实值 */
		u16 CH6_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道7 真实值 */
		u16 CH7_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道8 真实值 */
		u16 CH8_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道9 真实值 */
		u16 CH9_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道10 真实值 */
		u16 CH10_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道11 真实值 */
		u16 CH11_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道12 真实值 */
		u16 CH12_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道13 真实值 */
		u16 CH13_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道14 真实值 */
		u16 CH14_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道15 真实值 */
		u16 CH15_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道16 真实值 */
		u16 CH16_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道17 真实值 */
		u16 CH17_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道18 真实值 */
		u16 CH18_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道19 真实值 */
		u16 CH19_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道20 真实值 */
		u16 CH20_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道21 真实值 */
		u16 CH21_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道22 真实值 */
		u16 CH22_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道23 真实值 */
		u16 CH23_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道24 真实值 */
		u16 CH24_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道25 真实值 */
		u16 CH25_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道26 真实值 */
		u16 CH26_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道27 真实值 */
		u16 CH27_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道28 真实值 */
		u16 CH28_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道29 真实值 */
		u16 CH29_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道30 真实值 */
		u16 CH30_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道31 真实值 */
		u16 CH31_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道32 真实值 */
		u16 CH32_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];


		/* 通道1 校准值 */
		u16 CH1_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道2 校准值 */
		u16 CH2_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道3 校准值 */
		u16 CH3_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道4 校准值 */
		u16 CH4_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道5 校准值 */
		u16 CH5_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道6 校准值 */
		u16 CH6_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道7 校准值 */
		u16 CH7_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道8 校准值 */
		u16 CH8_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道9 校准值 */
		u16 CH9_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道10 校准值 */
		u16 CH10_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道11 校准值 */
		u16 CH11_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道12 校准值 */
		u16 CH12_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道13 校准值 */
		u16 CH13_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道14 校准值 */
		u16 CH14_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道15 校准值 */
		u16 CH15_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道16 校准值 */
		u16 CH16_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道17 校准值 */
		u16 CH17_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道18 校准值 */
		u16 CH18_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道19 校准值 */
		u16 CH19_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道20 校准值 */
		u16 CH20_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道21 校准值 */
		u16 CH21_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道22 校准值 */
		u16 CH22_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道23 校准值 */
		u16 CH23_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道24 校准值 */
		u16 CH24_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道25 校准值 */
		u16 CH25_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道26 校准值 */
		u16 CH26_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道27 校准值 */
		u16 CH27_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道28 校准值 */
		u16 CH28_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道29 校准值 */
		u16 CH29_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道30 校准值 */
		u16 CH30_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道31 校准值 */
		u16 CH31_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* 通道32 校准值 */
		u16 CH32_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];

		
	}Inf_State;
	u8 Inf_State_Space[Instru_Cal_Tab_Space_Size];
}INSTRU_DATA_TAB;

/* 仪器 的状态信息表 */
typedef union 
{
	struct
	{
		/* 系统 状态信息  */
		INSTRU_SYSTEM	Instru_System;
		
		/* 在 系统系的 仪器类型 选项可以选择对应类型的数据结构来配置后面的地址内容。 */
		/* 电源 状态信息 */
		INSTRU_POWER	Instru_Power;
		/* 实时时钟 信息 */
		INSTRU_RTC		Instru_RTC;
		/* GSM 状态 信息 */
		INSTRU_GSM		Instru_GSM;
		/* 传感器 状态 信息 */
		INSTRU_SENSOR	Instru_Sensor;
		/* 菜单控制 状态信息 */
		INSTRU_MENU		Instru_Menu;
		/* 传感器通道 数据表 */
		INSTRU_DATA_TAB	Instru_Data_Tab;
		
//		/* 仪器 信息 保留空间 */
//		u8 Inf_State_Reserve[Instru_Inf_State_Surplus_Space_Size-4];//保留空间//这里的减1是因为后面还有一个 配置标志
		
		/* 仪器配置表 设置 标志 */			//	00:未配置过（默认配置），01:已经完成配置
		u8 Data_Update_Flag;				//	00:未配置过（默认配置），程序运行后自行设置。（该选项 可更改）
	}Inf_State;/* 对应模块的状态信息 */
	u8 Inf_State_Space[Instru_Inf_State_Space_Size];
}INSTRU_CONFIG_INF_TAB;/* 仪器配置信息表 */

/* 系统 固定信息 */
typedef union 
{
	struct 
	{
		/* 系统 语言 */								//	00:中文（默认配置），	01:英文		其他:无效
		u8	 System_Language						;
	}Inf_State;	
	u8 Inf_Space[Instru_System_Fix_Inf_Space_Size];
}INSTRU_SYSTEM_FIX;

/* 电源 固定信息 */
typedef union
{
	struct
	{
		/* 外接电源 状态改变过滤时间 *///	00:未配置过（默认配置），程序运行后自行设置。（该选项 可更改）
		u8 ET_Status_Update_Time;
		/* 电池 状态改变过滤时间 */	//	00:未配置过（默认配置），程序运行后自行设置。（该选项 可更改）
		u8 BT_Status_Update_Time;
		/* 电池待能量的ADC采样值 */	//	00:未配置过（默认配置），程序运行后自行设置。（该选项不可更改）
		u16	BT_Quantity_Sampl_Value;
	}Inf;
	u8 Inf_Space[Instru_Power_Fix_Inf_Space_Size];
}INSTRU_POWER_FIX;


/* 仪器 传感器  固定信息 通道数据结构 */
typedef struct {
	/* 传感器 仪器采样值 */			//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
	float Sensor_Sampl_Data;
	/* 传感器 实时数据（校准值） */	//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项 可更改）
	float Sensor_Real_Time_Data;
	/* 传感器 报警上限时间计数寄存器 *///	00:未配置过的地址（默认配置）。（程序运行后自行设置。（该选项 可更改）
	u16 Sensor_Up_Alarm_Cnt;
	/* 传感器 报警下限时间计数寄存器 *///	00:未配置过的地址（默认配置）。（程序运行后自行设置。（该选项 可更改）
	u16 Sensor_Low_Alarm_Cnt;
	/* 传感器 报警取消标志 */			//	00:未设置（默认配置），01:取消报警；程序运行后自行设置。（该选项 可更改）
	BOOL	Sensor_Suspend_Alarm_Flag;
	/* 传感器 电量状态 */				//	00: 电池电量低报警（默认配置）；01:电池电量正常；
	u8	Sensor_Power_State;
	/* 传感器 报警延时时间 */			//	00:未设置（默认配置）
	u16	Sensor_Alarm_Delay;
	/* 传感器 数据做假 上限 */
	float Sensor_Fraud_Up_Limit;	
	/* 传感器 数据做假 下限 */
	float Sensor_Fraud_Low_Limit;
	
}INSTRU_SENSOR_CHANNEL_FIX;

/* 传感器 固定信息 */
typedef union
{
	struct
	{
		/* 通道 1 */
		INSTRU_SENSOR_CHANNEL_FIX		CH1;
		/* 通道 2 */
		INSTRU_SENSOR_CHANNEL_FIX		CH2;
		/* 通道 3 */
		INSTRU_SENSOR_CHANNEL_FIX		CH3;
		/* 通道 4 */
		INSTRU_SENSOR_CHANNEL_FIX		CH4;
		/* 通道 5 */
		INSTRU_SENSOR_CHANNEL_FIX		CH5;
		/* 通道 6 */
		INSTRU_SENSOR_CHANNEL_FIX		CH6;
		/* 通道 7 */
		INSTRU_SENSOR_CHANNEL_FIX		CH7;
		/* 通道 8 */
		INSTRU_SENSOR_CHANNEL_FIX		CH8;
		/* 通道 9 */
		INSTRU_SENSOR_CHANNEL_FIX		CH9;
		/* 通道 10 */
		INSTRU_SENSOR_CHANNEL_FIX		CH10;
		/* 通道 11 */
		INSTRU_SENSOR_CHANNEL_FIX		CH11;
		/* 通道 12 */
		INSTRU_SENSOR_CHANNEL_FIX		CH12;
		/* 通道 13 */
		INSTRU_SENSOR_CHANNEL_FIX		CH13;
		/* 通道 14 */
		INSTRU_SENSOR_CHANNEL_FIX		CH14;
		/* 通道 15 */
		INSTRU_SENSOR_CHANNEL_FIX		CH15;
		/* 通道 16 */
		INSTRU_SENSOR_CHANNEL_FIX		CH16;
		/* 通道 17 */
		INSTRU_SENSOR_CHANNEL_FIX		CH17;
		/* 通道 18 */
		INSTRU_SENSOR_CHANNEL_FIX		CH18;
		/* 通道 19 */
		INSTRU_SENSOR_CHANNEL_FIX		CH19;
		/* 通道 20 */
		INSTRU_SENSOR_CHANNEL_FIX		CH20;
		/* 通道 21 */
		INSTRU_SENSOR_CHANNEL_FIX		CH21;
		/* 通道 22 */
		INSTRU_SENSOR_CHANNEL_FIX		CH22;
		/* 通道 23 */
		INSTRU_SENSOR_CHANNEL_FIX		CH23;
		/* 通道 24 */
		INSTRU_SENSOR_CHANNEL_FIX		CH24;
		/* 通道 25 */
		INSTRU_SENSOR_CHANNEL_FIX		CH25;
		/* 通道 26 */
		INSTRU_SENSOR_CHANNEL_FIX		CH26;
		/* 通道 27 */
		INSTRU_SENSOR_CHANNEL_FIX		CH27;
		/* 通道 28 */
		INSTRU_SENSOR_CHANNEL_FIX		CH28;
		/* 通道 29 */
		INSTRU_SENSOR_CHANNEL_FIX		CH29;
		/* 通道 30 */
		INSTRU_SENSOR_CHANNEL_FIX		CH30;
		/* 通道 31 */
		INSTRU_SENSOR_CHANNEL_FIX		CH31;
		/* 通道 32 */
		INSTRU_SENSOR_CHANNEL_FIX		CH32;

	}Inf;
	u8 Inf_Space[Instru_Sensor_Fix_Inf_Space_Size];
}INSTRU_SENSOR_FIX;

/* 存储数据块FRAM 状态信息 */
typedef union 
{
	struct 
	{
		/* 存储数据块FRAM 工作状态 */
		u8 Chip_Work_State; 			//	00:芯片异常（默认配置） 01:芯片正常 	程序运行后自行设置。（该选项不可更改）
	}Inf_State;
	u8 Inf_State_Space[Instru_Fram_Fix_Inf_Space_Size];
}INSTRU_FRAM_FIX;

/* 网关存储信息 */
typedef union 
{
	struct 
	{
		/* 网关地址 */
		uint8_t Adder;
		/* 信道(波段) */
		uint8_t Channel;
		/* 数据丢失时间 */
		uint8_t ActiveTime;
		/* 工作模式（正常/调试） */
		uint8_t Mode;
		/* 通信波特率 */
		uint32_t Baudrate;
		/* 网关工作状态 */
		uint8_t WorkState;
	}Inf_State;
	u8 Inf_State_Space[Instru_Gateway_Fix_Inf_Space_Size];
}INSTRU_Gateway_FIX;

/* 存储数据块Flash 信息状态 */
typedef union 
{
	struct 
	{
		/* 存储数据块Flash 芯片容量 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项不可更改)
		u32 Chip_Capacity;
		/* 存储数据块Flash 数据 页 大小 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项不可更改)
		u32 Chip_Page_Size;
		/* 存储数据块Flash 数据 页 数量 */	//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项不可更改)
		u32 Chip_Page_Num;
		/* 存储数据块Flash 数据 扇区 大小 *///	00:未配置（默认配置），出厂前需要配置该项内容。(该选项不可更改)
		u32 Sector_Size;
		/* 存储数据块Flash 数据 扇区 指针 *///	00:未配置（默认配置），出厂前需要配置该项内容。(该选项不可更改)
		u32 Sector_Pointer;	
		/* 存储数据块Flash 数据存储数量 */	//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
		u32 Storage_Data_NUM;
		/* 存储数据块Flash 写指针 */		//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
		s32	Write_Pointer;	
		/* 存储数据块Flash 读指针 */		//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
		s32	Read_Pointer;	

		/* 存储数据块Flash 工作状态 */
		ErrorStatus Chip_Work_State; //	00:芯片异常（默认配置） 01:芯片正常 	程序运行后自行设置。（该选项不可更改）
	}Inf_State;
	u8 Inf_State_Space[Instru_Flash_Fix_Inf_Space_Size];
}INSTRU_FLASH_FIX;

/* 串口1通信 状态信息 */
typedef union 
{
	struct 
	{
		/* 接收数据数量 寄存器 */			//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
		u8 	Rece_Com_Data_Size;
		/* 数据帧长度 寄存器 */			//	00:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
		u8	Rece_Com_Packet_Len;
		/* 数据接收 超时保护 寄存器 */		//	300:未配置过的地址（默认配置），程序运行后自行设置。（该选项不可更改）
		signed short int Rece_Com_Timeout;
	}Inf_State;
	u8 Inf_State_Space[Instru_Com1_Fix_Inf_Space_Size];
}INSTRU_COM1_FIX;

/* 打印数据索引 */
typedef union
{
	struct
	{
		/* 打印起始时间 */
		u32 Print_Start_Time;
		/* 打印结束时间 */
		u32 Print_End_Time;
		/* 记录的打印起始时间 */
		u32 Print_Record_Start_Time;
		/* 记录的打印结束时间 */
		u32 Print_Record_End_Time;
		/* 记录的打印起始数据地址（存在Flash的地址） */
		s32 Print_Record_Start_Adder;
		/* 记录的打印结束数据地址（存在Flash的地址） */
		s32 Print_Record_End_Adder;
		/* 打印非超标数据时间间隔(默认:5分钟) */
		u32 Print_Nrl_Itl;
		/* 打印超标数据时间间隔(默认:2分钟) */
		u32 Print_Out_Itl;
		/* 打印车牌号 */
		char Print_Card_NO[Print_Card_NO_Size];
		/* 打印公司名称 */
		char Print_Company_Name[Print_Company_Name_Size];
		/* 记录的卸货开始数据地址数组（存在Flash的地址） */
		s32 Print_Start_Upload_Adder[Print_Start_upload_Adder_Size];
/* 记录的卸货结束数据地址数组（存在Flash的地址） */
		s32 Print_Stop_Upload_Adder[Print_Stop_upload_Adder_Size];	
		/*卸货开始地址计数点 范围是：0~99*/
		u32 Print_Start_Upload_Counter;		
		/*卸货结束地址计数点 范围是：0~99*/
		u32 Print_Stop_Upload_Counter;
		/*显示卸货开始口型图标标志位   0代表不显示   1代表显示*/
		u32 Print_Start_Upload_Pic_Flag;
	}Inf_State;
	u8 Inf_State_Space[Instru_Print_Fix_Inf_Space_Size];
}INSTRU_PRINT_FIX;


/* Wifi模块相关参数信息*/
typedef union
{
	struct
	{
		/* 要连接Wifi的名字 */
		char Wifi_Name[Wifi_Name_Size];
	  /* 要连接Wifi的密码 */
		char Wifi_Password[Wifi_Passward_Size];	
	  /* 要连接Wifi的加密方式 */
		u32   Wifi_WSKEY; 
	}Inf_State;
  u8 Inf_State_Space[Instru_Wifi_Fix_Inf_Space_Size];
}INSTRU_WIFI_FIX;

/* Wifi模块Wlan参数、IP地址取方式及工作状态获信息*/
typedef union
{
	struct
	{
		/*Wifi模块工作状态*/
		u8   Wifi_Work_State;//0:模块初始化不成功   1:模块初始化成功
		/*Wifi模块网络加密方式*/
		u8   Wifi_Encryption_Way;//0:Disable  1:OPENWEP  2:SHAREDWEP  3:WPA-PSK TKIP  4:WPA-PSK AES  5:WPA2-PSK TKIP  6:WPA2-PSK AES 
		/*Wifi模块IP地址获取方式*/
		u8   Wifi_IP_Get_Way;//0:Static(静态获取)   1:DHCP(动态获取)
		/*Wifi模块保留字节*/
		u8   Wifi_IP_Res;
		    /*Wifi模块Mac地址*/
		char Wifi_Mac_Address[Wifi_Mac_Address_Size];
    /*Wifi模块本地IP地址*/
		char Wifi_Local_IP[Wifi_Local_IP_Size];
		    /*Wifi模块子网掩码*/
		char Wifi_Subnet_Mask[Wifi_Subnet_Mask_Size];
		    /*Wifi模块网关IP地址*/
		char Wifi_Gateway_IP[Wifi_Gateway_IP_Size];
	}Inf;
  u8   Inf_State_Space[Instru_Wifi_Inf_Space_Size];
}INSTRU_WIFI;





/* 仪器 的状态信息表 */
typedef union
{
	struct
	{
		/* 系统 固定信息  */
		INSTRU_SYSTEM_FIX	Instru_System;
		/* 电源 状态信息 */
		INSTRU_POWER_FIX	Instru_Power;
		/* 传感器 状态 信息 */
		INSTRU_SENSOR_FIX	Instru_Sensor;
		/* 外部存储器 FRAM 信息 */
		INSTRU_FRAM_FIX		Instru_Fram;
		/* 外部存储器 网关 信息 */
		INSTRU_Gateway_FIX	Instru_Gateway;
		/* 外部存储器 Flash 信息 */
		INSTRU_FLASH_FIX	Instru_Flash;
		/* 串口1通信 状态信息 */
		INSTRU_COM1_FIX		Instru_Com1;
		/* 打印 信息 */
		INSTRU_PRINT_FIX	Instru_Print;
		/* 仪器配置表 设置 标志 */			  //	00:未配置过（默认配置），01:已经完成配置
		INSTRU_WIFI_FIX    Instru_Wifi; //Wifi连接的wlan网络名称、密码及加密方式
		
		u8                Data_Update_Flag;				    //	00:未配置过（默认配置），程序运行后自行设置。（该选项 可更改）
	}Inf_State;/* 对应模块的状态信息 */
	u8 Inf_State_Space[Instru_Fix_Inf_Tab_Space_Size];
}INSTRU_FIX_INF_TAB;/* 仪器固定信息表 */
















extern   INSTRU_FIX_INF_TAB              Instru_Fix_Inf_State_Tab;	
extern   const INSTRU_FIX_INF_TAB        Default_Instru_Fix_Inf_State_Tab;	
        
extern   INSTRU_CONFIG_INF_TAB           Instru_Config_Inf_Tab;	
extern   const INSTRU_CONFIG_INF_TAB     Default_Instru_Config_Inf_State_Tab;	
        
extern   INSTRU_WIFI_FIX                 Instru_Wifi_NamwPWD;//Wifi名字及密码
                                         
extern   INSTRU_WIFI                     Instru_Wifi_Fix_Tab;//Wifi工作状态、Wlan等参数




//系统
/* 仪器编号 */
extern u8 Serial_Number[Serial_Number_Menu_Buffer_Size];
/* 软件版本 中文 */
extern u8 Software_Version_CN[Version_Inf_Buffer_Size];
/* 硬件版本 中文 */
extern u8 Hardware_Version_CN[Version_Inf_Buffer_Size];
/* 软件版本 英文 */
extern u8 Software_Version_EN[Version_Inf_Buffer_Size];
/* 硬件版本 英文 */
extern u8 Hardware_Version_EN[Version_Inf_Buffer_Size];








/************************************************************************************************/
void Mcu_Pgm_Ude_State_Check		(void);
void Mcu_Cer_Htc_Data				(void);


#endif

