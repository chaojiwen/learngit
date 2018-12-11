/************************************************************************************************/
/** @file              : MenuFirmwareUrerade.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月13日															*/
/** @date changed      : 2015年10月16日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->固件升级														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "System_Config.h"

#if (Firmware_Urerade_Function_Enable == 0x01)

#include "Ks0108.H"
#include "Menu.H"

#include "app.h"



/* 仪器固件升级界面 中英文 菜单栏 */
static const struct Menu Menu_Fwe_Ure[] =
{
	MenuWhat(Menu_Yes_NO_CN),
	MenuWhat(Menu_Yes_NO_EN),
};


/* 固件升级数据指针 */
#define Menu_Fwe_Ure_Data_P		(*Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Remote_Upgrade_Fun)


/************************************************************************************************/
/* 函数名	: DispMenuFweUreInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示固件升级界面信息																*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuFweUreInit(void)
{
	
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Fwe_Ure[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		/* 设置设置项的位置，设置默认状态为不选择升级 */
        Item = 1;
        Where = Item;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Stm_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));	

    }

}

/************************************************************************************************/
/* 函数名	: DispMenuFweUreUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置固件升级功能																	*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuFweUreUp(void)
{
//	INT8U      err;

    switch(Item){
        case 0:
		{
			/* 设置 远程升级 标志 */
//			*Menu_Fwe_Ure_Data_P = 0x01;
			/* 修改 仪器 信息表 设置标志 */
//			Write_Config_Tab((u8*)Menu_Fwe_Ure_Data_P,sizeof(*Menu_Fwe_Ure_Data_P));
			/* 发送 接收完成 程序升级数据包 标志 */
//			OSFlagPost(		//向标志发信号
//			Fwe_Ure_Flag,	//发送标志的指针
//			(OS_FLAGS)0XFF, //选择要发送的信号 给第1位发信号 0010  同样把2强制转化为OS_FLAGS型的数据
//							//因为信号为OS_FLAGS型的
//			OS_FLAG_SET,	//信号有效的选项 信号置1  OS_FLAG_SET为置1  OS_FLAG_CLR为置0
//			&err);			//标志信号    
//			/* 配置信息 */
//			GSM_Config_Tab[0].CR.Change_Type = 1;
//			GSM_Config_Tab[0].CR.Change_Adder = 0;
//			GSM_Config_Tab[0].CR.Change_Data_Len = Instru_System_Inf_Space_Size;	
			/* 根据当前用户的级别设置参数 */
			if(User == Domestic)//普通用户
			{	
				/* 显示参数配置错误 提示信息 */
				Parameter_Setting_Failure();
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuFweUreIndex,FlashMode_AutoInit);
			}else if((User == Admin)||(User == Super_Admin))
			{
				/* 管理员帐号或超级管理员帐号 */
					/* 参数设置成 */
					Parameter_Setting_Successful();
					/* 退回到上一级界面 */
					POP();
				
			}else
				{
				    /* 将函数指针指向初始化函数,更新显示 */
					Jump2Menu(MenuFweUreIndex,FlashMode_NoAction);
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
				/* 将函数指针指向初始化函数




				*/
				Jump2Menu(MenuFweUreIndex,FlashMode_AutoInit);
			}	else if((User == Admin)||(User == Super_Admin))
			{
				/* 管理员帐号或超级管理员帐号 */
					/* 参数设置成 */
					Parameter_Setting_Successful();
					/* 退回到上一级界面 */
					POP();
			}else
			 {
			
			    /* 将函数指针指向初始化函数,更新显示 */
				Jump2Menu(MenuFweUreIndex,FlashMode_NoAction);
			
			 }
			
			
			
	        break;
        }

        default:
		{
            /* 将函数指针指向初始化函数,更新显示 */
			Jump2Menu(MenuFweUreIndex,FlashMode_NoAction);
//			return;
		}
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuFweUreeDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出固件升级界面																	*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuFweUreDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuFweUreLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择升级功能																		*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuFweUreLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuFweUreIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuFweUreRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择放弃升级功能																	*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuFweUreRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuFweUreIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuFweUreOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuFweUreOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuFweUreIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuFweUreCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuFweUreCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuFweUreIndex,FlashMode_NoAction);
}  

#endif

/******************* (C) ZHOU *****END OF FILE****/


