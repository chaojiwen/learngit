/*************************************************************************************************/
/*************************************     32位平台测试     ***************************************/
/* 创建日期：2015/1/13										 									 */
/*************************************************************************************************/
#include "Log_INF.h"

#include "Dev_Fram.h"

#include <stdio.h>
#include <string.h>

/*
*********************************************************************************************************
*	函 数 名: Instru_Inf_State_Tab_State_Check
*	功能说明: 检测 仪器 配置信息表
*	形    参：无
*	返 回 值: 无
*	说明	：在主系统中间隔100MS检测一次。
*********************************************************************************************************
*/
void Instru_Inf_State_Tab_State_Check(INSTRU_CONFIG_INF_TAB* Instru_data_update)
{
//	
//	/* 判断 系统 状态信息 数据更新 */
//	if(Instru_data_update->Inf_State.Instru_System.Inf.Data_Update_Flag)
//	{
//		/* 清除 数据更新标志 */
//		Instru_data_update->Inf_State.Instru_System.Inf.Data_Update_Flag = 0;
//		/* 系统 状态信息 				有 更新数据 */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_System.Inf_Space,
//						  Instru_Inf_Tab_Mem_System_adder,
//						  Instru_System_Inf_Space_Size);
//		
//	}else if(Instru_Inf_State_Tab.Inf_State.Instru_Power.Inf_State.Data_Update_Flag)
//	{
//		/* 清除 数据更新标志 */
//		Instru_data_update->Inf_State.Instru_Power.Inf_State.Data_Update_Flag = 0;
//		/* 电源 状态信息 				有 更新数据 */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Power.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Power_adder,
//						  Instru_Power_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_Fram.Inf_State.Data_Update_Flag)
//	{
//		/* 清除 数据更新标志 */
//		Instru_data_update->Inf_State.Instru_Fram.Inf_State.Data_Update_Flag = 0;
//		/* 外部存储器 FRAM 状态信息 	有 更新数据 */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Fram.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Fram_adder,
//						  Instru_Fram_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_Flash.Inf_State.Data_Update_Flag)
//	{
//		/* 清除 数据更新标志 */
//		Instru_data_update->Inf_State.Instru_Flash.Inf_State.Data_Update_Flag = 0;
//		/* 外部存储器 Flash 状态信息 	有 更新数据 */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Flash.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Flash_adder,
//						  Instru_Flash_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_Display_Device.Inf_State.Data_Update_Flag)
//	{
//		/* 清除 数据更新标志 */	
//		Instru_data_update->Inf_State.Instru_Display_Device.Inf_State.Data_Update_Flag = 0;
//		/* 显示设备 状态 信息 		有 更新数据 */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Display_Device.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Disp_Dev_adder,
//						  Instru_Disp_Dev_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_RTC.Inf_State.Data_Update_Flag)
//	{
//		/* 清除 数据更新标志 */
//		Instru_data_update->Inf_State.Instru_RTC.Inf_State.Data_Update_Flag = 0;
//		/* 实时时钟 状态 信息 		有 更新数据 */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_RTC.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_RTC_adder,
//						  Instru_RTC_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_GSM.Inf_State.Data_Update_Flag)
//	{
//		/* 清除 数据更新标志 */
//		Instru_data_update->Inf_State.Instru_GSM.Inf_State.Data_Update_Flag = 0;
//		/* GSM 状态 信息 			有 更新数据 */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_GSM.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_GSM_adder,
//						  Instru_GSM_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_Sensor.Inf_State.Data_Update_Flag)
//	{
//		/* 清除 数据更新标志 */
//		Instru_data_update->Inf_State.Instru_Sensor.Inf_State.Data_Update_Flag = 0;
//		/* 传感器 状态 信息 			有 更新数据 */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Sensor.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Sensor_adder,
//						  Instru_Sensor_Inf_State_Space_Size);
//	}
//	
//	

}

///*************************************************************************************************/
///* 函数名：Instru_Inf_State_Tab_Config															 */
///* 描述  ：仪器 信息表 配置																	 	 */
///* 输入  ：无																					 */
///* 返回  ：无																					 */
///* 注意  ：																						 */
///*************************************************************************************************/
//void Instru_Inf_State_Tab_Config(void)
//{
//	/* 判断 FRAM 工作状态 */
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram.Inf_State.Chip_Work_State == 0)
//	{
////		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
////		{
////			/* FRAM 工作不正常 */
////			printf("\rFRAM运行不正常工作.\r\n");
////		}		
//		/* 停止程序运行 */		
//		while(1);
//	}else if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram.Inf_State.Chip_Work_State == 1)
//	{
//		/* FRAM 工作正常 */
//		/* 读取 仪器 信息表 设置标志 */
////		Read_Config_Tab(&Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag,sizeof(Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag));
//		/* 判断 仪器 信息表 设置标志 */
//		if(Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag == 1)
//		{
//			/* 仪器 信息表 已经配置过 */
//			/* 从外部的存储块中获取 仪器 信息表. */
//			Read_Config_Tab(Instru_Config_Inf_Tab.Inf_State_Space,sizeof(INSTRU_CONFIG_INF_TAB));
//		}else
//		{
////			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
////			{
////				/* 仪器 信息表 未配置过 */
////				printf("\r仪器信息表未配置过.\r\n");				
////			}
//			/* 使用 默认的 配置信息 */
//			/* 配置 仪器 信息表 标志 */
//			Instru_Config_Inf_Tab = Default_Instru_Config_Inf_State_Tab;
////			/* 设置 SN号 */
////			memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf_Space,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),ENV_SN_NO_SIZE);
//			/* 写 仪器 信息表 设置标志 */
//			Write_Config_Tab(Instru_Config_Inf_Tab.Inf_State_Space,sizeof(INSTRU_CONFIG_INF_TAB));
//		}
//		
//		/* 读取 仪器 固定信息表 设置标志 */
////		Read_Config_Tab(&Instru_Fix_Inf_State_Tab.Inf_State.Data_Update_Flag,sizeof(Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag));
//		/* 判断 仪器 固定信息表 设置标志 */
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Data_Update_Flag == 1)
//		{
//			/* 仪器 固定信息表 已经配置过 */
//			/* 从外部的存储块中获取 仪器 信息表. */
//			Read_Config_Tab(Instru_Fix_Inf_State_Tab.Inf_State_Space,sizeof(INSTRU_FIX_INF_TAB));
//		}else
//		{
////			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
////			{
////				/* 仪器 信息表 未配置过 */
////				printf("\r仪器信息表未配置过.\r\n");
////			}	
//			/* 使用 默认的 配置信息 */
//			/* 配置 仪器 信息表 标志 */
//			Instru_Fix_Inf_State_Tab = Default_Instru_Fix_Inf_State_Tab;
//			/* 写 仪器 信息表 设置标志 */
//			Write_Config_Tab(Instru_Fix_Inf_State_Tab.Inf_State_Space,sizeof(INSTRU_FIX_INF_TAB));
//		}
//		
//	}else
//	{
//		/* 停止程序运行 *///正常情况下不能运行到的地方。		
//		while(1);
//	}
//}






/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

