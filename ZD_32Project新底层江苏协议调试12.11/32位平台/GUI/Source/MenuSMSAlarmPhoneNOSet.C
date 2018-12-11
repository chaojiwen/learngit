/************************************************************************************************/
/** @file              : MenuSMSAamPhoneNOSet.c													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月30日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->短信报警号码设置界面											*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Ks0108.h"
#include "Menu.h"


#include "Log_SMS.h"


/* 设置短信号码的数据缓存大小 */
#define SMS_Aam_Poe_NO_Set_Buffer_Size				(11u)

/* 设置短信号码的数据缓存 */
static u8 SMS_Aam_Poe_NO_Set_Buffer[SMS_Aam_Poe_NO_Set_Buffer_Size] = {0};

/* 短信号码界面 中英文 菜单栏 数组 */
static const struct Menu SMS_Aam_Poe_NO_Set[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};

/* 短信报警号码数据指针. */
#define SMS_Aam_Poe_NO_P(num) (((MOBILE_NO*)(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1))+num)





/************************************************************************************************/
/* 函数名	: SMS_Set_Para_Dis																	*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
static void SMS_Set_Para_Dis(u8 cursor)
{

	
	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[0],Line_2_PosX+Font1608_Size*4 ,Line_2_PosY,1);
			break;
		}
		
		case 1:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[1],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);	
			break;
		}
		
		case 2:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[2],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
			break;
		}
		
		case 3:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[3],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
			break;
		}
		
		case 4:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[4],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
			break;
		}
		
		case 5:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[5],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
			break;
		}
		
		case 6:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[6],Line_2_PosX+Font1608_Size*10 ,Line_2_PosY,1);	
			break;
		}
		
		case 7:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[7],Line_2_PosX+Font1608_Size*11 ,Line_2_PosY,1);
			break;
		}
		
		case 8:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[8],Line_2_PosX+Font1608_Size*12 ,Line_2_PosY,1);
			break;
		}
		
		case 9:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[9],Line_2_PosX+Font1608_Size*13 ,Line_2_PosY,1);
			break;
		}
		
		case 10:
		{
			GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[10],Line_2_PosX+Font1608_Size*14 ,Line_2_PosY,1);
			break;
		}
		
		default:
		{
			break;
		}
	}

}

/************************************************************************************************/
/* 函数名	: SMS_Set_Para_Set																	*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2015/6/11																			*/
/************************************************************************************************/
static void SMS_Set_Para_Set(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	SMS_Set_Para_Dis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	SMS_Set_Para_Dis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;
	
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOSetInit														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示设置短信号码界面信息															*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/



void DispMenuSMSAamPoeNOSetInit(void)
{
	MOBILE_NO *sms_no_p;
	
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
        /* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&SMS_Aam_Poe_NO_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		if(Flash != FlashMode_ReFlash)
		{
			/* 设置项目光标 */
			Item = 0;
			Where = 0;			
		}
		/* 设置光标位置 */
		Input_Para_Win_Cursor = 0;
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_SMS_Aam_Poe_NO[*Language].Text+ItemBackup[ItemBackup_i-1]));
		/* 选择 短信号码 */
#if	(SMS_User_Number == 0)
		/* 退回到上一级界面 */
		POP();
#else
		sms_no_p = SMS_Aam_Poe_NO_P(ItemBackup[ItemBackup_i-1]);
