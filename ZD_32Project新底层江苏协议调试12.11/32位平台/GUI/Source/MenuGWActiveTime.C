/************************************************************************************************/
/** @file              : MenuGWActiveTime.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年12月01日															*/
/** @date changed      : 2016年12月01日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->网关数据有效时间显示/设置												*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "System_Config.h"

#if (Gateway_NJSY_Enable == 0x01)

#include "Ks0108.H"
#include "Menu.H"

#include "Mcu_Config.h"

#include "App_Gateway_NJSY.h"

/* 网关数据有效时间数据寄存器大小 */
#define MenuGWActiveTime_BufSize 			(3u)

/* 上传网络数据有效时间数据指针 */
#define MenuGWActiveTimeDatap (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.ActiveTime)

/* 网关数据有效时间数据寄存器 */
static u8 MenuGWActiveTime_Buf[4] = {0};


/* 网关数据有效时间数据寄存器参数设置界面 中英文 菜单栏 */
const struct Menu MenuGWActiveTime_Item[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};


/************************************************************************************************/
/* 函数名	: MenuGWActiveTime_ParaDis																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2016/12/01																		*/
/************************************************************************************************/
static void MenuGWActiveTime_ParaDis(u8 cursor)
{

	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispDecAt(MenuGWActiveTime_Buf[0],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
			break;
		}
		
		case 1:
		{
			GUI_DispDecAt(MenuGWActiveTime_Buf[1],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);	
			break;
		}
		
		case 2:
		{
			GUI_DispDecAt(MenuGWActiveTime_Buf[2],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
			break;
		}
		
		
		default:
		{
			break;
		}
	}
}

