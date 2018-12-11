/************************************************************************************************/
/** @file              : Sve_Fix_Cfg_Tab.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年11月10日															*/
/** @date changed      : 2015年11月10日															*/
/** @brief             : c file																	*/
/** @description       : 固定配置表(业务层)														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "Sve_Fix_Cfg_Tab.h"
#include "Data_Link_Fix_Cfg_Tab.h" 
#include "Mcu_Config.h"
#include "Dev_Fram.h"

#include "stdio.h"

/* 固定配置表 物理地址 */
#define Fix_Cfg_Tab_Mem_Psl_Add				        (&Instru_Fix_Inf_State_Tab)
/* 固定配置表信息 物理地址 */
#define Fix_Cfg_Tab_Psl_Add					          (&Fix_Cfg_Tab_Inf)
/* 固定配置表信息 第一个成员相对地址 */
#define Fix_Cfg_Tab_Mbr_Frt_Add				        (8u)
/* 固定配置表信息 第一个成员物理地址 */
#define Fix_Cfg_Tab_Mbr_Psl_Add				        (((u32)Fix_Cfg_Tab_Psl_Add)+Fix_Cfg_Tab_Mbr_Frt_Add)
/* 固定配置表在外部存储器中存储的最多成员数量 */
#define Fix_Cfg_Tab_Mbr_Sre_Max				        (63u)
/* 固定配置表在外部存储器中存储空间 */
// 成员数量(4字节)+预留(4字节)+63个成员(504字节) = 总共512字节
#define Fix_Cfg_Tab_Sre_Size				          (2*sizeof(u32)+Fix_Cfg_Tab_Mbr_Sre_Max*sizeof(Fix_Cfg_Tab_Mbr_Def))
/* 固定配置表信息中成员的数量 */
#define Fix_Cfg_Tab_Mbr_Num					          (36u)////33+3  Wifi连接的wlan网络名称、密码及加密方式
/* 固定配置表信息 成员数量 的相对地址 */
#define Old_Fix_Cfg_Tab_Mbr_Num_Ver			      (0u)
/* 固定配置表信息 成员数量 寄存器的大小 */
#define Old_Fix_Cfg_Tab_Mbr_Num_Size		      (sizeof(Fix_Cfg_Tab_Inf.Mbr.Mbr_Num))
	
/* 固定配置表信息 状态 的相对地址 */
#define Old_Fix_Cfg_Tab_Stat_Ver			        (4u)
/* 固定配置表信息 成员数量 寄存器的大小 */
#define Old_Fix_Cfg_Tab_Stat_Size			        (sizeof(Fix_Cfg_Tab_Inf.Mbr.Mbr_State))


/* 配置表配置状态:Value:0XA5A5配置表已配置,其他值为未配置。 */
#define Fix_Cfg_Tab_Stat					            (0XA5A5)

/* 固定配置表在外部存储器中存储的信息结构 */
typedef union 
{
	struct
	{
		/* 配置表成员数量 */
		u32						Mbr_Num;
		/* 配置表状态 */
		u32						Mbr_State;
		
		
		/* 第 1 个 成员 */
		Fix_Cfg_Tab_Mbr_Def		Mbr_Sys_Lge;

		/* 第 2 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_ET_Sta_Ude_Time;

		/* 第 3 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_BT_Sta_Ude_Time;

		/* 第 4 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Sen;

		/* 第 5 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Chip_Ccy;

		/* 第 6 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Page_Size;

		/* 第 7 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Page_Num;

		/* 第 8 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Scr_Size;

		/* 第 9 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Scr_Ptr;

		/* 第 10 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Sre_Data_Num;

		/* 第 11 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Wie_Ptr;

		/* 第 12 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Read_Ptr;

		/* 第 13 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Work_Sta;

		/* 第 14 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Sat_Time;

		/* 第 15 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_End_Time;

		/* 第 16 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Rcd_Sat_Time;

		/* 第 17 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Rcd_End_Time;

		/* 第 18 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Rcd_Sat_Add;
		
		/* 第 19 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Rcd_End_Add;

		/* 第 20 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Nrl_Itl;

		/* 第 21 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Out_Itl;
		
		/* 第 22 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Card_NO;

		/* 第 23 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Cpy_Name;
		
		/* 第 24 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_StartUpload;
		/* 第 25 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_StopUpload;
		/* 第 26 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_StartCounter;
		/* 第 27 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_StopCounter;
		/* 第 28 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Start_Upload_Pic_Flag;
		
		
		/* 第 29 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_GatewayAdder;
		
		/* 第 30 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_GatewayChannel;
		
		/* 第 31个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_GatewayInvalidTime;
		
		/* 第 32 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_GatewayMode;
		
		/* 第 33 个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_GatewayBaudrate;
		/* 第 34个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_WifiName;//Wifi名称
	  /* 第 35个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_Password;//Wifi密码及加密方式
	  /* 第 36个 成员 */
		Fix_Cfg_Tab_Mbr_Def 	Mbr_Pit_WSKEY;//Wifi加密方式
	}Mbr;
	/* 数据缓存 */
	u8 Data_Buf[Fix_Cfg_Tab_Sre_Size];
}Fix_Cfg_Tab_Def;

