/**
  ******************************************************************************
  * @file              : Log_RF_Card.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015年4月12日 09:55
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
***/
#include "System_Config.h"

#if (Wireless_RF_Card_Config_Enable == 0x01)

#include "Log_Sensor.h"
#include "Log_RF_Card.h"
#include "Log_Adc1.h"

#include "Mcu_DMA.h"
#include "Mcu_Usart.h"
#include "Mcu_led.h"

#include "app.h"


#define Radioactive_Location 1 //放射源定位




/************************************************************************************************/
/* 无线RF卡片类型结构 */
typedef enum {
			  Null_RF_Card 			= 0,	//没有配置过的卡片
			  Single_TEM_RF_Card 	= 1,	//单温卡片
			  TEM_HUM_RF_Card 		= 2,	//温湿卡片
			 }Wireless_RF_Card_Type_Struct;
/************************************************************************************************/
/* 无线RF卡片接收数据寄存器 数据大小 */
#define Wireless_RF_Card_Rece_Data_Buffer_Size	(32u)
/* 无线RF卡片接收数据寄存器 */
	  char Wireless_RF_Card_Rece_Data_Buffer[Wireless_RF_Card_Rece_Data_Buffer_Size];
/* 无线RF卡片接收数据寄存器 指针（这里主要是为了给DMA付地址用的） */
const char *Wireless_RF_Card_Data_Buffer_Pointer = Wireless_RF_Card_Rece_Data_Buffer;
/************************************************************************************************/
/* 无线RF卡片接收数据状态寄存器结构 */
typedef enum {
			  NULL_Rece_Data 	= 0,	//无接收数据
			  HT_Rece_Data 		= 1,	//半完成接收数据
			  TC_Rece_Data 		= 2,	//完成接收数据
			 }Wireless_RF_Card_Rece_Data_SR_Struct;
/* 无线RF卡片接收数据状态寄存器 */
static Wireless_RF_Card_Rece_Data_SR_Struct Wireless_RF_Rece_Data_SR;
/************************************************************************************************/
/* 无线RF卡片数据寄存器大小 */
#define Wireless_RF_Card_Collection_Data_DR_Size	(32u)
/* 无线RF卡片数据寄存器 */
static float Wireless_RF_Card_Collection_Data_DR[Wireless_RF_Card_Collection_Data_DR_Size] = 
{
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	
};
/************************************************************************************************/		 
/* 无线RF卡片电池电量数据寄存器大小 */
#define Wireless_RF_Card_Power_DR_Size	(32u)
/* 无线RF卡片数据寄存器 */
static u8 	Wireless_RF_Power_Data_DR[Wireless_RF_Card_Power_DR_Size];
/************************************************************************************************/	 	 
/* 无线RF卡片数据缓存大小 */
#define Wireless_RF_Card_Data_Buffer_Size	(16u)
/* 无线RF卡片数据帧头 */
#define Wireless_RF_Card_Data_Frames_Head	(0X24)			 
/* 无线RF卡片数据结构 */
typedef union 
{
	struct			 
	{
		/* 数据 帧头 (这个是常数:0X24) */
		u8  Data_Frames_Head;
		/* 项目ID */	
		u8 	UID1;
		u8 	UID0;		
		/* 无线RF卡片ID */
		u8 ADS_ID_3;
		u8 ADS_ID_2;
		u8 ADS_ID_1;
		u8 ADS_ID_0;
		/* 保留状态 */
		u8  Reserve;
		/* 无线RF卡片的电池电压.(默认是ADC值,数据需要计算:VCC = (BV/256)*3.3，电压小于 2.5V为欠压。) */
		u8  Crad_Power;
		/* 湿度数据 */
		u8 HUM0;
		u8 HUM1;
		/* 温度数据 */
		u8 TMP0;
		u8 TMP1;		
		/* 发射序列号 */
		u8  TX_Serial_Num;
		/* 接收信号强度.(默认是HEX值，数据需要计算:信号强度 = -((~RSSI)+1)) */
		u8  RSSI;
		/* 校验.(其前面所有数据校验和) */
		u8  Check;
	}Reg;
	u8 Data_Buffer[Wireless_RF_Card_Data_Buffer_Size];
}Wireless_RF_Card_Data_Struct;
/************************************************************************************************/
/* 记录卡片接收数据的时间参数。 */
/* 记录数据数组的缓存大小。 */
#define Wireless_RF_Card_Data_Rece_Time_Buffer	(32u)
/* 记录数据数组 */
static u16 Wireless_RF_Card_Data_Rece_Time[Wireless_RF_Card_Data_Rece_Time_Buffer];
/************************************************************************************************/	 	


/************************************************************************************************/
/* 函数名	: Wireless_RF_Card_Init_Congif_Delay										 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置卡片接收模块的延时函数						 									*/
/* 创建日期	: 2015/04/16																		*/
/************************************************************************************************/
static void Wireless_RF_Card_Init_Congif_Delay(u32 cnt)
{
	while(cnt)
	{
		cnt--;
	}
}

/************************************************************************************************/
/* 函数名	: Wireless_RF_Card_GPIO_Config												 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置无线卡片模块的控制引脚						 									*/
/* 创建日期	: 2015/04/12																		*/
/************************************************************************************************/
static void Wireless_RF_Card_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RF_Card_Power_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= RF_Card_Power_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	/*
		PC13，PC14和PC15引脚通过电源开关进行供电，而这个电源开关只能够吸收有限的电流(3mA)。因此这三个引
		脚作为输出引脚时有以下限制：在同一时间只有一个引脚能作为输出，作为输出脚时只能工作在2MHz模式下，
		最大驱动负载为30pF，并且不能作为电流源(如驱动LED)。
	*/	
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Init(RF_Card_Power_PORT, &GPIO_InitStructure);
}


