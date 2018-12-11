/************************************************************************************************/
/** @file              : MenuGWMode.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年12月01日															*/
/** @date changed      : 2016年12月01日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->网关模式显示/设置												*/
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


/* 网关模式数据指针 */
#define MenuGWModeDatap (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State.Mode)


/* 网关模式界面 中文 菜单项目栏 */
static u8 * const MenuGWMode_CN[]=
{
	{"正常模式"},
	{"调试模式"},
};

/* 网关模式界面 英文 菜单项目栏 */
static u8 * const MenuGWMode_EN[]=
{
	{"Release Mode"},
	{"Debug Mode"},
};

/* 网关模式界面 中英文 菜单项目栏 */
static const struct Menu MenuGWMode_Item[] =
{
	MenuWhat(MenuGWMode_CN),
	MenuWhat(MenuGWMode_EN),
};


/************************************************************************************************/
/* 函数名	: DispMenuGWModeInit															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示网关模式界面信息																*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWModeInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&MenuGWMode_Item[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		/* 设置设置项的位置，设置默认状态为不选择升级 */
		if(GW_Release == *((GW_Mode*)MenuGWModeDatap))
		{
			/* 正常模式 */
			Item = 0;
		}else
		{
			/* 调试模式 */
			Item = 1;
		}
		Where = Item;
		
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(MenuGW_Item[*Language].Text+ItemBackup[ItemBackup_i-1]));	
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuGWModeUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWModeUp(void)
{
	switch(Item)
	{
        case 0:
		{
			/* 根据当前用户的级别设置参数 */
			if(User == Domestic)
			{	
				/* 显示参数配置错误 提示信息 */
				Parameter_Setting_Failure();
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuGWModeIndex,FlashMode_AutoInit);
			}else if((User == Admin)||(User == Super_Admin))
			{
				/* 管理员帐号或超级管理员帐号 */
				
				/* 判断网关模式状态 */
				if(GW_Debug == *((GW_Mode*)MenuGWModeDatap))
				{
					/* 设置网关模式操作状态 */
					GW_Result gState;
					
					/* 发送修改网关模式命令 */
					gState = AppGW_API.SetMode(GW_Release);
					
					/* 判断命令执行结果 */
					if(GW_Success == gState)
					{
						/* 重启网关 */
						AppGW_API.Restart();
					
						/* 将新的网关地址写入到寄存器中 */
						*((GW_Mode*)MenuGWModeDatap) = GW_Release;						
					
						/* 修改仪器配置表中对应网关地址数据 */
						Write_Config_Tab(MenuGWModeDatap,sizeof(*MenuGWModeDatap));
					
						/* 参数设置成 */
						Parameter_Setting_Successful();
						/* 退回到上一级界面 */
						POP();	
					}else
					{
						/* 设置参数失败。 */
						Parameter_Setting_Failure();
						/* 更新菜单界面显示 */
						Jump2Menu(MenuGWModeIndex,FlashMode_AutoInit);
					}
				}else
				{
				    /* 将函数指针指向初始化函数,更新显示 */
					Jump2Menu(MenuGWModeIndex,FlashMode_NoAction);
				}
			}
			break;
		}
        case 1:
		{
			/* 根据当前用户的级别设置参数 */
			if(User == Domestic)//普通用户
			{	
				/* 显示参数配置错误 提示信息 */
				Parameter_Setting_Failure();
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuGWModeIndex,FlashMode_AutoInit);
			}else if((User == Admin)||(User == Super_Admin))
			{
				/* 管理员帐号或超级管理员帐号 */
				
				/* 判断当前打印GSM调试信息状态 */
				if(GW_Release == *((GW_Mode*)MenuGWModeDatap))
				{
					/* 设置网关模式操作状态 */
					GW_Result gState;
					
					/* 发送修改网关模式命令 */
					gState = AppGW_API.SetMode(GW_Debug);
					
					/* 判断命令执行结果 */
					if(GW_Success == gState)
					{
						/* 重启网关 */
						AppGW_API.Restart();
						
						/* 将新的网关地址写入到寄存器中 */
						*((GW_Mode*)MenuGWModeDatap) = GW_Debug;
						
						/* 修改仪器配置表中对应网关地址数据 */
						Write_Config_Tab(MenuGWModeDatap,sizeof(*MenuGWModeDatap));
					
						/* 参数设置成 */
						Parameter_Setting_Successful();
						/* 退回到上一级界面 */
						POP();	
					}else
					{
						/* 设置参数失败。 */
						Parameter_Setting_Failure();
						/* 更新菜单界面显示 */
						Jump2Menu(MenuGWModeIndex,FlashMode_AutoInit);
					}
				}else
				{
				    /* 将函数指针指向初始化函数,更新显示 */
					Jump2Menu(MenuGWModeIndex,FlashMode_NoAction);
				}
			}
            break;
        }

		default:
		{
            /* 将函数指针指向初始化函数,更新显示 */
			Jump2Menu(MenuGWModeIndex,FlashMode_NoAction);
			break;
        }
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuGWModeDown																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出网关模式界面																		*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWModeDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuGWModeLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWModeLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWModeIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWModeRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 																					*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWModeRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWModeIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWModeOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWModeOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWModeIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuGWModeCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2016/12/01																		*/
/* 修改日期	: 2016/12/01																		*/
/************************************************************************************************/
void DispMenuGWModeCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuGWModeIndex,FlashMode_NoAction);
}  


#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
