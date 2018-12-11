/************************************************************************************************/
/** @file              : MenuNormalSaveInterval.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月08日															*/
/** @date changed      : 2015年10月08日															*/
/** @brief             : c file	(完善程序功能.)													*/
/** @description       : GUI菜单->传感器正常状态下记录数据时间间隔.								*/
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


/* 正常状态数据存储间隔界面的数据缓存大小 */
#define Nml_Save_Irl_Buf_Size				(6u)//6个字节的数据缓存

/* 正常状态数据存储间隔数据寄存器 */
static Times Nml_Save_Irl_DR;

/* 正常状态数据存储间隔界面的数据缓存 */
static u8 Nml_Save_Irl_Buf[Nml_Save_Irl_Buf_Size] = {0};

/* 正常状态的记录时间间隔界面 中英文 菜单栏 */
static const struct Menu Menu_Nml_Save_Irl[] =
{
MenuWhat(Menu_Digital_Input),
MenuWhat(Menu_Digital_Input),
};

/* " 正常存储间隔"数据指针 */
#define Menu_Nml_Save_Irl_Data_P 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval)



/************************************************************************************************/
/* 函数名	: Normal_Save_Para_Dis																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置																*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
static void Normal_Save_Para_Dis(u8 cursor)
{

	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			//小时-数据-十位		
			GUI_DispDecAt(Nml_Save_Irl_Buf[0],Line_3_PosX+Font1608_Size*5 ,Line_3_PosY,1);
			break;
		}
		
		case 1:
		{
			//小时-数据-个位		
			GUI_DispDecAt(Nml_Save_Irl_Buf[1],Line_3_PosX+Font1608_Size*6 ,Line_3_PosY,1);
			break;
		}
		
		case 2:
		{
			//分钟-数据-十位		
			GUI_DispDecAt(Nml_Save_Irl_Buf[2],Line_3_PosX+Font1608_Size*8 ,Line_3_PosY,1);
			break;
		}
		
		case 3:
		{
			//分钟-数据-个位	
			GUI_DispDecAt(Nml_Save_Irl_Buf[3],Line_3_PosX+Font1608_Size*9 ,Line_3_PosY,1);
			break;
		}
		
		case 4:
		{
			//秒钟-数据-十位	
			GUI_DispDecAt(Nml_Save_Irl_Buf[4],Line_3_PosX+Font1608_Size*11 ,Line_3_PosY,1);
			break;
		}
		
		case 5:
		{
			//秒钟-数据-个位	
			GUI_DispDecAt(Nml_Save_Irl_Buf[5],Line_3_PosX+Font1608_Size*12 ,Line_3_PosY,1);
			break;
		}
		
		default:
		{
			break;
		}
	}

}

/************************************************************************************************/
/* 函数名	: Normal_Save_Para_Set																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色										*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
static void Normal_Save_Para_Set(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	Normal_Save_Para_Dis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	Normal_Save_Para_Dis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;
	
}

/************************************************************************************************/
/* 函数名	: DispMenuNmlSaveIrlInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示参数界面信息																	*/
/* 创建日期	: 2015/06/08																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuNmlSaveIrlInit(void)
{

	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
        /* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Nml_Save_Irl[*Language];
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
		GUI_Disp_Headline(*(Menu_Run_Para[*Language].Text+ItemBackup[ItemBackup_i-1]));
		/* 设置 正常状态数据存储间隔 参数 */
		/* 设置小时参数 */
		Nml_Save_Irl_Buf[0] = (*Menu_Nml_Save_Irl_Data_P/Hour_Few_Second_Num)/10;
		Nml_Save_Irl_Buf[1] = (*Menu_Nml_Save_Irl_Data_P/Hour_Few_Second_Num)%10;
		/* 设置分钟参数 */
		Nml_Save_Irl_Buf[2] = ((*Menu_Nml_Save_Irl_Data_P%Hour_Few_Second_Num)/Minute_Few_Second_Num)/10;
		Nml_Save_Irl_Buf[3] = ((*Menu_Nml_Save_Irl_Data_P%Hour_Few_Second_Num)/Minute_Few_Second_Num)%10;
		/* 设置秒钟参数 */
		Nml_Save_Irl_Buf[4] = (*Menu_Nml_Save_Irl_Data_P%Minute_Few_Second_Num)/10;
		Nml_Save_Irl_Buf[5] = (*Menu_Nml_Save_Irl_Data_P%Minute_Few_Second_Num)%10;
		
		/* 显示固定位置的符号 */
		GUI_DispStringAt((*Language == CN)?(u8 *)"设置:时:分:秒":(u8 *)" Set:HH:MM:SS",Line_2_PosX,Line_2_PosY);
		GUI_DispStringAt(":",Line_3_PosX+Font1608_Size*7,Line_3_PosY);
		GUI_DispStringAt(":",Line_3_PosX+Font1608_Size*10,Line_3_PosY);
		
		/* 显示 正常状态的记录时间间隔的参数 */
