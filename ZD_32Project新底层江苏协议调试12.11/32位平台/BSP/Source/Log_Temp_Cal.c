/**
  ******************************************************************************
  * @file              : Log_Temp_Cal.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 10/03/2015 17:41:56
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
  */

#include "System_Config.h"

#include "Log_Temp_Cal.h"
#include "Log_RF_Card.h"

#include <string.h>
#include <stdlib.h>


/*************************************************************************************************/
/* 温度传感器默认的数据表 */
#define NTC_10K_3950_Table_Buffer_Size	(231u)
/* 温度对应的ADC值的NTC数据表（3950） */
const u16 NTC_10K_3950[NTC_10K_3950_Table_Buffer_Size] = 
{
	0x0003,0x0003,0x0004,0x0004,0x0005,0x0005,0x0006,0x0006,
	0x0007,0x0008,0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,
	0x000F,0x0010,0x0011,0x0013,0x0015,0x0017,0x0019,0x001B,
	0x001D,0x0020,0x0022,0x0025,0x0028,0x002C,0x002F,0x0033,
	0x0037,0x003B,0x0040,0x0045,0x004A,0x0050,0x0056,0x005D,
	0x0063,0x006B,0x0072,0x007B,0x0083,0x008C,0x0096,0x00A0,
	0x00AB,0x00B7,0x00C3,0x00D0,0x00DD,0x00EB,0x00FA,0x0109,
	0x011A,0x012B,0x013D,0x014F,0x0163,0x0177,0x018D,0x01A3,
	0x01BA,0x01D2,0x01EB,0x0205,0x0220,0x023B,0x0258,0x0276,
	0x0295,0x02B4,0x02D5,0x02F6,0x0319,0x033C,0x0360,0x0385,
	0x03AB,0x03D2,0x03F9,0x0421,0x044A,0x0474,0x049E,0x04C9,
	0x04F5,0x0521,0x054D,0x057A,0x05A8,0x05D5,0x0603,0x0631,
	0x0660,0x068E,0x06BD,0x06EB,0x071A,0x0748,0x0776,0x07A4,
	0x07D2,0x0800,0x082D,0x085A,0x0887,0x08B3,0x08DF,0x090A,
	0x0935,0x095F,0x0989,0x09B2,0x09DA,0x0A02,0x0A29,0x0A4F,
	0x0A75,0x0A9A,0x0ABE,0x0AE2,0x0B05,0x0B27,0x0B48,0x0B69,
	0x0B89,0x0BA8,0x0BC6,0x0BE4,0x0C01,0x0C1D,0x0C39,0x0C54,
	0x0C6E,0x0C88,0x0CA1,0x0CB9,0x0CD0,0x0CE7,0x0CFE,0x0D13,
	0x0D28,0x0D3D,0x0D51,0x0D64,0x0D77,0x0D89,0x0D9B,0x0DAC,
	0x0DBD,0x0DCD,0x0DDD,0x0DEC,0x0DFB,0x0E0A,0x0E18,0x0E25,
	0x0E32,0x0E3F,0x0E4B,0x0E58,0x0E63,0x0E6E,0x0E79,0x0E84,
	0x0E8E,0x0E99,0x0EA2,0x0EAC,0x0EB5,0x0EBE,0x0EC6,0x0ECF,
	0x0ED7,0x0EDF,0x0EE6,0x0EEE,0x0EF5,0x0EFC,0x0F03,0x0F09,
	0x0F10,0x0F16,0x0F1C,0x0F22,0x0F27,0x0F2D,0x0F32,0x0F37,
	0x0F3C,0x0F41,0x0F46,0x0F4B,0x0F4F,0x0F53,0x0F58,0x0F5C,
	0x0F60,0x0F64,0x0F67,0x0F6B,0x0F6F,0x0F72,0x0F75,0x0F79,
	0x0F7C,0x0F7F,0x0F82,0x0F85,0x0F88,0x0F8B,0x0F8D,0x0F90,
	0x0F92,0x0F95,0x0F97,0x0F9A,0x0F9C,0x0F9E,0x0FA0,0x0FA2,
	0x0FA5,0x0FA7,0x0FA9,0x0FAB,0x0FAC,0x0FAE,0x0FB0,
};

