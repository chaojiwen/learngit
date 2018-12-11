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
/* ʪ�ȴ�����Ĭ�ϵ����ݱ��С */
#define HIH_5030_Table_Buffer_Size		(11u)
/* ����Τ�� ʪ��ʪ�ȴ����� 5030 ADC���ݱ� */
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
/* ʪ�ȴ������¶Ȳ������� */
static float Humi_Temp_Comp;

/* ��ʵֵ ���� ��ı� */
static u16 CH_Actual_Value_Buffer		[INSTRU_SENSOR_Data_Tab_Size];
/* ԭʼֵ ���� ��ı� */
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
/* ������  	: Humi_Get_Temp_Comp_Value															 */
/* ����		: sensor_t:������ͨ������ֵ��sensor_f���¶ȴ�����У׼ֵ									 */
/* ���		: ��			 																	 */
/* ����		: ���ʪ�ȴ������¶Ȳ�������															 */
/* ��������	: 2014/10/27																		 */
/*************************************************************************************************/
void Humi_Get_Temp_Comp_Value(void)
{
/* ���� */
#define	Ambient_Temp	(25.0)


/* �ж� ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� �ڼ� ͨ�� */
#if (Hum_Sensor_Tmp_Comp_Enable == 0X01)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��1 */
	/* ��� ͨ��1 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X02)	
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��2 */
	/* ��� ͨ��2 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X03)	
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��3 */
	/* ��� ͨ��3 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X04)	
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��4 */
	/* ��� ͨ��4 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X05)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��5 */
	/* ��� ͨ��5 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X06)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��6 */
	/* ��� ͨ��6 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X07)	
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��7 */	
	/* ��� ͨ��7 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X08)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��8 */
	/* ��� ͨ��8 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X09)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��9 */
	/* ��� ͨ��9 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0A)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��10 */
	/* ��� ͨ��10 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0B)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��11 */
	/* ��� ͨ��11 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0C)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��12 */
	/* ��� ͨ��12 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0D)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��13 */
	/* ��� ͨ��13 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0E)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��14 */
	/* ��� ͨ��14 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X0F)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��15 */
	/* ��� ͨ��15 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X10)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��16 */
	/* ��� ͨ��16 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X11)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��17 */
	/* ��� ͨ��17 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X12)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��18 */
	/* ��� ͨ��18 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X13)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��19 */
	/* ��� ͨ��19 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X14)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��20 */
	/* ��� ͨ��20 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X15)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��21 */
	/* ��� ͨ��21 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X16)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��22 */
	/* ��� ͨ��22 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X17)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��23 */
	/* ��� ͨ��23 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X18)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��24 */
	/* ��� ͨ��24 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X19)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��25 */
	/* ��� ͨ��25 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1A)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��26 */
	/* ��� ͨ��26 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1B)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��27 */
	/* ��� ͨ��27 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1C)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��28 */
	/* ��� ͨ��28 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1D)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��29 */
	/* ��� ͨ��29 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1E)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��30 */
	/* ��� ͨ��30 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X1F)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��31 */
	/* ��� ͨ��31 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data;
#elif (Hum_Sensor_Tmp_Comp_Enable == 0X20)
	/* ʪ�ȴ������¶Ȳ����Ĵ�����ѡ�� ͨ��32 */
	/* ��� ͨ��32 ���¶ȴ�����ֵ */
	Humi_Temp_Comp = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data;
#else
	#error "ʪ�ȴ�����������ͨ���������ô���."
#endif	

#undef	Ambient_Temp
}
#endif


