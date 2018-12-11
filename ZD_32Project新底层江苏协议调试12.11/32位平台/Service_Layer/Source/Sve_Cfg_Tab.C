/************************************************************************************************/
/** @file              : Sve_Cfg_Tab.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年11月05日															*/
/** @date changed      : 2015年11月05日															*/
/** @brief             : c file																	*/
/** @description       : 配置表(业务层)															*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "Sve_Cfg_Tab.h"
#include "DataLink_Cfg_Tab.h"
#include "Mcu_Config.h"
#include "Dev_Fram.h"
// "配置表" 和 "配置表信息" 是不同的寄存器 
/* 配置表 物理地址 */
#define Cfg_Tab_Mem_Psl_Add					(&Instru_Config_Inf_Tab)
/* 当前 配置表信息 物理地址 */
#define Cfg_Tab_Psl_Add						  (&Cfg_Tab_Inf)
/* 配置表信息 第一个成员相对地址 */
#define Cfg_Tab_Mbr_Frt_Add					(8u)
/* 配置表信息 第一个成员物理地址 */
#define Cfg_Tab_Mbr_Psl_Add					(((u32)Cfg_Tab_Psl_Add)+Cfg_Tab_Mbr_Frt_Add)
/* 配置表在外部存储器中存储的最多成员数量 */
#define Cfg_Tab_Mbr_Sre_Max					(63u)
/* 配置表在外部存储器中存储空间 */
// 成员数量(4字节)+预留(4字节)+63个成员(504字节) = 总共512字节
#define Cfg_Tab_Sre_Size					  (2*sizeof(u32)+Cfg_Tab_Mbr_Sre_Max*sizeof(Cfg_Tab_Mbr_Def))
/* 配置表信息中成员的数量 */
#define Cfg_Tab_Mbr_Num						  (39u)

/* 配置表信息 成员数量 的相对地址 */
#define Old_Cfg_Tab_Mbr_Num_Ver			(0u)
/* 配置表信息 成员数量 寄存器的大小 */
#define Old_Cfg_Tab_Mbr_Num_Size		(sizeof(Cfg_Tab_Inf.Mbr.Mbr_Num))

/* 配置表信息 状态 的相对地址 */
#define Old_Cfg_Tab_Stat_Ver				(4u)
/* 配置表信息 成员数量 寄存器的大小 */
#define Old_Cfg_Tab_Stat_Size				(sizeof(Cfg_Tab_Inf.Mbr.Mbr_State))


/* 配置表配置状态:Value:0XA5A5配置表已配置,其他值为未配置。 */
#define Cfg_Tab_Stat						    (0XA5A5)


/* 配置表在外部存储器中存储的信息结构 */
typedef union 
{
	struct
	{
		/* 配置表成员数量 */
		u32						Mbr_Num;
		/* 配置表配置状态 */
		u32 					Mbr_State;
		/* 第 1 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Sil_Nbr;
		
		/* 第 2 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Ste_Vsn;
		
		/* 第 3 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Hde_Vsn;
		
		/* 第 4 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Rme_Uge;
		
		/* 第 5 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Itu_Type;
				
		/* 第 6 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_ET_Sud_Aam_Swh;
				
		/* 第 7 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_ET_Lgt_Aam_Swh;
		
		/* 第 8 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_ET_SMS_Aam_Swh;
		
		/* 第 9 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_BT_Sud_Aam_Swh;
		
		/* 第 10 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_BT_Lgt_Aam_Swh;
		
		/* 第 11 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_BT_SMS_Aam_Swh;

		/* 第 12 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Dmn_Name_ID1;

		/* 第 13 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_IP_Add_ID1;

		/* 第 14 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Port_ID1;

		/* 第 15 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Dmn_Name_ID2;

		/* 第 16 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_IP_Add_ID2;

		/* 第 17个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Port_ID2;

		/* 第 18个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Dmn_Name_ID3;

		/* 第 19个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_IP_Add_ID3;

		/* 第 20 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Port_ID3;

		/* 第 21 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Aam_SMS_NO1;

		/* 第 22 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Aam_SMS_NO2;

		/* 第 23 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Aam_SMS_NO3;

		/* 第 24 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Sen;

		/* 第 25 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Sud_Aam_Mtr_Swh;

		/* 第 26 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Lgt_Aam_Mtr_Swh;

		/* 第 27 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_SMS_Aam_Mtr_Swh;

		/* 第 28 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Aam_Nml_SMS;

		/* 第 29 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Sud_Aam_Dly_Time;

		/* 第 30 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Lgt_Aam_Dly_Time;

		/* 第 31 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_SMS_Aam_Dly_Time;

		/* 第 32 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Data_Clt_Irl;

		/* 第 33 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Aam_Save_Data_Irl;

		/* 第 34 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Nrl_Save_Data_Irl;

		/* 第 35 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Uld_Data_Irl;

		/* 第 36 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_SMS_Rpt_Aam_Irl;

		/* 第 37 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Save_Data_Swh;

		/* 第 38 个 成员 */
		Cfg_Tab_Mbr_Def		Mbr_Cbe_Data_Tab;
	
		/* 第 39 个 成员 */		
		Cfg_Tab_Mbr_Def		Mbr_RTC_Data;
		
	}Mbr;
	/* 数据缓存 */
	u8 Data_Buf[Cfg_Tab_Sre_Size];
}Cfg_Tab_Def;