/* 固定配置表成员编号 */
typedef enum {
	/* 系统语言 */
	Sys_Lge_NO = 1, 
	/* 外接电源状态改变过滤时间 */
	ET_Sta_Ude_Time_NO = 2,
	/* 电池状态改变过滤时间 */
	BT_Sta_Ude_Time_NO = 3,
	/* 传感器状态信息 */
	Sen_NO = 4,
	/* 外设存储芯片容量 */
	Chip_Ccy_NO = 5,
	/* 外设存储芯片页大小 */
	Page_Size_NO = 6,
	/* 外设存储芯片页数量 */
	Page_Num_NO = 7,
	/* 外设存储芯片扇区大小 */
	Scr_Size_NO = 8,
	/* 外设存储芯片扇区指针 */
	Scr_Ptr_NO = 9,
	/* 外设存储芯片数据存储数量  */
	Sre_Data_Num_NO = 10,
	/* 外设存储芯片写指针	*/
	Wie_Ptr_NO = 11,
	/* 外设存储芯片读指针	*/
	Read_Ptr_NO = 12,
	/* 外设存储芯片工作状态  */
	Work_Sta_NO = 13,
	/* 打印起始时间  */
	Pit_Sat_Time_NO = 14,
	/* 打印结束时间  */
	Pit_End_Time_NO = 15,
	/* 记录的打印起始时间	*/
	Pit_Rcd_Sat_Time_NO = 16,
	/* 记录的打印结束时间	*/
	Pit_Rcd_End_Time_NO =17,
	/* 记录的打印起始数据地址	*/
	Pit_Rcd_Sat_Add_NO = 18,
	/* 记录的打印结束数据地址	*/
	Pit_Rcd_End_Add_NO = 19,
	/* 打印非超标数据时间间隔	*/
	Pit_Nrl_Itl_NO = 20,
	/* 打印超标数据时间间隔  */
	Pit_Out_Itl_NO = 21,
	/* 打印车牌号 */
	Pit_Card_NO = 22,
	/* 打印公司名称*/
	Pit_Cpy_Name = 23,
	/* 卸货开始 */
	Start_Upload_No = 24,
	/* 卸货结束 */
	Stop_Upload_No = 25,
	/*卸货开始地址计数点 范围是：0~99*/
	Start_Upload_Counter_No = 26,
	/*卸货结束地址计数点 范围是：0~99*/
	Stop_Upload_Counter_No = 27,
	/*显示卸货开始口型图标标志位   0代表不显示   1代表显示*/
	Start_Upload_Pic_Flag_No = 28,
	/* 网关地址 */
	Pit_GatewayAdder = 29,
	/* 信道(波段) */
	Pit_GatewayChannel = 30,
	/* 数据丢失时间 */
	Pit_GatewayActiveTime = 31,
	/* 工作模式（正常/调试） */
	Pit_GatewayMode = 32,
	/* 通信波特率 */
	Pit_GatewayBaudrate = 33,
	/* Wifi名称 */
	Pit_WifiName = 34,
	/* Wifi密码及加密方式 */
	Pit_WifiPassword = 35,
	/* Wifi加密方式 */
	Pit_Wifi_WSKEY = 36,
} Fix_Cfg_Tab_Mbr_NO;

/* 固定配置表成员1:系统语言 */
#if (1)
#define Fix_Cfg_Tab_Mbr_Sys_Lge_NO		        (Sys_Lge_NO)
#define Fix_Cfg_Tab_Mbr_Sys_Lge_Size	        (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_System.Inf_State.System_Language))
#define Fix_Cfg_Tab_Mbr_Sys_Lge_Ver		        (1u)
#define Fix_Cfg_Tab_Mbr_Sys_Lge_Sre		        (0)
#endif                                        

