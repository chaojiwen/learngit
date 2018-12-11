/************************************************************************************************/
/** @file              : Mcu_USART2.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年12月14日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--STM32F103-USART2逻辑层文件						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0      ZRDang      2016年12月14日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Mcu_USART2.h"

#include "ucos_ii.h"


/************************************************************************************************/
/* 接收数据函数指针 */
void (*Mcu_USART2_ReceDataP)(uint8_t);

/************************************************************************************************/
/* USART2	接受数据缓冲区 */	   

/************************************************************************************************/



/************************************************************************************************/
/* 函数名	: Mcu_USART2_ModeInit										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化USART2通信模式功能						 										*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
//void USART2_IRQHandler(void)
//{
//	OS_CPU_SR  cpu_sr;
//	
//	
//	OS_ENTER_CRITICAL();  	//保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR
//	OSIntNesting++;			//中断嵌套标志
//	OS_EXIT_CRITICAL();		//恢复全局中断标志 

//	
//	/* USART2 接收中断(每次接收一个字节) */
//	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
// 	{	
//		
//		/* 接收GPS信息 */
//		Mcu_USART2_ReceDataP(USART_ReceiveData(USART2));

//		/* 清除接收中断标志 */
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	
// 	}

//	/* 判断接收溢出中断状态 */
//	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) != RESET)
//    {
//		/* 产生溢出中断 */
//		/* 清除溢出中断 */
//		USART_ClearFlag(USART2,USART_FLAG_ORE);
//		USART_ReceiveData(USART2);
//    }

//	/* 在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换。 */
//	OSIntExit();
//}

/************************************************************************************************/
/* 函数名	: Mcu_USART2_GPIOInit										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化USART2的GPIO						 												*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void Mcu_USART2_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 通信GPIO */
	/* 配置时钟 */
	RCC_APB2PeriphClockCmd(USART2_TX_CLK | USART2_RX_CLK , ENABLE );
	
	GPIO_SetBits(USART2_TX_PORT,USART2_TX_PIN);
	GPIO_SetBits(USART2_RX_PORT,USART2_RX_PIN);

	/* GPIO模式 */
	GPIO_InitStructure.GPIO_Pin 	= 	USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* 写入寄存器 */
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);

	
	/* GPIO模式 */
	GPIO_InitStructure.GPIO_Pin 	= 	USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* 写入寄存器 */
	GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);
	
}



/************************************************************************************************/
/* 函数名	: Mcu_USART2_ModeInit										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化USART2通信模式功能						 										*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void Mcu_USART2_ModeInit(uint32_t baudrate)
{
	USART_InitTypeDef USART_InitStructure;

	
	/* 配置通信功能:USART2 */
	/* 配置时钟 */
	USART_DeInit(USART2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
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
	USART_Init(USART2,&USART_InitStructure);
	
	/* 配置接收中断 */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	/* 配置串口DMA功能 */
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	
	/* 使能USART2 */
	USART_Cmd(USART2 , ENABLE);
	
}


/************************************************************************************************/
/* 函数名	: USART2_Init										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化USART2通信模式功能						 										*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void USART2_Init(uint32_t baudrate,void (*funtp)(uint8_t))
{
	/* 初始化USART2通信模式功能 */
	Mcu_USART2_ModeInit(baudrate);	
	
	/* 初始化USART2的GPIO */
	Mcu_USART2_GPIOInit();

	/* 初始化接收数据函数指针 */
	Mcu_USART2_ReceDataP = funtp;
}

/************************************************************************************************/
/* 函数名	: Mcu_USART2_GPIODeInit										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 反向初始化USART2的GPIO						 										*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void Mcu_USART2_GPIODeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 通信GPIO */
	
	/* GPIO模式 */
	GPIO_InitStructure.GPIO_Pin 	= 	USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AIN;
	/* 写入寄存器 */
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);
	
	
	/* GPIO模式 */
	GPIO_InitStructure.GPIO_Pin 	= 	USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AIN;
//	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_10MHz;
	/* 写入寄存器 */
	GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);
}

/************************************************************************************************/
/* 函数名	: Mcu_USART2_Close										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化USART2通信模式功能						 										*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void Mcu_USART2_Close(void)
{
	/* 使能USART2 */
	USART_Cmd(USART2 , DISABLE);
	/* 将GPIO口设置为模拟输入 */
	Mcu_USART2_GPIODeInit();
}

/************************************************************************************************/
/* 函数名	: Mcu_USART2_Open										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化USART2通信模式功能						 										*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void Mcu_USART2_Open(void)
{
	/* 使能USART2 */
	USART_Cmd(USART2 , ENABLE);
	/* 将GPIO口设置为模拟输入 */
	Mcu_USART2_GPIOInit();
}

/************************************************************************************************/
/* 函数名	: USART2_SendData										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化USART2通信模式功能						 										*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void USART2_SendData(uint8_t data)
{	
	
	USART_SendData(USART2,data);
}





/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

