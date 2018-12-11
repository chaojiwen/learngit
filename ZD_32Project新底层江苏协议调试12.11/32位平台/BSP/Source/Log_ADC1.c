/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/10/24																			 */
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
/* ADC���������ݼĴ��� */
static u16 	ADC_Collection_Data_Buffer[ADC_Collection_Data_Buffer_Size];
/* ADC �洢�ɼ����ݼĴ�����С */
#define		ADC_Store_Data_Buffer_Size			(8u)
/* ADC �洢�ɼ����ݼĴ��� */
static u16 	ADC_Store_Data_Buffer[ADC_Collection_Data_Buffer_Size][ADC_Store_Data_Buffer_Size];
/* ADC ͨ��DMA��ʽ��������ݼĴ��� */
	   u16 	ADC_DMA_Rece_DR[ADC_Collection_Data_Buffer_Size];
/************************************************************************************************/

static int 	Comp(const void *p1,const void *p2)
{
	return *((u16*)p2) >  *((u16*)p1)?1:-1; 
}

/* �����ݻ���ȡƽ�� */
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
/* ������  ��ADC_Data_Buffer_Init														 	 	 */
/* ����	  ��(void *p_arg) 																		 */
/* ���	  : �� 																					 */
/* ����	  ������ADCת������ȡADC���ݣ��������ݣ��������												 */
/* �������ڣ�2015/1/05																			 */
/*************************************************************************************************/
void ADC_Data_Buffer_Init(void)
{
	u8 num;
	
	/* ����ADC���ݻ��� */
	for(num=0;num<ADC_Collection_Data_Buffer_Size;num++)
	{
		ADC_Get_Value();
	}
	
}
	
//ADC_To_Sensor_Value
/*************************************************************************************************/
/* ������  ��ADC_Get_Value																 	 	 */
/* ����	  ��(void *p_arg) 																		 */
/* ���	  : �� 																					 */
/* ����	  ������ADCת������ȡADC���ݣ��������ݣ��������												 */
/* �������ڣ�2015/1/05																			 */
/*************************************************************************************************/
void ADC_Get_Value(void)
{	
	static u8 adc_buffer_p = 0;
	u8 num;
	u16 array[ADC_Store_Data_Buffer_Size];
	
	//�׶�1������������Դ������ADCת������ô�����ͨ����ADC���ݡ�
	/* ��ʼADC1ת�� */
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	/* �ȴ� DMA ���� ת�� ��� *///���������1MS����
	do{
		OSTimeDly(1);
	/* �ж� DMA �����Ƿ�ת����ɡ� */
	}while(DMA_GetFlagStatus(DMA1_FLAG_TC1) != SET);
	/* DMA �����Ƿ�ת�����! */
	
	//�׶�2 ��������ͨ����ADC���ݡ������ݽ������򣬹��˵��쳣���ݣ�ȡ�������ݵ�ƽ��ֵ��
	/* �����»�õ�ADC����д�� ���� */
	for(num = 0;num < ADC_Collection_Data_Buffer_Size;num ++)
	{
		/* ת������ */
		ADC_Store_Data_Buffer[num][adc_buffer_p] = ADC_DMA_Rece_DR[num];
	}

	/* ����ָ�� */
	adc_buffer_p++;
	/* �ж�ָ��Խ�� */
	if(adc_buffer_p >= ADC_Store_Data_Buffer_Size)
	{
		/* ָ��Խ��,����0�� */
		adc_buffer_p = 0;
	}
	
	/* ��ÿ������ͨ�����ݽ����������򣩡� */
	for(num = 0;num < ADC_Collection_Data_Buffer_Size;num ++)
	{
		/* ��� ���� */
		memset(array,0x00,sizeof(array));
		
		/* �����ݻ������ݸ�ֵ����ʱ���������� */
		memcpy(array,ADC_Store_Data_Buffer[num],sizeof(ADC_Store_Data_Buffer[num]));
		
		/* ���� */
		qsort(array,ADC_Store_Data_Buffer_Size,sizeof(u16),Comp);
		
		/* ȥ�����2�����ݡ�ȥ�����2�����ݣ����ȡƽ���� */
		ADC_Collection_Data_Buffer[num] = ADC_Data_Avg(&array[1],(ADC_Store_Data_Buffer_Size-2));
	}
}