/* 固定配置表成员2:外接电源状态改变过滤时间 */
#if (1)
#define Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_NO		(ET_Sta_Ude_Time_NO)
#define Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_Size	(sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power.Inf.ET_Status_Update_Time))
#define Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_Ver		(1u)
#define Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_Sre		(sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_System))
#endif

/* 固定配置表成员3:电池状态改变过滤时间 */
#if (1)
#define Fix_Cfg_Tab_Mbr_BT_Sta_Ude_Time_NO		(BT_Sta_Ude_Time_NO)
#define Fix_Cfg_Tab_Mbr_BT_Sta_Ude_Time_Size	(sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power.Inf.BT_Status_Update_Time))
#define Fix_Cfg_Tab_Mbr_BT_Sta_Ude_Time_Ver		(1u)
#define Fix_Cfg_Tab_Mbr_BT_Sta_Ude_Time_Sre		(Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_Sre + Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_Size)
#endif

/* 固定配置表成员4:传感器状态信息 */
#if (1)
#define Fix_Cfg_Tab_Mbr_Sen_NO					      (Sen_NO)
#define Fix_Cfg_Tab_Mbr_Sen_Size				      (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor))
#define Fix_Cfg_Tab_Mbr_Sen_Ver					      (2u)
#define Fix_Cfg_Tab_Mbr_Sen_Sre					      (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_System) +\
												                      sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power))
#endif
												 
/* 固定配置表成员29:网关地址  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_NO		(Pit_GatewayAdder)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_Size	(sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.Adder))
#define Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_Ver	(1u)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_Sre	(sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_System) +\
												                       sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power) +\
												                       sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor) +\
												                       sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram))
#endif

/* 固定配置表成员30:网关信道（波段）  */
#if (1)                                             
#define Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_NO	      (Pit_GatewayChannel)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_Size	    (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.Channel))
#define Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_Ver	    (1u)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_Sre	    (Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_Sre + Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_Size)
#endif                                            

/* 固定配置表成员31:数据有效时间  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_NO	  (Pit_GatewayActiveTime)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_Size	(sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.ActiveTime))
#define Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_Ver	  (1u)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_Sre	  (Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_Sre + Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_Size)
#endif

/* 固定配置表成员32:网关工作模式  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayMode_NO		      (Pit_GatewayMode)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayMode_Size	      (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.Mode))
#define Fix_Cfg_Tab_Mbr_Pit_GatewayMode_Ver		      (1u)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayMode_Sre		      (Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_Sre + Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_Size)
#endif                                              

/* 固定配置表成员33:网关通信波特率  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayBaudrate_NO		  (Pit_GatewayBaudrate)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayBaudrate_Size	  (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.Baudrate))
#define Fix_Cfg_Tab_Mbr_Pit_GatewayBaudrate_Ver		  (1u)
#define Fix_Cfg_Tab_Mbr_Pit_GatewayBaudrate_Sre		  (Fix_Cfg_Tab_Mbr_Pit_GatewayMode_Sre + Fix_Cfg_Tab_Mbr_Pit_GatewayMode_Size)
#endif


												 
/* 固定配置表成员5:外设存储芯片容量 */
#if (1)
#define Fix_Cfg_Tab_Mbr_Chip_Ccy_NO				         (Chip_Ccy_NO)
#define Fix_Cfg_Tab_Mbr_Chip_Ccy_Size			         (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Chip_Capacity))
#define Fix_Cfg_Tab_Mbr_Chip_Ccy_Ver			         (1u)
#define Fix_Cfg_Tab_Mbr_Chip_Ccy_Sre			         (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_System) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram)	+\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway))
#endif											 
/* 固定配置表成员6:外设存储芯片页大小 */
#if (1)
#define Fix_Cfg_Tab_Mbr_Page_Size_NO			         (Page_Size_NO)
#define Fix_Cfg_Tab_Mbr_Page_Size_Size			       (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Chip_Page_Size))
#define Fix_Cfg_Tab_Mbr_Page_Size_Ver			         (1u)
#define Fix_Cfg_Tab_Mbr_Page_Size_Sre			         (Fix_Cfg_Tab_Mbr_Chip_Ccy_Sre + Fix_Cfg_Tab_Mbr_Chip_Ccy_Size)
#endif

