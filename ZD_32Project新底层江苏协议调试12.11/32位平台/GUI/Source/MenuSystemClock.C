/************************************************************************************************/
/** @file              : MenuSystemClock.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月12日															*/
/** @date changed      : 2015年10月13日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->系统时钟														*/
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


#include "Dev_RTC.h"


/* 菜单设置的开始年份 */
#define Year_Start		(2000u)
/* 菜单设置的最大年份 */
#define Year_End		(2100u)
/* 菜单设置的最小月份 */
#define Month_Min		(1u)
/* 菜单设置的最大月份 */
#define Month_Max		(12u)
/* 菜单设置的最小天数 */
#define Day_Min			(1u)
/* 菜单设置的最大天数 */
#define Day_Max			(28u)

/* 菜单设置的最小时钟 */
#define Hour_Min		(0u)
/* 菜单设置的最大时钟 */
#define Hour_Max		(23u)
/* 菜单设置的最小分钟 */
#define Minute_Min		(0u)
/* 菜单设置的最大分钟 */
#define Minute_Max		(59u)



/* 系统时钟界面数据缓存大小 */
#define Stm_Cok_Buf_Size (10u)
/* 系统时钟界面数据缓存 */
static u8 Stm_Cok_Buf[Stm_Cok_Buf_Size] = {0};
/* 系统时钟数据寄存器 *///ZWC
 Times Stm_Cok_DR;

/* 系统时钟 中英文 菜单栏 */
const struct Menu Menu_Stm_Cok[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};



/************************************************************************************************/
/* 函数名	: Stm_Cok_Para_Dis																	*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2015/06/12																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
static void Stm_Cok_Para_Dis(u8 cursor)
{

	
	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispDecAt(Stm_Cok_Buf[0],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
			break;
		}
		
		case 1:
		{
			GUI_DispDecAt(Stm_Cok_Buf[1],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
			break;
		}
		
		case 2:
		{
			GUI_DispDecAt(Stm_Cok_Buf[2],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
			break;
		}
		
		case 3:
		{
			GUI_DispDecAt(Stm_Cok_Buf[3],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
			break;
		}
		
		case 4:
		{
			GUI_DispDecAt(Stm_Cok_Buf[4],Line_2_PosX+Font1608_Size*11 ,Line_2_PosY,1);
			break;
		}
		
		case 5:
		{
			GUI_DispDecAt(Stm_Cok_Buf[5],Line_2_PosX+Font1608_Size*12 ,Line_2_PosY,1);
			break;
		}
		
		case 6:
		{
			GUI_DispDecAt(Stm_Cok_Buf[6],Line_3_PosX+Font1608_Size*6-4 ,Line_3_PosY,1);
			break;
		}
		
		case 7:
		{
			GUI_DispDecAt(Stm_Cok_Buf[7],Line_3_PosX+Font1608_Size*7-4 ,Line_3_PosY,1);
			break;
		}
		
		case 8:
		{
			GUI_DispDecAt(Stm_Cok_Buf[8],Line_3_PosX+Font1608_Size*9-4 ,Line_3_PosY,1);
			break;
		}
		
		case 9:
		{
			GUI_DispDecAt(Stm_Cok_Buf[9],Line_3_PosX+Font1608_Size*10-4 ,Line_3_PosY,1);
			break;
		}
	

		
		default:
		{
			break;
		}
	}
	
}

/************************************************************************************************/
/* 函数名	: Stm_Cok_Para_Set																	*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2015/06/12																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
static void Stm_Cok_Para_Set(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	Stm_Cok_Para_Dis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	Stm_Cok_Para_Dis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;

}

/************************************************************************************************/
/* 函数名	: DispMenuStmCokInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示时钟界面信息																	*/
/* 创建日期	: 2015/06/12																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuStmCokInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Stm_Cok[*Language];
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
		GUI_Disp_Headline(*(Menu_Stm_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));
		/* 获取系统时钟数据 */
		Stm_Cok_DR = System_Time;
		/* 设置 时钟 参数 */
		Stm_Cok_Buf[0]  = Stm_Cok_DR.Year%100/10;
		Stm_Cok_Buf[1]  = Stm_Cok_DR.Year%10;
	
		Stm_Cok_Buf[2]  = Stm_Cok_DR.Month%100/10;
		Stm_Cok_Buf[3]  = Stm_Cok_DR.Month%10;
	
		Stm_Cok_Buf[4]  = Stm_Cok_DR.Day%100/10;
		Stm_Cok_Buf[5]  = Stm_Cok_DR.Day%10;
	
		Stm_Cok_Buf[6]  = Stm_Cok_DR.Hour%100/10;
		Stm_Cok_Buf[7]  = Stm_Cok_DR.Hour%10;
	
		Stm_Cok_Buf[8]  = Stm_Cok_DR.Min%100/10;
		Stm_Cok_Buf[9]  = Stm_Cok_DR.Min%10;
		/* 显示固定位置的符号 */
		GUI_DispStringAt(".",Line_2_PosX+Font1608_Size*7 ,Line_2_PosY);
		GUI_DispStringAt(".",Line_2_PosX+Font1608_Size*10 ,Line_2_PosY);
		GUI_DispStringAt(":",Line_3_PosX+Font1608_Size*8-4,Line_3_PosY);
		GUI_DispDecAt(2,Line_2_PosX+Font1608_Size*3 ,Line_2_PosY,1);
		GUI_DispDecAt(0,Line_2_PosX+Font1608_Size*4 ,Line_2_PosY,1);
		
		/* 显示 正常状态的记录时间间隔的参数 */