//		GUI_DispDecAt(Nml_Save_Irl_Buf[0],Line_3_PosX+Font1608_Size*5 ,Line_3_PosY,1);
		GUI_DispDecAt(Nml_Save_Irl_Buf[1],Line_3_PosX+Font1608_Size*6 ,Line_3_PosY,1);
		GUI_DispDecAt(Nml_Save_Irl_Buf[2],Line_3_PosX+Font1608_Size*8 ,Line_3_PosY,1);
		GUI_DispDecAt(Nml_Save_Irl_Buf[3],Line_3_PosX+Font1608_Size*9 ,Line_3_PosY,1);
		GUI_DispDecAt(Nml_Save_Irl_Buf[4],Line_3_PosX+Font1608_Size*11 ,Line_3_PosY,1);
		GUI_DispDecAt(Nml_Save_Irl_Buf[5],Line_3_PosX+Font1608_Size*12 ,Line_3_PosY,1);
		
		/* 显示当前修改的参数位置 */
		Normal_Save_Para_Set(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();	
    }
}

/************************************************************************************************/
/* 函数名	: DispMenuNmlSaveIrlUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置数据																			*/
/* 创建日期	: 2015/06/08																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuNmlSaveIrlUp(void)
{


    switch(Item){
        case 0:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 0;
			break;
		}
        case 1:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 1;		
            break;
        }
        case 2:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 2;
            break;
        }
        case 3:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 3;
            break;
        }
        case 4:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 4;
            break;
        }
        case 5:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 5;
            break;
        }
        case 6:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 6;
            break;
        }
        case 7:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 7;
            break;
        }
        case 8:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 8;
            break;
        }
        case 9:
		{
			/* 设置缓存数据 */
			Nml_Save_Irl_Buf[Input_Para_Win_Cursor] = 9;
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
	
	Nml_Save_Irl_DR.Hour = Nml_Save_Irl_Buf[0]*10 + Nml_Save_Irl_Buf[1];
	
	/* 判断设置的 时 参数 */
	if(Nml_Save_Irl_DR.Hour > Hour_Max)
	{
		Nml_Save_Irl_DR.Hour = Hour_Max;
		Nml_Save_Irl_Buf[0] = Nml_Save_Irl_DR.Hour%100/10;
		Nml_Save_Irl_Buf[1] = Nml_Save_Irl_DR.Hour%10;
	}

	Nml_Save_Irl_DR.Min = Nml_Save_Irl_Buf[2]*10 + Nml_Save_Irl_Buf[3];
	
	/* 判断设置的 分 参数 */
	if(Nml_Save_Irl_DR.Min > Minute_Max)
	{
		Nml_Save_Irl_DR.Min = Minute_Max;
		Nml_Save_Irl_Buf[2] = Nml_Save_Irl_DR.Min%100/10;
		Nml_Save_Irl_Buf[3] = Nml_Save_Irl_DR.Min%10;
	}
	
	Nml_Save_Irl_DR.Sec = Nml_Save_Irl_Buf[4]*10 + Nml_Save_Irl_Buf[5];
	
	/* 判断设置的 秒 参数 */
	if(Nml_Save_Irl_DR.Sec > Second_Max)
	{
		Nml_Save_Irl_DR.Sec = Minute_Max;
		Nml_Save_Irl_Buf[4] = Nml_Save_Irl_DR.Sec%100/10;
		Nml_Save_Irl_Buf[5] = Nml_Save_Irl_DR.Sec%10;
	}
	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor < (Nml_Save_Irl_Buf_Size-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}
	
	/* 显示当前修改的参数位置 */
	Normal_Save_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuNmlSaveIrlIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuNmlSaveIrlDown															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 正常状态存储时间间隔 界面													*/
