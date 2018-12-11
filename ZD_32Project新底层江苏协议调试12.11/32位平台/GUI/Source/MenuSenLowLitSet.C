/************************************************************************************************/
/** @file              : MenuSensorTempLowLimit.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月10日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->温度传感器下限界面												*/
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


#include "Log_Sensor.h"


/************************************************************************************************/


/* 温度传感器上限界面的数据缓存大小 */
#define Sen_Low_Lit_Set_Buf_Size			(5u)

/* 温度传感器上限界面的数据缓存 */
static u8 Sen_Low_Lit_Set_Buf[Sen_Low_Lit_Set_Buf_Size] = {0};

/* 传感器类型. */
#define Menu_Sen_Cha(ch) 	((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))

/* 传感器数据指针. */
static INSTRU_SENSOR_CHANNEL *Menu_Sen_Data_P;



/************************************************************************************************/
/* 函数名	: Sen_Low_Lit_Dis																	*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 指示当前修改的参数位置																*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
static void Sen_Low_Lit_Dis(u8 cursor)
{
	

	/* 根据光标显示对应位置的参数 */
	switch(cursor)
	{
		case 0:
		{
			GUI_DispCharAt(Sen_Low_Lit_Set_Buf[0],Line_2_PosX+Font1608_Size*4,Line_2_PosY);
			break;
		}
		
		case 1:
		{
			GUI_DispDecAt(Sen_Low_Lit_Set_Buf[1],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
			break;
		}
		
		case 2:
		{
			GUI_DispDecAt(Sen_Low_Lit_Set_Buf[2],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
			break;
		}
		
		case 3:
		{
			GUI_DispDecAt(Sen_Low_Lit_Set_Buf[3],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
			break;
		}
		
		case 4:
		{
			GUI_DispDecAt(Sen_Low_Lit_Set_Buf[4],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);
			break;
		}
		
		default:
		{
			break;
		}
	}

	
}

/************************************************************************************************/
/* 函数名	: Sen_Low_Lit_Set															*/
/* 输入		: cursor:修改参数的位置																*/
/* 输出		: 无 																				*/
/* 作用		: 恢复前一次修改参数反白的底色，设置当前参数反白底色										*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
static void Sen_Low_Lit_Set(u8 cursor)
{
	static u8 old_cursor = 0;

	
	/* 根据光标显示对应位置的参数 */
	Sen_Low_Lit_Dis(old_cursor);
	
	/* 设置反白字体 */
	GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
	/* 根据光标显示对应位置的参数 */
	Sen_Low_Lit_Dis(cursor);
	/* 设置正常字体 */
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
	/* 设置旧光标位置 */
	old_cursor = cursor;

}


/************************************************************************************************/
/* 函数名	: Disp_Menu_Tmp																		*/
/* 输入		: data:数据.																		*/
/* 输出		: 无 																				*/
/* 作用		: 显示温度类型数据界面.																*/
/* 创建日期	: 2015/9/7																			*/
/************************************************************************************************/
static void Disp_Menu_Tmp(float data)
{
	

		/* 设置 温度传感器上限 参数 */
		/* 判断温度数据的 正负值  */
		if(data >= 0.0)
		{
			Sen_Low_Lit_Set_Buf[0] = '+';
			
		}else
		{
			Sen_Low_Lit_Set_Buf[0] = '-';
		}
		Sen_Low_Lit_Set_Buf[1] = (u8)((u32)data%1000/100);
		Sen_Low_Lit_Set_Buf[2] = (u8)((u32)data%100/10);
		Sen_Low_Lit_Set_Buf[3] = (u8)((u32)data%10);
		Sen_Low_Lit_Set_Buf[4] = (u8)((u32)(data*10)%10);
		/* 显示固定位置的符号 */
		GUI_DispStringAt(".",Line_2_PosX+Font1608_Size*8 ,Line_2_PosY);
		GUI_DispCharAt((*((u8*)"℃")),Line_2_PosX+Font1608_Size*11 ,Line_2_PosY);
		GUI_DispCharAt((*(((u8*)"℃")+1)),Line_2_PosX+Font1608_Size*12 ,Line_2_PosY);

		/* 显示温度传感器上限参数 */
//		GUI_DispCharAt(Sensor_Temp_Up_Limit_Bufferr[0],Line_2_PosX+Font1608_Size*5,Line_2_PosY);
		GUI_DispDecAt(Sen_Low_Lit_Set_Buf[1],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
		GUI_DispDecAt(Sen_Low_Lit_Set_Buf[2],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
		GUI_DispDecAt(Sen_Low_Lit_Set_Buf[3],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
		GUI_DispDecAt(Sen_Low_Lit_Set_Buf[4],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);

		/* 显示当前修改的参数位置 */
		Sen_Low_Lit_Set(Input_Para_Win_Cursor);

}


/************************************************************************************************/
/* 函数名	: Disp_Menu_Hum																		*/
/* 输入		: data:数据.																		*/
/* 输出		: 无 																				*/
/* 作用		: 显示湿度类型数据界面.																*/
/* 创建日期	: 2015/9/7																			*/
/************************************************************************************************/
static void Disp_Menu_Hum(float data)
{
	

	/* 设置 湿度传感器上限 参数 */
	/* 判断温度数据的 正负值  */
	if(data >= 0.0)
	{
		Sen_Low_Lit_Set_Buf[0] = '+';
		
	}else
	{
		Sen_Low_Lit_Set_Buf[0] = '-';
	}
	Sen_Low_Lit_Set_Buf[1] = (u8)((u32)data%1000/100);
	Sen_Low_Lit_Set_Buf[2] = (u8)((u32)data%100/10);
	Sen_Low_Lit_Set_Buf[3] = (u8)((u32)data%10);
	Sen_Low_Lit_Set_Buf[4] = (u8)((u32)(data*10)%10);
	/* 显示固定位置的符号 */
	GUI_DispStringAt(".",Line_2_PosX+Font1608_Size*8 ,Line_2_PosY);
	GUI_DispStringAt("%RH",Line_2_PosX+Font1608_Size*11 ,Line_2_PosY);
	/* 显示温度传感器上限参数 */
//		GUI_DispCharAt(Sensor_Temp_Up_Limit_Bufferr[0],Line_2_PosX+Font1608_Size*5,Line_2_PosY);
	GUI_DispDecAt(Sen_Low_Lit_Set_Buf[1],Line_2_PosX+Font1608_Size*5 ,Line_2_PosY,1);
	GUI_DispDecAt(Sen_Low_Lit_Set_Buf[2],Line_2_PosX+Font1608_Size*6 ,Line_2_PosY,1);
	GUI_DispDecAt(Sen_Low_Lit_Set_Buf[3],Line_2_PosX+Font1608_Size*7 ,Line_2_PosY,1);
	GUI_DispDecAt(Sen_Low_Lit_Set_Buf[4],Line_2_PosX+Font1608_Size*9 ,Line_2_PosY,1);

	/* 显示当前修改的参数位置 */
	Sen_Low_Lit_Set(Input_Para_Win_Cursor);

}


/************************************************************************************************/
/* 函数名	: Sen_Up_Lit_Set																	*/
/* 输入		: sen_type:传感器类型;data:数据.																*/
/* 输出		: 无 																				*/
/* 作用		: 根据传感器类型显示不同的界面和数据(现在只有2种:温度和湿度)。									*/
/* 创建日期	: 2015/9/7																			*/
/************************************************************************************************/
static void Disp_Menu_Sen_Data(u8 sen_type,float data)
{

	/* 根据传感器类型显示不同的界面和数据. */
	switch(sen_type)
	{
		
		case Sensor_Tmp:
		{
			/* 温度传感器 */
			Disp_Menu_Tmp(data);
			break;
		}
	
		case Sensor_Hum:
		{
			/* 湿度传感器 */
			Disp_Menu_Hum(data);
			break;
		}
	
		case Sensor_RF_Card_Single_Tmp:
		{
			/* 单温卡片 */
			Disp_Menu_Tmp(data);
			break;
		}
		
		case Sensor_RF_Card_Humiture_Tmp:
		{
			/* 温湿卡片->温度通道 */
			Disp_Menu_Tmp(data);
			break;
		}	
		
		case Sensor_RF_Card_Humiture_Hum:
		{
			/* 温湿卡片->湿度通道 */
			Disp_Menu_Hum(data);
			break;
		}			

		case Sensor_GPS_POS_Lon:
		{
			/* 经度通道 */
			break;
		}	

		case Sensor_GPS_POS_Lat:
		{
			/* 纬度通道 */
			break;
		}	

		case Sensor_GPS_POS_Spd:
		{
			/* 速度通道 */
			break;
		}	
		
		case Sensor_PT1000:
		{
			/* PT1000温度传感器 */
			Disp_Menu_Tmp(data);
			break;
		}
		
		case Sensor_GW_NJSY_Tmp:
		{
			/* 南京盛亿网关 温度传感器 */
			Disp_Menu_Tmp(data);
			break;
		}
		
		case Sensor_GW_NJSY_Hum:
		{
			/* 南京盛亿网关 湿度传感器 */
			Disp_Menu_Tmp(data);
			break;
		}

		default:
		{
			break;
		}

	}



}


/************************************************************************************************/
/* 函数名	: DispMenuSenLowLitSetInit													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示温度传感器下限界面信息															*/
/* 创建日期	: 2015/6/10																			*/
/************************************************************************************************/
void DispMenuSenLowLitSetInit(void)
{
	
	
	//手动设置参数
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Sen_Lit_Set[*Language];
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
		GUI_Disp_Headline(*(Menu_Sen_Up_Low_lit_Item[*Language].Text+ItemBackup[ItemBackup_i-1]));

		/* 初始化传感器数据指针 */
		Menu_Sen_Data_P = Menu_Sen_Cha(ItemBackup[ItemBackup_i-2]);

		/* 显示数据界面 */
		Disp_Menu_Sen_Data(Menu_Sen_Data_P->Type,Menu_Sen_Data_P->SENSOR_Low_Limit);

				
		/* 显示当前修改的参数位置 */
		Sen_Low_Lit_Set(Input_Para_Win_Cursor);
		/* 设置输入参数窗口 */
		MenuInputPara();
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuSenLowLitSetUp														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置数据																			*/
/* 创建日期	: 2015/6/10																			*/
/************************************************************************************************/
void DispMenuSenLowLitSetUp(void)
{

	
    switch(Item)
	{
        case 0:
		{
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 0;
			}
			break;
		}
		
        case 1:
		{
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 1;		
			}
            break;
        }
		
        case 2:
		{
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 2;		
			}
            break;
        }
		
        case 3:
		{
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 3;		
			}
            break;
        }
		
        case 4:
		{
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 4;		
			}
            break;
        }
		
        case 5:
		{
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 5;		
			}
            break;
        }
		
		case 6:
		{ 
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 6;		
			}
            break;
        }
		
		case 7:
		{
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 7;		
			}
            break;
        }
		
		case 8:
		{
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 8;		
			}
            break;
        }
		
		case 9:
		{
			if(Input_Para_Win_Cursor != 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = 9;		
			}
            break;
		}
		
		case 10:
		{
			if(Input_Para_Win_Cursor == 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = '+';
			}
			break;
        }
		case 11:
		{
			if(Input_Para_Win_Cursor == 0)
			{
				Sen_Low_Lit_Set_Buf[Input_Para_Win_Cursor] = '-';
			}
			break;
		}
        case 12:
		{
			/* 跳过设置缓存数据 */
			__nop();
            break;
        }	
		
      
        default:
		{

			Jump2Menu(MenuSenLowLitSetIndex,FlashMode_NoAction);
			break;
        }
    }
	/* 设置下一个光标位置 */
	if(Input_Para_Win_Cursor < (Sen_Low_Lit_Set_Buf_Size-1))
	{
		Input_Para_Win_Cursor++;
	}else
	{
	 	Input_Para_Win_Cursor = 0;
	}
	
	/* 显示当前修改的参数位置 */
	Sen_Low_Lit_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuSenLowLitSetIndex,FlashMode_NoAction);
	
}

