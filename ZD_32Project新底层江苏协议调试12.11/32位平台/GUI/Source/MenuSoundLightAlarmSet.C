/************************************************************************************************/
/** @file              : MenuSoundLightAlarmSetIndex.c											*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月25日															*/
/** @date changed      : 2015年09月25日															*/
/** @date changed      : 2015年10月21日(遗漏光报警功能)											*/
/** @brief             : c file																	*/
/** @description       : GUI菜单 -> 声光报警通道设置界面											*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "Ks0108.H"
#include "Menu.H"

/* 声光报警通道设置界面 中文 菜单项目栏 */
static u8 * const Menu_Sud_Lgt_Aam_Set_CN[]=
{
	{Menu_ON_CN},
	{Menu_OFF_CN},		
};

/* 声光报警通道设置界面 英文 菜单项目栏 */
static u8 * const Menu_Sud_Lgt_Aam_Set_EN[]=
{
	{Menu_ON_EN},
	{Menu_OFF_EN},		
};

/* 声光报警通道设置界面 中英文 菜单栏 */
const struct Menu Menu_Sud_Lgt_Aam_Set[] =
{
	MenuWhat(Menu_Sud_Lgt_Aam_Set_CN),
	MenuWhat(Menu_Sud_Lgt_Aam_Set_EN),
};

/* 传感器报警上限时间计数寄存器指针 */
#define Sud_Lgt_Aam_Up_Cnt_P(Cha)	(&((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Up_Alarm_Cnt)	

/* 传感器报警下限时间计数寄存器指针 */
#define Sud_Lgt_Aam_Low_Cnt_P(Cha)	(&((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Low_Alarm_Cnt)
	
/* 传感器 报警取消标志指针 */
#define Sud_Lgt_Aam_Suspend_P(Cha)	(&((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Suspend_Alarm_Flag)

/* 通道报警状态指针 */
#define Sud_Lgt_Aam_State_P(Cha) 	(&((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Alarm_State)

/* 通道报警开关状态 */
#define Sud_Aam_Switch(Cha) 		(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Sound_Alarm_Switch)

/* 通道报警开关状态 */
#define Lgt_Aam_Switch(Cha) 		(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Light_Alarm_Switch)


/* 通道声音报警开关状态指针 */
#define Sud_Aam_Switch_State_P(Cha) (&((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Sound_Alarm_Switch)

/* 通道光报警开关状态指针 */
#define Lgt_Aam_Switch_State_P(Cha) (&((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Light_Alarm_Switch)



