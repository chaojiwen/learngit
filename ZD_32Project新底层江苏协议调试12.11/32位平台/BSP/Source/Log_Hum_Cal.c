/**
  ******************************************************************************
  * @file              : Log_Hum_Cal.c
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

#include "Log_Hum_Cal.h"

#include "Log_RF_Card.h"
#include "Log_Sensor.h"

#include <string.h>
#include <stdlib.h>


/*************************************************************************************************/
/* 湿度传感器默认的数据表大小 */
#define HIH_5030_Table_Buffer_Size		(11u)
/* 霍尼韦尔 湿度湿度传感器 5030 ADC数据表 */
const u16 HIH_5030[HIH_5030_Table_Buffer_Size] = 
{
	{620}, //0%RH
	{918}, //10%RH
	{1134},//20%RH
	{1411},//30%RH
	{1669},//40%RH
	{1904},//50%RH
	{2204},//60%RH
	{2482},//70%RH
	{2782},//80%RH
	{3060},//90%RH
	{3227},//100%RH
};

/*************************************************************************************************/
/* 湿度传感器温度补偿参数 */
static float Humi_Temp_Comp;

/* 真实值 排序 后的表 */
static u16 CH_Actual_Value_Buffer		[INSTRU_SENSOR_Data_Tab_Size];
/* 原始值 排序 后的表 */
static u16 CH_Calibration_Value_Buffer	[INSTRU_SENSOR_Data_Tab_Size];

/*************************************************************************************************/

/*************************************************************************************************/
static u8 Humi_Cal_Data_Tab_Qsort(u16* CH_Actual_Value,u16* CH_Calibration_Value);
/*************************************************************************************************/
static int 	Comp(const void *p1,const void *p2)
{
	return *((u16*)p2) > *((u16*)p1)?1:-1; 	
}



#if	(Hum_Sensor_Tmp_Comp_Enable == 0x01)
/*************************************************************************************************/
/* 函数名  	: Humi_Get_Temp_Comp_Value															 */
/* 输入		: sensor_t:传感器通道类型值；sensor_f：温度传感器校准值									 */
/* 输出		: 无			 																	 */
/* 作用		: 获得湿度传感器温度补偿参数															 */
/* 创建日期	: 2014/10/27																		 */
/*************************************************************************************************/
void Humi_Get_Temp_Comp_Value(void)
{
/* 常温 */
#define	Ambient_Temp	(25.0)


/* 判断 湿度传感器温度补偿的传感器选择 第几 通道 */
#if (Hum_Sensor_Tmp_Comp_Enable == 0X01)
	/* 湿度传感器温度补偿的传感器选择 通道1 */
	/* 获得 通道1 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X02)	
	/* 湿度传感器温度补偿的传感器选择 通道2 */
	/* 获得 通道2 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X03)	
	/* 湿度传感器温度补偿的传感器选择 通道3 */
	/* 获得 通道3 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X04)	
	/* 湿度传感器温度补偿的传感器选择 通道4 */
	/* 获得 通道4 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X05)
	/* 湿度传感器温度补偿的传感器选择 通道5 */
	/* 获得 通道5 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X06)
	/* 湿度传感器温度补偿的传感器选择 通道6 */
	/* 获得 通道6 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X07)	
	/* 湿度传感器温度补偿的传感器选择 通道7 */	
	/* 获得 通道7 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X08)
	/* 湿度传感器温度补偿的传感器选择 通道8 */
	/* 获得 通道8 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X09)
	/* 湿度传感器温度补偿的传感器选择 通道9 */
	/* 获得 通道9 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0A)
	/* 湿度传感器温度补偿的传感器选择 通道10 */
	/* 获得 通道10 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0B)
	/* 湿度传感器温度补偿的传感器选择 通道11 */
	/* 获得 通道11 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0C)
	/* 湿度传感器温度补偿的传感器选择 通道12 */
	/* 获得 通道12 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0D)
	/* 湿度传感器温度补偿的传感器选择 通道13 */
	/* 获得 通道13 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0E)
	/* 湿度传感器温度补偿的传感器选择 通道14 */
	/* 获得 通道14 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0F)
	/* 湿度传感器温度补偿的传感器选择 通道15 */
	/* 获得 通道15 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X10)
	/* 湿度传感器温度补偿的传感器选择 通道16 */
	/* 获得 通道16 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X11)
	/* 湿度传感器温度补偿的传感器选择 通道17 */
	/* 获得 通道17 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X12)
	/* 湿度传感器温度补偿的传感器选择 通道18 */
	/* 获得 通道18 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X13)
	/* 湿度传感器温度补偿的传感器选择 通道19 */
	/* 获得 通道19 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X14)
	/* 湿度传感器温度补偿的传感器选择 通道20 */
	/* 获得 通道20 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X15)
	/* 湿度传感器温度补偿的传感器选择 通道21 */
	/* 获得 通道21 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X16)
	/* 湿度传感器温度补偿的传感器选择 通道22 */
	/* 获得 通道22 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X17)
	/* 湿度传感器温度补偿的传感器选择 通道23 */
	/* 获得 通道23 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X18)
	/* 湿度传感器温度补偿的传感器选择 通道24 */
	/* 获得 通道24 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X19)
	/* 湿度传感器温度补偿的传感器选择 通道25 */
	/* 获得 通道25 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1A)
	/* 湿度传感器温度补偿的传感器选择 通道26 */
	/* 获得 通道26 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1B)
	/* 湿度传感器温度补偿的传感器选择 通道27 */
	/* 获得 通道27 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1C)
	/* 湿度传感器温度补偿的传感器选择 通道28 */
	/* 获得 通道28 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1D)
	/* 湿度传感器温度补偿的传感器选择 通道29 */
	/* 获得 通道29 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1E)
	/* 湿度传感器温度补偿的传感器选择 通道30 */
	/* 获得 通道30 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1F)
	/* 湿度传感器温度补偿的传感器选择 通道31 */
	/* 获得 通道31 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X20)
	/* 湿度传感器温度补偿的传感器选择 通道32 */
	/* 获得 通道32 的温度传感器值 */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data;
#else
	#error "湿度传感器传感器通道参数配置错误."
#endif	

#undef	Ambient_Temp
}
#endif


