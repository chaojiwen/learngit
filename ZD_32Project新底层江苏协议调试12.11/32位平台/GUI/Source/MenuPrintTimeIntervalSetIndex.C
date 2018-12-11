/************************************************************************************************/
/** @file              : MenuPrintTimeIntervalSetIndex.c										*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月15日															*/
/** @date changed      : 2015年10月15日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->打印时间间隔设置												*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "System_Config.h"

#if (Print_Function_Enable == 0x01)


#include "Ks0108.H"
#include "Menu.H"




/* 一小时的秒钟数 */
#define	Hour_Few_Second_Num					(3600u)
/* 一分钟的秒钟数 */
#define	Minute_Few_Second_Num				(60u)

/* 菜单设置的最小时钟 */
#define Hour_Min							(0u)
/* 菜单设置的最大时钟 */
#define Hour_Max							(23u)

/* 菜单设置的最小分钟 */
#define Minute_Min							(0u)
/* 菜单设置的最大分钟 */
#define Minute_Max							(59u)

/* 菜单设置的最小秒 */
#define Second_Min							(0u)
/* 菜单设置的最大秒 */
#define Second_Max							(59u)


/* 最大可设置的秒钟数 */
#define	Set_Max_Second_Num					(86399u)//最大23小时59分59秒

/* 打印间隔界面的数据缓存大小 */
#define Pit_Time_Irl_Set_Buf_Size			(6u)

/* 正常状态数据存储间隔数据寄存器 */
static Times Pit_Time_Irl_DR;

/* 打印间隔界面的数据缓存 */
static u8 Pit_Time_Irl_Set_Buf[Pit_Time_Irl_Set_Buf_Size] = {0};

/* 非超标状态的打印时间间隔界面 中英文 菜单栏 */
static const struct Menu Menu_Pit_Time_Irl_Set[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};

/* 非超标状态打印时间间隔数据指针 */
#define Pit_Time_Irl_Data_P  (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)

/************************************************************************************************/
/* 函数名	: Pit_Time_Irl_Para_Dis																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2015/09/15																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
static void Pit_Time_Irl_Para_Dis(u8 cursor)
{

	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			//小时-数据-十位		
			GUI_DispDecAt(Pit_Time_Irl_Set_Buf[0],Line_3_PosX+Font1608_Size*5 ,Line_3_PosY,1);
			break;
		}
		
		case 1:
		{
			//小时-数据-个位		
			GUI_DispDecAt(Pit_Time_Irl_Set_Buf[1],Line_3_PosX+Font1608_Size*6 ,Line_3_PosY,1);
			break;
		}
		
		case 2:
		{
			//分钟-数据-十位		
			GUI_DispDecAt(Pit_Time_Irl_Set_Buf[2],Line_3_PosX+Font1608_Size*8 ,Line_3_PosY,1);
			break;
		}
		
		case 3:
		{
			//分钟-数据-个位	
			GUI_DispDecAt(Pit_Time_Irl_Set_Buf[3],Line_3_PosX+Font1608_Size*9 ,Line_3_PosY,1);
			break;
		}
		
		case 4:
		{
			//秒钟-数据-十位	
			GUI_DispDecAt(Pit_Time_Irl_Set_Buf[4],Line_3_PosX+Font1608_Size*11 ,Line_3_PosY,1);
			break;
		}
		
		case 5:
		{
			//秒钟-数据-个位	
			GUI_DispDecAt(Pit_Time_Irl_Set_Buf[5],Line_3_PosX+Font1608_Size*12 ,Line_3_PosY,1);
			break;
		}
		
		default:
		{
			break;
		}
	}

}

/************************************************************************************************/
/* 函数名	: Pit_Time_Irl_Para_Set																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2015/09/15																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
static void Pit_Time_Irl_Para_Set(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	Pit_Time_Irl_Para_Dis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	Pit_Time_Irl_Para_Dis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;
	
}

/************************************************************************************************/
/* 函数名	: DispMenuPitTimeIrlSetInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 打印时间间隔时间数据															*/
/* 创建日期	: 2015/09/15																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitTimeIrlSetInit(void)
{

	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
        /* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Pit_Time_Irl_Set[*Language];
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
		/* 设置 正常状态数据存储间隔 参数 */
		/* 设置小时参数 */
		Pit_Time_Irl_Set_Buf[0] = (*Pit_Time_Irl_Data_P/Hour_Few_Second_Num)/10;
		Pit_Time_Irl_Set_Buf[1] = (*Pit_Time_Irl_Data_P/Hour_Few_Second_Num)%10;
		/* 设置分钟参数 */
		Pit_Time_Irl_Set_Buf[2] = ((*Pit_Time_Irl_Data_P%Hour_Few_Second_Num)/Minute_Few_Second_Num)/10;
		Pit_Time_Irl_Set_Buf[3] = ((*Pit_Time_Irl_Data_P%Hour_Few_Second_Num)/Minute_Few_Second_Num)%10;
		/* 设置秒钟参数 */
		Pit_Time_Irl_Set_Buf[4] = (*Pit_Time_Irl_Data_P%Minute_Few_Second_Num)/10;
		Pit_Time_Irl_Set_Buf[5] = (*Pit_Time_Irl_Data_P%Minute_Few_Second_Num)%10;
		
		/* 显示固定位置的符号 */
		GUI_DispStringAt((*Language == CN)?(u8 *)"设置:时:分:秒":(u8 *)" Set:HH:MM:SS",Line_2_PosX,Line_2_PosY);
		GUI_DispStringAt(":",Line_3_PosX+Font1608_Size*7,Line_3_PosY);
		GUI_DispStringAt(":",Line_3_PosX+Font1608_Size*10,Line_3_PosY);
		
		/* 显示 正常状态的记录时间间隔的参数 */
