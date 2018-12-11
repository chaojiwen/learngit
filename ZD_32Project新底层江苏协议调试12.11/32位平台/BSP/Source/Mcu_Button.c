/*
*********************************************************************************************************
*	                                  
*	模块名称 : 按键驱动模块	（参考armfly开发板）	 
*	文件名称 : Mcu_Button.c
*	版    本 : V1.0
*	说    明 : 实现按键的检测，具有软件滤波机制，可以检测如下事件：
*			  (1) 按键按下
*			  (2) 按键弹起
*			  (3) 长按键
*			  (4) 长按时自动连发
*			  (5) 组合键
*
*	修改记录 :
*	    版本号    日期          作者                 说明
*		v1.0    2011-12-07   WildFire Team  ST固件库版本 V3.5.0版本。
*
*	Copyright (C), 2012-2013, WildFire Team
*
*********************************************************************************************************
*/
#include "Mcu_Button.h"
#include "stm32f10x.h"
#include <stdio.h>

/* 用于查询按键值时保存当前信号量数据的数据结构 */
OS_MBOX_DATA KEY_Mbox_DATA;

static BUTTON_T s_BtnSure;		   	/* 确定 键 */
static BUTTON_T s_BtnForward;	   	/* 向前 键 */
static BUTTON_T s_BtnBackward;   	/* 向后 键 */
static BUTTON_T s_BtnCancel;   		/* 取消 键 */

static KEY_FIFO_T s_Key;	  	   /* 按键FIFO变量,结构体 */

static void bsp_InitButtonVar(void);
// static void bsp_InitButtonHard(void);
static void bsp_DetectButton(BUTTON_T *_pBtn);

