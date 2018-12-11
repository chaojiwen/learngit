/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/10/24																			 */
/*************************************************************************************************/
#include "Log_Adc1.h"
#include "Log_Sensor.h"
#include "Log_Battery.h"
#include "Log_Hum_Cal.h"
#include "Log_Power.h"
#include "Log_Temp_Cal.h"

#include "Mcu_ADC.h"
#include "Mcu_Config.h"
#include "Mcu_Power.h"
#include "Mcu_Usart.h"	

#include "ucos_ii.h"

#include <string.h>
#include <stdlib.h>



/************************************************************************************************/
/* ADC采样的数据寄存器 */
static u16 	ADC_Collection_Data_Buffer[ADC_Collection_Data_Buffer_Size];
/* ADC 存储采集数据寄存器大小 */
#define		ADC_Store_Data_Buffer_Size			(8u)
/* ADC 存储采集数据寄存器 */
static u16 	ADC_Store_Data_Buffer[ADC_Collection_Data_Buffer_Size][ADC_Store_Data_Buffer_Size];
/* ADC 通过DMA方式传输的数据寄存器 */
	   u16 	ADC_DMA_Rece_DR[ADC_Collection_Data_Buffer_Size];
/************************************************************************************************/

static int 	Comp(const void *p1,const void *p2)
{
	return *((u16*)p2) >  *((u16*)p1)?1:-1; 
}

/* 对数据缓存取平均 */
static int ADC_Data_Avg(u16* array,u8 size)
{
	u8 num;
	u32 total=0;
	
	for(num = 0,total = 0;num < size; num ++)
	{
		total += array[num];
	}
	
	return total/size;
}

/*************************************************************************************************/
/* 函数名  ：ADC_Data_Buffer_Init														 	 	 */
/* 输入	  ：(void *p_arg) 																		 */
/* 输出	  : 无 																					 */
/* 作用	  ：启动ADC转换，获取ADC数据，整理数据，输出数据												 */
/* 创建日期：2015/1/05																			 */
/*************************************************************************************************/
void ADC_Data_Buffer_Init(void)
{
	u8 num;
	
	/* 设置ADC数据缓存 */
	for(num=0;num<ADC_Collection_Data_Buffer_Size;num++)
	{
		ADC_Get_Value();
	}
	
}
	
//ADC_To_Sensor_Value
/*************************************************************************************************/
/* 函数名  ：ADC_Get_Value																 	 	 */
/* 输入	  ：(void *p_arg) 																		 */
/* 输出	  : 无 																					 */
/* 作用	  ：启动ADC转换，获取ADC数据，整理数据，输出数据												 */
/* 创建日期：2015/1/05																			 */
/*************************************************************************************************/
void ADC_Get_Value(void)
{	
	static u8 adc_buffer_p = 0;
	u8 num;
	u16 array[ADC_Store_Data_Buffer_Size];
	
	//阶段1、开传感器电源，启动ADC转换，获得传感器通道的ADC数据。
	/* 开始ADC1转换 */
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	/* 等待 DMA 数据 转换 完成 *///这个过程在1MS以内
	do{
		OSTimeDly(1);
	/* 判断 DMA 数据是否转换完成。 */
	}while(DMA_GetFlagStatus(DMA1_FLAG_TC1) != SET);
	/* DMA 数据是否转换完成! */
	
	//阶段2 处理传感器通道的ADC数据。对数据进行排序，过滤掉异常数据，取整体数据的平均值。
	/* 将最新获得的ADC数据写入 缓存 */
	for(num = 0;num < ADC_Collection_Data_Buffer_Size;num ++)
	{
		/* 转移数据 */
		ADC_Store_Data_Buffer[num][adc_buffer_p] = ADC_DMA_Rece_DR[num];
	}

	/* 设置指针 */
	adc_buffer_p++;
	/* 判断指针越界 */
	if(adc_buffer_p >= ADC_Store_Data_Buffer_Size)
	{
		/* 指针越界,返回0点 */
		adc_buffer_p = 0;
	}
	
	/* 对每个数据通道数据进行排序（升序）。 */
	for(num = 0;num < ADC_Collection_Data_Buffer_Size;num ++)
	{
		/* 清除 数据 */
		memset(array,0x00,sizeof(array));
		
		/* 将数据缓存数据赋值到临时变量数组中 */
		memcpy(array,ADC_Store_Data_Buffer[num],sizeof(ADC_Store_Data_Buffer[num]));
		
		/* 排序 */
		qsort(array,ADC_Store_Data_Buffer_Size,sizeof(u16),Comp);
		
		/* 去掉最低2个数据、去掉最高2个数据，最后取平均。 */
		ADC_Collection_Data_Buffer[num] = ADC_Data_Avg(&array[1],(ADC_Store_Data_Buffer_Size-2));
	}
}