//		GUI_DispDecAt(Pit_Time_Irl_Set_Buf[0],Line_3_PosX+Font1608_Size*5 ,Line_3_PosY,1);
		GUI_DispDecAt(Pit_Time_Irl_Set_Buf[1],Line_3_PosX+Font1608_Size*6 ,Line_3_PosY,1);
		GUI_DispDecAt(Pit_Time_Irl_Set_Buf[2],Line_3_PosX+Font1608_Size*8 ,Line_3_PosY,1);
		GUI_DispDecAt(Pit_Time_Irl_Set_Buf[3],Line_3_PosX+Font1608_Size*9 ,Line_3_PosY,1);
		GUI_DispDecAt(Pit_Time_Irl_Set_Buf[4],Line_3_PosX+Font1608_Size*11 ,Line_3_PosY,1);
		GUI_DispDecAt(Pit_Time_Irl_Set_Buf[5],Line_3_PosX+Font1608_Size*12 ,Line_3_PosY,1);
		
		/* 显示当前修改的参数位置 */
		Pit_Time_Irl_Para_Set(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();	
    }
}

/************************************************************************************************/
/* 函数名	: DispMenuPitTimeIrlSetUp															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择打印时间间隔数据																*/
/* 创建日期	: 2015/09/15																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitTimeIrlSetUp(void)
{


    switch(Item){
        case 0:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 0;
			break;
		}
        case 1:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 1;		
            break;
        }
        case 2:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 2;
            break;
        }
        case 3:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 3;
            break;
        }
        case 4:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 4;
            break;
        }
        case 5:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 5;
            break;
        }
        case 6:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 6;
            break;
        }
        case 7:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 7;
            break;
        }
        case 8:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 8;
            break;
        }
        case 9:
		{
			/* 设置缓存数据 */
			Pit_Time_Irl_Set_Buf[Input_Para_Win_Cursor] = 9;
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

	Pit_Time_Irl_DR.Hour = Pit_Time_Irl_Set_Buf[0]*10 + Pit_Time_Irl_Set_Buf[1];
	
	/* 判断设置的 时 参数 */
	if(Pit_Time_Irl_DR.Hour > Hour_Max)
	{
		Pit_Time_Irl_DR.Hour = Hour_Max;
		Pit_Time_Irl_Set_Buf[0] = Pit_Time_Irl_DR.Hour%100/10;
		Pit_Time_Irl_Set_Buf[1] = Pit_Time_Irl_DR.Hour%10;
	}

	Pit_Time_Irl_DR.Min = Pit_Time_Irl_Set_Buf[2]*10 + Pit_Time_Irl_Set_Buf[3];
	
	/* 判断设置的 分 参数 */
	if(Pit_Time_Irl_DR.Min > Minute_Max)
	{
		Pit_Time_Irl_DR.Min = Minute_Max;
		Pit_Time_Irl_Set_Buf[2] = Pit_Time_Irl_DR.Min%100/10;
		Pit_Time_Irl_Set_Buf[3] = Pit_Time_Irl_DR.Min%10;
	}
	
	Pit_Time_Irl_DR.Sec = Pit_Time_Irl_Set_Buf[4]*10 + Pit_Time_Irl_Set_Buf[5];
	
	/* 判断设置的 秒 参数 */
	if(Pit_Time_Irl_DR.Sec > Second_Max)
	{
		Pit_Time_Irl_DR.Sec = Minute_Max;
		Pit_Time_Irl_Set_Buf[4] = Pit_Time_Irl_DR.Sec%100/10;
		Pit_Time_Irl_Set_Buf[5] = Pit_Time_Irl_DR.Sec%10;
	}

	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor < (Pit_Time_Irl_Set_Buf_Size-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}
	
	/* 显示当前修改的参数位置 */
	Pit_Time_Irl_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuPitTimeIrlSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitTimeIrlSetDown															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 正常状态存储时间间隔 界面													*/
