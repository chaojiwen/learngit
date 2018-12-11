/************************************************************************************************/
/** @file              : MenuDesktop.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月4日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->设置界面														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "System_Config.h"

#include "App_Save.h"

#include "Ks0108.H"
#include "Menu.H"

#include "Log_Alarm.h"
#include "Log_Battery.h"
#include "Log_GSM.h"
#include "Log_Print.h"
#include "Log_RF_Card.h"
#include "Log_Sensor.h"
#include "Log_IAP.h"

#include <string.h>
#include <stdio.h>

/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>菜单首界面相关的宏设置
//	<e>菜单首界面项目选择
//	<i>根据需求设置<菜单首界面>项目
#define DeskTop_Set_Enable					(0x01)

//	<e>首界面项目切换显示时间
//  <o.0..20>								<1-300:1><#*1000>
//	<i>这里的时间单位是毫秒(ms),设置范围在1到300秒之间。
#define Menu_DeskTop_Switch_Time			(3000)
//  </e>

//	<e>菜单空闲状态时间
//  <o.0..20>								<5-300:1><#*1000>
//	<i>这里的时间单位是毫秒(ms),设置范围在20到300秒之间。
#define Menu_Idle_State_Time				(20000)
//  </e>



//  </e>

// </h>
//	<<< end of configuration section >>>


/********************************* 宏定义 **********************************/
#define System_Times_String_PosX     		  (0u)
#define System_Times_String_PosY     		  (16u)
#define System_Times_String_XSize    		  (80u)
#define System_Times_String_YSize    		  (48u)
                                          
#define System_Date_String_PosX     		  (System_Times_String_PosX+System_Times_String_XSize+8)
#define System_Date_String_PosY     		  (16u)
#define System_Date_String_XSize    		  (40u)
#define System_Date_String_YSize    		  (16u)
                                          
#define Sensor_ICO_PosX     				      (0u)	
#define Sensor_ICO_PosY     				      (16u)
#define Sensor_ICO_XSize    				      (24u)
#define Sensor_ICO_YSize    				      (48u)
                                       
#define Temp_Sensor_Data_Dis_PosX     		(32u)	
#define Temp_Sensor_Data_Dis_PosY     		(16u)
#define Temp_Sensor_Data_Dis_XSize    		(48u)
#define Temp_Sensor_Data_Dis_YSize    		(48u)

#define Humi_Sensor_Data_Dis_PosX     		(32u)
#define Humi_Sensor_Data_Dis_PosY     		(16u)
#define Humi_Sensor_Data_Dis_XSize    		(48u)
#define Humi_Sensor_Data_Dis_YSize    		(48u)
                                          
#define Lon_Sensor_Data_Dis_PosX     	    (32u)	
#define Lon_Sensor_Data_Dis_PosY     	    (16u)
#define Lon_Sensor_Data_Dis_XSize    	    (128u)
#define Lon_Sensor_Data_Dis_YSize    	    (64u)
                                          
#define Lat_Sensor_Data_Dis_PosX     	    (32u)	
#define Lat_Sensor_Data_Dis_PosY     	    (16u)
#define Lat_Sensor_Data_Dis_XSize    	    (128u)
#define Lat_Sensor_Data_Dis_YSize    	    (64u)
                                          
#define Spd_Sensor_Data_Dis_PosX     	    (32u)	
#define Spd_Sensor_Data_Dis_PosY     	    (16u)
#define Spd_Sensor_Data_Dis_XSize    	    (128u)
#define Spd_Sensor_Data_Dis_YSize    	    (64u)
                                          
#define RF_Card_Rece_Data_Timeout_PosX     	(48u)
#define RF_Card_Rece_Data_Timeout_PosY     	(32u)
#define RF_Card_Rece_Data_Timeout_XSize    	(RF_Card_Rece_Data_Timeout_PosX*3)
#define RF_Card_Rece_Data_Timeout_YSize    	(16u)

#define Sensor_Data_Dis_PosX     			(32u)
#define Sensor_Data_Dis_PosY     			(16u)
#define Sensor_Data_Dis_XSize    			(108u)
#define Sensor_Data_Dis_YSize    			(64u)


/************************************************************************************************/
/* 菜单首界面项目循序显示时间寄存器 */
static u32 Menu_DeskTop_Switch_Time_Reg = 0;
/* 菜单空闲状态时间寄存器 */
static u32 Menu_Idle_State_Time_Reg = 0;
/* 菜单首界面项目界面位置寄存器 */
static u32 Menu_DeskTop_Disp_Reg = 0;
/************************************************************************************************/

#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
/* 实时数据存储开关 数据指针 */
static FunctionalState *Menu_Save_Data_Switch_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch;
#endif



/************************************************************************************************/
/* 函数名	: Menu_RF_Card_GUI_Dis																*/
/* 输入		: 界面指针:gui_vle;传感器信息结构指针:sensor_inf										*/
/* 输出		: 无 																				*/
/* 作用		: 根据界面指针位置，找出对应通道类，假如是卡片类型，再判断卡片的ID是否配置:配置就显示该通道数据，没有配置就跳过.	*/
/* 创建日期	: 2015/10/27																		*/
/************************************************************************************************/ 
static BOOL Menu_RF_Card_GUI_Dis(u8 gui_vle,INSTRU_SENSOR *sensor_inf)
{

	INSTRU_SENSOR_CHANNEL *sensor_ch = (INSTRU_SENSOR_CHANNEL*)(&sensor_inf->Inf_State_Space[gui_vle*sizeof(INSTRU_SENSOR_CHANNEL)]);
	
	if ((sensor_ch->Type == Sensor_RF_Card_Single_Tmp)||
		(sensor_ch->Type == Sensor_RF_Card_Humiture_Tmp)||
		(sensor_ch->Type == Sensor_RF_Card_Humiture_Hum)||
		(sensor_ch->Type == Sensor_GW_NJSY_Tmp)||
		(sensor_ch->Type == Sensor_GW_NJSY_Hum)||
		(sensor_ch->Type == Sensor_GW_HZZH_Tmp)||
		(sensor_ch->Type == Sensor_GW_HZZH_Hum))
	
	{
		/* 卡片类型. */
		
		/* 判断卡片ID是否配置过 */
		if(sensor_ch->SENSOR_RF_Card_ID_NO == 0)
		{
			/* 卡片ID没有配置过 */
			return FALSE;
		
		}else
		{
			/* 卡片ID有配置过 */
			return TRUE;
		}
	}else
	{
		/* 不是卡片类型 */
		return TRUE;
	}


}

/************************************************************************************************/
/* 函数名	: Menu_DeskTop_Switch_Reset															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 清除首界面项目切换时间寄存器数据.														*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/ 
void Menu_DeskTop_Switch_Reset(u32 time)
{
	Menu_DeskTop_Switch_Time_Reg = time;
}

/************************************************************************************************/
/* 函数名	: Menu_Idle_State_Reset																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 清除菜单空闲状态时间寄存器数据.														*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/ 
void Menu_Idle_State_Reset(u32 time)
{
	Menu_Idle_State_Time_Reg = time;
}

/************************************************************************************************/
/* 函数名	: Menu_DeskTop_Switch_Disp															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 菜单首界面项目循序显示。																*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/ 
void Menu_DeskTop_Switch_Disp(u32 time)
{
	BOOL gui_dis_state = FALSE;
	
	/* 判断 菜单首界面项目循序显示时间是否大于设定的时间。 */
	if(time - Menu_DeskTop_Switch_Time_Reg >= Menu_DeskTop_Switch_Time)
	{
		/* 菜单首界面项目循序显示时间大于显示时间，切换下一个项目。 */
		/* 判断 当前的菜单页面状态 */
		if((Layer == 0)||(Layer == 1))
		{
			/* 将菜单首界面项目指针指向下一个项目。 */
			Menu_DeskTop_Disp_Reg = (++Menu_DeskTop_Disp_Reg)%(Sensor_CH_Config_Num+1);
			
			for(; (Menu_DeskTop_Disp_Reg != 0) ; Menu_DeskTop_Disp_Reg = (++Menu_DeskTop_Disp_Reg)%(Sensor_CH_Config_Num+1))
			{
				/* 根据界面指针位置，找出对应通道类，假如是卡片类型，再判断卡片的ID是否配置:配置就显示该通道数据，没有配置就跳过。 */
				gui_dis_state = Menu_RF_Card_GUI_Dis(Menu_DeskTop_Disp_Reg-1,&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor);
				
				/* 判断界面显示状态 */
				if(gui_dis_state == TRUE)
				{
					/* 显示当前指针的界面 */
					break;
				}
			}
			/* 设置 菜单首界面项目循序显示时间寄存器 */
			Menu_DeskTop_Switch_Time_Reg = time;
			/* 将函数指针指向待机界面 */
			Jump2Menu(MenuDeskTopIndex,FlashMode_ManualInit);
		}
	}
	
}
/************************************************************************************************/
/* 函数名	: Menu_Idle_State_Check																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 检测菜单是否长时间处于空闲状态,当菜单长时间处于空闲状态时返回首界面。						*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/ 
void Menu_Idle_State_Check(u32 time)
{	
	
	
	/* 判断 菜单空闲状态时间寄存器 的时间到现在是否大于设定的时间。 */
	if(time - Menu_Idle_State_Time_Reg >= Menu_Idle_State_Time)
	{
		/* 菜单处于空闲状态 */
		/* 判断 当前的菜单页面状态 */
		if((Layer != 0)&&(Layer != 1))
		{
			/*当前的菜单页面不再首界面。需要调会首界面。 */
			/* 初始化菜单系统。 */
			InitMenuVal();
			/* 判断 当前 是否为管理员或超级管理员模式 */
			if(User != Domestic)
			{
				/* 当前模式为:管理员或超级管理员模式 */
				/* 在退出功能设置界面时，管理员或超级管理员模式将被设置为普通人模式 */
				User = Domestic;
			}
			/* 设置 菜单空闲状态时间寄存器 */
			Menu_Idle_State_Time_Reg = time;
			/* 将函数指针指向待机界面 */
			Jump2Menu(MenuDeskTopIndex,FlashMode_ManualInit);
		}
	}

}

#if ((Wireless_RF_Card_Config_Enable == 0x01)||\
	 (GPS_Config_Enable == 0x01)||\
	 (Gateway_NJSY_Enable == 0x01)||\
	 (Gateway_HZZH_Enable == 0x01))