/************************************************************************************************/
/* 函数名	: MenuGWActiveTime_ParaSet																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2016/12/01																		*/
/************************************************************************************************/
static void MenuGWActiveTime_ParaSet(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	MenuGWActiveTime_ParaDis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	MenuGWActiveTime_ParaDis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWActiveTimeInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示网关数据有效时间界面信息																*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWActiveTimeInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&MenuGWActiveTime_Item[*Language];
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
		GUI_Disp_Headline(*(MenuGW_Item[*Language].Text+ItemBackup[ItemBackup_i-1]));

		/* 设置 网关数据有效时间 参数 */
		/* IP数据有效时间数据 */
		MenuGWActiveTime_Buf[0] = *MenuGWActiveTimeDatap%1000/100;
		MenuGWActiveTime_Buf[1] = *MenuGWActiveTimeDatap%100/10;
		MenuGWActiveTime_Buf[2] = *MenuGWActiveTimeDatap%10;
		MenuGWActiveTime_Buf[3] = 0;
		
		/* 显示 网关数据有效时间 参数 */
//		GUI_DispDecAt(MenuGWActiveTime_Buf[0],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
		GUI_DispDecAt(MenuGWActiveTime_Buf[1],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
		GUI_DispDecAt(MenuGWActiveTime_Buf[2],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
		GUI_DispDecAt(MenuGWActiveTime_Buf[3],Line_2_PosX+Font1608_Size*8,Line_2_PosY,1);
		GUI_DispStringAt((*Language == CN)?(u8 *)"秒":(u8 *)"S",Line_2_PosX+Font1608_Size*10,Line_2_PosY);
		/* 显示当前修改的参数位置 */
		MenuGWActiveTime_ParaSet(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();	
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWActiveTimeUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWActiveTimeUp(void)
{
	/* 网关数据有效时间数据 */
	uint16_t gActiveTimeValue;
	
	
	switch(Item)
	{
		case 0:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 0;
			break;
		}
		case 1:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 1;
			break;
		}
		case 2:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 2;
			break;
		}
		case 3:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 3;
			break;
		}
		case 4:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 4;
			break;
		}
		case 5:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 5;
			break;
		}
		case 6:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 6;
			break;
		}
		case 7:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 7;
			break;
		}
		case 8:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 8;
			break;
		}
		case 9:
		{
			/* 设置缓存数据 */
			MenuGWActiveTime_Buf[Input_Para_Win_Cursor] = 9;
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
	
	/* 获取网关数据有效时间数据 */
	gActiveTimeValue = 
	MenuGWActiveTime_Buf[0]*1000 + 
	MenuGWActiveTime_Buf[1]*100 + 
	MenuGWActiveTime_Buf[2]*10 +
	MenuGWActiveTime_Buf[3];
	
	/* 判断设置的 月 参数 */
	if(gActiveTimeValue > 2550)
	{
		MenuGWActiveTime_Buf[0] = 2;
		MenuGWActiveTime_Buf[1] = 5;
		MenuGWActiveTime_Buf[2] = 5;
		MenuGWActiveTime_Buf[3] = 0;
	}else if(gActiveTimeValue == 0)
	{
		MenuGWActiveTime_Buf[0] = 0;
		MenuGWActiveTime_Buf[1] = 0;
		MenuGWActiveTime_Buf[2] = 1;
		MenuGWActiveTime_Buf[3] = 0;
	}
	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor < (MenuGWActiveTime_BufSize-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}
	else
	{
	 	/* 数据缓存指针在最后一个缓存数据有效时间，将指针指向第一个数据缓存数据有效时间。 */
		Input_Para_Win_Cursor = 0;
	}

	MenuGWActiveTime_ParaSet(0);
	MenuGWActiveTime_ParaSet(1);
	MenuGWActiveTime_ParaSet(2);
	MenuGWActiveTime_ParaSet(3);
	
	/* 显示当前修改的参数位置 */
	MenuGWActiveTime_ParaSet(Input_Para_Win_Cursor);
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuGWActiveTimeIndex,FlashMode_NoAction);
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWActiveTimeDown																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出网关数据有效时间界面																		*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWActiveTimeDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuGWActiveTimeLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 															*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWActiveTimeLeft(void)
{
	/* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuGWActiveTimeIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWActiveTimeRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 															*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWActiveTimeRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuGWActiveTimeIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWActiveTimeOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWActiveTimeOK(void)
{
	/* 判断当前用户类型 */
	if(User == Domestic)//普通用户
	{
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 更新菜单界面显示 */
		Jump2Menu(MenuGWActiveTimeIndex,FlashMode_AutoInit);
	}else if(User == Admin)
	{
		/* 网关数据有效时间参数 */
		uint8_t gActiveTime;
		/* 设置网关数据有效时间操作状态 */
		GW_Result gState;
		
		
		/* 设置 网关数据有效时间 */
		gActiveTime = 
		MenuGWActiveTime_Buf[0]*100 + 
		MenuGWActiveTime_Buf[1]*10 + 
		MenuGWActiveTime_Buf[2];
		
		/* 发送修改网关数据有效时间命令 */
		gState = AppGW_API.SetActiveTime(gActiveTime);
		
		/* 判断命令执行结果 */
		if(GW_Success == gState)
		{
			/* 重启网关 */
			AppGW_API.Restart();
			
			/* 将新的网关数据有效时间写入到寄存器中 */
			*MenuGWActiveTimeDatap = gActiveTime;
			
			/* 修改仪器配置表中对应网关数据有效时间数据 */
			Write_Config_Tab(MenuGWActiveTimeDatap,sizeof(*MenuGWActiveTimeDatap));
			
			/* 数据写入成功 */
			Parameter_Setting_Successful();
			
			/* 退回到上一级界面 */
			POP();
		}else
		{
			/* 设置参数失败。 */
			Parameter_Setting_Failure();
			/* 更新菜单界面显示 */
			Jump2Menu(MenuGWActiveTimeIndex,FlashMode_AutoInit);
		}
	}else if(User == Super_Admin)//超级管理员
	{
		/* 更新菜单界面显示 */
		Jump2Menu(MenuGWActiveTimeIndex,FlashMode_NoAction);
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuGWActiveTimeCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWActiveTimeCancel(void)
{
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = MenuGWActiveTime_BufSize-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存数据有效时间，将指针指向第一个数据缓存数据有效时间。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	MenuGWActiveTime_ParaSet(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuGWActiveTimeIndex,FlashMode_NoAction);
}  


#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