/* 固定配置表成员7:外设存储芯片页数量 */
#if (1)
#define Fix_Cfg_Tab_Mbr_Page_Num_NO				         (Page_Num_NO)
#define Fix_Cfg_Tab_Mbr_Page_Num_Size			         (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Chip_Page_Num))
#define Fix_Cfg_Tab_Mbr_Page_Num_Ver			         (1u)
#define Fix_Cfg_Tab_Mbr_Page_Num_Sre			         (Fix_Cfg_Tab_Mbr_Page_Size_Sre + Fix_Cfg_Tab_Mbr_Page_Size_Size)
#endif

/* 固定配置表成员8:外设存储芯片扇区大小 */
#if (1)
#define Fix_Cfg_Tab_Mbr_Scr_Size_NO				         (Scr_Size_NO)
#define Fix_Cfg_Tab_Mbr_Scr_Size_Size			         (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Sector_Size))
#define Fix_Cfg_Tab_Mbr_Scr_Size_Ver			         (1u)
#define Fix_Cfg_Tab_Mbr_Scr_Size_Sre			         (Fix_Cfg_Tab_Mbr_Page_Num_Sre + Fix_Cfg_Tab_Mbr_Page_Num_Size)
#endif

/* 固定配置表成员9:外设存储芯片扇区指针 */
#if (1)
#define Fix_Cfg_Tab_Mbr_Scr_Ptr_NO				          (Scr_Ptr_NO)
#define Fix_Cfg_Tab_Mbr_Scr_Ptr_Size			          (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Sector_Pointer))
#define Fix_Cfg_Tab_Mbr_Scr_Ptr_Ver				          (1u)
#define Fix_Cfg_Tab_Mbr_Scr_Ptr_Sre				          (Fix_Cfg_Tab_Mbr_Scr_Size_Sre + Fix_Cfg_Tab_Mbr_Scr_Size_Size)
#endif

