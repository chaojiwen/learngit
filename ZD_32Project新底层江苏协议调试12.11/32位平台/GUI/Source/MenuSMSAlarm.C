/************************************************************************************************/
/** @file              : MenuSMSAlarm.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月09日															*/
/** @Change date       : 2015年09月29日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->短信报警														*/
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

#include <string.h>
#include <stdio.h>

/* "报警总开关" 中文 字符串 */
#define Menu_Mtr_Sth_Str_CN	"总开关"
/* "低电量报警" 中文 字符串 */
#define Menu_Low_Pwr_Str_CN	"低电量报警"
/* "断电报警"   中文 字符串 */
#define Menu_Cut_Pwr_Str_CN	"断电报警"

/* "报警总开关" 英文 字符串 */
#define Menu_Mtr_Sth_Str_EN	"Mtr Sth"
/* "低电量报警" 英文 字符串 */
#define Menu_Low_Pwr_Str_EN	"Low Power"
/* "断电报警"   英文 字符串 */
#define Menu_Cut_Pwr_Str_EN	"Cut Power"


/* "报警总开关" 项目 中文 字符串内容 */
static u8 Menu_Mtr_Sth_Item_Str[17];
/* "低电量报警" 项目 中文 字符串内容 */
static u8 Menu_Low_Pwr_Item_Str[17];
/* "断电报警" 项目 中文 字符串内容 */
static u8 Menu_Cut_Pwr_Item_Str[17];


/* 短信报警界面 中文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_CN[]=
{
	{Menu_Mtr_Sth_Item_Str},
	{Menu_Low_Pwr_Item_Str},
	{Menu_Cut_Pwr_Item_Str},
	{"通道报警"},
	{"报警号码"},
	{"重复报警间隔"},
};

/* 短信报警界面 英文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_EN[]=
{
	{Menu_Mtr_Sth_Item_Str},
	{Menu_Low_Pwr_Item_Str},
	{Menu_Cut_Pwr_Item_Str},
	{"Channel"},
	{"Phone NO"},
	{"Rpt Aam Irl"},
};


/* 短信报警界面 中英文 菜单栏 */
const struct Menu Menu_SMS_Aam[] =
{
	MenuWhat(Menu_SMS_Aam_CN),
	MenuWhat(Menu_SMS_Aam_EN),
};


/* 短信报警总开关 指针 */
#define Mtr_SMS_Sth_P 		 (&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch)

/* 低电量 "短信" 报警开关 指针 */
#define Low_Pwr_SMS_Sth_P 	 (&Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch)

/* 断电 "短信" 报警开关 指针 */
#define Cut_Pwr_SMS_Sth_P 	 (&Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch)


/************************************************************************************************/
/* 函数名	: Set_Item_Bar_Str																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置项目条字符串																	*/
/* 创建日期	: 2015/09/29																			*/
/************************************************************************************************/
static void Set_Item_Bar_Str(u8 *str,char *Item,char * aam_state)
{
	/* 设置 项目字符串. */
	sprintf((char*)str,"%s(%s)",Item,aam_state);
}


/************************************************************************************************/
/* 函数名	: DispMenuSMSAamInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 短信报警界面 设置项目														*/
/* 创建日期	: 2015/06/04																		*/
/* 修改日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamInit(void)
{
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		
		/* 设置  报警总开关 项目字符串 */
		Set_Item_Bar_Str(Menu_Mtr_Sth_Item_Str,
						((*Language == CN) ? Menu_Mtr_Sth_Str_CN : Menu_Mtr_Sth_Str_EN),
						(*Mtr_SMS_Sth_P  == TRUE) ? 
						((*Language == CN) ? Menu_ON_CN  : Menu_ON_EN) : 
						((*Language == CN) ? Menu_OFF_CN : Menu_OFF_EN));

		/* 设置  低电量短信报警 项目字符串 */
		Set_Item_Bar_Str(Menu_Low_Pwr_Item_Str,
						((*Language == CN) ? Menu_Low_Pwr_Str_CN : Menu_Low_Pwr_Str_EN),
						((*Low_Pwr_SMS_Sth_P  == TRUE)&&(*Low_Pwr_SMS_Sth_P == TRUE)) ? 
						((*Language == CN) ? Menu_ON_CN  : Menu_ON_EN) : 
						((*Language == CN) ? Menu_OFF_CN : Menu_OFF_EN));
						
		/* 设置  断电短信报警 项目字符串 */
		Set_Item_Bar_Str(Menu_Cut_Pwr_Item_Str,
						((*Language == CN) ? Menu_Cut_Pwr_Str_CN : Menu_Cut_Pwr_Str_EN),
						((*Cut_Pwr_SMS_Sth_P  == TRUE)&&(*Cut_Pwr_SMS_Sth_P == TRUE)) ? 
						((*Language == CN) ? Menu_ON_CN	: Menu_ON_EN) : 
						((*Language == CN) ? Menu_OFF_CN : Menu_OFF_EN));

		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_SMS_Aam[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		Item = 0;		
		Where = 0;

		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Alarm_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));	
    }
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 短信报警功能 项目																*/
/* 创建日期	: 2015/06/04																		*/
/* 修改日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamUp(void)
{
 
    switch(Item)
	{
		case 0:
		{
			/* 进入 短信报警总开关 设置界面 */
			Jump2Menu(MenuSMSAamMtrSthSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
			break;
		}

		case 1:
		{
			/* 进入 低电量短信报警 设置界面 */
            Jump2Menu(MenuLowPwrSMSAamSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;
        }
		
        case 2:
		{
			/* 进入 断电短信报警 界面 */
            Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;

        }

        case 3:
		{
			/* 进入 短信报警通道选择 界面 */
            Jump2Menu(MenuSMSAamChaSecIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;

        }

		case 4:
		{
			/* 进入 短信报警号码 界面 */
            Jump2Menu(MenuSMSAamPoeNOIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;
        }

		case 5:
		{
			/* 进入 短信重复报警间隔 界面 */
            Jump2Menu(MenuSMSAamRptIrlIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;
        }


        default:
		{
			/* 未定义界面,设置界面。 */
			Jump2Menu(MenuSMSAamIndex,FlashMode_NoAction);
			break;
		}
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 短信报警 界面																*/
/* 创建日期	: 2015/06/04																		*/
/* 修改日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamDown()
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 前面一个短信功能选项 功能																*/
/* 创建日期	: 2015/06/04																			*/
/* 修改日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamLeft()
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 后面一个短信功能选项 功能																	*/
/* 创建日期	: 2015/06/04																			*/
/* 修改日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamRight()
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSoundLightAamOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/04																		*/
/* 修改日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSoundLightAamOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/04																		*/
/* 修改日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/