/* 配置表成员编号 */
typedef enum {
	
	/* 序列号 */
	Sil_Nbr_NO = 1,
	/* 软件版本 */
	Ste_Vsn_NO = 2,
	/* 硬件版本 */
	Hde_Vsn_NO = 3,
	/* 远程升级 */
	Rme_Uge_NO = 4,
	/* 仪器类型 */
	Itu_Type_NO = 5,
	/* 外接电源声报警开关 */
	ET_Sud_Aam_Swh_NO = 6,
	/* 外接电源光报警开关 */
	ET_Lgt_Aam_Swh_NO = 7,
	/* 外接电源短信报警开关 */
	ET_SMS_Aam_Swh_NO = 8,
	/* 电池电量声报警开关 */
	BT_Sud_Aam_Swh_NO = 9,									 
	/* 电池电量光报警开关 */
	BT_Lgt_Aam_Swh_NO = 10,
	/* 电池电量短信报警开关 */
	BT_SMS_Aam_Swh_NO = 11,
	/* GSM模块 服务器1 域名 */
	Dmn_Name_ID1_NO = 12,
	/* GSM模块 服务器1 IP地址 */
	IP_Add_ID1_NO = 13,
	/* GSM模块 服务器1 端口 */
	Port_ID1_NO = 14,
	/* GSM模块 服务器2 域名 */
	Dmn_Name_ID2_NO = 15,
	/* GSM模块 服务器2 IP地址 */
	IP_Add_ID2_NO = 16,
	/* GSM模块 服务器2 端口 */
	Port_ID2_NO = 17,
	/* GSM模块 服务器3 域名 */
	Dmn_Name_ID3_NO = 18,
	/* GSM模块 服务器3 IP地址 */
	IP_Add_ID3_NO = 19,
	/* GSM模块 服务器3 端口 */
	Port_ID3_NO = 20,
	/* GSM模块 报警短信 号码1 */
	Aam_SMS_NO1_NO = 21,
	/* GSM模块 报警短信 号码2 */
	Aam_SMS_NO2_NO = 22,
	/* GSM模块 报警短信 号码3 */
	Aam_SMS_NO3_NO = 23,
	/* 传感器状态信息 */
	Mbr_Sen_NO = 24,
	/* 声音报警总开关 */
	Sud_Aam_Mtr_Swh_NO = 25,
	/* 光 报警总开关 */
	Lgt_Aam_Mtr_Swh_NO = 26,
	/* 短信报警总开关 */
	SMS_Aam_Mtr_Swh_NO = 27,
	/* 报警正常短信提示 */
	Aam_Nml_SMS_NO = 28,
	/* 声音报警的延时时间 */
	Sud_Aam_Dly_Time_NO = 29,
	/* 光报警的延时时间 */
	Lgt_Aam_Dly_Time_NO = 30,
	/* 短信报警的延时时间 */
	SMS_Aam_Dly_Time_NO = 31,
	/* 数据采集间隔 */
	Data_Clt_Irl_NO = 32,
	/* 报警数据记录间隔 */
	Aam_Save_Data_Irl_NO = 33,
	/* 正常数据记录间隔 */
	Nrl_Save_Data_Irl_NO = 34,
	/* 上传数据间隔 */
	Uld_Data_Irl_NO	= 35,
	/* 短信重复报警时间间隔 */
	SMS_Rpt_Aam_Irl_NO = 36,
	/* 实时数据存储开关 */
	Save_Data_Swh_NO = 37,
	/* 传感器数据校准表 */
	Cbe_Data_Tab_NO = 38,
	/* RTC数据 */
	RTC_Data_NO = 39,
	
	

} Fix_Cfg_Tab_Mbr_NO;


// 配置表成员 的地址应该用 相对偏移地址来表示，现在用的方式不适合后期的更改。

/* 配置表成员1:序列号 */
#if (1)
#define Cfg_Tab_Mbr_Sil_Nbr_NO			(Sil_Nbr_NO)
#define Cfg_Tab_Mbr_Sil_Nbr_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number))
#define Cfg_Tab_Mbr_Sil_Nbr_Ver			(1u)
#define Cfg_Tab_Mbr_Sil_Nbr_Sre			(0)
#endif

/* 配置表成员2:软件版本 */
#if (1)
#define Cfg_Tab_Mbr_Ste_Vsn_NO			(Ste_Vsn_NO)
#define Cfg_Tab_Mbr_Ste_Vsn_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Software_Version))
#define Cfg_Tab_Mbr_Ste_Vsn_Ver			(1u)
#define Cfg_Tab_Mbr_Ste_Vsn_Sre			(Cfg_Tab_Mbr_Sil_Nbr_Sre + Cfg_Tab_Mbr_Sil_Nbr_Size)
#endif

/* 配置表成员3:硬件版本 */
#if (1)
#define Cfg_Tab_Mbr_Hde_Vsn_NO			(Hde_Vsn_NO)
#define Cfg_Tab_Mbr_Hde_Vsn_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Hardware_Version))
#define Cfg_Tab_Mbr_Hde_Vsn_Ver			(1u)
#define Cfg_Tab_Mbr_Hde_Vsn_Sre			(Cfg_Tab_Mbr_Ste_Vsn_Sre + Cfg_Tab_Mbr_Ste_Vsn_Size)
#endif

