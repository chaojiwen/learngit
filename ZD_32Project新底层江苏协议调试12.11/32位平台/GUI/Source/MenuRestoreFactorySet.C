/************************************************************************************************/
/** @file              : MenuRestoreFactorySet.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月12日															*/
/** @date changed      : 2015年10月14日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->恢复出厂设置													*/
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

#include "Log_GSM.h"
#include "Dev_RTC.h"
#include "BSP.h"


/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>恢复出厂设置相关的宏设置
//	<e>恢复出厂设置项目选择
//	<i>根据实际需要设置<恢复出厂设置>项目设置
#define Rte_Fty_Set_Enable					(0x01)

//	<e>配置表“系统信息”设定
#define Rte_Fty_Set_System_Config_Enable	(0x00)
//  </e>

//	<e>配置表“电源信息”设定
#define Rte_Fty_Set_Power_Config_Enable		(0x01)
//  </e>

//	<e>配置表“时钟信息”设定
#define Rte_Fty_Set_RTC_Config_Enable		(0x00)
//  </e>

//	<e>配置表“GSM信息”设定
#define Rte_Fty_Set_GSM_Config_Enable		(0x01)
//  </e>

//	<e>配置表“传感器信息”设定
#define Rte_Fty_Set_Sensor_Config_Enable	(0x01)
//  </e>

//	<e>配置表“菜单信息”设定
#define Rte_Fty_Set_Menu_Config_Enable		(0x01)
//  </e>

//	<e>配置表“校准表信息”设定
#define Rte_Fty_Set_Data_Tab_Config_Enable	(0x00)
//  </e>



//	<e>固定配置表“系统信息”设定
#define Rte_Fty_Set_System_Fix_Enable		(0x00)
//  </e>

//	<e>固定配置表“电源信息”设定
#define Rte_Fty_Set_Power_Fix_Enable		(0x00)
//  </e>

//	<e>固定配置表“传感器信息”设定
#define Rte_Fty_Set_Sensor_Fix_Enable		(0x01)
//  </e>

//	<e>固定配置表“FRAM信息”设定
#define Rte_Fty_Set_Fram_Fix_Enable			(0x01)
//  </e>

//	<e>固定配置表“Flash信息”设定
#define Rte_Fty_Set_Flash_Fix_Enable		(0x01)
//  </e>

//	<e>固定配置表“串口1信息”设定
#define Rte_Fty_Set_Com1_Fix_Enable			(0x00)
//  </e>

//	<e>固定配置表“打印信息”设定
#define Rte_Fty_Set_Print_Fix_Enable		(0x01)
//  </e>

//	<e>固定配置表“盛亿网关”设定
#define Rte_Fty_Set_Gateway_Fix_Enable		(0x01)
//  </e>





//  </e>

// </h>
//	<<< end of configuration section >>>


/* 仪器恢复出厂界面 中英文 菜单栏 */
static const struct Menu Menu_Rte_Fty_Set[] =
{
	MenuWhat(Menu_Yes_NO_CN),
	MenuWhat(Menu_Yes_NO_EN),
};


/************************************************************************************************/
/* 函数名	: Instru_Rte_Fty_Set																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 回复出厂设置功能函数																*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
 void Instru_Rte_Fty_Set(void)
{

	/* 喂狗:由于设置参数的时间太长，并且占用CPU，因此先喂狗，保证有1.5S的时间操作。 */
	if(GPIO_ReadOutputDataBit(MAX823_WDI_PORT,MAX823_WDI_PIN))
	{
		/* 喂狗 */
		MAX823_WDI_Low;
	}else
	{
		/* 喂狗 */
		MAX823_WDI_High;
	}
	
	//配置 Instru_Config_Inf_Tab 全过程大概需要1S。
	/* 恢复配置表信息。这里选择着个配置的原因是配置表的一些参数在后期不能修改（会有改动），因此着个配置会有更加灵活的选择性。  */
	
#if (Rte_Fty_Set_System_Config_Enable == 0x01)
	/* 恢复 系统信息 配置表 */
	Instru_Config_Inf_Tab.Inf_State.Instru_System = Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_System;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf_Space,sizeof(INSTRU_SYSTEM));
#endif
#if (Rte_Fty_Set_Power_Config_Enable == 0x01)
	/* 恢复 电源信息 配置表 */
	Instru_Config_Inf_Tab.Inf_State.Instru_Power = Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Power;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State_Space,sizeof(INSTRU_POWER));
#endif
#if (Rte_Fty_Set_RTC_Config_Enable == 0x01)
	/* 恢复 时钟信息 配置表 */
	Instru_Config_Inf_Tab.Inf_State.Instru_RTC = Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_RTC;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State_Space,sizeof(INSTRU_RTC));
#endif
#if (Rte_Fty_Set_GSM_Config_Enable == 0x01)
	/* 恢复 GSM信息 配置表 */
	Instru_Config_Inf_Tab.Inf_State.Instru_GSM = Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_GSM;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State_Space,sizeof(INSTRU_GSM));
#endif
#if (Rte_Fty_Set_Sensor_Config_Enable == 0x01)
	/* 恢复 传感器信息 配置表 */
	Instru_Config_Inf_Tab.Inf_State.Instru_Sensor = Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space,sizeof(INSTRU_SENSOR));	
#endif
#if (Rte_Fty_Set_Menu_Config_Enable == 0x01)
	/* 恢复 菜单信息 配置表 */
	Instru_Config_Inf_Tab.Inf_State.Instru_Menu = Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Menu;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State_Space,sizeof(INSTRU_MENU));	
