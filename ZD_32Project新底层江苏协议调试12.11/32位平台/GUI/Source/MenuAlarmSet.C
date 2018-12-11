/************************************************************************************************/
/** @file              : MenuAlarmSet.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年9月1日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->设置界面														*/
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



/* 设置界面 中文 菜单项目栏 */
static u8 * const Menu_Alarm_Set_CN[]=
{
	{"通道阈值"},
	{"声光报警"},
	{"短信报警"},
	{"报警延时"},
};

/* 设置界面 英文 菜单项目栏 */
static u8 * const Menu_Alarm_Set_EN[]=
{
	{"Cha Tsd"},
	{"Sud Lgt Alarm"},
	{"SMS Alarm"},
	{"Alarm Delay"},
};

/* 设置界面 中英文 菜单栏 */
const struct Menu Menu_Alarm_Set[] =
{
	MenuWhat(Menu_Alarm_Set_CN),
	MenuWhat(Menu_Alarm_Set_EN),
};


/************************************************************************************************/
/* 函数名	: DispMenuAlarmSetInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示设置界面信息																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuAlarmSetInit(void)
{
	
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Alarm_Set[*Language];
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
/* 函数名	: DispMenuAlarmSetUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入选择的设置界面																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuAlarmSetUp(void)
{

    switch(Item)
	{
		case 0:
		{
			/* 进入 报警上下限设置  通道选择界面 */
            Jump2Menu(MenuChaTsdIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		case 1:
		{
			/* 进入声光报警设置界面 */
            Jump2Menu(MenuSudLgtAamIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
        case 2:
		{
			/* 进入短信报警设置界面 */
            Jump2Menu(MenuSMSAamIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
        case 3:
		{
			/* 进入 报警报警延时时间 设置界面 */
            Jump2Menu(MenuAamDlyIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }


        default:
		{
			/* 未定义界面,设置界面。 */
            Jump2Menu(MenuAlarmSetIndex,FlashMode_NoAction);
            break;
        }
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuAlarmSetDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出设置界面																		*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuAlarmSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuAlarmSetLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择设置界面的项目																*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuAlarmSetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuAlarmSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuAlarmSetRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择设置界面的项目																*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuAlarmSetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuAlarmSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuAlarmSetOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）															*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuAlarmSetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuAlarmSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuAlarmSetCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）															*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuAlarmSetCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuAlarmSetIndex,FlashMode_NoAction);
}   

/******************* (C) ZHOU *****END OF FILE****/
 

