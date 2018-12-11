/************************************************************************************************/
/** @file              : MenuGWChannel.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年12月01日															*/
/** @date changed      : 2016年12月01日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->网关信道显示/设置												*/
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

/* 网关信道数据寄存器大小 */
#define MenuGWChannel_BufSize 			(1u)

/* 上传网络信道数据指针 */
#define MenuGWChannelDatap (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.Channel)

/* 网关信道数据寄存器 */
static u8 MenuGWChannel_Buf = 0;


/* 网关信道数据寄存器参数设置界面 中英文 菜单栏 */
const struct Menu MenuGWChannel_Item[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};


/************************************************************************************************/
/* 函数名	: MenuGWChannel_ParaDis																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2016/12/01																		*/
/************************************************************************************************/
static void MenuGWChannel_ParaDis(u8 cursor)
{

	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispDecAt(MenuGWChannel_Buf,Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
			break;
		}
		
		
		default:
		{
			break;
		}
	}
}

/************************************************************************************************/
/* 函数名	: MenuGWChannel_ParaSet																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2016/12/01																		*/
/************************************************************************************************/
static void MenuGWChannel_ParaSet(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	MenuGWChannel_ParaDis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	MenuGWChannel_ParaDis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWChannelInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示网关信道界面信息																*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWChannelInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&MenuGWChannel_Item[*Language];
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

		/* 设置 网关信道 参数 */
		/* IP信道数据 */
		MenuGWChannel_Buf = *MenuGWChannelDatap%10;
	
		/* 显示当前修改的参数位置 */
		MenuGWChannel_ParaSet(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();	
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWChannelUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWChannelUp(void)
{
	/* 网关地址数据 */
	uint8_t gChannelValue;
	
	switch(Item)
	{
		case 0:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 0;
			break;
		}
		case 1:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 1;
			break;
		}
		case 2:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 2;
			break;
		}
		case 3:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 3;
			break;
		}
		case 4:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 4;
			break;
		}
		case 5:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 5;
			break;
		}
		case 6:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 6;
			break;
		}
		case 7:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 7;
			break;
		}
		case 8:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 8;
			break;
		}
		case 9:
		{
			/* 设置缓存数据 */
			MenuGWChannel_Buf = 9;
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
	
	/* 获取网关信道数据 */
	gChannelValue = MenuGWChannel_Buf;
	/* 判断信道数据是否超出范围 */
	if(gChannelValue > 7)
	{
		/* 修改网关信道数据寄存器 */
		MenuGWChannel_Buf = 7;
	}
	

	/* 显示当前修改的参数位置 */
	MenuGWChannel_ParaSet(Input_Para_Win_Cursor);
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuGWChannelIndex,FlashMode_NoAction);
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWChannelDown																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出网关信道界面																		*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWChannelDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuGWChannelLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWChannelLeft(void)
{
	/* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuGWChannelIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWChannelRight															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWChannelRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuGWChannelIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWChannelOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWChannelOK(void)
{
	/* 判断当前用户类型 */
	if(User == Domestic)//普通用户
	{
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 更新菜单界面显示 */
		Jump2Menu(MenuGWChannelIndex,FlashMode_AutoInit);
	}else if(User == Admin)
	{
		/* 网关信道参数 */
		uint8_t gChannel;
		/* 设置网关信道操作状态 */
		GW_Result gState;
		
		
		/* 设置 网关信道 */
		gChannel = MenuGWChannel_Buf;
		
		/* 发送修改网关信道命令 */
		gState = AppGW_API.SetChannel(gChannel);
		
		/* 判断命令执行结果 */
		if(GW_Success == gState)
		{
			/* 重启网关 */
			AppGW_API.Restart();
			
			/* 将新的网关信道写入到寄存器中 */
			*MenuGWChannelDatap = gChannel;
			
			/* 修改仪器配置表中对应网关信道数据 */
			Write_Config_Tab(MenuGWChannelDatap,sizeof(*MenuGWChannelDatap));
			
			/* 数据写入成功 */
			Parameter_Setting_Successful();
			
			/* 退回到上一级界面 */
			POP();
		}else
		{
			/* 设置参数失败。 */
			Parameter_Setting_Failure();
			/* 更新菜单界面显示 */
			Jump2Menu(MenuGWChannelIndex,FlashMode_AutoInit);
		}
	}else if(User == Super_Admin)//超级管理员
	{
		/* 更新菜单界面显示 */
		Jump2Menu(MenuGWChannelIndex,FlashMode_NoAction);
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuGWChannelCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWChannelCancel(void)
{
	/* 显示当前修改的参数位置 */
	MenuGWChannel_ParaSet(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuGWChannelIndex,FlashMode_NoAction);
}  


#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
