/**
  ******************************************************************************
  * @file              : Log_COM.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015年3月12日 09:55
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
  */
#include "System_Config.h"
  
#include "BSP.h" 
 
#include "Log_COM.h"

#include "Log_Download.h"
#include "Log_GSM.h"
#include "Log_Sensor.h"

#include "Dev_Fram.h"

#include "Mcu_Config.h"
#include "Mcu_Usart.h"

#include <string.h>
#include "app.h"
#include "Mcu_Beep.h"

#include "GSM_app.h"

#include "String_Utility.h"

#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	
#include "Log_GSM_Config.h"
#endif

/************************GSM模块信号强度结构体*****************ZWC********/
extern  GSM_GPRS_SR_Struct GPRS_SR_Reg;
extern void DispMenuDeskTopDown(void);
//static void Com_Config_SMSAamPoeNOSet (void);
/************************************************************************************************/
extern void Instru_Rte_Fty_Set(void);
extern void Parameter_Setting_Successful(void);

/* 打印调试标志 *///ZWC
extern BOOL GSM_DEBUG_Flag;// TRUE




/**********************************串口控制数据记录的启动和停止*********************************/

extern void App_Save_Config(FunctionalState New_state);
/**********************************用于读取数据记录的状态***************************************/
/* 实时数据存储开关 数据指针 SaveDataZWC */
static FunctionalState *Save_Data_Switch_A = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch;

/********************************通过串口电池连接状态和电池电量************************************/
extern u8 Power_ET_Alarm_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);

/*************************RTC时钟设置******************************************/
extern u8 HEX_To_BCD(u8 hex_data);    //HEX转为BCD子程序   
extern u8 BCD_To_HEX(u8 hex_data);    //BCD转为HEX子程序 

extern u8 HEX_Hi4(u8 Data);//取出一个字节数据的高4位
extern u8 HEX_Lo4(u8 Data);//取出一个字节数据的低4位

#define System_Year_Start		(2000u)//串口设置时间；年份开始开始时间



/* 传感器校准通信数据据帧 接收 寄存器 数量 */
#define Com_Sensor_Cal_RX_Reg_Size			(COM_Sensor_Cal_EVENT_Q_Len)
/* 传感器校准通信数据据帧存储寄存器 */
COM_SENSOR_CAL_TYPEDEF	Com_Sensor_Cal_RX_Reg[Com_Sensor_Cal_RX_Reg_Size];

/************************************************************************************************/
//原始版本的传感器结构
//由于配置工具暂时没有人接手更新，这个问题只能先这样做一下处理。
/* 传感器 通道通信数据结构 */
typedef struct {
	/* 传感器 类型 */						//	0:没有设定的传感器类型	1:温度传感器		2:湿度传感器		3:单温卡片	4：温湿卡片	其他:无效
	u32				Type						:8;
	/* 传感器 报警开关 */						//	1:报警开			0:报警关
	u32				SENSOR_Alarm_Switch			:1;	
	/* 传感器 声音报警开关 */					//	1:报警开			0:报警关
	u32				SENSOR_Sound_Alarm_Switch	:1;
	/* 传感器 光报警开关 */					//	1:报警开			0:报警关
	u32				SENSOR_Light_Alarm_Switch	:1;
	/* 传感器 短信报警开关 */					//	1:报警开			0:报警关
	u32				SENSOR_SMS_Alarm_Switch		:1;
	/* 传感器 开关保留 */						//	00:未配置（默认配置）。(该选项不可更改)
	u32				SENSOR_Alarm_Switch_Reserve	:4;	
	/* 传感器 报警状态 */						//	00:传感器未报警	01:传感器报警
	u32				SENSOR_Alarm_State			:2;
	/* 传感器 报警状态保留 */					//	00:未配置（默认配置）。(该选项不可更改)
	u32 			SENSOR_Alarm_State_Reserve	:6;	
	/* 传感器 信息 保留空间 */				//	00:未配置（默认配置）。(该选项不可更改)
	u32				SENSOR_State_Inf_Reserve	:8;
	/* 卡片ID号 */							//	0:未配置（默认配置）
	u32				SENSOR_RF_Card_ID_NO;
	/* 传感器 SN号 */						//	0:未配置（默认配置）
	u8				SENSOR_SN_NO[SENSOR_SN_NO_Size];
	/* 传感器 报警上限 */						//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
	float			SENSOR_Up_Limit;
	/* 传感器 报警下限 */						//	00:未配置（默认配置），出厂前需要配置该项内容。(该选项 可更改)
	float			SENSOR_Low_Limit;
	
}INSTRU_SENSOR_Old;

/************************************************************************************************/

/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Send_Q																*/
/**	功能说明: 发送传感器校准数据消息																*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void Com_Sensor_Cal_Send_Q(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	INT8U      	err;
	OS_Q_DATA	com_sensor_cal_q;

	/* 获得 传感器校准队列 信息 */
	err = OSQQuery (COM_Sensor_Cal_EVENT_Q,&com_sensor_cal_q);
	/* 判断查询队列成功 */
	if(err == OS_ERR_NONE)
	{
		/* 判断 队列中消息数量是否超出限制 */
		if(com_sensor_cal_q.OSNMsgs < COM_Sensor_Cal_EVENT_Q_Len)
		{
			/* 队列 未超限 */
			/* 拷贝传感器校准数据数据 */
			Com_Sensor_Cal_RX_Reg[com_sensor_cal_q.OSNMsgs] = *Cal_Inf;
			/* 向对列中发送消息 */
			err = OSQPost(COM_Sensor_Cal_EVENT_Q,&Com_Sensor_Cal_RX_Reg[com_sensor_cal_q.OSNMsgs]);
		}else
		{
			/* 队列 超限 */
			/* 不发送消息 */
			__nop();
		}	
	}
}

/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Get_Check															*/
/**	功能说明: 计算传感器通信帧的校验																*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
u8 Com_Sensor_Cal_Get_Check(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	u8 check = 0;
	u8 num;
	
	/* 计算传感器通信帧的校验 */
	check += Cal_Inf->Head_Code;
	check += Cal_Inf->Instru_NO;
	check += Cal_Inf->Cmd_Code;
	check += Cal_Inf->Chanel_Num;
	check += Cal_Inf->Data_Len;
	
	for(num = 0;num < (Cal_Inf->Data_Len+1);num++)
	{
		check += Cal_Inf->Com_Data[num];
	}
	
	return check;
}

/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Deleted_Buffer_Data													*/
/**	功能说明: 清除 数据缓存的数据																	*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Deleted_Buffer_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* 清除 数据缓存的数据 */
	memset(Cal_Inf,0X00,sizeof(COM_SENSOR_CAL_TYPEDEF));
}

/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Deleted_Buffer_Data													*/
/**	功能说明: 清除 数据缓存的数据																*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static BOOL Log_Com_Sensor_CH_Check(u8 ch)
{

	/* 获取对应通道的真实值和校准值 */
	switch(ch)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			return TRUE;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			return TRUE;
		}
#endif	
		
		default:
		{
			/* 获取的通道目标不存在 */

			return FALSE;
		}
	}

}

#if (1)

#if (GSM_Module_Type==2) //ESP8266Wifi模块
/************************************************************************************************/
/**	函 数 名: Com_Config_Read_Write_Wifi_Name	*/
/**	功能说明: 读和写Wifi名称***************/
/**	形    参：参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_Name(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	/* 写Wifi名称 应答的数据长度 */
#define Com_Read_Write_Wifi_Name_Data_Len	(1u) //最大1个传输数据
/* 写Wifi名称 应答协议长度 */
//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
#define Com_Read_Write_Wifi_Name_Len		       (Com_Read_Write_Wifi_Name_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* 写Wifi名称的数据地址 */
#define  Read_Write_Wifi_Name_Data_P				   	(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name)
COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_Name;//接收串口数据 
/*Cal_Inf->Chanel_Num=0x55代表读操作    Read_Write_Wifi_Name.Chanel_Num=0xAA代表写操作*/
	    if((Cal_Inf->Chanel_Num==0x55)&&(Cal_Inf->Data_Len<=Wifi_Name_Size))//读
			{	
	        /* 配置 实时数据 参数 */
					Read_Write_Wifi_Name.Head_Code 	   = Com_Sensor_Cal_Rece_Head_Code;
					Read_Write_Wifi_Name.Instru_NO 	   = Cal_Inf->Instru_NO;//仪器编号
					Read_Write_Wifi_Name.Cmd_Code	     = Com_Read_Write_Wifi_Name_Cmd_Code;
					Read_Write_Wifi_Name.Chanel_Num    = Cal_Inf->Chanel_Num;
					Read_Write_Wifi_Name.Data_Len	     = strlen(Read_Write_Wifi_Name_Data_P);
					/* 将数据拷贝到发送寄存器中 */
					memcpy(Read_Write_Wifi_Name.Com_Data,Read_Write_Wifi_Name_Data_P,Read_Write_Wifi_Name.Data_Len);
					/* 设置 常数 */
					*((u8*)&(Read_Write_Wifi_Name.Com_Data[Read_Write_Wifi_Name.Data_Len])) = Com_Sensor_Cal_Constant;
					/* 设置 校验 */
					*((u8*)&(Read_Write_Wifi_Name.Com_Data[Read_Write_Wifi_Name.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Name);
					/* 响应 实时数据 数据信息 */
					USART_printf(USART1,(u8*)&Read_Write_Wifi_Name,(Read_Write_Wifi_Name.Data_Len + Com_Sensor_Cal_Fix_Data_Len));		
			
			}
			else if((Cal_Inf->Chanel_Num==0xAA)&&(Cal_Inf->Data_Len<=Wifi_Name_Size))//写
			{
				/* 清除数据区内容 *//* 写Wifi名称 */
				memset(Read_Write_Wifi_Name_Data_P,'\0',Wifi_Name_Size);
				/* 将数据拷贝到发送寄存器中 */
				memcpy(Read_Write_Wifi_Name_Data_P,Cal_Inf->Com_Data,(Cal_Inf->Data_Len > (Wifi_Name_Size - 1)) ? (Wifi_Name_Size - 1):Cal_Inf->Data_Len);
				Write_Config_Tab((u8*)Read_Write_Wifi_Name_Data_P,sizeof(Read_Write_Wifi_Name_Data_P));/* 保存Wifi名称 */
				/* 回复上位机指令参数组装 */
				Read_Write_Wifi_Name.Head_Code       	= Com_Sensor_Cal_Rece_Head_Code;
				Read_Write_Wifi_Name.Instru_NO 	      = Cal_Inf->Instru_NO;//仪器编号
				Read_Write_Wifi_Name.Cmd_Code		      = Com_Read_Write_Wifi_Name_Cmd_Code;
				Read_Write_Wifi_Name.Chanel_Num	      = Cal_Inf->Chanel_Num;
				Read_Write_Wifi_Name.Data_Len		      = Com_Read_Write_Wifi_Name_Data_Len;
				/* 将数据拷贝到发送寄存器中 */
				Read_Write_Wifi_Name.Com_Data[0]	    = Com_Sensor_Cal_Set_Para_Succeed;
				/* 设置 常数 */
				Read_Write_Wifi_Name.Com_Data[1]	    = Com_Sensor_Cal_Constant;
				/* 设置 校验 */
				Read_Write_Wifi_Name.Com_Data[2] 	    = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Name);
				/* 响应 实时数据 数据信息 */
				USART_printf(USART1,(u8*)&Read_Write_Wifi_Name,Com_Read_Write_Wifi_Name_Len);	
				
				
			
				
			}
			
   #undef Com_Read_Write_Wifi_Name_Data_Len
   #undef Com_Read_Write_Wifi_Name_Len
   #undef Write_Wifi_Name_Data_P				
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Read_Write_Wifi_WSKEY*/
/**	功能说明: 读和写Wifi_WSKEY(加密方式和密码)***************/
/**	形    参：参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_WSKEY(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	/*写Wifi_WSKEY(加密方式和密码) 应答的数据长度 */
#define Com_Read_Write_Wifi_WSKEY_Data_Len	  (1u) //最大1个传输数据
/* 写Wifi_WSKEY(加密方式和密码) 应答协议长度 */
//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
#define Com_Read_Write_Wifi_WSKEY_Len		       (Com_Read_Write_Wifi_WSKEY_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* 写Wifi_WSKEY(加密方式和密码)的数据地址 */
#define  Read_Write_Wifi_WSKEY_Data_P				   (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password)	
COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_WSKEY;//接收串口数据 
/*Cal_Inf->Chanel_Num=0x55代表读操作    Cal_Inf->Chanel_Num=0xAA代表写操作*/
	    if((Cal_Inf->Chanel_Num==0x55)&&(Cal_Inf->Data_Len<=Wifi_Passward_Size))//读
			{				
	        /* 配置 实时数据 参数 */
					Read_Write_Wifi_WSKEY.Head_Code 	   = Com_Sensor_Cal_Rece_Head_Code;
					Read_Write_Wifi_WSKEY.Instru_NO 	   = Cal_Inf->Instru_NO;//仪器编号
					Read_Write_Wifi_WSKEY.Cmd_Code	     = Com_Read_Write_Wifi_WSKEY_Cmd_Code;
					Read_Write_Wifi_WSKEY.Chanel_Num     = Cal_Inf->Chanel_Num;
					Read_Write_Wifi_WSKEY.Data_Len	     = strlen(&Read_Write_Wifi_WSKEY_Data_P[1])+2;
					Read_Write_Wifi_WSKEY.Com_Data[0]    =0;//默认无加密方式
					/* 将数据拷贝到发送寄存器中 */
					memcpy(&Read_Write_Wifi_WSKEY.Com_Data[1],Read_Write_Wifi_WSKEY_Data_P,Read_Write_Wifi_WSKEY.Data_Len);
				
					/* 设置 常数 */
					*((u8*)&(Read_Write_Wifi_WSKEY.Com_Data[Read_Write_Wifi_WSKEY.Data_Len])) = Com_Sensor_Cal_Constant;
					/* 设置 校验 */
					*((u8*)&(Read_Write_Wifi_WSKEY.Com_Data[Read_Write_Wifi_WSKEY.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_WSKEY);
				
				
					/* 响应 实时数据 数据信息 */
					USART_printf(USART1,(u8*)&Read_Write_Wifi_WSKEY,(Read_Write_Wifi_WSKEY.Data_Len + Com_Sensor_Cal_Fix_Data_Len));		
			
			}
			else if((Cal_Inf->Chanel_Num==0xAA)&&(Cal_Inf->Data_Len<=Wifi_Passward_Size))//写
			{
				/* 清除数据区内容 *//* 写Wifi名称 */
				memset(Read_Write_Wifi_WSKEY_Data_P,'\0',Wifi_Passward_Size);
				/* 将数据拷贝到发送寄存器中 */
				memcpy(Read_Write_Wifi_WSKEY_Data_P,&Cal_Inf->Com_Data[1],(Cal_Inf->Data_Len > (Wifi_Passward_Size - 1)) ? (Wifi_Passward_Size - 1):Cal_Inf->Data_Len-1);
				
				Write_Config_Tab((u8*)Read_Write_Wifi_WSKEY_Data_P,sizeof(Read_Write_Wifi_WSKEY_Data_P));/* 保存Wifi密码  */
				/* 回复上位机指令参数组装 */
				Read_Write_Wifi_WSKEY.Head_Code       	= Com_Sensor_Cal_Rece_Head_Code;
				Read_Write_Wifi_WSKEY.Instru_NO 	      = Cal_Inf->Instru_NO;//仪器编号
				Read_Write_Wifi_WSKEY.Cmd_Code		      = Com_Read_Write_Wifi_WSKEY_Cmd_Code;
				Read_Write_Wifi_WSKEY.Chanel_Num	      = Cal_Inf->Chanel_Num;
				Read_Write_Wifi_WSKEY.Data_Len		      = Com_Read_Write_Wifi_WSKEY_Data_Len;
				/* 将数据拷贝到发送寄存器中 */
				Read_Write_Wifi_WSKEY.Com_Data[0]	      = Com_Sensor_Cal_Set_Para_Succeed;
				/* 设置 常数 */
				Read_Write_Wifi_WSKEY.Com_Data[1]	      = Com_Sensor_Cal_Constant;
				/* 设置 校验 */
				Read_Write_Wifi_WSKEY.Com_Data[2] 	    = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_WSKEY);
//////////						OSTimeDly(100);//延时100ms
				/* 响应 实时数据 数据信息 */
				USART_printf(USART1,(u8*)&Read_Write_Wifi_WSKEY,Com_Read_Write_Wifi_WSKEY_Len);	
				
				
				
					GSM_Config_Reg.WifiNamePWD_Config_State=TRUE;//上位机下发了配置Wifi模块的名字及密码命令
//////				GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,SetWifiNamePWD_EVENT);//发送设置Wifi模块名字和密码消息
				
							
			}
#undef Com_Read_Write_Wifi_Name_Data_Len
#undef Com_Read_Write_Wifi_Name_Len
#undef Write_Wifi_Name_Data_P	
			
}
/************************************************************************************************/
/**	函 数 名: Com_Config_Read_Write_Wifi_Wlan*/
/**	功能说明: 读和写Wifi_WANN(LOCALIP:本地IP地址、  MASK(子网掩码) 、GATEWAY(网关地址))***************/
/**	形    参：参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_Wlan(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
   /* 应答协议寄存器 */
   COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_Wlan;//接收串口数据 
/*Cal_Inf->Chanel_Num=0x55代表读操作    Cal_Inf->Chanel_Num=0xAA代表写操作*/
	    if(Cal_Inf->Chanel_Num==0x55)//读
			{	
	      	memset((u8*)&Read_Write_Wifi_Wlan,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF)); 
				  ConvertIP_StringToArray(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,&Read_Write_Wifi_Wlan.Com_Data[0],3,".");//本地IP地址  4个字节  3个  .
				  ConvertIP_StringToArray(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,&Read_Write_Wifi_Wlan.Com_Data[4],3,".");//子网掩码 
				  ConvertIP_StringToArray(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,&Read_Write_Wifi_Wlan.Com_Data[8],3,"."); //网关IP地址 
				
				  Read_Write_Wifi_Wlan.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	        Read_Write_Wifi_Wlan.Instru_NO 	= Cal_Inf->Instru_NO;
	        Read_Write_Wifi_Wlan.Cmd_Code		= Com_Read_Write_Wifi_WALN_Cmd_Code;
	        Read_Write_Wifi_Wlan.Chanel_Num	= Cal_Inf->Chanel_Num;
	        Read_Write_Wifi_Wlan.Data_Len		= 12u;//4个字节LOCALIP+4个字节MASK+4个字节GATEWAY
				  Read_Write_Wifi_Wlan.Com_Data[Read_Write_Wifi_Wlan.Data_Len] = Com_Sensor_Cal_Constant;
	        Read_Write_Wifi_Wlan.Com_Data[Read_Write_Wifi_Wlan.Data_Len+1] = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Wlan);
	        USART_printf(USART1,(u8*)&Read_Write_Wifi_Wlan,(Com_Sensor_Cal_Fix_Data_Len+Read_Write_Wifi_Wlan.Data_Len));	
			}
			else if(Cal_Inf->Chanel_Num==0xAA)//写
			{
				
				

		    sprintf(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,"%d.%d.%d.%d",
		                               Cal_Inf->Com_Data[0],
		                               Cal_Inf->Com_Data[1],
		                               Cal_Inf->Com_Data[2],
		                               Cal_Inf->Com_Data[3]);	//本地IP地址
				
				
			  sprintf(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,"%d.%d.%d.%d",
		                               Cal_Inf->Com_Data[4],
		                               Cal_Inf->Com_Data[5],
		                               Cal_Inf->Com_Data[6],
		                               Cal_Inf->Com_Data[7]);//子网掩码 
				
			  sprintf(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,"%d.%d.%d.%d",
		                               Cal_Inf->Com_Data[8],
		                               Cal_Inf->Com_Data[9],
		                               Cal_Inf->Com_Data[10],
		                               Cal_Inf->Com_Data[11]);//网关IP地址

				Read_Write_Wifi_Wlan.Head_Code 	   = Com_Sensor_Cal_Rece_Head_Code;
	      Read_Write_Wifi_Wlan.Instru_NO 	   = Cal_Inf->Instru_NO;
	      Read_Write_Wifi_Wlan.Cmd_Code	     = ComConfigDataNetworkWrite;
	      Read_Write_Wifi_Wlan.Chanel_Num    = Cal_Inf->Chanel_Num;
	      Read_Write_Wifi_Wlan.Data_Len	     = (1u);	
				Read_Write_Wifi_Wlan.Com_Data[0]	 = Com_Sensor_Cal_Set_Para_Succeed;
	      Read_Write_Wifi_Wlan.Com_Data[1]	 = Com_Sensor_Cal_Constant;
	      Read_Write_Wifi_Wlan.Com_Data[2]   = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Wlan);
				