/* 固定配置表成员10:外设存储芯片数据存储数量  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Sre_Data_Num_NO			        (Sre_Data_Num_NO)
#define Fix_Cfg_Tab_Mbr_Sre_Data_Num_Size		        (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM))
#define Fix_Cfg_Tab_Mbr_Sre_Data_Num_Ver		        (1u)
#define Fix_Cfg_Tab_Mbr_Sre_Data_Num_Sre		        (Fix_Cfg_Tab_Mbr_Scr_Ptr_Sre + Fix_Cfg_Tab_Mbr_Scr_Ptr_Size)
#endif

/* 固定配置表成员11:外设存储芯片写指针  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Wie_Ptr_NO				         (Wie_Ptr_NO)
#define Fix_Cfg_Tab_Mbr_Wie_Ptr_Size			         (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Write_Pointer))
#define Fix_Cfg_Tab_Mbr_Wie_Ptr_Ver				         (1u)
#define Fix_Cfg_Tab_Mbr_Wie_Ptr_Sre				         (Fix_Cfg_Tab_Mbr_Sre_Data_Num_Sre + Fix_Cfg_Tab_Mbr_Sre_Data_Num_Size)
#endif

/* 固定配置表成员12:外设存储芯片读指针  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Read_Ptr_NO				         (Read_Ptr_NO)
#define Fix_Cfg_Tab_Mbr_Read_Ptr_Size			         (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Read_Pointer))
#define Fix_Cfg_Tab_Mbr_Read_Ptr_Ver			         (1u)
#define Fix_Cfg_Tab_Mbr_Read_Ptr_Sre			         (Fix_Cfg_Tab_Mbr_Wie_Ptr_Sre + Fix_Cfg_Tab_Mbr_Wie_Ptr_Size)
#endif                                             

/* 固定配置表成员13:外设存储芯片工作状态  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Work_Sta_NO				         (Work_Sta_NO)
#define Fix_Cfg_Tab_Mbr_Work_Sta_Size			         (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Chip_Work_State))
#define Fix_Cfg_Tab_Mbr_Work_Sta_Ver			         (1u)
#define Fix_Cfg_Tab_Mbr_Work_Sta_Sre			         (Fix_Cfg_Tab_Mbr_Read_Ptr_Sre + Fix_Cfg_Tab_Mbr_Read_Ptr_Sre)
#endif		

/* 固定配置表成员14:打印起始时间  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Sat_Time_NO			       (Pit_Sat_Time_NO)
#define Fix_Cfg_Tab_Mbr_Pit_Sat_Time_Size		       (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Time))
#define Fix_Cfg_Tab_Mbr_Pit_Sat_Time_Ver		       (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Sat_Time_Sre		       (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_System) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1))
#endif											 
												 
/* 固定配置表成员15:打印结束时间  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_End_Time_NO			        (Pit_End_Time_NO)
#define Fix_Cfg_Tab_Mbr_Pit_End_Time_Size		        (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_End_Time))
#define Fix_Cfg_Tab_Mbr_Pit_End_Time_Ver		        (1u)
#define Fix_Cfg_Tab_Mbr_Pit_End_Time_Sre		        (Fix_Cfg_Tab_Mbr_Pit_Sat_Time_Sre + Fix_Cfg_Tab_Mbr_Pit_Sat_Time_Size)
#endif                                              

/* 固定配置表成员16:记录的打印起始时间  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_NO		      (Pit_Rcd_Sat_Time_NO)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_Size	      (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time))
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_Ver	      (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_Sre	      (Fix_Cfg_Tab_Mbr_Pit_End_Time_Sre + Fix_Cfg_Tab_Mbr_Pit_End_Time_Size)
#endif

/* 固定配置表成员17:记录的打印结束时间  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_NO		      (Pit_Rcd_End_Time_NO)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_Size	      (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_End_Time))
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_Ver	      (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_Sre	      (Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_Sre + Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_Size)
#endif

/* 固定配置表成员18:记录的打印起始数据地址  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_NO		      (Pit_Rcd_Sat_Add_NO)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_Size	      (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder))
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_Ver		      (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_Sre		      (Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_Sre + Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_Size)
#endif

/* 固定配置表成员19:记录的打印结束数据地址  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_NO		      (Pit_Rcd_End_Add_NO)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_Size	      (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_End_Adder))
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_Ver		      (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_Sre		      (Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_Sre + Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_Size)
#endif

/* 固定配置表成员20:打印非超标数据时间间隔  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_NO			        (Pit_Nrl_Itl_NO)
#define Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_Size		        (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl))
#define Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_Ver			        (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_Sre			        (Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_Sre + Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_Size)
#endif                                              

/* 固定配置表成员21:打印超标数据时间间隔  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Out_Itl_NO			        (Pit_Out_Itl_NO)
#define Fix_Cfg_Tab_Mbr_Pit_Out_Itl_Size		        (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl))
#define Fix_Cfg_Tab_Mbr_Pit_Out_Itl_Ver			        (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Out_Itl_Sre			        (Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_Sre + Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_Size)
#endif

/* 固定配置表成员22:打印车牌号  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Card_NO_NO			        (Pit_Card_NO)
#define Fix_Cfg_Tab_Mbr_Pit_Card_NO_Size		        (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO))
#define Fix_Cfg_Tab_Mbr_Pit_Card_NO_Ver			        (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Card_NO_Sre			        (Fix_Cfg_Tab_Mbr_Pit_Out_Itl_Sre + Fix_Cfg_Tab_Mbr_Pit_Out_Itl_Size)
#endif

/* 固定配置表成员23:打印公司名称  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_NO			        (Pit_Cpy_Name)
#define Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_Size		        (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name))
#define Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_Ver		        (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_Sre		        (Fix_Cfg_Tab_Mbr_Pit_Card_NO_Sre + Fix_Cfg_Tab_Mbr_Pit_Card_NO_Size)
#endif
/* 固定配置表成员24:卸货开始  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_NO		      (Start_Upload_No)
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Size		    (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder))
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Ver		    (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Sre		    (Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_Sre + Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_Size)
#endif
/* 固定配置表成员25:卸货结束  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_NO		      (Stop_Upload_No)
#define Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Size		    (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder))
#define Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Ver		      (1u)
#define Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Sre		      (Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Sre + Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Size)
#endif
/* 固定配置表成员26:卸货开始地址计数点  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_NO		(Start_Upload_Counter_No)
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_Size	(sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter))
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_Ver	(1u)
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_Sre	(Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Sre + Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Size)
#endif
/* 固定配置表成员27:卸货开始地址计数点  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_NO		(Stop_Upload_Counter_No)
#define Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_Size	(sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter))
#define Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_Ver		(1u)
#define Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_Sre		(Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_Sre + Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_Size)
#endif
/* 固定配置表成员28:显示卸货开始口型图标标志位  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Pic_Flag_NO		(Start_Upload_Pic_Flag_No)
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Pic_Flag_Size	(sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag))
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Pic_Flag_Ver		(1u)
#define Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Pic_Flag_Sre		(Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_Sre + Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_Size)
#endif




/* 固定配置表成员34:Wifi名字  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_WifiName		           (Pit_WifiName)
#define Fix_Cfg_Tab_Mbr_Pit_WifiName_Size		       (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name))
#define Fix_Cfg_Tab_Mbr_Pit_WifiName_Ver		       (1u)
#define Fix_Cfg_Tab_Mbr_Pit_WifiName_Sre		       (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_System)  +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power)   +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor)  +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram)    +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway) +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash)   +\
												                            sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1)    +\
																										sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print)   )
#endif
/* 固定配置表成员35:Wifi密码  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_WifiPassword		       (Pit_WifiPassword)
#define Fix_Cfg_Tab_Mbr_Pit_WifiPassword_Size		   (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password))
#define Fix_Cfg_Tab_Mbr_Pit_WifiPassword_Ver		   (1u)
#define Fix_Cfg_Tab_Mbr_Pit_WifiPassword_Sre		   (Fix_Cfg_Tab_Mbr_Pit_WifiName_Sre+Fix_Cfg_Tab_Mbr_Pit_WifiName_Size)
#endif	
/* 固定配置表成员36:Wifi加密方式  */
#if (1)
#define Fix_Cfg_Tab_Mbr_Pit_WifiWSKEY		           (Pit_Wifi_WSKEY)
#define Fix_Cfg_Tab_Mbr_Pit_WifiWSKEY_Size		     (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_WSKEY))
#define Fix_Cfg_Tab_Mbr_Pit_WifiWSKEY_Ver		       (1u)
#define Fix_Cfg_Tab_Mbr_Pit_WifiWSKEY_Sre		       (Fix_Cfg_Tab_Mbr_Pit_WifiPassword_Sre+Fix_Cfg_Tab_Mbr_Pit_WifiPassword_Size)
#endif




