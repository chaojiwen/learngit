/************************************************************************************************/
/** @file              : Menu.c																	*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月8日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单 公共代码															*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "Menu.h"
#include "Ks0108.h"
#include "PIC.c"

#include "stm32f10x.h"




/******************************************菜单相关变量**************************************************/

u8 Flash = 0;               	//跳转进入子菜单 刷新标志  >1刷新

u16 KeyFuncIndex = 0;        	//索引
//3个备份数组
u16 FatherIndex[MenuSize+1]; 	//父菜单备份数组
u8 Layer = 0;              		//菜单层数

u8 ItemBackup[MenuSize];      	//当前菜单项备份
u8 ItemBackup_i = 0;          	//

u8 WhereBackup[MenuSize];     	//反显位置备份
u8 WhereBackup_i = 0;         	//

u8 Item;            //当前Bar项  范围:[0,ItemNum-1]  包括0 包括ItemNum-1  共包括ItemNum个取值
u8 ItemNum;         //Bar项数
u8 Where;           //当前反显位置,[0,DispMax-1]  反映反显的Bar在屏幕上的位置
//单屏可以显示的最大项 通常这是个固定值--因为你的液晶的大小决定了你一次可以显示的多少,你也可以在程序中修改,但记得维护它--因为所有的Bar型菜单都是要用到它的
#define DispMax  		(3u)
u8 DispMin;
#define DispdataMax  	(5u)
u8 DispdataMin;


//当前Bar显示内容指针
//u8 const **DispItem; //项显示内容指针
u8 **DispItem; //项显示内容指针



//菜单资源指针
struct Menu * BarMenu;
//Tip指针
struct Menu * TipBar;

/* 参数选择窗口的光标位置 */
u8 Input_Para_Win_Cursor = 0;

/************************************************************************************************************************/
/* |---->指针对象存放地址,也就是{"Function"},{"Parameter"},{"Measure"},等等这些文本存放在哪里									*/
/* |         |---->指针存放的位置,是这些字符的首地址应当存放在哪里  因为这是个指针数组存放的内容就是每个字符串的开始地址			*/
/* |         |																											*/
/* u8 const * const TopItemEN[]=																						*/
/* {																													*/
/* {"Function"},																										*/
/* {"Parameter"},																										*/
/* {"Measure"},																											*/
/* {"Language"},																										*/
/* {"Test"},																											*/
/* 创建日期	: 2015/6/16																									*/
/************************************************************************************************************************/

/* 温度传感器上限界面 中文 菜单栏 */
u8 * const Menu_Yes_NO_CN[]=
{
	{"是"},
	{"否"},
};

/* 温度传感器上限界面 英文 菜单栏 */
u8 * const Menu_Yes_NO_EN[]=
{
	{"Yes"},
	{"No"},
};

/* 数字输入窗口 中英文 菜单栏 */
u8 * const Menu_Digital_Input[]=
{
{"0"},
{"1"},
{"2"},
{"3"},
{"4"},
{"5"},
{"6"},
{"7"},
{"8"},
{"9"},
{"*"},
};

/* 温度传感器上限界面 中英文 菜单栏 */
static u8 * const Menu_Sensor_Lit_Set_CN[]=
{
{"0"},
{"1"},
{"2"},
{"3"},
{"4"},
{"5"},
{"6"},
{"7"},
{"8"},
{"9"},
{"+"},
{"-"},
{"*"},
};


/* 温度传感器上限界面 中英文 菜单栏 */
const struct Menu Menu_Sen_Lit_Set[] =
{
	MenuWhat(Menu_Sensor_Lit_Set_CN),
	MenuWhat(Menu_Sensor_Lit_Set_CN),
};


/* 图标索引 */
u8 const * const Ico_Index[]=
{
	Function,   //功能
	Para,       //参数
	Warning,    //询问	
	Action,     //测量
#if (Print_Function_Enable == 0x01)	
	Print,		//打印
#endif		
#if ((Gateway_NJSY_Enable == 0x01)||(Gateway_HZZH_Enable == 0x01))	
	Ico_Gateway,//网关
#endif
#if (GSM_Module_Type == 0x02)	
	Ico_Wifi,//Wifi
#endif	
		
//	Question,   //时钟
//	Time,
//	Sound,
};

