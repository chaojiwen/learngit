/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "app.h"
#include "System_Config.h"

#include "Dev_Fram.h"

#include "Mcu_GPS.h"
#include "Mcu_Usart.h"

#include "Log_COM.h"
#include "Log_Download.h"
#include "Log_RF_Card.h"
#include "Log_Sensor.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

vu32 nSystemTimingSystick = 0;
vs32 			TimingDelay;


void SysTick_Handler(void)    
{    
    
		OSIntEnter();     
    OSTimeTick(); 
		TimingDelay--;
		nSystemTimingSystick++;
    OSIntExit();     
} 

/* USART1接收中断函数 */
void USART1_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;

	/* 通信校验寄存器 */
	u8 	com_check = 0;
	/* 临时计数器 */
	u8	num;
	/* 完整的数据帧 */
	COM_SENSOR_CAL_TYPEDEF* com_sensor_cal_reg;
	
	OS_ENTER_CRITICAL();  	//保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//中断嵌套标志
	OS_EXIT_CRITICAL();		//恢复全局中断标志 
	

	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		/* 接收 第一个数据 */
		COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size] = USART_ReceiveData(USART1);
		
		/* 判断 校准数据 合法性 */
		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size == 0)
		{
			/* 数据存储数量为0 */
			/* 判断 接收头码 */
			if(COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size] == Com_Sensor_Cal_Send_Head_Code)
			{
				/* 接收头码正确,累加接收数据数量 */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size++;
				/* 设置 数据帧 接收的超时时间 */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout = Com_Sensor_Cal_Timeout;
			}
		}else if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size == 4)/* 判断接收的数据数量 */
		{
			/* 获取当前数据帧的长度=[1(头码)+1(设备地址)+1(操作码)+1(通道号)]+1(数据长度)+N(数据)+1(常数)+1(校验和) */
			Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len = 
			Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size + 1 + 
			COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size] + 1 + 1;
			/* 接收头码正确,累加接收数据数量 */
			Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size++;
		}else
		{
			/* 接收头码正确,累加接收数据数量 */
			Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size++;
			/* 判断 有数据帧在接收 */
			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len != 0)
			{
				/* 判断数据接收完成 */
				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size == 
				   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len)
				{
					/* 数据接收完成 */
					/* 判断接收到的常数是否正确 */
					if(COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len-2] == Com_Sensor_Cal_Constant)
					{
						/* 接收常数正确 */
						/* 计算当前帧数据校验 */
						for(num = 0; num < (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len-1); num++)
						{
							/* 和校验 */
							com_check	+=	COM1_RX_Buffer[num];
						}
						/* 判断当前校验校验数据是否合法 */
						if(com_check == COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len-1])
						{
							/* 数据校验正常 */
							/* 设置通信数据帧信息 */
							com_sensor_cal_reg = (COM_SENSOR_CAL_TYPEDEF*)COM1_RX_Buffer;
							/* 发送当前传感器校准数据消息 */
							Com_Sensor_Cal_Send_Q(com_sensor_cal_reg);
						}
					}					
					/* 清除接收据数据量 */
					Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size = 0;
					/* 清除数据帧长度 */
					Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len = 0;
					/* 清除超时时间 */
					Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout = 0;
				}
			}
			
			/* 接收数据缓存保护 */
			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size == Com_Sensor_Cal_Packet_Len)
			{
				/* 超出当前通信协议最大收据接收量 */
				/* 清除接收据数据量 */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size = 0;
				/* 清除数据帧长度 */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len = 0;
				/* 清除超时时间 */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout = 0;
			}
		}
		
		/* 这里应该添加一个 通信 超时的机制 */
		
		/* 这里必须要清除中断标志位，否则会导致 系统一直进中断。 */
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	/* 清除中断标志位 */
	}
	OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换   
} 

