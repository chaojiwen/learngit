/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/10/22																			 */
/*************************************************************************************************/
#include "Log_Battery.h"
#include "Ks0108.h"
#include "Menu.H"


// 读取GPIO口电平值应使用STM32库函数，而不应直接读取寄存器(某些IO口可以通过直接读取寄存器正确读出，有些不行)
#define Battery_Miss_Check()      GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)	

/* 电池电量%0值，对应的ADC值 */
#define Battery_Level_0		(2080ul)
/* 电池电量%100值，对应的ADC值 */
#define Battery_Level_100	(2638ul)

static void Battery_Charge_Display(void);


/*
*********************************************************************************************************
*	函 数 名: Check_Battery_State
*	功能说明: z周期检测电池电压
*	形    参：无
*	返 回 值: 无
*	说明	：在主系统中间隔50MS检测一次。
*********************************************************************************************************
*/
void Power_BT_Quantity_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* 电量值 */
	uint8_t powerValue;
	/* 电池电量状态累计计数器 */
	static u16 bt_quantity_state_cnt = 0;
	/* 首次电源检测标志 */
	static BOOL powerFirstCheckFlag = FALSE;
	
	
	/* 判断当前前电池电量 */
	if(Battery_Level_0 >= power_f->Inf.BT_Quantity_Sampl_Value)
	{
		/* 电量低于0%，设置0% */
		powerValue = 0;
	}else if(Battery_Level_100 <= power_f->Inf.BT_Quantity_Sampl_Value)
	{
		/* 电量大于100%，设置100% */
		powerValue = 100;
	}else
	{
		/* 计算当前电量 */
		powerValue = (((uint32_t)power_f->Inf.BT_Quantity_Sampl_Value-Battery_Level_0)*100)/(Battery_Level_100-Battery_Level_0);
	}
	
	/* 判断当前是否为第一次电池电量检测 */
	if(powerFirstCheckFlag == FALSE)
	{
		/* 设置当前电量为仪器的电量 */
		power_t->Inf_State.BT_Quantity_State = powerValue;
		/* 设置首次电源检测标志 */
		powerFirstCheckFlag = TRUE;
		return;
	}
	
	/* 判断 当前的电量 */
	if(power_t->Inf_State.BT_Quantity_State != powerValue)
	{
		/* 设置  电池电量状态累计计数器 */
		bt_quantity_state_cnt++;
		/* 判断 电池电量状态累计计数器 值 是否大于设定值 */
		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
		{
			/* 电池电量状态累计计数器值大于设定值 */
			/* 清除电池电量状态累计计数器 */
			bt_quantity_state_cnt = 0;
			/* 设置电池电量状态 */
			power_t->Inf_State.BT_Quantity_State = powerValue;
		}
	}else 
	{
		/* 判断 电池电量状态累计计数器 是否存在数值 */
		if(bt_quantity_state_cnt)
		{
			/* 电池电量状态累计计数器有值 */
			/* 清除 电池电量状态累计计数器值 */
			bt_quantity_state_cnt = 0;
		}
	}

}

/*
*********************************************************************************************************
*	函 数 名: Check_Battery_State
*	功能说明: z周期检测电池电压
*	形    参：无
*	返 回 值: 无
*	说明	：在主系统中间隔50MS检测一次。
*********************************************************************************************************
*/
//void Power_BT_Quantity_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
//{
/////* 未接 电池 *///....-3.0V
////#define Battery_Miss					((u16)1861)
///* 电池电量0基准 *///....-3.5V			（由于GSM模块最低的工作电压为3.4+，所以这里设置为3.5V，保证GSM模块能够正常工作。）
//#define Battery_Quantity_Null_Base		((u16)2173)
///* 电池电量1基准 *///3.5V-3.65V
//#define Battery_Quantity_One_Base		((u16)2265)
///* 电池电量2基准 *///3.65V-3.8V
//#define Battery_Quantity_Two_Base		((u16)2358)
///* 电池电量3基准 *///3.8V-3.95V
//#define Battery_Quantity_There_Base		((u16)2451)
///* 电池电量4基准 *///3.95V-4.25V
//#define Battery_Quantity_Four_Base		((u16)2638)


