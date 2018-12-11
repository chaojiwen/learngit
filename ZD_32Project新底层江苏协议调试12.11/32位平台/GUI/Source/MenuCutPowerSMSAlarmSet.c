/************************************************************************************************/
/** @file              : MenuCutPowerSMSAlarmSet.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月29日															*/
/** @date changed      : 2015年09月29日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->断电短信报警开关设置											*/
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

/* 断电短信报警界面 中文 菜单项目栏 */
static u8 * const Menu_Cut_Pwr_SMS_Aam_Set_CN[]=
{
	{Menu_ON_CN},
	{Menu_OFF_CN},		
};

/* 断电短信报警界面 英文 菜单项目栏 */
static u8 * const Menu_Cut_Pwr_SMS_Aam_Set_EN[]=
{
	{Menu_ON_EN},
	{Menu_OFF_EN},		
};

/* 断电短信报警界面 中英文 菜单栏 */
const struct Menu Menu_Cut_Pwr_SMS_Aam_Set[] =
{
	MenuWhat(Menu_Cut_Pwr_SMS_Aam_Set_CN),
	MenuWhat(Menu_Cut_Pwr_SMS_Aam_Set_EN),
};


/* 断电报警开关指针 */
static BOOL *Cut_Pwr_SMS_Aam_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch;


/************************************************************************************************/
/* 函数名	: DispMenuCutPwrSMSAamSetInit													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 断电短信报警 设置信息														*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetInit(void)
{
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_Cut_Pwr_SMS_Aam_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		Item = (*Cut_Pwr_SMS_Aam_Sth_P  == TRUE) ? (0u) : (1u);		
        Where = Item;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_SMS_Aam[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuCutPwrSMSAamSetUp														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置 断电短信报警 功能开关														*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetUp(void)
{

    switch(Item)
	{
        case 0:
		{
			/* 判断短信断电报警状态. */
			if(*Cut_Pwr_SMS_Aam_Sth_P == FALSE)
			{
				/* 短信报警状态为:关,开启声光报警. */
				*Cut_Pwr_SMS_Aam_Sth_P = TRUE;

				/* 修改 仪器 信息表 设置标志 */
				Write_Config_Tab((u8*)Cut_Pwr_SMS_Aam_Sth_P,sizeof(*Cut_Pwr_SMS_Aam_Sth_P));
				
				/* 退回到上一级界面 */
				POP();
			}else
			{
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
			}
			break;
        }
		
        case 1:
		{
			/* 判断短信断电报警状态. */
			if(*Cut_Pwr_SMS_Aam_Sth_P == TRUE)
			{
				/* 短信报警状态为:开,关闭声光报警. */
				*Cut_Pwr_SMS_Aam_Sth_P = FALSE;
				
				/* 修改 仪器 信息表 设置标志 */
				Write_Config_Tab((u8*)Cut_Pwr_SMS_Aam_Sth_P,sizeof(*Cut_Pwr_SMS_Aam_Sth_P));
				
				/* 退回到上一级界面 */
				POP();
			}else
			{
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
			}
	        break;
        }


        default:
		{
			/* 未定义界面,设置界面。 */
			/* 将函数指针指向初始化函数 */
			Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
			break;
		}
    }


}

/************************************************************************************************/
/* 函数名	: DispMenuCutPwrSMSAamSetDown													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 断电短信报警 界面															*/
/* 创建日期	: 2015/09/29																	*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuCutPwrSMSAamSetLeft													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "开" 断电短信报警 功能														*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuCutPwrSMSAamSetRight														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 "闭" 断电短信报警 功能																	*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuCutPwrSMSAamSetOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuCutPwrSMSAamSetCancel															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/29																			*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/


