/************************************************************************************************/
/** @file              : MenuPrintSet.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月15日															*/
/** @date changed      : 2015年10月13日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->打印设置														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_Print.h"

#if (Print_Function_Enable == 0x01)

#include "Ks0108.H"
#include "Menu.H"




/* 打印界面 中文 菜单项目栏 */
static u8 * const Menu_Pit_Set_CN[] = 
{
	{"打印起始时间"},
	{"打印结束时间"},
	{"打印时间间隔"},
	{"打印数据"},
};

/* 打印界面 英文 菜单项目栏 */
static u8 * const Menu_Pit_Set_EN[] = 
{
	{"Pit Start Time"},
	{"Pit End Time"},
	{"Pit Interval"},
	{"Pit Data"},
};

/* 打印设置菜单项目栏 */
const struct Menu Menu_Pit_Set[] = 
{
	MenuWhat(Menu_Pit_Set_CN),
	MenuWhat(Menu_Pit_Set_EN),
};


/************************************************************************************************/
/* 函数名	: DispMenuPitSetInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示打印界面信息																	*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuPitSetInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Pit_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		if(Flash != FlashMode_ReFlash)
		{
			/* 设置项目光标 */
			Item = 0;
			Where = 0;			
		}
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Ico[*Language].Text+ItemBackup[ItemBackup_i-1]));	

	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuPitSetUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入选择的打印界面																*/
/* 创建日期	: 2015/06/15																			*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuPitSetUp(void)
{

	switch(Item)
	{
        case 0:
		{
			/* 进入 打印起始时间设置 界面 */
            Jump2Menu(MenuPitSatTimeSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }

        case 1:
		{
			/* 进入 打印结束时间设置 界面 */
            Jump2Menu(MenuPitEndTimeSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }

        case 2:
		{
			/* 进入 打印时间间隔设置 界面 */
            Jump2Menu(MenuPitTimeIrlSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }

        case 3:
		{
			/* 进入 打印数据设置 界面 */
            Jump2Menu(MenuPitDataIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }


		
        default:
		{
			/* 未定义界面,设置界面。 */
            Jump2Menu(MenuPitSetIndex,FlashMode_NoAction);
			break;
		}
    }
}

/************************************************************************************************/
/* 函数名	: DispMenuPitSetDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出打印界面																		*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuPitSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuPitSetLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择打印界面的项目															*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuPitSetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuPitSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitSetRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择打印界面的项目															*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuPitSetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuPitSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitSetOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/15																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuPitSetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuPitSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitSetCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/4																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuPitSetCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuPitSetIndex,FlashMode_NoAction);
}  

#endif
/******************* (C) COPYRIGHT 2015 HZ Zeda Team ****END OF FILE****/