/* 创建日期	: 2015/09/15																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitTimeIrlSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuPitTimeIrlSetLeft															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择打印时间间隔数据																*/
/* 创建日期	: 2015/09/15																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitTimeIrlSetLeft(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuPitTimeIrlSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitTimeIrlSetLeft															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择打印时间间隔数据																*/
/* 创建日期	: 2015/09/15																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitTimeIrlSetRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuPitTimeIrlSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitTimeIrlSetOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置选择打印时间间隔数据（参数计时生效）											*/
/* 创建日期	: 2015/09/15																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitTimeIrlSetOK(void)
{
	/* 设置的时间参数 */
	u32 normalsavetime;
	
	/* 获取设置的时间值 */
	normalsavetime = 
	Pit_Time_Irl_Set_Buf[0]*Hour_Few_Second_Num*10		+
	Pit_Time_Irl_Set_Buf[1]*Hour_Few_Second_Num			+
	Pit_Time_Irl_Set_Buf[2]*Minute_Few_Second_Num*10	+
	Pit_Time_Irl_Set_Buf[3]*Minute_Few_Second_Num		+
	Pit_Time_Irl_Set_Buf[4]*10							+
	Pit_Time_Irl_Set_Buf[5];
	
	/* 判断设置的时间值是否在可以设置的范围内 */
	if((normalsavetime == 0)||(normalsavetime > Set_Max_Second_Num))
	{
		/* 设置的时间参数超出的设置范围 */
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 更新菜单界面显示 */
		Jump2Menu(MenuPitTimeIrlSetIndex,FlashMode_AutoInit);
		/* 退出函数 */
		return;
	}
	

	//时间的输入方式应该更佳人性化点比较好。
	*Pit_Time_Irl_Data_P	= normalsavetime;
	
	/* 修改 仪器 信息表 设置标志 */
	Write_Config_Tab((u8*)Pit_Time_Irl_Data_P,sizeof(*Pit_Time_Irl_Data_P));
	
//	/* 配置信息 */
//	GSM_Config_Tab[5].CR.Change_Type = 6;
//	GSM_Config_Tab[5].CR.Change_Adder = 0;
//	GSM_Config_Tab[5].CR.Change_Data_Len = Instru_Menu_Inf_State_Space_Size;
	/* 参数设置成功 */
	Parameter_Setting_Successful();
	/* 退回到上一级界面 */
	POP();

}

/************************************************************************************************/
/* 函数名	: DispMenuPitTimeIrlSetCancel														*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 光标指向前一个参数的位置															*/
/* 创建日期	: 2015/09/15																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitTimeIrlSetCancel(void) 
{

	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = Pit_Time_Irl_Set_Buf_Size-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	Pit_Time_Irl_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuPitTimeIrlSetIndex,FlashMode_NoAction);
}

#endif

/******************* (C) ZHOU *****END OF FILE****/

