/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/10/22																			 */
/*************************************************************************************************/
#include "Log_Power.h"






/*
*********************************************************************************************************
*	函 数 名: Power_Link_State_Check
*	功能说明: 配置系统参数
*	形    参：无
*	返 回 值: 无
*	说明	：在主系统中间隔50MS检测一次。
*********************************************************************************************************
*/
void Power_ET_Link_State_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
#define ET_Link_On		(Bit_RESET)
#define ET_Link_Off		(Bit_SET)	
	/* 外电连接端口状态 */
	u8 power_pin_static;
	/* 外电状态累计计数器 */
	static u16 et_link_cnt = 0;

	/* 读取GPIO口的状态 */
	power_pin_static = Ext_Power_ON_OFF_Check();

	/* 判断 单前 外接电的连接状态 */
	if(power_t->Inf_State.ET_Link_State == TRUE)
	{
		/* 外接点处于连接状态 */
		/* 判断外电检测端口状态（有电/无电）  */
		if(power_pin_static == ET_Link_Off)
		{
			/* 设置 外电状态累计计数器 */
			et_link_cnt++;
			/* 判断 当前的 掉电状态的时间 是否 设定的值（默认1S） */
			if(et_link_cnt > power_f->Inf.ET_Status_Update_Time)
			{
				/* 清除计数 */
				et_link_cnt = 0;	
				/* 设置状态 */
				power_t->Inf_State.ET_Link_State = FALSE;
			}
		}else
		{
			/* 清除计数 */
			/* 由于 电源检测端口可能 输出脉冲信号，因此需要做保护。 */
			if(et_link_cnt)
			{
				/* 减掉 外电状态累计计数器 数值 */
				et_link_cnt--;
			}	
		}
	}else
	{
		/* 外接点处于未连接状态 */
		/* 判断外电检测端口状态（有电/无电）  */
		if(power_pin_static == ET_Link_On)
		{
			/* 设置 外电状态累计计数器 */
			et_link_cnt++;
			/* 判断 当前的 上电状态的时间 是否 设定的值（默认1S） */
			if(et_link_cnt >  power_f->Inf.ET_Status_Update_Time)
			{
				/* 清除计数 */
				et_link_cnt = 0;	
				/* 设置状态 */
				power_t->Inf_State.ET_Link_State = TRUE;
			}
		}else
		{
			/* 由于 电源检测端口可能 输出脉冲信号，因此需要做保护。 */
			if(et_link_cnt)
			{
				/* 减掉 外电状态累计计数器 数值 */
				et_link_cnt--;
			}	
		}
	}
	
#undef ET_Link_On
#undef ET_Link_Off
}

/*
*********************************************************************************************************
*	函 数 名: Power_ET_SET_Alarm_State
*	功能说明: 外接电源的报警状态设置
*	形    参：power_t: 外电连接报警状态;power_f:暂时没有使用；
*	返 回 值: 无
*	说明	：在主系统中间隔50MS检测一次。
*********************************************************************************************************
*/
void Power_ET_SET_Alarm_State(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* 判断当前外电连接状态 */
	if(power_t->Inf_State.ET_Link_State == FALSE)
	{
		/* 外电未连接 */
		/* 设置 掉电报警 */
		power_t->Inf_State.ET_Alarm_State =	TRUE; 
	}else
	{
		/* 外电 已连接 */
		/* 设置 不报警 */
		power_t->Inf_State.ET_Alarm_State =	FALSE; 
	}
}

/*
*********************************************************************************************************
*	函 数 名: Power_ET_Alarm_Check
*	功能说明: 外电报警状态检测
*	形    参：power_t: 外电连接报警状态;power_f:暂时没有使用；
*	返 回 值: 无
*	说明	：在主系统中间隔50MS检测一次。
*********************************************************************************************************
*/
u8 Power_ET_Alarm_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{

	/* 通道报警状态 */
	u8 ch_alarm_sr = 0;

	/* 判断 外电连接的报警状态 */
	if(power_t->Inf_State.ET_Alarm_State  == TRUE)
	{
		/* 当前外电连接为报警状态 */
		/* 根据当前信息设置，设置报警内容。 */
		ch_alarm_sr |= ((power_t->Inf_State.ET_Sound_Alarm_Switch == TRUE) ? 0X1:0X0);
		ch_alarm_sr |= ((power_t->Inf_State.ET_Light_Alarm_Switch == TRUE) ? 0X2:0X0);
		ch_alarm_sr |= ((power_t->Inf_State.ET_SMS_Alarm_Switch	  == TRUE) ? 0X4:0X0);

	}

	return ch_alarm_sr;
}


/************************************************************************************************/
/* 函数名	: Power_ET_Para_Init														 	 	*/
/* 输入  	: *bt_state: 电池状态指针。															*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化配置外接电源状态参数							 								*/
/* 创建日期 	: 2015/08/13																		*/
/************************************************************************************************/
void Power_ET_Para_Init(INSTRU_POWER *power_state)
{

	/* 设置外电连接状态：连接 */
	power_state->Inf_State.ET_Link_State = TRUE;
	/* 设置外电报警状态：未报警 */
	power_state->Inf_State.ET_Alarm_State = FALSE;
	
}






/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