/************************************************************************************************/
/* 函数名	: RF_Card_Init														 	 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置无线温湿度卡片								 									*/
/* 创建日期	: 2015/04/12																		*/
/* 修改日期	: 2015/11/11																		*/
/************************************************************************************************/
void RF_Card_Init(void)
{
/* 模块通信的波特率:38400(默认值) */
#define	RF_Card_Define_BaudRate		(38400u)


	/* 配置模块的控制引脚 */
	Wireless_RF_Card_GPIO_Config();
	
	//0、关 模块电源
	Close_RF_Card_Power;
	/* 保证模块彻底断电，20ms */
	Wireless_RF_Card_Init_Congif_Delay(0X20000);	

	//1、配置数据接方式（使用DMA从串口寄存器接收数据）
	DMA_Rece_USART2_Data_Config();

	//2、配置模块的通信端口
	USART2_Config(RF_Card_Define_BaudRate);
	/* 设置 USART2 DMA 功能 */
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	/* 使能串口功能 */
	USART_Cmd(USART2 , ENABLE );		


	//3、开 模块电源
	Open_RF_Card_Power;


#undef	RF_Card_Define_BaudRate	

}


/************************************************************************************************/
/* 函数名	: RF_Card_Reset_Rece_Module											 	 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 复位接收模块									 									*/
/* 创建日期	: 2015/04/16																		*/
/************************************************************************************************/
static void RF_Card_Reset_Rece_Module(void)
{
	//0、关 模块电源
	Close_RF_Card_Power;	
	/* 保证模块彻底断电，20ms */
	Wireless_RF_Card_Init_Congif_Delay(0X35000);	
	
	//1、配置数据接方式（使用DMA从串口寄存器接收数据）
	DMA_Rece_USART2_Data_Config();

	//2、开 模块电源	
	Open_RF_Card_Power;
}


/************************************************************************************************/
/* 函数名	: RF_Card_Request_Process_TC												 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 仪器接收一组RF卡片数据完成，请求处理数据。（DMA接收数据完成 中断处理函数）					*/
/* 创建日期	: 2015/04/15																		*/
/************************************************************************************************/
void RF_Card_Request_Process_TC(void)
{
	
	/* 设置 DMA 接收数据半完成标志 */
	Wireless_RF_Rece_Data_SR = TC_Rece_Data;
	
	/* 发送 RF卡片的 信号量 */
	OSSemPost(RF_Card_QUE_Sem);

}

/************************************************************************************************/
/* 函数名	: RF_Card_Request_Process_HT												 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 仪器接收一组RF卡片数据完成，请求处理数据。（DMA接收数据半 中断处理函数）					*/
/* 创建日期	: 2015/04/15																		*/
/************************************************************************************************/
void RF_Card_Request_Process_HT(void)
{

	/* 设置 DMA 接收数据半完成标志 */
	Wireless_RF_Rece_Data_SR = HT_Rece_Data;
	
	/* 发送 RF卡片的 信号量 */
	OSSemPost(RF_Card_QUE_Sem);

}


/************************************************************************************************/
/* 函数名	: RF_Card_Screen															 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 筛选RF卡片数据									 									*/
/* 创建日期	: 2015/04/12																		*/
/************************************************************************************************/
void RF_Card_Screen(INSTRU_SENSOR *sensor_t)
{

	/* 无线RF卡片数据寄存器指针 */
	Wireless_RF_Card_Data_Struct *rf_card_data_reg;
	/* 计数器 */
	u8 num;
	/* 数据校验 */
	u8 check;
	/* 无线RF卡片 ID 指针 */
	u32 rf_card_id;
	
	/* 温度数据 */
	float	tempdata;
	/* 湿度数据 */
	float	humidata;

	
	//1、获取 无线RF卡片接收数据寄存器 数据
	if(Wireless_RF_Rece_Data_SR == HT_Rece_Data)
	{
		rf_card_data_reg = (Wireless_RF_Card_Data_Struct*)Wireless_RF_Card_Rece_Data_Buffer;
	}else if(Wireless_RF_Rece_Data_SR == TC_Rece_Data)
	{
		rf_card_data_reg = (Wireless_RF_Card_Data_Struct*)&Wireless_RF_Card_Rece_Data_Buffer[Wireless_RF_Card_Data_Buffer_Size];

	}else
	{
		__nop();
		return;
	}
	
	/* 清除 无线RF卡片 接收数据状态 */
	Wireless_RF_Rece_Data_SR = NULL_Rece_Data;
	//2、数据数据合法性检测:
	/* 
		a、帧头;
		b、校验和;
	*/
	if(rf_card_data_reg->Reg.Data_Frames_Head != Wireless_RF_Card_Data_Frames_Head)
	{
		/* 
			假如头帧不是 0X24 这个就是比较严重的问题，有可能是数据错位的结果，必须计时修正，
			否则，就收不到完整的数据。（等于收不到数据）
		*/
		/* 复位模块 */
		RF_Card_Reset_Rece_Module();
		return;	
	}
	
	
	for(num = 0,check = 0;num < (Wireless_RF_Card_Data_Buffer_Size-1) ;num++)
	{
		/* 求和校验 */
		check += rf_card_data_reg->Data_Buffer[num];
	}
	
	if(check != rf_card_data_reg->Reg.Check)
	{
		/* 校验 错误 */
		__nop();
		return;
	}
	/* 暂时只有这两个校验，如果有需要可以在这个后面继续添加 */
	
	//3、筛选ID匹配的数据(这里我们只对卡片ID进行匹配操作)
	/* 获取卡片ID */
	rf_card_id = ((((u32)rf_card_data_reg->Reg.ADS_ID_3) << 24) |
				  (((u32)rf_card_data_reg->Reg.ADS_ID_2) << 16) |
				  (((u32)rf_card_data_reg->Reg.ADS_ID_1) << 8)  |
				   ((u32)rf_card_data_reg->Reg.ADS_ID_0));
	
	/* 判断当前卡片是单温还是温湿的 */// 值:1750等于单温卡片的最高温度125℃
	if((rf_card_data_reg->Reg.HUM0 == 0x00)&&
	   (rf_card_data_reg->Reg.HUM1 == 0x00)&&
	   (((((s16)rf_card_data_reg->Reg.TMP0)<<8)+rf_card_data_reg->Reg.TMP1) < 1750))
	{
		/* 湿度数据全部为0说明当前卡片为单温 */
		tempdata = (((float)((((s16)rf_card_data_reg->Reg.TMP0)<<8)+rf_card_data_reg->Reg.TMP1)-500.0)/10.0);
		humidata = Wireless_RF_Card_Data_Rece_Timeout_Para;
	}else
	{
		/* 当前卡片温湿卡片 */
		tempdata = (((float)((((s16)rf_card_data_reg->Reg.TMP0)<<8)+rf_card_data_reg->Reg.TMP1)*175.72)/65536.0-46.85);
		humidata = ((((float)((((s16)rf_card_data_reg->Reg.HUM0)<<8)+rf_card_data_reg->Reg.HUM1)*125.0)/65536.0)-6);
	}
	
/* 判断 传感器功能是否配置 */
#if (Wireless_RF_Card_Config_Enable == 0x01)

/* 判断是否有温湿度卡片 */
#if (0 == Sensor_RF_Humiture_Num)
	/* 没有 */
	(void)humidata;
#endif
/* 判断 通道1 */
#if	((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH1.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1] 	= tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_1] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_1] = 0;
	
	}
