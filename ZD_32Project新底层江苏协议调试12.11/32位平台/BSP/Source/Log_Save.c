/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/10/27																			 */
/*************************************************************************************************/

#include "interface.h"
#include  "app.h"

#include "Log_Adc1.h"
#include "Log_Battery.h"
#include "Log_RF_Card.h"
#include "Log_Save.h"
#include "Log_Sensor.h"

#include "Dev_Flash.h"
#include "Dev_Fram.h"
#include "Dev_RTC.h"

#include "Mcu_Usart.h"
#include "string.h"


/* 存储传感器数据寄存器 */
static Save_Sensor_Data_Struct Save_Sensor_Reg;
/* 存储数据时间戳 */
static DATETIME_Storage_Format Save_Timestamp;

/* 函数声明 */
static void Sort_Out_Save_Data(INSTRU_SENSOR_FIX* sensor_f);


/*
*********************************************************************************************************
*	函 数 名: Cycle_Save_Data
*	功能说明: 周期性调用 保存 “日期时间”、“传感器数据”、“GPS数据”、等
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* 被删除数据的FLASH数据扇区 */
	u32 flash_sector_erase;
	
	
	//这样写的目的是应为避免FLASH在边界操时，当前数据扇区数据写入正常，当时在后一个扇区数据就写入异常。这样就会导致仪器时钟无法上传数据了。
	/* 判断Flash写指针 超出 1个扇区 范围,真 擦除将要写的Flash页 */
	if((((fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer + Save_Sensor_Data_Len)%Flash_Capacity_Size) >= fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer)&&
	   (((fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer + Save_Sensor_Data_Len)%Flash_Capacity_Size) < (fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer+Flash_Sector_Size)))
	{
		/* 给调度器上锁 */
		OSSchedLock();
		/* 写入数据超过一个扇区大小，需要擦除一个扇区来保存以后的数据 */
		SPI_FLASH_SectorErase(fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer);
		/* 给调度器解锁 */
		OSSchedUnlock();
		/* 将扇区指针定位到下一个扇区 */
		fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer = (fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer+Flash_Sector_Size)%Flash_Capacity_Size;
	}
	
	/* 整理需要保存的数据 */
	Sort_Out_Save_Data(&fix_tab->Inf_State.Instru_Sensor);
	/* 将缓存数据转移到Flash中 */
	Flash_Write_Sensor_Data((u8*)&Save_Sensor_Reg,
							fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer%Flash_Capacity_Size,
							Save_Sensor_Data_Len);
	
	/* 设置Flash写指针 */
	fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer 	= (fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer+Save_Sensor_Data_Len)%Flash_Capacity_Size;
	/* 设置Flash数据存储数量 */
	fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	+= Save_Sensor_Data_Len;
	
	/* 设置 前一次 被删除数据的 FLASH扇区 */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer == 0)
	{
		/* 被删除的扇区是最后一个 */
		flash_sector_erase = Flash_Capacity_Size - Flash_Sector_Size;
	}else
	{
		/* 被删除的扇区不是最后一个 */
		flash_sector_erase = fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer - Flash_Sector_Size;
	}
	
	/* 判断度数据地址指针是否在被删除的FLASH扇区内。 */
	if((fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer >= flash_sector_erase)&&
	   (fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer < (flash_sector_erase+Flash_Sector_Size))&&
	   ((fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer-(s32)Save_Sensor_Data_Len) < fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer))
	{
		
		/* 判断打印的起始数据地址 小于 Flash扇区地址 */
		while(fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer < (flash_sector_erase+Flash_Sector_Size))
		{
			/* 打印的起始数据地址 小于 Flash扇区地址 */
			/* 设置 打印起始数据地址 */
			fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer += Save_Sensor_Data_Len;
		}
		/* 设置 打印起始数据地址 */
		fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer = fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size;
	}
	
	/* 保存Flash配置表参数 */
	Write_Config_Tab(fix_tab->Inf_State.Instru_Flash.Inf_State_Space,sizeof(INSTRU_FLASH_FIX));
	
}