/************************************************************************************************/
/* 函数名	: Disp_Desk_Top_Sonsor_Data_Timeout_Item											*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 显示数据接收超时							 										*/
/* 创建日期	：2015/8/20																			*/
/************************************************************************************************/
static void Disp_Desk_Top_Sonsor_Data_Timeout_Item(void)
{
	u8 Inf_string[16]={0};

	
	/* 整合信息字符 */
	sprintf((char*)Inf_string,"无信号");
	/* 显示无信号 */
	GUI_DispStringAt(Inf_string,RF_Card_Rece_Data_Timeout_PosX,RF_Card_Rece_Data_Timeout_PosY);

}
#endif


#if ((Wireless_RF_Card_Config_Enable == 0x01)||\
	 (Gateway_NJSY_Enable == 0x01)||\
	 (Gateway_HZZH_Enable == 0x01))
/************************************************************************************************/
/* 函数名	: DispDeskTopSonsorRFCardTimeoutItem												*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 显示无线卡数据接收超时							 									*/
/* 创建日期	：2015/6/4																			*/
/************************************************************************************************/
static void DispDeskTopSonsorRFCardTimeoutItem(void)
{
	/* 显示无线号 */
	Disp_Desk_Top_Sonsor_Data_Timeout_Item();
}
#endif

#if (GPS_Config_Enable == 0x01)
/************************************************************************************************/
/* 函数名	: Disp_Desk_Top_Sonsor_Lon_Timeout_Item														*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 显示经度数据接收超时							 									*/
/* 创建日期	：2015/8/20																			*/
/************************************************************************************************/
static void Disp_Desk_Top_Sonsor_Lon_Timeout_Item(void)
{
	/* 显示无线号 */
	Disp_Desk_Top_Sonsor_Data_Timeout_Item();
}

/************************************************************************************************/
/* 函数名	: Disp_Desk_Top_Sonsor_Lat_Timeout_Item														*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 显示纬度数据接收超时							 									*/
/* 创建日期	：2015/8/20																			*/
/************************************************************************************************/
static void Disp_Desk_Top_Sonsor_Lat_Timeout_Item(void)
{
	/* 显示无线号 */
	Disp_Desk_Top_Sonsor_Data_Timeout_Item();
}

/************************************************************************************************/
/* 函数名	: Disp_Desk_Top_Sonsor_Spd_Timeout_Item														*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 显示速度数据接收超时							 									*/
/* 创建日期	：2015/8/20																			*/
/************************************************************************************************/
static void Disp_Desk_Top_Sonsor_Spd_Timeout_Item(void)
{
	/* 显示无线号 */
	Disp_Desk_Top_Sonsor_Data_Timeout_Item();
}
#endif

/************************************************************************************************/
/* 函数名	: DispDeskTopSonsorHumItem															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示传感器通道号或者卡片SN号															*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
static void DispDeskTopSonsorLabel(u8 ch_no,INSTRU_SENSOR *sensor_t)
{
/* 通道标号图片的宽度 */
#define Sensor_Label_BMP_XSize	(32u)
/* 通道标号图片的高度 */
#define Sensor_Label_BMP_YSize	(16u)
/* 显示界面1 */
#define UI_1    (1u)
/* 显示界面2 */
#define UI_2    (2u)
/* 显示界面3 */
#define UI_3    (3u)
/* 显示界面4 */
#define UI_4    (4u)
/* 显示界面5 */
#define UI_5    (5u)
/* 显示界面6 */
#define UI_6    (6u)
/* 显示界面7 */
#define UI_7    (7u)
/* 显示界面8 */
#define UI_8    (8u)
/* 显示界面9 */
#define UI_9    (9u)
/* 显示界面10 */
#define UI_10    (10u)
/* 显示界面11 */
#define UI_11    (11u)
/* 显示界面12 */
#define UI_12    (12u)
/* 显示界面13 */
#define UI_13    (13u)
/* 显示界面14 */
#define UI_14    (14u)
/* 显示界面15 */
#define UI_15    (15u)
/* 显示界面16 */
#define UI_16    (16u)
/* 显示界面17 */
#define UI_17    (17u)
/* 显示界面18 */
#define UI_18    (18u)
/* 显示界面19 */
#define UI_19    (19u)
/* 显示界面20 */
#define UI_20    (20u)
/* 显示界面21 */
#define UI_21    (21u)
/* 显示界面22 */
#define UI_22    (22u)
/* 显示界面23 */
#define UI_23    (23u)
/* 显示界面24 */
#define UI_24    (24u)
/* 显示界面25 */
#define UI_25    (25u)
/* 显示界面26 */
#define UI_26    (26u)
/* 显示界面27 */
#define UI_27    (27u)
/* 显示界面28 */
#define UI_28    (28u)
/* 显示界面29 */
#define UI_29    (29u)
/* 显示界面30 */
#define UI_30    (30u)
/* 显示界面31 */
#define UI_31    (31u)
/* 显示界面32 */
#define UI_32    (32u)
	
	
	switch(ch_no)
	{
		
#if   (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01) ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_1:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH1.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_1:
		{
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_1],Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_2:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH2.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;	
		}
#else	
		case UI_2:
		{		
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_2],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}		
#endif
			
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_3:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH3.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}			
#else	
		case UI_3:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_3],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_4:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH4.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_4:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_4],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_5:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH5.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_5:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_5],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_6:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH6.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_6:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_6],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_7:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH7.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_7:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_7],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_8:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH8.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_8:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_8],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_9:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH9.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_9:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_9],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_10:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH10.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_10:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_10],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_11:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH11.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_11:
		{	
		/* 显示传感器通道号 */
		GUI_Draw_CH_NO(Sensor_CH_Index[UI_11],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_12:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH12.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_12:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_12],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif		
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_13:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH13.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_13:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_13],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_14:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH14.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_14:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_14],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_15:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH15.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_15:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_15],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_16:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH16.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_16:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_16],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif		
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_17:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH17.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_17:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_17],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_18:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH18.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_18:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_18],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_19:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH19.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_19:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_19],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_20:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH20.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_20:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_20],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif		
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_21:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH21.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_21:
		{		
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_21],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_22:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH22.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_22:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_22],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_23:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH23.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_23:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_23],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_24:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH24.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_24:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_24],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif			
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_25:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH25.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_25:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_25],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_26:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH26.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_26:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_26],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_27:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH27.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_27:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_27],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_28:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH28.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_28:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_28],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}		
#endif		
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_29:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH29.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_29:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_29],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif
		
#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_30:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH30.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_30:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_30],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_31:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH31.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_31:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_31],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif

#if    (((Wireless_RF_Card_Config_Enable == 0x01)||\
		(Gateway_NJSY_Enable == 0x01)  ||\
		(Gateway_HZZH_Enable == 0x01)) &&\
	   ((Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum) ||\
		(Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp) ||\
		(Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum)))
		case UI_32:
		{
			/* 显示传感器通道号 */
			GUI_Draw_Card_SN_NO(sensor_t->Inf_State.CH32.SENSOR_RF_Card_ID_NO,Sensor_CH_PosX,Sensor_CH_PosY);
			break;		
		}
#else	
		case UI_32:
		{	
			/* 显示传感器通道号 */
			GUI_Draw_CH_NO(Sensor_CH_Index[UI_32],Sensor_CH_PosX,Sensor_CH_PosY);
			break;
		}	
#endif				
		
        default:
		{
			/* 清除 显示信息区域的内容 */
			TipClr(Sensor_CH_PosX,Sensor_CH_PosY,Sensor_CH_PosX+Sensor_Label_BMP_XSize,Sensor_CH_PosY+Sensor_Label_BMP_YSize);
			break;
        }
	}
	
	
	
//		/* 显示传感器通道号 */
//		GUI_Draw_Bar_BMP(Sensor_CH_Index[ch_no],Sensor_CH_PosX,Sensor_CH_PosY);	
	
	
/* 通道标号图片的宽度 */
#undef Sensor_Label_BMP_XSize
/* 通道标号图片的高度 */
#undef Sensor_Label_BMP_YSize
/* 显示界面1 */
#undef UI_1
/* 显示界面2 */
#undef UI_2
/* 显示界面3 */
#undef UI_3 
/* 显示界面4 */
#undef UI_4 
/* 显示界面5 */
#undef UI_5  
/* 显示界面6 */
#undef UI_6    
/* 显示界面7 */
#undef UI_7 
/* 显示界面8 */
#undef UI_8 
/* 显示界面9 */
#undef UI_9 
/* 显示界面10 */
#undef UI_10 
/* 显示界面11 */
#undef UI_11
/* 显示界面12 */
#undef UI_12 
/* 显示界面13 */
#undef UI_13
/* 显示界面14 */
#undef UI_14 
/* 显示界面15 */
#undef UI_15 
/* 显示界面16 */
#undef UI_16 
/* 显示界面17 */
#undef UI_17 
/* 显示界面18 */
#undef UI_18  
/* 显示界面19 */
#undef UI_19 
/* 显示界面20 */
#undef UI_20  
/* 显示界面21 */
#undef UI_21   
/* 显示界面22 */
#undef UI_22    
/* 显示界面23 */
#undef UI_23    
/* 显示界面24 */
#undef UI_24    
/* 显示界面25 */
#undef UI_25    
/* 显示界面26 */
#undef UI_26    
/* 显示界面27 */
#undef UI_27    
/* 显示界面28 */
#undef UI_28    
/* 显示界面29 */
#undef UI_29    
/* 显示界面30 */
#undef UI_30    
/* 显示界面31 */
#undef UI_31  
/* 显示界面32 */
#undef UI_32  
}