/*************************************************************************************************/
/* ������	��ADC_Trans_Data															 	 	 */
/* ����		��data_buffer:���ݻ���																 */
/* ���		: �� 																				 */
/* ����		�����ɼ�����ADC����ת��ָ���ļĴ�����													 */
/* ��������	: 2015/04/20																		 */
/*************************************************************************************************/
void ADC_Trans_Data(float *data_buffer)
{
/* �������ת�Ƶ�λ�� */
#define Battery_Data_Place_Offset	(32u)
/* ������ݴ洢��λ�� */
#define Battery_Data_Store_Offset    (8u)

	
/* �ж� ͨ��1 */
#if		Sensor_CH1_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_1) 	= ADC_Collection_Data_Buffer[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_1) 	= ADC_Collection_Data_Buffer[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_1) 	= ADC_Collection_Data_Buffer[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_PT1000
		*(data_buffer + Sensors_CH_1) 	= ADC_Collection_Data_Buffer[Sensors_CH_1];
#endif	
	
/* �ж� ͨ��2 */
#if		Sensor_CH2_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_2) 		= ADC_Collection_Data_Buffer[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_2) 	= ADC_Collection_Data_Buffer[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_2) 		= ADC_Collection_Data_Buffer[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_PT1000
		*(data_buffer + Sensors_CH_2) 		= ADC_Collection_Data_Buffer[Sensors_CH_2];
#endif	
	
/* �ж� ͨ��3 */
#if		Sensor_CH3_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_3) 		= ADC_Collection_Data_Buffer[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_3) 	= ADC_Collection_Data_Buffer[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_3) 		= ADC_Collection_Data_Buffer[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_PT1000
#error	"������ͨ��3��������ΪPT1000����������Ϊ��ǰӲ����֧�֡�"
#endif	
	
/* �ж� ͨ��4 */
#if		Sensor_CH4_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_4) 		= ADC_Collection_Data_Buffer[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_4) 	= ADC_Collection_Data_Buffer[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_4) 		= ADC_Collection_Data_Buffer[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_PT1000
		#error	"������ͨ��4��������ΪPT1000����������Ϊ��ǰӲ����֧�֡�"
#endif	
	
/* �ж� ͨ��5 */
#if		Sensor_CH5_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_5) 		= ADC_Collection_Data_Buffer[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_CarbonDioxide
		*(data_buffer + Sensors_CH_5) 	= ADC_Collection_Data_Buffer[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_5) 		= ADC_Collection_Data_Buffer[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_PT1000
		#error	"������ͨ��5��������ΪPT1000����������Ϊ��ǰӲ����֧�֡�"
#endif	
	
/* �ж� ͨ��6 */
#if		Sensor_CH6_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_6) 		= ADC_Collection_Data_Buffer[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_6) 		= ADC_Collection_Data_Buffer[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_PT1000
		#error	"������ͨ��6��������ΪPT1000����������Ϊ��ǰӲ����֧�֡�"
#endif	
	
/* �ж� ͨ��7 */
#if		Sensor_CH7_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_7) 		= ADC_Collection_Data_Buffer[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_7) 		= ADC_Collection_Data_Buffer[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_DoorContact
		*(data_buffer + Sensors_CH_7) 		= ADC_Collection_Data_Buffer[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_PT1000
		#error	"������ͨ��7��������ΪPT1000����������Ϊ��ǰӲ����֧�֡�"
#endif	
	
/* �ж� ͨ��8 */
#if		Sensor_CH8_Config_Type == Sensor_Tmp
		*(data_buffer + Sensors_CH_8) 		= ADC_Collection_Data_Buffer[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_Hum
		*(data_buffer + Sensors_CH_8) 		= ADC_Collection_Data_Buffer[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_PT1000
		#error	"������ͨ��8��������ΪPT1000����������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��9 */
#if		((Sensor_CH9_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH9_Config_Type == Sensor_Hum)||\
		 (Sensor_CH9_Config_Type == Sensor_PT1000))
#error	"������ͨ��9��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��10 */
#if		((Sensor_CH10_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH10_Config_Type == Sensor_Hum)||\
		 (Sensor_CH10_Config_Type == Sensor_PT1000))
#error	"������ͨ��10��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��11 */
#if		((Sensor_CH11_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH11_Config_Type == Sensor_Hum)||\
		 (Sensor_CH11_Config_Type == Sensor_PT1000))
#error	"������ͨ��11��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��12 */
#if		((Sensor_CH12_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH12_Config_Type == Sensor_Hum)||\
		 (Sensor_CH12_Config_Type == Sensor_PT1000))
#error	"������ͨ��12��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��13 */
#if		((Sensor_CH13_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH13_Config_Type == Sensor_Hum)||\
		 (Sensor_CH13_Config_Type == Sensor_PT1000))
