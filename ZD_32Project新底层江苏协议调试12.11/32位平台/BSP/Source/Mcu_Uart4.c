/************************************************************************************************/
/** @file              : Mcu_Uart4.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��14��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--STM32F103-Uart4�߼����ļ�						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��11��14��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Mcu_Uart4.h"

#include "ucos_ii.h"


/************************************************************************************************/
/* �������ݺ���ָ�� */
void (*Mcu_Uart4_ReceDataP)(uint8_t);

/************************************************************************************************/
/* Uart4	�������ݻ����� */	   

/************************************************************************************************/



/************************************************************************************************/
/* ������	: Mcu_Uart4_ModeInit										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��Uart4ͨ��ģʽ����						 										*/
/* ��������	: 2016/11/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void UART4_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	
	
	OS_ENTER_CRITICAL();  	//����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//�ж�Ƕ�ױ�־
	OS_EXIT_CRITICAL();		//�ָ�ȫ���жϱ�־ 

	
	/* Uart4 �����ж�(ÿ�ν���һ���ֽ�) */
	if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)
 	{	
		
		/* ����GPS��Ϣ */
		Mcu_Uart4_ReceDataP(USART_ReceiveData(UART4));

		/* ��������жϱ�־ */
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);	
 	}

	/* �жϽ�������ж�״̬ */
	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE) != RESET)
    {
		/* ��������ж� */
		/* �������ж� */
		USART_ClearFlag(UART4,USART_FLAG_ORE);
		USART_ReceiveData(UART4);
    }

	/* ��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л��� */
	OSIntExit();
}

/************************************************************************************************/
/* ������	: Mcu_Uart4_GPIOInit										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��Uart4��GPIO						 												*/
/* ��������	: 2016/11/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void Mcu_Uart4_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ͨ��GPIO */
	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(Uart4_TX_CLK | Uart4_RX_CLK , ENABLE );
	
	GPIO_SetBits(Uart4_TX_PORT,Uart4_TX_PIN);
	GPIO_SetBits(Uart4_RX_PORT,Uart4_RX_PIN);

	/* GPIOģʽ */
	GPIO_InitStructure.GPIO_Pin 	= 	Uart4_TX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* д��Ĵ��� */
	GPIO_Init(Uart4_TX_PORT, &GPIO_InitStructure);

	
	/* GPIOģʽ */
	GPIO_InitStructure.GPIO_Pin 	= 	Uart4_RX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* д��Ĵ��� */
	GPIO_Init(Uart4_RX_PORT, &GPIO_InitStructure);
	
}



/************************************************************************************************/
/* ������	: Mcu_Uart4_ModeInit										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��Uart4ͨ��ģʽ����						 										*/
/* ��������	: 2016/11/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void Mcu_Uart4_ModeInit(uint32_t baudrate)
{
	USART_InitTypeDef USART_InitStructure;

	
	/* ����ͨ�Ź���:Uart4 */
	/* ����ʱ�� */
	USART_DeInit(UART4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
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
	USART_Init(UART4,&USART_InitStructure);
	
	/* ���ý����ж� */
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	
	/* ���ô���DMA���� */
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
	
	/* ʹ��Uart4 */
	USART_Cmd(UART4 , ENABLE);
	
}


/************************************************************************************************/
/* ������	: Uart4_Init										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��Uart4ͨ��ģʽ����						 										*/
/* ��������	: 2016/11/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void Uart4_Init(uint32_t baudrate,void (*funtp)(uint8_t))
{
	/* ��ʼ��Uart4ͨ��ģʽ���� */
	Mcu_Uart4_ModeInit(baudrate);	
	
	/* ��ʼ��Uart4��GPIO */
	Mcu_Uart4_GPIOInit();

	/* ��ʼ���������ݺ���ָ�� */
	Mcu_Uart4_ReceDataP = funtp;
}

/************************************************************************************************/
/* ������	: Mcu_Uart4_GPIODeInit										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �����ʼ��Uart4��GPIO						 										*/
/* ��������	: 2016/11/28																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void Mcu_Uart4_GPIODeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ͨ��GPIO */

	/* GPIOģʽ */
	GPIO_InitStructure.GPIO_Pin 	= 	Uart4_TX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AIN;
	/* д��Ĵ��� */
	GPIO_Init(Uart4_TX_PORT, &GPIO_InitStructure);

	
	/* GPIOģʽ */
	GPIO_InitStructure.GPIO_Pin 	= 	Uart4_RX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* д��Ĵ��� */
	GPIO_Init(Uart4_RX_PORT, &GPIO_InitStructure);
}

/************************************************************************************************/
/* ������	: Mcu_Uart4_Close										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��Uart4ͨ��ģʽ����						 										*/
/* ��������	: 2016/11/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void Mcu_Uart4_Close(void)
{
	/* ʹ��Uart4 */
	USART_Cmd(UART4 , DISABLE);
	/* ��GPIO������Ϊģ������ */
	Mcu_Uart4_GPIODeInit();
}

/************************************************************************************************/
/* ������	: Mcu_Uart4_Open										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��Uart4ͨ��ģʽ����						 										*/
/* ��������	: 2016/11/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void Mcu_Uart4_Open(void)
{
	/* ʹ��Uart4 */
	USART_Cmd(UART4 , ENABLE);
	/* ��GPIO������Ϊģ������ */
	Mcu_Uart4_GPIOInit();
}

/************************************************************************************************/
/* ������	: Uart4_SendData										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��Uart4ͨ��ģʽ����						 										*/
/* ��������	: 2016/11/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void Uart4_SendData(uint8_t data)
{	
	
	USART_SendData(UART4,data);
}





/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