/************************************************************************************************/
/* 函数名	: DispDeskTopSonsorHumItem															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 湿度传感器项目显示																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
static void DispDeskTopSonsorHumItem(float display_data)
{
	u8 Inf_string[16]={0};
	u8 dis_data_point;


	/* 清0 指针数据 */
	dis_data_point = 0;

	/* 整合信息字符, */
	sprintf((char*)Inf_string,"%d.%01d%%",
	((u16)display_data),
	(((u16)(display_data*10))%10));

	/* 判断数据位数 */
	if(Inf_string[1] == '.')
	{
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 清除 百位 数据 */
		GUI_DispCharAt(' ',Humi_Sensor_Data_Dis_PosX				 ,Humi_Sensor_Data_Dis_PosY);
		/* 清除 十位 数据 */
		GUI_DispCharAt(' ',Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Humi_Sensor_Data_Dis_PosY);
		/* 显示 个位 数据 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Humi_Sensor_Data_Dis_PosY);
	}else if(Inf_string[2] == '.')
	{
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 清除 百位 数据 */
		GUI_DispCharAt(' ',Humi_Sensor_Data_Dis_PosX				 ,Humi_Sensor_Data_Dis_PosY);
		/* 显示整数温度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Humi_Sensor_Data_Dis_PosY);
		GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Humi_Sensor_Data_Dis_PosY);
	}else if(Inf_string[3] == '.')
	{
		/* 第3位数据为 小数点 :说明数据只有 3位数 */
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 显示整数温度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX,				   Humi_Sensor_Data_Dis_PosY);
		GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Humi_Sensor_Data_Dis_PosY);
		GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Humi_Sensor_Data_Dis_PosY);
	}

	/* 设置字体 */
	GUI_SetEnFont(En_12x24);
	/* 小数点 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*3,Humi_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	/* 显示小数温度值 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*4,Humi_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
	/* 显示 摄氏度 符号 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*4,Humi_Sensor_Data_Dis_PosY);

}

/************************************************************************************************/
/* 函数名	: DispDeskTopSonsorTmpItem															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 温度传感器项目显示																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
static void DispDeskTopSonsorTmpItem(float display_data)
{
	u8 Inf_string[16]={0};
	u8 dis_data_point;


	/* 清0 指针数据 */
	dis_data_point = 0;

	/* 整合信息字符, */
	sprintf((char*)Inf_string,"%c%d.%01d℃",
	((display_data >= 0.0) ? '+':'-'),	
	((u16)display_data),
	(((u16)(display_data*10))%10));

	/* 判断数据位数 */
	if(Inf_string[2] == '.')
	{
		/* 第2位数据为 小数点 :说明数据只有 1位数 */
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 清除 百位 数据 */
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY);
		/* 清除 十位 数据 */
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Temp_Sensor_Data_Dis_PosY);
		/* 设置字体 */
		GUI_SetEnFont(En_12x24);
		/* 显示温度 正负 号 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 显示整数温度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Temp_Sensor_Data_Dis_PosY);
	
	}else if(Inf_string[3] == '.')
	{
		/* 第3位数据为 小数点 :说明数据只有 2位数 */
		/* 设置字体 */
		GUI_SetEnFont(En_12x24);
		/* 清除百位数据 */
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY);
		/* 显示温度 正负 号 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 显示整数温度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Temp_Sensor_Data_Dis_PosY);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Temp_Sensor_Data_Dis_PosY);
	}else if(Inf_string[4] == '.')
	{
	 	/* 第4位数据为 小数点 :说明数据只有 3位数 */
		/* 设置字体 */
		GUI_SetEnFont(En_12x24);
		/* 显示 温度 正负 号 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY);
		/* 百位 数据显示 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 显示整数温度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Temp_Sensor_Data_Dis_PosY);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Temp_Sensor_Data_Dis_PosY);
	}

	/* 设置字体 */
	GUI_SetEnFont(En_12x24);
	/* 小数点 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*3,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	/* 显示小数温度值 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*4,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
	/* 显示 摄氏度 符号 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*4,Temp_Sensor_Data_Dis_PosY);
	GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*4+En_8x16_XSIZE*1,Temp_Sensor_Data_Dis_PosY);

}

/************************************************************************************************/
/* 函数名	: DispDeskTopSonsorCarbonDioxideItem															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 二氧化碳传感器项目显示																	*/
/************************************************************************************************/
static void DispDeskTopSonsorCarbonDioxideItem(float display_data)
{
	u8 Inf_string[6]={0};
	u8 dis_data_point;
	
	//printf("display_data=%05f\r\n",display_data);
	/* 整合信息字符, */
	sprintf((char*)Inf_string,"%05dPPM",((u16)display_data));
	
	/* 设置字体 */
	GUI_SetEnFont(En_12x24);
	
	/* 清0 指针数据 */
	dis_data_point = 0;

	/* 显示5位数值 */
	if(display_data>=10000)
	{
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*1,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*2,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*3,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*4,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	}
	/* 显示4位数值 */
	else if(display_data>=1000)
	{
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*1,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*2,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*3,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*4,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	}
	/* 显示3位数值 */
	else if(display_data>=100)
	{
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*2,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*3,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*4,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	}
	/* 显示2位数值 */
	else if(display_data>=10)
	{
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*3,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*4,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	}
	/* 显示1位数值 */
	else
	{
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		dis_data_point++;
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*4,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	}



	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
  /* 显示PPM */
	GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*5+En_8x16_XSIZE*1,Temp_Sensor_Data_Dis_PosY);
	GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*5+En_8x16_XSIZE*2,Temp_Sensor_Data_Dis_PosY);
	GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE*5+En_8x16_XSIZE*3,Temp_Sensor_Data_Dis_PosY);
}

/************************************************************************************************/
/* 函数名	: DispDeskTopSonsorDoorContactItem															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 门磁开关项目显示																	*/
/************************************************************************************************/
static void DispDeskTopSonsorDoorContactItem(float display_data)
{
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
	
	if(display_data>0.5)
	{
		//GUI_DispStringAt((u8 *)"OPEN",Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispStringAt((u8 *)"打开",Temp_Sensor_Data_Dis_PosX+3*En_8x16_XSIZE,Temp_Sensor_Data_Dis_PosY+En_8x16_YSIZE);
	}
	else
	{
		//GUI_DispStringAt((u8 *)"CLOSE",Temp_Sensor_Data_Dis_PosX+En_12x24_XSIZE,Temp_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
		GUI_DispStringAt((u8 *)"关闭",Temp_Sensor_Data_Dis_PosX+3*En_8x16_XSIZE,Temp_Sensor_Data_Dis_PosY+En_8x16_YSIZE);
	}
}