/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamSetInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 声光报警通道设置 信息														*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSudLgtAamSetInit(void)
{
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_Sud_Lgt_Aam_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;

		/* 判断选择的通道 */
		if(ItemBackup[ItemBackup_i-1] == 0)
		{
			/* 第一个选项是"全部通道设置",因此固定设置. */ 
			Item = 0;
        	Where = 0;
		}else
		{
			/* 设置固定通道. */
			Item = ((Sud_Aam_Switch((ItemBackup[ItemBackup_i-1]-1)) == TRUE)&&
					(Lgt_Aam_Switch((ItemBackup[ItemBackup_i-1]-1)) == TRUE)) ? 0x0 : 0x1;
        	Where = Item;
		}

		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Sud_Lgt_Aam_Cha_Sec[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamSetUp															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: "设置"声光报警.															*/
/* 创建日期	: 2015/10/21																		*/
/************************************************************************************************/
void DispMenuSudLgtAamSetUp(void)
{
	u8 num;

	/* 判断选择的通道 */
	if(ItemBackup[ItemBackup_i-1] == 0)
	{
		switch(Item)
		{
			case 0:
			{
				/* "开" 所有通道声光报警功能. */
				for(num = 0;num < Sensor_CH_Config_Num;num++)
				{
					/* "开" 单通道声音报警功能. */
					*Sud_Aam_Switch_State_P(num) = TRUE;
					
					/* 修改 仪器 信息表 设置标志. */
					Write_Config_Tab((u8*)Sud_Aam_Switch_State_P(num),sizeof(*Sud_Aam_Switch_State_P(num)));
					
					/* "开" 单通道光报警功能. */
					*Lgt_Aam_Switch_State_P(num) = TRUE;
					
					/* 修改 仪器 信息表 设置标志. */
					Write_Config_Tab((u8*)Lgt_Aam_Switch_State_P(num),sizeof(*Lgt_Aam_Switch_State_P(num)));
					
				
					/* 清除报警状态 */
					*Sud_Lgt_Aam_State_P(num) = 0;
					
					/* 清除上限时间计数寄存器 */
					*Sud_Lgt_Aam_Up_Cnt_P(num) = 0;
					
					/* 清除下限时间计数寄存器 */
					*Sud_Lgt_Aam_Low_Cnt_P(num) = 0;
					
					/* 清除报警取消标志 */
					*Sud_Lgt_Aam_Suspend_P(num) = FALSE;
				}
				
				/* 退回到上一级界面 */
				POP();
				break;
			}
			
			case 1:
			{
				
				/* "关" 所有通道声光报警功能. */
				for(num = 0;num < Sensor_CH_Config_Num;num++)
				{
					/* "开" 单通道声音报警功能. */
					*Sud_Aam_Switch_State_P(num) = FALSE;
					
					/* 修改 仪器 信息表 设置标志. */
					Write_Config_Tab((u8*)Sud_Aam_Switch_State_P(num),sizeof(*Sud_Aam_Switch_State_P(num)));
					
					/* "开" 单通道光报警功能. */
					*Lgt_Aam_Switch_State_P(num) = FALSE;
					
					/* 修改 仪器 信息表 设置标志. */
					Write_Config_Tab((u8*)Lgt_Aam_Switch_State_P(num),sizeof(*Lgt_Aam_Switch_State_P(num)));
					
					/* 清除报警状态 */
					*Sud_Lgt_Aam_State_P(num) = 0;
					
					/* 清除上限时间计数寄存器 */
					*Sud_Lgt_Aam_Up_Cnt_P(num) = 0;
					
					/* 清除下限时间计数寄存器 */
					*Sud_Lgt_Aam_Low_Cnt_P(num) = 0;
					
					/* 清除报警取消标志 */
					*Sud_Lgt_Aam_Suspend_P(num) = FALSE;
				}
			
				/* 退回到上一级界面 */
				POP();
				break;
			}
		
			default:
			{
				/* 未定义界面,设置界面。 */
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuSudLgtAamSetIndex,FlashMode_NoAction);
				break;
			}
		}
	}else
	{
		switch(Item)
		{
			case 0:
			{
				/* 判断通道报警状态 */
				if((Sud_Aam_Switch((ItemBackup[ItemBackup_i-1]-1)) == FALSE)||(Lgt_Aam_Switch((ItemBackup[ItemBackup_i-1]-1)) == FALSE))
				{
					/* "开" 单通道声音报警功能. */
					*Sud_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1)) = TRUE;
					
//					/* 修改 仪器 信息表 设置标志. */
//					Write_Config_Tab((u8*)Sud_Aam_Switch_State_P(num),sizeof(*Sud_Aam_Switch_State_P(num)));
		 			/* 修改 仪器 信息表 设置标志. */
				Write_Config_Tab((u8*)Sud_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1)),sizeof(*Sud_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1))));
					
					
					
					
					/* "开" 单通道光报警功能. */
					*Lgt_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1)) = TRUE;