////////							OSTimeDly(100);//延时100ms
	      USART_printf(USART1,(u8*)&Read_Write_Wifi_Wlan,(Com_Sensor_Cal_Fix_Data_Len+1));	
				
				
				GSM_Config_Reg.WifiWLAN_Config_State=TRUE;//上位机下发了配置Wifi模块的WLAN参数命令
				

				  
				GSM_Config_Reg.GSM_Config_State=TRUE;//开始配置Wifi模块参数
				GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);//下发初始化Wifi模块消息
	
				
				
				
////////				
////////				//			/* Wifi设置WifiWALN参数*/
////////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,SetWifiWLAN_EVENT);
				
				
			}		
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Wifi_NetP_Read														*/
/**	功能说明: 读Wifi数据服务器的配置参数																*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Wifi_NetP_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
//	/* 网络配置指针 */
//	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;

	/* 设置局部变量初始值 */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* 配置数据服务器参数 */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= Com_Read_Write_Wifi_NETP_Cmd_Code;
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 6u;


   ackDataNetworkRead.Com_Data[0]=(0xFF00&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1)>>8;
   ackDataNetworkRead.Com_Data[1]=(0xFF&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1);
	 ackDataNetworkRead.Com_Data[2]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A;
   ackDataNetworkRead.Com_Data[3]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B;
	 ackDataNetworkRead.Com_Data[4]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C;
   ackDataNetworkRead.Com_Data[5]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D;
	 

		   
	/* 设置 常数 */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* 设置 校验 */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}

/************************************************************************************************/
/**	函 数 名: Com_Config_Wifi_NetP_Write														*/
/**	功能说明: 写数据服务器的配置参数																*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Wifi_NetP_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	  /* 应答协议寄存器 */
	  COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	  /* 数据合法性标志 */
	  BOOL data_vldy = TRUE;

////////	
////////		/* 获取网络数据 */
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A = Cal_Inf->Com_Data[2];
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B = Cal_Inf->Com_Data[3];
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C = Cal_Inf->Com_Data[4];
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D = Cal_Inf->Com_Data[5];
////////	
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1 = (Cal_Inf->Com_Data[0]<<8)|(Cal_Inf->Com_Data[1]&0xff);
	
	
	
////		/* 设置清除域名寄存器 */
////		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,'\0',32);


////		/* 修改仪器配置表中对应IP的数据 */
////		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address,
////						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address));
////		/* 修改仪器配置表中对应端口的数据 */
////		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1,
////						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1));
////		/* 修改仪器配置表中对应域名的数据 */
////		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
////						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));	

	
	/* 配置 实时数据 参数 */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	  = Com_Read_Write_Wifi_NETP_Cmd_Code;
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	  = (1u);
	
	/* 将数据拷贝到发送寄存器中 */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* 设置 常数 */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Read_Write_Wifi_NetP*/
/**	功能说明: 读和写Wifi的NETP相关参数IP地址和端口***************/
/**	形    参：参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_NetP(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{ 
/*Cal_Inf->Chanel_Num=0x55代表读操作    Cal_Inf->Chanel_Num=0xAA代表写操作*/
	         if(Cal_Inf->Chanel_Num==0x55)//读
			     {	
						 
						 Com_Config_Wifi_NetP_Read( Cal_Inf);

			     }
			    else if(Cal_Inf->Chanel_Num==0xAA)//写
			    {
	           Com_Config_Wifi_NetP_Write( Cal_Inf); 
					 }						

}
/************************************************************************************************/
/**	函 数 名: Com_Config_Read_Write_Wifi_DHCP*/
/**	功能说明: 读和写Wifi模块获取IP地址方式***************/
/**	形    参：参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_DHCP(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
   /* 应答协议寄存器 */
   COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_DHCP;//接收串口数据 
/*Cal_Inf->Chanel_Num=0x55代表读操作    Cal_Inf->Chanel_Num=0xAA代表写操作*/
	    if(Cal_Inf->Chanel_Num==0x55)//读
			{	
	      	memset((u8*)&Read_Write_Wifi_DHCP,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF)); 
				  Read_Write_Wifi_DHCP.Head_Code 	  = Com_Sensor_Cal_Rece_Head_Code;
	        Read_Write_Wifi_DHCP.Instru_NO 	  = Cal_Inf->Instru_NO;
	        Read_Write_Wifi_DHCP.Cmd_Code		  = Com_Read_Write_Wifi_DHCP_Cmd_Code;
	        Read_Write_Wifi_DHCP.Chanel_Num	  = Cal_Inf->Chanel_Num;
	        Read_Write_Wifi_DHCP.Data_Len		  = 1u;//1个字节      
				  Read_Write_Wifi_DHCP.Com_Data[0]  = Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way  ;//0x00代表获取IP地址的方式为STATIC   0x01代表获取IP地址的方式为DHCP 
				  Read_Write_Wifi_DHCP.Com_Data[Read_Write_Wifi_DHCP.Data_Len] = Com_Sensor_Cal_Constant;
	        Read_Write_Wifi_DHCP.Com_Data[Read_Write_Wifi_DHCP.Data_Len+1] = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_DHCP);
	        USART_printf(USART1,(u8*)&Read_Write_Wifi_DHCP,(Com_Sensor_Cal_Fix_Data_Len+Read_Write_Wifi_DHCP.Data_Len));	
			}
			else if(Cal_Inf->Chanel_Num==0xAA)//写
			{
				 Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way = Cal_Inf->Com_Data[0];//0x00代表获取IP地址的方式为STATIC   0x01代表获取IP地址的方式为DHCP 
				 Read_Write_Wifi_DHCP.Head_Code 	       = Com_Sensor_Cal_Rece_Head_Code;
	       Read_Write_Wifi_DHCP.Instru_NO 	       = Cal_Inf->Instru_NO;
	       Read_Write_Wifi_DHCP.Cmd_Code	         = Com_Read_Write_Wifi_DHCP_Cmd_Code;
	       Read_Write_Wifi_DHCP.Chanel_Num         = Cal_Inf->Chanel_Num;
	       Read_Write_Wifi_DHCP.Data_Len	         = (1u);	
				 Read_Write_Wifi_DHCP.Com_Data[0]	       = Com_Sensor_Cal_Set_Para_Succeed;
	       Read_Write_Wifi_DHCP.Com_Data[1]	       = Com_Sensor_Cal_Constant;
	       Read_Write_Wifi_DHCP.Com_Data[2]        = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_DHCP);
				
////////////							OSTimeDly(100);//延时100ms
	       USART_printf(USART1,(u8*)&Read_Write_Wifi_DHCP,(Com_Sensor_Cal_Fix_Data_Len+1));	
				
				
				GSM_Config_Reg.WifiWLAN_GET_IP_WAY_Config_State=TRUE;//上位机下发了配置Wifi模块的获取IP地址方式命令
				
				if((GSM_Config_Reg.WifiWLAN_Config_State==FALSE)&&(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==0x01))
				{
				     GSM_Config_Reg.GSM_Config_State=TRUE;//开始配置Wifi模块参数
				   GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);//下发初始化Wifi模块消息
				}
				
				
				
				
////////				 /* Wifi模块获取IP地址方式参数*/
////////			   GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,SetWifiWLAN_GET_IP_WAY_EVENT);

			}		
}
/************************************************************************************************/
/**	函 数 名: Com_Config_Read_Write_Wifi_MAC*/
/**	功能说明: 读和写Wifi模块的MAC地址***************/
/**	形    参：参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_MAC(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
   /* 应答协议寄存器 */
   COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_MAC;//接收串口数据 
/*Cal_Inf->Chanel_Num=0x55代表读操作    Cal_Inf->Chanel_Num=0xAA代表写操作*/
	    if(Cal_Inf->Chanel_Num==0x55)//读
			{	
	      	memset((u8*)&Read_Write_Wifi_MAC,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF)); 
				  Read_Write_Wifi_MAC.Head_Code 	  = Com_Sensor_Cal_Rece_Head_Code;
	        Read_Write_Wifi_MAC.Instru_NO 	  = Cal_Inf->Instru_NO;
	        Read_Write_Wifi_MAC.Cmd_Code		  = Com_Read_Write_Wifi_MAC_Cmd_Code;
	        Read_Write_Wifi_MAC.Chanel_Num	  = Cal_Inf->Chanel_Num;
	        Read_Write_Wifi_MAC.Data_Len		  = strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address);//MAC地址长度字节 
				  memcpy(Read_Write_Wifi_MAC.Com_Data,Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address, Read_Write_Wifi_MAC.Data_Len);
				  Read_Write_Wifi_MAC.Com_Data[Read_Write_Wifi_MAC.Data_Len]    = Com_Sensor_Cal_Constant;
	        Read_Write_Wifi_MAC.Com_Data[Read_Write_Wifi_MAC.Data_Len+1]  = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_MAC);
	        USART_printf(USART1,(u8*)&Read_Write_Wifi_MAC,(Com_Sensor_Cal_Fix_Data_Len+Read_Write_Wifi_MAC.Data_Len));

				
			}
			else if(Cal_Inf->Chanel_Num==0xAA)//写
			{
				
////////				 memcpy(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address,Cal_Inf->Com_Data,Cal_Inf->Data_Len);

				 Read_Write_Wifi_MAC.Head_Code 	       = Com_Sensor_Cal_Rece_Head_Code;
	       Read_Write_Wifi_MAC.Instru_NO 	       = Cal_Inf->Instru_NO;
	       Read_Write_Wifi_MAC.Cmd_Code	         = Com_Read_Write_Wifi_MAC_Cmd_Code;
	       Read_Write_Wifi_MAC.Chanel_Num        = Cal_Inf->Chanel_Num;
	       Read_Write_Wifi_MAC.Data_Len	         = (1u);	
				 Read_Write_Wifi_MAC.Com_Data[0]	     = Com_Sensor_Cal_Set_Para_Succeed;
	       Read_Write_Wifi_MAC.Com_Data[1]	     = Com_Sensor_Cal_Constant;
	       Read_Write_Wifi_MAC.Com_Data[2]       = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_MAC);
	       USART_printf(USART1,(u8*)&Read_Write_Wifi_MAC,(Com_Sensor_Cal_Fix_Data_Len+1));	

			}		
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Rest_Wifi*/
/**	功能说明:复位或者恢复出厂设置Wifi模块***************/
/**	形    参：参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Rest_Wifi(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
   /* 应答协议寄存器 */
   COM_SENSOR_CAL_TYPEDEF                          Read_Rest_Wifi;//接收串口数据 
/*Cal_Inf->Chanel_Num=0x55代表读操作    Cal_Inf->Chanel_Num=0xAA代表写操作*/
      if(Cal_Inf->Chanel_Num==0xAA)//写
			{
				  if(Cal_Inf->Com_Data[0]==1)
					{//完成WIFI参数修改，退出配置模式，更新配置后进入自动上传模式。

					}
					else if(Cal_Inf->Com_Data[0]==2)
					{
									OSTimeDly(100);//延时100ms
						//强制复位WIFI模块
					  /* 复位Wifi模块*/
             GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,RestWifi_EVENT);
					}
					else if(Cal_Inf->Com_Data[0]==3)
					{
									OSTimeDly(100);//延时100ms
						//强制恢复出厂设置
		   	    /* 恢复出厂设置Wifi模块*/
      		 	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Factory_Data_Reset_Wifi_EVENT);
					}
					
				 Read_Rest_Wifi.Head_Code 	           = Com_Sensor_Cal_Rece_Head_Code;
	       Read_Rest_Wifi.Instru_NO 	           = Cal_Inf->Instru_NO;
	       Read_Rest_Wifi.Cmd_Code	         = Com_Set_Rest_Wifi_Cmd_Code;
	       Read_Rest_Wifi.Chanel_Num        = Cal_Inf->Chanel_Num;
	       Read_Rest_Wifi.Data_Len	         = (1u);	
				 Read_Rest_Wifi.Com_Data[0]	     = Com_Sensor_Cal_Set_Para_Succeed;
	       Read_Rest_Wifi.Com_Data[1]	     = Com_Sensor_Cal_Constant;
	       Read_Rest_Wifi.Com_Data[2]       = Com_Sensor_Cal_Get_Check(&Read_Rest_Wifi);
	       USART_printf(USART1,(u8*)&Read_Rest_Wifi,(Com_Sensor_Cal_Fix_Data_Len+1));		
			}		
}
#endif
/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Read_Real_Data														*/
/**	功能说明: 获取传感器实时数据																	*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Real_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Real_Data_Len	 	(11u)//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+4(数据)+1(常数)+1(校验和)
#define Com_Sensor_Cal_Read_data_buffer_Size	(sizeof(float))//单精度浮点型数据长度
/* 最多配置的传感器数量 */
	COM_SENSOR_CAL_TYPEDEF read_real_data_ack;
	float com_data_buffer;

	/* 配置 实时数据 参数 */
	read_real_data_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;	
	read_real_data_ack.Instru_NO 	= Cal_Inf->Instru_NO;
	read_real_data_ack.Cmd_Code	 	= Com_Sensor_Cal_Cmd_Code_Read_Real_Data;
	read_real_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_real_data_ack.Data_Len		= Com_Sensor_Cal_Read_data_buffer_Size;
	
	
	/* 判断获取的是 第几通道 的传感器数据曲线 */
	/* 获取对应通道的真实值和校准值 */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
		default:
		{
			/* 获取的通道目标不存在 */
			__nop();
			return;
		}
	}

	memcpy(read_real_data_ack.Com_Data,(u8*)(&com_data_buffer),Com_Sensor_Cal_Read_data_buffer_Size);
	/* 设置 常数 */
	read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size] = Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size + 1] 	= Com_Sensor_Cal_Get_Check(&read_real_data_ack);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&read_real_data_ack,Com_Sensor_Cal_Read_Real_Data_Len);
#undef Com_Sensor_Cal_Read_Real_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
}

