/************************************************************************************************/
/** @file              : Main.c																	*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��11��11��															*/
/** @brief             : c file																	*/
/** @description       : 32Ϊƽ̨Ӧ���ļ�															*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "ucos_ii.h"
#include "app.h" 
#include "Mcu_Config.h"
#include "System_Config.h"
/************************************************************************************************/
/* ------------------------------------ ϵͳ�ж� �� ���� ---------------------------------------- */
/************************************************************************************************/
/* ����1 �ж� */
#define USART1_Interrupt
/* ����2 �ж� */
#define USART2_Interrupt
/* ����4 �ж� */
#define Uart4_Interrupt
/* �ⲿ����0 �ж� */
#define EXTI0_Interrupt
/* ģ�⿴�Ź� �ж� */
#define AnalogWatchDog_Interrupt
/* DMA1 �ж� */
#define DMA1_Interrupt


/* ���� ����1 ��ռ���ȼ��������ȼ� */
#ifdef	USART1_Interrupt
#define USART1_Preemption_Priority			(2u)
#define USART1_Sub_Priority					(2u)
#endif

/* ���� ����2 ��ռ���ȼ��������ȼ� */
#ifdef	USART2_Interrupt
#define USART2_Preemption_Priority			(2u)
#define USART2_Sub_Priority					(4u)
#endif

/* ���� ����1 ��ռ���ȼ��������ȼ� */
#ifdef	Uart4_Interrupt
#define UART4_Preemption_Priority			(2u)
#define UART4_Sub_Priority					(3u)
#endif

/* ���� �ⲿ����0 ��ռ���ȼ��������ȼ� */
#ifdef	EXTI0_Interrupt
#define EXTI0_Preemption_Priority			(1u)
#define EXTI0_Sub_Priority					(1u)
#endif

/* ���� ģ�⿴�Ź� ��ռ���ȼ��������ȼ� */
#ifdef	AnalogWatchDog_Interrupt
#define AnalogWatchDog_Preemption_Priority	(1u)
#define AnalogWatchDog_Sub_Priority			(0u)
#endif

/* ���� DMA1 ��ռ���ȼ��������ȼ� */
#ifdef	DMA1_Interrupt
#define DMA1_Channel6_Preemption_Priority	(1u)
#define DMA1_Channel6_Sub_Priority			(3u)
#endif


/************************************************************************************************/
/* ---------------------------------------- ���� ���� ------------------------------------------ */
/************************************************************************************************/
/* ��ʼ����ջ�ռ� */
static OS_STK Startup_Task_Stk[STARTUP_TASK_STK_SIZE];

/************************************************************************************************/
/* ������	: Mcu_NVIC_Configuration															*/
/* ����  	: �� 																				*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ�����������жϣ��жϷ��飬���ö�Ӧ���жϡ���ռ�������ȼ���					 		*/
/* �������� 	: 2015/11/11																		*/
/* ע��		: �������Ӧ�ڶ�Ӧ�ĵ����豻���õ�ʹ�����òŶԡ�(������ʱ����д������ʱ���ٸ���)				*/
/************************************************************************************************/
static void Mcu_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* �����ж����ȼ�����*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);											//�жϷ�������Ϊ1
	
	
	/* ʹ��USART1�ж� */
#ifdef	USART1_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;					//�����ⲿ�ж�Դ��USART1�жϣ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= USART1_Preemption_Priority;	//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= USART1_Sub_Priority;			//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//ʹ��ͨ��
	NVIC_Init(&NVIC_InitStructure);  								  						//��Ĵ�����д������
#endif
	
	/* ʹ��USART2�ж� */
#ifdef	USART2_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;					//�����ⲿ�ж�Դ��USART2�жϣ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= USART2_Preemption_Priority;	//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= USART2_Sub_Priority;			//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//ʹ��ͨ��
	NVIC_Init(&NVIC_InitStructure);  								  						//��Ĵ�����д������
#endif
	
	/* ʹ��UART4�ж� */
