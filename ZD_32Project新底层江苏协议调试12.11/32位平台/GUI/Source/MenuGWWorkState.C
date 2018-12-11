/************************************************************************************************/
/** @file              : MenuGWWorkState.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月30日															*/
/** @date changed      : 2016年11月30日															*/
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

#if (Gateway_NJSY_Enable == 0x01)

#include "Ks0108.H"
#include "Menu.H"

#include "Mcu_Config.h"

/* 网关状态寄存器指针 */
#define MenuGWWorkStatep			(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.WorkState)
/* 显示网关状态字符串X位置 */
#define MenuGWWorkStateStr_PosX		(48u)
/* 显示网关状态字符串Y位置 */
#define MenuGWWorkStateStr_PosY		(Line_2_PosY)

/* 系统版本界面 中文 菜单项目栏 */
u8 * const MenuGWWorkState_CN[]=
{
	{"故障"},
	{"正常"},
};

/* 系统版本界面 英文 菜单项目栏 */
u8 * const MenuGWWorkState_EN[]=
{
	{"Fault"},
	{"Normal"},
};

/************************************************************************************************/
/* 函数名	: DispMenuGWWorkStateInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示网关状态界面信息																*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWWorkStateInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		
		/* 判断当前的语言 */
		if(*Language)
		{
			/* 英文 */
			/* 显示设备状态 */
			GUI_DispStringAt(MenuGWWorkState_EN[*MenuGWWorkStatep],(MenuGWWorkStateStr_PosX-8),MenuGWWorkStateStr_PosY);
		}else
		{
			/* 中文 */
			/* 显示设备状态 */
			GUI_DispStringAt(MenuGWWorkState_CN[*MenuGWWorkStatep],MenuGWWorkStateStr_PosX,MenuGWWorkStateStr_PosY);
		}
		
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(MenuGW_Item[*Language].Text+ItemBackup[ItemBackup_i-1]));
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWWorkStateUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWWorkStateUp(void)
{

	switch(Item)
	{
        default:
		{
			Jump2Menu(MenuGWWorkStateIndex,FlashMode_NoAction);
			break;
        }
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuGWWorkStateDown																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出网关界面																		*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWWorkStateDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuGWWorkStateLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择网关界面的项目															*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWWorkStateLeft(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWWorkStateIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWWorkStateRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择网关界面的项目															*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWWorkStateRight(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWWorkStateIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWWorkStateOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWWorkStateOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWWorkStateIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWWorkStateCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWWorkStateCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWWorkStateIndex,FlashMode_NoAction);
}  


#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