#else
/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Read_Real_Data														*/
/**	功能说明: 获取传感器实时数据																	*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Real_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Real_Data_Len	 	(9u)//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+2(数据)+1(常数)+1(校验和)
#define Com_Sensor_Cal_Read_data_buffer_Size	(2u)//数据长度：2字节
#define Com_Sensor_Cal_Data_Ratio_K				(10u)//数据放大倍数
#define Com_Sensor_Cal_Temp_Data_Offset_Value	(2731u)//温度值数据的偏移值


	/* 最多配置的传感器数量 */
		COM_SENSOR_CAL_TYPEDEF read_real_data_ack;
		u16 com_data_buffer;
	
		/* 配置 实时数据 参数 */
		read_real_data_ack.Head_Code	= Com_Sensor_Cal_Rece_Head_Code;	
		read_real_data_ack.Instru_NO	= Cal_Inf->Instru_NO;
		read_real_data_ack.Cmd_Code 	= Com_Sensor_Cal_Cmd_Code_Read_Real_Data;
		read_real_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
		read_real_data_ack.Data_Len 	= Com_Sensor_Cal_Read_data_buffer_Size;
		
		
		/* 判断获取的是 第几通道 的传感器数据曲线 */
		/* 获取对应通道的真实值和校准值 */
		switch(Cal_Inf->Chanel_Num)
		{
#if	(Sensor_CH1_Config_Type != Sensor_Null)
			case Sensors_CH_1:
			{
#if	((Sensor_CH1_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}	
			
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
			case Sensors_CH_2:
			{
#if	((Sensor_CH2_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
			case Sensors_CH_3:
			{
#if	((Sensor_CH3_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
			case Sensors_CH_4:
			{
#if	((Sensor_CH4_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif
				break;
			}
#endif	
			
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
			case Sensors_CH_5:
			{
#if	((Sensor_CH5_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH5_Config_Type == Sensor_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
			case Sensors_CH_6:
			{
#if	((Sensor_CH6_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH6_Config_Type == Sensor_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
			case Sensors_CH_7:
			{
#if	((Sensor_CH7_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH7_Config_Type == Sensor_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
			case Sensors_CH_8:
			{
#if	((Sensor_CH8_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH8_Config_Type == Sensor_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
			case Sensors_CH_9:
			{
#if	((Sensor_CH9_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH9_Config_Type == Sensor_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}				
#endif	
			
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
			case Sensors_CH_10:
			{
#if	((Sensor_CH10_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH10_Config_Type == Sensor_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
			case Sensors_CH_11:
			{
#if	((Sensor_CH11_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH11_Config_Type == Sensor_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
			case Sensors_CH_12:
			{
#if	((Sensor_CH12_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH12_Config_Type == Sensor_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
			case Sensors_CH_13:
			{
#if	((Sensor_CH13_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH13_Config_Type == Sensor_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
			case Sensors_CH_14:
		{
#if	((Sensor_CH14_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH14_Config_Type == Sensor_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
			case Sensors_CH_15:
			{
#if	((Sensor_CH15_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH15_Config_Type == Sensor_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
			case Sensors_CH_16:
			{
#if	((Sensor_CH16_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH16_Config_Type == Sensor_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
			case Sensors_CH_17:
			{
#if	((Sensor_CH17_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH17_Config_Type == Sensor_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
			case Sensors_CH_18:
			{
#if	((Sensor_CH18_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH18_Config_Type == Sensor_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
			case Sensors_CH_19:
			{
#if	((Sensor_CH19_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH19_Config_Type == Sensor_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
			case Sensors_CH_20:
			{
#if	((Sensor_CH20_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH20_Config_Type == Sensor_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
			case Sensors_CH_21:
			{
#if	((Sensor_CH21_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH21_Config_Type == Sensor_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
			case Sensors_CH_22:
			{
#if	((Sensor_CH22_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH22_Config_Type == Sensor_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
			case Sensors_CH_23:
			{
#if	((Sensor_CH23_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH23_Config_Type == Sensor_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
			case Sensors_CH_24:
			{
#if	((Sensor_CH24_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH24_Config_Type == Sensor_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
			case Sensors_CH_25:
			{
#if	((Sensor_CH25_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH25_Config_Type == Sensor_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
			case Sensors_CH_26:
			{
#if	((Sensor_CH26_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH26_Config_Type == Sensor_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
			case Sensors_CH_27:
			{
#if	((Sensor_CH27_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH27_Config_Type == Sensor_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
			case Sensors_CH_28:
			{
#if	((Sensor_CH28_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH28_Config_Type == Sensor_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
			case Sensors_CH_29:
			{
#if	((Sensor_CH29_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH29_Config_Type == Sensor_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
			case Sensors_CH_30:
			{
#if	((Sensor_CH30_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH30_Config_Type == Sensor_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
			case Sensors_CH_31:
			{
#if	((Sensor_CH31_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH31_Config_Type == Sensor_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
			case Sensors_CH_32:
			{
#if	((Sensor_CH32_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH32_Config_Type == Sensor_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
			default:
			{
				/* 获取的通道目标不存在 */
				__nop();
				return;
			}
		}
#endif
		memcpy(read_real_data_ack.Com_Data,(u8*)(&com_data_buffer),Com_Sensor_Cal_Read_data_buffer_Size);
		/* 设置 常数 */
		read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size] = Com_Sensor_Cal_Constant;
		/* 设置 校验 */
		read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size + 1]	= Com_Sensor_Cal_Get_Check(&read_real_data_ack);
		/* 响应 实时数据 数据信息 */
		USART_printf(USART1,(u8*)&read_real_data_ack,Com_Sensor_Cal_Read_Real_Data_Len);

		
#undef Com_Sensor_Cal_Read_Real_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
#undef Com_Sensor_Cal_Data_Ratio_K
#undef Com_Sensor_Cal_Temp_Data_Offset_Value
}
#endif


#if (1)
/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Read_Orig_Data														*/
/**	功能说明: 获取传感器原始数据																	*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Orig_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Orig_Data_Len		(11u)//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+2(数据)+1(常数)+1(校验和)
#define Com_Sensor_Cal_Read_data_buffer_Size	(sizeof(float))//单精度浮点型数据长度
	COM_SENSOR_CAL_TYPEDEF read_Orig_data_ack;
	float com_data_buffer;

	/* 配置 实时数据 参数 */
	read_Orig_data_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_Orig_data_ack.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	read_Orig_data_ack.Cmd_Code	 	= Com_Sensor_Cal_Cmd_Code_Read_Orig_Data;
	read_Orig_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_Orig_data_ack.Data_Len		= Com_Sensor_Cal_Read_data_buffer_Size;
	
	
	/* 判断获取的是 第几通道 的传感器数据曲线 */
	/* 获取对应通道的真实值和校准值 */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
			break;
		}
#endif	
		
		default:
		{
			/* 获取的通道目标不存在 */
			__nop();
			return;
		}
	}
	
	memcpy(read_Orig_data_ack.Com_Data,(u8*)&com_data_buffer,Com_Sensor_Cal_Read_data_buffer_Size);
	/* 设置 常数 */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size] 	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size+1] = Com_Sensor_Cal_Get_Check(&read_Orig_data_ack);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&read_Orig_data_ack,Com_Sensor_Cal_Read_Orig_Data_Len);
#undef Com_Sensor_Cal_Read_Orig_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
}
#else
/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Read_Orig_Data														*/
/**	功能说明: 获取传感器原始数据																	*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Orig_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Orig_Data_Len		(9u)//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+2(数据)+1(常数)+1(校验和)
#define Com_Sensor_Cal_Read_data_buffer_Size	(2u)//数据长度：2字节
#define Com_Sensor_Cal_Data_Ratio_K				(10u)//数据放大倍数
#define Com_Sensor_Cal_Temp_Data_Offset_Value	(2731u)//温度值数据的偏移值


	COM_SENSOR_CAL_TYPEDEF read_Orig_data_ack;
	u16 com_data_buffer;

	/* 配置 实时数据 参数 */
	read_Orig_data_ack.Head_Code	= Com_Sensor_Cal_Rece_Head_Code;
	read_Orig_data_ack.Instru_NO	= Cal_Inf->Instru_NO;//仪器编号
	read_Orig_data_ack.Cmd_Code 	= Com_Sensor_Cal_Cmd_Code_Read_Orig_Data;
	read_Orig_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_Orig_data_ack.Data_Len 	= Com_Sensor_Cal_Read_data_buffer_Size;
	
			
			/* 判断获取的是 第几通道 的传感器数据曲线 */
			/* 获取对应通道的真实值和校准值 */
			switch(Cal_Inf->Chanel_Num)
			{
#if	(Sensor_CH1_Config_Type != Sensor_Null)
				case Sensors_CH_1:
				{
#if	((Sensor_CH1_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
				case Sensors_CH_2:
				{
#if	((Sensor_CH2_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
				case Sensors_CH_3:
				{
#if	((Sensor_CH3_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
				case Sensors_CH_4:
				{
#if	((Sensor_CH4_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif
					break;
				}
#endif	
				
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
				case Sensors_CH_5:
				{
#if	((Sensor_CH5_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH5_Config_Type == Sensor_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
				case Sensors_CH_6:
				{
#if	((Sensor_CH6_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH6_Config_Type == Sensor_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
				case Sensors_CH_7:
				{
#if	((Sensor_CH7_Config_Type == Sensor_Tmp)||\
					(Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH7_Config_Type == Sensor_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
				case Sensors_CH_8:
				{
#if	((Sensor_CH8_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH8_Config_Type == Sensor_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
				case Sensors_CH_9:
				{
#if	((Sensor_CH9_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH9_Config_Type == Sensor_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}				
#endif	
				
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
				case Sensors_CH_10:
				{
#if	((Sensor_CH10_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH10_Config_Type == Sensor_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
				case Sensors_CH_11:
				{
#if	((Sensor_CH11_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH11_Config_Type == Sensor_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
				case Sensors_CH_12:
				{
#if	((Sensor_CH12_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH12_Config_Type == Sensor_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
				case Sensors_CH_13:
				{
#if	((Sensor_CH13_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH13_Config_Type == Sensor_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
				case Sensors_CH_14:
			{
#if	((Sensor_CH14_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH14_Config_Type == Sensor_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
				case Sensors_CH_15:
				{
#if	((Sensor_CH15_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH15_Config_Type == Sensor_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
				case Sensors_CH_16:
				{
#if	((Sensor_CH16_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH16_Config_Type == Sensor_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
				case Sensors_CH_17:
				{
#if	((Sensor_CH17_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH17_Config_Type == Sensor_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
				case Sensors_CH_18:
				{
#if	((Sensor_CH18_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH18_Config_Type == Sensor_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
				case Sensors_CH_19:
				{
#if	((Sensor_CH19_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH19_Config_Type == Sensor_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
				case Sensors_CH_20:
				{
#if	((Sensor_CH20_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH20_Config_Type == Sensor_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
				case Sensors_CH_21:
				{
#if	((Sensor_CH21_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH21_Config_Type == Sensor_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
				case Sensors_CH_22:
				{
#if	((Sensor_CH22_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH22_Config_Type == Sensor_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
				case Sensors_CH_23:
				{
#if	((Sensor_CH23_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH23_Config_Type == Sensor_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
				case Sensors_CH_24:
				{
#if	((Sensor_CH24_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH24_Config_Type == Sensor_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
				case Sensors_CH_25:
				{
#if	((Sensor_CH25_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH25_Config_Type == Sensor_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
				case Sensors_CH_26:
				{
#if	((Sensor_CH26_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH26_Config_Type == Sensor_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
				case Sensors_CH_27:
				{
#if	((Sensor_CH27_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH27_Config_Type == Sensor_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
				case Sensors_CH_28:
				{
#if	((Sensor_CH28_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH28_Config_Type == Sensor_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
				case Sensors_CH_29:
				{
#if	((Sensor_CH29_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH29_Config_Type == Sensor_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
				case Sensors_CH_30:
				{
#if	((Sensor_CH30_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH30_Config_Type == Sensor_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
				case Sensors_CH_31:
				{
#if	((Sensor_CH31_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH31_Config_Type == Sensor_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
				case Sensors_CH_32:
				{
#if	((Sensor_CH32_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH32_Config_Type == Sensor_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
				default:
				{
					/* 获取的通道目标不存在 */
					__nop();
					return;
				}
			}

	
	memcpy(read_Orig_data_ack.Com_Data,(u8*)&com_data_buffer,Com_Sensor_Cal_Read_data_buffer_Size);
	/* 设置 常数 */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size+1] = Com_Sensor_Cal_Get_Check(&read_Orig_data_ack);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&read_Orig_data_ack,Com_Sensor_Cal_Read_Orig_Data_Len);
#undef Com_Sensor_Cal_Read_Orig_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
#undef Com_Sensor_Cal_Data_Ratio_K
#undef Com_Sensor_Cal_Temp_Data_Offset_Value
}
#endif


/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Write_Curve_Data													*/
/**	功能说明: 修改传感器对应通道曲线数据															*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Write_Curve_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* 传感器校准 写曲线 数据帧长度 */
#define Com_Sensor_Cal_Write_Curve_Data_Len			(1u)//最大103个传输数据
/* 传感器校准 写曲线 数据帧长度 *///数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
#define Com_Sensor_Cal_Write_Curve_Packet_Len		(Com_Sensor_Cal_Write_Curve_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//最大103个传输数据	
	
	/* 传感器真实值数据指针 */
	u16* sensor_act_curve_data_p;
	/* 传感器真实值数据指针 */
	u16* sensor_cal_curve_data_p;
	/* 发送 传感器写曲线 数据帧 寄存器 */
	COM_SENSOR_CAL_TYPEDEF write_curve_ack;
	
	
	/* 判断获取的是 第几通道 的传感器数据曲线 */
	/* 获取对应通道的真实值和校准值 */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value;
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value;
			break;
		}
#endif	
		
		default:
		{
			/* 获取的通道目标不存在 */
			__nop();
			return;
		}
	}
	

	/* 更新对应传感器通道的校准数据 */
	memcpy((u8*)sensor_cal_curve_data_p,Cal_Inf->Com_Data,Com_Sensor_Cal_Curve_Data_Len);
	memcpy((u8*)sensor_act_curve_data_p,&Cal_Inf->Com_Data[Com_Sensor_Cal_Curve_Data_Len],Com_Sensor_Cal_Curve_Data_Len);

	/* 修改 仪器 信息表 设置标志 */
	Write_Config_Tab((u8*)sensor_act_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	
	/* 修改 仪器 信息表 设置标志 */
	Write_Config_Tab((u8*)sensor_cal_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	
	/* 设置 响应数据帧 参数 */
	write_curve_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_curve_ack.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	write_curve_ack.Cmd_Code	= Com_Sensor_Cal_Cmd_Code_Write_Curve_Data;
	write_curve_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	write_curve_ack.Data_Len	= Com_Sensor_Cal_Write_Curve_Data_Len;
	/* 将数据拷贝到发送寄存器中 */
	write_curve_ack.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* 设置 常数 */
	write_curve_ack.Com_Data[1]	= Com_Sensor_Cal_Constant;	
	/* 设置 校验 */
	write_curve_ack.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_curve_ack);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&write_curve_ack,Com_Sensor_Cal_Write_Curve_Packet_Len);
	
/* 传感器校准 写曲线 相应数据长度 *///数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+96(数据)+1(常数)+1(校验和)
#undef Com_Sensor_Cal_Write_Curve_Data_Len
/* 传感器校准 写曲线 数据帧长度 */
#undef Com_Sensor_Cal_Write_Curve_Packet_Len
}

/************************************************************************************************/
/**	函 数 名: Com_Sensor_Cal_Read_Curve_Data														*/
/**	功能说明: 读传感器对应通道曲线数据																*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Curve_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* 传感器真实值数据指针 */
	u16* sensor_act_curve_data_p;
	/* 传感器真实值数据指针 */
	u16* sensor_cal_curve_data_p;
	/* 发送 传感器读曲线 数据帧 寄存器 */
	COM_SENSOR_CAL_TYPEDEF read_curve_ack;
	
/* 传感器校准 读曲线 相应数据长度 *///数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+32(数据)+1(常数)+1(校验和)
#define Com_Sensor_Cal_Read_Curve_Data_Len		(Com_Sensor_Cal_Data_Buffer_Size)
/* 传感器校准 读曲线 数据帧长度 */
#define Com_Sensor_Cal_Read_Curve_Packet_Len	(Com_Sensor_Cal_Read_Curve_Data_Len+Com_Sensor_Cal_Fix_Data_Len)


	/* 判断获取的是 第几通道 的传感器数据曲线 */
	/* 获取对应通道的真实值和校准值 */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			/* 获取传感器通道1真实值数据曲线 */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value;
			/* 获取传感器通道1校准值数据曲线 */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value;
			break;
		}
#endif	
		
		default:
		{
			/* 获取的通道目标不存在 */
			__nop();
			return;
		}
	}
	
	/* 设置 响应数据帧 参数 */
	read_curve_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_curve_ack.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	read_curve_ack.Cmd_Code	 	= Com_Sensor_Cal_Cmd_Code_Read_Curve_Data;
	read_curve_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_curve_ack.Data_Len		= Com_Sensor_Cal_Read_Curve_Data_Len;

	/* 将数据拷贝到发送寄存器中 */
	/* 这里校准值放前面（16字节），真实值放后面（16字节） */
	memcpy(read_curve_ack.Com_Data,(u8*)sensor_cal_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	memcpy(&read_curve_ack.Com_Data[Com_Sensor_Cal_Curve_Data_Len],(u8*)sensor_act_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	/* 设置 常数 */
	read_curve_ack.Com_Data[read_curve_ack.Data_Len]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	read_curve_ack.Com_Data[read_curve_ack.Data_Len+1]	= Com_Sensor_Cal_Get_Check(&read_curve_ack);
	/* 响应 实时数据 数据信息 *///数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+16(数据)+1(常数)+1(校验和)
	USART_printf(USART1,(u8*)&read_curve_ack,Com_Sensor_Cal_Read_Curve_Packet_Len);
	

/* 传感器校准 读曲线 相应数据长度 *///数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+96(数据)+1(常数)+1(校验和)
#undef Com_Sensor_Cal_Read_Curve_Data_Len
/* 传感器校准 读曲线 数据帧长度 */
#undef Com_Sensor_Cal_Read_Curve_Packet_Len
}


/************************************************************************************************/
/**	函 数 名: Com_Config_SN_Write																*/
/**	功能说明: 写SN号																				*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void static Com_Config_SN_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* 写SN号 应答的数据长度 */
#define Com_Write_SN_NO_Data_Len	(1u) //最大1个传输数据
/* 写SN号 应答协议长度 */
#define Com_Write_SN_NO_Len			(Com_Write_SN_NO_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
	/* 写SN号 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF write_sn_no;
	

	/* 设置SN号 */
	Flash_Write_SN_NO(Cal_Inf->Com_Data);
	
	memcpy(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number,Cal_Inf->Com_Data,sizeof(SERIAL_NUMBER));
				/* 修改 仪器 信息表 设置标志 */
 	Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number,sizeof(SERIAL_NUMBER));
	
	

	/* 配置 实时数据 参数 */
	write_sn_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_sn_no.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	write_sn_no.Cmd_Code	= Com_Config_SN_Cmd_Code_Write;
	write_sn_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	write_sn_no.Data_Len	= Com_Write_SN_NO_Data_Len;
	/* 将数据拷贝到发送寄存器中 */
	write_sn_no.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* 设置 常数 */
	write_sn_no.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	write_sn_no.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_sn_no);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&write_sn_no,Com_Write_SN_NO_Len);
	
	
#undef Com_Write_SN_NO_Data_Len	
#undef Com_Read_SN_NO_Len
}

/************************************************************************************************/
/**	函 数 名: Com_Config_SN_Read																	*/
/**	功能说明: 读SN号																				*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void static Com_Config_SN_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* 读 SN 号应答协议长度 */
#define Com_Read_SN_NO_Len	(ENV_SN_NO_SIZE+Com_Sensor_Cal_Fix_Data_Len)//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+6(数据)+1(常数)+1(校验和)
	/* 读 SN 号应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF read_sn_no;
	

	/* 配置 实时数据 参数 */
	read_sn_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_sn_no.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	read_sn_no.Cmd_Code	 	= Com_Config_SN_Cmd_Code_Read;
	read_sn_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	read_sn_no.Data_Len		= ENV_SN_NO_SIZE;
////	/* 读取SN号 */
////	Flash_Read_SN_NO(read_sn_no.Com_Data);
	
	memcpy(read_sn_no.Com_Data,&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number,sizeof(SERIAL_NUMBER));
	
	
	
	/* 设置 常数 */
	read_sn_no.Com_Data[ENV_SN_NO_SIZE] 		= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	read_sn_no.Com_Data[ENV_SN_NO_SIZE + 1] 	= Com_Sensor_Cal_Get_Check(&read_sn_no);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&read_sn_no,Com_Read_SN_NO_Len);
#undef Com_Read_SN_NO_Len
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Sensor_Write															*/
/**	功能说明: 写传感器配置																			*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Sensor_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* 写传感器配置 数据长度 */
#define Com_Write_Sensor_Data_Len		(1u) //最大1个传输数据
/* 写传感器配置 数据帧长度 */
#define Com_Write_Sensor_Len			(Com_Write_Sensor_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
/* 数长度 */
#define Write_Sensor_Data_Len			(sizeof(INSTRU_SENSOR_Old))
/* 数长度 */
#define Write_Sensor_Para_Data_Len		(sizeof(INSTRU_SENSOR_CHANNEL_FIX))
/* 传感器通道指针 */
#define Sensor_Cha_P(cha)				(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[(cha-1)*sizeof(INSTRU_SENSOR_CHANNEL)])
	
	/* 写传感器配置寄存器 */
	COM_SENSOR_CAL_TYPEDEF write_sensor_config;
	/*  */
	INSTRU_SENSOR_CHANNEL_FIX	*sensor_para_p;
	/* 传感器通信数据寄存器. */
	INSTRU_SENSOR_Old			sensor_data;


	/* 获取对应通道的真实值和校准值 */
	switch(Cal_Inf->Chanel_Num-1)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			break;
		}
#endif	
		
		default:
		{
			/* 获取的通道目标不存在 */
			__nop();
			return;
		}
	}
	
	/* 清除缓存数据 */
	memset((u8*)&sensor_data,0x00,sizeof(INSTRU_SENSOR_Old));

	/* 将数据拷贝到发送寄存器中 */
	memcpy((u8*)&sensor_data,Cal_Inf->Com_Data,Write_Sensor_Data_Len);
	
	/* 将获得的传感器数据转移到配置表中 */
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_RF_Card_ID_NO = sensor_data.SENSOR_RF_Card_ID_NO;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[0] 	= sensor_data.SENSOR_SN_NO[0];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[1] 	= sensor_data.SENSOR_SN_NO[1];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[2] 	= sensor_data.SENSOR_SN_NO[2];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[3] 	= sensor_data.SENSOR_SN_NO[3];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Up_Limit 	= sensor_data.SENSOR_Up_Limit;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Low_Limit 	= sensor_data.SENSOR_Low_Limit;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->Type 				= sensor_data.Type;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_Switch 		= (sensor_data.SENSOR_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Sound_Alarm_Switch = (sensor_data.SENSOR_Sound_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Light_Alarm_Switch = (sensor_data.SENSOR_Light_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SMS_Alarm_Switch 	= (sensor_data.SENSOR_SMS_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_State =  0;
			
	/* 修改 仪器 信息表 设置标志 */
	Write_Config_Tab((u8*)Sensor_Cha_P(Cal_Inf->Chanel_Num),sizeof(INSTRU_SENSOR_CHANNEL));

	/* 设置传感器相关的参数指针 */
	sensor_para_p = (INSTRU_SENSOR_CHANNEL_FIX*)&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[(Cal_Inf->Chanel_Num-1)*Write_Sensor_Para_Data_Len];

	/* 清除参数指针 */
	sensor_para_p->Sensor_Up_Alarm_Cnt = 0;	
	sensor_para_p->Sensor_Low_Alarm_Cnt = 0;
	sensor_para_p->Sensor_Suspend_Alarm_Flag = FALSE;

	/* 配置 实时数据 参数 */
	write_sensor_config.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_sensor_config.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	write_sensor_config.Cmd_Code	= Com_Config_Sensor_Cmd_Code_Write;
	write_sensor_config.Chanel_Num	= Cal_Inf->Chanel_Num;
	write_sensor_config.Data_Len	= 1;
	/* 将数据拷贝到发送寄存器中 */
	write_sensor_config.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* 设置 常数 */
	write_sensor_config.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	write_sensor_config.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_sensor_config);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&write_sensor_config,Com_Write_Sensor_Len);
	
	
#undef Com_Write_Sensor_Data_Len
#undef Com_Write_Sensor_Len
#undef Write_Sensor_Data_Len
#undef Sensor_Cha_P
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Sensor_Read																*/
/**	功能说明: 读传感器配置																			*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Sensor_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* 传感器配置 数据长度 */
#define Com_Read_Sensor_Data_Len	(sizeof(INSTRU_SENSOR_Old))
/* 传感器配置 数据帧长度 */
#define Com_Read_Sensor_Len			  (Com_Read_Sensor_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
/* 传感器通道指针 */
#define Sensor_Cha_P(cha)			    (&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[(cha-1)*sizeof(INSTRU_SENSOR_CHANNEL)])

	
	/* 写传感器配置寄存器 */
	COM_SENSOR_CAL_TYPEDEF read_sensor_config;
	/* 传感器通信数据寄存器. */
	INSTRU_SENSOR_Old			sensor_data;


	/* 获取对应通道的真实值和校准值 */
	switch(Cal_Inf->Chanel_Num-1)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			break;
		}
#endif	
		
		default:
		{
			/* 获取的通道目标不存在 */
			__nop();
			return;
		}
	}
	
	/* 清除缓存数据 */
	memset((u8*)&sensor_data,0x00,sizeof(INSTRU_SENSOR_Old));
	
	/* 配置 实时数据 参数 */
	read_sensor_config.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_sensor_config.Instru_NO 	= Cal_Inf->Instru_NO;
	read_sensor_config.Cmd_Code	 	= Com_Config_Sensor_Cmd_Code_Read;
	read_sensor_config.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_sensor_config.Data_Len		= Com_Read_Sensor_Data_Len;
	
	/* 将获得的传感器数据转移到配置表中 */	
	sensor_data.SENSOR_RF_Card_ID_NO 	= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_RF_Card_ID_NO;
	sensor_data.SENSOR_SN_NO[0] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[0];
	sensor_data.SENSOR_SN_NO[1] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[1];
	sensor_data.SENSOR_SN_NO[2] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[2];
	sensor_data.SENSOR_SN_NO[3] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[3];
	sensor_data.SENSOR_Up_Limit 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Up_Limit;
	sensor_data.SENSOR_Low_Limit 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Low_Limit;
	sensor_data.Type 					= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->Type;
	sensor_data.SENSOR_Alarm_Switch 	= (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_Sound_Alarm_Switch = (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Sound_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_Light_Alarm_Switch = (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Light_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_SMS_Alarm_Switch   = (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SMS_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_Alarm_State 		  = ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_State;
	
	/* 将数据拷贝到发送寄存器中 */
	memcpy(read_sensor_config.Com_Data,(u8 *)&sensor_data,Com_Read_Sensor_Data_Len);
	/* 设置 常数 */
	read_sensor_config.Com_Data[Com_Read_Sensor_Data_Len]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	read_sensor_config.Com_Data[Com_Read_Sensor_Data_Len+1]	= Com_Sensor_Cal_Get_Check(&read_sensor_config);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&read_sensor_config,Com_Read_Sensor_Len);

#undef Com_Read_Sensor_Data_Len
#undef Com_Read_Sensor_Len
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Card_NO_Write															*/
/**	功能说明: 写打印车牌号																		*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF											*/
/**	返 回 值: 无																				*/
/************************************************************************************************/
static void Com_Config_Card_NO_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* 写打印车牌号 应答的数据长度 */
#define Com_Write_Card_NO_Data_Len	(1u) //最大1个传输数据
/* 写打印车牌号 应答协议长度 */
//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
#define Com_Write_Card_NO_Len		(Com_Write_Card_NO_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* 打印车牌号的数据地址 */
#define Card_NO_Data_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO)

	/* 写打印车牌号 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF write_card_no;
	

	/* 设置打印车牌号 */
	/* 清除数据区内容 */
	memset(Card_NO_Data_P,'\0',Print_Card_NO_Size);
	/* 将数据拷贝到发送寄存器中 */
	memcpy(Card_NO_Data_P,Cal_Inf->Com_Data,(Cal_Inf->Data_Len >(9u))? (9u):Cal_Inf->Data_Len);
	/* 保存 打印车牌号 参数配置 */
	Write_Config_Tab((u8*)Card_NO_Data_P,sizeof(Card_NO_Data_P));

	/* 配置 实时数据 参数 */
	write_card_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_card_no.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	write_card_no.Cmd_Code		= Com_Config_Card_NO_Cmd_Code_Write;
	write_card_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	write_card_no.Data_Len		= Com_Write_Card_NO_Data_Len;
	/* 将数据拷贝到发送寄存器中 */
	write_card_no.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* 设置 常数 */
	write_card_no.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	write_card_no.Com_Data[2] 	= Com_Sensor_Cal_Get_Check(&write_card_no);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&write_card_no,Com_Write_Card_NO_Len);
	
	
#undef Com_Write_Card_NO_Data_Len	
#undef Com_Write_Card_NO_Len
#undef Card_NO_Data_P
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Card_NO_Read																	*/
/**	功能说明: 读SN号																				*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Card_NO_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* 打印车牌号的数据地址 */
#define Card_NO_Data_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO)
	/* 读打印车牌号应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF read_card_no;
	

	/* 配置 实时数据 参数 */
	read_card_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_card_no.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	read_card_no.Cmd_Code	= Com_Config_Card_NO_Cmd_Code_Read;
	read_card_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	read_card_no.Data_Len	= strlen(Card_NO_Data_P);
	/* 将数据拷贝到发送寄存器中 */
	memcpy(read_card_no.Com_Data,Card_NO_Data_P,read_card_no.Data_Len);
 
	/* 设置 常数 */
	*((u8*)&(read_card_no.Com_Data[read_card_no.Data_Len])) = Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	*((u8*)&(read_card_no.Com_Data[read_card_no.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&read_card_no);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&read_card_no,(read_card_no.Data_Len + Com_Sensor_Cal_Fix_Data_Len));
	

#undef Card_NO_Data_P
}


/************************************************************************************************/
/**	函 数 名: Com_Config_Company_Name_Read														*/
/**	功能说明: 读配置 打印公司名称																*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF											*/
/**	返 回 值: 无																				*/
/************************************************************************************************/
static void Com_Config_Company_Name_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* 打印 公司名称 的数据地址 */
#define Card_Cpy_Name_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name)
	/* 读打印车牌号应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF read_cpy_name;
	

	/* 配置 实时数据 参数 */
	read_cpy_name.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_cpy_name.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	read_cpy_name.Cmd_Code		= Com_Config_Company_Name_Cmd_Code_Read;
	read_cpy_name.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	read_cpy_name.Data_Len		= strlen(Card_Cpy_Name_P);
	/* 将数据拷贝到发送寄存器中 */
	memcpy(read_cpy_name.Com_Data,Card_Cpy_Name_P,read_cpy_name.Data_Len);
	
	/* 设置 常数 */
	*((u8*)&(read_cpy_name.Com_Data[read_cpy_name.Data_Len])) = Com_Sensor_Cal_Constant;
	
	/* 设置 校验 */
	*((u8*)&(read_cpy_name.Com_Data[read_cpy_name.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&read_cpy_name);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&read_cpy_name,read_cpy_name.Data_Len + Com_Sensor_Cal_Fix_Data_Len);
	

#undef Card_Cpy_Name_P
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Company_Name_Write														*/
/**	功能说明: 写配置 打印公司名称																*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF											*/
/**	返 回 值: 无																				*/
/************************************************************************************************/
static void Com_Config_Company_Name_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* 写打公司名称 应答的数据长度 */
#define Com_Write_Cpy_Name_Data_Len	(1u) //最大1个传输数据
/* 写打公司名称 应答协议长度 */
//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
#define Com_Write_Cpy_Name_Len		(Com_Write_Cpy_Name_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* 打印公司名称的数据地址 */
#define Cpy_Name_Data_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name)

	
	/* 写打印公司名称 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF write_cpy_name;
	

	/* 设置打印公司名称 */
	/* 清除数据区内容 */
	memset(Cpy_Name_Data_P,'\0',Print_Company_Name_Size);
	/* 将数据拷贝到发送寄存器中 */
	memcpy(Cpy_Name_Data_P,Cal_Inf->Com_Data,(Cal_Inf->Data_Len > (Print_Company_Name_Size - 1)) ? (Print_Company_Name_Size - 1):Cal_Inf->Data_Len);
	/* 保存公司名称配置 */
	Write_Config_Tab((u8*)Cpy_Name_Data_P,sizeof(Cpy_Name_Data_P));

	/* 配置 实时数据 参数 */
	write_cpy_name.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_cpy_name.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	write_cpy_name.Cmd_Code		= Com_Config_Company_Name_Cmd_Code_Write;
	write_cpy_name.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	write_cpy_name.Data_Len		= Com_Write_Cpy_Name_Data_Len;
	/* 将数据拷贝到发送寄存器中 */
	write_cpy_name.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* 设置 常数 */
	write_cpy_name.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	write_cpy_name.Com_Data[2] 	= Com_Sensor_Cal_Get_Check(&write_cpy_name);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&write_cpy_name,Com_Write_Cpy_Name_Len);
	
	
#undef Com_Write_Cpy_Name_Data_Len
#undef Com_Write_Cpy_Name_Len
#undef Cpy_Name_Data_P
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Fad_Data_Write															*/
/**	功能说明: 写传感器通道作弊上下限值																*/
/**	形    参：传感器通道数据作弊消息体：COM_SENSOR_CAL_TYPEDEF										*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Fad_Data_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* 做作弊通道上限数据指针 */
#define Fad_Up_Lmt_Data_P(ch)			(&(((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Fraud_Up_Limit))
/* 做作弊通道下限数据指针 */
#define Fad_Low_Lmt_Data_P(ch)			(&(((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Fraud_Low_Limit))
/* 数据长度 */
#define Com_Write_Fad_Data_Len			(1u)
/* 应答协议长度 */
//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
#define Com_Write_Fad_Data_Func_Len		(Com_Write_Fad_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* 消息体数据长度 */
#define Com_Inf_Data_Len				(8u)
/* 传感器通道上限参数 */
#define Sen_Up_Lmt_Data(ch)				(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Up_Limit)
/* 传感器通道上限参数 */
#define Sen_Low_Lmt_Data(ch)			(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Low_Limit)


	/* 传感器通道数据作弊 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF write_fad_data_func;

	/* 数据合法性标志 */
	BOOL data_vldy = TRUE;
	/* 作弊上下限数据参数 */
	float* fad_up_lmt = (float*)Cal_Inf->Com_Data;
	float* fad_low_lmt = (float*)(&Cal_Inf->Com_Data[sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)))]);

	/* 判断设置的数据是否合法 */
	/* 判断通道是否合法 */
	/* 获取通道参数检测 */
	data_vldy = Log_Com_Sensor_CH_Check((Cal_Inf->Chanel_Num-1));
	
	/* 判断是否合法 */
	if((data_vldy == TRUE)&&(Cal_Inf->Data_Len != Com_Inf_Data_Len))
	{
		/* 数据长度不合法 */
		data_vldy = FALSE;
	}
	
	/* 判断参数合法性 */
	if(((data_vldy == TRUE)&&(*fad_up_lmt < *fad_low_lmt))||
	   (*fad_up_lmt > 1000)||(*fad_low_lmt < -1000))
	{
		/* 参数不合法 */
		data_vldy = FALSE;
		
	}else
	{
		/* 数据长度合法 */
		/* 作弊上限大于下限 */
		if((*fad_low_lmt > Sen_Up_Lmt_Data((Cal_Inf->Chanel_Num-1)))||(*fad_up_lmt < Sen_Low_Lmt_Data((Cal_Inf->Chanel_Num-1))))
		{
			/* 作弊上下限不在传感器上下限范围内 */
			/* 参数不合法 */
			data_vldy = FALSE;
		}
	}
		
	/* 判断数据合法性标志 */
	if(data_vldy == TRUE)
	{
		/* 参数合法 */
		/* 设置作弊传感器上下限参数 */
		/* 设置上限	*/
		memcpy(Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),Cal_Inf->Com_Data,sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));
		/* 设置下限 */
		memcpy(Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),&Cal_Inf->Com_Data[sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)))],sizeof(*Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));

		/* 保存参数 */
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));
		
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),sizeof(*Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));
	}


	/* 配置 实时数据 参数 */
	write_fad_data_func.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_fad_data_func.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	write_fad_data_func.Cmd_Code	= Com_Config_Fad_Data_Cmd_Code_Write;
	write_fad_data_func.Chanel_Num	= Cal_Inf->Chanel_Num;
	write_fad_data_func.Data_Len	= Com_Write_Fad_Data_Len;
	
	/* 将数据拷贝到发送寄存器中 */
	write_fad_data_func.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* 设置 常数 */
	write_fad_data_func.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	write_fad_data_func.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_fad_data_func);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&write_fad_data_func,Com_Write_Fad_Data_Func_Len);
	

#undef Fad_Up_Lmt_Data_P
#undef Fad_Low_Lmt_Data_P
#undef Com_Write_Fad_Data_Len
#undef Com_Write_Fad_Data_Func_Len
#undef Com_Inf_Data_Len
#undef Sen_Up_Lmt_Data
#undef Sen_Low_Lmt_Data
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Fad_Data_Read															*/
/**	功能说明: 读传感器通道作弊上下限值																*/
/**	形    参：传感器通道数据作弊消息体：COM_SENSOR_CAL_TYPEDEF										*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Fad_Data_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* 做作弊通道数据指针 */
#define Fad_Data_P(ch)			((char*)(&(((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Fraud_Up_Limit)))

/* 数据长度 */
#define Com_Read_Fad_Data_Len	(8u)

/* 消息体数据长度 */
#define Com_Inf_Data_Len		(0u)

/* 应答协议长度 */
//数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+8(数据长度)+1(数据)+1(常数)+1(校验和)
#define Com_Read_Fad_Len		(Com_Read_Fad_Data_Len+Com_Sensor_Cal_Fix_Data_Len)


	/* 传感器通道数据作弊 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF 		read_fad_data_func;

	/* 数据合法性标志 */
	BOOL data_vldy = TRUE;

	/* 判断设置的数据是否合法 */
	/* 判断通道是否合法 */
	/* 获取通道参数检测 */
	data_vldy = Log_Com_Sensor_CH_Check((Cal_Inf->Chanel_Num-1));

	/* 判断是否合法 */
	if((data_vldy == TRUE)&&(Cal_Inf->Data_Len != Com_Inf_Data_Len))
	{
		/* 数据长度不合法 */
		data_vldy = FALSE;
	}

	/* 配置 实时数据 参数 */
	read_fad_data_func.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_fad_data_func.Instru_NO 	= Cal_Inf->Instru_NO;//仪器编号
	read_fad_data_func.Cmd_Code		= Com_Config_Fad_Data_Cmd_Code_Read;
	read_fad_data_func.Chanel_Num	= Cal_Inf->Chanel_Num;
	
	if(data_vldy == TRUE)
	{
		/* 设置数据长度 */
		read_fad_data_func.Data_Len		= Com_Read_Fad_Data_Len;

		/* 将数据拷贝到发送寄存器中 */
		memcpy(read_fad_data_func.Com_Data,Fad_Data_P((Cal_Inf->Chanel_Num-1)),Com_Read_Fad_Data_Len);
		
		/* 设置 常数 */
		read_fad_data_func.Com_Data[Com_Read_Fad_Data_Len]		= Com_Sensor_Cal_Constant;
		
		/* 设置 校验 */
		read_fad_data_func.Com_Data[Com_Read_Fad_Data_Len+1]	= Com_Sensor_Cal_Get_Check(&read_fad_data_func);	
	}else
	{
		/* 设置数据长度 */
		read_fad_data_func.Data_Len		= 1;
		
		/* 设置 常数 */
		read_fad_data_func.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Fail;
		
		/* 设置 常数 */
		read_fad_data_func.Com_Data[1]	= Com_Sensor_Cal_Constant;
		
		/* 设置 校验 */
		read_fad_data_func.Com_Data[1]	= Com_Sensor_Cal_Get_Check(&read_fad_data_func);
	}

	
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&read_fad_data_func,Com_Read_Fad_Len);


#undef Fad_Data_P
#undef Com_Read_Fad_Data_Len
#undef Com_Inf_Data_Len
#undef Com_Read_Fad_Len
}


/************************************************************************************************/
/**	函 数 名: Com_Config_DataNetwork_Read														*/
/**	功能说明: 读数据服务器的配置参数																*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_DataNetwork_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
	/* 网络配置指针 */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;
	/* 域名字符串长度 */
	u32	domainNameLen;
	
	
	/* 设置局部变量初始值 */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* 计算域名字符串长度 */
	domainNameLen = strlen((char*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1);
	/* 判断域名的数据长度 */
	if(domainNameLen > 31u)
	{
		/* 大于31，则设置为最大31 */
		domainNameLen = 31u;
	}
	
	/* 配置数据服务器参数 */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= ComConfigDataNetworkRead;
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 38u;

	/* 设置发送数据数据 */
	/* 设置IP */
	networkDatap->IP.Server_Address = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address;
	/* 设置端口 */
	networkDatap->Port = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1;
	/* 设置域名 */
	memcpy(networkDatap->DomainName,
		   Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
		   domainNameLen);
		   
	/* 设置 常数 */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* 设置 校验 */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}



/************************************************************************************************/
/**	函 数 名: Com_Config_DataNetwork_Read														*/
/**	功能说明: 读配置服务器的配置参数																*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Network_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
	/* 网络配置指针 */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;
	/* 域名字符串长度 */
	u32	domainNameLen;
	
	
	/* 设置局部变量初始值 */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* 计算域名字符串长度 */
	domainNameLen = strlen((char*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2);
	/* 判断域名的数据长度 */
	if(domainNameLen > 31u)
	{
		/* 大于31，则设置为最大31 */
		domainNameLen = 31u;
	}
	
	/* 配置数据服务器参数 */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= ComConfigNetworkRead;//读配置服务器操作码
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 38u;

	/* 设置发送数据数据 */
	/* 设置IP */
	networkDatap->IP.Server_Address = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Server_Address;
	/* 设置端口 */
	networkDatap->Port = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2;
	/* 设置域名 */
	memcpy(networkDatap->DomainName,
		   Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
		   domainNameLen);
		   
	/* 设置 常数 */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* 设置 校验 */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}
/************************************************************************************************/
/**	函 数 名: Com_Config_Upgrade_Network_Read											*/
/**	功能说明: 读升级服务器的配置参数																*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Upgrade_Network_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
	/* 网络配置指针 */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;
	/* 域名字符串长度 */
	u32	domainNameLen;
	
	
	/* 设置局部变量初始值 */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* 计算域名字符串长度 */
	domainNameLen = strlen((char*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3);
	/* 判断域名的数据长度 */
	if(domainNameLen > 31u)
	{
		/* 大于31，则设置为最大31 */
		domainNameLen = 31u;
	}
	
	/* 配置数据服务器参数 */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= ComConfigUpgradeNetworkRead;//读升级服务器操作码
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 38u;

	/* 设置发送数据数据 */
	/* 设置IP */
	networkDatap->IP.Server_Address = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Server_Address;
	/* 设置端口 */
	networkDatap->Port = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3;
	/* 设置域名 */
	memcpy(networkDatap->DomainName,
		   Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
		   domainNameLen);
		   
	/* 设置 常数 */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* 设置 校验 */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}

/************************************************************************************************/
/**	函 数 名: Com_Config_DataNetwork_Write														*/
/**	功能说明: 写数据服务器的配置参数																*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_DataNetwork_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* 网络配置指针 */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)Cal_Inf->Com_Data;
	/* 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	/* 数据合法性标志 */
	BOOL data_vldy = TRUE;
	/* 域名字符串长度 */
	u32	domainNameLen;
	
	/* 合法性检测 */
	if(data_vldy == TRUE)
	{
		/* 检测域名数据长度 */
		domainNameLen = strlen((char*)networkDatap->DomainName);
		
		/* 判断余名长度是否合理 */
		if(domainNameLen > 31u)
		{
			/* 域名数据长度不合理 */
			data_vldy = FALSE;
		}
	}
	
	
	/* 判断数据合法性标志 */
	if(data_vldy == TRUE)
	{
		/* 获取网络数据 */
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1 = networkDatap->IP;
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1 = networkDatap->Port;
		/* 设置清除域名寄存器 */
		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,'\0',32);
		memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,networkDatap->DomainName,domainNameLen);

		/* 修改仪器配置表中对应IP的数据 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address));
		/* 修改仪器配置表中对应端口的数据 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1));
		/* 修改仪器配置表中对应域名的数据 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));	
	}
	
	/* 配置 实时数据 参数 */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	= ComConfigDataNetworkWrite;
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	= (1u);
	
	/* 将数据拷贝到发送寄存器中 */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* 设置 常数 */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}



/************************************************************************************************/
/**	函 数 名: Com_Config_DataNetwork_Write														*/
/**	功能说明: 写配置服务器的配置参数																*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Network_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* 网络配置指针 */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)Cal_Inf->Com_Data;
	/* 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	/* 数据合法性标志 */
	BOOL data_vldy = TRUE;
	/* 域名字符串长度 */
	u32	domainNameLen;
	
	/* 合法性检测 */
	if(data_vldy == TRUE)
	{
		/* 检测域名数据长度 */
		domainNameLen = strlen((char*)networkDatap->DomainName);
		
		/* 判断余名长度是否合理 */
		if(domainNameLen > 31u)
		{
			/* 域名数据长度不合理 */
			data_vldy = FALSE;
		}
	}
	
	
	/* 判断数据合法性标志 */
	if(data_vldy == TRUE)
	{
		/* 获取网络数据 */
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2 = networkDatap->IP;
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2 = networkDatap->Port;
		/* 设置清除域名寄存器 */
		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,'\0',32);
		memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,networkDatap->DomainName,domainNameLen);

		/* 修改仪器配置表中对应IP的数据 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Server_Address,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Server_Address));
		/* 修改仪器配置表中对应端口的数据 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2));
		/* 修改仪器配置表中对应域名的数据 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));	
	}
	
	/* 配置 实时数据 参数 */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	= ComConfigNetworkWrite;//配置服务器操作码
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	= (1u);
	
	/* 将数据拷贝到发送寄存器中 */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* 设置 常数 */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}


/************************************************************************************************/
/**	函 数 名: Com_Config_Upgrade_Network_Write												*/
/**	功能说明: 写升级服务器的配置参数																*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Upgrade_Network_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* 网络配置指针 */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)Cal_Inf->Com_Data;
	/* 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	/* 数据合法性标志 */
	BOOL data_vldy = TRUE;
	/* 域名字符串长度 */
	u32	domainNameLen;
	
	/* 合法性检测 */
	if(data_vldy == TRUE)
	{
		/* 检测域名数据长度 */
		domainNameLen = strlen((char*)networkDatap->DomainName);
		
		/* 判断余名长度是否合理 */
		if(domainNameLen > 31u)
		{
			/* 域名数据长度不合理 */
			data_vldy = FALSE;
		}
	}
	
	
	/* 判断数据合法性标志 */
	if(data_vldy == TRUE)
	{
		/* 获取网络数据 */
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3 = networkDatap->IP;
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3 = networkDatap->Port;
		/* 设置清除域名寄存器 */
		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,'\0',32);
		memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,networkDatap->DomainName,domainNameLen);

		/* 修改仪器配置表中对应IP的数据 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Server_Address,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Server_Address));
		/* 修改仪器配置表中对应端口的数据 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3));
		/* 修改仪器配置表中对应域名的数据 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));	
	}
	
	/* 配置 实时数据 参数 */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	= ComConfigUpgradeNetworkWrite;//写升级服务器操作码
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	= (1u);
	
	/* 将数据拷贝到发送寄存器中 */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* 设置 常数 */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* 设置 校验 */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* 响应 实时数据 数据信息 */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}












































/************************************************************************************************/
/**	函 数 名: Com_Config_SMSAamPoeNOSet												*/
/**	功能说明: 设置短信报警号码															*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
///************************************************************************************************/
//static void Com_Config_SMSAamPoeNOSet (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
/* 短信报警号码数据指针. */
#define SMS_Aam_Poe_NO_P_1(num) (((MOBILE_NO*)(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1))+num)
static void Com_Config_SMSAamPoeNOSet (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	/* 写报警手机号 应答的数据长度 */
  #define Com_Write_SMSAamPoeNOSet_Data_Len	(1u) //最大1个传输数据
	
	#define Com_Write_SMSAamPoeNOSet_Len	(Com_Write_SMSAamPoeNOSet_Data_Len+Com_Config_SMSAamPoeNO_Fix_Data_Len) //数据长度：[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+1(数据)+1(常数)+1(校验和)
	
	
	   MOBILE_NO *sms_no_p;//手机 号码 结构体
		/* 写报警手机号 应答协议寄存器 */
	COM_SENSOR_CAL_TYPEDEF      write_SMSAamPoeNOSet;//接收串口数据
/**********************处理上位机指令*****************************************/
	sms_no_p = SMS_Aam_Poe_NO_P_1(Cal_Inf->Chanel_Num-1);//设置手机号码位置地址，参数0:代表号码一,参数1:代表号码 二,参数2:代表号码三;
	if(Cal_Inf->Data_Len==Check_SMSAamPhone_Len)
	{/* 将设置的短信号码写入数据寄存器 */
		sms_no_p->Serial_Number_Number.Number_1  = (Cal_Inf->Com_Data[0]-0x30);
		sms_no_p->Serial_Number_Number.Number_2  = (Cal_Inf->Com_Data[1]-0x30);
		sms_no_p->Serial_Number_Number.Number_3  = (Cal_Inf->Com_Data[2]-0x30);
		sms_no_p->Serial_Number_Number.Number_4  = (Cal_Inf->Com_Data[3]-0x30);
		sms_no_p->Serial_Number_Number.Number_5  = (Cal_Inf->Com_Data[4]-0x30);
		sms_no_p->Serial_Number_Number.Number_6  = (Cal_Inf->Com_Data[5]-0x30);
		sms_no_p->Serial_Number_Number.Number_7  = (Cal_Inf->Com_Data[6]-0x30);
		sms_no_p->Serial_Number_Number.Number_8  = (Cal_Inf->Com_Data[7]-0x30);
		sms_no_p->Serial_Number_Number.Number_9  = (Cal_Inf->Com_Data[8]-0x30);
		sms_no_p->Serial_Number_Number.Number_10 = (Cal_Inf->Com_Data[9]-0x30);
		sms_no_p->Serial_Number_Number.Number_11 = (Cal_Inf->Com_Data[10]-0x30);
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)sms_no_p,sizeof(MOBILE_NO));
/***********************响应上位机指令****************************************/
   /* 配置 实时数据 参数 */
    write_SMSAamPoeNOSet.Head_Code =Com_Sensor_Cal_Rece_Head_Code;//头码
    write_SMSAamPoeNOSet.Instru_NO =Cal_Inf->Instru_NO;//仪器编号
    write_SMSAamPoeNOSet.Cmd_Code  =Com_Config_SMSAamPoeNO_Cmd_Code_Write;//写报警手机号码操作码
		write_SMSAamPoeNOSet.Chanel_Num=Cal_Inf->Chanel_Num;//对应的号码序号
		write_SMSAamPoeNOSet.Data_Len  =0x01;
		write_SMSAamPoeNOSet.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/
		write_SMSAamPoeNOSet.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	  write_SMSAamPoeNOSet.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_SMSAamPoeNOSet);/* 设置 校验 */
   /* 响应 实时数据 数据信息 */
  	USART_printf(USART1,(u8*)&write_SMSAamPoeNOSet,Com_Write_SMSAamPoeNOSet_Len);
	}
	else
	{
		
	/***********************响应上位机指令****************************************/
   /* 配置 实时数据 参数 */
    write_SMSAamPoeNOSet.Head_Code =Com_Sensor_Cal_Rece_Head_Code;//头码
    write_SMSAamPoeNOSet.Instru_NO =Cal_Inf->Instru_NO;//仪器编号
    write_SMSAamPoeNOSet.Cmd_Code  =Com_Config_SMSAamPoeNO_Cmd_Code_Write;//写报警手机号码操作码
		write_SMSAamPoeNOSet.Chanel_Num=Cal_Inf->Chanel_Num;//对应的号码序号
		write_SMSAamPoeNOSet.Data_Len  =0x01;
		write_SMSAamPoeNOSet.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*操作失败标志*/
		write_SMSAamPoeNOSet.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	  write_SMSAamPoeNOSet.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_SMSAamPoeNOSet);/* 设置 校验 */
   /* 响应 实时数据 数据信息 */
  	USART_printf(USART1,(u8*)&write_SMSAamPoeNOSet,Com_Write_SMSAamPoeNOSet_Len);
	
	}

}
/************************************************************************************************/
/**	函 数 名: Com_Config_SMSAamPoeNORead												*/
/**	功能说明:读取短信报警号码															*/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
///************************************************************************************************/
static void Com_Config_SMSAamPoeNORead (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	
       MOBILE_NO *sms_no_p;//手机 号码 结构体 
       COM_SENSOR_CAL_TYPEDEF      read_SMSAamPoeNOSet;//发送串口数据(包含手机号信息)
/***********************响应上位机指令****************************************/
     /* 配置 实时数据 参数 */
sms_no_p = SMS_Aam_Poe_NO_P_1(Cal_Inf->Chanel_Num-1);//定位到要发送手机号码位置地址，参数0:代表号码一,参数1:代表号码 二,参数2:代表号码三;	

	    read_SMSAamPoeNOSet.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	    read_SMSAamPoeNOSet.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	    read_SMSAamPoeNOSet.Cmd_Code =Com_Config_SMSAamPoeNO_Cmd_Code_Read;//读报警手机号码操作码
			read_SMSAamPoeNOSet.Chanel_Num=Cal_Inf->Chanel_Num;//对应的号码序号
	    read_SMSAamPoeNOSet.Data_Len=Check_SMSAamPhone_Len_Read;//手机号码长度;默认为11位
	                                     
	
      read_SMSAamPoeNOSet.Com_Data[0]=(sms_no_p->Serial_Number_Number.Number_1+0x30);
	    read_SMSAamPoeNOSet.Com_Data[1]=(sms_no_p->Serial_Number_Number.Number_2+0x30);
      read_SMSAamPoeNOSet.Com_Data[2]=(sms_no_p->Serial_Number_Number.Number_3+0x30);
	    read_SMSAamPoeNOSet.Com_Data[3]=(sms_no_p->Serial_Number_Number.Number_4+0x30);
	    read_SMSAamPoeNOSet.Com_Data[4]=(sms_no_p->Serial_Number_Number.Number_5+0x30);
	    read_SMSAamPoeNOSet.Com_Data[5]=(sms_no_p->Serial_Number_Number.Number_6+0x30);
      read_SMSAamPoeNOSet.Com_Data[6]=(sms_no_p->Serial_Number_Number.Number_7+0x30);
	    read_SMSAamPoeNOSet.Com_Data[7]=(sms_no_p->Serial_Number_Number.Number_8+0x30);
	    read_SMSAamPoeNOSet.Com_Data[8]=(sms_no_p->Serial_Number_Number.Number_9+0x30);
      read_SMSAamPoeNOSet.Com_Data[9]=(sms_no_p->Serial_Number_Number.Number_10+0x30);
	    read_SMSAamPoeNOSet.Com_Data[10]=(sms_no_p->Serial_Number_Number.Number_11+0x30);
	    read_SMSAamPoeNOSet.Com_Data[11]=0;
	    read_SMSAamPoeNOSet.Com_Data[12]=0;
	    read_SMSAamPoeNOSet.Com_Data[13]=0;
	    read_SMSAamPoeNOSet.Com_Data[14]=0;
			read_SMSAamPoeNOSet.Com_Data[15]=Com_Sensor_Cal_Constant;/* 设置 常数 */;
			read_SMSAamPoeNOSet.Com_Data[16]=Com_Sensor_Cal_Get_Check(&read_SMSAamPoeNOSet);/* 设置 校验 */;

	  /* 响应 实时数据 数据信息 */
  	USART_printf(USART1,(u8*)&read_SMSAamPoeNOSet,22);
	                                               
}
/************************************************************************************************/
/**	函 数 名: Com_Config_Time_Interval_Set											*/
/**	功能说明:设置时间间隔（上传时间间隔、历史记录时间间隔、打印时间间隔、 报警记录间隔）****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
/* 非超标状态打印时间间隔数据指针 */
#define Pit_Time_Irl_Data_P_1  (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
/* " 正常存储间隔"数据指针 */
#define Menu_Nml_Save_Irl_Data_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval)
/* "报警存储间隔"数据指针 */
#define Menu_Aam_Save_Irl_Data_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval)
static void Com_Config_Time_Interval_Set (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	
		COM_SENSOR_CAL_TYPEDEF      write_Time_Interval_Set;//接收串口数据
     /* 设置的时间参数 */
     u32 normalsavetime;//打印间隔
	 //  u32 uploadtime;//上传间隔间隔
	   u32 alarm_intervaltime;//报警记录间隔
	   u32 normalsavetHHH;//历史记录间隔
    /* 获取设置的时间值 */
    normalsavetime =((Cal_Inf->Com_Data[5]<<8)+Cal_Inf->Com_Data[4])*60;//打印间隔
	//  uploadtime     =((Cal_Inf->Com_Data[1]<<8)+Cal_Inf->Com_Data[0])*60;//上传间隔
    alarm_intervaltime =((Cal_Inf->Com_Data[7]<<8)+Cal_Inf->Com_Data[6])*60;//报警记录间隔
  	normalsavetHHH=((Cal_Inf->Com_Data[3]<<8)+Cal_Inf->Com_Data[2])*60;//历史记录间隔
	
    	/* 判断设置的时间值是否在可以设置的范围内 */
	if( ((normalsavetime == 0)||(normalsavetime > Com_Config_Time_Interval_Set_Max_Second_Num))||
		  ((normalsavetHHH == 0)||(normalsavetHHH > Com_Config_Time_Interval_Set_Max_Second_Num))||
	    ((alarm_intervaltime == 0)||(alarm_intervaltime > Com_Config_Time_Interval_Set_Max_Second_Num))
	)//设置时间间隔失败
	
	{
		/***********************响应上位机指令****************************************/
	/* 配置 实时数据 参数 */
	write_Time_Interval_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
  write_Time_Interval_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	write_Time_Interval_Set.Cmd_Code =Com_Config_Time_Interval_Cmd_Code_Write;//写报 时间间隔操作码	
	write_Time_Interval_Set.Chanel_Num=0xFF;	
	write_Time_Interval_Set.Data_Len=0X00;
	write_Time_Interval_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*操作失败标志*/
	write_Time_Interval_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	write_Time_Interval_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_Time_Interval_Set);/* 设置 校验 */	
	 /* 响应 实时数据 数据信息 */
  USART_printf(USART1,(u8*)&write_Time_Interval_Set,8);
	//	return;
	}
	else//设置时间间隔成功
	{
/***********设置打印时间间隔***************************************/		
		//时间的输入方式应该更佳人性化点比较好。
	*Pit_Time_Irl_Data_P_1	= normalsavetime;	
	/* 修改 仪器 信息表 设置标志 */
	 Write_Config_Tab((u8*)Pit_Time_Irl_Data_P_1,sizeof(*Pit_Time_Irl_Data_P_1));
/******************************************************************/
///***********设置上传时间间隔***************************************/	
//			//时间的输入方式应该更佳人性化点比较好。
//		*Menu_Nml_Save_Irl_Data_A	= uploadtime;
/***********设置历史记录时间间隔***************************************/	
			//时间的输入方式应该更佳人性化点比较好。
		*Menu_Nml_Save_Irl_Data_A	= normalsavetHHH;		
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)Menu_Nml_Save_Irl_Data_A,sizeof(*Menu_Nml_Save_Irl_Data_A));
/*******************************************************************/
/*************设置报警记录间隔************************************/
		/* 设置报警状态的报警时间间隔 */
		*Menu_Aam_Save_Irl_Data_A	= alarm_intervaltime;
		
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)Menu_Aam_Save_Irl_Data_A,sizeof(*Menu_Aam_Save_Irl_Data_A));
			
/***********************响应上位机指令****************************************/
	/* 配置 实时数据 参数 */
	write_Time_Interval_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
  write_Time_Interval_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	write_Time_Interval_Set.Cmd_Code =Com_Config_Time_Interval_Cmd_Code_Write;//写报 时间间隔操作码	
	write_Time_Interval_Set.Chanel_Num=0xFF;	
	write_Time_Interval_Set.Data_Len=0x01;
	write_Time_Interval_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	write_Time_Interval_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	write_Time_Interval_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_Time_Interval_Set);/* 设置 校验 */	
	 /* 响应 实时数据 数据信息 */
  USART_printf(USART1,(u8*)&write_Time_Interval_Set,8);	
	}

}



/************************************************************************************************/
/**	函 数 名: Com_Config_Time_Interval_Read											*/
/**	功能说明:读取时间间隔（上传时间间隔、历史记录时间间隔、打印时间间隔、 报警记录间隔）****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
//u32 mmn=0;
static void Com_Config_Time_Interval_Read	(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
     COM_SENSOR_CAL_TYPEDEF      read_Time_Interval_Set;//接收串口数据
	
	       /* 设置的时间参数 */
     u32 normalsavetime;//打印间隔
	 //  u32 uploadtime;//上传间隔间隔
	   u32 alarm_intervaltime;//报警记录间隔
	   u32 normalsavetHHH;//历史记录间隔
	
	   normalsavetHHH=(*Menu_Nml_Save_Irl_Data_A)/60;//历史记录间隔
	   alarm_intervaltime=(*Pit_Time_Irl_Data_P_1)/60;//报警记录间隔
	   normalsavetime=(*Menu_Aam_Save_Irl_Data_A)/60;//打印间隔
	
	
	   read_Time_Interval_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Time_Interval_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Time_Interval_Set.Cmd_Code =Com_Config_Time_Interval_Cmd_Code_Read;//读 时间间隔 操作码
	   read_Time_Interval_Set.Chanel_Num=0xFF;
	   read_Time_Interval_Set.Data_Len=0X08;
	   read_Time_Interval_Set.Com_Data[0]=0x78;
	   read_Time_Interval_Set.Com_Data[1]=0x00;
	   read_Time_Interval_Set.Com_Data[2]=(normalsavetHHH&0xFF);/**/
	   read_Time_Interval_Set.Com_Data[3]=(normalsavetHHH>>8);
	   read_Time_Interval_Set.Com_Data[4]=(alarm_intervaltime&0xFF);/**/ 
	   read_Time_Interval_Set.Com_Data[5]=(alarm_intervaltime>>8);
	   read_Time_Interval_Set.Com_Data[6]=(normalsavetime&0xFF);/**/;
	   read_Time_Interval_Set.Com_Data[7]=(normalsavetime>>8);;
	   read_Time_Interval_Set.Com_Data[8]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Time_Interval_Set.Com_Data[9]=Com_Sensor_Cal_Get_Check(&read_Time_Interval_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Time_Interval_Set,15);	
//     mmn=*Pit_Time_Irl_Data_P_1;//获取打印时间间隔数据



}

/************************************************************************************************/
/**	函 数 名: Com_Config_Alarm_time_delay_Write										*/
/**	功能说明:写报警延时间隔****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
/* 通道报警延时数据指针. */
//u32 bbk=0;
#define Cha_Aam_Dly_Data_A(cha)		(&((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[cha*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Alarm_Delay)
static void Com_Config_Alarm_time_delay_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{

     	/* 设置的时间参数 */
	     u32 alarmsavetime;
	  	/* 计数变量 */
	     u8 num;
	   COM_SENSOR_CAL_TYPEDEF    Alarm_time_delay_Set;//接收串口数据
	
	   alarmsavetime =(Cal_Inf->Com_Data[1]<<8)+Cal_Inf->Com_Data[0];//将串口接收的两个数据合成一个整形数据即为报警延时间隔时间
//	   bbk=alarmsavetime;
	   if((alarmsavetime==0)||(alarmsavetime>Com_Config_Alarm_time_delay_SetMaxSecond))//超过最大的报警时间间隔3600秒（1个小时�
		 {
		 
			 	/***********************响应上位机指令****************************************/
	     /* 配置 实时数据 参数 */
	    Alarm_time_delay_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
      Alarm_time_delay_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	    Alarm_time_delay_Set.Cmd_Code =Com_Config_Alarm_time_delay_Cmd_Code_Write;//写报警延时间隔操作码	
	    Alarm_time_delay_Set.Chanel_Num=0xFF;	
	    Alarm_time_delay_Set.Data_Len=0X02;
	    Alarm_time_delay_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*操作失败标志*/
	    Alarm_time_delay_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	    Alarm_time_delay_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Alarm_time_delay_Set);/* 设置 校验 */	
	     /* 响应 实时数据 数据信息 */
      USART_printf(USART1,(u8*)&Alarm_time_delay_Set,8);
			
			 
		 }
		 else//执行设置报警延时间隔操作
		 {
		 
		       /* 设置 所有通道的 报警延时参数 */
		    for(num = 0; num < Sensor_CH_Config_Num; num++)
		  {
			  /* 设置正常状态的报警时间间隔 */
			   *Cha_Aam_Dly_Data_A(num) = alarmsavetime;
			  /* 修改 仪器 信息表 设置标志 */
		   	Write_Config_Tab((u8*)Cha_Aam_Dly_Data_A(num),sizeof(*Cha_Aam_Dly_Data_A(num)));
				
			}
				
			/***********************响应上位机指令****************************************/
	/* 配置 实时数据 参数 */
	Alarm_time_delay_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
  Alarm_time_delay_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	Alarm_time_delay_Set.Cmd_Code =Com_Config_Alarm_time_delay_Cmd_Code_Write;//写报警延时间隔操作码	
	Alarm_time_delay_Set.Chanel_Num=0xFF;	
	Alarm_time_delay_Set.Data_Len=0x02;
	Alarm_time_delay_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	Alarm_time_delay_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	Alarm_time_delay_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Alarm_time_delay_Set);/* 设置 校验 */			
	 /* 响应 实时数据 数据信息 */
  USART_printf(USART1,(u8*)&Alarm_time_delay_Set,8);						
	}
}
/************************************************************************************************/
/**	函 数 名: Com_Config_Alarm_time_delay_Read									*/
/**	功能说明:读报警延时间隔****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Alarm_time_delay_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	
   COM_SENSOR_CAL_TYPEDEF      read_Alarm_time_delay_Set;//接收串口数据


	   read_Alarm_time_delay_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Alarm_time_delay_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Alarm_time_delay_Set.Cmd_Code =Com_Config_Alarm_time_delay_Cmd_Code_Read;//读 延时报警间隔 操作码
	   read_Alarm_time_delay_Set.Chanel_Num=0xFF;
	   read_Alarm_time_delay_Set.Data_Len=0X02;
	   read_Alarm_time_delay_Set.Com_Data[1]=((*Cha_Aam_Dly_Data_A(Sensors_CH_1))>>8);         
	   read_Alarm_time_delay_Set.Com_Data[0]=((*Cha_Aam_Dly_Data_A(Sensors_CH_1))&0xFF); 
	   read_Alarm_time_delay_Set.Com_Data[2]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Alarm_time_delay_Set.Com_Data[3]=Com_Sensor_Cal_Get_Check(&read_Alarm_time_delay_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Alarm_time_delay_Set,9);	
}

/* 报警总开关 "声" 状态指针 */
#define Sud_Aam_Mtr_Sth_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)
/* 报警总开关 "光" 状态指针 */
#define Lgt_Aam_Mtr_Sth_A	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch)
/************************************************************************************************/
/**	函 数 名: Com_Config_Sound_lightSwitch_Read									*/
/**	功能说明:读取声光报警总开关状态****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Sound_lightSwitch_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Sound_lightSwitch_Set;//接收串口数据
	    
	
	   read_Sound_lightSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Sound_lightSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Sound_lightSwitch_Set.Cmd_Code =Com_ConfigTotal_audible_light_alarm_Cmd_Code_Read;//读 声光报警总开关 操作码
	   read_Sound_lightSwitch_Set.Chanel_Num=0xFF;
	   read_Sound_lightSwitch_Set.Data_Len=0X01;      
	   read_Sound_lightSwitch_Set.Com_Data[0]=((*Sud_Aam_Mtr_Sth_A)&&(*Lgt_Aam_Mtr_Sth_A)); 
	   read_Sound_lightSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Sound_lightSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Sound_lightSwitch_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Sound_lightSwitch_Set,8);	
	
     




}
/************************************************************************************************/
/**	函 数 名: Com_Config_Sound_lightSwitch_Write									*/
/**	功能说明:写声光报警总开关状态****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Sound_lightSwitch_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	      COM_SENSOR_CAL_TYPEDEF      read_Sound_lightSwitch_Set;//接收串口数据
	
          if((Cal_Inf->Com_Data[0]==Switch_ON)||(Cal_Inf->Com_Data[0]==Switch_OFF))
					{
					   	/* 声光报警总开关状态为:关,开启 通道 声光报警. */
				        *Sud_Aam_Mtr_Sth_A =(BOOL)(Cal_Inf->Com_Data[0]);
				        *Lgt_Aam_Mtr_Sth_A =(BOOL)(Cal_Inf->Com_Data[0]);
				      /* 修改 仪器 信息表 设置标志 */
				    Write_Config_Tab((u8*)Sud_Aam_Mtr_Sth_A,sizeof(*Sud_Aam_Mtr_Sth_A));
				    Write_Config_Tab((u8*)Lgt_Aam_Mtr_Sth_A,sizeof(*Lgt_Aam_Mtr_Sth_A));
					
	/***********************响应上位机指令****************************************/
	     /* 配置 实时数据 参数 */
	    read_Sound_lightSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
      read_Sound_lightSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	    read_Sound_lightSwitch_Set.Cmd_Code =Com_ConfigTotal_audible_light_alarm_Cmd_Code_Write;//写声光报警总开关操作码	
	    read_Sound_lightSwitch_Set.Chanel_Num=0xFF;	
	    read_Sound_lightSwitch_Set.Data_Len=0x01;
	    read_Sound_lightSwitch_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	    read_Sound_lightSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	    read_Sound_lightSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Sound_lightSwitch_Set);/* 设置 校验 */			
	    /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Sound_lightSwitch_Set,8);
								
					}





}
/************************************************************************************************/
/**	函 数 名: Com_Config_SMSalarmSwitch_Read									*/
/**	功能说明:读取短信报警总开关状态****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
/* "短信报警"总开关状态指针 */
#define SMS_Aam_Mtr_Sth_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch)
static void Com_Config_SMSalarmSwitch_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_SMSalarmSwitch_Set;//接收串口数据 

     read_SMSalarmSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_SMSalarmSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_SMSalarmSwitch_Set.Cmd_Code =Com_Config_SMS_alarm_total_switch_Cmd_Code_Read;//读 短信报警总开关 操作码
	   read_SMSalarmSwitch_Set.Chanel_Num=0xFF;
	   read_SMSalarmSwitch_Set.Data_Len=0X01;      
	   read_SMSalarmSwitch_Set.Com_Data[0]=(*SMS_Aam_Mtr_Sth_A); 
	   read_SMSalarmSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_SMSalarmSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_SMSalarmSwitch_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_SMSalarmSwitch_Set,8);	
}
/************************************************************************************************/
/**	函 数 名: Com_Config_SMSalarmSwitch_Write									*/
/**	功能说明:写短信报警总开关状态****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_SMSalarmSwitch_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
     COM_SENSOR_CAL_TYPEDEF      Write_SMSalarmSwitch_Set;//接收串口数据

     if((Cal_Inf->Com_Data[0]==Switch_ON)||(Cal_Inf->Com_Data[0]==Switch_OFF))
					{
					   	/* 短信报警总开关状态为:关,开启*/
				        *SMS_Aam_Mtr_Sth_A =(BOOL)(Cal_Inf->Com_Data[0]);
				    /* 修改 仪器 信息表 设置标志 */
				     Write_Config_Tab((u8*)SMS_Aam_Mtr_Sth_A,sizeof(*SMS_Aam_Mtr_Sth_A));
	/***********************响应上位机指令****************************************/
	     /* 配置 实时数据 参数 */
	    Write_SMSalarmSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
      Write_SMSalarmSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	    Write_SMSalarmSwitch_Set.Cmd_Code =Com_Config_SMS_alarm_total_switch_Cmd_Code_Write;//写短信报警总开关操作码	
	    Write_SMSalarmSwitch_Set.Chanel_Num=0xFF;	
	    Write_SMSalarmSwitch_Set.Data_Len=0x01;
	    Write_SMSalarmSwitch_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	    Write_SMSalarmSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	    Write_SMSalarmSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Write_SMSalarmSwitch_Set);/* 设置 校验 */			
	    /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&Write_SMSalarmSwitch_Set,8);
								
					}

}