#error	"������ͨ��13��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��14 */
#if		((Sensor_CH14_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH14_Config_Type == Sensor_Hum)||\
		 (Sensor_CH14_Config_Type == Sensor_PT1000))
#error	"������ͨ��14��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��15 */
#if		((Sensor_CH15_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH15_Config_Type == Sensor_Hum)||\
		 (Sensor_CH15_Config_Type == Sensor_PT1000))
#error	"������ͨ��15��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��16 */
#if		((Sensor_CH16_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH16_Config_Type == Sensor_Hum)||\
		 (Sensor_CH16_Config_Type == Sensor_PT1000))
#error	"������ͨ��16��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif
	
/* �ж� ͨ��17 */
#if		((Sensor_CH17_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH17_Config_Type == Sensor_Hum)||\
		 (Sensor_CH17_Config_Type == Sensor_PT1000))
#error	"������ͨ��17��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��18 */
#if		((Sensor_CH18_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH18_Config_Type == Sensor_Hum)||\
		 (Sensor_CH18_Config_Type == Sensor_PT1000))
#error	"������ͨ��18��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��19 */
#if		((Sensor_CH19_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH19_Config_Type == Sensor_Hum)||\
		 (Sensor_CH19_Config_Type == Sensor_PT1000))
#error	"������ͨ��19��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��20 */
#if		((Sensor_CH20_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH20_Config_Type == Sensor_Hum)||\
		 (Sensor_CH20_Config_Type == Sensor_PT1000))
#error	"������ͨ��20��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��21 */
#if		((Sensor_CH21_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH21_Config_Type == Sensor_Hum)||\
		 (Sensor_CH21_Config_Type == Sensor_PT1000))
#error	"������ͨ��21��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��22 */
#if		((Sensor_CH22_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH22_Config_Type == Sensor_Hum)||\
		 (Sensor_CH22_Config_Type == Sensor_PT1000))
#error	"������ͨ��22��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��23 */
#if		((Sensor_CH23_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH23_Config_Type == Sensor_Hum)||\
		 (Sensor_CH23_Config_Type == Sensor_PT1000))
#error	"������ͨ��23��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��24 */
#if		((Sensor_CH24_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH24_Config_Type == Sensor_Hum)||\
		 (Sensor_CH24_Config_Type == Sensor_PT1000))
#error	"������ͨ��24��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif

/* �ж� ͨ��25 */
#if		((Sensor_CH25_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH25_Config_Type == Sensor_Hum)||\
		 (Sensor_CH25_Config_Type == Sensor_PT1000))
#error	"������ͨ��25��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��26 */
#if		((Sensor_CH26_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH26_Config_Type == Sensor_Hum)||\
		 (Sensor_CH26_Config_Type == Sensor_PT1000))
#error	"������ͨ��26��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��27 */
#if		((Sensor_CH27_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH27_Config_Type == Sensor_Hum)||\
		 (Sensor_CH27_Config_Type == Sensor_PT1000))
#error	"������ͨ��27��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��28 */
#if		((Sensor_CH28_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH28_Config_Type == Sensor_Hum)||\
		 (Sensor_CH28_Config_Type == Sensor_PT1000))
#error	"������ͨ��28��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��29 */
#if		((Sensor_CH29_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH29_Config_Type == Sensor_Hum)||\
		 (Sensor_CH29_Config_Type == Sensor_PT1000))
#error	"������ͨ��29��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��30 */
#if		((Sensor_CH30_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH30_Config_Type == Sensor_Hum)||\
		 (Sensor_CH30_Config_Type == Sensor_PT1000))
#error	"������ͨ��30��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��31 */
#if		((Sensor_CH31_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH31_Config_Type == Sensor_Hum)||\
		 (Sensor_CH31_Config_Type == Sensor_PT1000))