/*************************************************************************************************/
/* 函数名	: Sort_Out_Save_Data												 	 			 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 整理需要保存到Fram中的数据 									 	 	 			 */
/* 创建日期  : 2015/03/30																		 */
/*************************************************************************************************/
static void Sort_Out_Save_Data(INSTRU_SENSOR_FIX* sensor_f)
{
	/* 传感器数据存放数据的开始位置 */
	u8 sensor_data_offset = 0;
	
	/* 清除数据缓存 */
	memset(Save_Sensor_Reg.Save_Sensor_Data_Buffer,0X00,sizeof(Save_Sensor_Data_Struct));

	/* 将 正 数点的数据保存到缓存中 */
	Save_Sensor_Reg.GPRS_IR.Timestamp = Save_Timestamp;

#if (Sensor_Function_Config_Enable != 0x00)
	//2、配置传感器参数
	
/* 判断 通道1 */
#if	(Sensor_CH1_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data;
#endif
	
/* 判断 通道2 */
#if	(Sensor_CH2_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data;
#endif
	
/* 判断 通道3 */
#if	(Sensor_CH3_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data;	
#endif	
	
/* 判断 通道4 */
#if	(Sensor_CH4_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data;	
#endif	
	
/* 判断 通道5 */
#if	(Sensor_CH5_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data;	
#endif

/* 判断 通道6 */
#if	(Sensor_CH6_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data;	
#endif

/* 判断 通道7 */
#if	(Sensor_CH7_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data;	
#endif

/* 判断 通道8 */
#if	(Sensor_CH8_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data;	
#endif

/* 判断 通道9 */
#if	(Sensor_CH9_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道10 */
#if	(Sensor_CH10_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道11 */
#if	(Sensor_CH11_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道12 */
#if	(Sensor_CH12_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道13 */
#if	(Sensor_CH13_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data;	
#endif

/* 判断 通道14 */
#if	(Sensor_CH14_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data;	
#endif

/* 判断 通道15 */
#if	(Sensor_CH15_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data;	
#endif

/* 判断 通道16 */
#if	(Sensor_CH16_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data;	
#endif

/* 判断 通道17 */
#if	(Sensor_CH17_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道18 */
#if	(Sensor_CH18_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道19 */
#if	(Sensor_CH19_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道20 */
#if	(Sensor_CH20_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道21 */
#if	(Sensor_CH21_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data;	
#endif

/* 判断 通道22 */
#if	(Sensor_CH22_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data;	
#endif

/* 判断 通道23 */
#if	(Sensor_CH23_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data;	
#endif

/* 判断 通道24 */
#if	(Sensor_CH24_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data;	
#endif

/* 判断 通道25 */
#if	(Sensor_CH25_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道26 */
#if	(Sensor_CH26_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道27 */
#if	(Sensor_CH27_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道28 */
#if	(Sensor_CH28_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data;	
#endif
	
/* 判断 通道29 */
#if	(Sensor_CH29_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data;	
#endif

/* 判断 通道30 */
#if	(Sensor_CH30_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data;	
#endif

/* 判断 通道31 */
#if	(Sensor_CH31_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data;	
#endif

/* 判断 通道32 */
#if	(Sensor_CH32_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data;	
#endif


	
#endif
	
	//3、后续可能还有其它数据
	{
		/* 补充数据 */
	}


}


/*************************************************************************************************/
/* 函数名	: Save_Get_Timestamp												 	 			 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 程序的主体，用以创建，删除，传的其他类型的事件 									 	 	 */
/* 创建日期  : 2015/03/30																		 */
/*************************************************************************************************/
void Save_Get_Timestamp(DATETIME_Storage_Format* Timestamp)
{
	/* 获取 时间戳 */
	Save_Timestamp = *Timestamp;
}