#endif
#if (Rte_Fty_Set_Data_Tab_Config_Enable == 0x01)
	/* 恢复 校准表信息 配置表 */
	Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab = Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Data_Tab;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State_Space,sizeof(INSTRU_DATA_TAB));	
#endif

	/* 恢复 配置表更新标志 */
	Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag = Default_Instru_Config_Inf_State_Tab.Inf_State.Data_Update_Flag;	
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag,
				   sizeof(Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag));


		   
	/* 喂狗:由于设置参数的时间太长，并且占用CPU，因此先喂狗，保证有1.5S的时间操作。 */
	if(GPIO_ReadOutputDataBit(MAX823_WDI_PORT,MAX823_WDI_PIN))
	{
		/* 喂狗 */
		MAX823_WDI_Low;
	}else
	{
		/* 喂狗 */
		MAX823_WDI_High;
	}


#if (Rte_Fty_Set_System_Fix_Enable == 0x01)
	//配置 Instru_Fix_Inf_State_Tab 全过程大概需要0.5S。
	/* 恢复 系统信息 固定配置表 */
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_System = Default_Instru_Fix_Inf_State_Tab.Inf_State.Instru_System;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_System.Inf_Space,sizeof(INSTRU_SYSTEM_FIX));
#endif
#if (Rte_Fty_Set_Power_Fix_Enable == 0x01)
	/* 恢复 电源信息 固定配置表 */
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power = Default_Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power.Inf_Space,sizeof(INSTRU_POWER_FIX));
#endif

#if (Rte_Fty_Set_Sensor_Fix_Enable == 0x01)
	/* 恢复 传感器信息 固定配置表 */
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor = Default_Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space,sizeof(INSTRU_SENSOR_FIX));
#endif

#if (Rte_Fty_Set_Fram_Fix_Enable == 0x01)
	/* 恢复 FRAM信息 固定配置表 */
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram = Default_Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram.Inf_State_Space,sizeof(INSTRU_FRAM_FIX));
#endif
#if (Rte_Fty_Set_Flash_Fix_Enable == 0x01)
	/* 恢复 Flash信息 固定配置表 */
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash = Default_Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State_Space,sizeof(INSTRU_FLASH_FIX));
#endif
#if (Rte_Fty_Set_Com1_Fix_Enable == 0x01)
	/* 恢复 串口1信息 固定配置表 */
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1 = Default_Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1;	
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State_Space,sizeof(INSTRU_COM1_FIX));
#endif
#if (Rte_Fty_Set_Print_Fix_Enable == 0x01)
	/* 恢复 串口1信息 固定配置表 */
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print = Default_Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print;	
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State_Space,sizeof(INSTRU_PRINT_FIX));
#endif
#if (Rte_Fty_Set_Gateway_Fix_Enable == 0x01)
	/* 恢复盛亿网关固定配置表 */
  Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway=Default_Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway.Inf_State_Space,sizeof(INSTRU_Gateway_FIX));
#endif
	/* 恢复 配置表更新标志 */
	Instru_Fix_Inf_State_Tab.Inf_State.Data_Update_Flag = Default_Instru_Fix_Inf_State_Tab.Inf_State.Data_Update_Flag;
	/* 修改配置表 */
	Write_Config_Tab((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Data_Update_Flag,
				   sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Data_Update_Flag));

	/* 重新配置系统参数 */
	System_Serial_Init();
	
	/* 复位温湿度数据寄存器 */
	GPRS_Reset_Sensor_Data_Reg();
	
}

/************************************************************************************************/
/* 函数名	: DispMenuRteFtySetInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示恢复出厂设置界面信息															*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuRteFtySetInit(void)
{
    
	
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Rte_Fty_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;	
		/* 设置设置项的位置，设置默认状态为不选择回复出厂设置 */
        Item = 1;
        Where = Item;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Stm_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));	

    }

}

/************************************************************************************************/
/* 函数名	: DispMenuRteFtySetUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置恢复出厂设置																	*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuRteFtySetUp(void)
{


	switch(Item)
	{
        case 0:
		{
			/* 根据当前用户的级别设置参数 */
			if(User == Domestic)//普通用户
			{	
				/* 显示参数配置错误 提示信息 */
				Parameter_Setting_Failure();
				/* 将函数指针指向初始化函数 */
				Jump2Menu(MenuRteFtySetIndex,FlashMode_ManualInit);
			}else if(User == Admin)
			{
				/* 显示参数配置错误 提示信息 */
				Parameter_Setting_Failure();
				/* 将函数指针指向初始化函数,更新显示 */
				Jump2Menu(MenuRteFtySetIndex,FlashMode_NoAction);
			}else if(User == Super_Admin)
			{
				/* 恢复出厂设置 */
				Instru_Rte_Fty_Set();
				/* 参数设置成 */
				Parameter_Setting_Successful();
				/* 退回到上一级界面 */
				POP();
			}
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
            /* 将函数指针指向初始化函数,更新显示 */
			Jump2Menu(MenuRteFtySetIndex,FlashMode_NoAction);
			break;
        }
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuRteFtySetDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出恢复出厂设置界面																*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuRteFtySetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuRteFtySetLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择升级功能																		*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuRteFtySetLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuRteFtySetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuRteFtySetRight															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择放弃升级功能																	*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuRteFtySetRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuRteFtySetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuRteFtySetOK																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuRteFtySetOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuRteFtySetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuRteFtySetCancel															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2015/06/13																		*/
/* 修改日期	: 2015/10/14																		*/
/************************************************************************************************/
void DispMenuRteFtySetCancel(void) 
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuRteFtySetIndex,FlashMode_NoAction);
}

/******************* (C) ZHOU *****END OF FILE****/