/*
	野火开发板按键口线分配：
	KEY1键     : PC6  (低电平表示按下)
	KEY2键     : PC7  (低电平表示按下)
	KEY3键     : PC8  (低电平表示按下)
	KEY4键     : PC9  (低电平表示按下)	
	Sure  	-----> KEY1
	Forward	-----> KEY2
	Backward-----> KEY3
	Cancel	-----> KEY4
	定义函数判断按键是否按下，返回值1 表示按下，0表示未按下
*/												
static uint8_t IsKeyDownSure(void) 		{if (GPIO_ReadInputDataBit(Btn1_PORT,Btn1_PIN) == Bit_SET) return 1; return 0;}
static uint8_t IsKeyDownForward(void) 	{if (GPIO_ReadInputDataBit(Btn2_PORT,Btn2_PIN) == Bit_SET) return 1; return 0;}
static uint8_t IsKeyDownBackward(void)	{if (GPIO_ReadInputDataBit(Btn3_PORT,Btn3_PIN) == Bit_SET) return 1; return 0;}	
static uint8_t IsKeyDownCancel(void)	{if (GPIO_ReadInputDataBit(Btn4_PORT,Btn4_PIN) == Bit_SET) return 1; return 0;}
/*
*********************************************************************************************************
*	函 数 名: Button_Init
*	功能说明: 初始化按键
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Button_Init(void)
{
	bsp_InitButtonVar();		/* 初始化按键变量 */
 	bsp_InitButtonHard();		/* 初始化按键硬件 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参：_KeyCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_Key.Buf[s_Key.Write] = _KeyCode;

	if (++s_Key.Write  >= KEY_FIFO_SIZE)
	{
		s_Key.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Get_Key_State
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t Get_Key_State(void)
{
	uint8_t ret;

	if (s_Key.Read == s_Key.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_Key.Buf[s_Key.Read];

		if (++s_Key.Read >= KEY_FIFO_SIZE)
		{
			s_Key.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyState
*	功能说明: 读取按键的状态
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_KeyState(uint8_t _ucKeyID)
{
	uint8_t ucState = 0;

	switch (_ucKeyID)
	{
		case KID_Sure:
			ucState = s_BtnSure.State;
			break;

		case KID_Forward:
			ucState = s_BtnForward.State;
			break;
		
		case KID_Backward:
			ucState = s_BtnBackward.State;
			break;
		
		case KID_Cancel:
			ucState = s_BtnCancel.State;
			break;				
		
	}

	return ucState;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonVar
*	功能说明: 初始化按键硬件
*	形    参：strName : 
*			  strDate : 
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitButtonHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOE ,
						   ENABLE);
	/* K2/K1/K4/K3 */
	GPIO_InitStructure.GPIO_Pin = Btn1_PIN | Btn2_PIN |
								  Btn3_PIN | Btn4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;    	
	GPIO_Init(GPIOC, &GPIO_InitStructure);   
	
	/* KEY_Power */
	GPIO_InitStructure.GPIO_Pin 	= KEY_Power_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	
	GPIO_Init(KEY_Power_PORT, &GPIO_InitStructure);	
	GPIO_SetBits(KEY_Power_PORT, KEY_Power_PIN );// turn off all led  
}
		

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonVar
*	功能说明: 初始化按键变量
*	形    参：strName : 例程名称字符串
*			  strDate : 例程发布日期
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitButtonVar(void)
{
	/* 对按键FIFO读写指针清零 */
	s_Key.Read = 0;
	s_Key.Write = 0;

	/* 初始化Sure按键变量，支持按下、弹起。 */
	s_BtnSure.IsKeyDownFunc = IsKeyDownSure;		/* 判断按键按下的函数 */
	s_BtnSure.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */
 	s_BtnSure.LongTime = BUTTON_LONG_TIME;			/* 长按时间 */
	s_BtnSure.Count = s_BtnSure.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnSure.State = 0;							/* 按键缺省状态，0为未按下 */
	s_BtnSure.KeyCodeDown = KEY_DOWN_Sure;			/* 按键按下的键值代码 */
	s_BtnSure.KeyCodeUp = KEY_UP_Sure;				/* 按键弹起的键值代码 */
 	s_BtnSure.KeyCodeLong = KEY_LONG_Sure;			/* 按键被持续按下的键值代码 */	
	s_BtnSure.RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
	s_BtnSure.RepeatCount = 0;						/* 连发计数器 */		

	/* 初始化Forward按键变量，支持按下、弹起、长按 */
	s_BtnForward.IsKeyDownFunc = IsKeyDownForward;		/* 判断按键按下的函数 */
	s_BtnForward.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */
	s_BtnForward.LongTime = BUTTON_LONG_TIME;			/* 长按时间, 0表示不检测 */
	s_BtnForward.Count = s_BtnForward.FilterTime / 2;	/* 计数器设置为滤波时间的一半 */
	s_BtnForward.State = 0;								/* 按键缺省状态，0为未按下 */
	s_BtnForward.KeyCodeDown = KEY_DOWN_Forward;		/* 按键按下的键值代码 */
	s_BtnForward.KeyCodeUp = KEY_UP_Forward;			/* 按键弹起的键值代码 */
	s_BtnForward.KeyCodeLong = KEY_LONG_Forward;		/* 按键被持续按下的键值代码 */
	s_BtnForward.RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
	s_BtnForward.RepeatCount = 0;						/* 连发计数器 , 应该是用来读连发数量*/		

	/* 初始化Backward键变量，支持按下、弹起、长按 */
	s_BtnBackward.IsKeyDownFunc = IsKeyDownBackward;		/* 判断按键按下的函数 */
	s_BtnBackward.FilterTime = BUTTON_FILTER_TIME;			/* 按键滤波时间 */
	s_BtnBackward.LongTime = BUTTON_LONG_TIME;				/* 长按时间 */
	s_BtnBackward.Count = s_BtnBackward.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnBackward.State = 0;								/* 按键缺省状态，0为未按下 */
	s_BtnBackward.KeyCodeDown = KEY_DOWN_Backward;			/* 按键按下的键值代码 */
	s_BtnBackward.KeyCodeUp = KEY_UP_Backward;				/* 按键弹起的键值代码，0表示不检测 */
	s_BtnBackward.KeyCodeLong = KEY_LONG_Backward;			/* 按键被持续按下的键值代码，0表示不检测 */
	s_BtnBackward.RepeatSpeed = 0;							/* 按键连发的速度，0表示不支持连发 */
	s_BtnBackward.RepeatCount = 0;							/* 连发计数器 */
	
	/* 初始化Cancel键变量，支持按下、弹起。 */
	s_BtnCancel.IsKeyDownFunc = IsKeyDownCancel;			/* 判断按键按下的函数 */
	s_BtnCancel.FilterTime = BUTTON_FILTER_TIME;			/* 按键滤波时间 */
	s_BtnCancel.LongTime = BUTTON_LONG_TIME;				/* 长按时间 */
	s_BtnCancel.Count = s_BtnCancel.FilterTime / 2;			/* 计数器设置为滤波时间的一半 */
	s_BtnCancel.State = 0;									/* 按键缺省状态，0为未按下 */
	s_BtnCancel.KeyCodeDown = KEY_DOWN_Cancel;				/* 按键按下的键值代码 */
	s_BtnCancel.KeyCodeUp = KEY_UP_Cancel;					/* 按键弹起的键值代码，0表示不检测 */
	s_BtnCancel.KeyCodeLong = KEY_LONG_Cancel;				/* 按键被持续按下的键值代码，0表示不检测 */
	s_BtnCancel.RepeatSpeed = 0;							/* 按键连发的速度，0表示不支持连发 */
	s_BtnCancel.RepeatCount = 0;							/* 连发计数器 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectButton
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参：按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectButton(BUTTON_T *_pBtn)
{
	/* 如果没有初始化按键函数，则报错
	if (_pBtn->IsKeyDownFunc == 0)
	{
		printf("Fault : DetectButton(), _pBtn->IsKeyDownFunc undefine");
	}
	*/

	if (_pBtn->IsKeyDownFunc())
	{
		if (_pBtn->Count < _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count < 2 * _pBtn->FilterTime)
		{
			_pBtn->Count++;
		}
		else
		{
			if (_pBtn->State == 0)
			{
				_pBtn->State = 1;

//				/* 发送按钮按下的消息 */
//				if (_pBtn->KeyCodeDown > 0)
//				{
//					/* 键值放入按键FIFO */
//					bsp_PutKey(_pBtn->KeyCodeDown);
//				}
			}

			if (_pBtn->LongTime > 0)
			{
				if (_pBtn->LongCount < _pBtn->LongTime)
				{
					/* 发送按钮持续按下的消息 */
					if (++_pBtn->LongCount == _pBtn->LongTime)
					{
						/* 键值放入按键FIFO */
						bsp_PutKey(_pBtn->KeyCodeLong);						
					}
				}
				else
				{
					if (_pBtn->RepeatSpeed > 0)
					{
						if (++_pBtn->RepeatCount >= _pBtn->RepeatSpeed)
						{
							_pBtn->RepeatCount = 0;
							/* 常按键后，每隔10ms发送1个按键 */
							bsp_PutKey(_pBtn->KeyCodeDown);														
						}
					}
				}
			}
		}
	}
	else
	{
		if(_pBtn->Count != 0)
		{
			if (_pBtn->State == 1)
			{
				_pBtn->State = 0;

				if (_pBtn->LongTime > 0)
				{
					/* 判断 长按 计数器 值 */
					if (_pBtn->LongCount < _pBtn->LongTime)
					{
						/* 发送按钮按下的消息 */
						if (_pBtn->KeyCodeDown > 0)
						{
							/* 键值放入按键FIFO */
							bsp_PutKey(_pBtn->KeyCodeDown);
						}		
					}else
					{

						__nop();
					}
				}else
				{
					/* 发送按钮按下的消息 */
					if (_pBtn->KeyCodeDown > 0)
					{
						/* 键值放入按键FIFO */
						bsp_PutKey(_pBtn->KeyCodeDown);
					}
				}

//				/* 发送按钮弹起的消息 */
//				if (_pBtn->KeyCodeUp > 0)
//				{
//					/* 键值放入按键FIFO */
//					bsp_PutKey(_pBtn->KeyCodeUp);			
//				}
			}
			_pBtn->LongCount = 0;
			_pBtn->RepeatCount = 0;
			_pBtn->Count =0;
		}
	}
}

