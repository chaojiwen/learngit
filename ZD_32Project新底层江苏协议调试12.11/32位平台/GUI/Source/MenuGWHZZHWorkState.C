/************************************************************************************************/
/** @file              : MenuGWHZZHWorkState.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017年01月11日															*/
/** @date changed      : 2017年01月11日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->网关工作状态显示												*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "System_Config.h"

#if (Gateway_HZZH_Enable == 0x01)

#include "Ks0108.H"
#include "Menu.H"
#include "App_Gateway_HZZH.h"


/* 显示网关状态字符串X位置 */
#define MenuGWHZZHWorkStateStr_PosX		(48u)
/* 显示网关状态字符串Y位置 */
#define MenuGWHZZHWorkStateStr_PosY		(Line_2_PosY)

/* 系统版本界面 中文 菜单项目栏 */
u8 * const MenuGWHZZHWorkState_CN[]=
{
	{"故障"},
	{"正常"},
};

/* 系统版本界面 英文 菜单项目栏 */
u8 * const MenuGWHZZHWorkState_EN[]=
{
	{"Fault"},
	{"Normal"},
};

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWorkStateInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示网关状态界面信息																*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWorkStateInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 网关工作状态 */
		GWHZZH_Result mGWHZZHWorkState;
		
		
		/* 清除屏幕显示 */
		GUI_Clear();
		
		/* 读当前杭州创辉网关状态 */
		mGWHZZHWorkState = AppGWHZZH_API.GetState();
		
		/* 判断当前的语言 */
		if(*Language)
		{
			/* 英文 */
			if(GWHZZH_Success == mGWHZZHWorkState)
			{
				/* 显示设备状态 */
				GUI_DispStringAt(MenuGWHZZHWorkState_EN[1],(MenuGWHZZHWorkStateStr_PosX-8),MenuGWHZZHWorkStateStr_PosY);
			}else
			{
				/* 显示设备状态 */
				GUI_DispStringAt(MenuGWHZZHWorkState_EN[0],(MenuGWHZZHWorkStateStr_PosX-8),MenuGWHZZHWorkStateStr_PosY);
			}
		}else
		{
			/* 中文 */
			if(GWHZZH_Success == mGWHZZHWorkState)
			{
				/* 显示设备状态 */
				GUI_DispStringAt(MenuGWHZZHWorkState_CN[1],MenuGWHZZHWorkStateStr_PosX,MenuGWHZZHWorkStateStr_PosY);
			}else
			{
				/* 显示设备状态 */
				GUI_DispStringAt(MenuGWHZZHWorkState_CN[0],MenuGWHZZHWorkStateStr_PosX,MenuGWHZZHWorkStateStr_PosY);
			}
		}
		
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(MenuGWHZZH_Item[*Language].Text+ItemBackup[ItemBackup_i-1]));
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWorkStateUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWorkStateUp(void)
{

	switch(Item)
	{
        default:
		{
			Jump2Menu(MenuGWHZZHWorkStateIndex,FlashMode_NoAction);
			break;
        }
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWorkStateDown																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出网关界面																		*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWorkStateDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWorkStateLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择网关界面的项目															*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWorkStateLeft(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWHZZHWorkStateIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWorkStateRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择网关界面的项目															*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWorkStateRight(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWHZZHWorkStateIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWorkStateOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWorkStateOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWHZZHWorkStateIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWorkStateCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWorkStateCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWHZZHWorkStateIndex,FlashMode_NoAction);
}  


#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