/************************************************************************************************/
/**	函 数 名: Com_Config_SystemTime_Write									*/
/**	功能说明:通过上位机设置系统时间****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
/* 系统时钟数据寄存器 */
static Times System_Cok_DR;
extern BOOL RTC_Init(BOOL reset_state);
extern void Write_System_Set(Times* time);
static void Com_Config_SystemTime_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	  COM_SENSOR_CAL_TYPEDEF      read_GSM_SystemTime_Set;//接收串口数据 
   // COM_SENSOR_CAL_TYPEDEF      Write_SystemTime_Set;//接收串口数据

          if((Cal_Inf->Chanel_Num==0xBB)&&
						 ((Cal_Inf->Com_Data[0]>=0x17)&&(Cal_Inf->Com_Data[0]<=0x34))&&
					   ((Cal_Inf->Com_Data[1]>=0x01)&&(Cal_Inf->Com_Data[1]<=0x12))&&
					   ((Cal_Inf->Com_Data[2]>=0x01)&&(Cal_Inf->Com_Data[2]<=0x31))&&
					   ((Cal_Inf->Com_Data[3]>0x00)&&(Cal_Inf->Com_Data[3]<=0x23))&&
					   ((Cal_Inf->Com_Data[4]>0x00)&&(Cal_Inf->Com_Data[4]<=0x59))&&
					   ((Cal_Inf->Com_Data[5]>0x00)&&(Cal_Inf->Com_Data[2]<=0x59))
					
					)
					{    
           
					     System_Cok_DR.Year= System_Year_Start+(Cal_Inf->Com_Data[0]);//设置年
					     System_Cok_DR.Month=(Cal_Inf->Com_Data[1]);//设置月
						   System_Cok_DR.Day=(Cal_Inf->Com_Data[2]);//设置日
						   System_Cok_DR.Hour=(Cal_Inf->Com_Data[3]);//设置时
						   System_Cok_DR.Min=(Cal_Inf->Com_Data[4]);//设置分
						   System_Cok_DR.Sec=(Cal_Inf->Com_Data[5]);//设置秒
						
						
						   /* 复位 RTC8025 */
		           RTC_Init(TRUE);	
						   /* 设置 时间 */ 
	        	 Write_System_Set(&System_Cok_DR);

					  Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year 	=System_Cok_DR.Year;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month 	= System_Cok_DR.Month;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day 	= System_Cok_DR.Day;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour 	= System_Cok_DR.Hour;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min 	= System_Cok_DR.Min;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec 	= System_Cok_DR.Sec;
		
	       	//为了解决RTC时钟芯片意外被写故障，这里先保存一次当前的实时时钟数据为\
		     当上电启动检测到RTC被异常后可以使用这个时间来做作实时时间。
		     /* 修改 仪器 信息表 设置标志 */
		       Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_RTC,
					    sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC));




					}
			else if(Cal_Inf->Chanel_Num==0xCC)
			   {
				 
		 read_GSM_SystemTime_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_GSM_SystemTime_Set.Instru_NO=0x01;//仪器编号
	   read_GSM_SystemTime_Set.Cmd_Code =Com_Config_SystemTime_Cmd_Code_Read_Write;//读系统时间操作码
	   read_GSM_SystemTime_Set.Chanel_Num=0xFF;
	   read_GSM_SystemTime_Set.Data_Len=0X06;      
	   read_GSM_SystemTime_Set.Com_Data[0]=HEX_To_BCD((Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year-0X30));//年
		 read_GSM_SystemTime_Set.Com_Data[1]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month);//月
	   read_GSM_SystemTime_Set.Com_Data[2]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day);//日
		 read_GSM_SystemTime_Set.Com_Data[3]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour);//时
	   read_GSM_SystemTime_Set.Com_Data[4]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min);//分
		 read_GSM_SystemTime_Set.Com_Data[5]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec);//秒
	   read_GSM_SystemTime_Set.Com_Data[6]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_GSM_SystemTime_Set.Com_Data[7]=Com_Sensor_Cal_Get_Check(&read_GSM_SystemTime_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_GSM_SystemTime_Set,13);	
					 
					 
				 }				
					
					

}