//	/* 电池电量状态累计计数器 */
//	static u16 bt_quantity_state_cnt = 0;
////	/* 查看当前状态 */
////	if((MCU_Power.Link_State == On)&&\
////	   ((Battery*)(b_Battery->Sensor))->State != Power_Full)
////	{  
////		((Battery*)(b_Battery->Sensor))->CNT++;
////		if(((Battery*)(b_Battery->Sensor))->CNT > ((Battery*)(b_Battery->Sensor))->Filter_Time)
////		{
////			((Battery*)(b_Battery->Sensor))->CNT = 0;
////			((Battery*)(b_Battery->Sensor))->State = Power_Full;
////		}	
////	}else 
//	/* 判断 当前的电量 */
//	if((power_f->Inf.BT_Quantity_Sampl_Value >= Battery_Quantity_There_Base)&&
//	   (power_t->Inf_State.BT_Quantity_State != Power_Four))
//	{	
//		/* 当前不处于4格电(满电)状态 */
//		/* 设置  电池电量状态累计计数器 */
//		bt_quantity_state_cnt++;
//		/* 判断 电池电量状态累计计数器 值 是否大于设定值 */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* 电池电量状态累计计数器值大于设定值 */
//			/* 清除电池电量状态累计计数器 */
//			bt_quantity_state_cnt = 0;
//			/* 设置电池电量状态 */
//			power_t->Inf_State.BT_Quantity_State = Power_Four;
//		}
//	}else if((power_f->Inf.BT_Quantity_Sampl_Value <  Battery_Quantity_There_Base)&&
//			 (power_f->Inf.BT_Quantity_Sampl_Value >= Battery_Quantity_Two_Base)&&
//			 (power_t->Inf_State.BT_Quantity_State != Power_There))
//	{	
//		/* 当前不处于3格电状态 */
//		/* 设置  电池电量状态累计计数器 */
//		bt_quantity_state_cnt++;
//		/* 判断 电池电量状态累计计数器 值 是否大于设定值 */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* 电池电量状态累计计数器值大于设定值 */
//			/* 清除电池电量状态累计计数器 */
//			bt_quantity_state_cnt = 0;
//			/* 设置电池电量状态 */
//			power_t->Inf_State.BT_Quantity_State = Power_There;
//		}
//	}else if((power_f->Inf.BT_Quantity_Sampl_Value  <  Battery_Quantity_Two_Base)&&\
//			 (power_f->Inf.BT_Quantity_Sampl_Value  >= Battery_Quantity_One_Base)&&\
//			 (power_t->Inf_State.BT_Quantity_State  != Power_Two))
//	{	
//		/* 当前不处于3格电状态 */
//		/* 设置  电池电量状态累计计数器 */
//		bt_quantity_state_cnt++;
//		/* 判断 电池电量状态累计计数器 值 是否大于设定值 */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* 电池电量状态累计计数器值大于设定值 */
//			/* 清除电池电量状态累计计数器 */
//			bt_quantity_state_cnt = 0;
//			/* 设置电池电量状态 */
//			power_t->Inf_State.BT_Quantity_State= Power_Two;
//		}
//	}else if((power_f->Inf.BT_Quantity_Sampl_Value < Battery_Quantity_One_Base)&&\
//			 (power_f->Inf.BT_Quantity_Sampl_Value >= Battery_Quantity_Null_Base)&&\
//			 (power_t->Inf_State.BT_Quantity_State != Power_One))
//	{	
//		/* 当前不处于3格电状态 */
//		/* 设置  电池电量状态累计计数器 */
//		bt_quantity_state_cnt++;
//		/* 判断 电池电量状态累计计数器 值 是否大于设定值 */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* 电池电量状态累计计数器值大于设定值 */
//			/* 清除电池电量状态累计计数器 */
//			bt_quantity_state_cnt = 0;
//			/* 设置电池电量状态 */
//			power_t->Inf_State.BT_Quantity_State = Power_One;
//		}
//	}else if((power_f->Inf.BT_Quantity_Sampl_Value < Battery_Quantity_Null_Base)&&\
//			 (power_t->Inf_State.BT_Quantity_State != Power_Null))
//	{	
//		/* 当前不处于3格电状态 */
//		/* 设置  电池电量状态累计计数器 */
//		bt_quantity_state_cnt++;
//		/* 判断 电池电量状态累计计数器 值 是否大于设定值 */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* 电池电量状态累计计数器值大于设定值 */
//			/* 清除电池电量状态累计计数器 */
//			bt_quantity_state_cnt = 0;
//			/* 设置电池电量状态 */
//			power_t->Inf_State.BT_Quantity_State = Power_Null;
//		}
//	}else 
//	{
//		/* 判断 电池电量状态累计计数器 是否存在数值 */
//		if(bt_quantity_state_cnt)
//		{
//			/* 电池电量状态累计计数器有值 */
//			/* 清除 电池电量状态累计计数器值 */
//			bt_quantity_state_cnt = 0;
//		}
//	}
//	
//#undef Battery_Quantity_Null_Base
//#undef Battery_Quantity_One_Base
//#undef Battery_Quantity_Two_Base
//#undef Battery_Quantity_There_Base
//#undef Battery_Quantity_Four_Base
//}