/* 配置表成员4:远程升级 */
#if (1)
#define Cfg_Tab_Mbr_Rme_Uge_NO			(Rme_Uge_NO)
#define Cfg_Tab_Mbr_Rme_Uge_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.UpdateFlag))
#define Cfg_Tab_Mbr_Rme_Uge_Ver			(1u)
#define Cfg_Tab_Mbr_Rme_Uge_Sre			(Cfg_Tab_Mbr_Hde_Vsn_Sre + Cfg_Tab_Mbr_Hde_Vsn_Size)
#endif

/* 配置表成员5:仪器类型 */
#if (1)
#define Cfg_Tab_Mbr_Itu_Type_NO			(Itu_Type_NO)
#define Cfg_Tab_Mbr_Itu_Type_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Instru_Type))
#define Cfg_Tab_Mbr_Itu_Type_Ver		(1u)
#define Cfg_Tab_Mbr_Itu_Type_Sre		(Cfg_Tab_Mbr_Rme_Uge_Sre + Cfg_Tab_Mbr_Rme_Uge_Size)
#endif

/* 配置表成员6:外接电源声报警开关 */
#if (1)
#define Cfg_Tab_Mbr_ET_Sud_Aam_Swh_NO	(ET_Sud_Aam_Swh_NO)
#define Cfg_Tab_Mbr_ET_Sud_Aam_Swh_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch))
#define Cfg_Tab_Mbr_ET_Sud_Aam_Swh_Ver	(1u)
#define Cfg_Tab_Mbr_ET_Sud_Aam_Swh_Sre	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System) + sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Link_State))
#endif

/* 配置表成员7:外接电源光报警开关 */
#if (1)
#define Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_NO	(ET_Lgt_Aam_Swh_NO)
#define Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch))
#define Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_Ver	(1u)
#define Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_Sre	(Cfg_Tab_Mbr_ET_Sud_Aam_Swh_Sre + Cfg_Tab_Mbr_ET_Sud_Aam_Swh_Size)
#endif

/* 配置表成员8:外接电源短信报警开关 */
#if (1)
#define Cfg_Tab_Mbr_ET_SMS_Aam_Swh_NO	(ET_SMS_Aam_Swh_NO)
#define Cfg_Tab_Mbr_ET_SMS_Aam_Swh_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch))
#define Cfg_Tab_Mbr_ET_SMS_Aam_Swh_Ver	(1u)
#define Cfg_Tab_Mbr_ET_SMS_Aam_Swh_Sre	(Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_Sre + Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_Size)
#endif

/* 配置表成员9:电池电量声报警开关 */
#if (1)
#define Cfg_Tab_Mbr_BT_Sud_Aam_Swh_NO	(BT_Sud_Aam_Swh_NO)
#define Cfg_Tab_Mbr_BT_Sud_Aam_Swh_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch))
#define Cfg_Tab_Mbr_BT_Sud_Aam_Swh_Ver	(1u)
#define Cfg_Tab_Mbr_BT_Sud_Aam_Swh_Sre	(Cfg_Tab_Mbr_ET_SMS_Aam_Swh_Sre+\
										 Cfg_Tab_Mbr_ET_SMS_Aam_Swh_Size+\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Alarm_State)+\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Charge_State)+\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State))									 
#endif
										 
/* 配置表成员10:电池电量光报警开关 */
#if (1)
#define Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_NO	(BT_Lgt_Aam_Swh_NO)
#define Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch))
#define Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_Ver	(1u)
#define Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_Sre	(Cfg_Tab_Mbr_BT_Sud_Aam_Swh_Sre + Cfg_Tab_Mbr_BT_Sud_Aam_Swh_Size)									 
#endif

/* 配置表成员11:电池电量短信报警开关 */
#if (1)
#define Cfg_Tab_Mbr_BT_SMS_Aam_Swh_NO	(BT_SMS_Aam_Swh_NO)
#define Cfg_Tab_Mbr_BT_SMS_Aam_Swh_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch))
#define Cfg_Tab_Mbr_BT_SMS_Aam_Swh_Ver	(1u)
#define Cfg_Tab_Mbr_BT_SMS_Aam_Swh_Sre	(Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_Sre + Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_Size)									 
#endif

/* 配置表成员12:GSM模块 服务器1 域名 */
#if (1)
#define Cfg_Tab_Mbr_Dmn_Name_ID1_NO		(Dmn_Name_ID1_NO)
#define Cfg_Tab_Mbr_Dmn_Name_ID1_Size	(32u)
#define Cfg_Tab_Mbr_Dmn_Name_ID1_Ver	(1u)
#define Cfg_Tab_Mbr_Dmn_Name_ID1_Sre	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System) +\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power) +\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC ))
#endif

/* 配置表成员13:GSM模块 服务器1 IP地址 */
#if (1)
#define Cfg_Tab_Mbr_IP_Add_ID1_NO		(IP_Add_ID1_NO)
#define Cfg_Tab_Mbr_IP_Add_ID1_Size		(sizeof(SERVER_IP_ADDRESS))
#define Cfg_Tab_Mbr_IP_Add_ID1_Ver		(1u)
#define Cfg_Tab_Mbr_IP_Add_ID1_Sre		(Cfg_Tab_Mbr_Dmn_Name_ID1_Sre + Cfg_Tab_Mbr_Dmn_Name_ID1_Size)									 
#endif