/************************************************************************************************/
/**	函 数 名: Com_Config_GSM_Signal_Strength_Read									*/
/**	功能说明:读取短信GSM模块信号强度****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_GSM_Signal_Strength_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_GSM_Signal_Strength_Set;//接收串口数据 

     read_GSM_Signal_Strength_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_GSM_Signal_Strength_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_GSM_Signal_Strength_Set.Cmd_Code =Com_Config_GSM_Signal_Strength_Cmd_Code_Read;//读 GSM模块信号强度操作码
	   read_GSM_Signal_Strength_Set.Chanel_Num=0xFF;
	   read_GSM_Signal_Strength_Set.Data_Len=0X01;      
	   read_GSM_Signal_Strength_Set.Com_Data[0]=(u8)GPRS_SR_Reg.GPRS_Sign_Stre;//GSM模块信号强度值
	   read_GSM_Signal_Strength_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_GSM_Signal_Strength_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_GSM_Signal_Strength_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_GSM_Signal_Strength_Set,8);	
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Cancel_Sound_Light_Alarm		*/
/**	功能说明:向上位机反馈取消声光报警成功****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Cancel_Sound_Light_Alarm(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Cancel_Sound_Light_Alarm_Set;//接收串口数据 

     read_Cancel_Sound_Light_Alarm_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Cancel_Sound_Light_Alarm_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Cancel_Sound_Light_Alarm_Set.Cmd_Code =Cancel_Sound_Light_Alarm_Cmd_Code;//取消声光报警
	   read_Cancel_Sound_Light_Alarm_Set.Chanel_Num=0xFF;
	   read_Cancel_Sound_Light_Alarm_Set.Data_Len=0X01;      
	   read_Cancel_Sound_Light_Alarm_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	   read_Cancel_Sound_Light_Alarm_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Cancel_Sound_Light_Alarm_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Cancel_Sound_Light_Alarm_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Cancel_Sound_Light_Alarm_Set,8);	
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Cancel_Sound_Light_Alarm		*/
/**	功能说明:向上位机反馈取消声光报警成功****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Clear_Historical_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Clear_Historical_Data_Set;//接收串口数据 

     read_Clear_Historical_Data_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Clear_Historical_Data_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Clear_Historical_Data_Set.Cmd_Code =Clear_Historical_Data_Cmd_Code;//清除历史数据操作码
	   read_Clear_Historical_Data_Set.Chanel_Num=0xFF;
	   read_Clear_Historical_Data_Set.Data_Len=0X01;      
	   read_Clear_Historical_Data_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	   read_Clear_Historical_Data_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Clear_Historical_Data_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Clear_Historical_Data_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Clear_Historical_Data_Set,8);	
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Open_GSM_Debug_Information	*/
/**	功能说明:向上位机反馈打开GSM模块串口调试信息成功****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Open_GSM_Debug_Information(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Open_GSM_Debug_Information_Set;//接收串口数据 

     read_Com_Config_Open_GSM_Debug_Information_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Com_Config_Open_GSM_Debug_Information_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Com_Config_Open_GSM_Debug_Information_Set.Cmd_Code =Open_GSM_Debug_Information_Cmd_Code;//打开GSM模块调试信息
	   read_Com_Config_Open_GSM_Debug_Information_Set.Chanel_Num=0xFF;
	   read_Com_Config_Open_GSM_Debug_Information_Set.Data_Len=0X01;      
	   read_Com_Config_Open_GSM_Debug_Information_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	   read_Com_Config_Open_GSM_Debug_Information_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Com_Config_Open_GSM_Debug_Information_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Open_GSM_Debug_Information_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Com_Config_Open_GSM_Debug_Information_Set,8);	
}


/************************************************************************************************/
/**	函 数 名: Com_Config_Close_GSM_Debug_Information	*/
/**	功能说明:向上位机反馈关闭GSM模块串口调试信息成功****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Close_GSM_Debug_Information(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Close_GSM_Debug_Information_Set;//接收串口数据 

     read_Com_Config_Close_GSM_Debug_Information_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Com_Config_Close_GSM_Debug_Information_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Com_Config_Close_GSM_Debug_Information_Set.Cmd_Code =Open_GSM_Debug_Information_Cmd_Code;//打开GSM模块调试信息
	   read_Com_Config_Close_GSM_Debug_Information_Set.Chanel_Num=0xFF;
	   read_Com_Config_Close_GSM_Debug_Information_Set.Data_Len=0X01;      
	   read_Com_Config_Close_GSM_Debug_Information_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	   read_Com_Config_Close_GSM_Debug_Information_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Com_Config_Close_GSM_Debug_Information_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Close_GSM_Debug_Information_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Com_Config_Close_GSM_Debug_Information_Set,8);	
}


/************************************************************************************************/
/**	函 数 名: Com_Config_PowerLink_And_Battery_Voltage_Information	*/
/**	功能说明:向上位机反馈关闭GSM模块串口调试信息成功****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static INSTRU_POWER* Battery=&Instru_Config_Inf_Tab.Inf_State.Instru_Power;//电源相关信息结构体指针
static void Com_Config_PowerLink_And_Battery_Voltage_Information(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set;//接收串口数据 

     read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Cmd_Code =Check_PowerLink_And_Battery_Voltage_Cmd_Code;//返回32位主板外接电及电池电压状态操作码
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Chanel_Num=0xFF;
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Data_Len=0X02;      
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[0]=Battery->Inf_State.ET_Link_State;//主板外接电状态
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[1]=Battery->Inf_State.BT_Quantity_State;//32位主板电池电量                                                                                    
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[2]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[3]=Com_Sensor_Cal_Get_Check(&read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set,9);	
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Open_Data_Save_Record	*/
/**	功能说明:向上位机反馈打开数据记录成功****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Open_Data_Save_Record(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Open_Data_Save_Record_Set;//接收串口数据 

     read_Com_Config_Open_Data_Save_Record_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Com_Config_Open_Data_Save_Record_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Com_Config_Open_Data_Save_Record_Set.Cmd_Code =Open_DataSave_Cmd_Code;//打开数据记录功能
	   read_Com_Config_Open_Data_Save_Record_Set.Chanel_Num=0xFF;
	   read_Com_Config_Open_Data_Save_Record_Set.Data_Len=0X01;      
	   read_Com_Config_Open_Data_Save_Record_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	   read_Com_Config_Open_Data_Save_Record_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Com_Config_Open_Data_Save_Record_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Open_Data_Save_Record_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Com_Config_Open_Data_Save_Record_Set,8);	
}
/************************************************************************************************/
/**	函 数 名: Com_Config_Open_Data_Save_Record	*/
/**	功能说明:向上位机反馈关闭数据记录成功****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Close_Data_Save_Record(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Close_Data_Save_Record_Set;//接收串口数据 

     read_Com_Config_Close_Data_Save_Record_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Com_Config_Close_Data_Save_Record_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Com_Config_Close_Data_Save_Record_Set.Cmd_Code =Open_DataSave_Cmd_Code;//打开数据记录功能
	   read_Com_Config_Close_Data_Save_Record_Set.Chanel_Num=0xFF;
	   read_Com_Config_Close_Data_Save_Record_Set.Data_Len=0X01;      
	   read_Com_Config_Close_Data_Save_Record_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*操作成功标志*/	
	   read_Com_Config_Close_Data_Save_Record_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Com_Config_Close_Data_Save_Record_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Close_Data_Save_Record_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Com_Config_Close_Data_Save_Record_Set,8);	
}


