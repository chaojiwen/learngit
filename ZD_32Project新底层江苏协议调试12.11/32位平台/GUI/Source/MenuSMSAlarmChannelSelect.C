/************************************************************************************************/
/** @file              : MenuSMSAlarmChannelSelectIndex.c								*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年09月29日															*/
/** @date changed      : 2015年09月29日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->短信报警通道选择界面											*/
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

#include <string.h>
#include <stdio.h>


/* 通道短信报警 项目 字符串内容 */
static u8 Menu_SMS_Aam_Cha_Sec_Item_Str[Sensor_CH_Config_Num][17];


/* 通道短信报警 中文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_Cha_Sec_CN[]=
{
	{"全部通道设置"},

#if	(Sensor_CH1_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_1],
#endif
	
#if	(Sensor_CH2_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_2],
#endif
	
#if	(Sensor_CH3_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_3],
#endif
	
#if	(Sensor_CH4_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_4],
#endif
	
#if	(Sensor_CH5_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_5],
#endif
	
#if	(Sensor_CH6_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_6],
#endif
	
#if	(Sensor_CH7_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_7],
#endif
	
#if	(Sensor_CH8_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_8],
#endif
	
#if	(Sensor_CH9_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_9],
#endif
	
#if	(Sensor_CH10_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_10],
#endif
	
#if	(Sensor_CH11_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_11],
#endif
	
#if	(Sensor_CH12_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_12],
#endif
	
#if	(Sensor_CH13_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_13],
#endif
	
#if	(Sensor_CH14_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_14],
#endif
	
#if	(Sensor_CH15_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_15],
#endif
	
#if	(Sensor_CH16_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_16],
#endif
	
#if	(Sensor_CH17_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_17],
#endif
	
#if	(Sensor_CH18_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_18],
#endif
	
#if	(Sensor_CH19_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_19],
#endif
	
#if	(Sensor_CH20_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_20],
#endif
	
#if	(Sensor_CH21_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_21],
#endif
	
#if	(Sensor_CH22_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_22],
#endif
	
#if	(Sensor_CH23_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_23],
#endif
	
#if	(Sensor_CH24_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_24],
#endif
	
#if	(Sensor_CH25_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_25],
#endif
	
#if	(Sensor_CH26_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_26],
#endif
	
#if	(Sensor_CH27_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_27],
#endif
	
#if	(Sensor_CH28_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_28],
#endif
	
#if	(Sensor_CH29_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_29],
#endif
	
#if	(Sensor_CH30_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_30],
#endif
	
#if	(Sensor_CH31_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_31],
#endif
	
#if	(Sensor_CH32_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_32],
#endif

};

/* 通道短信报警 英文 菜单项目栏 */
static u8 * const Menu_SMS_Aam_Cha_Sec_EN[]=
{
	{"All Config"},
#if	(Sensor_CH1_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_1],
#endif
			
#if	(Sensor_CH2_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_2],
#endif
			
#if	(Sensor_CH3_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_3],
#endif
			
#if	(Sensor_CH4_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_4],
#endif
			
#if	(Sensor_CH5_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_5],
#endif
			
#if	(Sensor_CH6_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_6],
#endif
			
#if	(Sensor_CH7_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_7],
#endif
			
#if	(Sensor_CH8_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_8],
#endif
			
#if	(Sensor_CH9_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_9],
#endif
			
#if	(Sensor_CH10_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_10],
#endif
			
#if	(Sensor_CH11_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_11],
#endif
			
#if	(Sensor_CH12_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_12],
#endif
			
#if	(Sensor_CH13_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_13],
#endif
			
#if	(Sensor_CH14_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_14],
#endif
			
#if	(Sensor_CH15_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_15],
#endif
			
#if	(Sensor_CH16_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_16],
#endif
			
#if	(Sensor_CH17_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_17],
#endif
			
#if	(Sensor_CH18_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_18],
#endif
			
#if	(Sensor_CH19_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_19],
#endif
			
#if	(Sensor_CH20_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_20],
#endif
			
#if	(Sensor_CH21_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_21],
#endif
			
#if	(Sensor_CH22_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_22],
#endif
			
#if	(Sensor_CH23_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_23],
#endif
			
#if	(Sensor_CH24_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_24],
#endif
			
#if	(Sensor_CH25_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_25],
#endif
			
#if	(Sensor_CH26_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_26],
#endif
			
#if	(Sensor_CH27_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_27],
#endif
			
#if	(Sensor_CH28_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_28],
#endif
			
#if	(Sensor_CH29_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_29],
#endif
			
#if	(Sensor_CH30_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_30],
#endif
			
#if	(Sensor_CH31_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_31],
#endif
			
#if	(Sensor_CH32_Config_Type != Sensor_Null)
	Menu_SMS_Aam_Cha_Sec_Item_Str[Sensors_CH_32],
#endif


};