/* 配置表成员14:GSM模块 服务器1 端口 */
#if (1)
#define Cfg_Tab_Mbr_Port_ID1_NO			(Port_ID1_NO)
#define Cfg_Tab_Mbr_Port_ID1_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1))
#define Cfg_Tab_Mbr_Port_ID1_Ver		(1u)
#define Cfg_Tab_Mbr_Port_ID1_Sre		(Cfg_Tab_Mbr_IP_Add_ID1_Sre + Cfg_Tab_Mbr_IP_Add_ID1_Size)									 
#endif

/* 配置表成员15:GSM模块 服务器2 域名 */
#if (1)
#define Cfg_Tab_Mbr_Dmn_Name_ID2_NO		(Dmn_Name_ID2_NO)
#define Cfg_Tab_Mbr_Dmn_Name_ID2_Size	(32u)
#define Cfg_Tab_Mbr_Dmn_Name_ID2_Ver	(1u)
#define Cfg_Tab_Mbr_Dmn_Name_ID2_Sre	(Cfg_Tab_Mbr_Port_ID1_Sre + Cfg_Tab_Mbr_Port_ID1_Size +\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.GSM_Inf_ID1_Reserve))									 
#endif

/* 配置表成员16:GSM模块 服务器2 IP地址 */
#if (1)
#define Cfg_Tab_Mbr_IP_Add_ID2_NO		(IP_Add_ID2_NO)
#define Cfg_Tab_Mbr_IP_Add_ID2_Size		(sizeof(SERVER_IP_ADDRESS))
#define Cfg_Tab_Mbr_IP_Add_ID2_Ver		(1u)
#define Cfg_Tab_Mbr_IP_Add_ID2_Sre		(Cfg_Tab_Mbr_Dmn_Name_ID2_Sre + Cfg_Tab_Mbr_Dmn_Name_ID2_Size)									 
#endif

/* 配置表成员17:GSM模块 服务器2 端口 */
#if (1)
#define Cfg_Tab_Mbr_Port_ID2_NO			(Port_ID2_NO)
#define Cfg_Tab_Mbr_Port_ID2_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2))
#define Cfg_Tab_Mbr_Port_ID2_Ver		(1u)
#define Cfg_Tab_Mbr_Port_ID2_Sre		(Cfg_Tab_Mbr_IP_Add_ID2_Sre + Cfg_Tab_Mbr_IP_Add_ID2_Size)									 
#endif

/* 配置表成员18:GSM模块 服务器3 域名 */
#if (1)
#define Cfg_Tab_Mbr_Dmn_Name_ID3_NO		(Dmn_Name_ID3_NO)
#define Cfg_Tab_Mbr_Dmn_Name_ID3_Size	(32u)
#define Cfg_Tab_Mbr_Dmn_Name_ID3_Ver	(1u)
#define Cfg_Tab_Mbr_Dmn_Name_ID3_Sre	(Cfg_Tab_Mbr_Port_ID2_Sre + Cfg_Tab_Mbr_Port_ID2_Size +\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.GSM_Inf_ID2_Reserve))									 
#endif

/* 配置表成员19:GSM模块 服务器3 IP地址 */
#if (1)
#define Cfg_Tab_Mbr_IP_Add_ID3_NO		(IP_Add_ID3_NO)
#define Cfg_Tab_Mbr_IP_Add_ID3_Size		(sizeof(SERVER_IP_ADDRESS))
#define Cfg_Tab_Mbr_IP_Add_ID3_Ver		(1u)
#define Cfg_Tab_Mbr_IP_Add_ID3_Sre		(Cfg_Tab_Mbr_Dmn_Name_ID3_Sre + Cfg_Tab_Mbr_Dmn_Name_ID3_Size)									 
#endif

/* 配置表成员20:GSM模块 服务器3 端口 */
#if (1)
#define Cfg_Tab_Mbr_Port_ID3_NO			(Port_ID3_NO)
#define Cfg_Tab_Mbr_Port_ID3_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3))
#define Cfg_Tab_Mbr_Port_ID3_Ver		(1u)
#define Cfg_Tab_Mbr_Port_ID3_Sre		(Cfg_Tab_Mbr_IP_Add_ID3_Sre + Cfg_Tab_Mbr_IP_Add_ID3_Size)
#endif

/* 配置表成员21:GSM模块 报警短信 号码1 */
#if (1)
#define Cfg_Tab_Mbr_Aam_SMS_NO1_NO		(Aam_SMS_NO1_NO)
#define Cfg_Tab_Mbr_Aam_SMS_NO1_Size	(sizeof(MOBILE_NO))
#define Cfg_Tab_Mbr_Aam_SMS_NO1_Ver		(1u)
#define Cfg_Tab_Mbr_Aam_SMS_NO1_Sre		(Cfg_Tab_Mbr_Port_ID3_Sre + Cfg_Tab_Mbr_Port_ID3_Size +\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.GSM_Inf_ID3_Reserve))									 
#endif

/* 配置表成员22:GSM模块 报警短信 号码2 */
#if (1)
#define Cfg_Tab_Mbr_Aam_SMS_NO2_NO		(Aam_SMS_NO2_NO)
#define Cfg_Tab_Mbr_Aam_SMS_NO2_Size	(sizeof(MOBILE_NO))
#define Cfg_Tab_Mbr_Aam_SMS_NO2_Ver		(1u)
#define Cfg_Tab_Mbr_Aam_SMS_NO2_Sre		(Cfg_Tab_Mbr_Aam_SMS_NO1_Sre + Cfg_Tab_Mbr_Aam_SMS_NO1_Size)									 
#endif

