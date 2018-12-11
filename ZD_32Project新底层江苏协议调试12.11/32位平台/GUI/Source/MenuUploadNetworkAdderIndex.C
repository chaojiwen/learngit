/************************************************************************************************/
/** @file              : MenuUploadNetworkAdderIndex.c											*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年06月08日															*/
/** @date changed      : 2015年10月08日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->上传数据的网络地址参数设置界面.(包含:IP和端口)				*/
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
#include "app.h"

#include "Log_GSM.h"

#include "Dev_RTC.h"

#include <stdio.h>
#include <string.h>


/* 上传数据的网络地址的数据缓存大小 */
#define Uld_Nwk_Add_Buf_Size 			(17u)

/* 上传数据的网络地址缓存 */
static u8 Uld_Nwk_Add_Buf[Uld_Nwk_Add_Buf_Size] = {0};


/* 上传数据的网络地址的参数设置界面 中英文 菜单栏 */
const struct Menu Menu_Uld_Nwk_Add_Buf[] =
{
	MenuWhat(Menu_Digital_Input),
	MenuWhat(Menu_Digital_Input),
};

/* 上传网络地址数据指针 */
#define Uld_Nwk_Add_Data_P (&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State)



/************************************************************************************************/
/* 函数名	: Uld_Nwk_Add_Para_Dis																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置															*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
static void Uld_Nwk_Add_Para_Dis(u8 cursor)
{

	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[0],Line_2_PosX ,Line_2_PosY,1);
			break;
		}
		
		case 1:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[1],Line_2_PosX+Font1608_Size*1 ,Line_2_PosY,1);	
			break;
		}
		
		case 2:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[2],Line_2_PosX+Font1608_Size*2 ,Line_2_PosY,1);
			break;
		}
		
		case 3:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[3],Line_2_PosX+Font1608_Size*4 ,Line_2_PosY,1);	
			break;
		}
		
		case 4:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[4],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);	
			break;
		}
		
		case 5:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[5],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);	
			break;
		}
		
		case 6:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[6],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);	
			break;
		}
		
		case 7:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[7],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);	
			break;
		}
		
		case 8:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[8],Line_2_PosX+Font1608_Size*10 ,Line_2_PosY,1);	
			break;
		}
		
		case 9:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[9],Line_2_PosX+Font1608_Size*12 ,Line_2_PosY,1);	
			break;
		}
		
		case 10:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[10],Line_2_PosX+Font1608_Size*13 ,Line_2_PosY,1);	
			break;
		}
		
		case 11:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[11],Line_2_PosX+Font1608_Size*14 ,Line_2_PosY,1);	
			break;
		}
		
		case 12:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[12],Line_3_PosX+Font1608_Size*8 ,Line_3_PosY,1);	
			break;
		}
		
		case 13:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[13],Line_3_PosX+Font1608_Size*9 ,Line_3_PosY,1);	
			break;
		}
		
		case 14:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[14],Line_3_PosX+Font1608_Size*10 ,Line_3_PosY,1);	
			break;
		}	
		
		case 15:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[15],Line_3_PosX+Font1608_Size*11 ,Line_3_PosY,1);	
			break;
		}
		
		case 16:
		{
			GUI_DispDecAt(Uld_Nwk_Add_Buf[16],Line_3_PosX+Font1608_Size*12 ,Line_3_PosY,1);	
			break;
		}
		
		default:
		{
			break;
		}
	}

}

/************************************************************************************************/
/* 函数名	: Uld_Nwk_Add_Para_Set																*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色								*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
static void Uld_Nwk_Add_Para_Set(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	Uld_Nwk_Add_Para_Dis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	Uld_Nwk_Add_Para_Dis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;
	
}

/************************************************************************************************/
/* 函数名	: DispMenuUldNwkAddInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示 上传数据的网络地址 信息														*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuUldNwkAddInit(void)
{

	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
        /* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Uld_Nwk_Add_Buf[*Language];
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
		GUI_Disp_Headline(*(Menu_Nwk_Para[*Language].Text+ItemBackup[ItemBackup_i-1]));
		/* 设置 IP和端口 参数 */
		/* IP地址数据 */
		Uld_Nwk_Add_Buf[0]  = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_A%1000/100;
		Uld_Nwk_Add_Buf[1]  =	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_A%100/10;
		Uld_Nwk_Add_Buf[2]  = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_A%10;
		
		Uld_Nwk_Add_Buf[3]  = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_B%1000/100;
		Uld_Nwk_Add_Buf[4]  = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_B%100/10;
		Uld_Nwk_Add_Buf[5]  = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_B%10;
		
		Uld_Nwk_Add_Buf[6]  = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_C%1000/100;
		Uld_Nwk_Add_Buf[7]  = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_C%100/10;
		Uld_Nwk_Add_Buf[8]  = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_C%10;
		
		Uld_Nwk_Add_Buf[9]  = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_D%1000/100;
		Uld_Nwk_Add_Buf[10] = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_D%100/10;
		Uld_Nwk_Add_Buf[11] = 	Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_D%10;

		/* COM数据 */
		Uld_Nwk_Add_Buf[12] = Uld_Nwk_Add_Data_P->Port_ID1%100000/10000;
		Uld_Nwk_Add_Buf[13] = Uld_Nwk_Add_Data_P->Port_ID1%10000/1000;
		Uld_Nwk_Add_Buf[14] = Uld_Nwk_Add_Data_P->Port_ID1%1000/100;
		Uld_Nwk_Add_Buf[15] = Uld_Nwk_Add_Data_P->Port_ID1%100/10;
		Uld_Nwk_Add_Buf[16] = Uld_Nwk_Add_Data_P->Port_ID1%10;
				
		/* 显示固定位置的符号 */
		if(*Language == EN)
		{
			GUI_DispStringAt("Port:",Line_3_PosX+Font1608_Size*3,Line_3_PosY);
		}else
		{
			GUI_DispStringAt("端口:",Line_3_PosX+Font1608_Size*3,Line_3_PosY);
		}
		GUI_DispStringAt(".",Line_2_PosX+Font1608_Size*3 ,Line_2_PosY);
		GUI_DispStringAt(".",Line_2_PosX+Font1608_Size*7 ,Line_2_PosY);
		GUI_DispStringAt(".",Line_2_PosX+Font1608_Size*11,Line_2_PosY);
		/* 显示 IP和端口的参数 */