#if (Sensor_Spd_Num > 0x00)
/************************************************************************************************/
/* 函数名	: Disp_Desk_Top_Sonsor_Spd_Item															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示速度界面																	*/
/* 创建日期	: 2015/8/20																			*/
/************************************************************************************************/
static void Disp_Desk_Top_Sonsor_Spd_Item(float display_data)
{
	u8 Inf_string[16]={0};
	u8 dis_data_point;


	/* 清0 指针数据 */
	dis_data_point = 0;

	/* 整合信息字符, */
	sprintf((char*)Inf_string,"%d.%01dkm/h",
	((u16)display_data),
	(((u16)(display_data*10))%10));

	/* 判断数据位数 */
	if(Inf_string[1] == '.')
	{
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 清除 十位 数据 */
		GUI_DispCharAt(' ',Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Humi_Sensor_Data_Dis_PosY);
		/* 显示 个位 数据 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Humi_Sensor_Data_Dis_PosY);
	}else if(Inf_string[2] == '.')
	{
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 显示整数温度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Humi_Sensor_Data_Dis_PosY);
		GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Humi_Sensor_Data_Dis_PosY);
	}

	/* 设置字体 */
	GUI_SetEnFont(En_12x24);
	/* 小数点 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*3,Humi_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	/* 显示小数温度值 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_16x48_XSIZE*4,Humi_Sensor_Data_Dis_PosY+En_12x24_YSIZE);
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
	/* 显示 摄氏度 符号 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_8x16_XSIZE*8,Humi_Sensor_Data_Dis_PosY);
	GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_8x16_XSIZE*9,Humi_Sensor_Data_Dis_PosY);
	GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_8x16_XSIZE*10,Humi_Sensor_Data_Dis_PosY);
	GUI_DispCharAt(Inf_string[dis_data_point++],Humi_Sensor_Data_Dis_PosX+En_8x16_XSIZE*11,Humi_Sensor_Data_Dis_PosY);


}
#endif


#if (Sensor_Lon_Num > 0x00)
/************************************************************************************************/
/* 函数名	: Disp_Desk_Top_Sonsor_Lon_Item															*/
/* 输入		: 经度数据																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示经度界面																		*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
static void Disp_Desk_Top_Sonsor_Lon_Item(float display_data)
{
#define En_Font_XSIZE		(En_12x24_XSIZE)
#define En_Font_YSIZE		(En_12x24_YSIZE)
#define lon_dge_dis_PosX	(32u)
#define lon_dge_dis_PosY	(16u)
#define lon_min_dis_PosX	(lon_dge_dis_PosX + En_12x24_XSIZE*5)
#define lon_min_dis_PosY	(lon_dge_dis_PosY)
#define lon_scd_dis_PosX	(lon_dge_dis_PosX)
#define lon_scd_dis_PosY	(lon_dge_dis_PosY + En_Font_YSIZE)

	/* 经度字段信息 */
	char lon_string[20]={0};
	/* 字段信息指针 */
	u8 dis_data_point;
	/* 经度_度、经度_分 经度_秒 */
	float lon_dge,lon_min,lon_scd;
	/* 由于 在sprintf 使用%f存在数据对齐的问题，因此另外设置连个变量来代替这个值. */
	u8 lon_scd_dec,lon_scd_int;
	
	
	/* 清0 指针数据 */
	dis_data_point = 0;
	/* 转换经度数据 */
	/* 度 */
	lon_dge = (s16)display_data;
	/* 分 */
	lon_min = (s16)((display_data-lon_dge)*60);
	/* 秒 */
	lon_scd = ((display_data-lon_dge)*60-lon_min)*60;
	lon_scd_int = lon_scd;
	lon_scd_dec = (lon_scd-lon_scd_int)*100;
	
	/* 判断数据是否为负数 */
	if(lon_scd < 0.0)
	{
		/* 转换成整数 */
		lon_scd *= -1;
	}
	
	/* 整合信息字符, */
	sprintf((char*)lon_string,"%d°%01d'%02d.%02d\"%c",
	((u16)lon_dge),
	((u16)lon_min),
	(lon_scd_int),
	(lon_scd_dec),
	((display_data < 0.0)?'W':'E')
	);

	/* 设置字体 */
	GUI_SetEnFont(En_12x24);
	/* 判断数据位数 */
	if((lon_dge < 10)&&(lon_dge > (-10)))
	{
		/* 清除 百位 数据 */
		GUI_DispCharAt(' ',lon_dge_dis_PosX,lon_dge_dis_PosY);
		/* 清除 十位 数据 */
		GUI_DispCharAt(' ',lon_dge_dis_PosX+En_Font_XSIZE*1,lon_dge_dis_PosY);
		/* 显示 个位 数据 */
		GUI_DispCharAt(lon_string[dis_data_point++],lon_dge_dis_PosX+En_Font_XSIZE*2,lon_dge_dis_PosY);
	}else if((lon_dge < 100)&&(lon_dge > (-100)))
	{
		/* 清除 百位 数据 */
		GUI_DispCharAt(' ',lon_dge_dis_PosX,lon_dge_dis_PosY);
		/* 显示 十位 数据 */
		GUI_DispCharAt(lon_string[dis_data_point++],lon_dge_dis_PosX+En_Font_XSIZE*1,lon_dge_dis_PosY);
		/* 显示 个位 数据 */
		GUI_DispCharAt(lon_string[dis_data_point++],lon_dge_dis_PosX+En_Font_XSIZE*2,lon_dge_dis_PosY);
	}else
	{

		/* 百位 数据显示 */
		GUI_DispCharAt(lon_string[dis_data_point++],lon_dge_dis_PosX,lon_dge_dis_PosY);
		/* 显示 十位 数据 */
		GUI_DispCharAt(lon_string[dis_data_point++],lon_dge_dis_PosX+En_Font_XSIZE*1,lon_dge_dis_PosY);
		/* 显示 个位 数据 */
		GUI_DispCharAt(lon_string[dis_data_point++],lon_dge_dis_PosX+En_Font_XSIZE*2,lon_dge_dis_PosY);
	}

	/* 显示'°'符号。 */
	GUI_DispCharAt(lon_string[dis_data_point++],lon_dge_dis_PosX+En_Font_XSIZE*3,lon_dge_dis_PosY);
	GUI_DispCharAt(lon_string[dis_data_point++],lon_dge_dis_PosX+En_Font_XSIZE*4,lon_dge_dis_PosY);
	/* 显示 分 经度值 */
	if((lon_min < 10)&&(lon_min > (-10)))
	{
		GUI_DispCharAt('0',lon_min_dis_PosX,lon_min_dis_PosY);
		GUI_DispCharAt(lon_string[dis_data_point++],lon_min_dis_PosX+En_Font_XSIZE*1,lon_min_dis_PosY);
	}else
	{
		GUI_DispCharAt(lon_string[dis_data_point++],lon_min_dis_PosX,lon_min_dis_PosY);
		GUI_DispCharAt(lon_string[dis_data_point++],lon_min_dis_PosX+En_Font_XSIZE*1,lon_min_dis_PosY);
	}
	/* 显示'''符号。 */
	GUI_DispCharAt(lon_string[dis_data_point++],lon_min_dis_PosX+En_Font_XSIZE*2,lon_min_dis_PosY);
	
	/* 显示秒数据 */
	if((lon_scd < 10)&&(lon_scd > (-10)))
	{
		GUI_DispCharAt('0',lon_scd_dis_PosX,lon_scd_dis_PosY);
		GUI_DispCharAt(lon_string[dis_data_point++],lon_scd_dis_PosX+En_Font_XSIZE*1,lon_scd_dis_PosY);
	}else
	{
		GUI_DispCharAt(lon_string[dis_data_point++],lon_scd_dis_PosX,lon_scd_dis_PosY);
		GUI_DispCharAt(lon_string[dis_data_point++],lon_scd_dis_PosX+En_Font_XSIZE*1,lon_scd_dis_PosY);
	}
	GUI_DispCharAt(lon_string[dis_data_point++],lon_scd_dis_PosX+En_Font_XSIZE*2,lon_scd_dis_PosY);
	GUI_DispCharAt(lon_string[dis_data_point++],lon_scd_dis_PosX+En_Font_XSIZE*3,lon_scd_dis_PosY);
	GUI_DispCharAt(lon_string[dis_data_point++],lon_scd_dis_PosX+En_Font_XSIZE*4,lon_scd_dis_PosY);
	GUI_DispCharAt(lon_string[dis_data_point++],lon_scd_dis_PosX+En_Font_XSIZE*5,lon_scd_dis_PosY);
	GUI_DispCharAt(lon_string[dis_data_point++],lon_scd_dis_PosX+En_Font_XSIZE*6,lon_scd_dis_PosY);

#undef En_Font_XSIZE
#undef En_Font_YSIZE
#undef lon_dge_dis_PosX	
#undef lon_dge_dis_PosY	
#undef lon_min_dis_PosX
#undef lon_min_dis_PosY
#undef lon_scd_dis_PosX
#undef lon_scd_dis_PosY

}
#endif

#if (Sensor_Lat_Num > 0x00)
/************************************************************************************************/
/* 函数名	: Disp_Desk_Top_Sonsor_Lat_Item															*/
/* 输入		: 纬度数据																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示纬度界面																		*/
/* 创建日期	: 2015/8/24																			*/
/************************************************************************************************/
static void Disp_Desk_Top_Sonsor_Lat_Item(float display_data)
{
#define En_Font_XSIZE		(En_12x24_XSIZE)
#define En_Font_YSIZE		(En_12x24_YSIZE)
#define lat_dge_dis_PosX	(32u)
#define lat_dge_dis_PosY	(16u)
#define lat_min_dis_PosX	(lat_dge_dis_PosX + En_12x24_XSIZE*5)
#define lat_min_dis_PosY	(lat_dge_dis_PosY)
#define lat_scd_dis_PosX	(lat_dge_dis_PosX)
#define lat_scd_dis_PosY	(lat_dge_dis_PosY + En_Font_YSIZE)

	/* 经度字段信息 */
	char lat_string[20]={0};
	/* 字段信息指针 */
	u8 dis_data_point;
	/* 经度_度、经度_分 经度_秒 */
	float lat_dge,lat_min,lat_scd;
	/* 由于 在sprintf 使用%f存在数据对齐的问题，因此另外设置连个变量来代替这个值. */
	u8 lat_scd_dec,lat_scd_int;
	
	
	/* 清0 指针数据 */
	dis_data_point = 0;
	/* 转换经度数据 */
	/* 度 */
	lat_dge = (s16)display_data;
	/* 分 */
	lat_min = (s16)((display_data-lat_dge)*60);
	/* 秒 */
	lat_scd = ((display_data-lat_dge)*60-lat_min)*60;
	lat_scd_int = lat_scd;
	lat_scd_dec = (lat_scd-lat_scd_int)*100;
	
	/* 判断数据是否为负数 */
	if(lat_scd < 0.0)
	{
		/* 转换成整数 */
		lat_scd *= -1;
	}
	
	/* 整合信息字符, */
	sprintf((char*)lat_string,"%d°%01d'%02d.%02d\"%c",
	((u16)lat_dge),
	((u16)lat_min),
	(lat_scd_int),
	(lat_scd_dec),
	((display_data < 0.0)?'S':'N')
	);

	/* 设置字体 */
	GUI_SetEnFont(En_12x24);
	/* 判断数据位数 */
	if((lat_dge < 10)&&(lat_dge > (-10)))
	{
		/* 清除 百位 数据 */
		GUI_DispCharAt(' ',lat_dge_dis_PosX,lat_dge_dis_PosY);
		/* 清除 十位 数据 */
		GUI_DispCharAt(' ',lat_dge_dis_PosX+En_Font_XSIZE*1,lat_dge_dis_PosY);
		/* 显示 个位 数据 */
		GUI_DispCharAt(lat_string[dis_data_point++],lat_dge_dis_PosX+En_Font_XSIZE*2,lat_dge_dis_PosY);
	}else if((lat_dge < 100)&&(lat_dge > (-100)))
	{
		/* 清除 百位 数据 */
		GUI_DispCharAt(' ',lat_dge_dis_PosX,lat_dge_dis_PosY);
		/* 显示 十位 数据 */
		GUI_DispCharAt(lat_string[dis_data_point++],lat_dge_dis_PosX+En_Font_XSIZE*1,lat_dge_dis_PosY);
		/* 显示 个位 数据 */
		GUI_DispCharAt(lat_string[dis_data_point++],lat_dge_dis_PosX+En_Font_XSIZE*2,lat_dge_dis_PosY);
	}else
	{

		/* 百位 数据显示 */
		GUI_DispCharAt(lat_string[dis_data_point++],lat_dge_dis_PosX,lat_dge_dis_PosY);
		/* 显示 十位 数据 */
		GUI_DispCharAt(lat_string[dis_data_point++],lat_dge_dis_PosX+En_Font_XSIZE*1,lat_dge_dis_PosY);
		/* 显示 个位 数据 */
		GUI_DispCharAt(lat_string[dis_data_point++],lat_dge_dis_PosX+En_Font_XSIZE*2,lat_dge_dis_PosY);
	}

	/* 显示'°'符号。 */
	GUI_DispCharAt(lat_string[dis_data_point++],lat_dge_dis_PosX+En_Font_XSIZE*3,lat_dge_dis_PosY);
	GUI_DispCharAt(lat_string[dis_data_point++],lat_dge_dis_PosX+En_Font_XSIZE*4,lat_dge_dis_PosY);
	/* 显示 分 经度值 */
	if((lat_min < 10)&&(lat_min > (-10)))
	{
		GUI_DispCharAt('0',lat_min_dis_PosX,lat_min_dis_PosY);
		GUI_DispCharAt(lat_string[dis_data_point++],lat_min_dis_PosX+En_Font_XSIZE*1,lat_min_dis_PosY);
	}else
	{
		GUI_DispCharAt(lat_string[dis_data_point++],lat_min_dis_PosX,lat_min_dis_PosY);
		GUI_DispCharAt(lat_string[dis_data_point++],lat_min_dis_PosX+En_Font_XSIZE*1,lat_min_dis_PosY);
	}
	/* 显示'''符号。 */
	GUI_DispCharAt(lat_string[dis_data_point++],lat_min_dis_PosX+En_Font_XSIZE*2,lat_min_dis_PosY);
	
	/* 显示秒数据 */
	if((lat_scd < 10)&&(lat_scd > (-10)))
	{
		GUI_DispCharAt('0',lat_scd_dis_PosX,lat_scd_dis_PosY);
		GUI_DispCharAt(lat_string[dis_data_point++],lat_scd_dis_PosX+En_Font_XSIZE*1,lat_scd_dis_PosY);
	}else
	{
		GUI_DispCharAt(lat_string[dis_data_point++],lat_scd_dis_PosX,lat_scd_dis_PosY);
		GUI_DispCharAt(lat_string[dis_data_point++],lat_scd_dis_PosX+En_Font_XSIZE*1,lat_scd_dis_PosY);
	}
	GUI_DispCharAt(lat_string[dis_data_point++],lat_scd_dis_PosX+En_Font_XSIZE*2,lat_scd_dis_PosY);
	GUI_DispCharAt(lat_string[dis_data_point++],lat_scd_dis_PosX+En_Font_XSIZE*3,lat_scd_dis_PosY);
	GUI_DispCharAt(lat_string[dis_data_point++],lat_scd_dis_PosX+En_Font_XSIZE*4,lat_scd_dis_PosY);
	GUI_DispCharAt(lat_string[dis_data_point++],lat_scd_dis_PosX+En_Font_XSIZE*5,lat_scd_dis_PosY);
	GUI_DispCharAt(lat_string[dis_data_point++],lat_scd_dis_PosX+En_Font_XSIZE*6,lat_scd_dis_PosY);


#undef En_Font_XSIZE
#undef En_Font_YSIZE
#undef lat_dge_dis_PosX	
#undef lat_dge_dis_PosY	
#undef lat_min_dis_PosX
#undef lat_min_dis_PosY
#undef lat_scd_dis_PosX
#undef lat_scd_dis_PosY

}
#endif




#if (Gateway_NJSY_Enable == 0x01)
/************************************************************************************************/
/* 函数名	: DispDeskTopGWRSSIItem															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 温度传感器项目显示																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
static void DispDeskTopGWRSSIItem(float rssi)
{
	u8 Inf_string[16]={0};
	u8 dis_data_point = 0;
	uint8_t	mStringLen;


	/* 整合信息字符, */
	sprintf((char*)Inf_string,"%dRF",((uint8_t)rssi));

	/* 计算字符串长度 */
	mStringLen = strlen((char*)Inf_string);
	
	/* 判断数据位数 */
	if(mStringLen == 3)
	{
		/* 有1位数据 */
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 清除 百位 数据 */
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY);
		/* 清除 十位 数据 */
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Temp_Sensor_Data_Dis_PosY);
		/* 显示个位信号强度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Temp_Sensor_Data_Dis_PosY);
	
	}else if(mStringLen == 4)
	{
		/* 有2位数据 */
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 清除百位数据 */
		GUI_DispCharAt(' ',Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY);
		/* 显示十位信号强度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Temp_Sensor_Data_Dis_PosY);
		/* 显示个位信号强度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Temp_Sensor_Data_Dis_PosY);
	}else if(mStringLen == 5)
	{
	 	/* 第4位数据为 小数点 :说明数据只有 3位数 */
		/* 设置字体 */
		GUI_SetEnFont(En_16x48);
		/* 显示百位信号强度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY);
		/* 显示十位信号强度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*1,Temp_Sensor_Data_Dis_PosY);
		/* 显示个位信号强度值 */
		GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*2,Temp_Sensor_Data_Dis_PosY);
	}else
	{
	
	}

	GUI_SetEnFont(En_8x16);
	/* 显示 摄氏度 符号 */
	GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*4,Temp_Sensor_Data_Dis_PosY);
	GUI_DispCharAt(Inf_string[dis_data_point++],Temp_Sensor_Data_Dis_PosX+En_16x48_XSIZE*4+En_8x16_XSIZE*1,Temp_Sensor_Data_Dis_PosY);

}
#endif

