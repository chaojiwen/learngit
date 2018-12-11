/************************************************************************************************/
/** @file              : MenuAlarmDelay.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月30日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->报警延时时间													*/
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
#define	Hour_Few_Second_Num		(3600u)
/* 一分钟的秒钟数 */
#define	Minute_Few_Second_Num	(60u)
/* 最大可设置的秒钟数 */
#define	Set_Max_Second_Num		(3600u)//最大1小时

/* 报警延时界面的数据缓存大小 */
#define Aam_Dly_Buf_Size		(6u)

/* 报警延时隔界面的数据缓存 */
static u8 Aam_Dly_Buf[Aam_Dly_Buf_Size] = {0};

/* 报警延时界面 中英文 菜单栏 */
const struct Menu Menu_Aam_Dly[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};

/* 通道报警延时数据指针. */
#define Cha_Aam_Dly_Data_P(cha)		(&((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[cha*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Alarm_Delay)


/************************************************************************************************/
/* 函数名	: Aam_Dly_Para_Dis																	*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
static void Aam_Dly_Para_Dis(u8 cursor)
{

	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			//小时-数据-十位		
			GUI_DispDecAt(Aam_Dly_Buf[0],Line_3_PosX+Font1608_Size*5 ,Line_3_PosY,1);
			break;
		}
		
		case 1:
		{
			//小时-数据-个位		
			GUI_DispDecAt(Aam_Dly_Buf[1],Line_3_PosX+Font1608_Size*6 ,Line_3_PosY,1);
			break;
		}
		
		case 2:
		{
			//分钟-数据-十位		
			GUI_DispDecAt(Aam_Dly_Buf[2],Line_3_PosX+Font1608_Size*8 ,Line_3_PosY,1);
			break;
		}
		
		case 3:
		{
			//分钟-数据-个位	
			GUI_DispDecAt(Aam_Dly_Buf[3],Line_3_PosX+Font1608_Size*9 ,Line_3_PosY,1);
			break;
		}
		
		case 4:
		{
			//秒钟-数据-十位	
			GUI_DispDecAt(Aam_Dly_Buf[4],Line_3_PosX+Font1608_Size*11 ,Line_3_PosY,1);
			break;
		}
		
		case 5:
		{
			//秒钟-数据-个位	
			GUI_DispDecAt(Aam_Dly_Buf[5],Line_3_PosX+Font1608_Size*12 ,Line_3_PosY,1);
			break;
		}
		
		default:
		{
			break;
		}
	}
	
}

/************************************************************************************************/
/* 函数名	: Aam_Dly_Para_Set																	*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
static void Aam_Dly_Para_Set(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	Aam_Dly_Para_Dis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	Aam_Dly_Para_Dis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;

}

/************************************************************************************************/
/* 函数名	: DispMenuAamDlyInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示报警状态的记录时间间隔界面信息												*/
/* 创建日期	: 2015/09/30																		*/
/************************************************************************************************/
void DispMenuAamDlyInit(void)
{
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Aam_Dly[*Language];
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
		GUI_Disp_Headline(*(Menu_Alarm_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));

		/* 设置 正常状态数据存储间隔 参数 */
		/* 设置小时参数 */
		Aam_Dly_Buf[0] = (*Cha_Aam_Dly_Data_P(Sensors_CH_1)/Hour_Few_Second_Num)/10;
		Aam_Dly_Buf[1] = (*Cha_Aam_Dly_Data_P(Sensors_CH_1)/Hour_Few_Second_Num)%10;
		/* 设置分钟参数 */
		Aam_Dly_Buf[2] = ((*Cha_Aam_Dly_Data_P(Sensors_CH_1)%Hour_Few_Second_Num)/Minute_Few_Second_Num)/10;
		Aam_Dly_Buf[3] = ((*Cha_Aam_Dly_Data_P(Sensors_CH_1)%Hour_Few_Second_Num)/Minute_Few_Second_Num)%10;
		/* 设置秒钟参数 */
		Aam_Dly_Buf[4] = (*Cha_Aam_Dly_Data_P(Sensors_CH_1)%Minute_Few_Second_Num)/10;
		Aam_Dly_Buf[5] = (*Cha_Aam_Dly_Data_P(Sensors_CH_1)%Minute_Few_Second_Num)%10;
		/* 显示固定位置的符号 */
		GUI_DispStringAt((*Language == CN)?(u8 *)"设置:时:分:秒":(u8 *)" Set:HH:MM:SS",Line_2_PosX,Line_2_PosY);
		GUI_DispStringAt(":",Line_3_PosX+Font1608_Size*7,Line_3_PosY);
		GUI_DispStringAt(":",Line_3_PosX+Font1608_Size*10,Line_3_PosY);
		
		/* 显示 正常状态的记录时间间隔的参数 */
//		GUI_DispDecAt(Alarm_Save_Interval_Buffer[0],Line_3_PosX+Font1608_Size*5 ,Line_3_PosY,1);
		GUI_DispDecAt(Aam_Dly_Buf[1],Line_3_PosX+Font1608_Size*6 ,Line_3_PosY,1);
		GUI_DispDecAt(Aam_Dly_Buf[2],Line_3_PosX+Font1608_Size*8 ,Line_3_PosY,1);
		GUI_DispDecAt(Aam_Dly_Buf[3],Line_3_PosX+Font1608_Size*9 ,Line_3_PosY,1);
		GUI_DispDecAt(Aam_Dly_Buf[4],Line_3_PosX+Font1608_Size*11 ,Line_3_PosY,1);
		GUI_DispDecAt(Aam_Dly_Buf[5],Line_3_PosX+Font1608_Size*12 ,Line_3_PosY,1);
		
		/* 显示当前修改的参数位置 */
		Aam_Dly_Para_Set(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuAamDlyUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置数据																			*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuAamDlyUp(void)
{
    switch(Item)
	{
        case 0:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 0;
			break;
		}
        case 1:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 1;		
            break;
        }
        case 2:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 2;
            break;
        }
        case 3:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 3;
            break;
        }
        case 4:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 4;
            break;
        }
        case 5:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 5;
            break;
        }
        case 6:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 6;
            break;
        }
        case 7:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 7;
            break;
        }
        case 8:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 8;
            break;
        }
        case 9:
		{
			/* 设置缓存数据 */
			Aam_Dly_Buf[Input_Para_Win_Cursor] = 9;
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
	if(Input_Para_Win_Cursor < (Aam_Dly_Buf_Size-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}
	
	/* 显示当前修改的参数位置 */
	Aam_Dly_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuAamDlyIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuAamDlyDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 正常状态存储时间间隔 界面													*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuAamDlyDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuAamDlyLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向前选择参数窗口数据																*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuAamDlyLeft(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuAamDlyIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuAamDlyRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向后选择参数窗口数据																*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuAamDlyRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuAamDlyIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuAamDlyOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置修改的参数（参数计时生效）													*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuAamDlyOK(void)
{
	/* 设置的时间参数 */
	u32 alarmsavetime;
	/* 计数变量 */
	u8 num;
	
	/* 获取设置的时间值 */
	alarmsavetime = 
	Aam_Dly_Buf[0]*Hour_Few_Second_Num*10	+
	Aam_Dly_Buf[1]*Hour_Few_Second_Num		+
	Aam_Dly_Buf[2]*Minute_Few_Second_Num*10	+
	Aam_Dly_Buf[3]*Minute_Few_Second_Num	+
	Aam_Dly_Buf[4]*10						+
	Aam_Dly_Buf[5];
	
	/* 判断设置的时间值是否在可以设置的范围内 */
	if(alarmsavetime > Set_Max_Second_Num)
	{
		/* 设置的时间参数超出的设置范围 */
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 将函数指针指向初始化函数 */
		Jump2Menu(MenuAamDlyIndex,FlashMode_ReFlash);
		/* 退出函数 */
		return;
	}
	
	if(User == Domestic)//普通用户
	{	
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 将函数指针指向初始化函数 */
		Jump2Menu(MenuAamDlyIndex,FlashMode_ReFlash);
	}else if(User == Admin)
	{
	
		/* 设置 所有通道的 报警延时参数 */
		for(num = 0; num < Sensor_CH_Config_Num; num++)
		{
			/* 设置正常状态的报警时间间隔 */
			*Cha_Aam_Dly_Data_P(num) = alarmsavetime;

			/* 修改 仪器 信息表 设置标志 */
			Write_Config_Tab((u8*)Cha_Aam_Dly_Data_P(num),sizeof(*Cha_Aam_Dly_Data_P(num)));
		}
	
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
		/* 将函数指针指向初始化函数,更新显示 */
		Jump2Menu(MenuAamDlyIndex,FlashMode_NoAction);
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuAamDlyCancel																*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 光标指向前一个参数的位置															*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuAamDlyCancel(void) 
{
	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = Aam_Dly_Buf_Size-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	Aam_Dly_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuAamDlyIndex,FlashMode_NoAction);
}

/******************* (C) ZHOU *****END OF FILE****/



