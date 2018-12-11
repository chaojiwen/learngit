/************************************************************************************************/
/** @file              : MenuPrintEndTimeSet.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年07月06日															*/
/** @date changed      : 2015年10月15日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->打印起始时间设置												*/
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

#include "Mcu_Config.h"

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



/* 打印时间数据缓存大小 */
#define Pit_End_Time_Set_Buf_Size 			(10u)
/* 打印时间数据缓存 */
static u8 Pit_End_Time_Set_Buf[Pit_End_Time_Set_Buf_Size] = {0};
/* 打印起始时间数据寄存器 */
static Times Pit_End_Time_Set_DR;

/* 清除历史数据界面 中英文 菜单栏 */
static const struct Menu Menu_Pit_End_Time_Set[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};

/* 打印起始时间数据指针 */
#define Menu_Pit_End_Time_Set_Data_P    (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_End_Time)


/************************************************************************************************/
/* 函数名	: Pit_End_Time_Set_Dis																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/15																		*/
/************************************************************************************************/
static void Pit_End_Time_Set_Dis(u8 cursor)
{

	
	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[0],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
			break;
		}
		
		case 1:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[1],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
			break;
		}
		
		case 2:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[2],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
			break;
		}
		
		case 3:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[3],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
			break;
		}
		
		case 4:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[4],Line_2_PosX+Font1608_Size*11 ,Line_2_PosY,1);
			break;
		}
		
		case 5:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[5],Line_2_PosX+Font1608_Size*12 ,Line_2_PosY,1);
			break;
		}
		
		case 6:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[6],Line_3_PosX+Font1608_Size*6-4 ,Line_3_PosY,1);
			break;
		}
		
		case 7:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[7],Line_3_PosX+Font1608_Size*7-4 ,Line_3_PosY,1);
			break;
		}
		
		case 8:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[8],Line_3_PosX+Font1608_Size*9-4 ,Line_3_PosY,1);
			break;
		}
		
		case 9:
		{
			GUI_DispDecAt(Pit_End_Time_Set_Buf[9],Line_3_PosX+Font1608_Size*10-4 ,Line_3_PosY,1);
			break;
		}

		
		default:
		{
			break;
		}
	}
	
}

/************************************************************************************************/
/* 函数名	: Pit_End_Time_Set																	*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/15																		*/
/************************************************************************************************/
static void Pit_End_Time_Set(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	Pit_End_Time_Set_Dis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	Pit_End_Time_Set_Dis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;

}