/*************************************************************************************************/
/* ������  	: Humi_Cal_Data_Tab_Qsort															 */
/* ����		: CH_Actual_Value:��ʵֵ���ݰ�ָ��;CH_Calibration_Value:У׼ֵ���ݰ�ָ��				 */
/* ���		: ��ǰͨ��У׼������ 																	 */
/* ����		: �Ե�ǰͨ����ʵֵ��ԭʼֵ���ݱ��������													 */
/* ��������	: 2014/10/27																		 */
/*************************************************************************************************/
static u8 Humi_Cal_Data_Tab_Qsort(u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
	/* У׼������� */
	u8 cal_num;
	
	/* ��� ���� */
	memset(CH_Actual_Value_Buffer,0x00,sizeof(CH_Actual_Value_Buffer)); 
	memset(CH_Calibration_Value_Buffer,0x00,sizeof(CH_Calibration_Value_Buffer)); 
	/* �����ݻ������ݸ�ֵ����ʱ���������� */
	memcpy(CH_Actual_Value_Buffer,CH_Actual_Value,sizeof(CH_Actual_Value_Buffer)); 
	/* ����ʵֵ���ݱ����� */
	qsort(CH_Actual_Value_Buffer,INSTRU_SENSOR_Data_Tab_Size,sizeof(u16),Comp);
	/* �����ݻ������ݸ�ֵ����ʱ���������� */
	memcpy(CH_Calibration_Value_Buffer,CH_Calibration_Value,sizeof(CH_Calibration_Value_Buffer)); 
	/* ����ʵֵ���ݱ����� */
	qsort(CH_Calibration_Value_Buffer,INSTRU_SENSOR_Data_Tab_Size,sizeof(u16),Comp);
	
	/* ѭ���ж� ��ǰͨ��У׼������ */
	for(cal_num = 0 ; cal_num < INSTRU_SENSOR_Data_Tab_Size ; cal_num ++)
	{
		/* �ж� ��ǰͨ��У׼������ */
		if(CH_Actual_Value_Buffer[cal_num] == 0)
		{
			/* ���ڵ�ǰ�������ǴӴ�С����˵��жϵ��е���0��Bufferʱ��˵���Ѿ�����С����ֵ�� */
			/* ����ѭ���ж� */
			break;
		}
	}
	/* ���ص�ǰͨ����У׼�������� */
	return cal_num;
}