/************************************************************************************************/
/* 函数名	: DispMenuSenLowLitSetDown													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 温度传感器下限 界面																*/
/* 创建日期	: 2015/6/10																			*/
/************************************************************************************************/
void DispMenuSenLowLitSetDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuSenLowLitSetLeft													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向前选择参数窗口数据																	*/
/* 创建日期	: 2015/6/10																			*/
/************************************************************************************************/
void DispMenuSenLowLitSetLeft(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinLeft(MenuSenLowLitSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuSenLowLitSetRight													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向后选择参数窗口数据																	*/
/* 创建日期	: 2015/6/8																			*/
/************************************************************************************************/
void DispMenuSenLowLitSetRight(void)
{
    /* 选择参数，并将函数指针指向对应界面的初始化函数 */
	MenuInputParaWinRight(MenuSenLowLitSetIndex);
}

/************************************************************************************************/
/* 函数名	: DispMenuSenLowLitSetOK														*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置修改温度传感器下限参数（参数计时生效）												*/
/* 创建日期	: 2015/6/10																			*/
/************************************************************************************************/
void DispMenuSenLowLitSetOK(void)
{

	float temp;

	if(User == Domestic)//普通用户
	{	
		/* 显示参数配置错误 提示信息 */
		Parameter_Setting_Failure();
		/* 将函数指针指向初始化函数 */
		Jump2Menu(MenuSenLowLitSetIndex,FlashMode_AutoInit);
	}else if(User == Admin)
	{
		if(Sen_Low_Lit_Set_Buf[0]=='+')
		{
			temp = (((float)Sen_Low_Lit_Set_Buf[1])*100+
					((float)Sen_Low_Lit_Set_Buf[2])*10+
					((float)Sen_Low_Lit_Set_Buf[3])+			
					((float)Sen_Low_Lit_Set_Buf[4])*0.1);
		}else
		{
			temp = (0.0-
				   (((float)Sen_Low_Lit_Set_Buf[1])*100+
					((float)Sen_Low_Lit_Set_Buf[2])*10+
					((float)Sen_Low_Lit_Set_Buf[3])+		
					((float)Sen_Low_Lit_Set_Buf[4])*0.1));
		}
		
		/* 判断设置的温度传感器下限值 是否 大于 传感器上限值 */
		if(temp > Menu_Sen_Data_P->SENSOR_Up_Limit)
		{
			//大于上限值
			/* 显示参数配置错误 提示信息 */
			Parameter_Setting_Failure();
			/* 将函数指针指向初始化函数 */
			Jump2Menu(MenuSenLowLitSetIndex,FlashMode_AutoInit);
		}else
		{
			/* 数值正常 */
			/* 将新的数据存入温度传感器缓存 */
			Menu_Sen_Data_P->SENSOR_Low_Limit = temp;
			
			/* 修改 仪器 配置表 对应的参数 */
			Write_Config_Tab((u8*)&Menu_Sen_Data_P->SENSOR_Low_Limit,
							sizeof(Menu_Sen_Data_P->SENSOR_Low_Limit));

//			/* 配置信息 */
//			GSM_Config_Tab[4].CR.Change_Type = 5;
//			GSM_Config_Tab[4].CR.Change_Adder = 0;
//			GSM_Config_Tab[4].CR.Change_Data_Len = Instru_Sensor_Inf_State_Space_Size;
			
			/* 显示参数配置成功 提示信息 */
			Parameter_Setting_Successful();
			/* 退回到上一级界面 */
			POP();
		}
	}else if(User == Super_Admin)//超级管理员
	{
		/* 将函数指针指向初始化函数,更新显示 */
		Jump2Menu(MenuSenLowLitSetIndex,FlashMode_NoAction);
	}
		
}

/************************************************************************************************/
/* 函数名	: DispMenuSenLowLitSetCancel													*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 光标指向前一个参数的位置																*/
/* 创建日期	: 2015/6/10																			*/
/************************************************************************************************/
void DispMenuSenLowLitSetCancel(void) 
{

	
	/* 判断数据缓存指针位置 */
	if(Input_Para_Win_Cursor == 0)
	{
		/* 设置完成一个数据缓存指针后，将指针指向下一个数据缓存。 */
		Input_Para_Win_Cursor = Sen_Low_Lit_Set_Buf_Size-1;
	}else
	{
	 	/* 数据缓存指针在最后一个缓存地址，将指针指向第一个数据缓存地址。 */
		Input_Para_Win_Cursor--;
	}
	
	/* 显示当前修改的参数位置 */
	Sen_Low_Lit_Set(Input_Para_Win_Cursor);
	
	/* 将函数指针指向初始化函数 */
	Jump2Menu(MenuSenLowLitSetIndex,FlashMode_NoAction);
}

/******************* (C) ZHOU *****END OF FILE****/