/*************************************************************************************************/
/* 函数名	: Save_Tmp_To_Float												 	 			 	 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 将存储的温度数据转换成浮点型的数据		 									 	 	 */
/* 创建日期  : 2015/04/28																		 */
/*************************************************************************************************/
float Save_Tmp_To_Float(float tmp)
{
	
	if(tmp == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		return Wireless_RF_Card_Data_Rece_Timeout_Value;
	}else
	{
		return tmp;
	}

}

/*************************************************************************************************/
/* 函数名	: Save_Hum_To_Float												 	 			 	 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 将存储的温度数据转换成浮点型的数据		 									 	 	 */
/* 创建日期  : 2015/04/28																		 */
/*************************************************************************************************/
float Save_Hum_To_Float(float hum)
{
	if(hum == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		return Wireless_RF_Card_Data_Rece_Timeout_Value;
	}else
	{
		return hum;	
	}
}

#if (Gateway_NJSY_Enable == 0x01)
/************************************************************************************************/
/* 函数名	: SaveGw_TimeoutParaCheck												 	 		*/
/* 输入		: data:数据																 		 	*/
/* 输出		: 假如数据不为无线信号数据，那么返回原始数据，假如是无信号数据，返回无信号值。 			*/
/* 作用		: 判断网关的温度数据是否为无信号的数据	(超时数据)	 									*/
/* 创建日期  : 2016/12/05																		*/
/************************************************************************************************/
float SaveGw_TimeoutParaCheck(float data)
{
	if(data == Gateway_NJSY_TimeoutPara)
	{
		return Gateway_NJSY_TimeoutValue;
	}else
	{
		return data;	
	}
}
#elif (Gateway_HZZH_Enable == 0x01)
/************************************************************************************************/
/* 函数名	: SaveGw_TimeoutParaCheck												 	 		*/
/* 输入		: data:数据																 		 	*/
/* 输出		: 假如数据不为无线信号数据，那么返回原始数据，假如是无信号数据，返回无信号值。 			*/
/* 作用		: 判断网关的温度数据是否为无信号的数据	(超时数据)	 									*/
/* 创建日期  : 2016/12/05																		*/
/************************************************************************************************/
float SaveGw_TimeoutParaCheck(float data)
{
	if(data == Gateway_HZZH_TimeoutPara)
	{
		return Gateway_HZZH_TimeoutValue;
	}else
	{
		return data;	
	}
}
#endif