#if (Gateway_NJSY_Enable == 0x01)
/* 网关工作模式数据指针 */
#define MenuGWModeDatap (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.Mode)
#endif
/************************************************************************************************/
/* 函数名	: DispDeskTopSonsorItem																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 待机界面下显示传感器项目																*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
static void DispDeskTopSonsorItem(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
#if ((Wireless_RF_Card_Config_Enable == 0x01)||\
	 (GPS_Config_Enable == 0x01)||\
	 (Gateway_NJSY_Enable == 0x01)||\
	 (Gateway_HZZH_Enable == 0x01))
	static BOOL update_dis = FALSE;
#endif
	
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
	/* 分割线 */
	GUI_DispCharAt('|',Sensor_ICO_PosX+Sensor_ICO_XSize,Line_2_PosY);
	GUI_DispCharAt('|',Sensor_ICO_PosX+Sensor_ICO_XSize,Line_3_PosY);
	GUI_DispCharAt('|',Sensor_ICO_PosX+Sensor_ICO_XSize,Line_4_PosY);

	/* 根据传感器类型，做对应显示 */
	switch(sensor_t->Type)
	{
		case Sensor_Null:
		{
		
			break;
		}
		case Sensor_Tmp:
		{
			/* 显示温度传感器的类型 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_Tmp],Sensor_ICO_PosX,Sensor_ICO_PosY);
			DispDeskTopSonsorTmpItem(sensor_f->Sensor_Real_Time_Data);
			break;
		}
		
		case Sensor_CarbonDioxide:
		{
			/* 显示二氧化碳传感器的类型 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_CarbonDioxide],Sensor_ICO_PosX,Sensor_ICO_PosY);
			DispDeskTopSonsorCarbonDioxideItem(sensor_f->Sensor_Real_Time_Data);
			break;
		}
		case Sensor_DoorContact:
		{
			/* 门磁开关传感器 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_DoorContact],Sensor_ICO_PosX,Sensor_ICO_PosY);
			DispDeskTopSonsorDoorContactItem(sensor_f->Sensor_Real_Time_Data);
			break;
		}
		case Sensor_PT1000:
		{
			/* 显示温度传感器的类型 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_PT1000],Sensor_ICO_PosX,Sensor_ICO_PosY);
			DispDeskTopSonsorTmpItem(sensor_f->Sensor_Real_Time_Data);
			break;
		}
		case Sensor_Hum:
		{
			/* 显示湿度传感器的类型 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_Hum],Sensor_ICO_PosX,Sensor_ICO_PosY);			
			DispDeskTopSonsorHumItem(sensor_f->Sensor_Real_Time_Data);
			break;
		}
#if (Wireless_RF_Card_Config_Enable == 0x01)
#if ((Sensor_RF_Single_Tmp_Num > 0x00) || (Sensor_RF_Humiture_Tmp_Num > 0x00))
		case Sensor_RF_Card_Single_Tmp:
		{
			/* 显示卡片图标 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_RF_Card_Single_Tmp],Sensor_ICO_PosX,Sensor_ICO_PosY);
			
			/* 清屏状态标志 */
			if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para))
			{
				update_dis = FALSE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != Wireless_RF_Card_Data_Rece_Timeout_Para))
			{
				update_dis = TRUE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}
			
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para) 
			{
				/* 卡片接收超时 */
				DispDeskTopSonsorRFCardTimeoutItem();
			}else
			{
				/* 显示温度数据 */
				DispDeskTopSonsorTmpItem(sensor_f->Sensor_Real_Time_Data);
			}
			
			break;
		}
		case Sensor_RF_Card_Humiture_Tmp:
		{
			/* 显示卡片图标 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_RF_Card_Humiture_Tmp],Sensor_ICO_PosX,Sensor_ICO_PosY);
			
			/* 清屏状态标志 */
			if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para))
			{
				update_dis = FALSE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != Wireless_RF_Card_Data_Rece_Timeout_Para))
			{
				update_dis = TRUE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}
			
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				/* 卡片接收超时 */
				DispDeskTopSonsorRFCardTimeoutItem();
			}else
			{
				/* 显示温度数据 */
				DispDeskTopSonsorTmpItem(sensor_f->Sensor_Real_Time_Data);
			}
			break;
		}
#endif
#if (Sensor_RF_Humiture_Hum_Num > 0x00)
		case Sensor_RF_Card_Humiture_Hum:
		{
			/* 显示卡片图标 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_RF_Card_Humiture_Hum],Sensor_ICO_PosX,Sensor_ICO_PosY);	
			
			/* 清屏状态标志 */
			if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para))
			{
				update_dis = FALSE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != Wireless_RF_Card_Data_Rece_Timeout_Para))
			{
				update_dis = TRUE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}
			
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				/* 卡片接收超时 */
				DispDeskTopSonsorRFCardTimeoutItem();
			}else
			{
				/* 显示湿度数据 */
				DispDeskTopSonsorHumItem(sensor_f->Sensor_Real_Time_Data);
			}
			break;
		}
#endif
#endif
	
#if (GPS_Config_Enable == 0x01)		
#if (Sensor_Lon_Num > 0x00)
case Sensor_GPS_POS_Lon:
	{
		/* 显示卡片图标 */
		GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_GPS_POS_Lon],Sensor_ICO_PosX,Sensor_ICO_PosY);	
		
		/* 清屏状态标志 */
		if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == GPS_Data_Rece_Timeout_Value))
		{
			update_dis = FALSE;
			/* 清除数据显示部分 */
			TipClr(Lon_Sensor_Data_Dis_PosX,Lon_Sensor_Data_Dis_PosY,Lon_Sensor_Data_Dis_XSize,Lon_Sensor_Data_Dis_YSize);
		}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != GPS_Data_Rece_Timeout_Value))
		{
			update_dis = TRUE;
			/* 清除数据显示部分 */
			TipClr(Lon_Sensor_Data_Dis_PosX,Lon_Sensor_Data_Dis_PosY,Lon_Sensor_Data_Dis_XSize,Lon_Sensor_Data_Dis_YSize);
		}
		
		if(sensor_f->Sensor_Real_Time_Data == GPS_Data_Rece_Timeout_Value)
		{
			/* 卡片接收超时 */
			Disp_Desk_Top_Sonsor_Lon_Timeout_Item();
		}else
		{
			/* 显示经度数据 */
			Disp_Desk_Top_Sonsor_Lon_Item(sensor_f->Sensor_Real_Time_Data);
		}
		break;
	}
#endif
#if (Sensor_Lat_Num > 0x00)
case Sensor_GPS_POS_Lat:
	{
		/* 显示卡片图标 */
		GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_GPS_POS_Lat],Sensor_ICO_PosX,Sensor_ICO_PosY);	
		
		/* 清屏状态标志 */
		if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == GPS_Data_Rece_Timeout_Value))
		{
			update_dis = FALSE;
			/* 清除数据显示部分 */
			TipClr(Lat_Sensor_Data_Dis_PosX,Lat_Sensor_Data_Dis_PosY,Lat_Sensor_Data_Dis_XSize,Lat_Sensor_Data_Dis_YSize);
		}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != GPS_Data_Rece_Timeout_Value))
		{
			update_dis = TRUE;
			/* 清除数据显示部分 */
			TipClr(Lat_Sensor_Data_Dis_PosX,Lat_Sensor_Data_Dis_PosY,Lat_Sensor_Data_Dis_XSize,Lat_Sensor_Data_Dis_YSize);
		}
		
		if(sensor_f->Sensor_Real_Time_Data == GPS_Data_Rece_Timeout_Value)
		{
			/* 卡片接收超时 */
			Disp_Desk_Top_Sonsor_Lat_Timeout_Item();
		}else
		{
			/* 显示纬度数据 */
			Disp_Desk_Top_Sonsor_Lat_Item(sensor_f->Sensor_Real_Time_Data);
		}
		break;
	}