/*************************************************************************************************/
/* 函数名	：ADC_Trans_Data															 	 	 */
/* 输入		：data_buffer:数据缓存																 */
/* 输出		: 无 																				 */
/* 作用		：将采集到的ADC数据转移指定的寄存器中													 */
/* 创建日期	: 2015/04/20																		 */
/*************************************************************************************************/
void ADC_Trans_Data(float *data_buffer)
{
/* 电池数据转移的位置 */
#define Battery_Data_Place_Offset	(32u)
/* 电池数据存储的位置 */
#define Battery_Data_Store_Offset    (8u)

	
/* 判断 通道1 */
#if		Sensor_CH1_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_1) 	= ADC_Collection_Data_Buffer[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_1) 	= ADC_Collection_Data_Buffer[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_1) 	= ADC_Collection_Data_Buffer[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_PT1000
		*(data_buffer + Sensors_CH_1) 	= ADC_Collection_Data_Buffer[Sensors_CH_1];
#endif	
	
/* 判断 通道2 */
#if		Sensor_CH2_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_2) 		= ADC_Collection_Data_Buffer[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_2) 	= ADC_Collection_Data_Buffer[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_2) 		= ADC_Collection_Data_Buffer[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_PT1000
		*(data_buffer + Sensors_CH_2) 		= ADC_Collection_Data_Buffer[Sensors_CH_2];
#endif	
	
/* 判断 通道3 */
#if		Sensor_CH3_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_3) 		= ADC_Collection_Data_Buffer[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_3) 	= ADC_Collection_Data_Buffer[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_3) 		= ADC_Collection_Data_Buffer[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_PT1000
#error	"传感器通道3不可配置为PT1000传感器，因为当前硬件不支持。"
#endif	
	
/* 判断 通道4 */
#if		Sensor_CH4_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_4) 		= ADC_Collection_Data_Buffer[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_4) 	= ADC_Collection_Data_Buffer[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_4) 		= ADC_Collection_Data_Buffer[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_PT1000
		#error	"传感器通道4不可配置为PT1000传感器，因为当前硬件不支持。"
#endif	
	
/* 判断 通道5 */
#if		Sensor_CH5_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_5) 		= ADC_Collection_Data_Buffer[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_5) 	= ADC_Collection_Data_Buffer[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_5) 		= ADC_Collection_Data_Buffer[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_PT1000
		#error	"传感器通道5不可配置为PT1000传感器，因为当前硬件不支持。"
#endif	
	
/* 判断 通道6 */
#if		Sensor_CH6_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_6) 		= ADC_Collection_Data_Buffer[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_6) 		= ADC_Collection_Data_Buffer[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_PT1000
		#error	"传感器通道6不可配置为PT1000传感器，因为当前硬件不支持。"
#endif	
	
/* 判断 通道7 */
#if		Sensor_CH7_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_7) 		= ADC_Collection_Data_Buffer[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_7) 		= ADC_Collection_Data_Buffer[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_DoorContact
		*(data_buffer + Sensors_CH_7) 		= ADC_Collection_Data_Buffer[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_PT1000
		#error	"传感器通道7不可配置为PT1000传感器，因为当前硬件不支持。"
#endif	
	
/* 判断 通道8 */
#if		Sensor_CH8_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_8) 		= ADC_Collection_Data_Buffer[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_8) 		= ADC_Collection_Data_Buffer[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_PT1000
		#error	"传感器通道8不可配置为PT1000传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道9 */
#if		((Sensor_CH9_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH9_Config_Type == Sensor_Hum)||\
		 (Sensor_CH9_Config_Type == Sensor_PT1000))
#error	"传感器通道9不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道10 */
#if		((Sensor_CH10_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH10_Config_Type == Sensor_Hum)||\
		 (Sensor_CH10_Config_Type == Sensor_PT1000))
