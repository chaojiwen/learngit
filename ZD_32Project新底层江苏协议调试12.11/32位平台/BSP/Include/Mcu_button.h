/*
*********************************************************************************************************
*	                                  
*	模块名称 : 按键驱动模块	（参考armfly开发板）	 
*	文件名称 : Mcu_button.h
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

#ifndef __Mcu_button_H
#define __Mcu_button_H

#include "stm32f10x.h"
#include "ucos_ii.h"       //uC/OS-II 系统函数头文件  

/********************************* 按键 端口定义 *********************************/
#ifndef Hardware_Version_V131

#define Btn1_PORT              GPIOC
#define Btn1_CLK               RCC_APB2Periph_GPIOC
#define Btn1_PIN               GPIO_Pin_6

#define Btn2_PORT              GPIOC
#define Btn2_CLK               RCC_APB2Periph_GPIOC 
#define Btn2_PIN               GPIO_Pin_8

#define Btn3_PORT              GPIOC
#define Btn3_CLK               RCC_APB2Periph_GPIOC
#define Btn3_PIN               GPIO_Pin_7

#define Btn4_PORT              GPIOC
#define Btn4_CLK               RCC_APB2Periph_GPIOC 
#define Btn4_PIN               GPIO_Pin_9

#define KEY_Power_PORT         GPIOE
#define KEY_Power_CLK          RCC_APB2Periph_GPIOE
#define KEY_Power_PIN          GPIO_Pin_5

#else


#define Btn1_PORT              GPIOC
#define Btn1_CLK               RCC_APB2Periph_GPIOC
#define Btn1_PIN               GPIO_Pin_9

#define Btn2_PORT              GPIOC
#define Btn2_CLK               RCC_APB2Periph_GPIOC 
#define Btn2_PIN               GPIO_Pin_11

#define Btn3_PORT              GPIOC
#define Btn3_CLK               RCC_APB2Periph_GPIOC
#define Btn3_PIN               GPIO_Pin_10

#define Btn4_PORT              GPIOC
#define Btn4_CLK               RCC_APB2Periph_GPIOC 
#define Btn4_PIN               GPIO_Pin_12

#define KEY_Power_PORT         GPIOE
#define KEY_Power_CLK          RCC_APB2Periph_GPIOE
#define KEY_Power_PIN          GPIO_Pin_5

#endif

/* 按键电源 控制引脚 */
#define KEY_Power_Open  	{GPIO_ResetBits(KEY_Power_PORT,KEY_Power_PIN);}
#define KEY_Power_Close 	{GPIO_SetBits(KEY_Power_PORT,KEY_Power_PIN);}

/* 
按键滤波时间50ms, 单位10ms
只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件

注意：不采用系统的时候是10ms执行一次，现在采用系统了，20ms执行一次
*/
#define BUTTON_FILTER_TIME 	2		/* 2*10 = 20ms */
#define BUTTON_LONG_TIME 	100		/* 100*20 = 2s ,认为长按事件 */
#define BUTTON_Repeat_TIME 	30		/* 30*20 = 600ms 认为连发事件 */

/*
	每个按键对应1个全局的结构体变量。
	其成员变量是实现滤波和多种按键状态所必须的
*/
typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t Count;			/* 滤波器计数器 */
	uint8_t FilterTime;		/* 滤波时间(最大255,表示2550ms) */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
	uint8_t KeyCodeUp;		/* 按键弹起的键值代码, 0表示不检测按键弹起 */
	uint8_t KeyCodeDown;	/* 按键按下的键值代码, 0表示不检测按键按下 */
	uint8_t KeyCodeLong;	/* 按键长按的键值代码, 0表示不检测长按 */
	uint8_t RepeatSpeed;	/* 连续按键周期 */
	uint8_t RepeatCount;	/* 连续按键计数器 */
}BUTTON_T;

/* 定义键值代码
	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2)	编译器可帮我们避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_DOWN_Sure,			/* 确定 键按下 */
	KEY_UP_Sure,			/* 确定 键弹起 */
	KEY_LONG_Sure,			/* 确定 键弹起 */
	
	KEY_DOWN_Forward,		/* 向前 键按下 */
	KEY_UP_Forward,			/* 向前 键弹起 */	
	KEY_LONG_Forward,		/* 向前 键长按 */
	
	KEY_DOWN_Backward,		/* 向后 键按下 */
	KEY_UP_Backward,		/* 向后 键弹起 */	
	KEY_LONG_Backward,		/* 向后 键长按 */	

	KEY_DOWN_Cancel,		/* 返回 键按下 */
	KEY_UP_Cancel,			/* 返回 键弹起 */
	KEY_LONG_Cancel,		/* 返回 键长按 */

// 	KEY_DOWN_USER_TAMPER	/* 组合键，USER键和WAKEUP键同时按下 */
}KEY_ENUM;

/* 按键ID */
enum
{
	KID_Sure = 0,
	KID_Forward,
	KID_Backward,
	KID_Cancel	
};

/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	20
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针 */
	uint8_t Write;					/* 缓冲区写指针 */
}KEY_FIFO_T;


/* 定义一个邮箱，    这只是一个邮箱指针，  OSMboxCreate函数会创建邮箱必需的资源 */
extern OS_EVENT *App_KEY_Mbox;
/* 用于查询按键值时保存当前信号量数据的数据结构 */
extern OS_MBOX_DATA KEY_Mbox_DATA;

/* 供外部调用的函数声明 */
void 	Button_Init			(void);
void 	bsp_PutKey			(uint8_t _KeyCode);
uint8_t Get_Key_State		(void);
void 	Key_State_Check_All	(void);
uint8_t bsp_KeyState		(uint8_t _ucKeyID);


//void 	Circulate_Data_Value_Deal(u32* data_p,u32 up_limit,u32 down_limit,s32 offset);

void bsp_InitButtonHard(void);

#endif



