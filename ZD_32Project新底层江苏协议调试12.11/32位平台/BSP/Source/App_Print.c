/************************************************************************************************/
/** @file              : App_Print.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年9月14日															*/
/** @brief             : c file																	*/
/** @description       : 应用打印功能文件														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "System_Config.h"


#if (Print_Function_Enable == 0x01)

#include "App_Print.h"

#include "Dev_Fram.h"

#include "Log_Time.h"

#include "Mcu_Config.h"



/************************************************************************************************/


/* 打印应用 打印功能数据指针 */
static INSTRU_PRINT_FIX *App_Print_Data_P = &Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print;

/************************************************************************************************/
/* 函数名	: App_Print_Date_Init													 	 		*/
/* 输入  	: 无																				*/
/* 输出		: 无. 																				*/
/* 作用	 	: 初始化打印日期时间																	*/
/* 创建日期	: 2015/11/26																		*/
/************************************************************************************************/
void App_Print_Date_Init(void)
{

/* 一天时间的秒钟数. */
#define Print_Updata_Interval	(60ul*60ul*24ul)
	
	/* 系统时间 */
	u32 system_second_b;
	Times system_time;
	/* 打印更新时间 */
	u32 updata_time;

	
	/* 读取 RTC8025 的时间数据 */
	Read_System_Date_Time_Data((Times*)&system_time);

	/* 转换当前的系统时间，获得系统的秒中数量. */
	system_second_b = RTC_Date_Time_To_Second(&system_time);
	
	/* 判断打印日期时间是否在一天的范围内。 */
	if((App_Print_Data_P->Inf_State.Print_Start_Time > system_second_b)||(App_Print_Data_P->Inf_State.Print_End_Time < system_second_b))
	{
		/* 设置更新时间 */
		updata_time = system_second_b%Print_Updata_Interval;
		
		/* 当前到达自动更新打印"起始时间"和"结束时间"的时刻. */
		App_Print_Data_P->Inf_State.Print_Start_Time = system_second_b + Print_Time_Updata - updata_time;
		App_Print_Data_P->Inf_State.Print_End_Time   = App_Print_Data_P->Inf_State.Print_Start_Time + Print_Updata_Interval;
		
		/* 修改 仪器 信息表 */
		Write_Config_Tab((u8*)&App_Print_Data_P->Inf_State.Print_Start_Time,
						sizeof(App_Print_Data_P->Inf_State.Print_Start_Time));
		
		Write_Config_Tab((u8*)&App_Print_Data_P->Inf_State.Print_End_Time,
						sizeof(App_Print_Data_P->Inf_State.Print_End_Time));	
	}

#undef Print_Updata_Interval

}

/************************************************************************************************/
/* 函数名	: App_Print_Updata_Date													 	 		*/
/* 输入  	: cur_time:当前时间(这个时间单位:秒).													*/
/* 输出		: 无. 																				*/
/* 作用	 	: 自动更新打印"起始时间"和"结束时间"(一天更新一次).										*/
/* 创建日期	: 2015/09/15																		*/
/************************************************************************************************/
void App_Print_Updata_Date(Times system_time)
{

/* 一天时间的秒钟数. */
#define Print_Updata_Interval	(60ul*60ul*24ul)

	/* 系统时间 */
	u32 system_second_b;
	/* 打印更新时间 */
	u32 updata_time;



	/* 转换当前的系统时间，获得系统的秒中数量. */
	system_second_b = RTC_Date_Time_To_Second(&system_time);
	/* 设置更新时间 */
	updata_time = system_second_b%Print_Updata_Interval;
	
	/* 判断更新到达时间. */
	if((updata_time == Print_Time_Updata)||
	  ((App_Print_Data_P->Inf_State.Print_Start_Time == 0)&&
	   (App_Print_Data_P->Inf_State.Print_End_Time   == 0)))
	{
		/* 当前到达自动更新打印"起始时间"和"结束时间"的时刻. */
		App_Print_Data_P->Inf_State.Print_Start_Time = system_second_b + Print_Time_Updata - updata_time;
		App_Print_Data_P->Inf_State.Print_End_Time   = App_Print_Data_P->Inf_State.Print_Start_Time + Print_Updata_Interval;
		/* 修改 仪器 信息表 */
		Write_Config_Tab((u8*)&App_Print_Data_P->Inf_State.Print_Start_Time,
						sizeof(App_Print_Data_P->Inf_State.Print_Start_Time));
		Write_Config_Tab((u8*)&App_Print_Data_P->Inf_State.Print_End_Time,
						sizeof(App_Print_Data_P->Inf_State.Print_End_Time));


	}
	


#undef Print_Updata_Interval
}

#endif

/******************* (C) HZ Zeda Team *****END OF FILE****/

















