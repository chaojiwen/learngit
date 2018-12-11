/************************************************************************************************/
/** @file              : MenuGWHZZHWkupTime.c													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017年01月11日															*/
/** @date changed      : 2017年01月11日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->网关 唤醒时间 显示/设置											*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "System_Config.h"

#if (Gateway_HZZH_Enable == 0x01)

#include "Ks0108.H"
#include "Menu.H"

#include "Mcu_Config.h"

#include "App_Gateway_HZZH.h"

/* 网关地址数据寄存器大小 */
#define MenuGWHZZHWkupTime_BufSize 			(4u)

/* 网关地址数据寄存器 */
static u8 MenuGWHZZHWkupTime_Buf[MenuGWHZZHWkupTime_BufSize] = {0};

/* 杭州创辉网关唤醒时间 */
static uint16_t MenuWkupTime;
/* 杭州创辉网关标签数量 */
static uint16_t MenuProbeNum;

/* 网关地址数据寄存器参数设置界面 中英文 菜单栏 */
const struct Menu MenuGWHZZHWkupTime_Item[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};


/************************************************************************************************/
/* 函数名	: MenuGWHZZHWkupTime_ParaDis																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2016/11/30																		*/
/************************************************************************************************/
static void MenuGWHZZHWkupTime_ParaDis(u8 cursor)
{

	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispDecAt(MenuGWHZZHWkupTime_Buf[0],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
			break;
		}
		
		case 1:
		{
			GUI_DispDecAt(MenuGWHZZHWkupTime_Buf[1],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);	
			break;
		}
		
		case 2:
		{
			GUI_DispDecAt(MenuGWHZZHWkupTime_Buf[2],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
			break;
		}
		
		case 3:
		{
			GUI_DispDecAt(MenuGWHZZHWkupTime_Buf[3],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
			break;
		}
		
		default:
		{
			break;
		}
	}
}

/************************************************************************************************/
/* 函数名	: MenuGWHZZHWkupTime_ParaSet																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2016/11/30																		*/
/************************************************************************************************/
static void MenuGWHZZHWkupTime_ParaSet(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	MenuGWHZZHWkupTime_ParaDis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	MenuGWHZZHWkupTime_ParaDis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWkupTimeInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示网关地址界面信息																*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWkupTimeInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 设置网关地址操作状态 */
		GWHZZH_Result mState;
		
		
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&MenuGWHZZHWkupTime_Item[*Language];
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
		GUI_Disp_Headline(*(MenuGWHZZH_Item[*Language].Text+ItemBackup[ItemBackup_i-1]));
		
		/* 读杭州创辉网关的唤醒时间 */
		mState = AppGWHZZH_API.ReadWkupTimeProbeNum(&MenuWkupTime,&MenuProbeNum);
		
		/* 判断读网关唤醒时间状态 */
		if(GWHZZH_Fail == mState)
		{
			/* 读网关唤醒时间失败，退出当前界面 */
			POP();
			return;
		}
		
		/* 设置 网关地址 参数 */
		/* IP地址数据 */
		MenuGWHZZHWkupTime_Buf[0] = MenuWkupTime%10000/1000;
		MenuGWHZZHWkupTime_Buf[1] = MenuWkupTime%1000/100;
		MenuGWHZZHWkupTime_Buf[2] = MenuWkupTime%100/10;
		MenuGWHZZHWkupTime_Buf[3] = MenuWkupTime%10;
		
		/* 显示 网关地址 参数 */
//		GUI_DispDecAt(MenuGWHZZHWkupTime_Buf[0],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
		GUI_DispDecAt(MenuGWHZZHWkupTime_Buf[1],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
		GUI_DispDecAt(MenuGWHZZHWkupTime_Buf[2],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
		GUI_DispDecAt(MenuGWHZZHWkupTime_Buf[3],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
		GUI_DispStringAt((*Language == CN)?(u8 *)"秒":(u8 *)"S",Line_2_PosX+Font1608_Size*10,Line_2_PosY);
		
		/* 显示当前修改的参数位置 */
		MenuGWHZZHWkupTime_ParaSet(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();	
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWkupTimeUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWkupTimeUp(void)
{
	/* 网关地址数据 */
	uint16_t gAdderValue;
	
	
	switch(Item)
	{
		case 0:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 0;
			break;
		}
		case 1:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 1;
			break;
		}
		case 2:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 2;
			break;
		}
		case 3:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 3;
			break;
		}
		case 4:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 4;
			break;
		}
		case 5:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 5;
			break;
		}
		case 6:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 6;
			break;
		}
		case 7:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 7;
			break;
		}
		case 8:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 8;
			break;
		}
		case 9:
		{
			/* 设置缓存数据 */
			MenuGWHZZHWkupTime_Buf[Input_Para_Win_Cursor] = 9;
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
	
	/* 获取网关地址数据 */
	gAdderValue = 
	MenuGWHZZHWkupTime_Buf[0]*1000 + MenuGWHZZHWkupTime_Buf[1]*100 + 
	MenuGWHZZHWkupTime_Buf[2]*10 + MenuGWHZZHWkupTime_Buf[3];
	/* 判断网关地址数据是否超出范围 */
	if(gAdderValue > 3599)
	{
		/* 超过最大值 */
		MenuGWHZZHWkupTime_Buf[0] = 3;
		MenuGWHZZHWkupTime_Buf[1] = 5;
		MenuGWHZZHWkupTime_Buf[2] = 9;
		MenuGWHZZHWkupTime_Buf[3] = 9;
	}else if(gAdderValue == 0)
	{
		/* 低于最小值 */
		MenuGWHZZHWkupTime_Buf[0] = 0;
		MenuGWHZZHWkupTime_Buf[1] = 0;
		MenuGWHZZHWkupTime_Buf[2] = 0;
		MenuGWHZZHWkupTime_Buf[3] = 1;
	}
	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor < (MenuGWHZZHWkupTime_BufSize-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}
	else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}

	/* 显示当前修改的参数位置 */
	MenuGWHZZHWkupTime_ParaSet(0);
	MenuGWHZZHWkupTime_ParaSet(1);
	MenuGWHZZHWkupTime_ParaSet(2);
	MenuGWHZZHWkupTime_ParaSet(3);
	
	/* 显示当前修改的参数位置 */
	MenuGWHZZHWkupTime_ParaSet(Input_Para_Win_Cursor);
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuGWHZZHWkupTimeIndex,FlashMode_NoAction);
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWkupTimeDown																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出网关地址界面																		*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWkupTimeDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWkupTimeLeft														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWkupTimeLeft(void)
{
	/* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuGWHZZHWkupTimeIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWkupTimeRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 															*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWkupTimeRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuGWHZZHWkupTimeIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWkupTimeOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWkupTimeOK(void)
{
	/* 判断当前用户类型 */
	if(User == Domestic)//普通用户
	{
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 更新菜单界面显示 */
		Jump2Menu(MenuGWHZZHWkupTimeIndex,FlashMode_AutoInit);
	}else if(User == Admin)
	{
		/* 网关 唤醒时间 参数 */
		uint16_t mWkupTime;
		/* 设置网关地址操作状态 */
		GWHZZH_Result mState;
		
		
		/* 设置 网关地址 */
		mWkupTime = 
		MenuGWHZZHWkupTime_Buf[0]*1000 + MenuGWHZZHWkupTime_Buf[1]*100 + 
		MenuGWHZZHWkupTime_Buf[2]*10   + MenuGWHZZHWkupTime_Buf[3];
		
		/* 发送修改网关地址命令 */
		mState = AppGWHZZH_API.SetWkupTime(mWkupTime,MenuProbeNum);
		
		/* 判断命令执行结果 */
		if(GWHZZH_Success == mState)
		{

			/* 数据写入成功 */
			Parameter_Setting_Successful();
			
			/* 退回到上一级界面 */
			POP();
		}else
		{
			/* 设置参数失败。 */
			Parameter_Setting_Failure();
			/* 更新菜单界面显示 */
			Jump2Menu(MenuGWHZZHWkupTimeIndex,FlashMode_AutoInit);
		}
	}else if(User == Super_Admin)//超级管理员
	{
		/* 更新菜单界面显示 */
		Jump2Menu(MenuGWHZZHWkupTimeIndex,FlashMode_NoAction);
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuGWHZZHWkupTimeCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWHZZHWkupTimeCancel(void)
{
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = MenuGWHZZHWkupTime_BufSize-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	MenuGWHZZHWkupTime_ParaSet(0);
	MenuGWHZZHWkupTime_ParaSet(1);
	MenuGWHZZHWkupTime_ParaSet(2);
	MenuGWHZZHWkupTime_ParaSet(3);
	
	/* 显示当前修改的参数位置 */
	MenuGWHZZHWkupTime_ParaSet(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuGWHZZHWkupTimeIndex,FlashMode_NoAction);
}  


#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