/* 配置表成员23:GSM模块 报警短信 号码3 */
#if (1)
#define Cfg_Tab_Mbr_Aam_SMS_NO3_NO		(Aam_SMS_NO3_NO)
#define Cfg_Tab_Mbr_Aam_SMS_NO3_Size	(sizeof(MOBILE_NO))
#define Cfg_Tab_Mbr_Aam_SMS_NO3_Ver		(1u)
#define Cfg_Tab_Mbr_Aam_SMS_NO3_Sre		(Cfg_Tab_Mbr_Aam_SMS_NO2_Sre + Cfg_Tab_Mbr_Aam_SMS_NO2_Size)									 
#endif

/* 配置表成员24:传感器状态信息 */
#if (1)
#define Cfg_Tab_Mbr_Sen_NO				(Mbr_Sen_NO)
#define Cfg_Tab_Mbr_Sen_Size			(sizeof(INSTRU_SENSOR))
#define Cfg_Tab_Mbr_Sen_Ver				(1u)
#define Cfg_Tab_Mbr_Sen_Sre				(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System) +\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power) +\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC ) +\
										 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM ))
#endif

/* 配置表成员25: 声音报警总开关 */
#if (1)
#define Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_NO		(Sud_Aam_Mtr_Swh_NO)
#define Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch))
#define Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_Ver		(1u)
#define Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_Sre		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC ) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM ) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Sensor ))
#endif

/* 配置表成员26: 光 报警总开关 */
#if (1)
#define Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_NO		(Lgt_Aam_Mtr_Swh_NO)
#define Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch))
#define Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_Ver		(1u)
#define Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_Sre		(Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_Sre + Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_Size)
#endif

/* 配置表成员27: 短信报警总开关 */
#if (1)
#define Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_NO		(SMS_Aam_Mtr_Swh_NO)
#define Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch))
#define Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_Ver		(1u)
#define Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_Sre		(Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_Sre + Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_Size)
#endif

/* 配置表成员28: 报警正常短信提示 */
#if (1)
#define Cfg_Tab_Mbr_Aam_Nml_SMS_NO			(Aam_Nml_SMS_NO)
#define Cfg_Tab_Mbr_Aam_Nml_SMS_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Alarm_Normal_SMS))
#define Cfg_Tab_Mbr_Aam_Nml_SMS_Ver			(1u)
#define Cfg_Tab_Mbr_Aam_Nml_SMS_Sre			(Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_Sre + Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_Size)
#endif

/* 配置表成员29: 声音报警的延时时间 */
#if (1)
#define Cfg_Tab_Mbr_Sud_Aam_Dly_Time_NO		(Sud_Aam_Dly_Time_NO)
#define Cfg_Tab_Mbr_Sud_Aam_Dly_Time_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Delay_Time))
#define Cfg_Tab_Mbr_Sud_Aam_Dly_Time_Ver	(1u)
#define Cfg_Tab_Mbr_Sud_Aam_Dly_Time_Sre	(Cfg_Tab_Mbr_Aam_Nml_SMS_Sre + Cfg_Tab_Mbr_Aam_Nml_SMS_Size)
#endif

/* 配置表成员30: 光报警的延时时间 */
#if (1)
#define Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_NO		(Lgt_Aam_Dly_Time_NO)
#define Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Light_Alarm_Delay_Time))
#define Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_Ver	(1u)
#define Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_Sre	(Cfg_Tab_Mbr_Sud_Aam_Dly_Time_Sre + Cfg_Tab_Mbr_Sud_Aam_Dly_Time_Size)
#endif

/* 配置表成员31: 短信报警的延时时间 */
#if (1)
#define Cfg_Tab_Mbr_SMS_Aam_Dly_Time_NO		(SMS_Aam_Dly_Time_NO)
#define Cfg_Tab_Mbr_SMS_Aam_Dly_Time_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Delay_Time))
#define Cfg_Tab_Mbr_SMS_Aam_Dly_Time_Ver	(1u)
#define Cfg_Tab_Mbr_SMS_Aam_Dly_Time_Sre	(Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_Sre + Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_Size)
#endif

/* 配置表成员32: 数据采集间隔 */
#if (1)
#define Cfg_Tab_Mbr_Data_Clt_Irl_NO			(Data_Clt_Irl_NO)
#define Cfg_Tab_Mbr_Data_Clt_Irl_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Collect_Data_Interval))
#define Cfg_Tab_Mbr_Data_Clt_Irl_Ver		(1u)
#define Cfg_Tab_Mbr_Data_Clt_Irl_Sre		(Cfg_Tab_Mbr_SMS_Aam_Dly_Time_Sre + Cfg_Tab_Mbr_SMS_Aam_Dly_Time_Size)
#endif

/* 配置表成员33: 报警数据记录间隔 */
#if (1)
#define Cfg_Tab_Mbr_Aam_Save_Data_Irl_NO	(Aam_Save_Data_Irl_NO)
#define Cfg_Tab_Mbr_Aam_Save_Data_Irl_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval))
#define Cfg_Tab_Mbr_Aam_Save_Data_Irl_Ver	(1u)
#define Cfg_Tab_Mbr_Aam_Save_Data_Irl_Sre	(Cfg_Tab_Mbr_Data_Clt_Irl_Sre + Cfg_Tab_Mbr_Data_Clt_Irl_Size)
#endif

