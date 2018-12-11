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
/* �¶ȴ�����Ĭ�ϵ����ݱ� */
#define NTC_10K_3950_Table_Buffer_Size	(231u)
/* �¶ȶ�Ӧ��ADCֵ��NTC���ݱ�3950�� */
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
/* ��ʵֵ ���� ��ı� */
static u16 CH_Actual_Value_Buffer		[INSTRU_SENSOR_Data_Tab_Size];
/* ԭʼֵ ���� ��ı� */
static u16 CH_Calibration_Value_Buffer	[INSTRU_SENSOR_Data_Tab_Size];
/* �ļ��������� */
static u8 Temp_Cal_Data_Tab_Qsort(u16* CH_Actual_Value,u16* CH_Calibration_Value);
/*************************************************************************************************/

static int 	Comp(const void *p1,const void *p2)
{
	return *((u16*)p2) > *((u16*)p1)?1:-1; 	
}


/*************************************************************************************************/
/* ������  	: Temp_Cal_Data_Tab_Qsort															 */
/* ����		: CH_Actual_Value:��ʵֵ���ݰ�ָ��;CH_Calibration_Value:У׼ֵ���ݰ�ָ��				 */
/* ���		: ��ǰͨ��У׼������ 																	 */
/* ����		: �Ե�ǰͨ����ʵֵ��ԭʼֵ���ݱ��������													 */
/* ��������	: 2014/10/27																		 */
/*************************************************************************************************/
static u8 Temp_Cal_Data_Tab_Qsort(u16* CH_Actual_Value,u16* CH_Calibration_Value)
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
/********************************************************************************************************/
/* ������  	: Temp_Get_Cal_Value																 		*/
/* ����		: Temp_Value:�¶���ʵֵCH_Actual_Value:��ʵֵ���ݰ�ָ��;CH_Calibration_Value:У׼ֵ���ݰ�ָ��	*/
/* ���		: �¶�У׼ֵ 																	 			*/
/* ����		: ���ݵ�ǰͨ����У׼���ݱ����ã����¶���ʵֵת����У׼ֵ���								 		*/
/* ��������	: 2015/03/25																		 		*/
/* �㷨		: Ty = ((Ty1-Ty0)/(Tx1-Tx0))(Tx-Tx0)+Ty0											 		*/
/********************************************************************************************************/
float Temp_Get_Cal_Value(float Temp_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
/* �¶ȴ�������������ֵ */
#define  CH_Actual_MAX_Value	(300.0)
	u8 num;
	u8 cal_num;
#if 0
	/* ���ݵ�ǰ���¶�����ȷ���������ڵ����� */
	if(Temp_Value < CH_Actual_Value[0])
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
	if(Temp_Value == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* ��Ƭ���ݽ��ճ�ʱ */
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	else if(Temp_Value >= CH_Actual_MAX_Value)
	{
		/* ��ǰ�����ݴ������ֵ���� */
		/* ����ǰ���¶����ó�����¶� */
		return CH_Actual_MAX_Value;
	}else
	{
		/* ��ǰ�������ڲ����ķ�Χ�� */
		/* ����ʵֵ�� */
		
		/* �Ե�ǰͨ����ʵֵ��ԭʼֵ���ݱ�������� */
		cal_num = Temp_Cal_Data_Tab_Qsort(CH_Actual_Value,CH_Calibration_Value);
		
		/* �жϵ�ǰͨ���Ƿ�У׼�� */
		if(cal_num == 0)
		{
			/* ��ǰͨ��û��У׼�������У׼ֵ������ʵֵ */
			return Temp_Value;
		}else if(cal_num == 1)
		{
			/* ��ǰͨ��У׼��һ���� */
			/* У׼һ����ʱ�����������߽��е��� */
			return (Temp_Value-(CH_Actual_Value_Buffer[0]-CH_Calibration_Value_Buffer[0]));
		}else
		{
			/* ��ǰͨ��У׼��������1����С��8���� */
			for(num = 0 ; num < (cal_num+1) ; num ++)
			{
				/* ѭ����� ��ǰ���¶����Ķ�У׼���� */
				if(Temp_Value >= CH_Actual_Value_Buffer[0])
				{
					/* �ж� ���� ����У׼�� */
					return (Temp_Value-(CH_Actual_Value_Buffer[0]-CH_Calibration_Value_Buffer[0]));
				}else if(Temp_Value < CH_Actual_Value_Buffer[cal_num-1])
				{
					/* �ж� ���� ����У׼�� */
					return (Temp_Value-(CH_Actual_Value_Buffer[cal_num-1]-CH_Calibration_Value_Buffer[cal_num-1]));
				}else
				{
					/* ѭ����� ��ǰ���¶����Ķ�У׼���� */
					if((Temp_Value < (CH_Actual_Value_Buffer[num])&&(Temp_Value >= (CH_Actual_Value_Buffer[num+1]))
					{
						/* ���� У׼������ */
						return((((CH_Calibration_Value_Buffer[num]-CH_Calibration_Value_Buffer[num+1])/(CH_Actual_Value_Buffer[num]-CH_Actual_Value_Buffer[num+1]))*
							   (Temp_Value-CH_Actual_Value_Buffer[num+1]))+CH_Calibration_Value_Buffer[num+1]);
					}
				}
			}
		}
		
		/* �����ǲ��������е��ĵط� */
		return Temp_Value;
	}
	
#undef  CH_Actual_MAX_Value
}

#else
/********************************************************************************************************/
/* ������  	: Temp_Get_Cal_Value																 		*/
/* ����		: Temp_Value:�¶���ʵֵCH_Actual_Value:��ʵֵ���ݰ�ָ��;CH_Calibration_Value:У׼ֵ���ݰ�ָ��	*/
/* ���		: �¶�У׼ֵ 																	 			*/
/* ����		: ���ݵ�ǰͨ����У׼���ݱ����ã����¶���ʵֵת����У׼ֵ���								 	*/
/* ��������	: 2015/03/25																		 		*/
/* �㷨		: Ty = ((Ty1-Ty0)/(Tx1-Tx0))(Tx-Tx0)+Ty0											 		*/
/********************************************************************************************************/
float Temp_Get_Cal_Value(float Temp_Value,u16* CH_Actual_Value,u16* CH_Calibration_Value)
{
/* �¶ȴ�������������ֵ */
#define  CH_Actual_MAX_Value	(300.0)
	u8 num;
	u8 cal_num;
	u16 temp_b;

#if 0
	/* ���ݵ�ǰ���¶�����ȷ���������ڵ����� */
	if(Temp_Value < CH_Actual_Value[0])
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
	if(Temp_Value == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* ��Ƭ���ݽ��ճ�ʱ */
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	else if(Temp_Value >= CH_Actual_MAX_Value)
	{
		/* ��ǰ�����ݴ������ֵ���� */
		/* ����ǰ���¶����ó�����¶� */
		return CH_Actual_MAX_Value;
	}else
	{
		/* ��ǰ�������ڲ����ķ�Χ�� */
		temp_b = Temp_Value*10+2731;
		
		/* �Ե�ǰͨ����ʵֵ��ԭʼֵ���ݱ�������� */
		cal_num = Temp_Cal_Data_Tab_Qsort(CH_Actual_Value,CH_Calibration_Value);
		
		/* �жϵ�ǰͨ���Ƿ�У׼�� */
		if(cal_num == 0)
		{
			/* ��ǰͨ��û��У׼�������У׼ֵ������ʵֵ */
			return Temp_Value;
		}else if(cal_num == 1)
		{
			/* ��ǰͨ��У׼��һ���� */
			/* У׼һ����ʱ�����������߽��е��� */
			return ((((float)temp_b-((float)CH_Actual_Value_Buffer[0]-(float)CH_Calibration_Value_Buffer[0]))-2731)/10);
		}else
		{
			/* ��ǰͨ��У׼��������1����С��8���� */
			for(num = 0 ; num < (cal_num+1) ; num ++)
			{
				/* ѭ����� ��ǰ���¶����Ķ�У׼���� */
				if(temp_b >= CH_Actual_Value_Buffer[0])
				{
					/* �ж� ���� ����У׼�� */
					return ((((float)temp_b-((float)CH_Actual_Value_Buffer[0]-(float)CH_Calibration_Value_Buffer[0]))-2731)/10);
				}else if(temp_b < CH_Actual_Value_Buffer[cal_num-1])
				{
					/* �ж� ���� ����У׼�� */
					return ((((float)temp_b-((float)CH_Actual_Value_Buffer[cal_num-1]-(float)CH_Calibration_Value_Buffer[cal_num-1]))-2731)/10);
				}else
				{
					/* ѭ����� ��ǰ���¶����Ķ�У׼���� */
					if((temp_b < CH_Actual_Value_Buffer[num])&&(temp_b >= CH_Actual_Value_Buffer[num+1]))
					{
						/* ���� У׼������ */
						return((((((((float)CH_Calibration_Value_Buffer[num])-((float)CH_Calibration_Value_Buffer[num+1]))/(((float)CH_Actual_Value_Buffer[num])-((float)CH_Actual_Value_Buffer[num+1])))*
							   (((float)temp_b)-((float)CH_Actual_Value_Buffer[num+1])))+CH_Calibration_Value_Buffer[num+1])-2731)/10);
					}
				}
			}
		}
		
		/* �����ǲ��������е��ĵط� */
		return Temp_Value;
	}
	
#undef  CH_Actual_MAX_Value
}

#endif


/*************************************************************************************************/
/* ������	:Temp_Get_Sampl_Value															 	 */
/* ����		:�¶ȴ�����AD�ɼ�����																	 */
/* ���		:�������¶�����ֵ���¶�ֵ*10��															 */
/* ����		:�������ADC����ת�����¶�����														 	 */
/* ��������	:2015/03/25																			 */
/* Voltage divide resistor Pull-Down 														 	 */
/*************************************************************************************************/
float Temp_Get_Sampl_Value(u16 Adc_DR)
{	
/* ���ݱ�� ��ʼ�¶ȵ� */
#define TemprStart 		(-80.0)
/* ���ݱ�� �����¶ȵ� */
#define TemprEnd 		(150.0)
/* ���ݱ�� �¶Ȳ�ѯ���� */
#define Scale      		(1u)
/* ���ݱ��¶Ȳ�ѯ��Χ */
#define TableSize  		(231u)

	
	/* ������ */
	u16 num;
	/* ���ݱ� ���� ��ʼ λ�� */
	int16_t SearchStart = 0 ;
	/* ���ݱ� ���� ���� λ�� */
	int16_t SearchEnd 	= (TableSize - 1);
	/* ���ݱ� ���� ��Χ */
	int16_t SearchMax 	= TableSize;
	
	
	/* �жϵ�ǰ��ADC�����Ƿ񳬳������ݱ�Ľ��� */
	if(Adc_DR <= NTC_10K_3950[SearchStart])
	{
		/* ADCֵ С�� ���ݱ�� ��ʼ λ�õ�ֵ */
		/* �����ʼλ�õ��¶����� */
		return (SearchStart * Scale + TemprStart);
	}else if(Adc_DR >= NTC_10K_3950[SearchEnd])
	{
		/* ADCֵ ���� ���ݱ�� ���� λ�õ�ֵ */
		/* �������λ�õ��¶����� */
		return (SearchEnd * Scale + TemprStart);
	}
		
	/* ѭ�� ���� ��ǰ�� ADC ���Ǹ��¶������ڣ�����1�棩 */
	for(num = 0 ; num < SearchMax ; num ++)
	{
		/* �ж� �����ķ�Χ  */
		if((SearchEnd - SearchStart) > 1)
		{
			/* ���� ��Χ���� 1�� */
			/* ���� ��ǰ ��ѯ��Χ���м�����ݱ�ֵ */
			if(NTC_10K_3950[ (SearchEnd + SearchStart) / 2 ] > Adc_DR)
			{
				/* ���ݱ�ֵ����ADCֵ */
				/* ���� ��ѯ��Χ�Ľ�����λ�� */
				SearchEnd = ( SearchEnd + SearchStart ) / 2;
			}
			else if(NTC_10K_3950[ ( SearchEnd + SearchStart ) / 2 ] < Adc_DR)
			{
				/* ���ݱ�ֵС��ADCֵ */
				/* ���� ��ѯ��Χ�Ŀ�ʼ��λ�� */
				SearchStart = ( SearchEnd + SearchStart ) / 2;
			}
			else
			{
				/* ���ݱ�ֵ����ADCֵ */
				/* ���� �¶����� */
				return (((float)((SearchEnd + SearchStart) / 2) * Scale) + TemprStart);
			}
		}
		else
		{
			/* ���� ��ΧС�� 1�� */
			/* �����¶� */
			return (((((float)Adc_DR-(float)NTC_10K_3950[SearchStart])/
					  ((float)NTC_10K_3950[SearchEnd]-(float)NTC_10K_3950[SearchStart]))+
					   ((float)SearchStart))+TemprStart);
		}
	}
	/* ���ﲻ�������е��� */
	return 0;
	
#undef TemprStart
#undef TemprEnd
#undef Scale
#undef TableSize
}

//������̼�ɼ�ֵ
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

//�Ŵſ���
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