/************************************************************************************************/
/* 函数名	: DispMenuPitEndTimeSetInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 打印起始时间设置 界面																*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/15																		*/
/************************************************************************************************/
void DispMenuPitEndTimeSetInit(void)
{

    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Pit_End_Time_Set[*Language];
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
		GUI_Disp_Headline(*(Menu_Pit_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));
		
		/* 设置起始时间*/
		RTC_Second_To_Date_Time(*Menu_Pit_End_Time_Set_Data_P,&Pit_End_Time_Set_DR);
		
		/* 设置 时钟 参数 */
		Pit_End_Time_Set_Buf[0]  = Pit_End_Time_Set_DR.Year%100/10;
		Pit_End_Time_Set_Buf[1]  = Pit_End_Time_Set_DR.Year%10;
	
		Pit_End_Time_Set_Buf[2]  = Pit_End_Time_Set_DR.Month%100/10;
		Pit_End_Time_Set_Buf[3]  = Pit_End_Time_Set_DR.Month%10;
	
		Pit_End_Time_Set_Buf[4]  = Pit_End_Time_Set_DR.Day%100/10;
		Pit_End_Time_Set_Buf[5]  = Pit_End_Time_Set_DR.Day%10;
	
		Pit_End_Time_Set_Buf[6]  = Pit_End_Time_Set_DR.Hour%100/10;
		Pit_End_Time_Set_Buf[7]  = Pit_End_Time_Set_DR.Hour%10;
	
		Pit_End_Time_Set_Buf[8]  = Pit_End_Time_Set_DR.Min%100/10;
		Pit_End_Time_Set_Buf[9]  = Pit_End_Time_Set_DR.Min%10;
		/* 显示固定位置的符号 */
		GUI_DispStringAt(".",Line_2_PosX+Font1608_Size*7 ,Line_2_PosY);
		GUI_DispStringAt(".",Line_2_PosX+Font1608_Size*10 ,Line_2_PosY);
		GUI_DispStringAt(":",Line_3_PosX+Font1608_Size*8-4,Line_3_PosY);
		GUI_DispDecAt(2,Line_2_PosX+Font1608_Size*3 ,Line_2_PosY,1);
		GUI_DispDecAt(0,Line_2_PosX+Font1608_Size*4 ,Line_2_PosY,1);
		
		/* 显示 正常状态的记录时间间隔的参数 */
//		GUI_DispDecAt(Pit_End_Time_Set_Buf[0],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
		GUI_DispDecAt(Pit_End_Time_Set_Buf[1],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
		GUI_DispDecAt(Pit_End_Time_Set_Buf[2],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
		GUI_DispDecAt(Pit_End_Time_Set_Buf[3],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
		GUI_DispDecAt(Pit_End_Time_Set_Buf[4],Line_2_PosX+Font1608_Size*11 ,Line_2_PosY,1);
		GUI_DispDecAt(Pit_End_Time_Set_Buf[5],Line_2_PosX+Font1608_Size*12 ,Line_2_PosY,1);
		GUI_DispDecAt(Pit_End_Time_Set_Buf[6],Line_3_PosX+Font1608_Size*6-4 ,Line_3_PosY,1);
		GUI_DispDecAt(Pit_End_Time_Set_Buf[7],Line_3_PosX+Font1608_Size*7-4 ,Line_3_PosY,1);
		GUI_DispDecAt(Pit_End_Time_Set_Buf[8],Line_3_PosX+Font1608_Size*9-4 ,Line_3_PosY,1);
		GUI_DispDecAt(Pit_End_Time_Set_Buf[9],Line_3_PosX+Font1608_Size*10-4 ,Line_3_PosY,1);
		
		/* 显示当前修改的参数位置 */
		Pit_End_Time_Set(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();

    }

}

/************************************************************************************************/
/* 函数名	: DispMenuPitEndTimeSetUp															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置 打印时间数据																	*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/15																		*/
/************************************************************************************************/
void DispMenuPitEndTimeSetUp(void)
{

	
	switch(Item)
	{
        case 0:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 0;
			break;
		}
        case 1:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 1;		
            break;
        }
        case 2:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 2;
            break;
        }
        case 3:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 3;
            break;
        }
        case 4:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 4;
            break;
        }
        case 5:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 5;
            break;
        }
        case 6:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 6;
            break;
        }
        case 7:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 7;
            break;
        }
        case 8:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 8;
            break;
        }
        case 9:
		{
			/* 设置缓存数据 */
			Pit_End_Time_Set_Buf[Input_Para_Win_Cursor] = 9;
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
	
	Pit_End_Time_Set_DR.Month = Pit_End_Time_Set_Buf[2]*10+
							Pit_End_Time_Set_Buf[3];
	/* 判断设置的 月 参数 */
	if(Pit_End_Time_Set_DR.Month > Month_Max)
	{
		Pit_End_Time_Set_DR.Month = Month_Max;
		Pit_End_Time_Set_Buf[2]  = Pit_End_Time_Set_DR.Month%100/10;
		Pit_End_Time_Set_Buf[3]  = Pit_End_Time_Set_DR.Month%10;
	}else if(Pit_End_Time_Set_DR.Month < Month_Min)
	{
		Pit_End_Time_Set_DR.Month = Month_Min;
		Pit_End_Time_Set_Buf[2]  = Pit_End_Time_Set_DR.Month%100/10;
		Pit_End_Time_Set_Buf[3]  = Pit_End_Time_Set_DR.Month%10;
	}
		
	Pit_End_Time_Set_DR.Day = Pit_End_Time_Set_Buf[4]*10+
						  Pit_End_Time_Set_Buf[5];
	/* 判断设置的 日期 参数 */
	if(Pit_End_Time_Set_DR.Day > Day_Max)
	{
		/* 设置当前日期  */
		if(Pit_End_Time_Set_DR.Day > month_days[Pit_End_Time_Set_DR.Month-1])
		{
			//这里存在一个 BUG 当二月日期有29天时没法设置
			Pit_End_Time_Set_DR.Day  = month_days[Pit_End_Time_Set_DR.Month-1];
			Pit_End_Time_Set_Buf[4] = Pit_End_Time_Set_DR.Day%100/10;
			Pit_End_Time_Set_Buf[5] = Pit_End_Time_Set_DR.Day%10;
		}
	}else if(Pit_End_Time_Set_DR.Day < Day_Min)
	{
		Pit_End_Time_Set_DR.Day  = Day_Min;
		Pit_End_Time_Set_Buf[4] = Pit_End_Time_Set_DR.Day%100/10;
		Pit_End_Time_Set_Buf[5] = Pit_End_Time_Set_DR.Day%10;
	}

	Pit_End_Time_Set_DR.Hour = Pit_End_Time_Set_Buf[6]*10 + Pit_End_Time_Set_Buf[7];
	/* 判断设置的 时 参数 */
	if(Pit_End_Time_Set_DR.Hour > Hour_Max)
	{
		Pit_End_Time_Set_DR.Hour = Hour_Max;
		Pit_End_Time_Set_Buf[6] = Pit_End_Time_Set_DR.Hour%100/10;
		Pit_End_Time_Set_Buf[7] = Pit_End_Time_Set_DR.Hour%10;
	}

	Pit_End_Time_Set_DR.Min = Pit_End_Time_Set_Buf[8]*10 + Pit_End_Time_Set_Buf[9];
	/* 判断设置的 分 参数 */
	if(Pit_End_Time_Set_DR.Min > Minute_Max)
	{
		Pit_End_Time_Set_DR.Min  = Minute_Max;
		Pit_End_Time_Set_Buf[8] = Pit_End_Time_Set_DR.Min%100/10;
		Pit_End_Time_Set_Buf[9] = Pit_End_Time_Set_DR.Min%10;
	}

	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor < (Pit_End_Time_Set_Buf_Size-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}
	
	/* 显示当前修改的参数位置 */
	Pit_End_Time_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuPitEndTimeSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitEndTimeSetDown															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 打印起始时间界面																*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/15																		*/
/************************************************************************************************/
void DispMenuPitEndTimeSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuPitEndTimeSetLeft															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 打印时间参数																	*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/15																		*/
/************************************************************************************************/
void DispMenuPitEndTimeSetLeft(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuPitEndTimeSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitEndTimeSetRight														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 打印时间参数																	*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/15																		*/
/************************************************************************************************/
void DispMenuPitEndTimeSetRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuPitEndTimeSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitEndTimeSetOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置当前选择的时间为打印时间														*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/15																		*/
/************************************************************************************************/
void DispMenuPitEndTimeSetOK(void)
{
//	if(User == Domestic)//普通用户
//	{	
//		/* 非管管理员模式，输入的参数视为无效 */
//		Parameter_Setting_Failure();
//		/* 将函数指针指向初始化函数 */
//		Jump2Menu(MenuPitEndTimeSetIndex,FlashMode_AutoInit);
//	}else if(User == Admin)
//	{
		/* 将时间数据分析到缓存中 */
		Pit_End_Time_Set_DR.Year 		= Year_Start +
									  Pit_End_Time_Set_Buf[0]*10+
									  Pit_End_Time_Set_Buf[1];
		
		Pit_End_Time_Set_DR.Month 		= Pit_End_Time_Set_Buf[2]*10+
									  Pit_End_Time_Set_Buf[3];
		
		Pit_End_Time_Set_DR.Day 		= Pit_End_Time_Set_Buf[4]*10+
									  Pit_End_Time_Set_Buf[5];
	
		Pit_End_Time_Set_DR.Hour 		= Pit_End_Time_Set_Buf[6]*10+
									  Pit_End_Time_Set_Buf[7];
	
		Pit_End_Time_Set_DR.Min 		= Pit_End_Time_Set_Buf[8]*10+
									  Pit_End_Time_Set_Buf[9];

		/* 清除秒计数器值 */
		Pit_End_Time_Set_DR.Sec	= 0;
		
		/* 设置 打印时间 */ 
		/* 获取打印起始时钟数据 */
		/* 设置起始时间*/
		*Menu_Pit_End_Time_Set_Data_P = RTC_Date_Time_To_Second(&Pit_End_Time_Set_DR);
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)Menu_Pit_End_Time_Set_Data_P,sizeof(*Menu_Pit_End_Time_Set_Data_P));
					  
//		/* 配置信息 */
//		GSM_Config_Tab[2].CR.Change_Type = 3;
//		GSM_Config_Tab[2].CR.Change_Adder = 0;
//		GSM_Config_Tab[2].CR.Change_Data_Len = Instru_RTC_Inf_State_Space_Size;
		/* 参数设置成 */
		Parameter_Setting_Successful();
		/* 退回到上一级界面 */
		POP();
//	}else if(User == Super_Admin)//超级管理员
//	{
//		/* 将函数指针指向初始化函数,更新显示 */
//		Jump2Menu(MenuPitEndTimeSetIndex,FlashMode_NoAction);
//	}
}

/************************************************************************************************/
/* 函数名	: DispMenuPitEndTimeSetCancel														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/15																		*/
/************************************************************************************************/
void DispMenuPitEndTimeSetCancel(void)
{
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = Pit_End_Time_Set_Buf_Size-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	Pit_End_Time_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuPitEndTimeSetIndex,FlashMode_NoAction);
}  

#endif
/******************* (C) COPYRIGHT 2015 HZ Zeda Team ****END OF FILE****/