/* 配置表成员34: 正常数据记录间隔 */
#if (1)
#define Cfg_Tab_Mbr_Nrl_Save_Data_Irl_NO	(Nrl_Save_Data_Irl_NO)
#define Cfg_Tab_Mbr_Nrl_Save_Data_Irl_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval))
#define Cfg_Tab_Mbr_Nrl_Save_Data_Irl_Ver	(1u)
#define Cfg_Tab_Mbr_Nrl_Save_Data_Irl_Sre	(Cfg_Tab_Mbr_Aam_Save_Data_Irl_Sre + Cfg_Tab_Mbr_Aam_Save_Data_Irl_Size)
#endif

/* 配置表成员35: 上传数据间隔 */
#if (1)
#define Cfg_Tab_Mbr_Uld_Data_Irl_NO			(Uld_Data_Irl_NO)
#define Cfg_Tab_Mbr_Uld_Data_Irl_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Upload_Data_Interval))
#define Cfg_Tab_Mbr_Uld_Data_Irl_Ver		(1u)
#define Cfg_Tab_Mbr_Uld_Data_Irl_Sre		(Cfg_Tab_Mbr_Nrl_Save_Data_Irl_Sre + Cfg_Tab_Mbr_Nrl_Save_Data_Irl_Size)
#endif

/* 配置表成员36: 短信重复报警时间间隔 */
#if (1)
#define Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_NO		(SMS_Rpt_Aam_Irl_NO)
#define Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_Size	(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval))
#define Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_Ver		(1u)
#define Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_Sre		(Cfg_Tab_Mbr_Uld_Data_Irl_Sre + Cfg_Tab_Mbr_Uld_Data_Irl_Size)
#endif

/* 配置表成员37: 实时数据存储开关 */
#if (1)
#define Cfg_Tab_Mbr_Save_Data_Swh_NO		(Save_Data_Swh_NO)
#define Cfg_Tab_Mbr_Save_Data_Swh_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch))
#define Cfg_Tab_Mbr_Save_Data_Swh_Ver		(1u)
#define Cfg_Tab_Mbr_Save_Data_Swh_Sre		(Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_Sre + Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_Size)
#endif

/* 配置表成员38: 传感器数据校准表 */
#if (1)
#define Cfg_Tab_Mbr_Cbe_Data_Tab_NO			(Cbe_Data_Tab_NO)
#define Cfg_Tab_Mbr_Cbe_Data_Tab_Size		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab))
#define Cfg_Tab_Mbr_Cbe_Data_Tab_Ver		(1u)
#define Cfg_Tab_Mbr_Cbe_Data_Tab_Sre		(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC ) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM ) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Sensor ) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu ))
#endif
											 
/* 配置表成员39: RTC实时时钟数据 */
#if (1)
#define Cfg_Tab_Mbr_RTC_Data_NO				(RTC_Data_NO)
#define Cfg_Tab_Mbr_RTC_Data_Size			(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC))
#define Cfg_Tab_Mbr_RTC_Data_Ver			(1u)
#define Cfg_Tab_Mbr_RTC_Data_Sre			(sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_System) +\
										 	 sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Power))
#endif
											 



