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

/* USART1�����жϺ��� */
void USART1_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;

	/* ͨ��У��Ĵ��� */
	u8 	com_check = 0;
	/* ��ʱ������ */
	u8	num;
	/* ����������֡ */
	COM_SENSOR_CAL_TYPEDEF* com_sensor_cal_reg;
	
	OS_ENTER_CRITICAL();  	//����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//�ж�Ƕ�ױ�־
	OS_EXIT_CRITICAL();		//�ָ�ȫ���жϱ�־ 
	

	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		/* ���� ��һ������ */
		COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size] = USART_ReceiveData(USART1);
		
		/* �ж� У׼���� �Ϸ��� */
		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size == 0)
		{
			/* ���ݴ洢����Ϊ0 */
			/* �ж� ����ͷ�� */
			if(COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size] == Com_Sensor_Cal_Send_Head_Code)
			{
				/* ����ͷ����ȷ,�ۼӽ����������� */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size++;
				/* ���� ����֡ ���յĳ�ʱʱ�� */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout = Com_Sensor_Cal_Timeout;
			}
		}else if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size == 4)/* �жϽ��յ��������� */
		{
			/* ��ȡ��ǰ����֡�ĳ���=[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+N(����)+1(����)+1(У���) */
			Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len = 
			Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size + 1 + 
			COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size] + 1 + 1;
			/* ����ͷ����ȷ,�ۼӽ����������� */
			Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size++;
		}else
		{
			/* ����ͷ����ȷ,�ۼӽ����������� */
			Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size++;
			/* �ж� ������֡�ڽ��� */
			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len != 0)
			{
				/* �ж����ݽ������ */
				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size == 
				   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len)
				{
					/* ���ݽ������ */
					/* �жϽ��յ��ĳ����Ƿ���ȷ */
					if(COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len-2] == Com_Sensor_Cal_Constant)
					{
						/* ���ճ�����ȷ */
						/* ���㵱ǰ֡����У�� */
						for(num = 0; num < (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len-1); num++)
						{
							/* ��У�� */
							com_check	+=	COM1_RX_Buffer[num];
						}
						/* �жϵ�ǰУ��У�������Ƿ�Ϸ� */
						if(com_check == COM1_RX_Buffer[Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len-1])
						{
							/* ����У������ */
							/* ����ͨ������֡��Ϣ */
							com_sensor_cal_reg = (COM_SENSOR_CAL_TYPEDEF*)COM1_RX_Buffer;
							/* ���͵�ǰ������У׼������Ϣ */
							Com_Sensor_Cal_Send_Q(com_sensor_cal_reg);
						}
					}					
					/* ������վ������� */
					Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size = 0;
					/* �������֡���� */
					Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len = 0;
					/* �����ʱʱ�� */
					Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout = 0;
				}
			}
			
			/* �������ݻ��汣�� */
			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size == Com_Sensor_Cal_Packet_Len)
			{
				/* ������ǰͨ��Э������վݽ����� */
				/* ������վ������� */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size = 0;
				/* �������֡���� */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len = 0;
				/* �����ʱʱ�� */
				Instru_Fix_Inf_State_Tab.Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout = 0;
			}
		}
		
		/* ����Ӧ�����һ�� ͨ�� ��ʱ�Ļ��� */
		
		/* �������Ҫ����жϱ�־λ������ᵼ�� ϵͳһֱ���жϡ� */
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	/* ����жϱ�־λ */
	}
	OSIntExit();  //��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л�   
} 

#if ((GPS_Config_Enable == 0x01)&&(Usart_2_Function_Config_Enable))
/************************************************************************************************/
/* ������	: USART2_IRQHandler															 	 	*/
/* ����  	: ��																				*/
/* ���		: �� 																				*/
/* ����		: ����2�����жϣ������ַ������(GPS��Ϣ�����ݽ���)		 							*/
/* ��������	: 2017/7/30																			*/
/************************************************************************************************/
void USART2_IRQHandler(void)
{								   	
	OS_CPU_SR  cpu_sr;
	char	   ch;
	
	
	OS_ENTER_CRITICAL();  	//����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//�ж�Ƕ�ױ�־
	OS_EXIT_CRITICAL();		//�ָ�ȫ���жϱ�־ 

	
	/* Usart2 �����ж�(ÿ�ν���һ���ֽ�) */
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
 	{	
		
		/* ���浱ǰ���յ������� */
		ch = USART_ReceiveData(USART2);

		/* ����GPS��Ϣ */
		GPS_Rec_Data_Inf_Flow(ch);

		/* ��������жϱ�־ */
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	
 	}

	/* �жϽ�������ж�״̬ */
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) != RESET)
    {
		/* ��������ж� */
		/* �������ж� */
		USART_ClearFlag(USART2,USART_FLAG_ORE);
		USART_ReceiveData(USART2);
    }
	

	/* ��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л��� */
	OSIntExit();

}
#endif