/* 信号强度图标索引 */
u8 const * const Mobile_Signal_Index[]=
{
	Mobile_Signal_Null,
	Mobile_Signal_One,
	Mobile_Signal_Two,
	Mobile_Signal_There,
	Mobile_Signal_Four,
};

/* Sim卡无网络信号 */
u8 const * const Mobile_SIM_NoSignal 	= Pic_SIM_NoSignal;

/* 未检测到Sim卡 */
u8 const * const Mobile_SIM_NotDetected = Pic_SIM_NotDetected;

/* SIM欠费停机,也有可能是空号 */
u8 const * const Mobile_SIM_ShutDown 	= Pic_SIM_ShutDown;

/* 通道警报图标索引 */
u8 const * const  Alarm_Index[]=
{
 Ico_Alarm_Null,
 Ico_Alarm_Full,  
};


#if 0
//短信图标索引
u8 const * const  Messages_Index[]=
{
 Messages_Null,
 Messages_Full,  
};

//蓝牙图标索引
u8 const * const  Bluetooth_Index[]=
{
 Bluetooth_Null,
 Bluetooth_Full,  
};

//3G信号图标索引
u8 const * const  Generation_3rd_Index[]=
{
 Generation_3rd_Null,
 Generation_3rd_Full,  
};

//数据通信图标索引
u8 const * const  DataComm_Index[]=
{
 DataComm_Null,
 DataComm_Full,  
};
#endif

/* 电池电量图标索引 */
u8 const * const Battery_Power_Index[]=
{
 Battery_Power_Null,
 Battery_Power_One,
 Battery_Power_Two,
 Battery_Power_There,
 Battery_Power_Four,
 Battery_Power_Full,
 Battery_Power_Miss,
};

/* 传感器类型图标 */
u8 const * const Sensor_Type_Index[]=
{
	Null_Sensor_ICO,
	Temp_Sensor_ICO,
	Huni_Sensor_ICO,
	RF_Card_Sensor_ICO,
	RF_Card_Sensor_ICO,
	RF_Card_Sensor_ICO,
	Lon_Lat_Sensor_ICO,
	Lon_Lat_Sensor_ICO,
	Speed_Sensor_ICO,
	Temp_Sensor_ICO,
	ICO_GWNJSY_Tmp,
	ICO_GWNJSY_Hum,
	ICO_GWHZZH_Tmp,
	ICO_GWHZZH_Hum,
	CarbonDioxide_Sensor_ICO,
	DoorContact_Sensor_ICO,
};

/* 用户类型图标 */
u8 const * const User_Type_Index[]=
{
	Domestic_ICO,
	Admin_ICO,
	Super_Admin_ICO,  
};

/* 传感器通道 */
u8 const * const Sensor_CH_Index[]=
{
	Date_Time_ICO,
//	Sensor_CH0_ICO,
	Sensor_CH1_ICO,
	Sensor_CH2_ICO,
	Sensor_CH3_ICO,
	Sensor_CH4_ICO,
	Sensor_CH5_ICO,
	Sensor_CH6_ICO,
	Sensor_CH7_ICO,
	Sensor_CH8_ICO,
	Sensor_CH9_ICO,
	Sensor_CH10_ICO,
	/* 现在硬件只可以配置8个板载传感器，假如后期硬件更改，可以配置32路传感器是，这个图标也要添加。 */
};


/* 数据存储开关 图标 */
u8 const * const Menu_Save_Data_Index[] =
{
	Menu_Disable_Save_Data_ICO,
	Menu_Enable_Save_Data_ICO,	
};
/* 卸货开始 图标 */
u8 const * const Menu_StartUpload_Index[] =
{
	Menu_StartUpload_ICO,
	Menu_StartUploadNull_ICO,	
};