#elif	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH1.SENSOR_RF_Card_ID_NO)
	{
		/* 获取湿度数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_1] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_1] = 0;
	}
#endif	
	
/* 判断 通道2 */
#if	((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH2.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_2] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_2] = 0;	
	}
#elif	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH2.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_2] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_2] = 0;	
	}
#endif
	
/* 判断 通道3 */
#if	((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH3.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_3] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_3] = 0;
	}
#elif	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH3.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_3] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_3] = 0;
	}
#endif			
	
/* 判断 通道4 */
#if	((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH4.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_4] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_4] = 0;
	}
#elif	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH4.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_4] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_4] = 0;
	}	
#endif			
	
/* 判断 通道5 */
#if	((Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH5.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_5] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_5] = 0;
	}
#elif	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH5.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_5] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_5] = 0;
	}	
#endif			
	
/* 判断 通道6 */
#if	((Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH6.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_6] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_6] = 0;
	}
#elif	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH6.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_6] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_6] = 0;
	}	
#endif			
	
/* 判断 通道7 */
#if	((Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH7.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_7] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_7] = 0;
	}
#elif	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH7.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_7] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_7] = 0;
	}	
#endif			
	
/* 判断 通道8 */
#if	((Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH8.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_8] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_8] = 0;
	}
#elif	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH8.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_8] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_8] = 0;
	}	
#endif			
	
	/* 判断 通道9 */
#if	((Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH9.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_9] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_9] = 0;
	}
#elif	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH9.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_9] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_9] = 0;
	}	
#endif	
	
	/* 判断 通道10 */
#if	((Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH10.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_10] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_10] = 0;
	}
#elif	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH10.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_10] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_10] = 0;
	}	
#endif	
	
	/* 判断 通道11 */
#if	((Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH11.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_11] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_11] = 0;
	}
#elif	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH11.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_11] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_11] = 0;
	}	
#endif	
	
	/* 判断 通道12 */
#if	((Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH12.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_12] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_12] = 0;
	}
#elif	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH12.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_12] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_12] = 0;
	}	
#endif	
	
	/* 判断 通道13 */
#if	((Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH13.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_13] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_13] = 0;
	}
#elif	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH13.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_13] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_13] = 0;
	}	
#endif	
	
	/* 判断 通道14 */
#if	((Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH14.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_14] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_14] = 0;
	}
#elif	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH14.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_14] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_14] = 0;
	}	
#endif	
	
	/* 判断 通道15 */
#if	((Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH15.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_15] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_15] = 0;
	}
#elif	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH15.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_15] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_15] = 0;
	}	
#endif	
	
	/* 判断 通道16 */
#if	((Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH16.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_16] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_16] = 0;
	}
#elif	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH16.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_16] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_16] = 0;
	}	
#endif	
	
	/* 判断 通道17 */
#if	((Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH17.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_17] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_17] = 0;
	}
#elif	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH17.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_17] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_17] = 0;
	}	
#endif	
	
	/* 判断 通道18 */
#if	((Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH18.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_18] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_18] = 0;
	}
#elif	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH18.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_18] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_18] = 0;
	}	
#endif	
	
	/* 判断 通道19 */
#if	((Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH19.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_19] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_19] = 0;
	}
#elif	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH19.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_19] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_19] = 0;
	}	
#endif	
	
	/* 判断 通道20 */
#if	((Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH20.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_20] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_20] = 0;
	}
#elif	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH20.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_20] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_20] = 0;
	}	
#endif	
	
	/* 判断 通道21 */
#if	((Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH21.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_21] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_21] = 0;
	}
#elif	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH21.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_21] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_21] = 0;
	}	
#endif	
	
	/* 判断 通道22 */
