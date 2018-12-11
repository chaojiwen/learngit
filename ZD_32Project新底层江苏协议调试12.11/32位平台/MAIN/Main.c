/************************************************************************************************/
/** @file              : Main.c																	*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年11月11日															*/
/** @brief             : c file																	*/
/** @description       : 32为平台应用文件															*/
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
/* ------------------------------------ 系统中断 宏 设置 ---------------------------------------- */
/************************************************************************************************/
/* 串口1 中断 */
#define USART1_Interrupt
/* 串口2 中断 */
#define USART2_Interrupt
/* 串口4 中断 */
#define Uart4_Interrupt
/* 外部线上0 中断 */
#define EXTI0_Interrupt
/* 模拟看门狗 中断 */
#define AnalogWatchDog_Interrupt
/* DMA1 中断 */
#define DMA1_Interrupt


/* 定义 串口1 抢占优先级和子优先级 */
#ifdef	USART1_Interrupt
#define USART1_Preemption_Priority			(2u)
#define USART1_Sub_Priority					(2u)
#endif

/* 定义 串口2 抢占优先级和子优先级 */
#ifdef	USART2_Interrupt
#define USART2_Preemption_Priority			(2u)
#define USART2_Sub_Priority					(4u)
#endif

/* 定义 串口1 抢占优先级和子优先级 */
#ifdef	Uart4_Interrupt
#define UART4_Preemption_Priority			(2u)
#define UART4_Sub_Priority					(3u)
#endif

/* 定义 外部线上0 抢占优先级和子优先级 */
#ifdef	EXTI0_Interrupt
#define EXTI0_Preemption_Priority			(1u)
#define EXTI0_Sub_Priority					(1u)
#endif

/* 定义 模拟看门狗 抢占优先级和子优先级 */
#ifdef	AnalogWatchDog_Interrupt
#define AnalogWatchDog_Preemption_Priority	(1u)
#define AnalogWatchDog_Sub_Priority			(0u)
#endif

/* 定义 DMA1 抢占优先级和子优先级 */
#ifdef	DMA1_Interrupt
#define DMA1_Channel6_Preemption_Priority	(1u)
#define DMA1_Channel6_Sub_Priority			(3u)
#endif


/************************************************************************************************/
/* ---------------------------------------- 变量 设置 ------------------------------------------ */
/************************************************************************************************/
/* 起始任务栈空间 */
static OS_STK Startup_Task_Stk[STARTUP_TASK_STK_SIZE];

/************************************************************************************************/
/* 函数名	: Mcu_NVIC_Configuration															*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化配置外设中断（中断分组，配置对应的中断、抢占、子优先级）					 		*/
/* 创建日期 	: 2015/11/11																		*/
/* 注意		: 这个函数应在对应的的外设被调用的使用设置才对。(现在暂时这样写后面有时间再更改)				*/
/************************************************************************************************/
static void Mcu_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 配置中断优先级分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);											//中断分组设置为1
	
	
	/* 使能USART1中断 */
#ifdef	USART1_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;					//配置外部中断源（USART1中断）
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= USART1_Preemption_Priority;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= USART1_Sub_Priority;			//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//使能通道
	NVIC_Init(&NVIC_InitStructure);  								  						//向寄存器中写如数据
#endif
	
	/* 使能USART2中断 */
#ifdef	USART2_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;					//配置外部中断源（USART2中断）
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= USART2_Preemption_Priority;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= USART2_Sub_Priority;			//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//使能通道
	NVIC_Init(&NVIC_InitStructure);  								  						//向寄存器中写如数据
#endif
	
	/* 使能UART4中断 */
#ifdef	Uart4_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel 						= UART4_IRQn;					//配置外部中断源（UART4中断）
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= UART4_Preemption_Priority;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= UART4_Sub_Priority;			//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//使能通道
	NVIC_Init(&NVIC_InitStructure);  								  						//向寄存器中写如数据
#endif
	
	/* 使能EXTI0中断 */
#ifdef	EXTI0_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;					//配置外部中断源（USART1中断）
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= EXTI0_Preemption_Priority;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= EXTI0_Sub_Priority;			//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//使能通道
	NVIC_Init(&NVIC_InitStructure);  								  						//向寄存器中写如数据
#endif	

#ifdef	AnalogWatchDog_Interrupt
	NVIC_InitStructure.NVIC_IRQChannel						= ADC1_2_IRQn;							//配置外部中断源（模拟看门狗中断）
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= AnalogWatchDog_Preemption_Priority;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= AnalogWatchDog_Sub_Priority;			//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;								//使能通道
	NVIC_Init(&NVIC_InitStructure); 		
#endif

	NVIC_InitStructure.NVIC_IRQChannel 						= DMA1_Channel6_IRQn;				//配置外部中断源（DMA1_Channel6中断）
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= DMA1_Channel6_Preemption_Priority;//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= DMA1_Channel6_Sub_Priority;		//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  		//使能通道
	NVIC_Init(&NVIC_InitStructure);  					
} 


/************************************************************************************************/
/* 函数名	: BSP_CPU_ClkFreq																	*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 读取CPU时钟频率，单位Hz														 		*/
/* 创建日期 	: 2015/11/11																		*/
/************************************************************************************************/
static uint32_t BSP_CPU_ClkFreq(void)
{
	static  RCC_ClocksTypeDef  rcc_clocks;
	
	RCC_GetClocksFreq(&rcc_clocks);
	
	return ((INT32U)rcc_clocks.HCLK_Frequency);
}

/************************************************************************************************/
/* 函数名	: SysTick_init																		*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置 SysTick 定时器														 		*/
/* 创建日期 	: 2015/11/11																		*/
/************************************************************************************************/
static void SysTick_init(void)    
{    
  SysTick_Config(BSP_CPU_ClkFreq()/OS_TICKS_PER_SEC);//初始化并使能 SysTick 定时器    
}
      
int main(void)
{
	
	/* 配置系统时钟: 默认主时钟设置为:72M */
	SystemInit();
	
#ifndef STARTUP_FROM_BOOTLOADER
	/* 设置MCU中断向量表的偏移量(相对地址格式) */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,BLOCK_APPLICATION_START-BASE_OF_ROM);
#endif
	
	/* 初始化并使能 SysTick 定时器 */
	SysTick_init();    				
	
	/* 中断源优先级初始化  */
	//这里系统任务还没跑起来，之前中断都不开。假如在启动UCOS-ii过程中又中断，很可能会启动失败。
	Mcu_NVIC_Configuration();
	
	/* 使能 复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	/* 设置 Jlink 的烧写调试方式 */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭JTAG功能(解决spi3的nss管脚与JTAG公用管脚冲突问题,但这样做无法通过JTAG烧录程序)
	
	/* 初始化 OS */
    OSInit();
	
	/* 创建 起始任务 */
    OSTaskCreate(Task_Start,(void *)0,&Startup_Task_Stk[STARTUP_TASK_STK_SIZE-1], Startup_Task_PRIO);
	
	/* 启动 OS */
    OSStart();
    
    return 0;
}


/************************************************ (C) HZ Zeda Team *********************************************/

