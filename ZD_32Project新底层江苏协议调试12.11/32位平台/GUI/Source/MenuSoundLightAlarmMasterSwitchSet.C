/************************************************************************************************/
/** @file              : MenuSoundLightAlarmMasterSwitchSetIndex.c								*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月29日															*/
/** @date changed      : 2015年09月29日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单 -> 声光报警总开关设置界面										*/
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

/* 声光报警总开关设置界面 中文 菜单项目栏 */
static u8 * const Menu_Sud_Lgt_Aam_Mtr_Sth_Set_CN[]=
{
	{Menu_ON_CN},
	{Menu_OFF_CN},		
};

/* 声光报警总开关设置界面 英文 菜单项目栏 */
static u8 * const Menu_Sud_Lgt_Aam_Mtr_Sth_Set_EN[]=
{
	{Menu_ON_EN},
	{Menu_OFF_EN},		
};

/* 声光报警总开关设置界面 中英文 菜单栏 */
const struct Menu Menu_Sud_Lgt_Aam_Mtr_Sth_Set[] =
{
	MenuWhat(Menu_Sud_Lgt_Aam_Mtr_Sth_Set_CN),
	MenuWhat(Menu_Sud_Lgt_Aam_Mtr_Sth_Set_EN),
};

/* 报警总开关 "声" 状态指针 */
#define Sud_Aam_Mtr_Sth_P 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)
/* 报警总开关 "光" 状态指针 */
#define Lgt_Aam_Mtr_Sth_P	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch)

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamMtrSthSetInit													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 声光报警总开关设置 信息														*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSudLgtAamMtrSthSetInit(void)
{
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_Sud_Lgt_Aam_Mtr_Sth_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		/* 设置固定通道. */
		Item = ((*Sud_Aam_Mtr_Sth_P == TRUE)&&(*Lgt_Aam_Mtr_Sth_P == TRUE)) ? 0x0 : 0x1;
    	Where = Item;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Sud_Lgt_Aam[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamMtrSthSetUp														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: "设置"声光报警总开关.																*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSudLgtAamMtrSthSetUp(void)
{

	switch(Item)
	{
		case 0:
		{
			/* 判断声光报警总开关状态 */
			if((*Sud_Aam_Mtr_Sth_P == FALSE)||(*Lgt_Aam_Mtr_Sth_P == FALSE))
			{
				/* 声光报警总开关状态为:关,开启 通道 声光报警. */
				*Sud_Aam_Mtr_Sth_P = TRUE;
				*Lgt_Aam_Mtr_Sth_P = TRUE;
	
				/* 修改 仪器 信息表 设置标志 */
				Write_Config_Tab((u8*)Sud_Aam_Mtr_Sth_P,sizeof(*Sud_Aam_Mtr_Sth_P));
				Write_Config_Tab((u8*)Lgt_Aam_Mtr_Sth_P,sizeof(*Lgt_Aam_Mtr_Sth_P));
				/* 退回到上一级界面 */
				POP();
			}else
			{
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuSudLgtAamMtrSthSetIndex,FlashMode_NoAction);
			}
			break;
		}
		
		case 1:
		{
			/* 判断声光报警总开关状态 */
			if((*Sud_Aam_Mtr_Sth_P == TRUE)||(*Lgt_Aam_Mtr_Sth_P == TRUE))
			{
				/* 声光报警总开关态为:开,关闭声光报警. */
				*Sud_Aam_Mtr_Sth_P = FALSE;
				*Lgt_Aam_Mtr_Sth_P = FALSE;
				
				/* 修改 仪器 信息表 设置标志 */
				Write_Config_Tab((u8*)Sud_Aam_Mtr_Sth_P,sizeof(*Sud_Aam_Mtr_Sth_P));
				Write_Config_Tab((u8*)Lgt_Aam_Mtr_Sth_P,sizeof(*Lgt_Aam_Mtr_Sth_P));
				/* 退回到上一级界面 */
				POP();
			}else
			{
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuSudLgtAamMtrSthSetIndex,FlashMode_NoAction);
			}
			break;
		}
	
		default:
		{
			/* 未定义界面,设置界面。 */
			/* 将函数指针指向初始化函数 */
			Jump2Menu(MenuSudLgtAamMtrSthSetIndex,FlashMode_NoAction);
			break;
		}
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamMtrSthSetDown													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: "退出"声光报警总开关																*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSudLgtAamMtrSthSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamMtrSthSetLeft													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "开" 声光报警总开关															*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSudLgtAamMtrSthSetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamMtrSthSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamMtrSthSetRight													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "关" 声光报警总开关															*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSudLgtAamMtrSthSetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamMtrSthSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamMtrSthSetOK														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSudLgtAamMtrSthSetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamMtrSthSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamMtrSthSetCancel													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSudLgtAamMtrSthSetCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamMtrSthSetIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/