/*************************************************************************************************/
/* 函数名  	: Humi_Cal_Data_Tab_Qsort															 */
/* 输入		: CH_Actual_Value:真实值数据白指针;CH_Calibration_Value:校准值数据白指针				 */
/* 输出		: 当前通道校准点数量 																	 */
/* 作用		: 对当前通道真实值和原始值数据表进行排序													 */
/* 创建日期	: 2014/10/27																		 */
/*************************************************************************************************/
static u8 Humi_Cal_Data_Tab_Qsort(u16* CH_Actual_Value,u16* CH_Calibration_Value)
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
/*************************************************************************************************/
/* 函数名  	: Humi_Get_Cal_Value															 	 */
/* 输入		: CH_Actual_Value:真实值数据白指针;CH_Calibration_Value:校准值数据白指针				 */
/* 输出		: 当前通道校准点数量 																	 */
/* 作用		: 温度的检测值：Hum_Value；传感器通道编号：CH_NO；传感器校准数据表：						 */
/* 算法		：Ty = ((Ty1-Ty0)/(Tx1-Tx0))(Tx-Tx0)+Ty0；											 */
/* 创建日期	: 2014/10/27																		 */
/*************************************************************************************************/
float Humi_Get_Cal_Value(float Humi_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
/* 湿度传感器的最大参数值 */
#define  CH_Actual_MAX_Value	(100.0)
	u8 num;
	u8 cal_num;
#if 0
	/* 根据当前的温度数据确定数据所在的区间 */
	if(Humi_Value < CH_Actual_Value[0])
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
	if(Humi_Value == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* 卡片数据接收超时 */
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	else if(Humi_Value >= CH_Actual_MAX_Value)
	{
		/* 当前的数据大于最大值区间 */
		/* 将当前的温度设置成最高温度 */
		return CH_Actual_MAX_Value;
	}else
	{
		/* 当前的数据在测量的范围内 */
		/* 对真实值的 */
		
		/* 对当前通道真实值和原始值数据表进行排序 */
		cal_num = Humi_Cal_Data_Tab_Qsort(CH_Actual_Value,CH_Calibration_Value);
		
		/* 判断当前通道是否校准过 */
		if(cal_num == 0)
		{
			/* 当前通道没有校准过，因此校准值等于真实值 */
			return Humi_Value;
		}else if(cal_num == 1)
		{
			/* 当前通道校准过一个点 */
			/* 校准一个点时，对整条曲线进行调整 */
			return (Humi_Value-(CH_Actual_Value_Buffer[0]-CH_Calibration_Value_Buffer[0]));
		}else
		{
			/* 当前通道校准带你数量大于1个（小于8个） */
			for(num = 0 ; num < (cal_num+1) ; num ++)
			{
				/* 循环检测 当前的温度在哪段校准区间 */
				if(Humi_Value >= CH_Actual_Value_Buffer[0])
				{
					/* 判断 大于 最大的校准点 */
					return (Humi_Value-(CH_Actual_Value_Buffer[0])-CH_Calibration_Value_Buffer[0]));
				}else if(Humi_Value < CH_Actual_Value_Buffer[cal_num-1])
				{
					/* 判断 大于 最大的校准点 */
					return (Humi_Value-(CH_Actual_Value_Buffer[cal_num-1])-CH_Calibration_Value_Buffer[cal_num-1]));
				}else
				{
					/* 循环检测 当前的温度在哪段校准区间 */
					if((Humi_Value < CH_Actual_Value_Buffer[num])&&(Humi_Value >= CH_Actual_Value_Buffer[num+1]))
					{
						/* 返回 校准的数据 */
						return((((CH_Calibration_Value_Buffer[num]-CH_Calibration_Value_Buffer[num+1])/(CH_Actual_Value_Buffer[num]-CH_Actual_Value_Buffer[num+1]))*
							   (Humi_Value-CH_Actual_Value_Buffer[num+1]))+CH_Calibration_Value_Buffer[num+1]);
					}
				}
			}
		}
		
		/* 这里是不可能运行到的地方 */
		return Humi_Value;
	}
	