/************************************************************************************************/
/* 函数名	: InitMenuVal																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 初始化菜单系统																		*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/ 
void InitMenuVal(void)
{
    /* 设置反显位置 */
	WhereBackup_i = 0;
	/* 设置菜单项 */
    ItemBackup_i = 0;
	/* 设置单前层 */
    Layer = 0;
	/* 设置父菜单项 */
    FatherIndex[Layer++] = MenuDeskTopIndex;
}

/************************************************************************************************/
/* 函数名	: PUSH																				*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 保存当前界面的界面参数（保存界面，为后面返回界面）										*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
void PUSH(void)
{
    WhereBackup[WhereBackup_i++] = Where;	//备份Where--->高亮显示的位置
    ItemBackup[ItemBackup_i++] = Item;		//备份Item--->当前项的位置
    FatherIndex[Layer++] = KeyFuncIndex;	//备份KeyFuncIndex--->状态机的状态
}

/************************************************************************************************/
/* 函数名	: POP																				*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一个菜单界面参数（返回上一级界面）												*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
void POP(void)
{
    Layer--;      //先弹出一层,因为使用的是++,因此当前Layer指向下一个数组位置
    KeyFuncIndex = FatherIndex[Layer-1];     //恢复KeyFuncIndex
    Where = WhereBackup[--WhereBackup_i];    //恢复高亮显示位置Where
    Item = ItemBackup[--ItemBackup_i];       //恢复项位置Item
//-------------------------------

//-------------------------------
    Flash = FlashMode_ReFlash;               //子项返回,请求刷新
}

/* 参数输入窗口的显示位置 */
#define InputParaWinXOffset   		(24u)//X偏置
#define InputParaWinYOffset   		(48u)//Y偏置