#endif
#if (Sensor_Spd_Num > 0x00)
case Sensor_GPS_POS_Spd:
	{
		/* 显示卡片图标 */
		GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_GPS_POS_Spd],Sensor_ICO_PosX,Sensor_ICO_PosY);	
		
		/* 清屏状态标志 */
		if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == GPS_Data_Rece_Timeout_Value))
		{
			update_dis = FALSE;
			/* 清除数据显示部分 */
			TipClr(Spd_Sensor_Data_Dis_PosX,Spd_Sensor_Data_Dis_PosY,Spd_Sensor_Data_Dis_XSize,Spd_Sensor_Data_Dis_YSize);
		}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != GPS_Data_Rece_Timeout_Value))
		{
			update_dis = TRUE;
			/* 清除数据显示部分 */
			TipClr(Spd_Sensor_Data_Dis_PosX,Spd_Sensor_Data_Dis_PosY,Spd_Sensor_Data_Dis_XSize,Spd_Sensor_Data_Dis_YSize);
		}
		
		if(sensor_f->Sensor_Real_Time_Data == GPS_Data_Rece_Timeout_Value)
		{
			/* 接收超时 */
			Disp_Desk_Top_Sonsor_Spd_Timeout_Item();
		}else
		{
			/* 显示速度数据 */
			Disp_Desk_Top_Sonsor_Spd_Item(sensor_f->Sensor_Real_Time_Data);
		}
		break;
	}
#endif
#endif
	
#if (Gateway_NJSY_Enable == 0x01)
#if (Sensor_GW_NJSY_Tmp_Num > 0x00)
		case Sensor_GW_NJSY_Tmp:
		{
			/* 显示卡片图标 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_GW_NJSY_Tmp],Sensor_ICO_PosX,Sensor_ICO_PosY);	
			
			/* 清屏状态标志 */
			if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == Gateway_NJSY_TimeoutPara))
			{
				update_dis = FALSE;
				/* 清除数据显示部分 */
				TipClr(Spd_Sensor_Data_Dis_PosX,Spd_Sensor_Data_Dis_PosY,Spd_Sensor_Data_Dis_XSize,Spd_Sensor_Data_Dis_YSize);
			}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != Gateway_NJSY_TimeoutPara))
			{
				update_dis = TRUE;
				/* 清除数据显示部分 */
				TipClr(Spd_Sensor_Data_Dis_PosX,Spd_Sensor_Data_Dis_PosY,Spd_Sensor_Data_Dis_XSize,Spd_Sensor_Data_Dis_YSize);
			}
			
			if(sensor_f->Sensor_Real_Time_Data == Gateway_NJSY_TimeoutPara)
			{
				/* 网关数据无效(在规定时间内没有收到对应探头数据) */
				DispDeskTopSonsorRFCardTimeoutItem();
			}else
			{
				/* 判断当前网关工作模式 */
				if(0x00 == *MenuGWModeDatap)
				{
					/* 正常模式，显示数据 */
					DispDeskTopSonsorTmpItem(sensor_f->Sensor_Real_Time_Data);
				}else if(0x01 == *MenuGWModeDatap)
				{
					/* 调试模式，显示信号强度 */
					DispDeskTopGWRSSIItem(sensor_f->Sensor_Real_Time_Data);
				}
			}
			break;
		}
#endif	
		
#if (Sensor_GW_NJSY_Hum_Num > 0x00)
		case Sensor_GW_NJSY_Hum:
		{
			/* 显示卡片图标 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_GW_NJSY_Hum],Sensor_ICO_PosX,Sensor_ICO_PosY);	
			
			/* 清屏状态标志 */
			if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == Gateway_NJSY_TimeoutPara))
			{
				update_dis = FALSE;
				/* 清除数据显示部分 */
				TipClr(Spd_Sensor_Data_Dis_PosX,Spd_Sensor_Data_Dis_PosY,Spd_Sensor_Data_Dis_XSize,Spd_Sensor_Data_Dis_YSize);
			}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != Gateway_NJSY_TimeoutPara))
			{
				update_dis = TRUE;
				/* 清除数据显示部分 */
				TipClr(Spd_Sensor_Data_Dis_PosX,Spd_Sensor_Data_Dis_PosY,Spd_Sensor_Data_Dis_XSize,Spd_Sensor_Data_Dis_YSize);
			}
			
			if(sensor_f->Sensor_Real_Time_Data == Gateway_NJSY_TimeoutPara)
			{
				/* 网关数据无效(在规定时间内没有收到对应探头数据) */
				DispDeskTopSonsorRFCardTimeoutItem();
			}else
			{
				/* 判断当前网关工作模式 */
				if(0x00 == *MenuGWModeDatap)
				{
					/* 正常模式，显示数据 */
					DispDeskTopSonsorHumItem(sensor_f->Sensor_Real_Time_Data);
				}else if(0x01 == *MenuGWModeDatap)
				{
					/* 调试模式，显示信号强度 */
					DispDeskTopGWRSSIItem(sensor_f->Sensor_Real_Time_Data);
				}
			}
			break;
		}
#endif		
#endif
		
#if (Gateway_HZZH_Enable == 0x01)
#if (Sensor_GW_HZZH_Tmp_Num > 0x00)
		case Sensor_GW_HZZH_Tmp:
		{
			/* 显示卡片图标 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_GW_HZZH_Tmp],Sensor_ICO_PosX,Sensor_ICO_PosY);
			
			/* 清屏状态标志 */
			if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == Gateway_HZZH_TimeoutPara))
			{
				update_dis = FALSE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != Gateway_HZZH_TimeoutPara))
			{
				update_dis = TRUE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}
			
			if(sensor_f->Sensor_Real_Time_Data == Gateway_HZZH_TimeoutPara)
			{
				/* 卡片接收超时 */
				DispDeskTopSonsorRFCardTimeoutItem();
			}else
			{
				/* 显示温度数据 */
				DispDeskTopSonsorTmpItem(sensor_f->Sensor_Real_Time_Data);
			}
			break;
		}
#endif
#if (Sensor_GW_HZZH_Hum_Num > 0x00)
		case Sensor_GW_HZZH_Hum:
		{
			/* 显示卡片图标 */
			GUI_Draw_Sensor_ICO(Sensor_Type_Index[Sensor_GW_HZZH_Hum],Sensor_ICO_PosX,Sensor_ICO_PosY);
			
			/* 清屏状态标志 */
			if((update_dis == TRUE)&&(sensor_f->Sensor_Real_Time_Data == Gateway_HZZH_TimeoutPara))
			{
				update_dis = FALSE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}else if((update_dis == FALSE)&&(sensor_f->Sensor_Real_Time_Data != Wireless_RF_Card_Data_Rece_Timeout_Para))
			{
				update_dis = TRUE;
				/* 清除数据显示部分 */
				TipClr(Temp_Sensor_Data_Dis_PosX,Temp_Sensor_Data_Dis_PosY,Sensor_Data_Dis_XSize,Sensor_Data_Dis_YSize);
			}
			
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				/* 卡片接收超时 */
				DispDeskTopSonsorRFCardTimeoutItem();
			}else
			{
				/* 显示湿度数据 */
				DispDeskTopSonsorHumItem(sensor_f->Sensor_Real_Time_Data);
			}
			break;
		}
#endif
#endif
		
        default:
		{
			break;
        }
	}
}

