#ifndef __Mcu_led_H
#define	__Mcu_led_H
#include "stm32f10x.h"
#include "ucos_ii.h"
/************************************************************************************************/
/* 光提示 队列 数据 类型 */
typedef enum {
	/* 无信号 类型 */
	Null_Signal_Light_EVENT		= 0X00,
	/* 传感器 报警 类型 */
	Sensor_Signal_Light_EVENT 	= 0X01,
	/* 掉电 报警 类型 */
	Power_Signal_Light_EVENT 	= 0X02,
	/* 电池电量 报警 类型 */
	Battery_Signal_Light_EVENT 	= 0X03,
	
}LIGHT_CMD_TYPE;

typedef struct
{
	u16		Light_Open_Time;	// 光提示 开 时间（ms）
	u16		Light_Close_Time;	// 光提示 关 时间（ms）
	BOOL	EVENT_Getarg;		// 事件 执行结果(FALSE:信号正在执行，TRUE:型号执行结束)
}LIGHT_QUE;
/************************************************************************************************/
/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define LED_ON            1
#define LED_OFF           0
#define Light_ON          1
#define Light_OFF         0
#define LED1_PORT         GPIOE
#define LED1_CLK          RCC_APB2Periph_GPIOE
#define LED1_PIN          GPIO_Pin_3
#define LED2_PORT         GPIOE
#define LED2_CLK          RCC_APB2Periph_GPIOE
#define LED2_PIN          GPIO_Pin_4
#define LED1(a)		if (a)	\
					GPIO_SetBits(LED1_PORT,LED1_PIN);\
					else		\
					GPIO_ResetBits(LED1_PORT,LED1_PIN)
/* 这个led端口是用来报警提示用的 */					
#define Light(a)	if (a)	\
					GPIO_SetBits(LED2_PORT,LED2_PIN);\
					else		\
					GPIO_ResetBits(LED2_PORT,LED2_PIN)
//#define LED3(a)	if (a)	\
//					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
//					else		\
//					GPIO_ResetBits(GPIOB,GPIO_Pin_0)

//#define LED4(a)	if (a)	\
//					GPIO_SetBits(GPIOE,GPIO_Pin_11);\
//					else		\
//					GPIO_ResetBits(GPIOE,GPIO_Pin_11)
//					
//#define LED5(a)	if (a)	\
//					GPIO_SetBits(GPIOE,GPIO_Pin_12);\
//					else		\
//					GPIO_ResetBits(GPIOE,GPIO_Pin_12)
void LED_GPIO_Init(void);
void Light_Send_Q(OS_EVENT* pevent,LIGHT_CMD_TYPE EVENT_Type);
void Sensor_Alarm_Light(void);
void Power_Alarm_Light(void);
void Battery_Alarm_Light(void);
void Heart_Runing_Led(void);//心跳运行指示灯   PE3
#endif /* __LED_H */
