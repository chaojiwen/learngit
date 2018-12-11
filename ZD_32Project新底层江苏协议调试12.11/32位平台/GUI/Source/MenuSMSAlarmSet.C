/************************************************************************************************/
/** @file              : MenuSMSAlarmSetIndex.c													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月30日															*/
/** @date changed      : 2015年09月30日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单 -> 短信报警通道设置界面										*/
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

/* 短信报警通道设置界面 中文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_Set_CN[]=
{
	{Menu_ON_CN},
	{Menu_OFF_CN},
};

/* 短信报警通道设置界面 英文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_Set_EN[]=
{
	{Menu_ON_EN},
	{Menu_OFF_EN},
};

/* 短信报警通道设置界面 中英文 菜单栏 */
const struct Menu Menu_SMS_Aam_Set[] =
{
	MenuWhat(Menu_SMS_Aam_Set_CN),
	MenuWhat(Menu_SMS_Aam_Set_EN),
};

/* 通道报警开关状态. */
#define SMS_Aam_Switch(Cha) (((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_SMS_Alarm_Switch)
/* 通道报警开关状态指针. */
#define SMS_Aam_Switch_State_P(Cha) (&((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_SMS_Alarm_Switch)
/* 通道报警指针. */
static BOOL *SMS_Aam_CHX_Switch_State_P;


/************************************************************************************************/
/* 函数名	: DispMenuSMSAamSetInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 短信报警通道设置 信息														*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamSetInit(void)
{
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_SMS_Aam_Set[*Language];
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
			Item = (SMS_Aam_Switch((ItemBackup[ItemBackup_i-1]-1)) == 0x0) ? 0x1 : 0x0;
        	Where = Item;
			/* 初始化传感器数据指针 */
			SMS_Aam_CHX_Switch_State_P = SMS_Aam_Switch_State_P((ItemBackup[ItemBackup_i-1]-1));
		}

		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_SMS_Aam_Cha_Sec[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamSetUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: "设置"短信报警.																	*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamSetUp(void)
{
	u8 num;

	/* 判断选择的通道 */
	if(ItemBackup[ItemBackup_i-1] == 0)
	{
		switch(Item)
		{
			case 0:
			{
				/* "开" 所有通道短信报警功能. */
				for(num = 0;num < Sensor_CH_Config_Num;num++)
				{
					/* 初始化传感器数据指针 */
					SMS_Aam_CHX_Switch_State_P = SMS_Aam_Switch_State_P(num);
					/* "开" 单通道短信报警功能. */
					*SMS_Aam_CHX_Switch_State_P = TRUE;
					/* 修改 仪器 信息表 设置标志. */
					Write_Config_Tab((u8*)SMS_Aam_CHX_Switch_State_P,sizeof(*SMS_Aam_CHX_Switch_State_P));
				}
				
				/* 退回到上一级界面 */
				POP();
				break;
			}
			
			case 1:
			{
				
				/* "关" 所有通道短信报警功能. */
				for(num = 0;num < Sensor_CH_Config_Num;num++)
				{
					/* 初始化传感器数据指针 */
					SMS_Aam_CHX_Switch_State_P = SMS_Aam_Switch_State_P(num);
					/* "开" 单通道短信报警功能. */
					*SMS_Aam_CHX_Switch_State_P = FALSE;
					/* 修改 仪器 信息表 设置标志 */
					Write_Config_Tab((u8*)SMS_Aam_CHX_Switch_State_P,sizeof(*SMS_Aam_CHX_Switch_State_P));
				}
			
				/* 退回到上一级界面 */
				POP();
				break;
			}
		
			default:
			{
				/* 未定义界面,设置界面。 */
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuSMSAamSetIndex,FlashMode_NoAction);
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
				if(SMS_Aam_Switch((ItemBackup[ItemBackup_i-1]-1)) == FALSE)
				{
					/* 通道报警状态为:关,开启 通道 短信报警. */
					*SMS_Aam_CHX_Switch_State_P = TRUE;
		
					/* 修改 仪器 信息表 设置标志 */
					Write_Config_Tab((u8*)SMS_Aam_CHX_Switch_State_P,sizeof(*SMS_Aam_CHX_Switch_State_P));
					/* 退回到上一级界面 */
					POP();
				}else
				{
					/* 将函数指针指向初始化函数 */
					Jump2Menu(MenuSMSAamSetIndex,FlashMode_NoAction);
				}
				break;
			}
			
			case 1:
			{
				/* 判断通道报警状态 */
				if(SMS_Aam_Switch((ItemBackup[ItemBackup_i-1]-1)) == TRUE)
				{
					/* 短信报警状态为:开,关闭短信报警. */
					*SMS_Aam_CHX_Switch_State_P = FALSE;
					
					/* 修改 仪器 信息表 设置标志 */
					Write_Config_Tab((u8*)SMS_Aam_CHX_Switch_State_P,sizeof(*SMS_Aam_CHX_Switch_State_P));
					/* 退回到上一级界面 */
					POP();
				}else
				{
					/* 将函数指针指向初始化函数 */
					Jump2Menu(MenuSMSAamSetIndex,FlashMode_NoAction);
				}
				break;
			}
		
		
			default:
			{
				/* 未定义界面,设置界面。 */
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuSMSAamSetIndex,FlashMode_NoAction);
				break;
			}
		}
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamSetDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: "退出"短信报警																	*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamSetLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "开" 短信报警																*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamSetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数. */
	Jump2Menu(MenuSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamSetRight															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "关" 短信报警																*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamSetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数. */
	Jump2Menu(MenuSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamSetOK																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamSetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数. */
	Jump2Menu(MenuSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamSetCancel															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamSetCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数. */
	Jump2Menu(MenuSMSAamSetIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/