/*
*********************************************************************************************************
*	函 数 名: Power_BT_Charge_State_Check
*	功能说明: 配置系统参数
*	形    参：无
*	返 回 值: 无
*	说明	：在主系统中间隔50MS检测一次。
*********************************************************************************************************
*/
void Power_BT_Charge_State_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
#define BT_Link_On		(Bit_RESET)
#define BT_Link_Off		(Bit_SET)	
	/* 外电连接端口状态 */
	u8 battery_pin_state;
	/* 充电状态累计计数器 */
	static u16 bt_charge_cnt = 0;	
	
	/* 读取GPIO口的状态 */
	battery_pin_state = Battery_Miss_Check();

	/* 判断 单前 电池充电状态 */
	if(power_t->Inf_State.BT_Charge_State == TRUE)
	{
		/* 当前处于充电状态 */
		/* 判断 状态 是否改变（不充电） */
		if(battery_pin_state == BT_Link_On)
		{
			/* 端口检测到充电结束 */
			/* 设置 充电状态累计计数器 */
			bt_charge_cnt++;
			if(bt_charge_cnt > power_f->Inf.BT_Status_Update_Time)
			{
				/* 清除计数 */
				bt_charge_cnt = 0;	
				/* 设置状态 */
				power_t->Inf_State.BT_Charge_State = FALSE;
			}
		}else
		{
			/* 清除计数 */
			/* 由于 电源检测端口可能 输出脉冲信号，因此需要做保护。 */
			if(bt_charge_cnt)
			{
				/* 减掉 充电状态累计计数器 数值 */
				bt_charge_cnt--;
			}	
		}
	}else
	{
		/* 仪器处于未充电状态 */
		/* 判断 状态 是否改变（充电） */
		if(battery_pin_state == BT_Link_Off)
		{
			/* 端口检测到充电信号 */
			/* 设置 充电状态累计计数器 */
			bt_charge_cnt++;
			if(bt_charge_cnt > power_f->Inf.BT_Status_Update_Time)
			{
				/* 清除计数 */
				bt_charge_cnt = 0;	
				/* 设置状态 */
				power_t->Inf_State.BT_Charge_State = TRUE;
			}
		}else
		{
			/* 由于 电源检测端口可能 输出脉冲信号，因此需要做保护。 */
			if(bt_charge_cnt)
			{
				/* 减掉 充电状态累计计数器 数值 */
				bt_charge_cnt--;
			}	
		}
	} 	
}


