/************************************************************************************************/
/** @file              : MenuGWAdder.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月30日															*/
/** @date changed      : 2016年11月30日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->网关地址显示/设置												*/
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

/* 网关地址数据寄存器大小 */
#define MenuGWAdder_BufSize 			(3u)

/* 上传网络地址数据指针 */
#define MenuGWAdderDatap (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.Adder)

/* 网关地址数据寄存器 */
static u8 MenuGWAdder_Buf[MenuGWAdder_BufSize] = {0};


/* 网关地址数据寄存器参数设置界面 中英文 菜单栏 */
const struct Menu MenuGWAdder_Item[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};


/************************************************************************************************/
/* 函数名	: MenuGWAdder_ParaDis																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2016/11/30																		*/
/************************************************************************************************/
static void MenuGWAdder_ParaDis(u8 cursor)
{

	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispDecAt(MenuGWAdder_Buf[0],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
			break;
		}
		
		case 1:
		{
			GUI_DispDecAt(MenuGWAdder_Buf[1],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);	
			break;
		}
		
		case 2:
		{
			GUI_DispDecAt(MenuGWAdder_Buf[2],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
			break;
		}
		
		
		default:
		{
			break;
		}
	}
}

/************************************************************************************************/
/* 函数名	: MenuGWAdder_ParaSet																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2016/11/30																		*/
/************************************************************************************************/
static void MenuGWAdder_ParaSet(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	MenuGWAdder_ParaDis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	MenuGWAdder_ParaDis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWAdderInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示网关地址界面信息																*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWAdderInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&MenuGWAdder_Item[*Language];
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

		/* 设置 网关地址 参数 */
		/* IP地址数据 */
		MenuGWAdder_Buf[0] = *MenuGWAdderDatap%1000/100;
		MenuGWAdder_Buf[1] = *MenuGWAdderDatap%100/10;
		MenuGWAdder_Buf[2] = *MenuGWAdderDatap%10;
		
		/* 显示 网关地址 参数 */
//		GUI_DispDecAt(MenuGWAdder_Buf[0],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
		GUI_DispDecAt(MenuGWAdder_Buf[1],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
		GUI_DispDecAt(MenuGWAdder_Buf[2],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
	
		/* 显示当前修改的参数位置 */
		MenuGWAdder_ParaSet(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();	
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWAdderUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWAdderUp(void)
{
	/* 网关地址数据 */
	uint16_t gAdderValue;
	
	
	switch(Item)
	{
		case 0:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 0;
			break;
		}
		case 1:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 1;
			break;
		}
		case 2:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 2;
			break;
		}
		case 3:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 3;
			break;
		}
		case 4:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 4;
			break;
		}
		case 5:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 5;
			break;
		}
		case 6:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 6;
			break;
		}
		case 7:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 7;
			break;
		}
		case 8:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 8;
			break;
		}
		case 9:
		{
			/* 设置缓存数据 */
			MenuGWAdder_Buf[Input_Para_Win_Cursor] = 9;
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
	MenuGWAdder_Buf[0]*100 + MenuGWAdder_Buf[1]*10 + MenuGWAdder_Buf[2];
	/* 判断网关地址数据是否超出范围 */
	if(gAdderValue > 255)
	{
		MenuGWAdder_Buf[0] = 2;
		MenuGWAdder_Buf[1] = 5;
		MenuGWAdder_Buf[2] = 5;
	}
	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor < (MenuGWAdder_BufSize-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}
	else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}

	MenuGWAdder_ParaSet(0);
	MenuGWAdder_ParaSet(1);
	MenuGWAdder_ParaSet(2);
	
	/* 显示当前修改的参数位置 */
	MenuGWAdder_ParaSet(Input_Para_Win_Cursor);
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuGWAdderIndex,FlashMode_NoAction);
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWAdderDown																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出网关地址界面																		*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWAdderDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuGWAdderLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 															*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWAdderLeft(void)
{
	/* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuGWAdderIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWAdderRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 															*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWAdderRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuGWAdderIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWAdderOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWAdderOK(void)
{
	/* 判断当前用户类型 */
	if(User == Domestic)//普通用户
	{
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 更新菜单界面显示 */
		Jump2Menu(MenuGWAdderIndex,FlashMode_AutoInit);
	}else if(User == Admin)
	{
		/* 网关地址参数 */
		uint8_t gAdder;
		/* 设置网关地址操作状态 */
		GW_Result gState;
		
		
		/* 设置 网关地址 */
		gAdder = 
		MenuGWAdder_Buf[0]*100 + MenuGWAdder_Buf[1]*10 + MenuGWAdder_Buf[2];
		
		/* 发送修改网关地址命令 */
		gState = AppGW_API.SetAdder(gAdder);
		
		/* 判断命令执行结果 */
		if(GW_Success == gState)
		{
			/* 重启网关 */
			AppGW_API.Restart();
			
			/* 将新的网关地址写入到寄存器中 */
			*MenuGWAdderDatap = gAdder;
			
			/* 修改仪器配置表中对应网关地址数据 */
			Write_Config_Tab(MenuGWAdderDatap,sizeof(*MenuGWAdderDatap));
			
			/* 数据写入成功 */
			Parameter_Setting_Successful();
			
			/* 退回到上一级界面 */
			POP();
		}else
		{
			/* 设置参数失败。 */
			Parameter_Setting_Failure();
			/* 更新菜单界面显示 */
			Jump2Menu(MenuGWAdderIndex,FlashMode_AutoInit);
		}
	}else if(User == Super_Admin)//超级管理员
	{
		/* 更新菜单界面显示 */
		Jump2Menu(MenuGWAdderIndex,FlashMode_NoAction);
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuGWAdderCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2016/11/30																		*/
/* 修改日期	: 2016/11/30																		*/
/************************************************************************************************/
void DispMenuGWAdderCancel(void)
{
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = MenuGWAdder_BufSize-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	MenuGWAdder_ParaSet(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuGWAdderIndex,FlashMode_NoAction);
}  


#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