#if	((Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH22.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_22] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_22] = 0;
	}
#elif	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH22.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_22] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_22] = 0;
	}	
#endif	
	
	/* 判断 通道23 */
#if	((Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH23.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_23] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_23] = 0;
	}
#elif	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH23.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_23] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_23] = 0;
	}	
#endif	
	
	/* 判断 通道24 */
#if	((Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH24.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_24] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_24] = 0;
	}
#elif	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH24.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_24] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_24] = 0;
	}	
#endif	
	
	/* 判断 通道25 */
#if	((Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH25.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_25] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_25] = 0;
	}
#elif	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH25.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_25] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_25] = 0;
	}	
#endif	
	
	
	/* 判断 通道26 */
#if	((Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH26.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_26] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_26] = 0;
	}
#elif	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH26.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_26] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_26] = 0;
	}	
#endif	
	
	/* 判断 通道27 */
#if	((Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH27.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_27] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_27] = 0;
	}
#elif	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH27.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_27] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_27] = 0;
	}	
#endif	
	
	/* 判断 通道28 */
#if	((Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH28.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_28] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_28] = 0;
	}
#elif	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH28.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_28] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_28] = 0;
	}	
#endif	
	
	/* 判断 通道29 */
#if	((Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH29.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_29] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_29] = 0;
	}
#elif	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH29.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_29] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_29] = 0;
	}	
#endif	
	
	/* 判断 通道30 */
#if	((Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH30.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_30] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_30] = 0;
	}
#elif	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH30.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_30] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_30] = 0;
	}	
#endif	
	
	/* 判断 通道31 */
#if	((Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH31.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_31] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_31] = 0;
	}
#elif	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH31.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_31] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_31] = 0;
	}	
#endif	
	
	/* 判断 通道32 */
#if	((Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* 判断当前的卡片是否为配置的卡片 */
	if(rf_card_id == sensor_t->Inf_State.CH32.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32] = tempdata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_32] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_32] = 0;
	}
#elif	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH32.SENSOR_RF_Card_ID_NO)
	{
		/* 获取单温数据 */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32] 	= humidata;
		/* 获取电池电量数据 */
		Wireless_RF_Power_Data_DR[Sensors_CH_32] = rf_card_data_reg->Reg.Crad_Power;
		/* 清除卡片超时时间 */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_32] = 0;
	}	
#endif	
	
	
#endif		
	
return;
}


/************************************************************************************************/
/* 函数名	: RF_Card_Trans_Sensor_Data														 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 将采集到的温湿度数据转移指定的寄存器中			 										*/
/* 创建日期	: 2015/04/16																		*/
/* 修改日期	: 2015/08/18																		*/
/************************************************************************************************/
void RF_Card_Trans_Sensor_Data(float *data_buffer)
{

/* 判断 通道1 */
#if		Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_1) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_1) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_1) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1];		
#endif	
	
/* 判断 通道2 */
#if		Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_2) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_2) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_2) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2];	
#endif	
	
/* 判断 通道3 */
#if		Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_3) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_3) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_3) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3];		
#endif	
	
/* 判断 通道4 */
#if		Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_4) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_4) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_4) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4];	
#endif	
	
/* 判断 通道5 */
#if		Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_5) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_5) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_5) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5];	
#endif	
	
/* 判断 通道6 */
#if		Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_6) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_6) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_6) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6];		
#endif	
	
/* 判断 通道7 */
#if		Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_7) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_7) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_7) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7];		
#endif	
	
/* 判断 通道8 */
#if		Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_8) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_8) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_8) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8];
#endif	

/* 判断 通道9 */
#if		Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_9) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9];
#elif 	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_9) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9];
#elif 	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_9) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9];
#endif	

/* 判断 通道10 */
#if		Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_10) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10];
#elif 	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_10) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10];
#elif 	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_10) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10];
#endif	

/* 判断 通道11 */
#if		Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_11) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11];
#elif 	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_11) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11];
#elif 	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_11) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11];
#endif	

/* 判断 通道12 */
#if		Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_12) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12];
#elif 	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_12) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12];
#elif 	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_12) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12];
#endif	

/* 判断 通道13 */
#if		Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_13) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13];
#elif 	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_13) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13];
#elif 	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_13) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13];
#endif	

/* 判断 通道14 */
#if		Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_14) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14];
#elif 	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_14) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14];
#elif 	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_14) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14];
#endif	

/* 判断 通道15 */
#if		Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_15) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15];
#elif 	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_15) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15];
#elif 	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_15) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15];
#endif	

/* 判断 通道16 */
#if		Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_16) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16];
#elif 	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_16) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16];
#elif 	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_16) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16];
#endif	

/* 判断 通道17 */
#if		Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_17) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17];
#elif 	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_17) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17];
#elif 	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_17) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17];
#endif	

/* 判断 通道18 */
#if		Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_18) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18];
#elif 	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_18) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18];
#elif 	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_18) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18];
#endif	

/* 判断 通道19 */
#if		Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_19) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19];
#elif 	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_19) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19];
#elif 	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_19) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19];
#endif	

/* 判断 通道20 */
#if		Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_20) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20];
#elif 	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_20) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20];
#elif 	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_20) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20];
#endif	

/* 判断 通道21 */
#if		Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_21) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21];
#elif 	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_21) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21];
#elif 	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_21) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21];
#endif	

/* 判断 通道22 */
#if		Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_22) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22];
#elif 	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_22) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22];
#elif 	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_22) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22];
#endif	

/* 判断 通道23 */
#if		Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_23) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23];
#elif 	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_23) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23];
#elif 	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_23) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23];
#endif	

/* 判断 通道24 */
#if		Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_24) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24];
#elif 	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_24) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24];
#elif 	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_24) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24];
#endif	

/* 判断 通道25 */
#if		Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_25) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25];
#elif 	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_25) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25];
#elif 	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_25) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25];
#endif	

/* 判断 通道26 */
#if		Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_26) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26];
#elif 	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_26) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26];
#elif 	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_26) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26];
#endif	

/* 判断 通道27 */
#if		Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_27) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27];
#elif 	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_27) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27];
#elif 	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_27) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27];
#endif	

/* 判断 通道28 */
#if		Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_28) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28];
#elif 	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_28) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28];
#elif 	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_28) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28];
#endif	

/* 判断 通道29 */
#if		Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_29) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29];
#elif 	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_29) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29];
#elif 	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_29) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29];
#endif	

/* 判断 通道30 */
#if		Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_30) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30];
#elif 	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_30) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30];
#elif 	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_30) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30];
#endif	

/* 判断 通道31 */
#if		Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_31) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31];
#elif 	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_31) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31];
#elif 	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_31) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31];
#endif	

/* 判断 通道32 */
#if		Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_32) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32];
#elif 	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_32) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32];
#elif 	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_32) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32];
#endif	

}

