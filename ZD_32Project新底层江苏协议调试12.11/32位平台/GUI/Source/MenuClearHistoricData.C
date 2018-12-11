/************************************************************************************************/
/** @file              : MenuClearHistoricData.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月15日															*/
/** @date changed      : 2015年10月14日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->清除历史数据													*/
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

/* 清除历史数据界面 中英文 菜单栏 */
static const struct Menu Menu_Cer_Htc_Data[] =
{
	MenuWhat(Menu_Yes_NO_CN),
	MenuWhat(Menu_Yes_NO_EN),
};


/************************************************************************************************/
/* 函数名	: DispMenuCerHtcDataInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示清除历史数据界面																*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuCerHtcDataInit(void)
{
   

    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Cer_Htc_Data[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		/* 设置设置项的位置，设置默认状态为不选择升级 */
        Item = 1;
        Where = Item;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Stm_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));	

    }

}

/************************************************************************************************/
/* 函数名	: DispMenuCerHtcDataUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置清除历史数据功能选项															*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuCerHtcDataUp(void)
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
				Jump2Menu(MenuCerHtcDataIndex,FlashMode_AutoInit);
			}else if(User == Admin)
			{
				/* 管理员帐号 */
				Mcu_Cer_Htc_Data();
				/* 参数设置成 */
				Parameter_Setting_Successful();
				/* 退回到上一级界面 */
				POP();
			}else if(User == Super_Admin)
			{
				/* 将函数指针指向初始化函数,更新显示 */
				Jump2Menu(MenuCerHtcDataIndex,FlashMode_NoAction);
			}
			
			break;
		}
        case 1:
		{
			/* 退回到上一级界面 */
			POP();
            break;
        }

		default:
		{
            /* 将函数指针指向初始化函数,更新显示 */
			Jump2Menu(MenuCerHtcDataIndex,FlashMode_NoAction);
			break;
        }
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuCerHtcDataDown															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出清除历史数据界面																*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuCerHtcDataDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuCerHtcDataLeft															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择清除历史数据功能																*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuCerHtcDataLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuCerHtcDataIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuCerHtcDataRight															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择放弃清除历史数据功能															*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuCerHtcDataRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuCerHtcDataIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuCerHtcDataOK																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuCerHtcDataOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuCerHtcDataIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuCerHtcDataCancel															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuCerHtcDataCancel(void) 
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuCerHtcDataIndex,FlashMode_NoAction);
}

/******************* (C) ZHOU *****END OF FILE****/


