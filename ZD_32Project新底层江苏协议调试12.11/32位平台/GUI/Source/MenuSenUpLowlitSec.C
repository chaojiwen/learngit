/************************************************************************************************/
/** @file              : MenuSenUpLowlitSec.c													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年9月2日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->传感器上下限选择界面。											*/
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


/*  传感器界面 中文语言 上限项目 字符串内容 */
static u8 Menu_Sensor_Temp_Up_lit_Str_CN[17];
/*  传感器界面 中文语言 下限项目 字符串内容 */
static u8 Menu_Sensor_Temp_Low_lit_Str_CN[17];

/*  传感器界面 英文语言 上限项目 字符串内容 */
static u8 Menu_Sensor_Temp_Up_lit_Str_EN[17];
/*  传感器界面 英文语言 下限项目 字符串内容 */
static u8 Menu_Sensor_Temp_Low_lit_Str_EN[17];


/* 传感器界面 中文 菜单项目栏 */
static u8 * const Menu_Sen_Up_Low_lit_Item_Str_CN[]=
{
	{Menu_Sensor_Temp_Up_lit_Str_CN},
	{Menu_Sensor_Temp_Low_lit_Str_CN},
};

/* 传感器界面 中文 菜单项目栏 */
static u8 * const Menu_Sen_Up_Low_lit_Item_Str_EN[]=
{
	{Menu_Sensor_Temp_Up_lit_Str_EN},
	{Menu_Sensor_Temp_Low_lit_Str_EN},
};

/* 传感器界面 中英文 菜单栏 */
const struct Menu Menu_Sen_Up_Low_lit_Item[] =
{
	MenuWhat(Menu_Sen_Up_Low_lit_Item_Str_CN),
	MenuWhat(Menu_Sen_Up_Low_lit_Item_Str_EN),
};

/* 通道指针 */
static INSTRU_SENSOR *CH_Data_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Sensor;
/* 通道号 */
static u8 Sen_Num;


/************************************************************************************************/
/* 函数名	: Set_Item_Bar_Str																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置项目条字符串																	*/
/* 创建日期	: 2015/9/2																			*/
/************************************************************************************************/
static void Set_Item_Bar_Str(u8 *str,char *Item,float data)
{
	/* 由于 在sprintf 使用%f存在数据对齐的问题，因此另外设置连个变量来代替这个值. */
	u8 Tmp_dec;
	u16 Tmp_int;
	
	
	/* 获取整数 */
	Tmp_int = data;
	/* 获取小数 */
	Tmp_dec = ((u16)(data*10))%10;
	/* 设置中英文语言下上下限显示项字符串。 */
	sprintf((char*)str,
			"%s(%c%d.%01d)",
			Item,
			((data >= 0.0) ? '+':'-'),
			Tmp_int,Tmp_dec);
	
}

/************************************************************************************************/
/* 函数名	: DispMenuSenUpLowlitSecInit														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示温度类型传感器界面																*/
/* 创建日期	: 2015/6/9																			*/
/************************************************************************************************/
void DispMenuSenUpLowlitSecInit(void)
{

	
	
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{	
		/* 清除屏幕显示 */
		GUI_Clear();
		
		/* 获取通道编号。 */
		Sen_Num = ItemBackup[ItemBackup_i-1];
		
		if(*Language == CN)
		{
			
			/* 设置中文上限项目条 */
			Set_Item_Bar_Str(Menu_Sensor_Temp_Up_lit_Str_CN,"上限",((INSTRU_SENSOR_CHANNEL*)(&CH_Data_P->Inf_State_Space[Sen_Num*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Up_Limit);
			/* 设置中文下限项目条 */
			Set_Item_Bar_Str(Menu_Sensor_Temp_Low_lit_Str_CN,"下限",((INSTRU_SENSOR_CHANNEL*)(&CH_Data_P->Inf_State_Space[Sen_Num*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Low_Limit);
			
		}else
		{
			/* 设置英文上限项目条 */
			Set_Item_Bar_Str(Menu_Sensor_Temp_Up_lit_Str_EN,"Up",((INSTRU_SENSOR_CHANNEL*)(&CH_Data_P->Inf_State_Space[Sen_Num*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Up_Limit);
			/* 设置英文下限项目条 */
			Set_Item_Bar_Str(Menu_Sensor_Temp_Low_lit_Str_EN,"Low",((INSTRU_SENSOR_CHANNEL*)(&CH_Data_P->Inf_State_Space[Sen_Num*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Low_Limit);
		}

		
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Sen_Up_Low_lit_Item[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		if(Flash != FlashMode_ReFlash)
		{
			/* 设置项目光标 */
			Item = 0;
			Where = 0;
		}
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Cha_Tsd[*Language].Text+ItemBackup[ItemBackup_i-1]));
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSenUpLowlitSecUp															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入选择温度传感器限制界面															*/
/* 创建日期	: 2015/6/9																			*/
/************************************************************************************************/
void DispMenuSenUpLowlitSecUp(void)
{ 	

	
	switch(Item)
	{
		case 0:
		{
			/* 进入 温度传感器上限 界面 */
			Jump2Menu(MenuSenUpLitSetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		case 1:
		{
			/* 进入 温度传感器下限 界面 */
			Jump2Menu(MenuSenLowLitSetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();			
            break;
        }
		
        default:
		{
			/* 未知数据类型，不进入界面。 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_NoAction);
			break;
		}
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuSenUpLowlitSecDown														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出温度传感器界面																	*/
/* 创建日期	: 2015/6/9																			*/
/************************************************************************************************/
void DispMenuSenUpLowlitSecDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSenUpLowlitSecLeft														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择温度传感器界面项目															*/
/* 创建日期	: 2015/6/9																			*/
/************************************************************************************************/
void DispMenuSenUpLowlitSecLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSenUpLowlitSecRight														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择温度传感器界面项目															*/
/* 创建日期	: 2015/6/9																			*/
/************************************************************************************************/
void DispMenuSenUpLowlitSecRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSenUpLowlitSecOK															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）															*/
/* 创建日期	: 2015/6/9																			*/
/************************************************************************************************/
void DispMenuSenUpLowlitSecOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuSenUpLowlitSecCancel														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）															*/
/* 创建日期	: 2015/6/9																			*/
/************************************************************************************************/
void DispMenuSenUpLowlitSecCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/