/************************************************************************************************/
/* 函数名	: RF_Card_Trans_Power_Data													 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 将采集到的卡片电量据转移指定的寄存器中			 										*/
/* 创建日期	: 2015/04/16																		*/
/************************************************************************************************/
void RF_Card_Trans_Power_Data(u8 *power_data_buffer)
{
	
/* 判断 通道1 */
#if		Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_1) 		= Wireless_RF_Power_Data_DR[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_1) 		= Wireless_RF_Power_Data_DR[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_1) 		= Wireless_RF_Power_Data_DR[Sensors_CH_1];		
#endif	
	
/* 判断 通道2 */
#if		Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_2) 		= Wireless_RF_Power_Data_DR[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_2) 		= Wireless_RF_Power_Data_DR[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_2) 		= Wireless_RF_Power_Data_DR[Sensors_CH_2];	
#endif	
	
/* 判断 通道3 */
#if		Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_3) 		= Wireless_RF_Power_Data_DR[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_3) 		= Wireless_RF_Power_Data_DR[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_3) 		= Wireless_RF_Power_Data_DR[Sensors_CH_3];		
#endif	
	
/* 判断 通道4 */
#if		Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_4) 		= Wireless_RF_Power_Data_DR[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_4) 		= Wireless_RF_Power_Data_DR[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_4) 		= Wireless_RF_Power_Data_DR[Sensors_CH_4];	
#endif	
	
/* 判断 通道5 */
#if		Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_5) 		= Wireless_RF_Power_Data_DR[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_5) 		= Wireless_RF_Power_Data_DR[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_5) 		= Wireless_RF_Power_Data_DR[Sensors_CH_5];	
#endif	
	
/* 判断 通道6 */
#if		Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_6) 		= Wireless_RF_Power_Data_DR[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_6) 		= Wireless_RF_Power_Data_DR[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_6) 		= Wireless_RF_Power_Data_DR[Sensors_CH_6];		
#endif	
	
/* 判断 通道7 */
#if		Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_7) 		= Wireless_RF_Power_Data_DR[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_7) 		= Wireless_RF_Power_Data_DR[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_7) 		= Wireless_RF_Power_Data_DR[Sensors_CH_7];		
#endif	
	
/* 判断 通道8 */
#if		Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_8) 		= Wireless_RF_Power_Data_DR[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_8) 		= Wireless_RF_Power_Data_DR[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_8) 		= Wireless_RF_Power_Data_DR[Sensors_CH_8];
#endif	

/* 判断 通道9 */
#if		Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_9) 		= Wireless_RF_Power_Data_DR[Sensors_CH_9];
#elif 	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_9) 		= Wireless_RF_Power_Data_DR[Sensors_CH_9];
#elif 	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_9) 		= Wireless_RF_Power_Data_DR[Sensors_CH_9];
#endif	

/* 判断 通道10 */
#if		Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_10) 		= Wireless_RF_Power_Data_DR[Sensors_CH_10];
#elif 	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_10) 		= Wireless_RF_Power_Data_DR[Sensors_CH_10];
#elif 	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_10) 		= Wireless_RF_Power_Data_DR[Sensors_CH_10];
#endif	

/* 判断 通道11 */
#if		Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_11) 		= Wireless_RF_Power_Data_DR[Sensors_CH_11];
#elif 	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_11) 		= Wireless_RF_Power_Data_DR[Sensors_CH_11];
#elif 	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_11) 		= Wireless_RF_Power_Data_DR[Sensors_CH_11];
#endif	

/* 判断 通道12 */
#if		Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_12) 		= Wireless_RF_Power_Data_DR[Sensors_CH_12];
#elif 	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_12) 		= Wireless_RF_Power_Data_DR[Sensors_CH_12];
#elif 	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_12) 		= Wireless_RF_Power_Data_DR[Sensors_CH_12];
#endif	

/* 判断 通道13 */
#if		Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_13) 		= Wireless_RF_Power_Data_DR[Sensors_CH_13];
#elif 	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_13) 		= Wireless_RF_Power_Data_DR[Sensors_CH_13];
#elif 	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_13) 		= Wireless_RF_Power_Data_DR[Sensors_CH_13];
#endif	

/* 判断 通道14 */
#if		Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_14) 		= Wireless_RF_Power_Data_DR[Sensors_CH_14];
#elif 	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_14) 		= Wireless_RF_Power_Data_DR[Sensors_CH_14];
#elif 	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_14) 		= Wireless_RF_Power_Data_DR[Sensors_CH_14];
#endif	

/* 判断 通道15 */
#if		Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_15) 		= Wireless_RF_Power_Data_DR[Sensors_CH_15];
#elif 	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_15) 		= Wireless_RF_Power_Data_DR[Sensors_CH_15];
#elif 	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_15) 		= Wireless_RF_Power_Data_DR[Sensors_CH_15];
#endif	

/* 判断 通道16 */
#if		Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_16) 		= Wireless_RF_Power_Data_DR[Sensors_CH_16];
#elif 	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_16) 		= Wireless_RF_Power_Data_DR[Sensors_CH_16];
#elif 	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_16) 		= Wireless_RF_Power_Data_DR[Sensors_CH_16];
#endif	

/* 判断 通道17 */
#if		Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_17) 		= Wireless_RF_Power_Data_DR[Sensors_CH_17];
#elif 	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_17) 		= Wireless_RF_Power_Data_DR[Sensors_CH_17];
#elif 	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_17) 		= Wireless_RF_Power_Data_DR[Sensors_CH_17];
#endif	

/* 判断 通道18 */
#if		Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_18) 		= Wireless_RF_Power_Data_DR[Sensors_CH_18];
#elif 	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_18) 		= Wireless_RF_Power_Data_DR[Sensors_CH_18];
#elif 	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_18) 		= Wireless_RF_Power_Data_DR[Sensors_CH_18];
#endif	

/* 判断 通道19 */
#if		Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_19) 		= Wireless_RF_Power_Data_DR[Sensors_CH_19];
#elif 	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_19) 		= Wireless_RF_Power_Data_DR[Sensors_CH_19];
#elif 	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_19) 		= Wireless_RF_Power_Data_DR[Sensors_CH_19];
#endif	

/* 判断 通道20 */
#if		Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_20) 		= Wireless_RF_Power_Data_DR[Sensors_CH_20];
#elif 	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_20) 		= Wireless_RF_Power_Data_DR[Sensors_CH_20];
#elif 	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_20) 		= Wireless_RF_Power_Data_DR[Sensors_CH_20];
#endif	

/* 判断 通道21 */
#if		Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_21) 		= Wireless_RF_Power_Data_DR[Sensors_CH_21];
#elif 	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_21) 		= Wireless_RF_Power_Data_DR[Sensors_CH_21];
#elif 	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_21) 		= Wireless_RF_Power_Data_DR[Sensors_CH_21];
#endif	

/* 判断 通道22 */
#if		Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_22) 		= Wireless_RF_Power_Data_DR[Sensors_CH_22];
#elif 	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_22) 		= Wireless_RF_Power_Data_DR[Sensors_CH_22];
#elif 	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_22) 		= Wireless_RF_Power_Data_DR[Sensors_CH_22];
#endif	

/* 判断 通道23 */
#if		Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_23) 		= Wireless_RF_Power_Data_DR[Sensors_CH_23];
#elif 	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_23) 		= Wireless_RF_Power_Data_DR[Sensors_CH_23];
#elif 	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_23) 		= Wireless_RF_Power_Data_DR[Sensors_CH_23];
#endif	

/* 判断 通道24 */
#if		Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_24) 		= Wireless_RF_Power_Data_DR[Sensors_CH_24];
#elif 	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_24) 		= Wireless_RF_Power_Data_DR[Sensors_CH_24];
#elif 	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_24) 		= Wireless_RF_Power_Data_DR[Sensors_CH_24];
#endif	

/* 判断 通道25 */
#if		Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_25) 		= Wireless_RF_Power_Data_DR[Sensors_CH_25];
#elif 	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_25) 		= Wireless_RF_Power_Data_DR[Sensors_CH_25];
#elif 	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_25) 		= Wireless_RF_Power_Data_DR[Sensors_CH_25];
#endif	

/* 判断 通道26 */
#if		Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_26) 		= Wireless_RF_Power_Data_DR[Sensors_CH_26];
#elif 	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_26) 		= Wireless_RF_Power_Data_DR[Sensors_CH_26];
#elif 	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_26) 		= Wireless_RF_Power_Data_DR[Sensors_CH_26];
#endif	

/* 判断 通道27 */
#if		Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_27) 		= Wireless_RF_Power_Data_DR[Sensors_CH_27];
#elif 	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_27) 		= Wireless_RF_Power_Data_DR[Sensors_CH_27];
#elif 	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_27) 		= Wireless_RF_Power_Data_DR[Sensors_CH_27];
#endif	

/* 判断 通道28 */
#if		Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_28) 		= Wireless_RF_Power_Data_DR[Sensors_CH_28];
#elif 	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_28) 		= Wireless_RF_Power_Data_DR[Sensors_CH_28];
#elif 	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_28) 		= Wireless_RF_Power_Data_DR[Sensors_CH_28];
#endif	

/* 判断 通道29 */
#if		Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_29) 		= Wireless_RF_Power_Data_DR[Sensors_CH_29];
#elif 	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_29) 		= Wireless_RF_Power_Data_DR[Sensors_CH_29];
#elif 	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_29) 		= Wireless_RF_Power_Data_DR[Sensors_CH_29];
#endif	

/* 判断 通道30 */
#if		Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_30) 		= Wireless_RF_Power_Data_DR[Sensors_CH_30];
#elif 	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_30) 		= Wireless_RF_Power_Data_DR[Sensors_CH_30];
#elif 	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_30) 		= Wireless_RF_Power_Data_DR[Sensors_CH_30];
#endif	

/* 判断 通道31 */
#if		Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_31) 		= Wireless_RF_Power_Data_DR[Sensors_CH_31];
#elif 	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_31) 		= Wireless_RF_Power_Data_DR[Sensors_CH_31];
#elif 	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_31) 		= Wireless_RF_Power_Data_DR[Sensors_CH_31];
#endif	

/* 判断 通道32 */
#if		Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_32) 		= Wireless_RF_Power_Data_DR[Sensors_CH_32];
#elif 	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_32) 		= Wireless_RF_Power_Data_DR[Sensors_CH_32];
#elif 	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_32) 		= Wireless_RF_Power_Data_DR[Sensors_CH_32];
#endif	

}

