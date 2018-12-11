/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/10/24																			 */
/*************************************************************************************************/
#include "Log_Protocol_ Analysis.h"

#include "common.h"
#include "Ks0108.h"	 

#include "Mcu_Beep.h"
#include "Mcu_led.h"

#include "Log_GSM.h"

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 接收数据缓存，判断配置表是否有修改，有修改的话，改配置表里的对应数据
*	传递参数：*buf：接收数组缓存，非空(配置信息)
					  len：数组缓存 长度
						* Config_Content：配置表 信息 结构体
						Packet_num：数组缓存里 包含几包数据!
						* tab：根据缓存修改的内容，修改内存中 仪器配置表
*	返 回 值: 无 __nop(); 
*********************************************************************************************************
*/
void RECBUF_config_change_Deal(char *buf,int len,CONFIG_TAB_CHANGE* Config_Content[],u16 Packet_num,INSTRU_CONFIG_INF_TAB* TAB)
{	
  u32 data_Length=0;
	u16 j=0,i=0;
	
	//里面用到的数组 要考虑下溢出保护的问题！
	for(i=0;i<Packet_num;i++)//数组中 有几包数据(小包数据) 处理几次
	{
		Config_Content[i]->Change_Type     = buf[0+data_Length];//类型
		Config_Content[i]->Change_Adder    = buf[1+data_Length];//地址
		Config_Content[i]->Change_Data_Len = buf[2+data_Length];//数据长度(小包)
		
		data_Length=data_Length+3;//类型 地址 数据长度 3字节	
		
		if(0x01==Config_Content[i]->Change_Type)//系统信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_System_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }
			data_Length=data_Length+j;
		}
		else if(0x02==Config_Content[i]->Change_Type)//电源状态信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Power_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }	  
			data_Length=data_Length+j;			
		}
		else if(0x03==Config_Content[i]->Change_Type)//系统时钟信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_RTC_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		else if(0x04==Config_Content[i]->Change_Type)//GSM模块信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_GSM_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }			
			data_Length=data_Length+j;			
		}
		else if(0x05==Config_Content[i]->Change_Type)//传感器信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Sensor_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		else if(0x06==Config_Content[i]->Change_Type)//菜单控制信息,即报警总开关信息地址
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Alarm_Master_Switch_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		else if(0x07==Config_Content[i]->Change_Type)//传感器校准数据表信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Cal_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		
	}
  
}
/*********************************************************************************************************
*	函 数 名: 
*	功能说明: 接收配置表信息结构体(传入的数据)，判断配置表是否有修改，有修改的话，取TAB里面的数据，并赋值给BUF
*	传递参数：*buf：空缓存，存储配置信息：类型+地址+数据长度+对应的配置数据
					  len：buf[] 数组缓存 总长度
						* Config_Content：配置表 信息 结构体(传入的数据)
						Packet_num：数组缓存里 包含几包数据!
						* tab：仪器配置表
*	返 回 值: 无 __nop(); 
*********************************************************************************************************
*/
void RECTAB_config_change_Deal(char *buf,int len,CONFIG_TAB_CHANGE* Config_Content[],u16 Packet_num,INSTRU_CONFIG_INF_TAB* TAB)
{
	//len 最大 1024
	u32 data_Length=0;
	u16 j=0,i=0;
	
	//里面用到的数组 要考虑下溢出保护的问题！
	for(i=0;i<Packet_num;i++)//数组中 有几包数据(小包数据) 处理几次
	{
		buf[0+data_Length] = Config_Content[i]->Change_Type;//类型
		buf[1+data_Length] = Config_Content[i]->Change_Adder;//地址
		buf[2+data_Length] = Config_Content[i]->Change_Data_Len;//数据长度(小包)
		
		data_Length=data_Length+3;//类型 地址 数据长度 3字节	
		
		if(0x01==Config_Content[i]->Change_Type)//系统信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_System_adder+Config_Content[i]->Change_Adder+j)));
      }
			data_Length=data_Length+j;
		}
		else if(0x02==Config_Content[i]->Change_Type)//电源状态信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Power_adder+Config_Content[i]->Change_Adder+j)));
      }	  
			data_Length=data_Length+j;			
		}
		else if(0x03==Config_Content[i]->Change_Type)//系统时钟信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_RTC_adder+Config_Content[i]->Change_Adder+j)));
      }				
			data_Length=data_Length+j;
		}
		else if(0x04==Config_Content[i]->Change_Type)//GSM模块信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_GSM_adder+Config_Content[i]->Change_Adder+j)));
      }			
			data_Length=data_Length+j;			
		}
		else if(0x05==Config_Content[i]->Change_Type)//传感器信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Sensor_adder+Config_Content[i]->Change_Adder+j)));
      }				
			data_Length=data_Length+j;
		}
		else if(0x06==Config_Content[i]->Change_Type)//菜单控制信息,即报警总开关信息地址
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Alarm_Master_Switch_adder+Config_Content[i]->Change_Adder+j)));
      }				
			data_Length=data_Length+j;
		}
		else if(0x07==Config_Content[i]->Change_Type)//传感器校准数据表信息
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Cal_adder+Config_Content[i]->Change_Adder+j)));
      }				
			data_Length=data_Length+j;
		}
	}
	
}