#endif
			
		/* 设置 存储的短信号码 参数 */
		SMS_Aam_Poe_NO_Set_Buffer[0]  = sms_no_p->Serial_Number_Number.Number_1;
		SMS_Aam_Poe_NO_Set_Buffer[1]  = sms_no_p->Serial_Number_Number.Number_2;
		SMS_Aam_Poe_NO_Set_Buffer[2]  = sms_no_p->Serial_Number_Number.Number_3;
		SMS_Aam_Poe_NO_Set_Buffer[3]  = sms_no_p->Serial_Number_Number.Number_4;
		SMS_Aam_Poe_NO_Set_Buffer[4]  = sms_no_p->Serial_Number_Number.Number_5;
		SMS_Aam_Poe_NO_Set_Buffer[5]  = sms_no_p->Serial_Number_Number.Number_6;		
		SMS_Aam_Poe_NO_Set_Buffer[6]  = sms_no_p->Serial_Number_Number.Number_7;		
		SMS_Aam_Poe_NO_Set_Buffer[7]  = sms_no_p->Serial_Number_Number.Number_8;		
		SMS_Aam_Poe_NO_Set_Buffer[8]  = sms_no_p->Serial_Number_Number.Number_9;		
		SMS_Aam_Poe_NO_Set_Buffer[9]  = sms_no_p->Serial_Number_Number.Number_10;
		SMS_Aam_Poe_NO_Set_Buffer[10] = sms_no_p->Serial_Number_Number.Number_11;
		/* 显示固定位置的符号 */		
		GUI_DispStringAt("NO:",Line_2_PosX,Line_2_PosY);					
		/* 显示 存储的短信号码 */						
		//GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[0],Line_2_PosX+Font1608_Size*4 ,Line_2_PosY,1);
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[1],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);	
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[2],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);	
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[3],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[4],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[5],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[6],Line_2_PosX+Font1608_Size*10 ,Line_2_PosY,1);	
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[7],Line_2_PosX+Font1608_Size*11 ,Line_2_PosY,1);
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[8],Line_2_PosX+Font1608_Size*12 ,Line_2_PosY,1);
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[9],Line_2_PosX+Font1608_Size*13 ,Line_2_PosY,1);
		GUI_DispDecAt(SMS_Aam_Poe_NO_Set_Buffer[10],Line_2_PosX+Font1608_Size*14 ,Line_2_PosY,1);
		
		/* 显示当前修改的参数位置 */
		SMS_Set_Para_Set(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();	

	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOSetUp															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置短信号																		*/
/* 创建日期	: 2015/6/11																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOSetUp(void)
{


    switch(Item)
	{
        case 0:
		{
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 0;
			break;
		}
		
        case 1:
		{
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 1;		
            break;
        }
		
        case 2:
		{
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 2;
            break;
        }
		
        case 3:
		{
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 3;
            break;
        }
		
        case 4:
		{
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 4;
            break;
        }
		
        case 5:
		{
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 5;
            break;
        }
		
		case 6:
		{ 
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 6;
            break;
        }
		
		case 7:
		{
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 7;
            break;
        }
		
		case 8:
		{
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 8;
            break;
        }
		
		case 9:
		{
			SMS_Aam_Poe_NO_Set_Buffer[Input_Para_Win_Cursor] = 9;
            break;
		}
        case 10:
		{
			/* 跳过设置缓存数据 */
			__nop();
            break;
        }	
		
		
        default:
		{
			break;
        }
    }
	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor < (SMS_Aam_Poe_NO_Set_Buffer_Size-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}

	/* 显示当前修改的参数位置 */
	SMS_Set_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuSMSAamPoeNOSetIndex,FlashMode_NoAction);

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOSetDown														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 短信号码设置 界面															*/
/* 创建日期	: 2015/6/11																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOSetLeft														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向前选择参数窗口数据																*/
/* 创建日期	: 2015/6/11																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOSetLeft(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuSMSAamPoeNOSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOSetRight														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向后选择参数窗口数据																*/
/* 创建日期	: 2015/6/11																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOSetRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuSMSAamPoeNOSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOSetOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置短信号码（参数计时生效）														*/
/* 创建日期	: 2015/6/11																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOSetOK(void)
{
	MOBILE_NO *sms_no_p;
	if(User == Domestic)//普通用户
	{	
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 更新菜单界面显示 */
		Jump2Menu(MenuSMSAamPoeNOSetIndex,FlashMode_AutoInit);
	}else if(User == Admin)
	{
		/* 短信报警号码数据指针. */
		sms_no_p = SMS_Aam_Poe_NO_P(ItemBackup[ItemBackup_i-1]);
		/* 将设置的短信号码写入数据寄存器 */
		sms_no_p->Serial_Number_Number.Number_1  = SMS_Aam_Poe_NO_Set_Buffer[0];
		sms_no_p->Serial_Number_Number.Number_2  = SMS_Aam_Poe_NO_Set_Buffer[1];
		sms_no_p->Serial_Number_Number.Number_3  = SMS_Aam_Poe_NO_Set_Buffer[2];
		sms_no_p->Serial_Number_Number.Number_4  = SMS_Aam_Poe_NO_Set_Buffer[3];
		sms_no_p->Serial_Number_Number.Number_5  = SMS_Aam_Poe_NO_Set_Buffer[4];
		sms_no_p->Serial_Number_Number.Number_6  = SMS_Aam_Poe_NO_Set_Buffer[5];
		sms_no_p->Serial_Number_Number.Number_7  = SMS_Aam_Poe_NO_Set_Buffer[6];
		sms_no_p->Serial_Number_Number.Number_8  = SMS_Aam_Poe_NO_Set_Buffer[7];
		sms_no_p->Serial_Number_Number.Number_9  = SMS_Aam_Poe_NO_Set_Buffer[8];
		sms_no_p->Serial_Number_Number.Number_10 = SMS_Aam_Poe_NO_Set_Buffer[9];
		sms_no_p->Serial_Number_Number.Number_11 = SMS_Aam_Poe_NO_Set_Buffer[10];
		
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)sms_no_p,sizeof(MOBILE_NO));

//		/* 配置信息 */
//		GSM_Config_Tab[3].CR.Change_Type = 4;
//		GSM_Config_Tab[3].CR.Change_Adder = 0;
//		GSM_Config_Tab[3].CR.Change_Data_Len = Instru_GSM_Inf_State_Space_Size;
								
		/* 参数设置成功 */
		Parameter_Setting_Successful();
		/* 退回到上一级界面 */
		POP();
	}else if(User == Super_Admin)//超级管理员
	{
		/* 更新菜单界面显示 */
		Jump2Menu(MenuSMSAamPoeNOSetIndex,FlashMode_NoAction);
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamPoeNOSetCancel														*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 光标指向前一个参数的位置															*/
/* 创建日期	: 2015/6/11																			*/
/************************************************************************************************/
void DispMenuSMSAamPoeNOSetCancel(void) 
{
	
	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = SMS_Aam_Poe_NO_Set_Buffer_Size-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	SMS_Set_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuSMSAamPoeNOSetIndex,FlashMode_NoAction);
	
}

/******************* (C) ZHOU *****END OF FILE****/