/************************************************************************************************/
/* 函数名	: Log_Save_Flag_Check													 	 		*/
/* 输入  	: *save_inf:存储配置信息指针;*system_time:系统时间指针.									*/
/* 输出		: 无. 																				*/
/* 作用	 	: 根据设定的存储时间间隔(正常存储时间,报警存储时间,实时数据存储开关),发送保存标志.			*/
/* 创建日期	: 2015/08/27																		*/
/************************************************************************************************/
void Log_Save_Flag_Check(INSTRU_MENU *save_inf,Times *system_time)
{
/* 电源电压值数据指针 */
#define Power_Value_P	(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power.Inf.BT_Quantity_Sampl_Value)	
	
	
	/* 发送存储标志的结果. */
	INT8U		err;
	/* 系统 秒 时间数据. */
	u32 		system_second_b;
	/* 转换系统时间会改变数据寄存器中的月份值，因此将系统时间放到这个临时诗句寄存器中. */
	Times 		time_b = *system_time;
	/* 数据存储时间记录. */
	static u32 	store_second_b = 0;

	
//	/* 判断当前电源的状态 */
//	if(Power_Record_Function_Check(*Power_Value_P) == FALSE)
//	{
//		/* 电源电压低于设定值，不进行数据记录功能. */
//		__nop();
//		return;
//	}

#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
	/* 判断 实时数据存储开关. */
	if(save_inf->Inf_State.Menu_Save_Data_Switch == TRUE)
#endif
	{

		/* 将系统的时间转换成秒中数据. */
		system_second_b = RTC_Date_Time_To_Second(&time_b);
		/* 判断数据存储是否在同一时间点上重复存储. */
		if(store_second_b == system_second_b)
		{
			/* 时间相同不再存储. */
			return;
		}
		/* 时间不相同,记录当前时间. */
		store_second_b = system_second_b;
		
		/* 当前传感器:未报警. */
		if((system_second_b%save_inf->Inf_State.Normal_Save_Data_Interval)==0)
		{
			/* 时间 达到 保存时间，启动一次保存. */
			/* 发送 接收完成 程序升级数据包 标志. */
			OSFlagPost( 				//向标志发信号
			Str_Save_Flag,				//发送标志的指针
			(OS_FLAGS)0XFF, 			//选择要发送的信号 给第1位发信号 0010  同样把2强制转化为OS_FLAGS型的数据
										//因为信号为OS_FLAGS型的
			OS_FLAG_SET,				//信号有效的选项 信号置1  OS_FLAG_SET为置1	OS_FLAG_CLR为置0
			&err);						//标志信号
		}
		/* 当前传感器:报警. */
		else if((system_second_b%save_inf->Inf_State.Alarm_Save_Data_Interval)==0)
		{
			/* 时间 达到 保存时间. */
			/* 判断 传感器当前报警状态. */
			if(0
#if (Sensor_CH1_Config_Type != Sensor_Null)				
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Alarm_State
#endif
#if (Sensor_CH2_Config_Type != Sensor_Null)			
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Alarm_State
#endif
#if (Sensor_CH3_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Alarm_State
#endif
#if (Sensor_CH4_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Alarm_State
#endif
#if (Sensor_CH5_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Alarm_State
#endif
#if (Sensor_CH6_Config_Type != Sensor_Null)			
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Alarm_State
#endif
#if (Sensor_CH7_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Alarm_State
#endif
#if (Sensor_CH8_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Alarm_State
#endif
#if (Sensor_CH9_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Alarm_State
#endif
#if (Sensor_CH10_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Alarm_State
#endif
#if (Sensor_CH11_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Alarm_State
#endif
#if (Sensor_CH12_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Alarm_State
#endif
#if (Sensor_CH13_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Alarm_State
#endif
#if (Sensor_CH14_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Alarm_State
#endif
#if (Sensor_CH15_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Alarm_State
#endif
#if (Sensor_CH16_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Alarm_State
#endif
#if (Sensor_CH17_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Alarm_State
#endif
#if (Sensor_CH18_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Alarm_State
#endif
#if (Sensor_CH19_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Alarm_State
#endif
#if (Sensor_CH20_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Alarm_State
#endif
#if (Sensor_CH21_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Alarm_State
#endif
#if (Sensor_CH22_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Alarm_State
#endif
#if (Sensor_CH23_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Alarm_State
#endif
#if (Sensor_CH24_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Alarm_State
#endif
#if (Sensor_CH25_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Alarm_State
#endif
#if (Sensor_CH26_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Alarm_State
#endif
#if (Sensor_CH27_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Alarm_State
#endif
#if (Sensor_CH28_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Alarm_State
#endif
#if (Sensor_CH29_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Alarm_State
#endif
#if (Sensor_CH30_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Alarm_State
#endif
#if (Sensor_CH31_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Alarm_State
#endif
#if (Sensor_CH32_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Alarm_State
#endif
			)
			{
				/* 发送 接收完成 程序升级数据包 标志 */
				OSFlagPost( 				//向标志发信号
				Str_Save_Flag,				//发送标志的指针
				(OS_FLAGS)0XFF, 			//选择要发送的信号 给第1位发信号 0010  同样把2强制转化为OS_FLAGS型的数据
											//因为信号为OS_FLAGS型的
				OS_FLAG_SET,				//信号有效的选项 信号置1  OS_FLAG_SET为置1	OS_FLAG_CLR为置0
				&err);						//标志信号
			}
		}


	}
	
}



/******************* (C) ZHOU *****END OF FILE****/