/************************************************************************************************/
/**	函 数 名: Com_Config_Open_Data_Save_Record	*/
/**	功能说明:向上位机反馈关闭数据记录成功****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_DataSave_Status_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_DataSave_Status_Set;//接收串口数据 

     read_Com_Config_DataSave_Status_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   read_Com_Config_DataSave_Status_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   read_Com_Config_DataSave_Status_Set.Cmd_Code =Check_DataSave_Status_Cmd_Code;//查询数据记录状态操作码
	   read_Com_Config_DataSave_Status_Set.Chanel_Num=0xFF;
	   read_Com_Config_DataSave_Status_Set.Data_Len=0X01;      
	   read_Com_Config_DataSave_Status_Set.Com_Data[0]=*Save_Data_Switch_A;/*数据记录状态；0：未启动；1：启动了数据记录*/	
	   read_Com_Config_DataSave_Status_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   read_Com_Config_DataSave_Status_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_DataSave_Status_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&read_Com_Config_DataSave_Status_Set,8);	
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Software_Version_NO_Major_Read	*/
/**	功能说明:读取32位平台的软件版本和硬件版本****************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Software_Version_NO_Major_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Software_Version_NO_Major_Set;//接收串口数据 

	
	   if(Cal_Inf->Chanel_Num==0xAA)
		 {
				 read_Com_Config_Software_Version_NO_Major_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
				 read_Com_Config_Software_Version_NO_Major_Set.Instru_NO=0x01;//仪器编号
				 read_Com_Config_Software_Version_NO_Major_Set.Cmd_Code =Com_Config_Software_Version_NO_Major_Cmd_Code_Read;//查询32位平台软件版本和硬件版本
				 read_Com_Config_Software_Version_NO_Major_Set.Chanel_Num=0xAA;
				 read_Com_Config_Software_Version_NO_Major_Set.Data_Len=0X08;      
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[4]=(Software_Version_NO_Major+0x30);/*软件 主版本编号 */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[5]=0X2E;/* .的ASCII码*/
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[6]=(Software_Version_NO_Child+0x30);/*软件 子版本编号 */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[7]=(Software_Version_NO_Stage+0x30);/*软件 阶段版本编号 */
			
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[0]=(Hardware_Version_NO_Major+0x30);/*硬件 主版本编号 */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[1]=0X2E;/* .的ASCII码*/
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[2]=(Hardware_Version_NO_Child+0x30);/*硬件 子版本编号 */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[3]=(Hardware_Version_NO_Stage+0x30);/*硬件 阶段版本编号 */
			
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[8]=Com_Sensor_Cal_Constant;/* 设置 常数 */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[9]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Software_Version_NO_Major_Set);/* 设置 校验 */	
				 /* 响应 实时数据 数据信息 */
				 USART_printf(USART1,(u8*)&read_Com_Config_Software_Version_NO_Major_Set,15);	
		 }
}