//					/* 修改 仪器 信息表 设置标志. */
//					Write_Config_Tab((u8*)Lgt_Aam_Switch_State_P(num),sizeof(*Lgt_Aam_Switch_State_P(num)));	
										/* 修改 仪器 信息表 设置标志. */
					Write_Config_Tab((u8*)Lgt_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1)),sizeof(*Lgt_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1))));	
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					/* 清除报警状态 */
					*Sud_Lgt_Aam_State_P((ItemBackup[ItemBackup_i-1]-1)) = 0;
					
					/* 清除上限时间计数寄存器 */
					*Sud_Lgt_Aam_Up_Cnt_P((ItemBackup[ItemBackup_i-1]-1)) = 0;
					
					/* 清除下限时间计数寄存器 */
					*Sud_Lgt_Aam_Low_Cnt_P((ItemBackup[ItemBackup_i-1]-1)) = 0;
					
					/* 清除报警取消标志 */
					*Sud_Lgt_Aam_Suspend_P((ItemBackup[ItemBackup_i-1]-1)) = FALSE;

					/* 退回到上一级界面 */
					POP();
				}else
				{
					/* 将函数指针指向初始化函数 */
					Jump2Menu(MenuSudLgtAamSetIndex,FlashMode_NoAction);
				}
				break;
			}
			
			case 1:
			{
				/* 判断通道报警状态 */
				if((Sud_Aam_Switch((ItemBackup[ItemBackup_i-1]-1)) == TRUE)||(Lgt_Aam_Switch((ItemBackup[ItemBackup_i-1]-1)) == TRUE))
				{
					/* "开" 单通道声音报警功能. */
					*Sud_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1)) = FALSE;
					
//					/* 修改 仪器 信息表 设置标志. */
//					Write_Config_Tab((u8*)Sud_Aam_Switch_State_P(num),sizeof(*Sud_Aam_Switch_State_P(num)));
					
							/* 修改 仪器 信息表 设置标志. */
					Write_Config_Tab((u8*)Sud_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1)),sizeof(*Sud_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1))));
					

					/* "开" 单通道光报警功能. */
					*Lgt_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1)) = FALSE;
					
//					/* 修改 仪器 信息表 设置标志. */
//					Write_Config_Tab((u8*)Lgt_Aam_Switch_State_P(num),sizeof(*Lgt_Aam_Switch_State_P(num)));
										/* 修改 仪器 信息表 设置标志. */
					Write_Config_Tab((u8*)Lgt_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1)),sizeof(*Lgt_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1))));
					

					
					/* 清除报警状态 */
					*Sud_Lgt_Aam_State_P((ItemBackup[ItemBackup_i-1]-1)) = 0;
					
					/* 清除上限时间计数寄存器 */
					*Sud_Lgt_Aam_Up_Cnt_P((ItemBackup[ItemBackup_i-1]-1)) = 0;
					
					/* 清除下限时间计数寄存器 */
					*Sud_Lgt_Aam_Low_Cnt_P((ItemBackup[ItemBackup_i-1]-1)) = 0;
					
					/* 清除报警取消标志 */
					*Sud_Lgt_Aam_Suspend_P((ItemBackup[ItemBackup_i-1]-1)) = FALSE;
					
					/* 退回到上一级界面 */
					POP();
				}else
				{
					/* 将函数指针指向初始化函数 */
					Jump2Menu(MenuSudLgtAamSetIndex,FlashMode_NoAction);
				}
				break;
			}
		
		
			default:
			{
				/* 未定义界面,设置界面。 */
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuSudLgtAamSetIndex,FlashMode_NoAction);
				break;
			}
		}
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamSetDown													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: "退出"声光报警															*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSudLgtAamSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamSetLeft													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "开" 声光报警														*/
/* 创建日期	: 2015/09/28																			*/
/************************************************************************************************/
void DispMenuSudLgtAamSetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamSetRight														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "关" 声光报警																			*/
/* 创建日期	: 2015/09/28																			*/
/************************************************************************************************/
void DispMenuSudLgtAamSetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamSetOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/28																			*/
/************************************************************************************************/
void DispMenuSudLgtAamSetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamSetCancel														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSudLgtAamSetCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamSetIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/






