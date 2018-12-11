/************************************************************************************************/
/** @file              : MenuGsmDebug.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年07月21日															*/
/** @date changed      : 2016年07月21日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->打印GSM调试信息												*/
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

#include "Log_GSM.h"

extern BOOL GSM_DEBUG_Flag;

/* 系统设置界面 中文 菜单项目栏 */
static u8 * const Menu_GsmDebug_CN[]=
{
	{"打开"},
	{"关闭"},
};

/* 系统设置界面 英文 菜单项目栏 */
static u8 * const Menu_GsmDebug_EN[]=
{
	{Menu_ON_EN},
	{Menu_OFF_EN},
};

/* 清除历史数据界面 中英文 菜单栏 */
static const struct Menu Menu_GsmDebug[] =
{
	MenuWhat(Menu_GsmDebug_CN),
	MenuWhat(Menu_GsmDebug_EN),
};


/************************************************************************************************/
/* 函数名	: DispMenuGsmDebugInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示清除历史数据界面																*/
/* 创建日期	: 2016年07月21日																		*/
/* 修改日期	: 2016年07月21日																		*/
/************************************************************************************************/
void DispMenuGsmDebugInit(void)
{
   

    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_GsmDebug[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		/* 设置设置项的位置，设置默认状态为不选择升级 */
		if(GSM_DEBUG_Flag == TRUE)
		{
			Item = 0;
		}else
		{
			Item = 1;
		}
        Where = Item;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Stm_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));	

    }

}

/************************************************************************************************/
/* 函数名	: DispMenuGsmDebugUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置清除历史数据功能选项															*/
/* 创建日期	: 2016年07月21日																		*/
/* 修改日期	: 2016年07月21日																		*/
/************************************************************************************************/
void DispMenuGsmDebugUp(void)
{

	switch(Item)
	{
        case 0:
		{
			/* 根据当前用户的级别设置参数 */
			if(User == Domestic)//普通用户
			{	
				/* 显示参数配置错误 提示信息 */
				Parameter_Setting_Failure();
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuGsmDebugIndex,FlashMode_AutoInit);
			}else if((User == Admin)||(User == Super_Admin))
			{
				/* 管理员帐号或超级管理员帐号 */
				
				/* 判断当前打印GSM调试信息状态 */
				if(GSM_DEBUG_Flag == FALSE)
				{
					/* 打开GSM调试信息输出 */
					GSM_DEBUG_Flag = TRUE;
					/* 参数设置成 */
					Parameter_Setting_Successful();
					/* 退回到上一级界面 */
					POP();
				}else
				{
				    /* 将函数指针指向初始化函数,更新显示 */
					Jump2Menu(MenuGsmDebugIndex,FlashMode_NoAction);
				}
			}
			break;
		}
        case 1:
		{
			/* 根据当前用户的级别设置参数 */
			if(User == Domestic)//普通用户
			{	
				/* 显示参数配置错误 提示信息 */
				Parameter_Setting_Failure();
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuGsmDebugIndex,FlashMode_AutoInit);
			}else if((User == Admin)||(User == Super_Admin))
			{
				/* 管理员帐号或超级管理员帐号 */
				
				/* 判断当前打印GSM调试信息状态 */
				if(GSM_DEBUG_Flag == TRUE)
				{
					/* 关闭GSM调试信息输出 */
					GSM_DEBUG_Flag = FALSE;
					/* 参数设置成 */
					Parameter_Setting_Successful();
					/* 退回到上一级界面 */
					POP();
				}else
				{
				    /* 将函数指针指向初始化函数,更新显示 */
					Jump2Menu(MenuGsmDebugIndex,FlashMode_NoAction);
				}
			}
            break;
        }

		default:
		{
            /* 将函数指针指向初始化函数,更新显示 */
			Jump2Menu(MenuGsmDebugIndex,FlashMode_NoAction);
			break;
        }
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuGsmDebugDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出清除历史数据界面																*/
/* 创建日期	: 2016年07月21日																		*/
/* 修改日期	: 2016年07月21日																		*/
/************************************************************************************************/
void DispMenuGsmDebugDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuGsmDebugLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择清除历史数据功能																*/
/* 创建日期	: 2016年07月21日																		*/
/* 修改日期	: 2016年07月21日																		*/
/************************************************************************************************/
void DispMenuGsmDebugLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGsmDebugIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGsmDebugRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择放弃清除历史数据功能															*/
/* 创建日期	: 2016年07月21日																		*/
/* 修改日期	: 2016年07月21日																		*/
/************************************************************************************************/
void DispMenuGsmDebugRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGsmDebugIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGsmDebugOK																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2016年07月21日																		*/
/* 修改日期	: 2016年07月21日																		*/
/************************************************************************************************/
void DispMenuGsmDebugOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGsmDebugIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGsmDebugCancel															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2016年07月21日																		*/
/* 修改日期	: 2016年07月21日																		*/
/************************************************************************************************/
void DispMenuGsmDebugCancel(void) 
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGsmDebugIndex,FlashMode_NoAction);
}

/******************* (C) ZHOU *****END OF FILE****/


