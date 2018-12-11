/************************************************************************************************/
/** @file              : MenuSoundLightAlarm.c													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月16日															*/
/** @date changed      : 2015年09月21日 完成													*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->声光报警 文件													*/
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
#define Menu_Mtr_Sth_Str_CN		"总开关"
/* "低电量报警" 中文 字符串 */
#define Menu_Low_Power_Str_CN	"低电量报警"
/* "断电报警"   中文 字符串 */
#define Menu_Cut_Power_Str_CN	"断电报警"

/* "报警总开关" 英文 字符串 */
#define Menu_Mtr_Sth_Str_EN		"Mtr Sth"
/* "低电量报警" 英文 字符串 */
#define Menu_Low_Power_Str_EN	"Low Power"
/* "断电报警"   英文 字符串 */
#define Menu_Cut_Power_Str_EN	"Cut Power"


/* "报警总开关" 项目 中文 字符串内容 */
static u8 Menu_Mtr_Sth_Item_Str[17];
/* "低电量报警" 项目 中文 字符串内容 */
static u8 Menu_Low_Power_Item_Str[17];
/* "断电报警" 项目 中文 字符串内容 */
static u8 Menu_Cut_Power_Item_Str[17];


/* 声光报警界面 中文 菜单项目栏 */
static u8 * const Menu_Sud_Lgt_Aam_CN[]=
{
	{Menu_Mtr_Sth_Item_Str},
	{Menu_Low_Power_Item_Str},
	{Menu_Cut_Power_Item_Str},
	{"通道报警"},
};

/* 声光报警界面 英文 菜单项目栏 */
static u8 * const Menu_Sud_Lgt_Aam_EN[]=
{
	{Menu_Mtr_Sth_Item_Str},
	{Menu_Low_Power_Item_Str},
	{Menu_Cut_Power_Item_Str},
	{"Channel"},
};


/* 声光报警界面 中英文 菜单栏 */
const struct Menu Menu_Sud_Lgt_Aam[] =
{
	MenuWhat(Menu_Sud_Lgt_Aam_CN),
	MenuWhat(Menu_Sud_Lgt_Aam_EN),
};


/* 报警总开关 "声" 报警开关 指针 */
static BOOL *Mtr_Sud_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch;
/* 报警总开关 "光" 报警开关 指针 */
static BOOL *Mtr_Lgt_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch;


/* 低电量 "声" 报警开关 指针 */
static BOOL *Low_Pwr_Sud_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch;
/* 低电量 "光" 报警开关 指针 */
static BOOL *Low_Pwr_Lgt_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch;


/* 断电 "声" 报警开关 指针 */
static BOOL *Cut_Pwr_Sud_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch;
/* 断电 "光" 报警开关 指针 */
static BOOL *Cut_Pwr_Lgt_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch;



/************************************************************************************************/
/* 函数名	: Set_Item_Bar_Str																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置项目条字符串																	*/
/* 创建日期	: 2015/09/16																		*/
/************************************************************************************************/
static void Set_Item_Bar_Str(u8 *str,char *Item,char * aam_state)
{
	/* 设置 项目字符串. */
	sprintf((char*)str,"%s(%s)",Item,aam_state);
}


/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 声光报警界面 设置项目														*/
/* 创建日期	: 2015/09/21																		*/
/************************************************************************************************/
void DispMenuSudLgtAamInit(void)
{
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置  报警总开关 项目字符串 */
		Set_Item_Bar_Str(Menu_Mtr_Sth_Item_Str,
						((*Language == CN) ? Menu_Mtr_Sth_Str_CN : Menu_Mtr_Sth_Str_EN),
						((*Mtr_Sud_Sth_P  == TRUE)&&(*Mtr_Lgt_Sth_P == TRUE)) ? 
						((*Language == CN) ? Menu_ON_CN  : Menu_ON_EN) : 
						((*Language == CN) ? Menu_OFF_CN : Menu_OFF_EN));

		/* 设置  低电量报警 项目字符串 */
		Set_Item_Bar_Str(Menu_Low_Power_Item_Str,
						((*Language == CN) ? Menu_Low_Power_Str_CN : Menu_Low_Power_Str_EN),
						((*Low_Pwr_Sud_Sth_P  == TRUE)&&(*Low_Pwr_Lgt_Sth_P == TRUE)) ? 
						((*Language == CN) ? Menu_ON_CN  : Menu_ON_EN) : 
						((*Language == CN) ? Menu_OFF_CN : Menu_OFF_EN));
						
		/* 设置  断电报警 项目字符串 */
		Set_Item_Bar_Str(Menu_Cut_Power_Item_Str,
						((*Language == CN) ? Menu_Cut_Power_Str_CN : Menu_Cut_Power_Str_EN),
						((*Cut_Pwr_Sud_Sth_P  == TRUE)&&(*Cut_Pwr_Lgt_Sth_P == TRUE)) ? 
						((*Language == CN) ? Menu_ON_CN	: Menu_ON_EN) : 
						((*Language == CN) ? Menu_OFF_CN : Menu_OFF_EN));

		
		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_Sud_Lgt_Aam[*Language];
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
/* 函数名	: DispMenuSudLgtAamUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置声光报警功能开光																*/
/* 创建日期	: 2015/09/21																			*/
/************************************************************************************************/
void DispMenuSudLgtAamUp(void)
{

    switch(Item)
	{
		case 0:
		{
			/* 进入 声光报警总开关 设置界面 */
			Jump2Menu(MenuSudLgtAamMtrSthSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
			break;
		}

		case 1:
		{
			/* 进入 低电量报警 设置界面 */
            Jump2Menu(MenuLowPwrSudLgtAamSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;
        }
		
        case 2:
		{
			/* 进入 断电报警 界面 */
            Jump2Menu(MenuCutPwrSudLgtAamSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;

        }

        case 3:
		{
			/* 进入 声光报警通道选择 界面 */
            Jump2Menu(MenuSudLgtAamChaSecIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
	        break;

        }


        default:
		{
			/* 未定义界面,设置界面。 */
			Jump2Menu(MenuSudLgtAamIndex,FlashMode_NoAction);
			break;
		}
    }
	
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamDown														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出声光报警界面																	*/
/* 创建日期	: 2015/09/21																			*/
/************************************************************************************************/
void DispMenuSudLgtAamDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamLeft														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择打开声光报警功能																	*/
/* 创建日期	: 2015/09/4																			*/
/************************************************************************************************/
void DispMenuSudLgtAamLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamRight														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择关闭声光报警功能																	*/
/* 创建日期	: 2015/09/21																			*/
/************************************************************************************************/
void DispMenuSudLgtAamRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/21																			*/
/************************************************************************************************/
void DispMenuSudLgtAamOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSudLgtAamCancel															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/21																			*/
/************************************************************************************************/
void DispMenuSudLgtAamCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSudLgtAamIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/