#if (0)
/*************************************************************************************************/
/* ������  	: Humi_Get_Cal_Value															 	 */
/* ����		: CH_Actual_Value:��ʵֵ���ݰ�ָ��;CH_Calibration_Value:У׼ֵ���ݰ�ָ��				 */
/* ���		: ��ǰͨ��У׼������ 																	 */
/* ����		: �¶ȵļ��ֵ��Hum_Value��������ͨ����ţ�CH_NO��������У׼���ݱ�						 */
/* �㷨		��Ty = ((Ty1-Ty0)/(Tx1-Tx0))(Tx-Tx0)+Ty0��											 */
/* ��������	: 2014/10/27																		 */
/*************************************************************************************************/
float Humi_Get_Cal_Value(float Humi_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
/* ʪ�ȴ�������������ֵ */
#define  CH_Actual_MAX_Value	(100.0)
	u8 num;
	u8 cal_num;
#if 0
	/* ���ݵ�ǰ���¶�����ȷ���������ڵ����� */
	if(Humi_Value < CH_Actual_Value[0])
	{
		/* ��ǰ������С�����ֵ���� */
		/* ����ǰ���¶����ó�����¶� */
		return CH_Actual_Value[0];
	}
#endif
	
	/* 
		�ж�����״̬��
		1����Ƭ���ݽ��ճ�ʱ��
		2�����ݳ���������Χ���������߼�û�����������
		...
	*/
	if(Humi_Value == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* ��Ƭ���ݽ��ճ�ʱ */
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	else if(Humi_Value >= CH_Actual_MAX_Value)
	{
		/* ��ǰ�����ݴ������ֵ���� */
		/* ����ǰ���¶����ó�����¶� */
		return CH_Actual_MAX_Value;
	}else
	{
		/* ��ǰ�������ڲ����ķ�Χ�� */
		/* ����ʵֵ�� */
		
		/* �Ե�ǰͨ����ʵֵ��ԭʼֵ���ݱ�������� */
		cal_num = Humi_Cal_Data_Tab_Qsort(CH_Actual_Value,CH_Calibration_Value);
		
		/* �жϵ�ǰͨ���Ƿ�У׼�� */
		if(cal_num == 0)
		{
			/* ��ǰͨ��û��У׼�������У׼ֵ������ʵֵ */
			return Humi_Value;
		}else if(cal_num == 1)
		{
			/* ��ǰͨ��У׼��һ���� */
			/* У׼һ����ʱ�����������߽��е��� */
			return (Humi_Value-(CH_Actual_Value_Buffer[0]-CH_Calibration_Value_Buffer[0]));
		}else
		{
			/* ��ǰͨ��У׼������������1����С��8���� */
			for(num = 0 ; num < (cal_num+1) ; num ++)
			{
				/* ѭ����� ��ǰ���¶����Ķ�У׼���� */
				if(Humi_Value >= CH_Actual_Value_Buffer[0])
				{
					/* �ж� ���� ����У׼�� */
					return (Humi_Value-(CH_Actual_Value_Buffer[0])-CH_Calibration_Value_Buffer[0]));
				}else if(Humi_Value < CH_Actual_Value_Buffer[cal_num-1])
				{
					/* �ж� ���� ����У׼�� */
					return (Humi_Value-(CH_Actual_Value_Buffer[cal_num-1])-CH_Calibration_Value_Buffer[cal_num-1]));
				}else
				{
					/* ѭ����� ��ǰ���¶����Ķ�У׼���� */
					if((Humi_Value < CH_Actual_Value_Buffer[num])&&(Humi_Value >= CH_Actual_Value_Buffer[num+1]))
					{
						/* ���� У׼������ */
						return((((CH_Calibration_Value_Buffer[num]-CH_Calibration_Value_Buffer[num+1])/(CH_Actual_Value_Buffer[num]-CH_Actual_Value_Buffer[num+1]))*
							   (Humi_Value-CH_Actual_Value_Buffer[num+1]))+CH_Calibration_Value_Buffer[num+1]);
					}
				}
			}
		}
		
		/* �����ǲ��������е��ĵط� */
		return Humi_Value;
	}
	