/*
*********************************************************************************************************
*	函 数 名: Battery_Power_Display
*	功能说明: 电池电量显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Battery_Power_Display(void)
{
	/* 判断 外接电 是否连接 */
	if(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Link_State == TRUE)
	{
		/* 电源连接 */
		/* 判断 电池电量是否已满 */
		if(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Charge_State == FALSE)
		{
			/* 电池电量未满 */
			/* 电池正在充电 */
			Battery_Charge_Display();
		}else
		{
			/* 电池电量已满 */
			/* 电池已经充满电 */
			GUI_Draw_Bar_BMP(Battery_Power_Index[Power_Full],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}
	}else
	{
		/* 外电未接，显示电池当前电量. *///由于未接外电，只有接电池系统才有可能工作。
		if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[0],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
				 (Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[1],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
				 (Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[2],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
				 (Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[3],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
				 (Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[4],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[4],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}
		
	}
}

/*
*********************************************************************************************************
*	函 数 名: Battery_Charge_Display
*	功能说明: 充电状态显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Battery_Charge_Display(void)
{
	static u8 num = 0;
	
	GUI_Draw_Bar_BMP(Battery_Power_Index[num++],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
	if(num > Power_Four)
	{
		num = Power_Null;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Power_BT_SET_Alarm_State
*	功能说明: 电池电量的报警状态设置
*	形    参：power_t: 电池电量报警状态;power_f:暂时没有使用；
*	返 回 值: 无
*	说明	：在主系统中间隔50MS检测一次。
*********************************************************************************************************
*/
void Power_BT_SET_Alarm_State(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* 判断当前电池电量状态 */
	if(power_t->Inf_State.BT_Quantity_State <= 10)
	{
		/* 电池电量不足 */
		/* 设置 电池电量报警 */
		power_t->Inf_State.BT_Alarm_State =	TRUE; 
	}else if((power_t->Inf_State.BT_Quantity_State > 10)&&(power_t->Inf_State.BT_Quantity_State<=100))
	{
		/* 电池电量正常 */
		/* 设置 不报警 */
		power_t->Inf_State.BT_Alarm_State =	FALSE; 
	}else
	{
		/* 未设置状态 */
		__nop();
	}
}

/*
*********************************************************************************************************
*	函 数 名: Power_BT_Alarm_Check
*	功能说明: 电池电量报警状态检测
*	形    参：power_t: 电池电量报警状态;power_f:暂时没有使用；
*	返 回 值: 无
*	说明	：在主系统中间隔50MS检测一次。
*********************************************************************************************************
*/
u8 Power_BT_Alarm_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	
	/* 通道报警状态 */
	u8 ch_alarm_sr = 0;
	
	/* 判断 电池电量低的报警状态 */
	if(power_t->Inf_State.BT_Alarm_State  == TRUE)
	{
		/* 当前外电连接为报警状态 */
		/* 根据当前信息设置，设置报警内容。 */
		ch_alarm_sr |= ((power_t->Inf_State.BT_Sound_Alarm_Switch == TRUE) ? 0X1:0X0);
		ch_alarm_sr |= ((power_t->Inf_State.BT_Light_Alarm_Switch == TRUE) ? 0X2:0X0);
		ch_alarm_sr |= ((power_t->Inf_State.BT_SMS_Alarm_Switch   == TRUE) ? 0X4:0X0);

	}
	
	return ch_alarm_sr;
}

/************************************************************************************************/
/* 函数名	: Battery_Para_Init															 	 	*/
/* 输入  	: *bt_state: 电池状态指针。															*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化配置电池状态参数							 									*/
/* 创建日期 	: 2015/08/13																		*/
/************************************************************************************************/
void Battery_Para_Init(INSTRU_POWER *bt_state)
{
	
	
	/* 设置电池充电状态：没在充电 */
	bt_state->Inf_State.BT_Charge_State = FALSE;
	/* 设置初始电量状态：满电 */
	bt_state->Inf_State.BT_Quantity_State = 100;
	/* 设置报警状态：为报警 */
	bt_state->Inf_State.BT_Alarm_State = FALSE;
}

///************************************************************************************************/
///* 函数名	: Power_Record_Function_Check												 	 	*/
///* 输入  	: pwr_vle:电源电压采集值.																*/
///* 输出		: TRUE:正常状态，FALSE:停止状态.														*/
///* 作用	 	: 检测电源电压,当电压低于3.5V时，仪器停止记录功能。	 									*/
///* 创建日期 	: 2015/10/10																		*/
///************************************************************************************************/
//BOOL Power_Record_Function_Check(u16 pwr_vle)
//{
///* 电池电量0基准 */		
//#define Battery_Quantity_Null_Base		((u16)2173)		//3.5V	
//	
//	
//	/* 判断电源电压值 */
//	if(pwr_vle < Battery_Quantity_Null_Base)
//	{
//		/* 电源电压值小于3.5V，仪器应该停止记录功能. */
//		return FALSE;
//	}
//	
//	return TRUE;
//	
//}



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

