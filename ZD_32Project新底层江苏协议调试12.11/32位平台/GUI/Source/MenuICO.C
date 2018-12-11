/************************************************************************************************/
/** @file              : MenuICO.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月4日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->设置界面														*/
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

#include "Log_Print.h"

#include "Gsm_Api.h"

/********************************* 宏定义 **********************************/
#define ICO_PosX     	(48u)
#define ICO_PosY     	(16u)
#define ICO_XSize    	(32u)
#define ICO_YSize    	(32u)

#define User_BMP_PosX   (0u)
#define User_BMP_PosY   (0u)
#define User_BMP_XSize  (21u)
#define User_BMP_YSize  (16u)

/* 菜单界面 中文 菜单项目栏 */
static  u8 * const Menu_Ico_CN[]=
{
	{"报警设置"},
	{"运行参数"},
	{"网络参数"},
	{"系统设置"},
#if (Print_Function_Enable == 0x01)
	{"打印设置"},
#endif
#if ((Gateway_NJSY_Enable == 0x01)||(Gateway_HZZH_Enable == 0x01))
	{"网关设置"},
#endif
#if (GSM_Module_Type == 0x02)		
	{"WIFI参数"},
#endif	
	
};

/* 菜单界面 英文 菜单项目栏 */
static  u8 * const Menu_Ico_EN[]=
{
	{"Alarm Set"},
	{"Run Para"},
	{"Network Para"},		
	{"System Set"},
#if (Print_Function_Enable == 0x01)	
	{"Print Set"},
#endif	
#if ((Gateway_NJSY_Enable == 0x01)||(Gateway_HZZH_Enable == 0x01))
	{"Gateway Set"},
#endif
#if (GSM_Module_Type == 0x02)
	{"Wifi Para"},
#endif	
};

/* 菜单界面 中英文 菜单栏 */
const struct Menu Menu_Ico[] =
{
	MenuWhat(Menu_Ico_CN),
	MenuWhat(Menu_Ico_EN),
};


/************************************************************************************************/
/* 函数名	: MenuICOLeft																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 菜单界面向上选择项目																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/ 
static void MenuICOLeft(void)
{
	/* 设置项目标号 */
	if(Item==0)
	{
        Item = ItemNum-1;
    }else
	{
        Item--;
    }
	/* 显示项目名称 */
    GUI_DispStringAtBar(*(DispItem+Item),0,Line_4_PosY,Line_4_XSize,GUI_TA_HCENTER);
	/* 显示项目ICO图标 */
    GUI_DrawIcon(Ico_Index[Item],ICO_PosX,ICO_PosY);
	/* 显示滚动条 */
    Bar(Item,ItemNum,MenuBarPosX,MenuBarNumPosX);
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuIcoIndex,FlashMode_NoAction); 
}