///* **********************************************************************************
//
// * 函数名：Circulate_Data_Value_Deal
// * 描述  ：对循环数值的处理
// * 输入  ：data_p -> 数据地址指针
//		   up_limit -> 数据上限
//		   down_limit -> 数据下限
//		   offset -> 数据偏移量
// * 输出  ：无
//
// * **********************************************************************************/
//void Circulate_Data_Value_Deal(u32* data_p,u32 up_limit,u32 down_limit,s32 offset)
//{
//	/* 判断偏移量正负值 */
//	if(offset > 0)
//	{
//		//正值
//		/* 判断是否超限	*/
//		if((((s32)(*data_p)) + offset) <= up_limit)
//		{
//			/* 没有超限 */
//			(*data_p) = (((s32)(*data_p))+ offset);
//		}
//	}else if(offset < 0)
//	{
//		//负值
//		/* 判断是否超限 */
//		if((((s32)(*data_p)) + offset) >= down_limit)
//		{
//			/* 没有超限 */
//			(*data_p) = (((s32)(*data_p)) + offset);
//		}
//		
//	}else
//	{
//		//等于0
//		/* 预留着，不做处理. */
//	}
//}

/************************************************************************************************/
/* 函数名	: Key_State_Check_All																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 检测所有按键。非阻塞状态，必须被周期性的调用。											*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/ 
void Key_State_Check_All(void)
{
	bsp_DetectButton(&s_BtnSure);		/* 确定 键 */
	bsp_DetectButton(&s_BtnForward);	/* 向前 键 */
	bsp_DetectButton(&s_BtnBackward);	/* 向后 键 */
	bsp_DetectButton(&s_BtnCancel);		/* 取消 键 */		
}
