/************************************************************************************************/
/** @file              : MenuUserPermSet.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月15日															*/
/** @date changed      : 2015年10月16日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->用户权限设置界面												*/
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


#include <string.h>


/* 用户权限设置界面的数据缓存大小 */
#define User_Perm_Set_Buffer_Size 			(6u)

/* 用户权限设置界面的数据缓存 */
static u8 User_Perm_Set_Buffer[User_Perm_Set_Buffer_Size] = {0};
/* 管理员密码 */
const u8 Admin_Password[] = {1,2,3,4,5,6};
/* 超级管理员密码 */
const u8 Super_Admin_Password[] = {4,5,6,7,8,9};
/* 根据当前用户输入的设备密码，设定不同用户身份.(1:普通用户；2:管理员) */
User_Type User = Domestic;

/* 用户权限设置菜单项目栏 */
static const struct Menu Menu_User_Perm_Set[] = 
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};

/************************************************************************************************/
/* 函数名	: User_Perm_Set_Para_Dis															*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置																*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/
static void User_Perm_Set_Para_Dis(u8 cursor)
{

	
	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispDecAt(User_Perm_Set_Buffer[0],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
			break;
		}
		
		case 1:
		{
			GUI_DispDecAt(User_Perm_Set_Buffer[1],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
			break;
		}
		
		case 2:
		{
			GUI_DispDecAt(User_Perm_Set_Buffer[2],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
			break;
		}
		
		case 3:
		{
			GUI_DispDecAt(User_Perm_Set_Buffer[3],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
			break;
		}
		
		case 4:
		{
			GUI_DispDecAt(User_Perm_Set_Buffer[4],Line_2_PosX+Font1608_Size*10 ,Line_2_PosY,1);
			break;
		}
		
		case 5:
		{
			GUI_DispDecAt(User_Perm_Set_Buffer[5],Line_2_PosX+Font1608_Size*11 ,Line_2_PosY,1);
			break;
		}
		
		default:
		{
			break;
		}
	}
	
}

/************************************************************************************************/
/* 函数名	: User_Perm_Set_Para_Set															*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色										*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/
static void User_Perm_Set_Para_Set(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	User_Perm_Set_Para_Dis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	User_Perm_Set_Para_Dis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;

}

/************************************************************************************************/
/* 函数名	: DispMenuUserPermSetInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示用户权限设置信息																	*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/
void DispMenuUserPermSetInit(void)
{

	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_User_Perm_Set[*Language];
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
		GUI_Disp_Headline((u8*)((*Language == EN)?"User Perm":"用户权限"));
		/* 设置 用户权限 参数 */
		User_Perm_Set_Buffer[0] = 0; 
		User_Perm_Set_Buffer[1] = 0;
		User_Perm_Set_Buffer[2] = 0;
		User_Perm_Set_Buffer[3] = 0;
		User_Perm_Set_Buffer[4] = 0;
		User_Perm_Set_Buffer[5] = 0;
		/* 显示固定位置的符号 */
		GUI_DispStringAt((u8*)((*Language == EN)?" PWD:":"密码:"),Line_2_PosX+Font1608_Size,Line_2_PosY);

		/* 显示 正常状态的记录时间间隔的参数 */
//		GUI_DispDecAt(User_Perm_Set_Buffer[0],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
		GUI_DispDecAt(User_Perm_Set_Buffer[1],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
		GUI_DispDecAt(User_Perm_Set_Buffer[2],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);	
		GUI_DispDecAt(User_Perm_Set_Buffer[3],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
		GUI_DispDecAt(User_Perm_Set_Buffer[4],Line_2_PosX+Font1608_Size*10 ,Line_2_PosY,1);
		GUI_DispDecAt(User_Perm_Set_Buffer[5],Line_2_PosX+Font1608_Size*11 ,Line_2_PosY,1);

		/* 显示当前修改的参数位置 */
		User_Perm_Set_Para_Set(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();
		
	}
		
}

/************************************************************************************************/
/* 函数名	: DispMenuUserPermSetUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置数据																			*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/
void DispMenuUserPermSetUp(void)
{

	
    switch(Item)
	{
        case 0:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 0;
			break;
		}
        case 1:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 1;
            break;
        }
        case 2:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 2;
            break;
        }
        case 3:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 3;
            break;
        }
        case 4:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 4;
            break;
        }
        case 5:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 5;
            break;
        }
		case 6:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 6;
            break;
        }
		case 7:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 7;
            break;
        }
		case 8:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 8;
            break;
        }
		case 9:
		{
			User_Perm_Set_Buffer[Input_Para_Win_Cursor] = 9;
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
	if(Input_Para_Win_Cursor < (User_Perm_Set_Buffer_Size-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}
	
	/* 显示当前修改的参数位置 */
	User_Perm_Set_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuUserPermSetIndex,FlashMode_NoAction);

}

/************************************************************************************************/
/* 函数名	: DispMenuUserPermSetDown															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 用户权限设置 界面																*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/
void DispMenuUserPermSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuAlarmSaveIntervalLeft														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向前选择参数窗口数据																	*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/
void DispMenuUserPermSetLeft(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuUserPermSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuUserPermSetRight															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向后选择参数窗口数据																	*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/
void DispMenuUserPermSetRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuUserPermSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuAlarmSaveIntervalOK														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置修改的参数（参数计时生效）														*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
void DispMenuUserPermSetOK(void)
{

	if(memcmp(User_Perm_Set_Buffer,(u8*)Admin_Password,User_Perm_Set_Buffer_Size)==0)
	{
		/* 设置当前用户为 “管理员” */
		User = Admin;
		/* 参数设置成功 */
		Parameter_Setting_Successful();
		/* 退回到上一级界面 */
		POP();
	}else if(memcmp(User_Perm_Set_Buffer,(u8*)Super_Admin_Password,User_Perm_Set_Buffer_Size)==0)
	{
		/* 设置当前用户为 “超级管理员” */
		User = Super_Admin;
		/* 参数设置成功 */
		Parameter_Setting_Successful();
		/* 退回到上一级界面 */
		POP();
	}else
	{
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 将函数指针指向初始化函数 */
		Jump2Menu(MenuUserPermSetIndex,FlashMode_AutoInit);
	}	

}
/************************************************************************************************/
/* 函数名	: DispMenuAlarmSaveIntervalCancel													*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 光标指向前一个参数的位置																*/
/* 创建日期	: 2015/6/3																			*/
/************************************************************************************************/
void DispMenuUserPermSetCancel(void) 
{

	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = User_Perm_Set_Buffer_Size-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	User_Perm_Set_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuUserPermSetIndex,FlashMode_NoAction);
	
}

/******************* (C) ZHOU *****END OF FILE****/


