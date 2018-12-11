/************************************************************************************************/
/** @file              : Mcu_DMA2.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��15��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--STM32F103-DMA2�߼����ļ�						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��11��15��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/

#include "Mcu_DMA2.h"




/************************************************************************************************/
/* ������	: DMA2_Init																 	 		*/
/* ����  	: PBA:Ŀ���ַ��MBA���ڴ��ַ��			 											*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��DMA2����									 										*/
/* ��������	: 2016/11/15																		*/
/************************************************************************************************/
void DMA2_Init(void)
{

	/* ����DMA2��ͨ��5��ģʽ */
	DMA_DeInit(DMA2_Channel5);		
	/* ʹ��DMAʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);

}

/************************************************************************************************/
/* ������	: DMA2_SendData														 	 			*/
/* ����  	: PBA:Ŀ���ַ��MBA���ڴ��ַ��			 											*/
/* ���		: �� 																				*/
/* ����	 	: ͨ��DMA2����ָ�����������ݵ�ָ��������ȥ							 					*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/*   ע��	: DMA2ʹ�õ��δ��䷽ʽ																*/
/************************************************************************************************/
void DMA2_SendData(uint32_t pba,uint32_t mba,uint16_t len)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	
	/* ��ʼ��DMA���ݽṹ */
	DMA_StructInit(&DMA_InitStructure);
	/* ����DMA2��ͨ��5��ģʽ */
	DMA_DeInit(DMA2_Channel5);	
	
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
	DMA_Init(DMA2_Channel5,&DMA_InitStructure);		
	
	/* ���ݴ��� */
	DMA_Cmd(DMA2_Channel5,ENABLE);	
}




/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