#undef  CH_Actual_MAX_Value
}
#else
/*************************************************************************************************/
/* ������  	: Humi_Get_Cal_Value															 	 */
/* ����		: CH_Actual_Value:��ʵֵ���ݰ�ָ��;CH_Calibration_Value:У׼ֵ���ݰ�ָ��				 */
/* ���		: ��ǰͨ��У׼������ 																	 */
/* ����		: �¶ȵļ��ֵ��Hum_Value��������ͨ����ţ�CH_NO��������У׼���ݱ�						 */
/* �㷨		��Ty = ((Ty1-Ty0)/(Tx1-Tx0))(Tx-Tx0)+Ty0��											 */
/* ��������	: 2014/10/27																		 */
/*************************************************************************************************/
float Humi_Get_Cal_Value(float Humi_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
/* ʪ�ȴ�������������ֵ */
#define  CH_Actual_MAX_Value	(100.0)
	u8 num;
	u8 cal_num;
	u16 humi_b;
	float humi_v;
	
#if 0
	/* ���ݵ�ǰ���¶�����ȷ���������ڵ����� */
	if(Humi_Value < CH_Actual_Value[0])
	{
		/* ��ǰ������С�����ֵ���� */
		/* ����ǰ���¶����ó�����¶� */
		return CH_Actual_Value[0];
	}
#endif
	
	/* 
		�ж�����״̬��
		1����Ƭ���ݽ��ճ�ʱ��
		2�����ݳ���������Χ���������߼�û�����������
		...
	*/
	if(Humi_Value == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* ��Ƭ���ݽ��ճ�ʱ */
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	else if(Humi_Value >= CH_Actual_MAX_Value)
	{
		/* ��ǰ�����ݴ������ֵ���� */
		/* ����ǰ���¶����ó�����¶� */
		humi_v = CH_Actual_MAX_Value;
	}else
	{
		/* ��ǰ�������ڲ����ķ�Χ�� */
		humi_b = Humi_Value*10;
		
		/* �Ե�ǰͨ����ʵֵ��ԭʼֵ���ݱ�������� */
		cal_num = Humi_Cal_Data_Tab_Qsort(CH_Actual_Value,CH_Calibration_Value);
		
		/* �жϵ�ǰͨ���Ƿ�У׼�� */
		if(cal_num == 0)
		{
			/* ��ǰͨ��û��У׼�������У׼ֵ������ʵֵ */
			humi_v = Humi_Value;
		}else if(cal_num == 1)
		{
			/* ��ǰͨ��У׼��һ���� */
			/* У׼һ����ʱ�����������߽��е��� */
			humi_v = (((float)humi_b-(CH_Actual_Value_Buffer[0]-CH_Calibration_Value_Buffer[0]))/10);
		}else
		{
			/* ��ǰͨ��У׼������������1����С��8���� */
			for(num = 0 ; num < (cal_num+1) ; num ++)
			{
				/* ѭ����� ��ǰ���¶����Ķ�У׼���� */
				if(humi_b >= CH_Actual_Value_Buffer[0])
				{
					/* �ж� ���� ����У׼�� */
					humi_v = (((float)humi_b-((float)CH_Actual_Value_Buffer[0]-(float)CH_Calibration_Value_Buffer[0]))/10);
				}else if(humi_b < CH_Actual_Value_Buffer[cal_num-1])
				{
					/* �ж� С�� ����У׼�� */
					humi_v = (((float)humi_b-((float)CH_Actual_Value_Buffer[cal_num-1]-(float)CH_Calibration_Value_Buffer[cal_num-1]))/10);
				}else
				{
					/* ѭ����� ��ǰ���¶����Ķ�У׼���� */
					if((humi_b < CH_Actual_Value_Buffer[num])&&(humi_b >= CH_Actual_Value_Buffer[num+1]))
					{
						/* ���� У׼������ */
						humi_v = (((((((float)CH_Calibration_Value_Buffer[num])-((float)CH_Calibration_Value_Buffer[num+1]))/(((float)CH_Actual_Value_Buffer[num])-((float)CH_Actual_Value_Buffer[num+1])))*
							   (((float)humi_b)-((float)CH_Actual_Value_Buffer[num+1])))+CH_Calibration_Value_Buffer[num+1])/10);
					}
				}
			}
		}
	}
	
	/* �ж�ʪ��ֵ�Ƿ����0%RH */
	if(humi_v < 0)
	{
		/* ����0%RH���������Ϊ0%RH */
		humi_v = 0;
	}else if(humi_v > 100)
	{
		/* ����0%RH���������Ϊ0%RH */
		humi_v = 100;
	}
	
#undef  CH_Actual_MAX_Value
	
	/* ���ز��� */
	return humi_v;	
}
#endif


/*************************************************************************************************/
/* ������	:Humi_Get_Sampl_Value															 	 */
/* ����		:ʪ�ȴ�����AD�ɼ�����																	 */
/* ���		:������ʪ������ֵ���¶�ֵ*10��															 */
/* ����		:�������ADC����ת����ʪ������														 	 */
/* ��������	:2015/03/26																			 */
/* Voltage divide resistor Pull-Down 														 	 */
/*************************************************************************************************/
#if 0
u16 Humi_Get_Sampl_Value(u16 Adc_DR)
{
/* ʪ�ȴ����� �¶Ȳ��� �������� */
#define	Humi_Constant_B		(1.0546)
/* ʪ�ȴ����� �¶Ȳ��� ����ϵ�� */
#define	Humi_Ratio_K		(0.00216)
/* ʪ�ȴ����� ���� */
#define humi_Accuracy		(10u)
	
	u8  humi_tab_num;
	u16 humi;
	
		
	/* ����ʪ�ȱ���������ӽ���2��ʪ��ֵ */
	for(humi_tab_num = 0;humi_tab_num < (HIH_5030_Table_Buffer_Size-1); humi_tab_num+=humi_Accuracy)
	{
		if(HIH_5030[humi_tab_num] > Adc_DR)
		{
			break;
		}
	}

	/* �ж�ʱ����¶��Ƿ��� */
	if(humi_tab_num == (HIH_5030_Table_Buffer_Size-1))
	{
		//�¶ȳ���99%RH��������ܻ���ӱ����ȹ��ܣ����ǳ�����Ȼ��ʾ99%RH.
		//���ʱ�� ˵�� ʪ�ȴ����� ����
#if 1
		/* �����ǿ��У�����������ݻ����ֵ */
		Adc_DR = HIH_5030[humi_tab_num];
#else
		Humi_Sensor_Failure();
#endif  
	}else if(humi_tab_num == 0)
	{
		//�¶ȵ���0%RH��������ܻ���ӱ����ȹ��ܣ�������Ȼ��ʾ0%RH.
		humi_tab_num = humi_Accuracy;
#if 1
		/* �����ǿ��У�����������ݻ���Сֵ */
		Adc_DR = HIH_5030[0];
#else
		Humi_Sensor_Failure();
#endif
	}

	//���ݼ����Heֵ�ٻ����¼���ʪ��ֵ																									        					
	humi = ((((float)humi_Accuracy/(HIH_5030[humi_tab_num]-HIH_5030[humi_tab_num-humi_Accuracy]))*(Adc_DR - HIH_5030[humi_tab_num-humi_Accuracy])) + humi_tab_num - humi_Accuracy)*10;					
	//����¶Ȳ���
	/* 
		ʪ�ȣ���ʵֵ��= ʪ�ȣ����ֵ��/��Humi_Constant_B-Humi_Ratio_K*T����ǰ�¶ȣ���
	*/
	humi = ((float)humi/((float)Humi_Constant_B-(Humi_Ratio_K*Humi_Temp_Comp)));
	
	/* �������θ�ʽ��ʪ������ */
	return humi;
#undef	Humi_Constant_B
#undef	Humi_Ratio_K
#undef  humi_Accuracy
}
#else
/*************************************************************************************************/
/* ������	:Humi_Get_Sampl_Value															 	 */
/* ����		:ʪ�ȴ�����AD�ɼ�����																	 */
/* ���		:������ʪ������ֵ���¶�ֵ*10��															 */
/* ����		:�������ADC����ת����ʪ������														 	 */
/* ��������	:2015/03/26																			 */
/* Voltage divide resistor Pull-Down 														 	 */
/*************************************************************************************************/
float Humi_Get_Sampl_Value(u16 Adc_DR)
{	
/* ���ݱ�� ��ʼʪ�ȵ� */
#define HumiStart 		(0u)
/* ���ݱ�� �����¶ȵ� */
#define HumiEnd 		(10u)
/* ���ݱ�� ʪ�Ȳ�ѯ���� */
#define Scale      		(10.0)
/* ���ݱ��¶Ȳ�ѯ��Χ */
#define TableSize  		(HIH_5030_Table_Buffer_Size-1)

/* ʪ�ȴ����� �¶Ȳ��� �������� */
#define	Humi_Comp_Constant_B	(1.0546)
/* ʪ�ȴ����� �¶Ȳ��� ����ϵ�� */
#define	Humi_Comp_Ratio_K		(0.00216)
/* ʪ�ȴ����� ���� */
#define humi_Accuracy			(10u)
	
	/* ������ */
	u16 num;
	/* ���ݱ� ���� ��ʼ λ�� */
	int16_t SearchStart = 0 ;
	/* ���ݱ� ���� ���� λ�� */
	int16_t SearchEnd 	= TableSize;
	/* ���ݱ� ���� ��Χ */
	int16_t SearchMax 	= TableSize;
	/* ʪ��ֵ */
	float humi_value;
	
	
	/* �жϵ�ǰ��ADC�����Ƿ񳬳������ݱ�Ľ��� */
	if(Adc_DR <= HIH_5030[SearchStart])
	{
		/* ADCֵ С�� ���ݱ�� ��ʼ λ�õ�ֵ */
		/* �����ʼλ�õ��¶����� */
		return ((SearchStart * Scale) + HumiStart);
	}else if(Adc_DR >= HIH_5030[SearchEnd])
	{
		/* ADCֵ ���� ���ݱ�� ���� λ�õ�ֵ */
		/* �������λ�õ��¶����� */
		return ((SearchEnd * Scale) + HumiStart);
	}
		
	/* ѭ�� ���� ��ǰ�� ADC ���Ǹ��¶������ڣ�����1�棩 */
	for(num = 0 ; num < SearchMax ; num ++)
	{
		/* �ж� �����ķ�Χ  */
		if((SearchEnd - SearchStart) > 1)
		{
			/* ���� ��Χ���� 1�� */
			/* ���� ��ǰ ��ѯ��Χ���м�����ݱ�ֵ */
			if(HIH_5030[ (SearchEnd + SearchStart) / 2 ] > Adc_DR)
			{
				/* ���ݱ�ֵ����ADCֵ */
				/* ���� ��ѯ��Χ�Ľ�����λ�� */
				SearchEnd = ( SearchEnd + SearchStart ) / 2;
			}
			else if(HIH_5030[ ( SearchEnd + SearchStart ) / 2 ] < Adc_DR)
			{
				/* ���ݱ�ֵС��ADCֵ */
				/* ���� ��ѯ��Χ�Ŀ�ʼ��λ�� */
				SearchStart = ( SearchEnd + SearchStart ) / 2;
			}
			else
			{
				/* ���ݱ�ֵ����ADCֵ */
				/* ���� �¶����� */
				humi_value = (((float)((SearchEnd + SearchStart) / 2) * Scale) + HumiStart);
				/* ���� ���� */
				break;
			}
		}
		else
		{
			/* ���� ��ΧС�� 1�� */
			/* �����¶� */
			humi_value = (((((float)Adc_DR-(float)HIH_5030[SearchStart])/
							 ((float)HIH_5030[SearchEnd]-(float)HIH_5030[SearchStart]))+
							 (float)SearchStart)*humi_Accuracy+HumiStart);
			/* ���� ���� */
			break;
		}
	}
	
	/* 
		ʪ�ȣ���ʵֵ��= ʪ�ȣ����ֵ��/��Humi_Constant_B-Humi_Ratio_K*T����ǰ�¶ȣ���
	*/
	humi_value = (humi_value/(Humi_Comp_Constant_B-(Humi_Comp_Ratio_K*Humi_Temp_Comp)));
	
	return humi_value;

/* ���ݱ�� ��ʼʪ�ȵ� */
#undef HumiStart
/* ���ݱ�� �����¶ȵ� */
#undef HumiEnd
/* ���ݱ�� ʪ�Ȳ�ѯ���� */
#undef Scale 
/* ���ݱ��¶Ȳ�ѯ��Χ */
#undef TableSize
/* ʪ�ȴ����� �¶Ȳ��� �������� */
#undef Humi_Comp_Constant_B
/* ʪ�ȴ����� �¶Ȳ��� ����ϵ�� */
#undef Humi_Comp_Ratio_K
/* ʪ�ȴ����� ���� */
#undef humi_Accuracy
}
#endif


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

