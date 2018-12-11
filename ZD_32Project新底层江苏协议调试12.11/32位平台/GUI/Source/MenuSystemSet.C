/************************************************************************************************/
/** @file              : MenuSystemSet.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月11日															*/
/** @date changed      : 2015年10月08日															*/
/** @brief             : c file	(去掉远程升级功能.)												*/
/** @description       : GUI菜单->系统设置界面													*/
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



/* 系统设置界面 中文 菜单项目栏 */
static u8 * const Menu_Stm_Set_CN[]=
{
{"语言选择"},
{"时钟设置"},
{"仪器版本"},
{"仪器编号"},
{"数据删除"},
{"出厂设置"},
{"Gsm 信息"},
{"系统升级"},
};

/* 系统设置界面 英文 菜单项目栏 */
static u8 * const Menu_Stm_Set_EN[]=
{
{"Language"},
{"Clock"},
{"Version"},
{"Equip ID"},
{"Clear Data"},
{"Factory Set"},
{"Gsm Debug"},
{"System Upgrade"},

};

/* 系统设置界面 中英文 菜单栏 数组 */
const struct Menu Menu_Stm_Set[] =
{
	MenuWhat(Menu_Stm_Set_CN),
	MenuWhat(Menu_Stm_Set_EN),
};


/************************************************************************************************/
/* 函数名	: DispMenuStmSetInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 系统设置 界面信息															*/
/* 创建日期	: 2015/06/11																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuStmSetInit(void)
{

	
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Stm_Set[*Language];
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
/* 函数名	: DispMenuStmSetUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入选择的系统界面																*/
/* 创建日期	: 2015/06/11																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuStmSetUp(void)
{

	switch(Item)
	{
        case 0:
		{	
			/* 进入 系统语言设置 界面 */
			Jump2Menu(MenuStmLgeIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;
        }
		
        case 1:
		{	
			/* 进入 系统时钟 界面 */
			Jump2Menu(MenuStmCokIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;
        }
		
		case 2:
		{	
			/* 进入 系统版本 界面 */
			Jump2Menu(MenuStmVsnIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
		    break;
		}
		
		case 3:
		{	
			/* 进入 仪器编号 界面 */
			Jump2Menu(MenuItuIDIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
		    break;
		}
		
		case 4:
		{	
			/* 进入 清除数据 界面 */
			Jump2Menu(MenuCerHtcDataIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH(); 
		    break;
		}
		
		case 5:
		{	
			/* 进入 恢复出厂设置 界面 */
			Jump2Menu(MenuRteFtySetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
		    break;
		}
		
		case 6:
		{	
			/* 进入 GSM模块调试信息 界面 */
			Jump2Menu(MenuGsmDebugIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
		    break;
		}
		
			case 7://程序升级界面调试2017.11.13
		{	
			/* 进入 固件升级界面 */
			Jump2Menu(MenuFweUreIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
		    break;
		}
		
		
		
		

        default:
		{
		    Jump2Menu(MenuStmSetIndex,FlashMode_NoAction);
			break;
        }
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuStmSetDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出系统设置界面																	*/
/* 创建日期	: 2015/06/11																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuStmSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuStmSetLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择系统项目																	*/
/* 创建日期	: 2015/06/11																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuStmSetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuStmSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuStmSetRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择系统项目																	*/
/* 创建日期	: 2015/06/11																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuStmSetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuStmSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuStmSetOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/11																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuStmSetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuStmSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuStmSetCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/11																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuStmSetCancel(void) 
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuStmSetIndex,FlashMode_NoAction);
}

/******************* (C) ZHOU *****END OF FILE****/