/* 配置表在外部存储器中存储的信息 */
const Cfg_Tab_Def  Cfg_Tab_Inf = 
{

	/* 配置表成员数量 */
	Cfg_Tab_Mbr_Num,
	/* 配置表状态 */
	Cfg_Tab_Stat,
	
	/* 第 1 个 成员: 序列号 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Sil_Nbr_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Sil_Nbr_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Sil_Nbr_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Sil_Nbr_Sre,
	
	/* 第 2 个 成员: 软件版本 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Ste_Vsn_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Ste_Vsn_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Ste_Vsn_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Ste_Vsn_Sre,
	
	/* 第 3 个 成员: 硬件版本 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Hde_Vsn_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Hde_Vsn_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Hde_Vsn_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Hde_Vsn_Sre,
	
	/* 第 4 个 成员: 远程升级 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Rme_Uge_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Rme_Uge_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Rme_Uge_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Rme_Uge_Sre,
	
	/* 第 5 个 成员: 仪器类型 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Itu_Type_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Itu_Type_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Itu_Type_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Itu_Type_Sre,
	
	/* 第 6 个 成员: 外接电源声报警开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_ET_Sud_Aam_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_ET_Sud_Aam_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_ET_Sud_Aam_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_ET_Sud_Aam_Swh_Sre,
	
	/* 第 7 个 成员: 外接电源光报警开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_ET_Lgt_Aam_Swh_Sre,
	
	/* 第 8 个 成员: 外接电源光报警开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_ET_SMS_Aam_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_ET_SMS_Aam_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_ET_SMS_Aam_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_ET_SMS_Aam_Swh_Sre,
	
	/* 第 9 个 成员: 电池电量声报警开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_BT_Sud_Aam_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_BT_Sud_Aam_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_BT_Sud_Aam_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_BT_Sud_Aam_Swh_Sre,
	
	/* 第 10 个 成员: 电池电量光报警开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_BT_Lgt_Aam_Swh_Sre,
	
	/* 第 11 个 成员: 电池电量短信报警开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_BT_SMS_Aam_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_BT_SMS_Aam_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_BT_SMS_Aam_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_BT_SMS_Aam_Swh_Sre,

	/* 第 12 个 成员: GSM模块 服务器1 域名 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Dmn_Name_ID1_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Dmn_Name_ID1_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Dmn_Name_ID1_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Dmn_Name_ID1_Sre,

	/* 第 13 个 成员: GSM模块 服务器1 IP地址 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_IP_Add_ID1_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_IP_Add_ID1_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_IP_Add_ID1_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_IP_Add_ID1_Sre,

	/* 第 14 个 成员: GSM模块 服务器1 端口 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Port_ID1_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Port_ID1_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Port_ID1_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Port_ID1_Sre,

	/* 第 15 个 成员: GSM模块 服务器2 域名 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Dmn_Name_ID2_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Dmn_Name_ID2_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Dmn_Name_ID2_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Dmn_Name_ID2_Sre,

	/* 第 16 个 成员: GSM模块 服务器2 IP地址 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_IP_Add_ID2_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_IP_Add_ID2_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_IP_Add_ID2_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_IP_Add_ID2_Sre,

	/* 第 17 个 成员: GSM模块 服务器2 端口 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Port_ID2_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Port_ID2_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Port_ID2_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Port_ID2_Sre,

	/* 第 18 个 成员: GSM模块 服务器3 域名 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Dmn_Name_ID3_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Dmn_Name_ID3_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Dmn_Name_ID3_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Dmn_Name_ID3_Sre,

	/* 第 19 个 成员: GSM模块 服务器3 IP地址 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_IP_Add_ID3_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_IP_Add_ID3_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_IP_Add_ID3_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_IP_Add_ID3_Sre,

	/* 第 20 个 成员: GSM模块 服务器3 端口 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Port_ID3_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Port_ID3_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Port_ID3_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Port_ID3_Sre,

	/* 第 21 个 成员: GSM模块 报警短信 号码1 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Aam_SMS_NO1_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Aam_SMS_NO1_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Aam_SMS_NO1_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Aam_SMS_NO1_Sre,

	/* 第 22 个 成员: GSM模块 报警短信 号码2 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Aam_SMS_NO2_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Aam_SMS_NO2_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Aam_SMS_NO2_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Aam_SMS_NO2_Sre,

	/* 第 23 个 成员: GSM模块 报警短信 号码3 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Aam_SMS_NO3_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Aam_SMS_NO3_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Aam_SMS_NO3_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Aam_SMS_NO3_Sre,

	/* 第 24 个 成员: 传感器状态信息 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Sen_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Sen_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Sen_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Sen_Sre,

	/* 第 25 个 成员: 声音报警总开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Sud_Aam_Mtr_Swh_Sre,

	/* 第 26 个 成员: 光 报警总开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Lgt_Aam_Mtr_Swh_Sre,

	/* 第 27 个 成员: 短信报警总开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_SMS_Aam_Mtr_Swh_Sre,

	/* 第 28 个 成员: 报警正常短信提示 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Aam_Nml_SMS_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Aam_Nml_SMS_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Aam_Nml_SMS_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Aam_Nml_SMS_Sre,

	/* 第 29 个 成员: 声音报警的延时时间 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Sud_Aam_Dly_Time_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Sud_Aam_Dly_Time_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Sud_Aam_Dly_Time_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Sud_Aam_Dly_Time_Sre,

	/* 第 30 个 成员: 光报警的延时时间 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Lgt_Aam_Dly_Time_Sre,

	/* 第 31 个 成员: 短信报警的延时时间 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_SMS_Aam_Dly_Time_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_SMS_Aam_Dly_Time_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_SMS_Aam_Dly_Time_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_SMS_Aam_Dly_Time_Sre,

	/* 第 32 个 成员: 数据采集间隔 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Data_Clt_Irl_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Data_Clt_Irl_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Data_Clt_Irl_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Data_Clt_Irl_Sre,

	/* 第 33 个 成员: 报警数据记录间隔 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Aam_Save_Data_Irl_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Aam_Save_Data_Irl_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Aam_Save_Data_Irl_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Aam_Save_Data_Irl_Sre,

	/* 第 34 个 成员: 正常数据记录间隔 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Nrl_Save_Data_Irl_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Nrl_Save_Data_Irl_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Nrl_Save_Data_Irl_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Nrl_Save_Data_Irl_Sre,

	/* 第 35 个 成员: 上传数据间隔 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Uld_Data_Irl_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Uld_Data_Irl_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Uld_Data_Irl_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Uld_Data_Irl_Sre,

	/* 第 36 个 成员: 短信重复报警时间间隔 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_SMS_Rpt_Aam_Irl_Sre,

	/* 第 37 个 成员: 实时数据存储开关 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Save_Data_Swh_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Save_Data_Swh_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Save_Data_Swh_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Save_Data_Swh_Sre,

	/* 第 38 个 成员: 传感器数据校准表 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_Cbe_Data_Tab_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_Cbe_Data_Tab_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_Cbe_Data_Tab_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_Cbe_Data_Tab_Sre,
	
	/* 第 39 个 成员: RTC实时时钟数据 */
	/* 成员名称(编号) */
	Cfg_Tab_Mbr_RTC_Data_NO,
	/* 成员版本 */
	Cfg_Tab_Mbr_RTC_Data_Ver,
	/* 成员数据存储空间大小(单位:字节) */
	Cfg_Tab_Mbr_RTC_Data_Size,	
	/* 成员存储的相对地址 */
	Cfg_Tab_Mbr_RTC_Data_Sre,


};


