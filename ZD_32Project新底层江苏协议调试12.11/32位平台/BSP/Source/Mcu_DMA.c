/**
  ******************************************************************************
  * @file              : Mcu_DMA.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015��4��12�� 09:55
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
***/
#include "System_Config.h"

#include "Mcu_DMA.h"

#include "Log_Adc1.h"
#include "Log_RF_Card.h"
#include "Log_Sensor.h"


/************************************************************************************************/
/* ������	: DMA_Rece_ADC1_Data_Config												 	 		*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ����ADC1���ݽӷ�ʽ							 										*/
/* ��������	: 2015/04/20																		*/
/************************************************************************************************/
void DMA_Rece_ADC1_Data_Config(void)
{
/* DMA�������ݵ�ַ */ 
#define DMA_Rece_ADC1_Data_BaseAddr	((u32)0x4001244C)
	
	DMA_InitTypeDef DMA_InitStructure;

	/* ʹ��DMAʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	/* ����ADC1��ͨ��1��DMAģʽ */
	DMA_DeInit(DMA1_Channel1);														//��λDMAͨ��1
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= DMA_Rece_ADC1_Data_BaseAddr;		//��������Ĵ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (u32)&ADC_DMA_Rece_DR[0];			//�����ڴ�ĵ�ַ
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC;			//�������ַ��Դ��MDA_PeripheralBaseAddr��	
	DMA_InitStructure.DMA_BufferSize 			= ADC_Collection_Data_Buffer_Size;	//���õ��δ������ݵ�����
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;		//��ʹ��DMA�������ַ����ģʽ
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;				//ʹ��DMA���ڴ��ַ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord; 	//�����������ݿ�ȣ����ֽڣ�16λ��
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_HalfWord;		//�����ڴ����ݿ�ȣ����ֽڣ�16λ��
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;				//ʹ��DMA��ѭ��ģʽ
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_Medium;				//����ͨ�������ȼ�
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;					//���𶯼Ĵ������Ĵ���ģʽ
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);										//������д��Ĵ���
	DMA_Cmd(DMA1_Channel1,ENABLE);													//ʹ��DMA1
	
#undef DMA_Rece_ADC1_Data_BaseAddr
}

#if (Wireless_RF_Card_Config_Enable == 0x01)
/************************************************************************************************/
/* ������	: DMA_Rece_USART2_Data_Config												 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ����USART2���ݽӷ�ʽ							 									*/
/* ��������	: 2015/04/12																		*/
/************************************************************************************************/
void DMA_Rece_USART2_Data_Config(void)
{
/* DMA�������ݵ�ַ */
#define DMA_Rece_USART2_Data_BaseAddr	((u32)0x40004404)
/* DMA���δ������ݵ����� */
#define DMA_Rece_USART2_Data_Num		(32u)	
	
	DMA_InitTypeDef DMA_InitStructure;

	/* ʹ��DMAʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	/* ��λ�Ĵ��� */
	DMA_Cmd(DMA1_Channel6,DISABLE);		
	/* ����ͨ��6��DMAģʽ */
	DMA_DeInit(DMA1_Channel6);														//��λDMAͨ��6
	
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= DMA_Rece_USART2_Data_BaseAddr;	//��������Ĵ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (u32)Wireless_RF_Card_Data_Buffer_Pointer;//�����ڴ�ĵ�ַ
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC;			//�������ַ��Դ��MDA_PeripheralBaseAddr��	
	DMA_InitStructure.DMA_BufferSize 			= DMA_Rece_USART2_Data_Num;			//���õ��δ������ݵ�����
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;		//��ʹ��DMA�������ַ����ģʽ
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;				//ʹ��DMA���ڴ��ַ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte; 		//�����������ݿ�ȣ����ֽڣ�8λ��
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;			//�����ڴ����ݿ�ȣ����ֽڣ�8λ��
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;				//ʹ��DMA��ѭ��ģʽ
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High;				//����ͨ�������ȼ�
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;					//���𶯼Ĵ������Ĵ���ģʽ
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);										//������д��Ĵ���
	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);									//����DMA1�����ݵ��жϷ�ʽ
	DMA_ITConfig(DMA1_Channel6,DMA_IT_HT,ENABLE);									//����DMA1�����ݵ��жϷ�ʽ
	
	DMA_Cmd(DMA1_Channel6,ENABLE);													//ʹ��DMA1��ͨ��6����


#undef DMA_Rece_USART2_Data_BaseAddr
#undef DMA_Rece_USART2_Data_Num	
}
#endif



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