/* 配置表在外部存储器中存储的信息 */
const Fix_Cfg_Tab_Def  Fix_Cfg_Tab_Inf = 
{
	/* 配置表成员数量 */
	Fix_Cfg_Tab_Mbr_Num,
	/* 配置表状态 */
	Fix_Cfg_Tab_Stat,
	
	/* 第 1 个 成员: 系统语言 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Sys_Lge_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Sys_Lge_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Sys_Lge_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Sys_Lge_Sre,

	/* 第 2 个 成员: 外接电源状态改变过滤时间 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_ET_Sta_Ude_Time_Sre,

	/* 第 3 个 成员: 电池状态改变过滤时间 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_BT_Sta_Ude_Time_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_BT_Sta_Ude_Time_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_BT_Sta_Ude_Time_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_BT_Sta_Ude_Time_Sre,

	/* 第 4 个 成员: 传感器状态信息 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Sen_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Sen_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Sen_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Sen_Sre,

	/* 第 5 个 成员: 外设存储芯片容量 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Chip_Ccy_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Chip_Ccy_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Chip_Ccy_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Chip_Ccy_Sre,

	/* 第 6 个 成员: 外设存储芯片页大小 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Page_Size_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Page_Size_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Page_Size_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Page_Size_Sre,

	/* 第 7 个 成员: 外设存储芯片页数量 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Page_Num_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Page_Num_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Page_Num_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Page_Num_Sre,

	/* 第 8 个 成员: 外设存储芯片扇区大小 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Scr_Size_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Scr_Size_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Scr_Size_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Scr_Size_Sre,

	/* 第 9 个 成员: 外设存储芯片扇区指针 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Scr_Ptr_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Scr_Ptr_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Scr_Ptr_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Scr_Ptr_Sre,

	/* 第 10 个 成员: 外设存储芯片数据存储数量 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Sre_Data_Num_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Sre_Data_Num_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Sre_Data_Num_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Sre_Data_Num_Sre,

	/* 第 11 个 成员: 外设存储芯片写指针 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Wie_Ptr_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Wie_Ptr_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Wie_Ptr_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Wie_Ptr_Sre,

	/* 第 12 个 成员: 外设存储芯片读指针 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Read_Ptr_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Read_Ptr_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Read_Ptr_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Read_Ptr_Sre,

	/* 第 13 个 成员: 外设存储芯片工作状态  */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Work_Sta_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Work_Sta_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Work_Sta_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Work_Sta_Sre,

	/* 第 14 个 成员: 打印起始时间  */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Sat_Time_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Sat_Time_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Sat_Time_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Sat_Time_Sre,

	/* 第 15 个 成员: 打印结束时间 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_End_Time_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_End_Time_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_End_Time_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_End_Time_Sre,

	/* 第 16 个 成员: 记录的打印起始时间 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Time_Sre,

	/* 第 17 个 成员: 记录的打印结束时间 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Time_Sre,

	/* 第 18 个 成员: 记录的打印起始数据地址 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_Sat_Add_Sre,

	/* 第 19 个 成员: 记录的打印结束数据地址 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Rcd_End_Add_Sre,

	/* 第 20 个 成员: 打印非超标数据时间间隔 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Nrl_Itl_Sre,

	/* 第 21 个 成员: 打印超标数据时间间隔 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Out_Itl_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Out_Itl_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Out_Itl_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Out_Itl_Sre,

	/* 第 22 个 成员: 打印车牌号 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Card_NO_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Card_NO_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Card_NO_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Card_NO_Sre,

	/* 第 23 个 成员: 打印公司名称 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Cpy_Name_Sre,
	
	/* 第 24 个 成员: 卸货开始 */
		/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_NO,	
		/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Ver,		
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Size,
		/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Sre,
		/* 第 25 个 成员: 卸货结束 */
		/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_NO,	
			/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Ver,
		/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Size,
			/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Sre,
	
		/* 第 26 个 成员: 卸货开始地址计数点*/
		/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_NO,
				/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_Ver,
			/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_Size,
				/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Counter_Sre,	
	
			/* 第 27 个 成员: 卸货结束地址计数点*/
		/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_NO,
				/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_Ver,
			/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_Size,
				/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Stop_Upload_Counter_Sre,	
	
		/* 第 28 个 成员: 显示卸货开始口型图标标志位 */
		/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Pic_Flag_NO,
				/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Pic_Flag_Ver,
		    /* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Pic_Flag_Size,
			  /* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_Start_Upload_Pic_Flag_Sre,	
	
	/* 第 29 个 成员: 网关地址 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayAdder_Sre,
	
	/* 第 30 个 成员: 网关信道（波段） */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayChannel_Sre,
	
	/* 第 31 个 成员: 数据有效时间 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayActiveTime_Sre,

	/* 第 32 个 成员: 网关工作模式 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayMode_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayMode_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayMode_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayMode_Sre,
	
	/* 第 33 个 成员: 网关通信波特率 */
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayBaudrate_NO,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayBaudrate_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_GatewayBaudrate_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_GatewayBaudrate_Sre,
	/* 第 34 个 成员: Wifi模块名字*/
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_WifiName,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_WifiName_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_WifiName_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_WifiName_Sre,
	/* 第 35 个 成员: Wifi模块密码*/
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_WifiPassword,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_WifiPassword_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_WifiPassword_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_WifiPassword_Sre,	
	/* 第 36 个 成员: Wifi加密方式*/
	/* 成员名称(编号) */
	Fix_Cfg_Tab_Mbr_Pit_WifiWSKEY,
	/* 成员版本 */
	Fix_Cfg_Tab_Mbr_Pit_WifiWSKEY_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Fix_Cfg_Tab_Mbr_Pit_WifiWSKEY_Size,	
	/* 成员存储的相对地址 */
	Fix_Cfg_Tab_Mbr_Pit_WifiWSKEY_Sre,
};


