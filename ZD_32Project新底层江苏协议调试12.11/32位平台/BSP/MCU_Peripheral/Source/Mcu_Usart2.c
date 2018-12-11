/************************************************************************************************/
/** @file              : Mcu_USART2.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��12��14��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--STM32F103-USART2�߼����ļ�						*/
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
#include "Mcu_USART2.h"

#include "ucos_ii.h"


/************************************************************************************************/
/* �������ݺ���ָ�� */
void (*Mcu_USART2_ReceDataP)(uint8_t);

/************************************************************************************************/
/* USART2	�������ݻ����� */	   

/************************************************************************************************/



/************************************************************************************************/
/* ������	: Mcu_USART2_ModeInit										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��USART2ͨ��ģʽ����						 										*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
//void USART2_IRQHandler(void)
//{
//	OS_CPU_SR  cpu_sr;
//	
//	
//	OS_ENTER_CRITICAL();  	//����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR
//	OSIntNesting++;			//�ж�Ƕ�ױ�־
//	OS_EXIT_CRITICAL();		//�ָ�ȫ���жϱ�־ 

//	
//	/* USART2 �����ж�(ÿ�ν���һ���ֽ�) */
//	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
// 	{	
//		
//		/* ����GPS��Ϣ */
//		Mcu_USART2_ReceDataP(USART_ReceiveData(USART2));

//		/* ��������жϱ�־ */
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	
// 	}

//	/* �жϽ�������ж�״̬ */
//	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) != RESET)
//    {
//		/* ��������ж� */
//		/* �������ж� */
//		USART_ClearFlag(USART2,USART_FLAG_ORE);
//		USART_ReceiveData(USART2);
//    }

//	/* ��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л��� */
//	OSIntExit();
//}

/************************************************************************************************/
/* ������	: Mcu_USART2_GPIOInit										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��USART2��GPIO						 												*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void Mcu_USART2_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ͨ��GPIO */
	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(USART2_TX_CLK | USART2_RX_CLK , ENABLE );
	
	GPIO_SetBits(USART2_TX_PORT,USART2_TX_PIN);
	GPIO_SetBits(USART2_RX_PORT,USART2_RX_PIN);

	/* GPIOģʽ */
	GPIO_InitStructure.GPIO_Pin 	= 	USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* д��Ĵ��� */
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);

	
	/* GPIOģʽ */
	GPIO_InitStructure.GPIO_Pin 	= 	USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* д��Ĵ��� */
	GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);
	
}



/************************************************************************************************/
/* ������	: Mcu_USART2_ModeInit										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��USART2ͨ��ģʽ����						 										*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void Mcu_USART2_ModeInit(uint32_t baudrate)
{
	USART_InitTypeDef USART_InitStructure;

	
	/* ����ͨ�Ź���:USART2 */
	/* ����ʱ�� */
	USART_DeInit(USART2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* ��ʼ������ */
	USART_StructInit(&USART_InitStructure);
	
	/* UART1����ģʽ���� */
	USART_InitStructure.USART_BaudRate 		= baudrate;
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
	USART_InitStructure.USART_Parity 		= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 			= USART_Mode_Tx | USART_Mode_Rx;
	/* д��Ĵ��� */
	USART_Init(USART2,&USART_InitStructure);
	
	/* ���ý����ж� */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	/* ���ô���DMA���� */
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	
	/* ʹ��USART2 */
	USART_Cmd(USART2 , ENABLE);
	
}


/************************************************************************************************/
/* ������	: USART2_Init										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��USART2ͨ��ģʽ����						 										*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void USART2_Init(uint32_t baudrate,void (*funtp)(uint8_t))
{
	/* ��ʼ��USART2ͨ��ģʽ���� */
	Mcu_USART2_ModeInit(baudrate);	
	
	/* ��ʼ��USART2��GPIO */
	Mcu_USART2_GPIOInit();

	/* ��ʼ���������ݺ���ָ�� */
	Mcu_USART2_ReceDataP = funtp;
}

/************************************************************************************************/
/* ������	: Mcu_USART2_GPIODeInit										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �����ʼ��USART2��GPIO						 										*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void Mcu_USART2_GPIODeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ͨ��GPIO */
	
	/* GPIOģʽ */
	GPIO_InitStructure.GPIO_Pin 	= 	USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AIN;
	/* д��Ĵ��� */
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);
	
	
	/* GPIOģʽ */
	GPIO_InitStructure.GPIO_Pin 	= 	USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AIN;
//	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* д��Ĵ��� */
	GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);
}

/************************************************************************************************/
/* ������	: Mcu_USART2_Close										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��USART2ͨ��ģʽ����						 										*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void Mcu_USART2_Close(void)
{
	/* ʹ��USART2 */
	USART_Cmd(USART2 , DISABLE);
	/* ��GPIO������Ϊģ������ */
	Mcu_USART2_GPIODeInit();
}

/************************************************************************************************/
/* ������	: Mcu_USART2_Open										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��USART2ͨ��ģʽ����						 										*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void Mcu_USART2_Open(void)
{
	/* ʹ��USART2 */
	USART_Cmd(USART2 , ENABLE);
	/* ��GPIO������Ϊģ������ */
	Mcu_USART2_GPIOInit();
}

/************************************************************************************************/
/* ������	: USART2_SendData										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��USART2ͨ��ģʽ����						 										*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void USART2_SendData(uint8_t data)
{	
	
	USART_SendData(USART2,data);
}





/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