/************************************************************************************************/
/* 函数名	: RF_Card_Get_Power_State													 	 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 根据给定的电量数据计算出当前卡片电量状态（BOOL：FLASH欠压，TRUE电量正常）			 		*/
/* 注意	 	: 现在的单温和温湿度卡片计算卡片电量的算法相同，当时有其他的无线仪器电量算法是不同的	 		*/
/* 电量算法	: VCC= (BV/256)*3.3，电压小于 2.5V为欠压.	 											*/
/* 创建日期	: 2015/05/21																		*/
/************************************************************************************************/
BOOL RF_Card_Get_Power_State(u8 power_data)
{
/* 卡片的基准电压 */
#define  RF_Card_VREF	(3.30)
/* 卡片的分压比例 */
#define  RF_Card_B		(256u)
/* 卡片电量状态阈值 */
#define  RF_Card_Power_State_HT	(2.50)
	
	/* 计算电量状态 */
	return((((float)power_data*RF_Card_VREF)/RF_Card_B)>RF_Card_Power_State_HT)?TRUE:FALSE;
	
#undef  RF_Card_VREF
#undef  RF_Card_B
#undef	RF_Card_Power_State_HT
}

/************************************************************************************************/
/* 函数名	: RF_Card_Rece_Data_State_Check													 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 检测设置卡片通道的数据接收是否超出设定时间，假如超出设置数据为ERR。				 		*/
/* 创建日期	: 2015/05/25																		*/
/************************************************************************************************/
void RF_Card_Rece_Data_State_Check(void)
{
/* 判断 传感器功能是否配置 */
#if (Wireless_RF_Card_Config_Enable == 0x01)

	/* 判断 通道1 */
	#if		(Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道1 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_1] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_1]++;
		}
	#endif		

	/* 判断 通道2 */
	#if		(Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道2 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_2] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_2]++;
		}
	#endif	

	/* 判断 通道3 */
	#if		(Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道3 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_3] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_3]++;
		}
	#endif	

	/* 判断 通道4 */
	#if		(Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道4 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_4] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_4]++;
		}
	#endif			

	/* 判断 通道5 */
	#if		(Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道5 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_5] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_5]++;
		}
	#endif		

	/* 判断 通道6 */
	#if		(Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道6 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_6] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_6]++;
		}
	#endif	

	/* 判断 通道7 */
	#if		(Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道7 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_7] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_7]++;
		}
	#endif	

	/* 判断 通道8 */
	#if		(Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道8 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_8] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_8]++;
		}
	#endif		

	/* 判断 通道9 */
	#if		(Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道9 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_9] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_9]++;
		}
	#endif		

	/* 判断 通道10 */
	#if		(Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道10 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_10] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_10]++;
		}
	#endif	

	/* 判断 通道11 */
	#if		(Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道11 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_11] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_11]++;
		}
	#endif	

	/* 判断 通道12 */
	#if		(Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道12 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_12] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_12]++;
		}
	#endif		

	/* 判断 通道13 */
	#if		(Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道13 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_13] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_13]++;
		}
	#endif		

	/* 判断 通道14 */
	#if		(Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道14 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_14] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_14]++;
		}
	#endif	

	/* 判断 通道15 */
	#if		(Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道15 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_15] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_15]++;
		}
	#endif	

	/* 判断 通道16 */
	#if		(Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道16 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_16] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_16]++;
		}
	#endif		
		
	/* 判断 通道17 */
	#if		(Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道17 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_17] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_17]++;
		}
	#endif		

	/* 判断 通道18 */
	#if		(Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道18 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_18] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_18]++;
		}
	#endif	

	/* 判断 通道19 */
	#if		(Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道19 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_19] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_19]++;
		}
	#endif	

	/* 判断 通道20 */
	#if		(Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道20 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_20] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_20]++;
		}
	#endif			

	/* 判断 通道21 */
	#if		(Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道21 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_21] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_21]++;
		}
	#endif		

	/* 判断 通道22 */
	#if		(Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道22 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_22] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_22]++;
		}
	#endif	

	/* 判断 通道23 */
	#if		(Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道23 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_23] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_23]++;
		}
	#endif	

	/* 判断 通道24 */
	#if		(Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道24 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_24] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_24]++;
		}
	#endif		

	/* 判断 通道25 */
	#if		(Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道25 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_25] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_25]++;
		}
	#endif		

	/* 判断 通道26 */
	#if		(Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道26 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_26] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_26]++;
		}
	#endif	

	/* 判断 通道27 */
	#if		(Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道27 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_27] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_27]++;
		}
	#endif	

	/* 判断 通道28 */
	#if		(Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道28 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_28] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_28]++;
		}
	#endif		

	/* 判断 通道29 */
	#if		(Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道29 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_29] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_29]++;
		}
	#endif		

	/* 判断 通道30 */
	#if		(Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道30 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_30] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_30]++;
		}
	#endif	

	/* 判断 通道31 */
	#if		(Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道31 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_31] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_31]++;
		}
	#endif	

	/* 判断 通道32 */
	#if		(Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 判断 通道32 是否超时  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_32] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* 接收卡片数据以超时，设置对应通道的数据为超时数据 */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* 接收卡片数据未超时，时间继续累加 */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_32]++;
		}
	#endif			
		

	