/************************************************************************************************/
/* 函数名	: Sve_Fix_Cfg_Tab_Init																*/
/* 输入		: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用		: 初始化固定配置表																	*/
/* 创建日期	: 2015/11/10																		*/
/************************************************************************************************/
void Sve_Fix_Cfg_Tab_Init(void)
{
	/* 旧固定配置表 成员数量 */
	u32                    old_fix_cfg_tab_mbr_num;
	/* 旧固定配置表 配置表状态 */
	u32                    old_fix_Cfg_Tab_state;
	/* 旧固定配置表 信息 */
	Fix_Cfg_Tab_Mbr_Def    old_fix_cfg_tab_mbr_inf;
	/* 配置成员计数器 */
	u32                    num1,num2;
	
	/* 1、设置固定配置表默认参数 */
	*((INSTRU_FIX_INF_TAB*)Fix_Cfg_Tab_Mem_Psl_Add) = Default_Instru_Fix_Inf_State_Tab;
	
	/* 2、读取 固定配置表成员数量 */
	Data_Link_Fix_Cfg_Tab_Inf_Read((char*)&old_fix_cfg_tab_mbr_num,
						  	  		Old_Fix_Cfg_Tab_Mbr_Num_Ver,
						  	  		Old_Fix_Cfg_Tab_Mbr_Num_Size);
	
	/* 判断 固定配置表成员数量 的合法性*/
	if((old_fix_cfg_tab_mbr_num <= Fix_Cfg_Tab_Mbr_Sre_Max) && (old_fix_cfg_tab_mbr_num > 0))
	{
		/* 固定配置表成员数量在设置范围内 */
		/* 3、读取配置表状态 */
		Data_Link_Fix_Cfg_Tab_Inf_Read((char*)&old_fix_Cfg_Tab_state,
			                                  Old_Fix_Cfg_Tab_Stat_Ver,
		                                    Old_Fix_Cfg_Tab_Stat_Size);
		/* 判断 配置状态 */
		if(old_fix_Cfg_Tab_state == Fix_Cfg_Tab_Stat)
		{
			/* 配置状态为已配置 */
			/* 3、循环读取固定配置表中的信息 */
			for(num1 = 0;num1 <old_fix_cfg_tab_mbr_num; num1++)
			{
				/* 读取 固定配置表成员信息 */
				Data_Link_Fix_Cfg_Tab_Inf_Read((char*)(&old_fix_cfg_tab_mbr_inf),
												                Fix_Cfg_Tab_Mbr_Frt_Add+num1*sizeof(Fix_Cfg_Tab_Mbr_Def),
												                sizeof(Fix_Cfg_Tab_Mbr_Def));
/*********************************调试*****************************************/																															
/**************************************************************************************/																															
				/* 从当前的配置表信息中查询相同 名称(编号) 的成员 */
				for(num2 = 0;num2 <Fix_Cfg_Tab_Inf.Mbr.Mbr_Num; num2++)
				{
					/* 判断当前的配置成员编号与旧的配置编号是否相同. */
					if(old_fix_cfg_tab_mbr_inf.Mbr_NO == ((Fix_Cfg_Tab_Mbr_Def*)(Fix_Cfg_Tab_Mbr_Psl_Add + num2*sizeof(Fix_Cfg_Tab_Mbr_Def)))->Mbr_NO)
					{
						/* 当前编号相同,则判断版本是否一致. */
						if(old_fix_cfg_tab_mbr_inf.Mbr_Ver == ((Fix_Cfg_Tab_Mbr_Def*)(Fix_Cfg_Tab_Mbr_Psl_Add + num2*sizeof(Fix_Cfg_Tab_Mbr_Def)))->Mbr_Ver)
						{
							/* 版本相同 设置 将旧配置表成员参数拷贝给当前配置表成员 */
							/* 读取 旧配置表成员参数 */
							Data_Link_Fix_Cfg_Tab_Para_Read((char*)((u32)Fix_Cfg_Tab_Mem_Psl_Add+((Fix_Cfg_Tab_Mbr_Def*)(Fix_Cfg_Tab_Mbr_Psl_Add + num2*sizeof(Fix_Cfg_Tab_Mbr_Def)))->Mbr_Sre_Add),
															old_fix_cfg_tab_mbr_inf.Mbr_Sre_Add,
															old_fix_cfg_tab_mbr_inf.Mbr_Size);
							/* 跳出查询 */
							break;
						}
					}
				}
			}
		}
	}

	/* 4、将当前配置表在外部存储器中存储的信息存重新存到外部的存储器中 */
	/* 写入 配置表成员信息 */
	Data_Link_Fix_Cfg_Tab_Inf_Write((char*)(&Fix_Cfg_Tab_Inf),0,sizeof(Fix_Cfg_Tab_Def));
	/* 保存最新的配置表 */
	Write_Config_Tab((u8*)Fix_Cfg_Tab_Mem_Psl_Add,sizeof(INSTRU_FIX_INF_TAB));
}