/************************************************************************************************/
/* 函数名	: Sve_Cfg_Tab_Init																	*/
/* 输入		: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用		: 初始化配置表																		*/
/* 创建日期	: 2015/11/05																		*/
/************************************************************************************************/
void Sve_Cfg_Tab_Init(void)
{
	/* 旧配置表 配置表成员数量 */
	u32 old_Cfg_Tab_mbr_num;
	/* 旧配置表 配置表状态 */
	u32 old_Cfg_Tab_state;
	/* 旧配置表 配置表信息 */
	Cfg_Tab_Mbr_Def old_Cfg_Tab_mbr_Inf;
	/* 配置成员计数器 */
	u32 num1,num2;
	
	/* 1、设置配置表默认参数 */
	*((INSTRU_CONFIG_INF_TAB*)Cfg_Tab_Mem_Psl_Add) = Default_Instru_Config_Inf_State_Tab;
	
	/* 2、读取配置表成员数量 */
	DataLink_Cfg_Tab_Inf_Read((char*)&old_Cfg_Tab_mbr_num,
						  	  Old_Cfg_Tab_Mbr_Num_Ver,
						  	  Old_Cfg_Tab_Mbr_Num_Size);
	/* 判断 配置表成员数量 的合法性*/
	if((old_Cfg_Tab_mbr_num <= Cfg_Tab_Mbr_Sre_Max) && (old_Cfg_Tab_mbr_num > 0))
	{
		/* 配置表成员数量在设置范围内 */
		/* 3、读取配置表状态 */
		DataLink_Cfg_Tab_Inf_Read((char*)&old_Cfg_Tab_state,
								  Old_Cfg_Tab_Stat_Ver,
								  Old_Cfg_Tab_Stat_Size);
		/* 判断 配置状态 */
		if(old_Cfg_Tab_state == Cfg_Tab_Stat)
		{
			/* 配置状态为已配置 */
			/* 3、循环读取配置表中的信息 */
			for(num1 = 0;num1 < old_Cfg_Tab_mbr_num; num1++)
			{
				/* 读取 配置表成员信息 */
				DataLink_Cfg_Tab_Inf_Read((char*)(&old_Cfg_Tab_mbr_Inf),
										  Cfg_Tab_Mbr_Frt_Add+num1*sizeof(Cfg_Tab_Mbr_Def),
										  sizeof(Cfg_Tab_Mbr_Def));

				/* 从当前的配置表信息中查询相同 名称(编号) 的成员 */
				for(num2 = 0;num2 < Cfg_Tab_Inf.Mbr.Mbr_Num; num2++)
				{
					/* 判断当前的配置成员编号与旧的配置编号是否相同. */
					if(old_Cfg_Tab_mbr_Inf.Mbr_NO == ((Cfg_Tab_Mbr_Def*)(Cfg_Tab_Mbr_Psl_Add + num2*sizeof(Cfg_Tab_Mbr_Def)))->Mbr_NO)
					{
						/* 当前编号相同,则判断版本是否一致. */
						if(old_Cfg_Tab_mbr_Inf.Mbr_Ver == ((Cfg_Tab_Mbr_Def*)(Cfg_Tab_Mbr_Psl_Add + num2*sizeof(Cfg_Tab_Mbr_Def)))->Mbr_Ver)
						{
							/* 版本相同 设置 将旧配置表成员参数拷贝给当前配置表成员 */
							/* 读取 旧配置表成员参数 */
							DataLink_Cfg_Tab_Para_Read((char*)((u32)Cfg_Tab_Mem_Psl_Add+((Cfg_Tab_Mbr_Def*)(Cfg_Tab_Mbr_Psl_Add + num2*sizeof(Cfg_Tab_Mbr_Def)))->Mbr_Sre_Add),
													  old_Cfg_Tab_mbr_Inf.Mbr_Sre_Add,
													  old_Cfg_Tab_mbr_Inf.Mbr_Size);
							/* 跳出查询 */
							break;
						}
					}
				}
			}
		}
	}

	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH1.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH2.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH2.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH3.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH3.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH4.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH4.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH5.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH5.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH6.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH6.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH7.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH7.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH8.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH8.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH9.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH9.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH10.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH10.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH11.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH11.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH12.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH12.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH13.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH13.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH14.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH14.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH15.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH15.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH16.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH16.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH17.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH17.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH18.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH18.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH19.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH19.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH20.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH20.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH21.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH21.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH22.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH22.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH23.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH23.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH24.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH24.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH25.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH25.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH26.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH26.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH27.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH27.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH28.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH28.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH29.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH19.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH30.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH30.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH31.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH31.Type;
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH32.Type=Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State.CH32.Type;
	
	/* 4、将当前配置表在外部存储器中存储的信息存重新存到外部的存储器中 */
	/* 写入 配置表成员信息 */
	DataLink_Cfg_Tab_Inf_Write((char*)(&Cfg_Tab_Inf),0,sizeof(Cfg_Tab_Def));
	/* 保存最新的配置表 */
	Write_Config_Tab((u8*)Cfg_Tab_Mem_Psl_Add,sizeof(INSTRU_CONFIG_INF_TAB));
}











