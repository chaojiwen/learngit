/************************************************************************************************/
/** @file              : MenuSMSAamPhoneNO.c													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月30日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->短信报警号码													*/
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


#include "Log_SMS.h"


/* 短信报警号码界面 中文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_Poe_NO_CN[]=
{
#if	(SMS_User_Number == 0)
{"无"},
#endif
	
#if (SMS_User_Number >= 1)
{"号码_1"},
#endif

#if (SMS_User_Number >= 2)
{"号码_2"},
#endif

#if (SMS_User_Number >= 3)
{"号码_3"},
#endif

#if (SMS_User_Number >= 4)
{"号码_4"},
#endif

#if (SMS_User_Number >= 5)
{"号码_5"},
#endif
};

/* 短信报警号码 英文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_Poe_NO_EN[]=
{
#if	(SMS_User_Number == 0)
{"NULL"},
#endif	
	
#if (SMS_User_Number >= 1)
{"SMS_NO1"},
#endif

#if (SMS_User_Number >= 2)
{"SMS_NO2"},
#endif

#if (SMS_User_Number >= 3)
{"SMS_NO3"},
#endif

#if (SMS_User_Number >= 4)
{"SMS_NO4"},
#endif

#if (SMS_User_Number >= 5)
{"SMS_NO5"},
#endif
};

/* 短信报警号码界面 中英文 菜单栏 数组 */
const struct Menu Menu_SMS_Aam_Poe_NO[] =
{
	MenuWhat(Menu_SMS_Aam_Poe_NO_CN),
	MenuWhat(Menu_SMS_Aam_Poe_NO_EN),
};


/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 报警短信号码 界面信息														*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOInit(void)
{
	
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{	
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_SMS_Aam_Poe_NO[*Language];
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
		GUI_Disp_Headline(*(Menu_SMS_Aam[*Language].Text+ItemBackup[ItemBackup_i-1]));
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入 选择的短信号码 界面															*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOUp(void)
{
	
	switch(Item)
	{
#if (SMS_User_Number == 0)
		case 0:
		{
			/* 将函数指针指向对应界面的初始化函数 */
			Jump2Menu(MenuSMSAamPoeNOIndex,FlashMode_NoAction);
            break;
        }
#elif (SMS_User_Number >= 1)
		case 0:
		{
			/* 进入 短信号码设置 界面 */
			Jump2Menu(MenuSMSAamPoeNOSetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
#endif		
		
#if (SMS_User_Number >= 2)	
		case 1:
		{
			/* 进入 短信号码设置 界面 */
			Jump2Menu(MenuSMSAamPoeNOSetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
#endif		
		
#if (SMS_User_Number >= 3)	 
		case 2:
		{
			/* 进入 短信号码设置 界面 */
			Jump2Menu(MenuSMSAamPoeNOSetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
#endif
		
#if (SMS_User_Number >= 4)	 
		case 3:
		{
			/* 进入 短信号码设置 界面 */
			Jump2Menu(MenuSMSAamPoeNOSetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
#endif
		
#if (SMS_User_Number >= 5)	 
		case 4:
		{
			/* 进入 短信号码设置 界面 */
			Jump2Menu(MenuSMSAamPoeNOSetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
#endif
		

        default:
		{

			/* 未知通道编号，不进入界面。 */
			Jump2Menu(MenuSMSAamPoeNOIndex,FlashMode_NoAction);
            break;
        }
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNODown															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出短信界面																		*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuSMSAamPoeNODown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOLeft																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择传短信号码项目																*/
/* 创建日期	: 2015/09/30																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamPoeNOIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNORight																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择短信号码项目																	*/
/* 创建日期	: 2015/09/30																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNORight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamPoeNOIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOOK																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/30																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamPoeNOIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOCancel																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/30																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOCancel(void) 
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamPoeNOIndex,FlashMode_NoAction);
}

/******************* (C) ZHOU *****END OF FILE****/