#if (0)
/* I/O���жϣ��ж���ΪPA0 */
void EXTI0_IRQHandler(void)
{
	INT8U      err;
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();  	//����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//�ж�Ƕ�ױ�־
	OS_EXIT_CRITICAL();		//�ָ�ȫ���жϱ�־ 
	
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
	 
		/* ���� Adc���� ʱ���־ */	
		OSFlagPost(							//���־���ź�
				  (Str_RTC8025_WakeUp_Flag),//���ͱ�־��ָ��
				  ((OS_FLAGS)0XFF), 		//ѡ��Ҫ���͵��ź� ����1λ���ź� 0010  ͬ����2ǿ��ת��ΪOS_FLAGS�͵�����
											//��Ϊ�ź�ΪOS_FLAGS�͵�
				   OS_FLAG_SET,				//�ź���Ч��ѡ�� �ź���1  OS_FLAG_SETΪ��1  OS_FLAG_CLRΪ��0
				   &err);	
		EXTI_ClearITPendingBit(EXTI_Line0); //����жϱ�־λ
	}  
	OSIntExit();  //��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л�   
}
#endif

#if ((Wireless_RF_Card_Config_Enable == 0x01) && (Usart_2_Function_Config_Enable))
/************************************************************************************************/
/* ������	: DMA1_Channel2_IRQHandler													 	 	*/
/* ����  	: ��																				*/
/* ���		: �� 																				*/
/* ����		: DMA1ͨ��2���жϺ���								 									*/
/* ��������	: 2015/4/14																			*/
/************************************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();  	//����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//�ж�Ƕ�ױ�־
	OS_EXIT_CRITICAL();		//�ָ�ȫ���жϱ�־ 
	
	/* �ж� DMA ������� �ж� */
	if(DMA_GetITStatus(DMA1_IT_TC6) == SET)
	{
		/* ���� RF��Ƭ�� �ź��� */
		RF_Card_Request_Process_TC();
		/* ����жϱ�־λ������ᵼ�� ϵͳһֱ���жϡ� */
		DMA_ClearITPendingBit(DMA1_IT_TC6);
	}
	/* �ж� DMA �봫����� �ж� */
	else if(DMA_GetITStatus(DMA1_IT_HT6) == SET)
	{
		/* ���� RF��Ƭ�� �ź��� */
		RF_Card_Request_Process_HT();
		/* ����жϱ�־λ������ᵼ�� ϵͳһֱ���жϡ� */
		DMA_ClearITPendingBit(DMA1_IT_HT6);
	}
	
	
	OSIntExit();  //��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л�   
}
#endif


/************************************************************************************************/
/* ������	: ADC1_2_IRQHandler													 	 			*/
/* ����  	: ��																				*/
/* ���		: �� 																				*/
/* ����		: ADC1�ж�(����ֻ�õ�ģ�⿴�Ź��ж�)								 				*/
/* ��������	: 2015/10/20																		*/
/************************************************************************************************/
void ADC1_2_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();  	//����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;			//�ж�Ƕ�ױ�־
	OS_EXIT_CRITICAL();		//�ָ�ȫ���жϱ�־
	
	/* �ж� DMA ������� �ж� */
	if(ADC_GetITStatus(ADC1, ADC_IT_AWD) == SET)
	{
		/* ����жϱ�־λ������ᵼ�� ϵͳһֱ���жϡ� */
		ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
		
		//Ϊ�˽��RTCʱ��оƬ���ⱻд���ϣ������ȱ���һ�ε�ǰ��ʵʱʱ������Ϊ\
		  ���ϵ�������⵽RTC���쳣�����ʹ�����ʱ��������ʵʱʱ�䡣
		/* �޸� ���� ��Ϣ�� ���ñ�־ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_RTC,
					    sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC));
		
		/* ��λ���� */
 	NVIC_SystemReset();
	}

	OSIntExit();  			//��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л� 
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