#error	"传感器通道10不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道11 */
#if		((Sensor_CH11_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH11_Config_Type == Sensor_Hum)||\
		 (Sensor_CH11_Config_Type == Sensor_PT1000))
#error	"传感器通道11不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道12 */
#if		((Sensor_CH12_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH12_Config_Type == Sensor_Hum)||\
		 (Sensor_CH12_Config_Type == Sensor_PT1000))
#error	"传感器通道12不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道13 */
#if		((Sensor_CH13_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH13_Config_Type == Sensor_Hum)||\
		 (Sensor_CH13_Config_Type == Sensor_PT1000))
#error	"传感器通道13不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道14 */
#if		((Sensor_CH14_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH14_Config_Type == Sensor_Hum)||\
		 (Sensor_CH14_Config_Type == Sensor_PT1000))
#error	"传感器通道14不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道15 */
#if		((Sensor_CH15_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH15_Config_Type == Sensor_Hum)||\
		 (Sensor_CH15_Config_Type == Sensor_PT1000))
#error	"传感器通道15不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道16 */
#if		((Sensor_CH16_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH16_Config_Type == Sensor_Hum)||\
		 (Sensor_CH16_Config_Type == Sensor_PT1000))
#error	"传感器通道16不可配置为板载传感器，因为当前硬件不支持。"
#endif
	
/* 判断 通道17 */
#if		((Sensor_CH17_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH17_Config_Type == Sensor_Hum)||\
		 (Sensor_CH17_Config_Type == Sensor_PT1000))
#error	"传感器通道17不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道18 */
#if		((Sensor_CH18_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH18_Config_Type == Sensor_Hum)||\
		 (Sensor_CH18_Config_Type == Sensor_PT1000))
#error	"传感器通道18不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道19 */
#if		((Sensor_CH19_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH19_Config_Type == Sensor_Hum)||\
		 (Sensor_CH19_Config_Type == Sensor_PT1000))
#error	"传感器通道19不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道20 */
#if		((Sensor_CH20_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH20_Config_Type == Sensor_Hum)||\
		 (Sensor_CH20_Config_Type == Sensor_PT1000))
#error	"传感器通道20不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道21 */
#if		((Sensor_CH21_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH21_Config_Type == Sensor_Hum)||\
		 (Sensor_CH21_Config_Type == Sensor_PT1000))
#error	"传感器通道21不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道22 */
#if		((Sensor_CH22_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH22_Config_Type == Sensor_Hum)||\
		 (Sensor_CH22_Config_Type == Sensor_PT1000))
#error	"传感器通道22不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道23 */
#if		((Sensor_CH23_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH23_Config_Type == Sensor_Hum)||\
		 (Sensor_CH23_Config_Type == Sensor_PT1000))
#error	"传感器通道23不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道24 */
#if		((Sensor_CH24_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH24_Config_Type == Sensor_Hum)||\
		 (Sensor_CH24_Config_Type == Sensor_PT1000))
#error	"传感器通道24不可配置为板载传感器，因为当前硬件不支持。"
#endif

/* 判断 通道25 */
#if		((Sensor_CH25_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH25_Config_Type == Sensor_Hum)||\
		 (Sensor_CH25_Config_Type == Sensor_PT1000))
#error	"传感器通道25不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道26 */
#if		((Sensor_CH26_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH26_Config_Type == Sensor_Hum)||\
		 (Sensor_CH26_Config_Type == Sensor_PT1000))
#error	"传感器通道26不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道27 */
#if		((Sensor_CH27_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH27_Config_Type == Sensor_Hum)||\
		 (Sensor_CH27_Config_Type == Sensor_PT1000))
#error	"传感器通道27不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道28 */
#if		((Sensor_CH28_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH28_Config_Type == Sensor_Hum)||\
		 (Sensor_CH28_Config_Type == Sensor_PT1000))
#error	"传感器通道28不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道29 */
#if		((Sensor_CH29_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH29_Config_Type == Sensor_Hum)||\
		 (Sensor_CH29_Config_Type == Sensor_PT1000))
#error	"传感器通道29不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道30 */
#if		((Sensor_CH30_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH30_Config_Type == Sensor_Hum)||\
		 (Sensor_CH30_Config_Type == Sensor_PT1000))