/* 通道短信报警 中英文 菜单栏 */
const struct Menu Menu_SMS_Aam_Cha_Sec[] =
{
	MenuWhat(Menu_SMS_Aam_Cha_Sec_CN),
	MenuWhat(Menu_SMS_Aam_Cha_Sec_EN),
};

/* 通道短信报警开关状态 */
#define SMS_Aam_Cha_Sec_Switch(Cha) (((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_SMS_Alarm_Switch)


/************************************************************************************************/
/* 函数名	: Set_Item_Bar_Str																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置项目条字符串																	*/
/* 创建日期	: 2015/09/29																			*/
/************************************************************************************************/
static void Set_Item_Bar_Str(u8 *str,char *Item,u8 ch_no,char * aam_state)
{
	/* 设置 项目字符串. */
	sprintf((char*)str,"%s-%d(%s)",Item,ch_no,aam_state);
}


/************************************************************************************************/
/* 函数名	: DispMenuSMSAamChaSecInit													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 短信报警通道选择														*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamChaSecInit(void)
{
	u8 num;

	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();

		/* 初始化 "菜单栏" 项目 */
		for(num = 0;num < Sensor_CH_Config_Num;num++)
		{
			/* 判断当前的语言 */
			if(*Language == CN)
			{
				Set_Item_Bar_Str(Menu_SMS_Aam_Cha_Sec_Item_Str[num],"通道",num+1,
								(SMS_Aam_Cha_Sec_Switch(num) == TRUE) ? Menu_ON_CN : Menu_OFF_CN);
			}else
			{
				Set_Item_Bar_Str(Menu_SMS_Aam_Cha_Sec_Item_Str[num],"CH",num+1,
								(SMS_Aam_Cha_Sec_Switch(num) == TRUE) ? Menu_ON_EN : Menu_OFF_EN);	
			}
			

		}
		
		/* 切换Bar显示内容 */
		BarMenu = (struct Menu *)&Menu_SMS_Aam_Cha_Sec[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		Item = 0;
        Where = 0;
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_SMS_Aam[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamChaSecUp														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 选择 短信报警通道														*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamChaSecUp(void)
{

	/* 判断当前选择项目是否在可设置的通道号内 */
	if(Item <= Sensor_CH_Config_Num)
	{
		/* 当前通道 可以 设置传感器报警功能. */
		/* 进入 短信报警设置 界面. */
		Jump2Menu(MenuSMSAamSetIndex,FlashMode_AutoInit);
		/* 进入到下一级界面 */
		PUSH();
	}else
	{
		/* 当前通道 不可以 设置传感器报警功能. */
		/* 未定义界面,设置界面. */
		/* 将函数指针指向初始化函数. */
		Jump2Menu(MenuSMSAamChaSecIndex,FlashMode_NoAction);
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamChaSecDown													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出  短信报警通道选择 界面															*/
/* 创建日期	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuSMSAamChaSecDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamChaSecLeft													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择 短信报警通道 功能														*/
/* 创建日期	: 2015/09/29																			*/
/************************************************************************************************/
void DispMenuSMSAamChaSecLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamChaSecIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamChaSecRight														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择  短信报警通道 功能																		*/
/* 创建日期	: 2015/09/29																			*/
/************************************************************************************************/
void DispMenuSMSAamChaSecRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamChaSecIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamChaSecOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/29																			*/
/************************************************************************************************/
void DispMenuSMSAamChaSecOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamChaSecIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSMSAamChaSecCancel															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/29																			*/
/************************************************************************************************/
void DispMenuSMSAamChaSecCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSMSAamChaSecIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/