#undef  CH_Actual_MAX_Value
}
#else
/*************************************************************************************************/
/* 函数名  	: Humi_Get_Cal_Value															 	 */
/* 输入		: CH_Actual_Value:真实值数据白指针;CH_Calibration_Value:校准值数据白指针				 */
/* 输出		: 当前通道校准点数量 																	 */
/* 作用		: 温度的检测值：Hum_Value；传感器通道编号：CH_NO；传感器校准数据表：						 */
/* 算法		：Ty = ((Ty1-Ty0)/(Tx1-Tx0))(Tx-Tx0)+Ty0；											 */
/* 创建日期	: 2014/10/27																		 */
/*************************************************************************************************/
float Humi_Get_Cal_Value(float Humi_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
/* 湿度传感器的最大参数值 */
#define  CH_Actual_MAX_Value	(100.0)
	u8 num;
	u8 cal_num;
	u16 humi_b;
	float humi_v;
	
#if 0
	/* 根据当前的温度数据确定数据所在的区间 */
	if(Humi_Value < CH_Actual_Value[0])
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
	if(Humi_Value == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* 卡片数据接收超时 */
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	else if(Humi_Value >= CH_Actual_MAX_Value)
	{
		/* 当前的数据大于最大值区间 */
		/* 将当前的温度设置成最高温度 */
		humi_v = CH_Actual_MAX_Value;
	}else
	{
		/* 当前的数据在测量的范围内 */
		humi_b = Humi_Value*10;
		
		/* 对当前通道真实值和原始值数据表进行排序 */
		cal_num = Humi_Cal_Data_Tab_Qsort(CH_Actual_Value,CH_Calibration_Value);
		
		/* 判断当前通道是否校准过 */
		if(cal_num == 0)
		{
			/* 当前通道没有校准过，因此校准值等于真实值 */
			humi_v = Humi_Value;
		}else if(cal_num == 1)
		{
			/* 当前通道校准过一个点 */
			/* 校准一个点时，对整条曲线进行调整 */
			humi_v = (((float)humi_b-(CH_Actual_Value_Buffer[0]-CH_Calibration_Value_Buffer[0]))/10);
		}else
		{
			/* 当前通道校准带你数量大于1个（小于8个） */
			for(num = 0 ; num < (cal_num+1) ; num ++)
			{
				/* 循环检测 当前的温度在哪段校准区间 */
				if(humi_b >= CH_Actual_Value_Buffer[0])
				{
					/* 判断 大于 最大的校准点 */
					humi_v = (((float)humi_b-((float)CH_Actual_Value_Buffer[0]-(float)CH_Calibration_Value_Buffer[0]))/10);
				}else if(humi_b < CH_Actual_Value_Buffer[cal_num-1])
				{
					/* 判断 小于 最大的校准点 */
					humi_v = (((float)humi_b-((float)CH_Actual_Value_Buffer[cal_num-1]-(float)CH_Calibration_Value_Buffer[cal_num-1]))/10);
				}else
				{
					/* 循环检测 当前的温度在哪段校准区间 */
					if((humi_b < CH_Actual_Value_Buffer[num])&&(humi_b >= CH_Actual_Value_Buffer[num+1]))
					{
						/* 返回 校准的数据 */
						humi_v = (((((((float)CH_Calibration_Value_Buffer[num])-((float)CH_Calibration_Value_Buffer[num+1]))/(((float)CH_Actual_Value_Buffer[num])-((float)CH_Actual_Value_Buffer[num+1])))*
							   (((float)humi_b)-((float)CH_Actual_Value_Buffer[num+1])))+CH_Calibration_Value_Buffer[num+1])/10);
					}
				}
			}
		}
	}
	
	/* 判断湿度值是否低于0%RH */
	if(humi_v < 0)
	{
		/* 低于0%RH，设置最低为0%RH */
		humi_v = 0;
	}else if(humi_v > 100)
	{
		/* 低于0%RH，设置最低为0%RH */
		humi_v = 100;
	}
	
#undef  CH_Actual_MAX_Value
	
	/* 返回参数 */
	return humi_v;	
}
#endif


/*************************************************************************************************/
/* 函数名	:Humi_Get_Sampl_Value															 	 */
/* 输入		:湿度传感器AD采集数据																	 */
/* 输出		:采样的湿度数据值（温度值*10）															 */
/* 作用		:将输入的ADC数据转换成湿度数据														 	 */
/* 创建日期	:2015/03/26																			 */
/* Voltage divide resistor Pull-Down 														 	 */
/*************************************************************************************************/
#if 0
u16 Humi_Get_Sampl_Value(u16 Adc_DR)
{
/* 湿度传感器 温度补偿 比例常数 */
#define	Humi_Constant_B		(1.0546)
/* 湿度传感器 温度补偿 比例系数 */
#define	Humi_Ratio_K		(0.00216)
/* 湿度传感器 精度 */
#define humi_Accuracy		(10u)
	
	u8  humi_tab_num;
	u16 humi;
	
		
	/* 根据湿度表来推算最接近的2个湿度值 */
	for(humi_tab_num = 0;humi_tab_num < (HIH_5030_Table_Buffer_Size-1); humi_tab_num+=humi_Accuracy)
	{
		if(HIH_5030[humi_tab_num] > Adc_DR)
		{
			break;
		}
	}

	/* 判断时间的温度是否超限 */
	if(humi_tab_num == (HIH_5030_Table_Buffer_Size-1))
	{
		//温度超出99%RH，后面可能会添加报警等功能；但是超出依然显示99%RH.
		//这个时候 说明 湿度传感器 故障
#if 1
		/* 这里就强制校正采样的数据回最大值 */
		Adc_DR = HIH_5030[humi_tab_num];
#else
		Humi_Sensor_Failure();
#endif  
	}else if(humi_tab_num == 0)
	{
		//温度低于0%RH，后面可能会添加报警等功能；低于依然显示0%RH.
		humi_tab_num = humi_Accuracy;
#if 1
		/* 这里就强制校正采样的数据回最小值 */
		Adc_DR = HIH_5030[0];
#else
		Humi_Sensor_Failure();
#endif
	}

	//根据计算的He值再换算事件的湿度值																									        					
	humi = ((((float)humi_Accuracy/(HIH_5030[humi_tab_num]-HIH_5030[humi_tab_num-humi_Accuracy]))*(Adc_DR - HIH_5030[humi_tab_num-humi_Accuracy])) + humi_tab_num - humi_Accuracy)*10;					
	//添加温度补偿
	/* 
		湿度（真实值）= 湿度（检测值）/（Humi_Constant_B-Humi_Ratio_K*T（当前温度））
	*/
	humi = ((float)humi/((float)Humi_Constant_B-(Humi_Ratio_K*Humi_Temp_Comp)));
	
	/* 返回整形格式的湿度数据 */
	return humi;
#undef	Humi_Constant_B
#undef	Humi_Ratio_K
#undef  humi_Accuracy
}
#else
/*************************************************************************************************/
/* 函数名	:Humi_Get_Sampl_Value															 	 */
/* 输入		:湿度传感器AD采集数据																	 */
/* 输出		:采样的湿度数据值（温度值*10）															 */
/* 作用		:将输入的ADC数据转换成湿度数据														 	 */
/* 创建日期	:2015/03/26																			 */
/* Voltage divide resistor Pull-Down 														 	 */
/*************************************************************************************************/
float Humi_Get_Sampl_Value(u16 Adc_DR)
{	
/* 数据表的 起始湿度点 */
#define HumiStart 		(0u)
/* 数据表的 结束温度点 */
#define HumiEnd 		(10u)
/* 数据表的 湿度查询精度 */
#define Scale      		(10.0)
/* 数据表温度查询范围 */
#define TableSize  		(HIH_5030_Table_Buffer_Size-1)

/* 湿度传感器 温度补偿 比例常数 */
#define	Humi_Comp_Constant_B	(1.0546)
/* 湿度传感器 温度补偿 比例系数 */
#define	Humi_Comp_Ratio_K		(0.00216)
/* 湿度传感器 精度 */
#define humi_Accuracy			(10u)
	
	/* 计数器 */
	u16 num;
	/* 数据表 搜索 开始 位置 */
	int16_t SearchStart = 0 ;
	/* 数据表 搜索 结束 位置 */
	int16_t SearchEnd 	= TableSize;
	/* 数据表 搜索 范围 */
	int16_t SearchMax 	= TableSize;
	/* 湿度值 */
	float humi_value;
	
	
	/* 判断当前的ADC数据是否超出，数据表的界限 */
	if(Adc_DR <= HIH_5030[SearchStart])
	{
		/* ADC值 小于 数据表的 起始 位置的值 */
		/* 输出起始位置的温度数据 */
		return ((SearchStart * Scale) + HumiStart);
	}else if(Adc_DR >= HIH_5030[SearchEnd])
	{
		/* ADC值 大于 数据表的 结束 位置的值 */
		/* 输出结束位置的温度数据 */
		return ((SearchEnd * Scale) + HumiStart);
	}
		
	/* 循环 搜索 当前的 ADC 在那个温度区间内（精度1℃） */
	for(num = 0 ; num < SearchMax ; num ++)
	{
		/* 判断 搜索的范围  */
		if((SearchEnd - SearchStart) > 1)
		{
			/* 搜索 范围大于 1℃ */
			/* 根据 当前 查询范围的中间点数据表值 */
			if(HIH_5030[ (SearchEnd + SearchStart) / 2 ] > Adc_DR)
			{
				/* 数据表值大于ADC值 */
				/* 设置 查询范围的结束点位置 */
				SearchEnd = ( SearchEnd + SearchStart ) / 2;
			}
			else if(HIH_5030[ ( SearchEnd + SearchStart ) / 2 ] < Adc_DR)
			{
				/* 数据表值小于ADC值 */
				/* 设置 查询范围的开始点位置 */
				SearchStart = ( SearchEnd + SearchStart ) / 2;
			}
			else
			{
				/* 数据表值等于ADC值 */
				/* 返回 温度数据 */
				humi_value = (((float)((SearchEnd + SearchStart) / 2) * Scale) + HumiStart);
				/* 跳出 搜索 */
				break;
			}
		}
		else
		{
			/* 搜索 范围小于 1℃ */
			/* 计数温度 */
			humi_value = (((((float)Adc_DR-(float)HIH_5030[SearchStart])/
							 ((float)HIH_5030[SearchEnd]-(float)HIH_5030[SearchStart]))+
							 (float)SearchStart)*humi_Accuracy+HumiStart);
			/* 跳出 搜索 */
			break;
		}
	}
	
	/* 
		湿度（真实值）= 湿度（检测值）/（Humi_Constant_B-Humi_Ratio_K*T（当前温度））
	*/
	humi_value = (humi_value/(Humi_Comp_Constant_B-(Humi_Comp_Ratio_K*Humi_Temp_Comp)));
	
	return humi_value;

/* 数据表的 起始湿度点 */
#undef HumiStart
/* 数据表的 结束温度点 */
#undef HumiEnd
/* 数据表的 湿度查询精度 */
#undef Scale 
/* 数据表温度查询范围 */
#undef TableSize
/* 湿度传感器 温度补偿 比例常数 */
#undef Humi_Comp_Constant_B
/* 湿度传感器 温度补偿 比例系数 */
#undef Humi_Comp_Ratio_K
/* 湿度传感器 精度 */
#undef humi_Accuracy
}
#endif


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