/************************************************************************************************/
/* 函数名	: MenuICORight																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 菜单界面向下选择项目																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/ 
static void MenuICORight(void)
{
	/* 设置项目标号 */
    if(Item == (ItemNum-1))
	{
        Item = 0;
    }else{
        Item++;
    }
	/* 显示项目名称 */
    GUI_DispStringAtBar(*(DispItem+Item),0,Line_4_PosY,Line_4_XSize,GUI_TA_HCENTER);
	/* 显示项目ICO图标 */
    GUI_DrawIcon(Ico_Index[Item],ICO_PosX,ICO_PosY);
	/* 显示滚动条 */
    Bar(Item,ItemNum,MenuBarPosX,MenuBarNumPosX);
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuIcoIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuIcoInit																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示菜单界面信息																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuIcoInit(void)
{

	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 显示菜单界面的项目内容 */
		ItemNum = Menu_Ico[*Language].TextNum;/* 多少数据项 */
		DispItem = Menu_Ico[*Language].Text;  /* 数据 */
		
		if(Flash != FlashMode_ReFlash)
		{
			/* 设置项目光标 */
			Item = 0;
			Where = 0;			
		}
		/* 用户的初始化代码请放在这里 */
		if(*Language == EN)
		{
			GUI_Disp_Headline("Menu");
		}else
		{
			GUI_Disp_Headline("菜单");
		}
		/* 显示项目 ICO 图标 */
		GUI_DrawIcon(Ico_Index[Item],ICO_PosX,ICO_PosY);
		
		/* 显示用户类型 */
		GUI_Draw_Bar_BMP(User_Type_Index[User],User_BMP_PosX,User_BMP_PosY);
		
		/* 显示菜单名称 */
		GUI_DispStringAtBar(*(Menu_Ico[*Language].Text+Item),Line_4_PosX,Line_4_PosY,
								Line_4_XSize,GUI_TA_HCENTER);
		/* 显示滚动条 */
		Bar(Item,ItemNum,MenuBarPosX,MenuBarNumPosX);
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuIcoUp																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入选择项目面																		*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuIcoUp(void)
{
  
    switch(Item)
	{
        case 0:
		{
			/* 进入 报警设置 界面 */
			Jump2Menu(MenuAlarmSetIndex,FlashMode_AutoInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
        case 1:{ 
            /* 进入 运行参数 界面 */
            Jump2Menu(MenuRunParaIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
        case 2:
		{
            /* 进入 网络参数 界面 */
            Jump2Menu(MenuNwkParaIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
        case 3:
		{
            /* 进入 系统设置 界面 */
            Jump2Menu(MenuStmSetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
#if (Print_Function_Enable == 0x01)
        case 4:
		{
            /* 进入 打印 界面 */
			Jump2Menu(MenuPitSetIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
#endif
		
#if (Gateway_NJSY_Enable == 0x01)
        case 5:
		{
            /* 进入 网关参数 界面 */
			Jump2Menu(MenuGWIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
#elif (Gateway_HZZH_Enable == 0x01)		
        case 5:
		{
            /* 进入 网关参数 界面 */
			Jump2Menu(MenuGWHZZHIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
      }
		
#endif
				
#if ((GSM_Module_Type == 0x02)&&((Gateway_NJSY_Enable == 0x01)||(Gateway_HZZH_Enable == 0x01)))
       case 6:
		{
            /* 进入 Wifi参数 界面 */
			Jump2Menu(MenuWifiIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
     }
#elif ((GSM_Module_Type == 0x02))	
       case 5:
		{
            /* 进入 Wifi参数 界面 */
			Jump2Menu(MenuWifiIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
     }	 	 
#endif

        default:
		 {
            Jump2Menu(MenuIcoIndex,FlashMode_NoAction);
            break;
     }
    }

}

/************************************************************************************************/
/* 函数名	: DispMenuIcoDown																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出菜单界面																		*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuIcoDown(void)
{
	/* 判断 当前 是否为管理员或超级管理员模式 */
	if(User != Domestic)
	{
		/* 当前模式为:管理员或超级管理员模式 */
		/* 在退出功能设置界面时，管理员或超级管理员模式将被设置为普通人模式 */
		User = Domestic;
	}
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuIcoLeft																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择菜单项目																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuIcoLeft(void)
{
	/* 向上选择菜单项目 */
	MenuICOLeft();
}

/************************************************************************************************/
/* 函数名	: DispMenuIcoLeft																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择菜单项目																	*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuIcoRight(void)
{
	/* 向下选择菜单项目 */
	MenuICORight();
}

/************************************************************************************************/
/* 函数名	: DispMenuIcoOK																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入密钥界面																		*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuIcoOK(void)
{
	/* 进入 密钥 界面 */
	Jump2Menu(MenuUserPermSetIndex,FlashMode_ManualInit);
	/* 进入到下一级界面 */
	PUSH();
}

/************************************************************************************************/
/* 函数名	: DispMenuIcoCancel																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）															*/
/* 创建日期	: 2015/6/4																			*/
/************************************************************************************************/
void DispMenuIcoCancel(void) 
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuIcoIndex,FlashMode_NoAction);
}

/******************* (C) ZHOU *****END OF FILE****/
 