/*************************************************************************************************/
//static u16 Temp_Float_To_Hex(float Temp);
/* 真实值 排序 后的表 */
static u16 CH_Actual_Value_Buffer		[INSTRU_SENSOR_Data_Tab_Size];
/* 原始值 排序 后的表 */
static u16 CH_Calibration_Value_Buffer	[INSTRU_SENSOR_Data_Tab_Size];
/* 文件函数声明 */
static u8 Temp_Cal_Data_Tab_Qsort(u16* CH_Actual_Value,u16* CH_Calibration_Value);
/*************************************************************************************************/

static int 	Comp(const void *p1,const void *p2)
{
	return *((u16*)p2) > *((u16*)p1)?1:-1; 	
}


/*************************************************************************************************/
/* 函数名  	: Temp_Cal_Data_Tab_Qsort															 */
/* 输入		: CH_Actual_Value:真实值数据白指针;CH_Calibration_Value:校准值数据白指针				 */
/* 输出		: 当前通道校准点数量 																	 */
/* 作用		: 对当前通道真实值和原始值数据表进行排序													 */
/* 创建日期	: 2014/10/27																		 */
/*************************************************************************************************/
static u8 Temp_Cal_Data_Tab_Qsort(u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
	/* 校准点计数器 */
	u8 cal_num;
	
	/* 清除 数据 */
	memset(CH_Actual_Value_Buffer,0x00,sizeof(CH_Actual_Value_Buffer)); 
	memset(CH_Calibration_Value_Buffer,0x00,sizeof(CH_Calibration_Value_Buffer)); 
	/* 将数据缓存数据赋值到临时变量数组中 */
	memcpy(CH_Actual_Value_Buffer,CH_Actual_Value,sizeof(CH_Actual_Value_Buffer)); 
	/* 对真实值数据表排序 */
	qsort(CH_Actual_Value_Buffer,INSTRU_SENSOR_Data_Tab_Size,sizeof(u16),Comp);
	/* 将数据缓存数据赋值到临时变量数组中 */
	memcpy(CH_Calibration_Value_Buffer,CH_Calibration_Value,sizeof(CH_Calibration_Value_Buffer)); 
	/* 对真实值数据表排序 */
	qsort(CH_Calibration_Value_Buffer,INSTRU_SENSOR_Data_Tab_Size,sizeof(u16),Comp);
	
	/* 循环判断 当前通道校准点数量 */
	for(cal_num = 0 ; cal_num < INSTRU_SENSOR_Data_Tab_Size ; cal_num ++)
	{
		/* 判断 当前通道校准点数量 */
		if(CH_Actual_Value_Buffer[cal_num] == 0)
		{
			/* 由于当前的排序是从大到小，因此当判断到有等于0的Buffer时就说明已经是最小的数值了 */
			/* 跳出循环判断 */
			break;
		}
	}
	/* 返回当前通道的校准点数据量 */
	return cal_num;
}


