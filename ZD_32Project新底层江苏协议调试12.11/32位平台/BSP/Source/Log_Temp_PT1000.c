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
/* PT1000�¶ȴ��������ݱ��С */
#define PT1000RelationTabSize		(19u)
/* PT1000�¶ȴ�����������ʼ�¶� */
#define PT1000MeasureStartTemp 		(-100.0f)
/* PT1000�¶ȴ��������������¶� */
#define PT1000MeasureEndTemp 		(80.0f)
/* PT1000�¶ȴ������������� */
#define PT1000RelationTabScale      (10.0f)

/* PT1000�¶ȴ������¶���ADC��ϵ�� */
static const u16 PT1000_Relation_Tab[PT1000RelationTabSize] = 
{
	{511}, //-100��
	{727}, //-90��
	{938}, //-80��
	{1143},//-70��
	{1343},//-60��
	{1539},//-50��
	{1730},//-40��
	{1917},//-30��
	{2100},//-20��
	{2279},//-10��
	{2455},//0��
	{2627},//10��
	{2795},//20��
	{2960},//30��
	{3121},//40��
	{3279},//50��
	{3433},//60��
	{3585},//70��
	{3735},//80��
};

#elif (MACRO_PT1000_Range == 0x01)

/*************************************************************************************************/
/* PT1000�¶ȴ��������ݱ��С */
#define PT1000RelationTabSize		(31u)
/* PT1000�¶ȴ�����������ʼ�¶� */
#define PT1000MeasureStartTemp 		(0.0f)
/* PT1000�¶ȴ��������������¶� */
#define PT1000MeasureEndTemp 		(300.0f)
/* PT1000�¶ȴ������������� */
#define PT1000RelationTabScale      (10.0f)

/* PT1000�¶ȴ������¶���ADC��ϵ�� */
static const u16 PT1000_Relation_Tab[PT1000RelationTabSize] = 
{
	{0},//0��
	{167},//10��
	{338},//20��
	{498},//30��
	{656},//40��
	{813},//50��
	{966},//60��
	{1116},//70��
	{1262},//80��
	{1408},//90��
	{1548},//100��
	{1691},//110��
	{1822},//120��
	{1956},//130��
	{2086},//140��
	{2217},//150��
	{2341},//160��
	{2466},//170��
	{2587},//180��
	{2707},//190��
	{2825},//200��
	{2940},//210��
	{3052},//220��
	{3165},//230��
	{3273},//240��
	{3381},//250��
	{3488},//260��
	{3590},//270��
	{3694},//280��
	{3793},//290��
	{3894},//300��
};


#endif


/*************************************************************************************************/

/*************************************************************************************************/
/* ������	:Temp_PT1000_Get_Sampl_Value															 	 */
/* ����		:�¶ȴ�����AD�ɼ�����																	 */
/* ���		:�������¶�����ֵ														 			 */
/* ����		:�������ADC����ת�����¶�����														 	 */
/* ��������	:2016/07/01																			 */
/*************************************************************************************************/
float Temp_PT1000_Get_Sampl_Value(u16 Adc_DR)
{	
	/* ������ */
	u16 num;
	/* ���ݱ� ���� ��ʼ λ�� */
	int16_t SearchStart = 0 ;
	/* ���ݱ� ���� ���� λ�� */
	int16_t SearchEnd 	= (PT1000RelationTabSize - 1);
	/* ���ݱ� ���� ��Χ */
	int16_t SearchMax 	= (PT1000RelationTabSize - 1);
	
	
	/* �жϵ�ǰ��ADC�����Ƿ񳬳������ݱ�Ľ��� */
	if(Adc_DR <= PT1000_Relation_Tab[SearchStart])
	{
		/* ADCֵ С�� ���ݱ�� ��ʼ λ�õ�ֵ */
		/* �����ʼλ�õ��¶����� */
		return (SearchStart * PT1000RelationTabScale + PT1000MeasureStartTemp);
	}else if(Adc_DR >= PT1000_Relation_Tab[SearchEnd])
	{
		/* ADCֵ ���� ���ݱ�� ���� λ�õ�ֵ */
		/* �������λ�õ��¶����� */
		return (SearchEnd * PT1000RelationTabScale + PT1000MeasureStartTemp);
	}
		
	/* ѭ�� ���� ��ǰ�� ADC ���Ǹ��¶������ڣ�����1�棩 */
	for(num = 0 ; num < SearchMax ; num ++)
	{
		/* �ж� �����ķ�Χ  */
		if((SearchEnd - SearchStart) > 1)
		{
			/* ���� ��Χ���� 1�� */
			/* ���� ��ǰ ��ѯ��Χ���м�����ݱ�ֵ */
			if(PT1000_Relation_Tab[ (SearchEnd + SearchStart) / 2 ] > Adc_DR)
			{
				/* ���ݱ�ֵ����ADCֵ */
				/* ���� ��ѯ��Χ�Ľ�����λ�� */
				SearchEnd = ( SearchEnd + SearchStart ) / 2;
			}
			else if(PT1000_Relation_Tab[ ( SearchEnd + SearchStart ) / 2 ] < Adc_DR)
			{
				/* ���ݱ�ֵС��ADCֵ */
				/* ���� ��ѯ��Χ�Ŀ�ʼ��λ�� */
				SearchStart = ( SearchEnd + SearchStart ) / 2;
			}
			else
			{
				/* ���ݱ�ֵ����ADCֵ */
				/* ���� �¶����� */
				return (((float)((SearchEnd + SearchStart) / 2) * PT1000RelationTabScale) + PT1000MeasureStartTemp);
			}
		}
		else
		{
			/* ���� ��ΧС�� 1�� */
			/* �����¶� */
			return (((((float)Adc_DR-(float)PT1000_Relation_Tab[SearchStart])/
					  ((float)PT1000_Relation_Tab[SearchEnd]-(float)PT1000_Relation_Tab[SearchStart]))+
					   (float)SearchStart)*PT1000RelationTabScale+PT1000MeasureStartTemp);
		}
	}
	
	/* ���ﲻ�������е��� */
	return 0;
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

