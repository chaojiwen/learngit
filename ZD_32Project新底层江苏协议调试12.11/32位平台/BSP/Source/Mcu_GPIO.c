/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/10/28																			 */
/*************************************************************************************************/
#include "Mcu_GPIO.h"
#include "common.h"



///* *********************************************************************************************************
// * ��������ADC1_GPIO_Config
// * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PC.01
// * ����  : ��
// * ���  ����
// * ����  ���ڲ�����
// * *********************************************************************************************************/
//static void ADC1_GPIO_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	/* ʹ��DMAʱ�� */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
//
//	/* ʹ��GPIOA\GPIOB\GPIOC��ADC1��ʱ�� */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |\
//						   RCC_APB2Periph_GPIOB |\
//						   RCC_APB2Periph_GPIOC |\
//						   RCC_APB2Periph_ADC1,ENABLE);
//
//	/* ����IO�Ĺ�����ʽ */		 //    CH0 			CH1			CH2
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;			  													//����PC1ΪADC�ĵ�ѹ��������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;															//IOΪģ������
//	GPIO_Init(GPIOA,&GPIO_InitStructure);//������д��Ĵ���
//
//	/* ����IO�Ĺ�����ʽ */		 //		CH3			CH4
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;			  													//����PC1ΪADC�ĵ�ѹ��������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;															//IOΪģ������
//	GPIO_Init(GPIOB,&GPIO_InitStructure);//������д��Ĵ���
//
//	/* ����IO�Ĺ�����ʽ */	   //	CH5				CH6			CH7			CH8
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;			  													//����PC1ΪADC�ĵ�ѹ��������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;															//IOΪģ������
//	GPIO_Init(GPIOC,&GPIO_InitStructure);//������д��Ĵ���
//
//}


/******************* (C) ZHOU *****END OF FILE****/
