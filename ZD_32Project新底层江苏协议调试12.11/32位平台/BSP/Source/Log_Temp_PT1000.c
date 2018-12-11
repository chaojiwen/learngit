/**
  ******************************************************************************
  * @file              : Log_Temp_PT1000.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 07/01/2016 14:41:56
  * @brief             : c file
  * @description       : PT1000 Device Driver
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
  */
#include "System_Config.h"

#include "Log_Temp_PT1000.h"


#if (MACRO_PT1000_Range == 0x00)

/*************************************************************************************************/
/* PT1000温度传感器数据表大小 */
#define PT1000RelationTabSize		(19u)
/* PT1000温度传感器测量起始温度 */
#define PT1000MeasureStartTemp 		(-100.0f)
/* PT1000温度传感器测量结束温度 */
#define PT1000MeasureEndTemp 		(80.0f)
/* PT1000温度传感器测量精度 */
#define PT1000RelationTabScale      (10.0f)

/* PT1000温度传感器温度与ADC关系表 */
static const u16 PT1000_Relation_Tab[PT1000RelationTabSize] = 
{
	{511}, //-100℃
	{727}, //-90℃
	{938}, //-80℃
	{1143},//-70℃
	{1343},//-60℃
	{1539},//-50℃
	{1730},//-40℃
	{1917},//-30℃
	{2100},//-20℃
	{2279},//-10℃
	{2455},//0℃
	{2627},//10℃
	{2795},//20℃
	{2960},//30℃
	{3121},//40℃
	{3279},//50℃
	{3433},//60℃
	{3585},//70℃
	{3735},//80℃
};

#elif (MACRO_PT1000_Range == 0x01)

/*************************************************************************************************/
/* PT1000温度传感器数据表大小 */
#define PT1000RelationTabSize		(31u)
/* PT1000温度传感器测量起始温度 */
#define PT1000MeasureStartTemp 		(0.0f)
/* PT1000温度传感器测量结束温度 */
#define PT1000MeasureEndTemp 		(300.0f)
/* PT1000温度传感器测量精度 */
#define PT1000RelationTabScale      (10.0f)

/* PT1000温度传感器温度与ADC关系表 */
static const u16 PT1000_Relation_Tab[PT1000RelationTabSize] = 
{
	{0},//0℃
	{167},//10℃
	{338},//20℃
	{498},//30℃
	{656},//40℃
	{813},//50℃
	{966},//60℃
	{1116},//70℃
	{1262},//80℃
	{1408},//90℃
	{1548},//100℃
	{1691},//110℃
	{1822},//120℃
	{1956},//130℃
	{2086},//140℃
	{2217},//150℃
	{2341},//160℃
	{2466},//170℃
	{2587},//180℃
	{2707},//190℃
	{2825},//200℃
	{2940},//210℃
	{3052},//220℃
	{3165},//230℃
	{3273},//240℃
	{3381},//250℃
	{3488},//260℃
	{3590},//270℃
	{3694},//280℃
	{3793},//290℃
	{3894},//300℃
};


#endif


/*************************************************************************************************/

/*************************************************************************************************/
/* 函数名	:Temp_PT1000_Get_Sampl_Value															 	 */
/* 输入		:温度传感器AD采集数据																	 */
/* 输出		:采样的温度数据值														 			 */
/* 作用		:将输入的ADC数据转换成温度数据														 	 */
/* 创建日期	:2016/07/01																			 */
/*************************************************************************************************/
float Temp_PT1000_Get_Sampl_Value(u16 Adc_DR)
{	
	/* 计数器 */
	u16 num;
	/* 数据表 搜索 开始 位置 */
	int16_t SearchStart = 0 ;
	/* 数据表 搜索 结束 位置 */
	int16_t SearchEnd 	= (PT1000RelationTabSize - 1);
	/* 数据表 搜索 范围 */
	int16_t SearchMax 	= (PT1000RelationTabSize - 1);
	
	
	/* 判断当前的ADC数据是否超出，数据表的界限 */
	if(Adc_DR <= PT1000_Relation_Tab[SearchStart])
	{
		/* ADC值 小于 数据表的 起始 位置的值 */
		/* 输出起始位置的温度数据 */
		return (SearchStart * PT1000RelationTabScale + PT1000MeasureStartTemp);
	}else if(Adc_DR >= PT1000_Relation_Tab[SearchEnd])
	{
		/* ADC值 大于 数据表的 结束 位置的值 */
		/* 输出结束位置的温度数据 */
		return (SearchEnd * PT1000RelationTabScale + PT1000MeasureStartTemp);
	}
		
	/* 循环 搜索 当前的 ADC 在那个温度区间内（精度1℃） */
	for(num = 0 ; num < SearchMax ; num ++)
	{
		/* 判断 搜索的范围  */
		if((SearchEnd - SearchStart) > 1)
		{
			/* 搜索 范围大于 1℃ */
			/* 根据 当前 查询范围的中间点数据表值 */
			if(PT1000_Relation_Tab[ (SearchEnd + SearchStart) / 2 ] > Adc_DR)
			{
				/* 数据表值大于ADC值 */
				/* 设置 查询范围的结束点位置 */
				SearchEnd = ( SearchEnd + SearchStart ) / 2;
			}
			else if(PT1000_Relation_Tab[ ( SearchEnd + SearchStart ) / 2 ] < Adc_DR)
			{
				/* 数据表值小于ADC值 */
				/* 设置 查询范围的开始点位置 */
				SearchStart = ( SearchEnd + SearchStart ) / 2;
			}
			else
			{
				/* 数据表值等于ADC值 */
				/* 返回 温度数据 */
				return (((float)((SearchEnd + SearchStart) / 2) * PT1000RelationTabScale) + PT1000MeasureStartTemp);
			}
		}
		else
		{
			/* 搜索 范围小于 1℃ */
			/* 计数温度 */
			return (((((float)Adc_DR-(float)PT1000_Relation_Tab[SearchStart])/
					  ((float)PT1000_Relation_Tab[SearchEnd]-(float)PT1000_Relation_Tab[SearchStart]))+
					   (float)SearchStart)*PT1000RelationTabScale+PT1000MeasureStartTemp);
		}
	}
	
	/* 这里不可能运行到。 */
	return 0;
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

