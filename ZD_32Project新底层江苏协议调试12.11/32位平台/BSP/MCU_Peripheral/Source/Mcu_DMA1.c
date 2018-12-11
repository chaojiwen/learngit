/************************************************************************************************/
/** @file              : Mcu_DMA1.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��12��14��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--STM32F103-DMA1�߼����ļ�						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��12��14��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/

#include "Mcu_DMA1.h"




/************************************************************************************************/
/* ������	: DMA1_Init																 	 		*/
/* ����  	: PBA:Ŀ���ַ��MBA���ڴ��ַ��			 											*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��DMA1����									 										*/
/* ��������	: 2016/12/14																		*/
/************************************************************************************************/
void DMA1_Init(void)
{
	/* ʹ��DMAʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

}

/************************************************************************************************/
/* ������	: DMA1_SendData														 	 			*/
/* ����  	: PBA:Ŀ���ַ��MBA���ڴ��ַ��			 											*/
/* ���		: �� 																				*/
/* ����	 	: ͨ��DMA1����ָ�����������ݵ�ָ��������ȥ							 					*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/*   ע��	: DMA1ʹ�õ��δ��䷽ʽ																*/
/************************************************************************************************/
void DMA1_SendData(uint32_t pba,uint32_t mba,uint16_t len)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	
	/* ��ʼ��DMA���ݽṹ */
	DMA_StructInit(&DMA_InitStructure);
	/* ����DMA1��ͨ��5��ģʽ */
	DMA_DeInit(DMA1_Channel7);
	
	/* �������в��� */
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= pba;								//��������Ĵ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr 		= mba;								//�����ڴ�ĵ�ַ
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralDST;			//�������ַ��Դ��MDA_PeripheralBaseAddr��	
	DMA_InitStructure.DMA_BufferSize 			= len;								//���õ��δ������ݵ�����
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;		//��ʹ��DMA�������ַ����ģʽ
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;				//ʹ��DMA���ڴ��ַ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte; 		//�����������ݿ�ȣ��ֽڣ�8λ��
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;			//�����ڴ����ݿ�ȣ��ֽڣ�8λ��
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;					//ʹ��DMA�ĵ��δ���ģʽ
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_Medium;				//����ͨ�������ȼ�
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;					//���𶯼Ĵ������Ĵ���ģʽ
	
	/* ������д��Ĵ��� */
	DMA_Init(DMA1_Channel7,&DMA_InitStructure);		
	
	/* ���ݴ��� */
	DMA_Cmd(DMA1_Channel7,ENABLE);	
}




/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