/* 创建日期	: 2015/06/08																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuNmlSaveIrlDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuNmlSaveIrlLeft															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向前选择参数窗口数据																*/
/* 创建日期	: 2015/06/08																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuNmlSaveIrlLeft(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuNmlSaveIrlIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuNmlSaveIrlLeft															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向后选择参数窗口数据																*/
/* 创建日期	: 2015/06/08																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuNmlSaveIrlRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuNmlSaveIrlIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuNmlSaveIrlOK																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置修改的参数（参数计时生效）													*/
/* 创建日期	: 2015/06/08																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuNmlSaveIrlOK(void)
{
	/* 设置的时间参数 */
	u32 normalsavetime;
	
	/* 获取设置的时间值 */
	normalsavetime = 
	Nml_Save_Irl_Buf[0]*Hour_Few_Second_Num*10	+
	Nml_Save_Irl_Buf[1]*Hour_Few_Second_Num		+
	Nml_Save_Irl_Buf[2]*Minute_Few_Second_Num*10+
	Nml_Save_Irl_Buf[3]*Minute_Few_Second_Num	+
	Nml_Save_Irl_Buf[4]*10						+
	Nml_Save_Irl_Buf[5];
	
	/* 判断设置的时间值是否在可以设置的范围内 */
	if((normalsavetime == 0)||(normalsavetime > Set_Max_Second_Num))
	{
		/* 设置的时间参数超出的设置范围 */
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 更新菜单界面显示 */
		Jump2Menu(MenuNmlSaveIrlIndex,FlashMode_AutoInit);
		/* 退出函数 */
		return;
	}
	
	//
	if(User == Domestic)//普通用户
	{	
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 更新菜单界面显示 */
		Jump2Menu(MenuNmlSaveIrlIndex,FlashMode_AutoInit);
	}else if(User == Admin)
	{
		//时间的输入方式应该更佳人性化点比较好。
		*Menu_Nml_Save_Irl_Data_P	= normalsavetime;
		
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)Menu_Nml_Save_Irl_Data_P,sizeof(*Menu_Nml_Save_Irl_Data_P));
		
//		/* 配置信息 */
//		GSM_Config_Tab[5].CR.Change_Type = 6;
//		GSM_Config_Tab[5].CR.Change_Adder = 0;
//		GSM_Config_Tab[5].CR.Change_Data_Len = Instru_Menu_Inf_State_Space_Size;
		/* 参数设置成功 */
		Parameter_Setting_Successful();
		/* 退回到上一级界面 */
		POP();
	}else if(User == Super_Admin)//超级管理员
	{
		/* 更新菜单界面显示 */
		Jump2Menu(MenuNmlSaveIrlIndex,FlashMode_NoAction);
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuNmlSaveIrlCancel															*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 光标指向前一个参数的位置															*/
/* 创建日期	: 2015/06/08																		*/
/* 修改日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuNmlSaveIrlCancel(void) 
{

	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = Nml_Save_Irl_Buf_Size-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	Normal_Save_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuNmlSaveIrlIndex,FlashMode_NoAction);
}

/******************* (C) ZHOU *****END OF FILE****/