//		GUI_DispDecAt(Uld_Nwk_Add_Buf[0],Line_2_PosX ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[1],Line_2_PosX+Font1608_Size*1 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[2],Line_2_PosX+Font1608_Size*2 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[3],Line_2_PosX+Font1608_Size*4 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[4],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[5],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[6],Line_2_PosX+Font1608_Size*8 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[7],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[8],Line_2_PosX+Font1608_Size*10 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[9],Line_2_PosX+Font1608_Size*12 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[10],Line_2_PosX+Font1608_Size*13 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[11],Line_2_PosX+Font1608_Size*14 ,Line_2_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[12],Line_3_PosX+Font1608_Size*8 ,Line_3_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[13],Line_3_PosX+Font1608_Size*9 ,Line_3_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[14],Line_3_PosX+Font1608_Size*10 ,Line_3_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[15],Line_3_PosX+Font1608_Size*11 ,Line_3_PosY,1);
		GUI_DispDecAt(Uld_Nwk_Add_Buf[16],Line_3_PosX+Font1608_Size*12 ,Line_3_PosY,1);
		
		/* 显示当前修改的参数位置 */
		Uld_Nwk_Add_Para_Set(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();	
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuUldNwkAddUp																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置 上传数据的网络地址 数据														*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuUldNwkAddUp(void)
{ 

	switch(Item)
	{
		case 0:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 0;
			break;
		}
		case 1:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 1;
			break;
		}
		case 2:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 2;
			break;
		}
		case 3:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 3;
			break;
		}
		case 4:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 4;
			break;
		}
		case 5:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 5;
			break;
		}
		case 6:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 6;
			break;
		}
		case 7:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 7;
			break;
		}
		case 8:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 8;
			break;
		}
		case 9:
		{
			/* 设置缓存数据 */
			Uld_Nwk_Add_Buf[Input_Para_Win_Cursor] = 9;
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
	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor < (Uld_Nwk_Add_Buf_Size-1))
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor++;
	}
	else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor = 0;
	}

	/* 显示当前修改的参数位置 */
	Uld_Nwk_Add_Para_Set(Input_Para_Win_Cursor);
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuUldNwkAddIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuUldNwkAddDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 上传数据的网络地址 界面														*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuUldNwkAddDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuUldNwkAddLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向后选择窗口参数数据																*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuUldNwkAddLeft(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuUldNwkAddIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuUldNwkAddRight															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向前选择窗口参数数据																*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuUldNwkAddRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuUldNwkAddIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuUldNwkAddOK																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置修改的参数（参数计时生效）													*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuUldNwkAddOK(void)
{

	/* 判断当前用户类型 */
	if(User == Domestic)//普通用户
	{
		/* 非管管理员模式，输入的参数视为无效 */
		Parameter_Setting_Failure();
		/* 更新菜单界面显示 */
		Jump2Menu(MenuUldNwkAddIndex,FlashMode_AutoInit);
	}else if(User == Admin)
	{
		
		/* 设置IP数据 */
		Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_A = 
		Uld_Nwk_Add_Buf[0]*100 + Uld_Nwk_Add_Buf[1]*10 + Uld_Nwk_Add_Buf[2];

		Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_B =
		Uld_Nwk_Add_Buf[3]*100 + Uld_Nwk_Add_Buf[4]*10 + Uld_Nwk_Add_Buf[5];
		
		Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_C =
		Uld_Nwk_Add_Buf[6]*100 + Uld_Nwk_Add_Buf[7]*10 + Uld_Nwk_Add_Buf[8];

		Uld_Nwk_Add_Data_P->IP_Address_ID1.Serial_Number_Number.Address_D = 
		Uld_Nwk_Add_Buf[9]*100 + Uld_Nwk_Add_Buf[10]*10 + Uld_Nwk_Add_Buf[11];

		/* 设置端口数据 */
		Uld_Nwk_Add_Data_P->Port_ID1 = 
		Uld_Nwk_Add_Buf[12]*10000 + Uld_Nwk_Add_Buf[13]*1000 + Uld_Nwk_Add_Buf[14]*100 + 
		Uld_Nwk_Add_Buf[15]*10 	  + Uld_Nwk_Add_Buf[16];
		
		/* 修改仪器配置表中对应IP的数据 */
		Write_Config_Tab((u8*)&Uld_Nwk_Add_Data_P->IP_Address_ID1.Server_Address,
						sizeof(Uld_Nwk_Add_Data_P->IP_Address_ID1.Server_Address));
		/* 修改仪器配置表中对应端口的数据 */
		Write_Config_Tab((u8*)&Uld_Nwk_Add_Data_P->Port_ID1,
						sizeof(Uld_Nwk_Add_Data_P->Port_ID1));
			
//		/* 配置信息 */
//		GSM_Config_Tab[3].CR.Change_Type = 4;
//		GSM_Config_Tab[3].CR.Change_Adder = 0;
//		GSM_Config_Tab[3].CR.Change_Data_Len = Instru_GSM_Inf_State_Space_Size;
								
//////		/* 重新配置 GSM模块 */
//////		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
      GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
		
		/* 数据写入成功 */
		Parameter_Setting_Successful();
		/* 退回到上一级界面 */
		POP();
	}else if(User == Super_Admin)//超级管理员
	{
		/* 更新菜单界面显示 */
		Jump2Menu(MenuUldNwkAddIndex,FlashMode_NoAction);
	}
}

/************************************************************************************************/
/* 函数名	: DispMenuUldNwkAddCancel															*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 光标指向前一个参数的位置															*/
/* 创建日期	: 2015/10/08																		*/
/************************************************************************************************/
void DispMenuUldNwkAddCancel(void) 
{
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = Uld_Nwk_Add_Buf_Size-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	Uld_Nwk_Add_Para_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuUldNwkAddIndex,FlashMode_NoAction);
}



/******************* (C) ZHOU *****END OF FILE****/