#if ((GPS_Config_Enable == 0x01)&&(Usart_2_Function_Config_Enable))
/************************************************************************************************/
/* 函数名	: USART2_IRQHandler															 	 	*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 串口2接收中断，特征字符串检测(GPS信息流数据接收)		 							*/
/* 创建日期	: 2017/7/30																			*/
/************************************************************************************************/
void USART2_IRQHandler(void)
{								   	
	OS_CPU_SR  cpu_sr;
	char	   ch;
	
	
	OS_ENTER_CRITICAL();  	//保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//中断嵌套标志
	OS_EXIT_CRITICAL();		//恢复全局中断标志 

	
	/* Usart2 接收中断(每次接收一个字节) */
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
 	{	
		
		/* 保存当前接收到的数据 */
		ch = USART_ReceiveData(USART2);

		/* 接收GPS信息 */
		GPS_Rec_Data_Inf_Flow(ch);

		/* 清除接收中断标志 */
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	
 	}

	/* 判断接收溢出中断状态 */
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) != RESET)
    {
		/* 产生溢出中断 */
		/* 清除溢出中断 */
		USART_ClearFlag(USART2,USART_FLAG_ORE);
		USART_ReceiveData(USART2);
    }
	

	/* 在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换。 */
	OSIntExit();

}
#endif

#if (0)
/* I/O线中断，中断线为PA0 */
void EXTI0_IRQHandler(void)
{
	INT8U      err;
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();  	//保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//中断嵌套标志
	OS_EXIT_CRITICAL();		//恢复全局中断标志 
	
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
	{
	 
		/* 发送 Adc采样 时间标志 */	
		OSFlagPost(							//向标志发信号
				  (Str_RTC8025_WakeUp_Flag),//发送标志的指针
				  ((OS_FLAGS)0XFF), 		//选择要发送的信号 给第1位发信号 0010  同样把2强制转化为OS_FLAGS型的数据
											//因为信号为OS_FLAGS型的
				   OS_FLAG_SET,				//信号有效的选项 信号置1  OS_FLAG_SET为置1  OS_FLAG_CLR为置0
				   &err);	
		EXTI_ClearITPendingBit(EXTI_Line0); //清除中断标志位
	}  
	OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换   
}
#endif

#if ((Wireless_RF_Card_Config_Enable == 0x01) && (Usart_2_Function_Config_Enable))
/************************************************************************************************/
/* 函数名	: DMA1_Channel2_IRQHandler													 	 	*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: DMA1通道2的中断函数								 									*/
/* 创建日期	: 2015/4/14																			*/
/************************************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();  	//保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//中断嵌套标志
	OS_EXIT_CRITICAL();		//恢复全局中断标志 
	
	/* 判断 DMA 传输完成 中断 */
	if(DMA_GetITStatus(DMA1_IT_TC6) == SET)
	{
		/* 发送 RF卡片的 信号量 */
		RF_Card_Request_Process_TC();
		/* 清除中断标志位，否则会导致 系统一直进中断。 */
		DMA_ClearITPendingBit(DMA1_IT_TC6);
	}
	/* 判断 DMA 半传输完成 中断 */
	else if(DMA_GetITStatus(DMA1_IT_HT6) == SET)
	{
		/* 发送 RF卡片的 信号量 */
		RF_Card_Request_Process_HT();
		/* 清除中断标志位，否则会导致 系统一直进中断。 */
		DMA_ClearITPendingBit(DMA1_IT_HT6);
	}
	
	
	OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换   
}
#endif


/************************************************************************************************/
/* 函数名	: ADC1_2_IRQHandler													 	 			*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: ADC1中断(现在只用到模拟看门狗中断)								 				*/
/* 创建日期	: 2015/10/20																		*/
/************************************************************************************************/
void ADC1_2_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();  	//保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//中断嵌套标志
	OS_EXIT_CRITICAL();		//恢复全局中断标志
	
	/* 判断 DMA 传输完成 中断 */
	if(ADC_GetITStatus(ADC1, ADC_IT_AWD) == SET)
	{
		/* 清除中断标志位，否则会导致 系统一直进中断。 */
		ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
		
		//为了解决RTC时钟芯片意外被写故障，这里先保存一次当前的实时时钟数据为\
		  当上电启动检测到RTC被异常后可以使用这个时间来做作实时时间。
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_RTC,
					    sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC));
		
		/* 复位仪器 */
 	NVIC_SystemReset();
	}

	OSIntExit();  			//在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换 
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