//		GUI_DispDecAt(Stm_Cok_Buf[0],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
		GUI_DispDecAt(Stm_Cok_Buf[1],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
		GUI_DispDecAt(Stm_Cok_Buf[2],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
		GUI_DispDecAt(Stm_Cok_Buf[3],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
		GUI_DispDecAt(Stm_Cok_Buf[4],Line_2_PosX+Font1608_Size*11 ,Line_2_PosY,1);
		GUI_DispDecAt(Stm_Cok_Buf[5],Line_2_PosX+Font1608_Size*12 ,Line_2_PosY,1);
		GUI_DispDecAt(Stm_Cok_Buf[6],Line_3_PosX+Font1608_Size*6-4 ,Line_3_PosY,1);
		GUI_DispDecAt(Stm_Cok_Buf[7],Line_3_PosX+Font1608_Size*7-4 ,Line_3_PosY,1);
		GUI_DispDecAt(Stm_Cok_Buf[8],Line_3_PosX+Font1608_Size*9-4 ,Line_3_PosY,1);
		GUI_DispDecAt(Stm_Cok_Buf[9],Line_3_PosX+Font1608_Size*10-4 ,Line_3_PosY,1);

		/* 显示当前修改的参数位置 */
		Stm_Cok_Para_Set(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuStmCokUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置数据																			*/
/* 创建日期	: 2015/06/12																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuStmCokUp(void)
{

	
    switch(Item)
	{
        case 0:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 0;
			break;
		}
        case 1:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 1;		
            break;
        }
        case 2:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 2;
            break;
        }
        case 3:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 3;
            break;
        }
        case 4:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 4;
            break;
        }
        case 5:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 5;
            break;
        }
        case 6:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 6;
            break;
        }
        case 7:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 7;
            break;
        }
        case 8:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 8;
            break;
        }
        case 9:
		{
			/* 设置缓存数据 */
			Stm_Cok_Buf[Input_Para_Win_Cursor] = 9;
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
	
	Stm_Cok_DR.Month = Stm_Cok_Buf[2]*10+
					   Stm_Cok_Buf[3];
	/* 判断设置的 月 参数 */
	if(Stm_Cok_DR.Month > Month_Max)
	{
		Stm_Cok_DR.Month = Month_Max;
		Stm_Cok_Buf[2]  = Stm_Cok_DR.Month%100/10;
		Stm_Cok_Buf[3]  = Stm_Cok_DR.Month%10;
	}else if(Stm_Cok_DR.Month < Month_Min)
	{
		Stm_Cok_DR.Month = Month_Min;
		Stm_Cok_Buf[2]  = Stm_Cok_DR.Month%100/10;
		Stm_Cok_Buf[3]  = Stm_Cok_DR.Month%10;
	}
		
	Stm_Cok_DR.Day = Stm_Cok_Buf[4]*10+
					 Stm_Cok_Buf[5];
	/* 判断设置的 日期 参数 */
	if(Stm_Cok_DR.Day > Day_Max)
	{
		/* 设置当前日期  */
		if(Stm_Cok_DR.Day > month_days[Stm_Cok_DR.Month-1])
		{
			//这里存在一个 BUG 当二月日期有29天时没法设置
			Stm_Cok_DR.Day  = month_days[Stm_Cok_DR.Month-1];
			Stm_Cok_Buf[4] = Stm_Cok_DR.Day%100/10;
			Stm_Cok_Buf[5] = Stm_Cok_DR.Day%10;
		}
	}else if(Stm_Cok_DR.Day < Day_Min)
	{
		Stm_Cok_DR.Day 	= Day_Min;
		Stm_Cok_Buf[4] = Stm_Cok_DR.Day%100/10;
		Stm_Cok_Buf[5] = Stm_Cok_DR.Day%10;
	}

	Stm_Cok_DR.Hour = Stm_Cok_Buf[6]*10+
					  Stm_Cok_Buf[7];
	/* 判断设置的 时 参数 */
	if(Stm_Cok_DR.Hour > Hour_Max)
	{
		Stm_Cok_DR.Hour = Hour_Max;
		Stm_Cok_Buf[6] = Stm_Cok_DR.Hour%100/10;
		Stm_Cok_Buf[7] = Stm_Cok_DR.Hour%10;
	}

	Stm_Cok_DR.Min = Stm_Cok_Buf[8]*10+
					 Stm_Cok_Buf[9];
	/* 判断设置的 分 参数 */
	if(Stm_Cok_DR.Min > Minute_Max)
	{
		Stm_Cok_DR.Min 	 = Minute_Max;
		Stm_Cok_Buf[8] = Stm_Cok_DR.Min%100/10;
		Stm_Cok_Buf[9] = Stm_Cok_DR.Min%10;
	}

	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor < (Stm_Cok_Buf_Size-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}
	
	/* 显示当前修改的参数位置 */
	Stm_Cok_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuStmCokIndex,FlashMode_NoAction);

}

/************************************************************************************************/
/* 函数名	: DispMenuStmCokDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 时钟 界面																	*/
/* 创建日期	: 2015/06/12																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuStmCokDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuStmCokLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向前选择参数窗口数据																*/
/* 创建日期	: 2015/06/12																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuStmCokLeft(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuStmCokIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuStmCokRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向后选择参数窗口数据																*/
/* 创建日期	: 2015/06/12																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuStmCokRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuStmCokIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuStmCokOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置修改的参数（参数计时生效）													*/
/* 创建日期	: 2015/06/12																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuStmCokOK(void)
{
	
	
	if(User == Domestic)//普通用户
	{	
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 将函数指针指向初始化函数 */
		Jump2Menu(MenuStmCokIndex,FlashMode_AutoInit);
	}else if(User == Admin)
	{
		/* 将时间数据分析到缓存中 */
		Stm_Cok_DR.Year 	= Year_Start +
							  Stm_Cok_Buf[0]*10+
							  Stm_Cok_Buf[1];
		
		Stm_Cok_DR.Month 	= Stm_Cok_Buf[2]*10+
							  Stm_Cok_Buf[3];
		
		Stm_Cok_DR.Day 		= Stm_Cok_Buf[4]*10+
							  Stm_Cok_Buf[5];
	
		Stm_Cok_DR.Hour 	= Stm_Cok_Buf[6]*10+
							  Stm_Cok_Buf[7];
	
		Stm_Cok_DR.Min 		= Stm_Cok_Buf[8]*10+
							  Stm_Cok_Buf[9];

		/* 清除秒计数器值 */
		Stm_Cok_DR.Sec	= 0;
		
		/* 复位 RTC8025 */
		RTC_Init(TRUE);
		/* 设置 时间 */ 
		Write_System_Date_Time_Data(&Stm_Cok_DR);
		
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year 	= Stm_Cok_DR.Year;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month 	= Stm_Cok_DR.Month;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day 	= Stm_Cok_DR.Day;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour 	= Stm_Cok_DR.Hour;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min 	= Stm_Cok_DR.Min;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec 	= Stm_Cok_DR.Sec;
		
		//为了解决RTC时钟芯片意外被写故障，这里先保存一次当前的实时时钟数据为\
		  当上电启动检测到RTC被异常后可以使用这个时间来做作实时时间。
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_RTC,
					    sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC));
					  
//		/* 配置信息 */
//		GSM_Config_Tab[2].CR.Change_Type = 3;
//		GSM_Config_Tab[2].CR.Change_Adder = 0;
//		GSM_Config_Tab[2].CR.Change_Data_Len = Instru_RTC_Inf_State_Space_Size;
		/* 参数设置成 */
		Parameter_Setting_Successful();
		/* 退回到上一级界面 */
		POP();
	}else if(User == Super_Admin)//超级管理员
	{
		/* 将函数指针指向初始化函数,更新显示 */
		Jump2Menu(MenuStmCokIndex,FlashMode_NoAction);
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuStmCokCancel																*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 光标指向前一个参数的位置															*/
/* 创建日期	: 2015/06/12																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuStmCokCancel(void) 
{

	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = Stm_Cok_Buf_Size-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	Stm_Cok_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuStmCokIndex,FlashMode_NoAction);
	
}

/******************* (C) ZHOU *****END OF FILE****/