#endif
}

/*************************************************************************************************/
/* 函数名	:RF_Card_Get_Single_Tmp_Card_Data													 */
/* 输入		:单温卡片的温度数据值																	 */
/* 输出		:单温卡片的温度采样值																	 */
/* 作用		:将单温卡片的温度数据转换成实际采样值												 	 */
/* 创建日期	:2015/08/14																			 */
/*************************************************************************************************/
float RF_Card_Get_Single_Tmp_Card_Data(float tmp_data)
{
/* 单温卡片温度测量范围（最大值：80℃），但我们要求的的数据范围要大的多（1000℃）。 */
#define Wireless_RF_Card_Single_Tmp_Data_MAX_Para	(10500u)
/* 卡片数据超出设定范围时上传到服务器的数据值 */
#define Wireless_RF_Card_Single_Tmp_Data_MAX_Value	(1000.0)



	/* 判断单温卡片温度数据值 */
	if(tmp_data == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* 数据值等于数据超时值，设置实际温度值为一个特定的参数，提供上位机软件识别。 */
		__nop();
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	//现在特殊的参数只有一个 超时数据（功能就是说明:在指定的一段时间内没有收到对应卡片的数据）。
	/* 判断数据是否在正常的量程范围内 */
	else if(tmp_data < Wireless_RF_Card_Single_Tmp_Data_MAX_Para)
	{
		/* 在温度量程范围内 */
		__nop();
		return (((float)tmp_data-500.0)/10.0);
	}else
	{
		/* 错误数据，单温卡片数据异常(数据大于等于最大值) */
		__nop();
		return Wireless_RF_Card_Single_Tmp_Data_MAX_Value;
	}
	
#undef Wireless_RF_Card_Single_Tmp_Data_MAX_Para
#undef Wireless_RF_Card_Single_Tmp_Data_MAX_Value
}

/*************************************************************************************************/
/* 函数名	:RF_Card_Get_Humiture_Tmp_Card_Data												 	 */
/* 输入		:温湿卡片的温度数据值																	 */
/* 输出		:温湿卡片的温度采样值																	 */
/* 作用		:将温湿卡片的温度数据转换成实际采样值												 	 */
/* 创建日期	:2015/05/25																			 */
/*************************************************************************************************/
float  RF_Card_Get_Humiture_Tmp_Card_Data(float tmp_data)
{
/* 
	温湿卡片温度测量范围（最大值：80℃），但我们要求的的数据范围要大的多（1000℃）,但是在理论上2字节的tmp_data数据最大为65535，
	能够测量的温度范围也就只能达到128.8℃。	
*/
	/* 单温卡片温度测量范围（最大值：80℃），但我们要求的的数据范围要大的多（1000℃）。 */
#define Wireless_RF_Card_Humiture_Tmp_Data_MAX_Para		(65535u)
	/* 卡片数据超出设定范围时上传到服务器的数据值 */
#define Wireless_RF_Card_Humiture_Tmp_Data_MAX_Value	(1000.0)



	/* 判断单温卡片温度数据值	 */
	if(tmp_data == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* 数据值等于数据超时值，设置实际温度值为一个特定的参数，提供上位机软件识别。 */
		__nop();
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	//现在特殊的参数只有一个 超时数据（功能就是说明:在指定的一段时间内没有收到对应卡片的数据）。
	/* 判断数据是否在正常的量程范围内 */
	else if(tmp_data < Wireless_RF_Card_Humiture_Tmp_Data_MAX_Para)
	{
		/* 在温度量程范围内 */
		__nop();
		return (((float)tmp_data*175.72)/65536.0-46.85);
	}else
	{
		/* 错误数据，单温卡片数据异常(数据大于等于最大值) */
		__nop();
		return Wireless_RF_Card_Humiture_Tmp_Data_MAX_Value;
	}
	
#undef Wireless_RF_Card_Humiture_Tmp_Data_MAX_Para
#undef Wireless_RF_Card_Humiture_Tmp_Data_MAX_Value

}

/*************************************************************************************************/
/* 函数名	:RF_Card_Get_Humiture_Hum_Card_Data												 	 */
/* 输入		:温湿卡片的温度数据值																	 */
/* 输出		:温湿卡片的温度采样值																	 */
/* 作用		:将温湿卡片的湿度数据转换成实际采样值												 	 */
/* 创建日期	:2015/05/25																			 */
/*************************************************************************************************/
float  RF_Card_Get_Humiture_Hum_Card_Data(float hum_data)
{
/* 温湿卡片温度测量范围（最大值：100%） */
#define Wireless_RF_Card_Humiture_Hum_Data_MAX	(55575u)
/* 湿度的最大值 100% */
#define Hum_Data_Value_MAX	(100.0)

	/* 判断单湿卡片湿度数据值	 */
	if(hum_data == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* 数据值等于数据超时值，设置实际温度值为一个特定的参数，提供上位机软件识别。 */
		__nop();
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	//现在特殊的参数只有一个 超时数据（功能就是说明:在指定的一段时间内没有收到对应卡片的数据）。
	/* 判断数据是否在正常的量程范围内 */
	else if(hum_data <= Wireless_RF_Card_Humiture_Hum_Data_MAX)
	{
		/* 在温度量程范围内 */
		__nop();
		return ((((float)hum_data*125.0)/65536.0)-6);
	}else
	{
		/* 错误数据，温湿卡片数据异常(数据大于等于最大值) */
		__nop();
		return Hum_Data_Value_MAX;
	}
	
	
#undef Wireless_RF_Card_Humiture_Hum_Data_MAX
#undef Hum_Data_Value_MAX
}

#endif


/******************* (C) HZ Zeda Team *****END OF FILE****/