/************************************************************************************************/
/**	函 数 名: Com_Config_Factory_Setting	*/
/**	功能说明:恢复出厂设置***************/
/**	形    参：服务器的配置参数消息指针：Cal_Inf													*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Com_Config_Factory_Setting(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF         Factory_Setting_Set;//接收串口数据 

	    if(Cal_Inf->Com_Data[0]==0x01)
			{	
		 Factory_Setting_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   Factory_Setting_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   Factory_Setting_Set.Cmd_Code =Com_Config_System_Factory_Setting;//恢复出厂设置操作码
	   Factory_Setting_Set.Chanel_Num=0xFF;
	   Factory_Setting_Set.Data_Len=0X01;      
	   Factory_Setting_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*执行恢复出厂设置任务成功*/	
	   Factory_Setting_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   Factory_Setting_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Factory_Setting_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&Factory_Setting_Set,8);	
			/* 恢复出厂设置 */
		 Instru_Rte_Fty_Set();
			}
			else
			{
		 Factory_Setting_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//头码
	   Factory_Setting_Set.Instru_NO=Cal_Inf->Instru_NO;//仪器编号
	   Factory_Setting_Set.Cmd_Code =Com_Config_System_Factory_Setting;//恢复出厂设置操作码
	   Factory_Setting_Set.Chanel_Num=0xFF;
	   Factory_Setting_Set.Data_Len=0X01;      
	   Factory_Setting_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*执行恢复出厂设置任务失败*/			
	   Factory_Setting_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* 设置 常数 */
	   Factory_Setting_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Factory_Setting_Set);/* 设置 校验 */	
		 /* 响应 实时数据 数据信息 */
     USART_printf(USART1,(u8*)&Factory_Setting_Set,8);
			}
}

