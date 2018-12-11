/************************************************************************************************/
/** @file              : Mcu_Uart4.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月14日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--STM32F103-Uart4逻辑层文件						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0      ZRDang      2016年11月14日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Mcu_Uart4.h"

#include "ucos_ii.h"


/************************************************************************************************/
/* 接收数据函数指针 */
void (*Mcu_Uart4_ReceDataP)(uint8_t);

/************************************************************************************************/
/* Uart4	接受数据缓冲区 */	   

/************************************************************************************************/



/************************************************************************************************/
/* 函数名	: Mcu_Uart4_ModeInit										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化Uart4通信模式功能						 										*/
/* 创建日期	: 2016/11/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void UART4_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	
	
	OS_ENTER_CRITICAL();  	//保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//中断嵌套标志
	OS_EXIT_CRITICAL();		//恢复全局中断标志 

	
	/* Uart4 接收中断(每次接收一个字节) */
	if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)
 	{	
		
		/* 接收GPS信息 */
		Mcu_Uart4_ReceDataP(USART_ReceiveData(UART4));

		/* 清除接收中断标志 */
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);	
 	}

	/* 判断接收溢出中断状态 */
	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE) != RESET)
    {
		/* 产生溢出中断 */
		/* 清除溢出中断 */
		USART_ClearFlag(UART4,USART_FLAG_ORE);
		USART_ReceiveData(UART4);
    }

	/* 在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换。 */
	OSIntExit();
}

/************************************************************************************************/
/* 函数名	: Mcu_Uart4_GPIOInit										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化Uart4的GPIO						 												*/
/* 创建日期	: 2016/11/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void Mcu_Uart4_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 通信GPIO */
	/* 配置时钟 */
	RCC_APB2PeriphClockCmd(Uart4_TX_CLK | Uart4_RX_CLK , ENABLE );
	
	GPIO_SetBits(Uart4_TX_PORT,Uart4_TX_PIN);
	GPIO_SetBits(Uart4_RX_PORT,Uart4_RX_PIN);

	/* GPIO模式 */
	GPIO_InitStructure.GPIO_Pin 	= 	Uart4_TX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* 写入寄存器 */
	GPIO_Init(Uart4_TX_PORT, &GPIO_InitStructure);

	
	/* GPIO模式 */
	GPIO_InitStructure.GPIO_Pin 	= 	Uart4_RX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* 写入寄存器 */
	GPIO_Init(Uart4_RX_PORT, &GPIO_InitStructure);
	
}



/************************************************************************************************/
/* 函数名	: Mcu_Uart4_ModeInit										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化Uart4通信模式功能						 										*/
/* 创建日期	: 2016/11/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void Mcu_Uart4_ModeInit(uint32_t baudrate)
{
	USART_InitTypeDef USART_InitStructure;

	
	/* 配置通信功能:Uart4 */
	/* 配置时钟 */
	USART_DeInit(UART4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	/* 初始化数据 */
	USART_StructInit(&USART_InitStructure);
	
	/* UART1工作模式配置 */
	USART_InitStructure.USART_BaudRate 		= baudrate;
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
	USART_InitStructure.USART_Parity 		= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 			= USART_Mode_Tx | USART_Mode_Rx;
	/* 写入寄存器 */
	USART_Init(UART4,&USART_InitStructure);
	
	/* 配置接收中断 */
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	
	/* 配置串口DMA功能 */
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
	
	/* 使能Uart4 */
	USART_Cmd(UART4 , ENABLE);
	
}


/************************************************************************************************/
/* 函数名	: Uart4_Init										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化Uart4通信模式功能						 										*/
/* 创建日期	: 2016/11/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void Uart4_Init(uint32_t baudrate,void (*funtp)(uint8_t))
{
	/* 初始化Uart4通信模式功能 */
	Mcu_Uart4_ModeInit(baudrate);	
	
	/* 初始化Uart4的GPIO */
	Mcu_Uart4_GPIOInit();

	/* 初始化接收数据函数指针 */
	Mcu_Uart4_ReceDataP = funtp;
}

/************************************************************************************************/
/* 函数名	: Mcu_Uart4_GPIODeInit										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 反向初始化Uart4的GPIO						 										*/
/* 创建日期	: 2016/11/28																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void Mcu_Uart4_GPIODeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 通信GPIO */

	/* GPIO模式 */
	GPIO_InitStructure.GPIO_Pin 	= 	Uart4_TX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AIN;
	/* 写入寄存器 */
	GPIO_Init(Uart4_TX_PORT, &GPIO_InitStructure);

	
	/* GPIO模式 */
	GPIO_InitStructure.GPIO_Pin 	= 	Uart4_RX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* 写入寄存器 */
	GPIO_Init(Uart4_RX_PORT, &GPIO_InitStructure);
}

/************************************************************************************************/
/* 函数名	: Mcu_Uart4_Close										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化Uart4通信模式功能						 										*/
/* 创建日期	: 2016/11/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void Mcu_Uart4_Close(void)
{
	/* 使能Uart4 */
	USART_Cmd(UART4 , DISABLE);
	/* 将GPIO口设置为模拟输入 */
	Mcu_Uart4_GPIODeInit();
}

/************************************************************************************************/
/* 函数名	: Mcu_Uart4_Open										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化Uart4通信模式功能						 										*/
/* 创建日期	: 2016/11/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void Mcu_Uart4_Open(void)
{
	/* 使能Uart4 */
	USART_Cmd(UART4 , ENABLE);
	/* 将GPIO口设置为模拟输入 */
	Mcu_Uart4_GPIOInit();
}

/************************************************************************************************/
/* 函数名	: Uart4_SendData										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化Uart4通信模式功能						 										*/
/* 创建日期	: 2016/11/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void Uart4_SendData(uint8_t data)
{	
	
	USART_SendData(UART4,data);
}





/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

