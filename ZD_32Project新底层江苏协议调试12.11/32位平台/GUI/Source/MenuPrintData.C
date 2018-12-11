/************************************************************************************************/
/** @file              : MenuPrintData.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年07月06日															*/
/** @date changed      : 2015年10月16日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->打印数据														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_Print.h"

#if (Print_Function_Enable == 0x01)

#include "Ks0108.H"
#include "Menu.H"
#include "app.h"

#include "Log_Alarm.h"
#include "Log_Print.h"


/* 打印传感器数据输出界面 中英文 菜单栏 */
static const struct Menu Menu_Pit_Data[] =
{
	MenuWhat(Menu_Yes_NO_CN),
	MenuWhat(Menu_Yes_NO_EN),
};

/* 打印起始时间 */
#define Menu_Pit_Sat_Time_Data_P		(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Time)
#define Menu_Pit_End_Time_Data_P		(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_End_Time)
#define Menu_Pit_Rcd_Sat_Time_Data_P	(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time)
#define Menu_Pit_Rcd_End_Time_Data_P	(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_End_Time)


/************************************************************************************************/
/* 函数名	: DispMenuPitDataInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 打印传感器数据输出界面设置信息													*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/13																		*/
/************************************************************************************************/
void DispMenuPitDataInit(void)
{
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_Pit_Data[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		/* 设置项的位置，设置默认状态为不选择打印. */
        Item = 1;
        Where = Item;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Pit_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuPitDataUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 无																				*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitDataUp(void)
{

	switch(Item)
	{
		case 0:
		{
			/* 1、设置的打印时间是否在记录的时间范围内。 */
			if(((Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Time >= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_End_Time)||
				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_End_Time <= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time))&&
				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Time >= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_End_Time)&&
				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time >= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_End_Time))
			{
				/* 在设置的时间范围内没有记录的传感器数据 */
				Parameter_Setting_Failure();
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuPitDataIndex,FlashMode_ManualInit);
				return;
			}else
			{
				/* 在设置的时间范围内有记录的传感器数据 */
				/* 发送打印信号量 */
				OSSemPost(Print_QUE_Sem);
			}

			/* 参数设置成 */
			Parameter_Setting_Successful();
			/* 退回到上一级界面 */
			POP();
			break;
		}
		case 1:
		{
			/* 退回到上一级界面 */
			POP();
			break;
		}
       
		default:
		{
			/* 将函数指针指向对应界面的初始化函数 */
			Jump2Menu(MenuPitDataIndex,FlashMode_NoAction);
			break;
		}
    }
}

/************************************************************************************************/
/* 函数名	: DispMenuPitDataDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出打印传感器数据面																*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitDataDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuPitDataLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择打印传感器数据																*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitDataLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuPitDataIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitDataRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择关闭声光报警功能																*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitDataRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuPitDataIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuPitDataOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitDataOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuPitDataIndex,FlashMode_NoAction);
	
}

/************************************************************************************************/
/* 函数名	: DispMenuPitDataOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/07/06																		*/
/* 修改日期	: 2015/10/16																		*/
/************************************************************************************************/
void DispMenuPitDataCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuPitDataIndex,FlashMode_NoAction);
}  

#endif
/******************* (C) ZHOU *****END OF FILE****/

