#ifndef __Log_RF_Card_
#define __Log_RF_Card_
			  	 

#include "stm32f10x.h"

#include "Mcu_Config.h"

#include "ucos_ii.h"



//#define Module Power 
#define RF_Card_Power_PORT       GPIOC
#define RF_Card_Power_CLK        RCC_APB2Periph_GPIOC 
#define RF_Card_Power_PIN        GPIO_Pin_15

#ifdef Hardware_Version_V131
/* 无限温湿度数据接收模块电源:开 */
#define Open_RF_Card_Power		{GPIO_ResetBits(RF_Card_Power_PORT,RF_Card_Power_PIN);}
/* 无限温湿度数据接收模块电源:关 */
#define Close_RF_Card_Power		{GPIO_SetBits(RF_Card_Power_PORT,RF_Card_Power_PIN);}
#else
/* 无限温湿度数据接收模块电源:开 */
#define Open_RF_Card_Power		{GPIO_SetBits(RF_Card_Power_PORT,RF_Card_Power_PIN);}
/* 无限温湿度数据接收模块电源:关 */
#define Close_RF_Card_Power		{GPIO_ResetBits(RF_Card_Power_PORT,RF_Card_Power_PIN);}

#endif



/* 无线RF卡片接收数据寄存器 指针（这里主要是为了给DMA付地址用的） */
extern const char *Wireless_RF_Card_Data_Buffer_Pointer;


/* 函数声明 */
void RF_Card_Init(void);
void RF_Card_Screen(INSTRU_SENSOR *sensor_t);

void RF_Card_Request_Process_TC(void);
void RF_Card_Request_Process_HT(void);

void RF_Card_Trans_Sensor_Data(float *data_buffer);
void RF_Card_Trans_Power_Data(u8 *power_data_buffer);

BOOL RF_Card_Get_Power_State(u8 power_data);

void RF_Card_Rece_Data_State_Check(void);

float RF_Card_Get_Single_Tmp_Card_Data(float tmp_data);
float RF_Card_Get_Humiture_Tmp_Card_Data(float tmp_data);
float RF_Card_Get_Humiture_Hum_Card_Data(float hum_data);

#endif  
	 