#error	"传感器通道30不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道31 */
#if		((Sensor_CH31_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH31_Config_Type == Sensor_Hum)||\
		 (Sensor_CH31_Config_Type == Sensor_PT1000))
#error	"传感器通道31不可配置为板载传感器，因为当前硬件不支持。"
#endif	

/* 判断 通道32 */
#if		((Sensor_CH32_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH32_Config_Type == Sensor_Hum)||\
		 (Sensor_CH32_Config_Type == Sensor_PT1000))
#error	"传感器通道32不可配置为板载传感器，因为当前硬件不支持。"
#endif
		

		/* 电池通道数据 */
		*(data_buffer + Battery_Data_Place_Offset) 	= ADC_Collection_Data_Buffer[Battery_Data_Store_Offset];

#undef Battery
}

/*************************************************************************************************/
/* 函数名	：ADC_AWD_Run_Init															 	 	 */
/* 输入		：无																	 			 	 */
/* 输出		: 无 																				 */
/* 作用		：仪器运行时对模拟看门狗的初始化													 	 */
/* 创建日期	: 2015/11/20																		 */
/*************************************************************************************************/
void ADC_AWD_Run_Init(void)
{
	/* 新的外接电源连接状态标志 */
	static FlagStatus new_et_power_state_flag = RESET;
	/* 旧的外接电源连接状态标志 */
	static FlagStatus old_et_power_state_flag = RESET;
	
	/* 获取电源连接状态 */
	new_et_power_state_flag = (Ext_Power_ON_OFF_Check()==Bit_RESET)?RESET:SET;
	
	/* 判断外接电源连接状态 */
	if(new_et_power_state_flag == Bit_RESET)
	{
		/* 外电已连接 */
		/* 判断老的电源状态 */
		if(old_et_power_state_flag != RESET)
		{
			/* 关闭模拟看门狗 */
			ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0FFF, 0x0000);
		}
			/* 设置老电源状态 */
			old_et_power_state_flag = RESET;		
	}else
	{
		/* 外电未连接 */
		/* 判断新电源状态与老的电源状态 */
		if(old_et_power_state_flag != SET)
		{
			/* 设置模拟看门狗 */
			/* 在ADC第一次数据采集完成后，如果程序没有复位说明当前的电池电压高于3.5V，仪器可以正常工作。接着之后将
			   模拟看门狗的下限电压设置成3.35V。*/
			ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0FFF, ADC1_Week_Voltage_Low);		
		}
			/* 设置老电源状态 */
			old_et_power_state_flag = SET;
	}
}

///*************************************************************************************************/
///* 函数名	：ADC_AWD_Frt_Init															 	 	 */
///* 输入		：无																	 			 	 */
///* 输出		: 无 																				 */
///* 作用		：仪器上电时对模拟看门狗的初始化													 	 */
///* 创建日期	: 2015/11/20																		 */
///*************************************************************************************************/
//void ADC_AWD_Frt_Init(void)
//{

//	/* 判断外接电源连接状态 */
//	if(Ext_Power_ON_OFF_Check() == Bit_RESET)
//	{
//		/* 外电已连接 */
//		/* 不设置模拟看门狗 */
//		__nop();
//	}else
//	{
//		/* 外电未连接 */
//		/* 设置模拟看门狗 */
//		/* 在ADC第一次数据采集完成后，如果程序没有复位说明当前的电池电压高于3.5V，仪器可以正常工作。接着之后将
//		   模拟看门狗的下限电压设置成3.35V。*/
//		ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0FFF, ADC1_Start_Voltage);
//	}

//}

/*************************************************************************************************/
/* 函数名	：ADC_Sam_Pwr_Sta															 	 	 */
/* 输入		：无																	 			 	 */
/* 输出		: 无 																				 */
/* 作用		：上电检测电池电压													 	 */
/* 创建日期	: 2015/12/11																		 */
/*************************************************************************************************/
void ADC_Sam_Pwr_Sta(void)
{

	do{
		/* 检测当前系统电源 */
		ADC_Data_Buffer_Init();	
	}while((ADC_Collection_Data_Buffer[8] < ADC1_Start_Voltage)&&
		   (Ext_Power_ON_OFF_Check() == Bit_SET));
	
}




/*************************************************************************************************/

