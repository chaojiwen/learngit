/******************** (C) zhou ***************************
 * �ļ���  ��
 * ����    ��       
 * ʵ��ƽ̨��32 λƽ̨�����ԣ�
 *
 * ��汾  ��ST3.5.0
 *
 * ����    ��zhou 
**********************************************************************************/
#include "Mcu_ADC.h"

#include "Mcu_DMA.h"

#include "Log_Adc1.h"
#include "Log_Sensor.h"




static void ADC1_Mode_config(void);
static void ADC1_GPIO_Config(void);

/************************************************************************************************/
/* ������	: ADC1_Mode_Config															 	 	*/
/* ����		: �� 																		 		*/
/* ���		: �� 																				*/
/* ����		: ����ADC1�Ĺ���ģʽΪMDAģʽ									 						*/
/* ��������	: 2015/04/10																		*/
/************************************************************************************************/
static void ADC1_Mode_config(void)
{
/* ADC����ͨ������ ������ʱ���� */
#define ADC1_CH_NUM		(9u)


	
	ADC_InitTypeDef ADC_InitStructure;

	DMA_Rece_ADC1_Data_Config();
	/* ����ADC1 */
	ADC_InitStructure.ADC_Mode 					= ADC_Mode_Independent;				//ʹ�õ�ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode 			= ENABLE;							//ʹ��ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode 	= DISABLE;							//ʹ������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;		//�������ⲿ�����¼�
	ADC_InitStructure.ADC_DataAlign 			= ADC_DataAlign_Right;				//����ADC1�����ݶ��뷽ʽΪ �Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel 			= ADC1_CH_NUM;						/* ���ʲô��˼����� */
	ADC_Init(ADC1,&ADC_InitStructure);												//������д��Ĵ���
	/* ����ADC1��ͨ������ *///����ADC1��ͨ��11ת��ʱ��Ϊ239.5����
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14,5,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15,6,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,7,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,8,ADC_SampleTime_239Cycles5);
	//BT->
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,9,ADC_SampleTime_239Cycles5);

	/* ���øߡ���ģ�⿴�Ź���ֵ */
	ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0FFF, 0X0000);
	/* ͨ��14����Ϊ��ģ�⿴�Ź�����ͨ�� */
	ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_8);
	/* ��һ������Ƶ��������ģ�⿴�Ź� */
	ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
	/* ʹ��ģ�⿴�Ź��ж� */
	ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
	
	/* ʹ��ADC��DMA���� */		  
	ADC_DMACmd(ADC1,ENABLE);																				
	/* ʹ��ADC1 */
	ADC_Cmd(ADC1,ENABLE);
	/* ��λADC1������У׼�Ĵ��� */
	ADC_ResetCalibration(ADC1);
	/* ��ⱻѡ�е�ADC1����У׼�Ĵ�����λ״̬��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ��ʼADC1��У׼ */
	ADC_StartCalibration(ADC1);
	/* ��ⱻѡ�е�ADC1����У׼�Ĵ�������״̬��� */
	while(ADC_GetCalibrationStatus(ADC1));
	/* ��ʼADC1ת�� */
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	
	 		
#undef ADC1_CH_NUM
}

/************************************************************************************************/
/* ������	: ADC1_GPIO_Config															 	 	*/
/* ����		: �� 																		 		*/
/* ���		: �� 																				*/
/* ����		: ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PC.01							 						*/
/* ��������	: 2015/04/10																		*/
/************************************************************************************************/
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��DMAʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	/* ʹ��GPIOA��ADC1��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
						   RCC_APB2Periph_GPIOB |
						   RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOE |
						   RCC_APB2Periph_ADC1,ENABLE);

	/* ����IO�Ĺ�����ʽ *///CH0\CH1\CH2\CH3\CH4\CH5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
								  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//IOΪģ������
	GPIO_Init(GPIOC,&GPIO_InitStructure);//������д��Ĵ���
	
	/* ����IO�Ĺ�����ʽ *///CH6\CH7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;//IOΪģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure);//������д��Ĵ���
	
	/* ����IO�Ĺ�����ʽ *///CH8->BT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//IOΪģ������
	GPIO_Init(GPIOB,&GPIO_InitStructure);//������д��Ĵ���
	
	/* ���� ������ �ĵ�Դ���� ���� */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	Sensor_Power_Close;
	GPIO_SetBits(GPIOE, GPIO_Pin_15);
	
#if (Sensor_PT1000_Num > 0x00)
	/* ���� ������ �ĵ�Դ���� ���� */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	Sensor_Power_Close;
	GPIO_SetBits(GPIOE, GPIO_Pin_12);
#endif
	
}

/************************************************************************************************/
/* ������	: ADC1_Init																 	 	*/
/* ����		: �� 																		 		*/
/* ���		: �� 																				*/
/* ����		: ����ADC1							 												*/
/* ��������	: 2015/04/10																		*/
/* �޸�����	: 2015/11/11																		*/
/************************************************************************************************/
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_config();
	
	/* ������Դ��ѹ���:�� */
	Itu_Pwr_Spe_Open;
	/* �������źż��  :�� */
	Sen_Pwr_Spe_Open;
	
#if (Sensor_PT1000_Num > 0x00)
	/* PT1000�������źż��  :�� */
	PT1000_Pwr_Spe_Open;
#endif
}


/******************* (C) ZHOU *****END OF FILE****/
