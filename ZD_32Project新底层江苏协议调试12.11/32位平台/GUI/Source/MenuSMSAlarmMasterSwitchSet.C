/************************************************************************************************/
/** @file              : MenuSMSAlarmMasterSwitchSetIndex.c								*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月29日															*/
/** @date changed      : 2015年09月29日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单 -> 短信报警总开关设置界面										*/
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

/* 短信报警总开关设置界面 中文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_Mtr_Sth_Set_CN[]=
{
	{Menu_ON_CN},
	{Menu_OFF_CN},
};

/* 短信报警总开关设置界面 英文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_Mtr_Sth_Set_EN[]=
{
	{Menu_ON_EN},
	{Menu_OFF_EN},
};

/* 短信报警总开关设置界面 中英文 菜单栏 */
const struct Menu Menu_SMS_Aam_Mtr_Sth_Set[] =
{
	MenuWhat(Menu_SMS_Aam_Mtr_Sth_Set_CN),
	MenuWhat(Menu_SMS_Aam_Mtr_Sth_Set_EN),
};

/* "短信报警"总开关状态指针 */
#define SMS_Aam_Mtr_Sth_P 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch)

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamMtrSthSetInit													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 短信报警总开关设置 信息														*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamMtrSthSetInit(void)
{
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_SMS_Aam_Mtr_Sth_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		/* 设置固定通道. */
		Item = (*SMS_Aam_Mtr_Sth_P == TRUE) ? 0x0 : 0x1;
    	Where = Item;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_SMS_Aam[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamMtrSthSetUp														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: "设置"短信报警总开关.																*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSMSAamMtrSthSetUp(void)
{

	switch(Item)
	{
		case 0:
		{
			/* 判断短信报警总开关状态 */
			if((*SMS_Aam_Mtr_Sth_P == FALSE))
			{
				/* 短信报警总开关状态为:关,开启报警. */
				*SMS_Aam_Mtr_Sth_P = TRUE;
	
				/* 修改 仪器 信息表 设置标志 */
				Write_Config_Tab((u8*)SMS_Aam_Mtr_Sth_P,sizeof(*SMS_Aam_Mtr_Sth_P));
				/* 退回到上一级界面 */
				POP();
			}else
			{
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuSMSAamMtrSthSetIndex,FlashMode_NoAction);
			}
			break;
		}
		
		case 1:
		{
			/* 判断短信报警总开关状态 */
			if(*SMS_Aam_Mtr_Sth_P == TRUE)
			{
				/* 短信报警总开关态为:开,关闭报警. */
				*SMS_Aam_Mtr_Sth_P = FALSE;
				
				/* 修改 仪器 信息表 设置标志 */
				Write_Config_Tab((u8*)SMS_Aam_Mtr_Sth_P,sizeof(*SMS_Aam_Mtr_Sth_P));
				/* 退回到上一级界面 */
				POP();
			}else
			{
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuSMSAamMtrSthSetIndex,FlashMode_NoAction);
			}
			break;
		}
	
		default:
		{
			/* 未定义界面,设置界面。 */
			/* 将函数指针指向初始化函数 */
			Jump2Menu(MenuSMSAamMtrSthSetIndex,FlashMode_NoAction);
			break;
		}
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamMtrSthSetDown													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: "退出"短信报警总开关																*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamMtrSthSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamMtrSthSetLeft													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "开" 短信报警总开关															*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamMtrSthSetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamMtrSthSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamMtrSthSetRight													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "关" 声光报警总开关															*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSMSAamMtrSthSetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamMtrSthSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamMtrSthSetOK														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSMSAamMtrSthSetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamMtrSthSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamMtrSthSetCancel													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/09/28																		*/
/************************************************************************************************/
void DispMenuSMSAamMtrSthSetCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamMtrSthSetIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/






