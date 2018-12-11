/************************************************************************************************/
/** @file              : MenuLowPowerSMSAlarmSet.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月29日															*/
/** @date changed      : 2015年09月29日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->低电量短信报警设置界面										*/
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

/* 低电量短信报警界面 中文 菜单项目栏 */
static u8 * const Menu_Low_Pwr_SMS_Aam_Set_CN[]=
{
	{Menu_ON_CN},
	{Menu_OFF_CN},		
};

/* 低电量短信报警界面 英文 菜单项目栏 */
static u8 * const Menu_Low_Pwr_SMS_Aam_Set_EN[]=
{
	{Menu_ON_EN},
	{Menu_OFF_EN},		
};

/* 低电量短信报警界面 中英文 菜单栏 */
const struct Menu Menu_Low_Pwr_SMS_Aam_Set[] =
{
	MenuWhat(Menu_Low_Pwr_SMS_Aam_Set_CN),
	MenuWhat(Menu_Low_Pwr_SMS_Aam_Set_EN),
};


/* 低电量 "短信" 报警开关指针 */
static BOOL *Low_Pwr_SMS_Aam_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch;


/************************************************************************************************/
/* 函数名	: DispMenuLowPwrSMSAamSetInit														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 低电量短信报警 设置信息														*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuLowPwrSMSAamSetInit(void)
{
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_Low_Pwr_SMS_Aam_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		Item = (*Low_Pwr_SMS_Aam_Sth_P  == TRUE) ? (0u) : (1u);		
        Where = Item;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_SMS_Aam[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuLowPwrSMSAamSetUp															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置 低电量短信报警 功能开关														*/
/* 创建日期	: 2015/09/21																		*/
/************************************************************************************************/
void DispMenuLowPwrSMSAamSetUp(void)
{

    switch(Item)
	{
        case 0:
		{
			/* 判断低电量 "短信" 报警状态 */
			if(*Low_Pwr_SMS_Aam_Sth_P == FALSE)
			{
				/* 短信报警状态为:关,开启声光报警. */
				*Low_Pwr_SMS_Aam_Sth_P = TRUE;

				/* 修改 仪器 信息表 设置标志 */
				Write_Config_Tab((u8*)Low_Pwr_SMS_Aam_Sth_P,sizeof(*Low_Pwr_SMS_Aam_Sth_P));
				/* 退回到上一级界面 */
				POP();
			}else
			{
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuLowPwrSMSAamSetIndex,FlashMode_NoAction);
			}
			break;
        }
		
        case 1:
		{
			/* 判断低电量 "短信" 报警状态 */
			if(*Low_Pwr_SMS_Aam_Sth_P == TRUE)
			{
				/* 短信报警状态为:开,关闭声光报警. */
				*Low_Pwr_SMS_Aam_Sth_P = FALSE;
				
				/* 修改 仪器 信息表 设置标志 */
				Write_Config_Tab((u8*)Low_Pwr_SMS_Aam_Sth_P,sizeof(*Low_Pwr_SMS_Aam_Sth_P));
				
				/* 退回到上一级界面 */
				POP();
			}else
			{
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuLowPwrSMSAamSetIndex,FlashMode_NoAction);
			}
	        break;
        }


        default:
		{
			/* 未定义界面,设置界面。 */
			/* 将函数指针指向初始化函数 */
			Jump2Menu(MenuLowPwrSMSAamSetIndex,FlashMode_NoAction);
			break;
		}
    }


}

/************************************************************************************************/
/* 函数名	: DispMenuLowPwrSMSAamSetDown														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 低电量声光报警 界面															*/
/* 创建日期	: 2015/09/22																		*/
/************************************************************************************************/
void DispMenuLowPwrSMSAamSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuLowPwrSMSAamSetLeft														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 打开 低电量声光报警 功能														*/
/* 创建日期	: 2015/09/22																		*/
/************************************************************************************************/
void DispMenuLowPwrSMSAamSetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuLowPwrSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuLowPwrSMSAamSetRight														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 关闭 低电量声光报警 功能														*/
/* 创建日期	: 2015/09/22																		*/
/************************************************************************************************/
void DispMenuLowPwrSMSAamSetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuLowPwrSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuLowPwrSMSAamSetOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/09/22																		*/
/************************************************************************************************/
void DispMenuLowPwrSMSAamSetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuLowPwrSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuLowPwrSudLgtAamSetCancel													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/09/22																		*/
/************************************************************************************************/
void DispMenuLowPwrSMSAamSetCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuLowPwrSMSAamSetIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/