#ifdef	Uart4_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel 						= UART4_IRQn;					//�����ⲿ�ж�Դ��UART4�жϣ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= UART4_Preemption_Priority;	//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= UART4_Sub_Priority;			//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//ʹ��ͨ��
	NVIC_Init(&NVIC_InitStructure);  								  						//��Ĵ�����д������
#endif
	
	/* ʹ��EXTI0�ж� */
#ifdef	EXTI0_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;					//�����ⲿ�ж�Դ��USART1�жϣ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= EXTI0_Preemption_Priority;	//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= EXTI0_Sub_Priority;			//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//ʹ��ͨ��
	NVIC_Init(&NVIC_InitStructure);  								  						//��Ĵ�����д������
#endif	

#ifdef	AnalogWatchDog_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel						= ADC1_2_IRQn;							//�����ⲿ�ж�Դ��ģ�⿴�Ź��жϣ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= AnalogWatchDog_Preemption_Priority;	//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= AnalogWatchDog_Sub_Priority;			//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;								//ʹ��ͨ��
	NVIC_Init(&NVIC_InitStructure); 		
#endif

	NVIC_InitStructure.NVIC_IRQChannel 						= DMA1_Channel6_IRQn;				//�����ⲿ�ж�Դ��DMA1_Channel6�жϣ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= DMA1_Channel6_Preemption_Priority;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= DMA1_Channel6_Sub_Priority;		//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  		//ʹ��ͨ��
	NVIC_Init(&NVIC_InitStructure);  					
} 


/************************************************************************************************/
/* ������	: BSP_CPU_ClkFreq																	*/
/* ����  	: �� 																				*/
/* ���		: �� 																				*/
/* ����	 	: ��ȡCPUʱ��Ƶ�ʣ���λHz														 		*/
/* �������� 	: 2015/11/11																		*/
/************************************************************************************************/
static uint32_t BSP_CPU_ClkFreq(void)
{
	static  RCC_ClocksTypeDef  rcc_clocks;
	
	RCC_GetClocksFreq(&rcc_clocks);
	
	return ((INT32U)rcc_clocks.HCLK_Frequency);
}

/************************************************************************************************/
/* ������	: SysTick_init																		*/
/* ����  	: �� 																				*/
/* ���		: �� 																				*/
/* ����	 	: ���� SysTick ��ʱ��														 		*/
/* �������� 	: 2015/11/11																		*/
/************************************************************************************************/
static void SysTick_init(void)    
{    
  SysTick_Config(BSP_CPU_ClkFreq()/OS_TICKS_PER_SEC);//��ʼ����ʹ�� SysTick ��ʱ��    
}
      
int main(void)
{
	
	/* ����ϵͳʱ��: Ĭ����ʱ������Ϊ:72M */
	SystemInit();
	
#ifndef STARTUP_FROM_BOOTLOADER
	/* ����MCU�ж��������ƫ����(��Ե�ַ��ʽ) */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,BLOCK_APPLICATION_START-BASE_OF_ROM);
#endif
	
	/* ��ʼ����ʹ�� SysTick ��ʱ�� */
	SysTick_init();    				
	
	/* �ж�Դ���ȼ���ʼ��  */
	//����ϵͳ����û��������֮ǰ�ж϶�����������������UCOS-ii���������жϣ��ܿ��ܻ�����ʧ�ܡ�
	Mcu_NVIC_Configuration();
	
	/* ʹ�� ����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	/* ���� Jlink ����д���Է�ʽ */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ر�JTAG����(���spi3��nss�ܽ���JTAG���ùܽų�ͻ����,���������޷�ͨ��JTAG��¼����)
	
	/* ��ʼ�� OS */
    OSInit();
	
	/* ���� ��ʼ���� */
    OSTaskCreate(Task_Start,(void *)0,&Startup_Task_Stk[STARTUP_TASK_STK_SIZE-1], Startup_Task_PRIO);
	
	/* ���� OS */
    OSStart();
    
    return 0;
}


/************************************************ (C) HZ Zeda Team *********************************************/