/************************************************************************************************/
/* 函数名	: DispDeskTopTimeItem																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 待机界面下显示日期时间项目															*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
static void DispDeskTopTimeItem(void)
{
	/* 保存时间字符串信息数组 */
	u8 times_string[16]={0};


	/* 整合时间字符串 */
	sprintf((char*)times_string,"%02d-%02d%02d:%02d:%02d",
	System_Time.Month,
	System_Time.Day,
	System_Time.Hour,
	System_Time.Min,
	System_Time.Sec);

	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
	GUI_DispCharAt(times_string[0],System_Date_String_PosX+En_8x16_XSIZE*0,System_Times_String_PosY);
	GUI_DispCharAt(times_string[1],System_Date_String_PosX+En_8x16_XSIZE*1,System_Times_String_PosY);
	GUI_DispCharAt(times_string[2],System_Date_String_PosX+En_8x16_XSIZE*2,System_Times_String_PosY);
	GUI_DispCharAt(times_string[3],System_Date_String_PosX+En_8x16_XSIZE*3,System_Times_String_PosY);
	GUI_DispCharAt(times_string[4],System_Date_String_PosX+En_8x16_XSIZE*4,System_Times_String_PosY);
	/* 设置字体 */
	GUI_SetEnFont(En_16x48);
	/* 显示字符串 */
	GUI_DispCharAt(times_string[5],System_Times_String_PosX+En_16x48_XSIZE*0,System_Times_String_PosY);
	GUI_DispCharAt(times_string[6],System_Times_String_PosX+En_16x48_XSIZE*1,System_Times_String_PosY);
	GUI_DispCharAt(times_string[7],System_Times_String_PosX+En_16x48_XSIZE*2,System_Times_String_PosY);
	GUI_DispCharAt(times_string[8],System_Times_String_PosX+En_16x48_XSIZE*3,System_Times_String_PosY);
	GUI_DispCharAt(times_string[9],System_Times_String_PosX+En_16x48_XSIZE*4,System_Times_String_PosY);
	/* 设置字体 */
	GUI_SetEnFont(En_12x24);
	GUI_DispCharAt(times_string[10],System_Times_String_PosX+En_16x48_XSIZE*5+En_12x24_XSIZE*0,System_Times_String_PosY+En_12x24_YSIZE);
	GUI_DispCharAt(times_string[11],System_Times_String_PosX+En_16x48_XSIZE*5+En_12x24_XSIZE*1,System_Times_String_PosY+En_12x24_YSIZE);
	GUI_DispCharAt(times_string[12],System_Times_String_PosX+En_16x48_XSIZE*5+En_12x24_XSIZE*2,System_Times_String_PosY+En_12x24_YSIZE);
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
}
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)
/*******************显示界面升级进度百分比界面***********************/
static void DispDeskTopUpdate(void)
{
	/* 保存时间字符串信息数组 */
	u8 times_string[16]={0};	
	u8   UpdateProgress=0;
	/*升级进度字符串信息数组 */
	u8 upgrade_progress_string[6]={0};
	/*升级提示信息字符串信息数组 */
	u8 Update_string[32]={0};
	
	 /*获取升级进度百分百信息 */
	UpdateProgress=IAP_App.UpdatePercent;
	
	/* 整合时间字符串 */
	sprintf((char*)upgrade_progress_string,"%03d",UpdateProgress);

	/* 整合时间字符串 */
	sprintf((char*)times_string,"%02d-%02d%02d:%02d:%02d",
	System_Time.Month,
	System_Time.Day,
	System_Time.Hour,
	System_Time.Min,
	System_Time.Sec);

	/* 设置字体 */
	GUI_SetEnFont(En_8x16);//设置"月/日"字体大小
	GUI_DispCharAt(times_string[0],System_Date_String_PosX+En_8x16_XSIZE*0,System_Times_String_PosY);
	GUI_DispCharAt(times_string[1],System_Date_String_PosX+En_8x16_XSIZE*1,System_Times_String_PosY);
	GUI_DispCharAt(times_string[2],System_Date_String_PosX+En_8x16_XSIZE*2,System_Times_String_PosY);
	GUI_DispCharAt(times_string[3],System_Date_String_PosX+En_8x16_XSIZE*3,System_Times_String_PosY);
	GUI_DispCharAt(times_string[4],System_Date_String_PosX+En_8x16_XSIZE*4,System_Times_String_PosY);
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);//设置"时:分"字体大小
	/* 显示字符串 */
	GUI_DispCharAt(times_string[5],System_Times_String_PosX+En_8x16_XSIZE*0,System_Times_String_PosY);
	GUI_DispCharAt(times_string[6],System_Times_String_PosX+En_8x16_XSIZE*1,System_Times_String_PosY);
	GUI_DispCharAt(times_string[7],System_Times_String_PosX+En_8x16_XSIZE*2,System_Times_String_PosY);
	GUI_DispCharAt(times_string[8],System_Times_String_PosX+En_8x16_XSIZE*3,System_Times_String_PosY);
	GUI_DispCharAt(times_string[9],System_Times_String_PosX+En_8x16_XSIZE*4,System_Times_String_PosY);
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);//设置"秒"字体大小
	GUI_DispCharAt(times_string[10],System_Times_String_PosX+En_8x16_XSIZE*5,System_Times_String_PosY);
	GUI_DispCharAt(times_string[11],System_Times_String_PosX+En_8x16_XSIZE*6,System_Times_String_PosY);
	GUI_DispCharAt(times_string[12],System_Times_String_PosX+En_8x16_XSIZE*7,System_Times_String_PosY);
	
	
		/* 设置字体 */
	GUI_SetEnFont(En_8x16);
		/* 整合信息字符 */
	sprintf((char*)Update_string,"系统正在");
		/* 显示提示 */
	GUI_DispStringAt(Update_string,0,32);
			/* 整合信息字符 */
	sprintf((char*)Update_string,"升级....");
		/* 显示提示 */
	GUI_DispStringAt(Update_string,0,48);

	/* 设置字体 */
	GUI_SetEnFont(En_12x24);
	if(UpdateProgress>=100)
  {
	GUI_DispCharAt(upgrade_progress_string[0],System_Times_String_PosX+En_8x16_XSIZE*8,36);
  }
	if(UpdateProgress>=10)
	{
  GUI_DispCharAt(upgrade_progress_string[1],System_Times_String_PosX+En_8x16_XSIZE*10,36);
	}
 
 	GUI_DispCharAt(upgrade_progress_string[2],System_Times_String_PosX+En_8x16_XSIZE*12,36);
 

		/* 设置字体 */
	GUI_SetEnFont(En_12x24);
  GUI_DispCharAt('%',System_Times_String_PosX+En_8x16_XSIZE*14,36);
	
}
#endif
#endif





/************************************************************************************************/
/* 函数名	: DispMenuEasyPrintItem																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示单前项目信息																	*/
/* 创建日期	: 2016/04/21																			*/
/************************************************************************************************/ 
static void DispMenuEasyPrintItem()
{
	/* 保存时间字符串信息数组 */
	u8 string[16]={0};
	/* 快捷打印剩余时间 */
	u32 Out_Time;
	
	GUI_DrawIcon(Ico_Index[4],48,16);
	
	/* 整合信息字符 */
	sprintf((char*)string,"继续");
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
	/* 显示提示 */
	GUI_DispStringAt(string,0,48);
	/* 整合信息字符 */
	sprintf((char*)string,"退出");
	/* 显示提示 */
	GUI_DispStringAt(string,96,48);
	
	
	/* 清除显示数据 */
	memset(string,'\0',sizeof(string));
	
	/* 获得快捷打印剩余时间 */
	Out_Time = Easy_Print_Get_Out_Time();
	/* 设置字体 */
	GUI_SetEnFont(En_12x24);
	/* 整合时间字符串 */
	sprintf((char*)string,"%02d",Out_Time);
	/* 显示印剩余时间 */
	GUI_DispStringAt(string,100,16);

	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
}

/************************************************************************************************/
/* 函数名	: DispMenuDeskTopItem																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示单前项目信息																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/ 
static void DispMenuDeskTopItem(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* 当前显示的界面 */
	static u32 now_int = 0;

	/* 确定界面指针所指的界面 */
	Menu_DeskTop_Disp_Reg = Menu_DeskTop_Disp_Reg%(Sensor_CH_Config_Num+1);

	/* 切换界面 */
	if(Menu_DeskTop_Disp_Reg != now_int)
	{
		/* 切换到下一个显示界面 */
		now_int = Menu_DeskTop_Disp_Reg;
		
		/* 通道号或者卡片SN好显示 */
		DispDeskTopSonsorLabel(Menu_DeskTop_Disp_Reg,&config_tab->Inf_State.Instru_Sensor);
		
		/* 清除 显示信息区域的内容 */
		TipClr(Line_2_PosX,
			   Line_2_PosY,
			   LCD_XSIZE,
			   LCD_YSIZE);

	}
	
	/* 判断界面指针的位置 */
	if(Menu_DeskTop_Disp_Reg == 0)
	{
		/* 时间界面 */
		DispDeskTopTimeItem();
	}
	
#if (Sensor_CH1_Config_Type != Sensor_Null)
	else if(Menu_DeskTop_Disp_Reg == 1)
	{
		/* 显示 传感器 通道一 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH1,&fix_tab->Inf_State.Instru_Sensor.Inf.CH1);
	}
#endif
	
#if (Sensor_CH2_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 2)
	{
		/* 显示 传感器 通道二 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH2,&fix_tab->Inf_State.Instru_Sensor.Inf.CH2);
	}
#endif
	
#if (Sensor_CH3_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 3)
	{
		/* 显示 传感器 通道三 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH3,&fix_tab->Inf_State.Instru_Sensor.Inf.CH3);
	}
#endif

#if (Sensor_CH4_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 4)
	{
		/* 显示 传感器 通道四 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH4,&fix_tab->Inf_State.Instru_Sensor.Inf.CH4);
	}
#endif
	
#if (Sensor_CH5_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 5)
	{
		/* 显示 传感器 通道五 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH5,&fix_tab->Inf_State.Instru_Sensor.Inf.CH5);
	}
#endif	
	
#if (Sensor_CH6_Config_Type != Sensor_Null) 	
	else if(Menu_DeskTop_Disp_Reg == 6)
	{
		/* 显示 传感器 通道六 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH6,&fix_tab->Inf_State.Instru_Sensor.Inf.CH6);
	}	
#endif		
	
#if (Sensor_CH7_Config_Type != Sensor_Null) 	
	else if(Menu_DeskTop_Disp_Reg == 7)
	{
		/* 显示 传感器 通道七 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH7,&fix_tab->Inf_State.Instru_Sensor.Inf.CH7);
	}	
#endif	

#if (Sensor_CH8_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 8)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH8,&fix_tab->Inf_State.Instru_Sensor.Inf.CH8);
	}	
#endif
	
#if (Sensor_CH9_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 9)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH9,&fix_tab->Inf_State.Instru_Sensor.Inf.CH9);
	}	
#endif
	
#if (Sensor_CH10_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 10)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH10,&fix_tab->Inf_State.Instru_Sensor.Inf.CH10);
	}	
#endif

#if (Sensor_CH11_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 11)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH11,&fix_tab->Inf_State.Instru_Sensor.Inf.CH11);
	}	
#endif

#if (Sensor_CH12_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 12)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH12,&fix_tab->Inf_State.Instru_Sensor.Inf.CH12);
	}	
#endif	
	
#if (Sensor_CH13_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 13)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH13,&fix_tab->Inf_State.Instru_Sensor.Inf.CH13);
	}	
#endif
	
#if (Sensor_CH14_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 14)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH14,&fix_tab->Inf_State.Instru_Sensor.Inf.CH14);
	}	
#endif

#if (Sensor_CH15_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 15)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH15,&fix_tab->Inf_State.Instru_Sensor.Inf.CH15);
	}	
#endif

#if (Sensor_CH16_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 16)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH16,&fix_tab->Inf_State.Instru_Sensor.Inf.CH16);
	}	
#endif	
	
#if (Sensor_CH17_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 17)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH17,&fix_tab->Inf_State.Instru_Sensor.Inf.CH17);
	}	
#endif
	
#if (Sensor_CH18_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 18)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH18,&fix_tab->Inf_State.Instru_Sensor.Inf.CH18);
	}	
#endif

#if (Sensor_CH19_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 19)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH19,&fix_tab->Inf_State.Instru_Sensor.Inf.CH19);
	}	
#endif

#if (Sensor_CH20_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 20)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH20,&fix_tab->Inf_State.Instru_Sensor.Inf.CH20);
	}	
#endif	
	
#if (Sensor_CH21_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 21)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH21,&fix_tab->Inf_State.Instru_Sensor.Inf.CH21);
	}	
#endif
	
#if (Sensor_CH22_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 22)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH22,&fix_tab->Inf_State.Instru_Sensor.Inf.CH22);
	}	
#endif

#if (Sensor_CH23_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 23)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH23,&fix_tab->Inf_State.Instru_Sensor.Inf.CH23);
	}	
#endif

#if (Sensor_CH24_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 24)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH24,&fix_tab->Inf_State.Instru_Sensor.Inf.CH24);
	}	
#endif	
	
#if (Sensor_CH25_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 25)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH25,&fix_tab->Inf_State.Instru_Sensor.Inf.CH25);
	}	
#endif
	
#if (Sensor_CH26_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 26)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH26,&fix_tab->Inf_State.Instru_Sensor.Inf.CH26);
	}	
#endif

#if (Sensor_CH27_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 27)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH27,&fix_tab->Inf_State.Instru_Sensor.Inf.CH27);
	}	
#endif

#if (Sensor_CH28_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 28)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH28,&fix_tab->Inf_State.Instru_Sensor.Inf.CH28);
	}	
#endif	
	
#if (Sensor_CH29_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 29)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH29,&fix_tab->Inf_State.Instru_Sensor.Inf.CH29);
	}	
#endif
	
#if (Sensor_CH30_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 30)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH30,&fix_tab->Inf_State.Instru_Sensor.Inf.CH30);
	}	
#endif

#if (Sensor_CH31_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 31)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH31,&fix_tab->Inf_State.Instru_Sensor.Inf.CH31);
	}	
#endif

#if (Sensor_CH32_Config_Type != Sensor_Null) 
	else if(Menu_DeskTop_Disp_Reg == 32)
	{
		/* 显示 传感器 通道八 参数 */
		DispDeskTopSonsorItem(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH32,&fix_tab->Inf_State.Instru_Sensor.Inf.CH32);
	}	