/************************************************************************************************/
/* 函数名	: MenuInputPara																 	 	*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 输入指定的参数 									 									*/
/* 创建日期 	: 2015/6/3																			*/
/************************************************************************************************/
void MenuInputPara(void)
{
u8 data_menu_i;          //计数变量 在菜单函数中用到
u8 data_menu_j;          //计数变量
//-------------------
//比较"显示屏可以显示的项的数目(DispMax)"跟"需要显示的数目(ItemNum)"
//找出小者,以小者为显示上限
//例如显示屏可以显示3项,需要显示的项有2项,那么显示2项,有3项,显示3项,有4项的话,显示3项
    if (ItemNum > DispdataMax) {
        DispdataMin = DispdataMax;
    } else {
        DispdataMin = ItemNum;
    }

    if (Item >= Where) {//未跨越零点
        data_menu_j = Item -Where;
    } else {            //跨越零点
        data_menu_j = ItemNum  - (Where - Item);
    }
    //此时Menu_J的值就是顶部(Where=0)显示的Item值
//----------------
    //显示全部的子菜单项
    for (data_menu_i = 0; data_menu_i < DispdataMin; data_menu_i++) {
        if (data_menu_j == Item){ //反显第Item项,位置是Where
			GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
			GUI_DispCharAt(**(DispItem+Item), Where*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
			GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
        } else {             //其它项 正常显示
			GUI_DispCharAt(**(DispItem+data_menu_j),data_menu_i*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
        }
        if (data_menu_j == (ItemNum-1)) {//到了最后一项，回环到最开始的一项
            data_menu_j = 0;
        } else {                    //下一项
            data_menu_j++;
        }
    }

	GUI_DispCharAt('<',InputParaWinXOffset-20,InputParaWinYOffset);
	GUI_DispCharAt('-',InputParaWinXOffset-12,InputParaWinYOffset);
	GUI_DispCharAt('-',InputParaWinXOffset+74,InputParaWinYOffset);
	GUI_DispCharAt('>',InputParaWinXOffset+82,InputParaWinYOffset);
}

/************************************************************************************************/
/* 函数名	: MenuInputParaWinLeft														 	 	*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 向左选择指定的参数								 									*/
/* 创建日期 	: 2015/6/3																			*/
/************************************************************************************************/
void MenuInputParaWinLeft(u16 Index)
{
    u8 Menu_i;          //计数变量 在菜单函数中用到
    u8 Menu_j;          //计数变量
    u8 ItemBack;
    ItemBack = Item;//备份原有的Item,取消原有反显时需要使用
    //维护Item,得到需要反显的Item
    if (Item == 0) {
        Item = ItemNum-1;
    } else {
        Item--;
    }

    //维护显示
    if (Where) 
	{//当前位置不为0,就是说不是在顶部,两次反显
	    //原来位置取消反显   项是ItemBack  位置Where
		GUI_DispCharAt(**(DispItem+ItemBack), (Where--)*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
		GUI_DispCharAt(**(DispItem+Item), Where*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}else if (ItemNum > DispdataMax)
	{	//需要显示的Bar多于单屏可以显示的Bar,那么重新显示全部
        Menu_j = Item;
        Menu_i = 0;
        //项是 Item 位置 0
		GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
		GUI_DispCharAt(**(DispItem+Menu_j), Menu_i*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);

//        BarDisp(*(DispItem+Menu_j),BarXOffset,Menu_i*BarYSize+BarYOffset,1);//反显
        if (Menu_j == (ItemNum-1)) 
		{   //维护Menu_j
            Menu_j=0;
        } else 
		{
            Menu_j++;
        }
        //显示其他项
        for (Menu_i = 1; Menu_i < DispdataMax; Menu_i++) 
		{
            GUI_DispCharAt(**(DispItem+Menu_j), Menu_i*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
            if (Menu_j == (ItemNum-1)) 
			{
                Menu_j=0;
            } else {
                Menu_j++;
            }
        }
        Where = 0;//当前位置依然在开头的0位置
	}else 
	{
        //当仅仅只有一项的时候，应当一直在Where=0的地方反显的！！！这里也能实现，因为最后是反显的函数
        Where = ItemNum-1;
        //取消顶部的原有的反显  项ItemBack  位置0
		GUI_DispCharAt(**(DispItem+ItemBack), 0*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		//反显根部  项Item  位置ItemNum-1
		GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
		GUI_DispCharAt(**(DispItem+Item), Where*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

    Jump2Menu(Index,FlashMode_NoAction);
}  

/************************************************************************************************/
/* 函数名	: MenuInputParaWinRight														 	 	*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 向右选择指定的参数								 									*/
/* 创建日期 	: 2015/6/3																			*/
/************************************************************************************************/
void MenuInputParaWinRight(u16 Index)
{
    u8 Menu_i;          //计数变量 在菜单函数中用到
    u8 Menu_j;          //计数变量
    u8 ItemBack;
    ItemBack = Item;//备份原有的Item,取消原有反显时需要使用
    if (Item == (ItemNum-1)) {
        Item = 0;
    } else {
        Item++;
    }

    //维护显示
	if (Where != (DispdataMin-1))
	{//当前位置不为0,就是说不是在顶部,两次反显
	    //原来位置取消反显   项是ItemBack  位置Where
		GUI_DispCharAt(**(DispItem+ItemBack), (Where++)*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
		GUI_DispCharAt(**(DispItem+Item), Where*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}else if (ItemNum > DispdataMax)
	{	//需要显示的Bar多于单屏可以显示的Bar,那么重新显示全部
        Menu_j = Item;
        Menu_i = DispdataMax;
        //项是 Item 位置 0
		GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
		GUI_DispCharAt(**(DispItem+Menu_j), (Menu_i-1)*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);

        if (Menu_j == 0) 
		{   //维护Menu_j
            Menu_j = ItemNum-1;
        } else 
		{
            Menu_j--;
        }
        //显示其他项
        for (Menu_i = DispdataMax-1;  Menu_i > 0; Menu_i--) 
		{
            GUI_DispCharAt(**(DispItem+Menu_j), (Menu_i-1)*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
            if (Menu_j == 0) 
			{
               Menu_j = ItemNum-1;
            } else {
               Menu_j--;
            }
        }
        Where = DispdataMax-1;//当前位置依然末尾位置
	}else 
	{
        //当仅仅只有一项的时候，应当一直在Where=0的地方反显的！！！这里也能实现，因为最后是反显的函数
        Where = 0;
        //取消顶部的原有的反显  项ItemBack  位置0
		GUI_DispCharAt(**(DispItem+ItemBack), (ItemNum-1)*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		//反显根部  项Item  位置ItemNum-1
		GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
		GUI_DispCharAt(**(DispItem+Item), Where*DataXSize+InputParaWinXOffset,InputParaWinYOffset);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

    Jump2Menu(Index,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: Parameter_Setting_Successful												 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置参数错误									 									*/
/* 创建日期 	: 2015/6/8																			*/
/************************************************************************************************/
void Parameter_Setting_Successful(void) 
{
	/* 显示菜单名字 */
	GUI_DispStringAtBar((*Language == CN)?(u8 *)"设置成功":(u8 *)"Successful",Line_4_PosX,Line_4_PosY,Line_4_XSize,GUI_TA_LEFT);
	/* 延时1秒 */
	GUI_Delay(1000);
}

/************************************************************************************************/
/* 函数名	: Parameter_Setting_Failure													 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置参数成功									 									*/
/* 创建日期 	: 2014/6/8																			*/
/************************************************************************************************/
void Parameter_Setting_Failure(void)
{
	/* 显示菜单名字 */
	GUI_DispStringAtBar((*Language == CN)?(u8 *)"参数错误":(u8 *)"Parameter error",Line_4_PosX,Line_4_PosY,Line_4_XSize,GUI_TA_LEFT);
	/* 延时1秒 */
	GUI_Delay(1000);
}

/************************************************************************************************/
/* 函数名	: Jump2Menu																	 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置菜单指针（跳转到指定菜单）									 					*/
/* 创建日期 	: 2014/6/8																			*/
/************************************************************************************************/
void Jump2Menu(u16 Index,u8 FlashMode)
{
    /* 设置函数序号指针 */
	KeyFuncIndex = Index;
	/* 设置界面更新方式 */
    if(FlashMode){
        Flash = FlashMode;
    }
}


/************************************************************************************************/
/* 函数名	: BarDisp																			*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 项目条 标签显示	（菜单显示）															*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
void BarDisp(u8 *s,u8 X,u8 Y,u8 HighLight)
{


    if (HighLight == 1) { //高亮显示
//------------------------------------
        GUI_SetTextMode(GUI_TEXTMODE_REVERSE);//更改字体颜色来突出显示
        GUI_DispStringAtBar(s,X,Y,MenuBarPosX-1,GUI_TA_LEFT);
        GUI_SetTextMode(GUI_TEXTMODE_NORMAL);        //恢复原有字体颜色和背景颜色
//------------------------------------
    } else {  //通常显示
        //显示Bar的Text
        GUI_DispStringAtBar(s,X,Y,MenuBarPosX-1,GUI_TA_LEFT);
    }

}

/************************************************************************************************/
/* 函数名	: BarMenuInit																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 当前界面 项目菜单初始化																*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
void BarMenuInit(void)
{
u8 Menu_i;          //计数变量 在菜单函数中用到
u8 Menu_j;          //计数变量
//-------------------
//比较"显示屏可以显示的项的数目(DispMax)"跟"需要显示的数目(ItemNum)"
//找出小者,以小者为显示上限
//例如显示屏可以显示3项,需要显示的项有2项,那么显示2项,有3项,显示3项,有4项的话,显示3项
    if (ItemNum > DispMax) {
        DispMin = DispMax;
    } else {
        DispMin = ItemNum;
    }

    if (Item >= Where) {//未跨越零点
        Menu_j = Item -Where;
    } else {            //跨越零点
        Menu_j = ItemNum  - (Where - Item);
    }
    //此时Menu_J的值就是顶部(Where=0)显示的Item值
//----------------
    //显示全部的子菜单项
    for (Menu_i = 0; Menu_i < DispMin; Menu_i++) {
        if (Menu_j == Item){ //反显第Item项,位置是Where
            BarDisp(*(DispItem+Item),BarXOffset,Where*BarYSize+BarYOffset,1);
        } else {             //其它项 正常显示
            BarDisp(*(DispItem+Menu_j),BarXOffset,Menu_i*BarYSize+BarYOffset,0);
        }
        if (Menu_j == (ItemNum-1)) {//到了最后一项，回环到最开始的一项
            Menu_j = 0;
        } else {                    //下一项
            Menu_j++;
        }
    }
    Bar(Item,ItemNum,MenuBarPosX,MenuBarNumPosX);                  //维护滚动条
}

/************************************************************************************************/
/* 函数名	: BarMenuLeft																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 菜单 项目 选择（选择前一个菜单项目）													*/
/* 创建日期	: 2015/6/8																			*/
/* 注释		: 可能的几种情况:	1、原有Bar的顶部,按键是向上,那么就需要刷新全部的显示;						*/
/* 			  				若是需要显示的Bar多于单屏可以显示的Bar,那么重新显示全部;					*/
/* 			  				若是需要显示的Bar不多于单屏可以显示的Bar,取消顶部高亮显示,				*/
/*							高亮显示最后一项Bar.													*/
/*							2、原有Bar不是在顶部,按键向上,那么取消原有Bar的高亮,高亮显示上一个Bar项	*/
/*							当然还需要维护:高亮显示的位置where/高亮显示Bar项的Item。					*/
/************************************************************************************************/
void BarMenuLeft(void)
{ 		
    u8 Menu_i;          //计数变量 在菜单函数中用到
    u8 Menu_j;          //计数变量
    u8 ItemBack;
    ItemBack = Item;//备份原有的Item,取消原有反显时需要使用
    //维护Item,得到需要反显的Item
    if (Item == 0) {
        Item = ItemNum-1;
    } else {
        Item--;
    }
    //维护显示
    if (Where) 
	{//当前位置不为0,就是说不是在顶部,两次反显

		//原来位置取消反显   项是ItemBack  位置Where
		BarDisp(*(DispItem+ItemBack),BarXOffset,(Where--)*BarYSize+BarYOffset,0);
		//上移后的位置反显   项是Item      位置Where-1 (Where--实现)
		BarDisp(*(DispItem+Item),BarXOffset,Where*BarYSize+BarYOffset,1);//高亮显示

    }
	else if(ItemBack != 0)
	{
        if(ItemNum > DispMax) 
		{//需要显示的Bar多于单屏可以显示的Bar,那么重新显示全部

            Menu_j = Item;
            Menu_i = 0;
            //项是 Item 位置 0
            BarDisp(*(DispItem+Menu_j),BarXOffset,Menu_i*BarYSize+BarYOffset,1);
            if (Menu_j == (ItemNum-1)) {   //维护Menu_j
                Menu_j=0;
            } else {
                Menu_j++;
            }
            //显示其他项
            for (Menu_i = 1; Menu_i < DispMax; Menu_i++) {
                BarDisp(*(DispItem+Menu_j),BarXOffset,Menu_i*BarYSize+BarYOffset,0);
                if (Menu_j == (ItemNum-1)) {
                    Menu_j=0;
                } else {
                    Menu_j++;
                }
            }
            Where = 0;//当前位置依然在开头的0位置
        } else {//单屏能显示完全部菜单项,"顶部 上移"应当跳转到底部(或者什么都不做)
            //当仅仅只有一项的时候，应当一直在Where=0的地方反显的！！！这里也能实现，因为最后是反显的函数
            Where = ItemNum-1;
            //取消顶部的原有的反显  项ItemBack  位置0
            BarDisp(*(DispItem+ItemBack),BarXOffset,0*BarYSize+BarYOffset,0);
            //反显根部  项Item  位置ItemNum-1
            BarDisp(*(DispItem+Item),BarXOffset,Where*BarYSize+BarYOffset,1);//反显
        }
    }
	else
	{
        Item = ItemBack;//Restore
    }

    Bar(Item,ItemNum,MenuBarPosX,120);                  //维护滚动条
    Jump2Menu(SelfMenuIndex,FlashMode_NoAction);		
}

/************************************************************************************************/
/* 函数名	: BarMenuRight																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 菜单 项目 选择（选择后一个菜单项目）													*/
/* 创建日期	: 2015/6/8																			*/
/* 注释		: 可能的几种情况:	1、原有Bar的顶部,按键是向上,那么就需要刷新全部的显示;						*/
/* 			  				若是需要显示的Bar多于单屏可以显示的Bar,那么重新显示全部;					*/
/* 			  				若是需要显示的Bar不多于单屏可以显示的Bar,取消顶部高亮显示,				*/
/*							高亮显示最后一项Bar.													*/
/*							2、原有Bar不是在顶部,按键向上,那么取消原有Bar的高亮,高亮显示上一个Bar项	*/
/*							当然还需要维护:高亮显示的位置where/高亮显示Bar项的Item。					*/
/************************************************************************************************/
void BarMenuRight(void)
{
  u8 Menu_i;          //计数变量 在菜单函数中用到
    u8 Menu_j;          //计数变量
    u8 ItemBack;
    ItemBack = Item;
//---维护Item
    if (Item == (ItemNum-1)) {
        Item = 0;
    } else {
        Item++;
    }
	
		
		
//------------
//---维护显示
    if (Where != (DispMin-1)) 
	{//当前位置不为最末,两次反显

        //原来位置取消反显   项是ItemBack  位置Where
        BarDisp(*(DispItem+ItemBack),BarXOffset,(Where++)*BarYSize+BarYOffset,0);
        //下移后的位置反显   项是Item      位置Where+1(Where++实现)
        BarDisp(*(DispItem+Item),BarXOffset,Where*BarYSize+BarYOffset,1);//反显

    }else if(ItemBack != (ItemNum-1))
	{//当前位置为DispMax    如果原来显示位置不是最后一项，则 底部  下移
        if(ItemNum > DispMax) 
		{//需要显示的Bar多于单屏可以显示的Bar,那么重新显示
            Menu_j = Item;
            Menu_i = DispMax;
            //项是 Item  位置是 DispMax
            BarDisp(*(DispItem+Menu_j),BarXOffset,(Menu_i-1)*BarYSize+BarYOffset,1);
            if (Menu_j == 0) {
                Menu_j = ItemNum-1;
            } else {
                Menu_j--;
            }
            //显示其他项
            for (Menu_i = DispMax-1; Menu_i > 0; Menu_i--) {
                BarDisp(*(DispItem+Menu_j),BarXOffset,(Menu_i-1)*BarYSize+BarYOffset,0);
                if (Menu_j == 0) {
                    Menu_j = ItemNum-1;
                } else {
                    Menu_j--;
                }
            }
            Where = DispMax-1;//当前位置依然末尾位置
        } else {//单屏能显示完全部菜单项,"底部 下移"应当跳转到顶部反显(或者什么都不做)
            Where = 0;
            //取消底部原有反显   项ItemBack  位置ItemNum-1
            BarDisp(*(DispItem+ItemBack),BarXOffset,(ItemNum-1)*BarYSize+BarYOffset,0);
            //反显顶部 项Item  位置0
            BarDisp(*(DispItem+Item),BarXOffset,Where*BarYSize+BarYOffset,1);//反显
        }
    }
	else
	{
        Item = ItemBack;
    }


    Bar(Item,ItemNum,MenuBarPosX,MenuBarNumPosX);
    Jump2Menu(SelfMenuIndex,FlashMode_NoAction);		
}

/******************* (C) ZHOU *****END OF FILE****/