#error	"������ͨ��31��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif	

/* �ж� ͨ��32 */
#if		((Sensor_CH32_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH32_Config_Type == Sensor_Hum)||\
		 (Sensor_CH32_Config_Type == Sensor_PT1000))
#error	"������ͨ��32��������Ϊ���ش���������Ϊ��ǰӲ����֧�֡�"
#endif
		

		/* ���ͨ������ */
		*(data_buffer + Battery_Data_Place_Offset) 	= ADC_Collection_Data_Buffer[Battery_Data_Store_Offset];

#undef Battery
}

/*************************************************************************************************/
/* ������	��ADC_AWD_Run_Init															 	 	 */
/* ����		����																	 			 	 */
/* ���		: �� 																				 */
/* ����		����������ʱ��ģ�⿴�Ź��ĳ�ʼ��													 	 */
/* ��������	: 2015/11/20																		 */
/*************************************************************************************************/
void ADC_AWD_Run_Init(void)
{
	/* �µ���ӵ�Դ����״̬��־ */
	static FlagStatus new_et_power_state_flag = RESET;
	/* �ɵ���ӵ�Դ����״̬��־ */
	static FlagStatus old_et_power_state_flag = RESET;
	
	/* ��ȡ��Դ����״̬ */
	new_et_power_state_flag = (Ext_Power_ON_OFF_Check()==Bit_RESET)?RESET:SET;
	
	/* �ж���ӵ�Դ����״̬ */
	if(new_et_power_state_flag == Bit_RESET)
	{
		/* ��������� */
		/* �ж��ϵĵ�Դ״̬ */
		if(old_et_power_state_flag != RESET)
		{
			/* �ر�ģ�⿴�Ź� */
			ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0FFF, 0x0000);
		}
			/* �����ϵ�Դ״̬ */
			old_et_power_state_flag = RESET;		
	}else
	{
		/* ���δ���� */
		/* �ж��µ�Դ״̬���ϵĵ�Դ״̬ */
		if(old_et_power_state_flag != SET)
		{
			/* ����ģ�⿴�Ź� */
			/* ��ADC��һ�����ݲɼ���ɺ��������û�и�λ˵����ǰ�ĵ�ص�ѹ����3.5V������������������������֮��
			   ģ�⿴�Ź������޵�ѹ���ó�3.35V��*/
			ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0FFF, ADC1_Week_Voltage_Low);		
		}
			/* �����ϵ�Դ״̬ */
			old_et_power_state_flag = SET;
	}
}

///*************************************************************************************************/
///* ������	��ADC_AWD_Frt_Init															 	 	 */
///* ����		����																	 			 	 */
///* ���		: �� 																				 */
///* ����		�������ϵ�ʱ��ģ�⿴�Ź��ĳ�ʼ��													 	 */
///* ��������	: 2015/11/20																		 */
///*************************************************************************************************/
//void ADC_AWD_Frt_Init(void)
//{

//	/* �ж���ӵ�Դ����״̬ */
//	if(Ext_Power_ON_OFF_Check() == Bit_RESET)
//	{
//		/* ��������� */
//		/* ������ģ�⿴�Ź� */
//		__nop();
//	}else
//	{
//		/* ���δ���� */
//		/* ����ģ�⿴�Ź� */
//		/* ��ADC��һ�����ݲɼ���ɺ��������û�и�λ˵����ǰ�ĵ�ص�ѹ����3.5V������������������������֮��
//		   ģ�⿴�Ź������޵�ѹ���ó�3.35V��*/
//		ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0FFF, ADC1_Start_Voltage);
//	}

//}

/*************************************************************************************************/
/* ������	��ADC_Sam_Pwr_Sta															 	 	 */
/* ����		����																	 			 	 */
/* ���		: �� 																				 */
/* ����		���ϵ����ص�ѹ													 	 */
/* ��������	: 2015/12/11																		 */
/*************************************************************************************************/
void ADC_Sam_Pwr_Sta(void)
{

	do{
		/* ��⵱ǰϵͳ��Դ */
		ADC_Data_Buffer_Init();	
	}while((ADC_Collection_Data_Buffer[8] < ADC1_Start_Voltage)&&
		   (Ext_Power_ON_OFF_Check() == Bit_SET));
	
}




/*************************************************************************************************/