/************************************************************************************************/
/**	函 数 名: COM_Sensor_Cal_Deal																*/
/**	功能说明: 处理 传感器校准数据																	*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void COM_Sensor_Cal_Deal(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	
	/* 根据 操作码 做对应的事情 */
	switch(Cal_Inf->Cmd_Code)
	{
#if (GSM_Module_Type==2) //ESP8266Wifi模块		
	/*************Wifi模块相关设置**********************/
		case Com_Read_Write_Wifi_Name_Cmd_Code:
		{
		  /* 读和写Wifi名称 */
		 Com_Config_Read_Write_Wifi_Name(Cal_Inf);
			break;
		}
		case Com_Read_Write_Wifi_WSKEY_Cmd_Code:
		{
	   /*读和写Wifi的WSKEY相关参数(加密方式和密码)*/
     Com_Config_Read_Write_Wifi_WSKEY(Cal_Inf);
			break;
		}
	 case Com_Read_Write_Wifi_WALN_Cmd_Code:
		{
	  /*读和写Wifi的WANN操作码相关参数(本地IP地址、子网掩码、网关IP地址 )*/
     Com_Config_Read_Write_Wifi_Wlan(Cal_Inf);
			break;
		}
		case Com_Read_Write_Wifi_NETP_Cmd_Code:
		{
   /* 读和写Wifi的NETP操作码相关参数(要连接的数据服务器的IP地址及端口号)*/
     Com_Config_Read_Write_Wifi_NetP(Cal_Inf);
			break;
		}	
		
		case Com_Read_Write_Wifi_DHCP_Cmd_Code:
		{
   /* 读和写Wifi获取IP地址的方式*/
     Com_Config_Read_Write_Wifi_DHCP(Cal_Inf);
			break;
		}
		case Com_Read_Write_Wifi_MAC_Cmd_Code:
		{
      /* 读和写Wifi模块的MAC地址*/
     Com_Config_Read_Write_Wifi_MAC(Cal_Inf);
			break;
		}	
		case Com_Set_Rest_Wifi_Cmd_Code:
 		{
		  /*复位或者恢复出厂设置*/
		 Com_Config_Rest_Wifi(Cal_Inf);
 		 break;
 		}			
#endif
/**************************************************/		
		case Com_Sensor_Cal_Cmd_Code_Read_Real_Data:
		{
			/* 读实时数据 */
			Com_Sensor_Cal_Read_Real_Data(Cal_Inf);
			break;
		}
		case Com_Sensor_Cal_Cmd_Code_Read_Orig_Data:
		{
			/* 读原始值 */
			Com_Sensor_Cal_Read_Orig_Data(Cal_Inf);
			break;
		}
		case Com_Sensor_Cal_Cmd_Code_Write_Curve_Data:
		{
			/* 写曲线 */
			Com_Sensor_Cal_Write_Curve_Data(Cal_Inf);
			break;
		}
		case Com_Sensor_Cal_Cmd_Code_Read_Curve_Data:
		{
			/* 读曲线 */
			Com_Sensor_Cal_Read_Curve_Data(Cal_Inf);
			break;
		}
		case Com_Config_SN_Cmd_Code_Write:
		{
			
			/* 写仪器号（SN号） */
			Com_Config_SN_Write(Cal_Inf);
			/* 配置仪器SN号 */
			Serial_Number_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number);
			/* 复位温湿度数据寄存器 */
			GPRS_Reset_Sensor_Data_Reg();
			break;
		}
		case Com_Config_SN_Cmd_Code_Read:
		{
			/* 读仪器号（SN号） */
			Com_Config_SN_Read(Cal_Inf);
			break;
		}
		case Com_Config_Sensor_Cmd_Code_Write:
		{
			/* 写传感器配置 */
			Com_Config_Sensor_Write(Cal_Inf);
			
#if (Wireless_RF_Card_Config_Enable == 0x01)
			/* 复位温湿度数据寄存器 */
			GPRS_Reset_Sensor_Data_Reg();
#endif
			break;
		}
		case Com_Config_Sensor_Cmd_Code_Read:
		{
			/* 读传感器配置 */
			Com_Config_Sensor_Read(Cal_Inf);
			break;
		}
		
 		/* 读打印车牌号 */
 		case Com_Config_Card_NO_Cmd_Code_Read:
 		{
 			Com_Config_Card_NO_Read(Cal_Inf);
 			break;
 		}
		
 		/* 写打印车牌号 */
 		case Com_Config_Card_NO_Cmd_Code_Write:
 		{
 			Com_Config_Card_NO_Write(Cal_Inf);
 			break;
 		}
		
		 /* 读 打印公司名称 */
 		case Com_Config_Company_Name_Cmd_Code_Read:
 		{
 			Com_Config_Company_Name_Read(Cal_Inf);
 			break;
 		}
		
 		/* 写 打印公司名称 */
 		case Com_Config_Company_Name_Cmd_Code_Write:
 		{
 			Com_Config_Company_Name_Write(Cal_Inf);
 			break;
 		}
		
		/* 写 数据做假功能 */
 		case Com_Config_Fad_Data_Cmd_Code_Write:
 		{
 			Com_Config_Fad_Data_Write(Cal_Inf);
 			break;
 		}
		
		/* 读 数据做假功能 */
 		case Com_Config_Fad_Data_Cmd_Code_Read:
 		{
 			Com_Config_Fad_Data_Read(Cal_Inf);
 			break;
 		}
		
		/* 写数据服务器域名、IP、端口 */
 		case ComConfigDataNetworkWrite:
 		{	
 			Com_Config_DataNetwork_Write(Cal_Inf);
			
////			/* 重新配置GSM模块 */
////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
 			break;
 		}	
		/* 读数据服务器域名、IP、端口 */
 		case ComConfigDataNetworkRead:
 		{
 			Com_Config_DataNetwork_Read(Cal_Inf);
 			break;
 		}
			/* 写配置服务器域名、IP、端口 */
 		case ComConfigNetworkWrite:
 		{	
 			Com_Config_Network_Write(Cal_Inf);
			
//////			/* 重新配置GSM模块 */
//////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
 			break;
 		}	
		/* 读配置服务器域名、IP、端口 */
 		case ComConfigNetworkRead:
 		{
 			 Com_Config_Network_Read(Cal_Inf);
 			break;
 		}
					/* 写升级服务器域名、IP、端口 */
 		case ComConfigUpgradeNetworkWrite:
 		{	
 			Com_Config_Upgrade_Network_Write(Cal_Inf);
			
//			/* 重新配置GSM模块 */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
 			break;
 		}	
		/* 读升级服务器域名、IP、端口 */
 		case ComConfigUpgradeNetworkRead:
 		{
 			Com_Config_Upgrade_Network_Read(Cal_Inf);
 			break;
 		}
			/* 读短息报警号码*/
 		case Com_Config_SMSAamPoeNO_Cmd_Code_Read:
 		{
 			Com_Config_SMSAamPoeNORead(Cal_Inf);
 			break;
 		}
			/* 写短息报警号码*/
 		case Com_Config_SMSAamPoeNO_Cmd_Code_Write:
 		{
 			Com_Config_SMSAamPoeNOSet(Cal_Inf);
 			break;
 		}
		 /*读时间间隔*/
		case Com_Config_Time_Interval_Cmd_Code_Read:
 		{
 	  	Com_Config_Time_Interval_Read	(Cal_Inf);
 			break;
 		}
/*设置时间间隔（上传时间间隔、历史记录时间间隔、打印时间间隔、 报警记录间隔）*/
		/**************写时间间隔***************************/
		case Com_Config_Time_Interval_Cmd_Code_Write:
 		{
 			 Com_Config_Time_Interval_Set(Cal_Inf);
 			break;
 		}
/***********************设置报警延时 (以秒为单位) *********************************/	

		 /************读报警延时间隔****************************/
		case Com_Config_Alarm_time_delay_Cmd_Code_Read  :
 		{
			
 			Com_Config_Alarm_time_delay_Read(Cal_Inf);
 			break;
 		}	
			
	 /************写报警延时间隔****************************/
		case Com_Config_Alarm_time_delay_Cmd_Code_Write :
 		{
 			Com_Config_Alarm_time_delay_Write(Cal_Inf);
 			break;
 		}

  /************读声光报警总开关状态*************************/
		case Com_ConfigTotal_audible_light_alarm_Cmd_Code_Read :
 		{
			
			
 			Com_Config_Sound_lightSwitch_Read(Cal_Inf);
 			break;
 		}			
 /************写声光报警总开关状态*************************/
		case Com_ConfigTotal_audible_light_alarm_Cmd_Code_Write :
 		{
 			Com_Config_Sound_lightSwitch_Write(Cal_Inf);
 			break;
 		}	
/************读短信报警总开关状态*************************/
		case Com_Config_SMS_alarm_total_switch_Cmd_Code_Read :
 		{
 			Com_Config_SMSalarmSwitch_Read(Cal_Inf);
 			break;
 		}				
/************写短信报警总开关状态*************************/
		case Com_Config_SMS_alarm_total_switch_Cmd_Code_Write :
 		{
 			Com_Config_SMSalarmSwitch_Write(Cal_Inf);
 			break;
 		}	

/************读取GSM模块信号强度*************************/
		case Com_Config_GSM_Signal_Strength_Cmd_Code_Read :
 		{

			Com_Config_GSM_Signal_Strength_Read(Cal_Inf);
 			break;
 		}	 
/************取消声光报警作用于按返回键取消声光报警的作用相同*************************/
		case Cancel_Sound_Light_Alarm_Cmd_Code:
 		{
			
			DispMenuDeskTopDown();//取消声光报警
			Beep(Beep_OFF);
			Com_Config_Cancel_Sound_Light_Alarm(Cal_Inf);//向上位机返回取消声光报警成功
			
 			break;
 		}	
/************清除历史数据0X52************************/
		case Clear_Historical_Data_Cmd_Code:
 		{

			Mcu_Cer_Htc_Data();//清楚历史数据函数
		  Com_Config_Clear_Historical_Data(Cal_Inf);//向上位机返回清除历史数据成功
 			break;
 		}	

/************打开GSM模块串口调试信息************************/
		case Open_GSM_Debug_Information_Cmd_Code:
 		{
       
			   if(Cal_Inf->Com_Data[0]==1)
				 {
				 	/* 打开GSM调试信息输出 */
//					GSM_DEBUG_Flag = TRUE;
					 
					DEBUG_Flag = TRUE;
			 Com_Config_Open_GSM_Debug_Information(Cal_Inf);//向上位机返回打开GSM模块串口调试信息成功 
				 }
				 else if(Cal_Inf->Com_Data[0]==0)
				 {
				 	/* 关闭GSM调试信息输出 */
//					GSM_DEBUG_Flag = FALSE;
						DEBUG_Flag = FALSE; 
			 Com_Config_Close_GSM_Debug_Information(Cal_Inf);//向上位机返回关闭GSM模块串口调试信息成功
				 
				 }
 			break;
 		}
/************输出32主板外接电状态和电池电压值信息************************/
		case Check_PowerLink_And_Battery_Voltage_Cmd_Code:
 		{
       
		Com_Config_PowerLink_And_Battery_Voltage_Information(Cal_Inf);//向上位机返回32位主板外接电状态及电池电压
 			break;
 		}		
/************开启数据记录************************/
		case Open_DataSave_Cmd_Code:
 		{
			  if(Cal_Inf->Com_Data[0]==1)
				{	
       	/* 启动 数据存储功能. */
			App_Save_Config(ENABLE);
		  Com_Config_Open_Data_Save_Record(Cal_Inf);//向上位机返回打开数据记录成功
				}
				else if(Cal_Inf->Com_Data[0]==0)
				{
				
				    /* 关闭 数据存储功能. */
			App_Save_Config(DISABLE);
			
		Com_Config_Close_Data_Save_Record(Cal_Inf);//向上位机返回关闭数据记录成功	
				
				}
				
 			break;
 		}			
///************停止数据记录************************/
//		case Close_DataSave_Cmd_Code:
// 		{
//    
// 			break;
// 		}	
/************查询数据记录的状态************************/
		case Check_DataSave_Status_Cmd_Code:
 		{
			
	   Com_Config_DataSave_Status_Read(Cal_Inf);//向上位机返回数据记录状态			
			 
 			break;
 		}

	case Com_Config_SystemTime_Cmd_Code_Read_Write://上位机软件设置 和读取系统时间
 		{
      Com_Config_SystemTime_Write(Cal_Inf);//读写系统时间
 			break;
 		}	
 case Com_Config_Software_Version_NO_Major_Cmd_Code_Read://上位机软件32位主板软件版本和硬件版本
 		{

			Com_Config_Software_Version_NO_Major_Read(Cal_Inf);//读取32位平台的软件版本和硬件版本
		 
 			break;
 		}	
 case Com_Config_System_Factory_Setting://恢复出厂设置指令
 		{
			
		 Com_Config_Factory_Setting(Cal_Inf);//恢复出厂设置指令
 			break;
 		}
 case  0x4F://串口测试功能码
 {
	 
////	 /* 单温卡片传感器 */
////#define SMS_Sensor_String	
//// char SMS_GBK[50];
////	 
////	 /* 设置电源报警信息 */
////	sprintf(SMS_GBK,SMS_Sensor_String);
	 
//////////      if(gsm_handle.sim800c.pGSM_Send_Sms("18738738517","单温卡片传感器123!!!"))
//////////			{
//////////			   printf("发送短信内容成功！！！\n");
//////////			
//////////			}else{
//////////			
//////////			printf("发送短信内容失败！！！\n");
//////////			}
    break;
 }

		
		default:
		{
			/* 未设定功能 不作处理 */
			__nop();
			break;
		}

	}	
	/* 事件执行完成 清除 数据缓存的数据 */
	Com_Sensor_Cal_Deleted_Buffer_Data(Cal_Inf);
}

/************************************************************************************************/
/**	函 数 名: COM_Sensor_Cal_Timeout_Check														*/
/**	功能说明: 传感器校准 通信超时检测																*/
/**	形    参：传感器校准数据：COM_SENSOR_CAL_TYPEDEF												*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
void COM_Sensor_Cal_Timeout_Check(INSTRU_FIX_INF_TAB* fix_tab,u16 check_time)
{
	
	/* 判断当前是否处于接收数据帧状态 */
	if(fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout > 0)
	{
		/* 当前处于 传感器校准数据帧接收状态 */
		fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout -= check_time;
		/* 判断 数据接收是否超时 */
		if(fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout <= 0)
		{
			/* 数据接收超时，结束数据接收 */
			/* 清除接收据数据量 */
			fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size = 0;
			/* 清除数据帧长度 */
			fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len = 0;
			/* 清除超时时间 */
			fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout = 0;
		}else
		{
			/* 数据接收未超时，继续数据接收 */
			__nop();
		}
	}else
	{
		/* 当前没有接收数据 */
		__nop();
	}
}

/******************* (C) COPYRIGHT 2015 HZ Zeda Team ****END OF FILE****/