#if (0)
/********************************************************************************************************/
/* 函数名  	: Temp_Get_Cal_Value																 		*/
/* 输入		: Temp_Value:温度真实值CH_Actual_Value:真实值数据白指针;CH_Calibration_Value:校准值数据白指针	*/
/* 输出		: 温度校准值 																	 			*/
/* 作用		: 根据当前通道的校准数据表设置，将温度真实值转换成校准值输出								 		*/
/* 创建日期	: 2015/03/25																		 		*/
/* 算法		: Ty = ((Ty1-Ty0)/(Tx1-Tx0))(Tx-Tx0)+Ty0											 		*/
/********************************************************************************************************/
float Temp_Get_Cal_Value(float Temp_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
/* 温度传感器的最大参数值 */
#define  CH_Actual_MAX_Value	(300.0)
	u8 num;
	u8 cal_num;
#if 0
	/* 根据当前的温度数据确定数据所在的区间 */
	if(Temp_Value < CH_Actual_Value[0])
	{
		/* 当前的数据小于最低值区间 */
		/* 将当前的温度设置成最低温度 */
		return CH_Actual_Value[0];
	}
#endif
	/* 
		判断数据状态：
		1、卡片数据接收超时；
		2、数据超出测量范围（正常的逻辑没有这种情况）
		...
	*/
	if(Temp_Value == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* 卡片数据接收超时 */
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	else if(Temp_Value >= CH_Actual_MAX_Value)
	{
		/* 当前的数据大于最大值区间 */
		/* 将当前的温度设置成最高温度 */
		return CH_Actual_MAX_Value;
	}else
	{
		/* 当前的数据在测量的范围内 */
		/* 对真实值的 */
		
		/* 对当前通道真实值和原始值数据表进行排序 */
		cal_num = Temp_Cal_Data_Tab_Qsort(CH_Actual_Value,CH_Calibration_Value);
		
		/* 判断当前通道是否校准过 */
		if(cal_num == 0)
		{
			/* 当前通道没有校准过，因此校准值等于真实值 */
			return Temp_Value;
		}else if(cal_num == 1)
		{
			/* 当前通道校准过一个点 */
			/* 校准一个点时，对整条曲线进行调整 */
			return (Temp_Value-(CH_Actual_Value_Buffer[0]-CH_Calibration_Value_Buffer[0]));
		}else
		{
			/* 当前通道校准数量大于1个（小于8个） */
			for(num = 0 ; num < (cal_num+1) ; num ++)
			{
				/* 循环检测 当前的温度在哪段校准区间 */
				if(Temp_Value >= CH_Actual_Value_Buffer[0])
				{
					/* 判断 大于 最大的校准点 */
					return (Temp_Value-(CH_Actual_Value_Buffer[0]-CH_Calibration_Value_Buffer[0]));
				}else if(Temp_Value < CH_Actual_Value_Buffer[cal_num-1])
				{
					/* 判断 大于 最大的校准点 */
					return (Temp_Value-(CH_Actual_Value_Buffer[cal_num-1]-CH_Calibration_Value_Buffer[cal_num-1]));
				}else
				{
					/* 循环检测 当前的温度在哪段校准区间 */
					if((Temp_Value < (CH_Actual_Value_Buffer[num])&&(Temp_Value >= (CH_Actual_Value_Buffer[num+1]))
					{
						/* 返回 校准的数据 */
						return((((CH_Calibration_Value_Buffer[num]-CH_Calibration_Value_Buffer[num+1])/(CH_Actual_Value_Buffer[num]-CH_Actual_Value_Buffer[num+1]))*
							   (Temp_Value-CH_Actual_Value_Buffer[num+1]))+CH_Calibration_Value_Buffer[num+1]);
					}
				}
			}
		}
		
		/* 这里是不可能运行到的地方 */
		return Temp_Value;
	}
	
#undef  CH_Actual_MAX_Value
}

#else
/********************************************************************************************************/
/* 函数名  	: Temp_Get_Cal_Value																 		*/
/* 输入		: Temp_Value:温度真实值CH_Actual_Value:真实值数据白指针;CH_Calibration_Value:校准值数据白指针	*/
/* 输出		: 温度校准值 																	 			*/
/* 作用		: 根据当前通道的校准数据表设置，将温度真实值转换成校准值输出								 	*/
/* 创建日期	: 2015/03/25																		 		*/
/* 算法		: Ty = ((Ty1-Ty0)/(Tx1-Tx0))(Tx-Tx0)+Ty0											 		*/
/********************************************************************************************************/
float Temp_Get_Cal_Value(float Temp_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
/* 温度传感器的最大参数值 */
#define  CH_Actual_MAX_Value	(300.0)
	u8 num;
	u8 cal_num;
	u16 temp_b;

#if 0
	/* 根据当前的温度数据确定数据所在的区间 */
	if(Temp_Value < CH_Actual_Value[0])
	{
		/* 当前的数据小于最低值区间 */
		/* 将当前的温度设置成最低温度 */
		return CH_Actual_Value[0];
	}
#endif
	/* 
		判断数据状态：
		1、卡片数据接收超时；
		2、数据超出测量范围（正常的逻辑没有这种情况）
		...
	*/
	if(Temp_Value == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* 卡片数据接收超时 */
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	else if(Temp_Value >= CH_Actual_MAX_Value)
	{
		/* 当前的数据大于最大值区间 */
		/* 将当前的温度设置成最高温度 */
		return CH_Actual_MAX_Value;
	}else
	{
		/* 当前的数据在测量的范围内 */
		temp_b = Temp_Value*10+2731;
		
		/* 对当前通道真实值和原始值数据表进行排序 */
		cal_num = Temp_Cal_Data_Tab_Qsort(CH_Actual_Value,CH_Calibration_Value);
		
		/* 判断当前通道是否校准过 */
		if(cal_num == 0)
		{
			/* 当前通道没有校准过，因此校准值等于真实值 */
			return Temp_Value;
		}else if(cal_num == 1)
		{
			/* 当前通道校准过一个点 */
			/* 校准一个点时，对整条曲线进行调整 */
			return ((((float)temp_b-((float)CH_Actual_Value_Buffer[0]-(float)CH_Calibration_Value_Buffer[0]))-2731)/10);
		}else
		{
			/* 当前通道校准数量大于1个（小于8个） */
			for(num = 0 ; num < (cal_num+1) ; num ++)
			{
				/* 循环检测 当前的温度在哪段校准区间 */
				if(temp_b >= CH_Actual_Value_Buffer[0])
				{
					/* 判断 大于 最大的校准点 */
					return ((((float)temp_b-((float)CH_Actual_Value_Buffer[0]-(float)CH_Calibration_Value_Buffer[0]))-2731)/10);
				}else if(temp_b < CH_Actual_Value_Buffer[cal_num-1])
				{
					/* 判断 大于 最大的校准点 */
					return ((((float)temp_b-((float)CH_Actual_Value_Buffer[cal_num-1]-(float)CH_Calibration_Value_Buffer[cal_num-1]))-2731)/10);
				}else
				{
					/* 循环检测 当前的温度在哪段校准区间 */
					if((temp_b < CH_Actual_Value_Buffer[num])&&(temp_b >= CH_Actual_Value_Buffer[num+1]))
					{
						/* 返回 校准的数据 */
						return((((((((float)CH_Calibration_Value_Buffer[num])-((float)CH_Calibration_Value_Buffer[num+1]))/(((float)CH_Actual_Value_Buffer[num])-((float)CH_Actual_Value_Buffer[num+1])))*
							   (((float)temp_b)-((float)CH_Actual_Value_Buffer[num+1])))+CH_Calibration_Value_Buffer[num+1])-2731)/10);
					}
				}
			}
		}
		
		/* 这里是不可能运行到的地方 */
		return Temp_Value;
	}
	
#undef  CH_Actual_MAX_Value
}

#endif


/*************************************************************************************************/
/* 函数名	:Temp_Get_Sampl_Value															 	 */
/* 输入		:温度传感器AD采集数据																	 */
/* 输出		:采样的温度数据值（温度值*10）															 */
/* 作用		:将输入的ADC数据转换成温度数据														 	 */
/* 创建日期	:2015/03/25																			 */
/* Voltage divide resistor Pull-Down 														 	 */
/*************************************************************************************************/
float Temp_Get_Sampl_Value(u16 Adc_DR)
{	
/* 数据表的 起始温度点 */
#define TemprStart 		(-80.0)
/* 数据表的 结束温度点 */
#define TemprEnd 		(150.0)
/* 数据表的 温度查询精度 */
#define Scale      		(1u)
/* 数据表温度查询范围 */
#define TableSize  		(231u)

	
	/* 计数器 */
	u16 num;
	/* 数据表 搜索 开始 位置 */
	int16_t SearchStart = 0 ;
	/* 数据表 搜索 结束 位置 */
	int16_t SearchEnd 	= (TableSize - 1);
	/* 数据表 搜索 范围 */
	int16_t SearchMax 	= TableSize;
	
	
	/* 判断当前的ADC数据是否超出，数据表的界限 */
	if(Adc_DR <= NTC_10K_3950[SearchStart])
	{
		/* ADC值 小于 数据表的 起始 位置的值 */
		/* 输出起始位置的温度数据 */
		return (SearchStart * Scale + TemprStart);
	}else if(Adc_DR >= NTC_10K_3950[SearchEnd])
	{
		/* ADC值 大于 数据表的 结束 位置的值 */
		/* 输出结束位置的温度数据 */
		return (SearchEnd * Scale + TemprStart);
	}
		
	/* 循环 搜索 当前的 ADC 在那个温度区间内（精度1℃） */
	for(num = 0 ; num < SearchMax ; num ++)
	{
		/* 判断 搜索的范围  */
		if((SearchEnd - SearchStart) > 1)
		{
			/* 搜索 范围大于 1℃ */
			/* 根据 当前 查询范围的中间点数据表值 */
			if(NTC_10K_3950[ (SearchEnd + SearchStart) / 2 ] > Adc_DR)
			{
				/* 数据表值大于ADC值 */
				/* 设置 查询范围的结束点位置 */
				SearchEnd = ( SearchEnd + SearchStart ) / 2;
			}
			else if(NTC_10K_3950[ ( SearchEnd + SearchStart ) / 2 ] < Adc_DR)
			{
				/* 数据表值小于ADC值 */
				/* 设置 查询范围的开始点位置 */
				SearchStart = ( SearchEnd + SearchStart ) / 2;
			}
			else
			{
				/* 数据表值等于ADC值 */
				/* 返回 温度数据 */
				return (((float)((SearchEnd + SearchStart) / 2) * Scale) + TemprStart);
			}
		}
		else
		{
			/* 搜索 范围小于 1℃ */
			/* 计数温度 */
			return (((((float)Adc_DR-(float)NTC_10K_3950[SearchStart])/
					  ((float)NTC_10K_3950[SearchEnd]-(float)NTC_10K_3950[SearchStart]))+
					   ((float)SearchStart))+TemprStart);
		}
	}
	/* 这里不可能运行到。 */
	return 0;
	
#undef TemprStart
#undef TemprEnd
#undef Scale
#undef TableSize
}

//二氧化碳采集值
float CarbonDioxide_Get_Sampl_Value(u16 Adc_DR)
{
	float vol;
	float ppm;
	vol=(float)Adc_DR/4096*3.3;
	//printf("vol=%f\r\n",vol);
	ppm=6250*(vol-0.4); //PPM
	//printf("ppm=%f\r\n",ppm);
	return ppm;
}

//门磁开关
float DoorContact_Get_Sampl_Value(u16 Adc_DR)
{
	float vol;
	float DoorContact_Status=0;
	vol=(float)Adc_DR/4096*3.3;
	//printf("vol=%f\r\n",vol);
	if(vol>2.0)
	{
		DoorContact_Status=0;
	}
	else
	{
		DoorContact_Status=1;
	}
	//printf("DoorContact_Status=%f\r\n",DoorContact_Status);
	return DoorContact_Status;
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