#endif	

}


/************************************************************************************************
*函数名     : Show_StartUpload_Pic															
*输入参数  	: 无										
*输出参数	  : 无 																				
*作用	 	    : 显示卸货图标并进行间隔闪烁																
*创建日期	  : 2018/05/24																		
************************************************************************************************/
static   void Show_StartUpload_Pic(void)
{
	static BOOL ShowUpload_Flag=FALSE;//控制卸货图标显示Flag 默认是不显示的状态
	
	 if(ShowUpload_Flag==TRUE)
	{
	  ShowUpload_Flag=FALSE;
	  GUI_Draw_Bar_BMP(Menu_StartUpload_Index[0],Menu_Save_Data_BMP_PosX,Menu_Save_Data_BMP_PosY);				   
	}
  else if(ShowUpload_Flag==FALSE)
	{
		ShowUpload_Flag=TRUE;
	  GUI_Draw_Bar_BMP(Menu_StartUpload_Index[1],Menu_Save_Data_BMP_PosX,Menu_Save_Data_BMP_PosY);			 
	}				 					

}

/************************************************************************************************/
/* 函数名	: DispMenuDesktop																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示待机界面																		*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/ 
void DispMenuDeskTop(void)
{	
	GSM_GPRS_SR_Struct gsmState;
	
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash)||(Flash == FlashMode_DeskTop))
	{
		
		if(Flash != FlashMode_DeskTop)
		{
			/* 清除屏幕显示 */
			GUI_Clear();
		}
		
		/* 待机界面 显示的内容和项目更多并且繁琐，因此，这里刷新功能可以充分利用。提高效率。 */
		/* 显示移动信号图标 */
		/* 判断当前模块状态 */
		gsmState = GPRS_Disp_Sign_Stre();
		
		if(gsmState.GPRS_Work_State == TRUE)
		{
			/* 模块工作正常,显示当前信号强度 */
			GUI_Draw_Bar_BMP(Mobile_Signal_Index[gsmState.GPRS_Sign_Stre_Disp],Mobile_Signal_BMP_PosX,Mobile_Signal_BMP_PosY);
		}else
		{
			/* 模块不正常工作，判断SIM卡信息 */
			if(gsmState.SimState == SIM_NoSignal)
			{
				/* 无网络信号 */
				GUI_Draw_Bar_BMP(Mobile_SIM_NoSignal,Mobile_Signal_BMP_PosX,Mobile_Signal_BMP_PosY);
			}else if(gsmState.SimState == SIM_NotDetected)
			{
				/* 卡未连接 */
				GUI_Draw_Bar_BMP(Mobile_SIM_NotDetected,Mobile_Signal_BMP_PosX,Mobile_Signal_BMP_PosY);
			}else if(gsmState.SimState == SIM_ShutDown)
			{
				/* 欠费停机 */
				GUI_Draw_Bar_BMP(Mobile_SIM_ShutDown,Mobile_Signal_BMP_PosX,Mobile_Signal_BMP_PosY);
			}
		}
#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag==1)
	 {
	 	 	Show_StartUpload_Pic();	
	 }
	 else
	 {
		 //		/* 显示 记录 当前数据记录状态. */
		GUI_Draw_Bar_BMP(Menu_Save_Data_Index[*Menu_Save_Data_Switch_P],Menu_Save_Data_BMP_PosX,Menu_Save_Data_BMP_PosY);		
	 }
#endif
		
		/* 显示通道状态 */
		if((Menu_DeskTop_Disp_Reg != 0)&&
		 (((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[(Menu_DeskTop_Disp_Reg-1)*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Alarm_State != 0))
		{
			GUI_Draw_Bar_BMP(Alarm_Index[1],Alarm_BMP_PosX,Alarm_BMP_PosY);
		}else
		{
			GUI_Draw_Bar_BMP(Alarm_Index[0],Alarm_BMP_PosX,Alarm_BMP_PosY);
		}	
		/* 显示电池图标 */
		Battery_Power_Display();
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x00)
		/*判断显示升级进度百分比界面*/
		if(*IAP_App.UpdateFlag == TRUE)
		{
		  DispDeskTopUpdate();//显示升级进度百分比界面
		}
		/* 判断快捷打印功能 */
		else if(Easy_Print_Get_State() == TRUE)
		{
			/* 显示快捷打印信息界面 */
			DispMenuEasyPrintItem();
		}
		else
		{
			/* 显示 传感器通道信息 日期时间 */
			DispMenuDeskTopItem(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);	
		}
		#endif
		#endif
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x01)
		/* 判断快捷打印功能 */
	if(Easy_Print_Get_State() == TRUE)
		{
			/* 显示快捷打印信息界面 */
			DispMenuEasyPrintItem();
		}
		else
		{
			/* 显示 传感器通道信息 日期时间 */
			DispMenuDeskTopItem(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);	
		}
		#endif
		#endif	

	}
}

/************************************************************************************************/
/* 函数名	: DispMenuDesktopUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入菜单界面																		*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuDeskTopUp(void)
{
	/* 初始化菜单系统 */
    InitMenuVal();

	/* 进入菜单界面。 */
    Jump2Menu(MenuIcoIndex,FlashMode_ManualInit);
	
	/* 进入到下一级界面。 */
	PUSH();
}

/************************************************************************************************/
/* 函数名	: DispMenuDesktopDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 待机界面下 取消当前的声光报警通道报警，并设置相应通道的取消报警标志位。					*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuDeskTopDown(void)
{

	/* 检测32路传感器的状态 */
	Set_Sensor_Suspend_Alarm_Flag_All_CH(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor);
	/* 将函数指针指向待机界面 */
    Jump2Menu(MenuDeskTopIndex,FlashMode_NoAction);

}

/************************************************************************************************/
/* 函数名	: DispMenuDesktopLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 查看上一个项目的信息内容（传感器数据或时间）											*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuDeskTopLeft(void)
{
	BOOL gui_dis_state = FALSE;
	
	/* 设置 待机界面 项目指针 */
	if(Menu_DeskTop_Disp_Reg)
	{
		Menu_DeskTop_Disp_Reg--;
	}else
	{
		Menu_DeskTop_Disp_Reg = Sensor_CH_Config_Num;
	}
	
	
	for(; (Menu_DeskTop_Disp_Reg != 0) ; Menu_DeskTop_Disp_Reg --)
	{
		/* 根据界面指针位置，找出对应通道类，假如是卡片类型，再判断卡片的ID是否配置:配置就显示该通道数据，没有配置就跳过。 */
		gui_dis_state = Menu_RF_Card_GUI_Dis(Menu_DeskTop_Disp_Reg-1,&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor);
		
		/* 判断界面显示状态 */
		if(gui_dis_state == TRUE)
		{
			/* 显示当前指针的界面 */
			break;
		}
	}
	
	/* 将函数指针指向待机界面 */
    Jump2Menu(MenuDeskTopIndex,FlashMode_ManualInit);
}

/************************************************************************************************/
/* 函数名	: DispMenuDesktopRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 查看下一个项目的信息内容（传感器数据或时间）											*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuDeskTopRight(void)
{
	BOOL gui_dis_state = FALSE;
	
	/* 设置 待机界面 项目指针 */
	Menu_DeskTop_Disp_Reg = (++Menu_DeskTop_Disp_Reg)%(Sensor_CH_Config_Num+1);
	
	for(; (Menu_DeskTop_Disp_Reg != 0) ; Menu_DeskTop_Disp_Reg = (++Menu_DeskTop_Disp_Reg)%(Sensor_CH_Config_Num+1))
	{
		/* 根据界面指针位置，找出对应通道类，假如是卡片类型，再判断卡片的ID是否配置:配置就显示该通道数据，没有配置就跳过。 */
		gui_dis_state = Menu_RF_Card_GUI_Dis(Menu_DeskTop_Disp_Reg-1,&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor);
		
		/* 判断界面显示状态 */
		if(gui_dis_state == TRUE)
		{
			/* 显示当前指针的界面 */
			break;
		}
	}
	
	
	/* 将函数指针指向待机界面 */
    Jump2Menu(MenuDeskTopIndex,FlashMode_ManualInit);
}

/************************************************************************************************/
/* 函数名	: DispMenuTopOK																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/6/4																			*/
/* 修改日期	: 2016/7/6																			*/
/************************************************************************************************/
void DispMenuDeskTopOK(void)
{
	/* 初始化菜单系统 */
    InitMenuVal();
	/* 设置“开始记录”界面 */
	Jump2Menu(MenuStartRecordIndex,FlashMode_ManualInit);
	/* 进入到下一级界面 */
	PUSH();
}

/************************************************************************************************/
/* 函数名	: DispMenuTopCancel																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/6/4																			*/
/* 修改日期	: 2016/7/6																			*/
/************************************************************************************************/
void DispMenuDeskTopCancel(void) 
{
	/* 初始化菜单系统 */
    InitMenuVal();
	/* 设置“停止记录”界面 */
	Jump2Menu(MenuStopRecordIndex,FlashMode_ManualInit);
	/* 进入到下一级界面 */
	PUSH();

} 

/******************* (C) ZHOU *****END OF FILE****/


