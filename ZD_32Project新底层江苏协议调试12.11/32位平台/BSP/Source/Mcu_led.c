#include "Mcu_led.h"
#include "app.h"



/* �� ���ź� ���� ��Ϣ */
//static LIGHT_QUE Light_QUE_Null_Alarm_Signal_Module		= {1u,1000u,TRUE};
/* ������ �� ���� ��Ϣ */
static LIGHT_QUE Light_QUE_Sensor_Alarm_Signal_Module		= {500u,500u,TRUE};
/* ���� �� ���� ��Ϣ */
static LIGHT_QUE Light_QUE_Power_Alarm_Signal_Module		= {100u,100u,TRUE};
/* ���� �� ���� ��Ϣ */
static LIGHT_QUE Light_QUE_Battery_Alarm_Signal_Module		= {200u,200u,TRUE};

/*
 * ��������LED_GPIO_Init
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void LED_GPIO_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//LED2/LED1
	GPIO_InitStructure.GPIO_Pin		= LED2_PIN | LED1_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	LED1(LED_OFF);
	Light(Light_OFF);
}

/*
*********************************************************************************************************
*	�� �� ��: Battery_Alarm_Light
*	����˵��: ��ص��� �ⱨ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Battery_Alarm_Light(void)
{					
	Light_Send_Q((OS_EVENT*)Light_EVENT_Str_Q,Battery_Signal_Light_EVENT);
}

/*
*********************************************************************************************************
*	�� �� ��: Power_Alarm_Light
*	����˵��: ��ӵ� �ⱨ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Power_Alarm_Light(void)
{					
	Light_Send_Q((OS_EVENT*)Light_EVENT_Str_Q,Power_Signal_Light_EVENT);
}

/*
*********************************************************************************************************
*	�� �� ��: Sensor_Alarm_Light
*	����˵��: ������ �ⱨ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Sensor_Alarm_Light(void)
{					
	Light_Send_Q((OS_EVENT*)Light_EVENT_Str_Q,Sensor_Signal_Light_EVENT);
}



/*
*********************************************************************************************************
*	�� �� ��: Heart_Runing_Led
*	����˵��: ��������ָʾ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Heart_Runing_Led(void)
{				
   static BOOL LED1_Flag=FALSE;//��������������Flag Ĭ�������״̬	
	/*********************�����ƿ��Ʒ���LEDZWC****************************/			
			   if(LED1_Flag==TRUE)
				 {
					 LED1_Flag=FALSE;
				   LED1(LED_ON);//ZWCled
				 
				 }
				 else if(LED1_Flag==FALSE)
				 {
					 LED1_Flag=TRUE;
				   LED1(LED_OFF);//ZWCled
				 }				 					
/***************************END***************************************/
	
}



/*
*********************************************************************************************************
*	�� �� ��: Light_Send_Q
*	����˵��: �ⱨ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Light_Send_Q(OS_EVENT* pevent,LIGHT_CMD_TYPE EVENT_Type)
	{
	INT8U     	err;
	OS_Q_DATA	Q_data;
	LIGHT_QUE*	EVENT_p;
	u16 		num;
	u32 		Q_p;

	
	/* ���͵� �¼� ����  */
	if(EVENT_Type == Null_Signal_Light_EVENT)
	{
		/* ���ź� ���� */
		/* ��ʱ û���� */
		return;
		
	}else if(EVENT_Type == Sensor_Signal_Light_EVENT)
	{
		/* ������ ���� ���� */
		EVENT_p = &Light_QUE_Sensor_Alarm_Signal_Module;	
	}else if(EVENT_Type == Power_Signal_Light_EVENT)
	{
		/* ������ ���� ���� */
		EVENT_p = &Light_QUE_Power_Alarm_Signal_Module;	
	}else if(EVENT_Type == Battery_Signal_Light_EVENT)
	{
		/* ������ ���� ���� */
		EVENT_p = &Light_QUE_Battery_Alarm_Signal_Module;	
	}
	
	
	/* ���򱨵����� ˵�������в����� �ϴ�������Ϣ */
	/* �ж� �ϴ����� ִ�н�� */
	if(EVENT_p->EVENT_Getarg == FALSE)
	{
		/* �ϴ�������Ϣ ���� ��ִ�У��ȴ�ִ�н��� */
		return;
	}
	
	//���� �ڴ�ռ����� �������Ϣֻ��һ�����ݿռ䣬��ˣ��ڷ�����Ϣǰ��Ҫȷ���ϴε���Ϣ�Ѿ���ִ���ˡ�
	//����ͬһ����Ϣ��ռ��2����Ϣ�ռ䣬����������Ϣ�޷����롣
	/* �����Ϣ�������Ƿ���� ���� ����״̬ ��Ϣ�� */
	err = OSQQuery(pevent,&Q_data);
	
	/* �жϲ�ѯ��� */
	if(err == OS_ERR_NONE)
	{
		/* ��ѯ�ɹ� */
		
		/* ���� �������Ƿ��Ѿ����� ����״̬ ��Ϣ�� */
		for(num = 0,Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQOut);
			num < Q_data.OSNMsgs;
			num ++)
		{
			/* �жϵ�ǰ����Ϣָ���뷢��ָ���Ƿ���ͬ */
			if((*(u32*)Q_p) == (u32)EVENT_p)
			{
				/* ��ȣ������д��� ����״̬ ��Ϣ,��ô�Ͳ��ڷ��� ����״̬ ��Ϣ */
				return;
			}
			/* ����ָ�뵽��һ��λ�� */
			Q_p += sizeof(LIGHT_QUE);
			/* �ж�ָ���Ƿ�Խ�� */
			if(Q_p == (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQEnd))
			{
				/* Խ��ͽ� ָ�� ָ��ͷλ�� */
				Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQStart);
			}
		}
	}else
	{
		/* ��ѯʧ�� */
		__nop();
	}
	
	/* ���������˵�� ������û�д��� �ź�ǿ�� ��Ϣ��
	   ��ˣ���Ҫ����һ�� �ź�ǿ�ȸ��� ��Ϣ�� */
	/* ��� ��Ϣ ִ�н�� */
	EVENT_p->EVENT_Getarg = FALSE;
	/* ���� ��Ϣ *///FIFO	
	err = OSQPost(pevent, EVENT_p);  
	/* �ж� ��Ϣ ���� ״̬ */
	if(err == OS_ERR_NONE)
	{
		/* �ж� ��Ϣ ���� �ɹ� */
		__nop();
	}else if(err == OS_ERR_Q_FULL)
	{
		/* ��Ϣ��������,�����ٽ�������Ϣ */
		/* ������� ������ ���� */
		while(1);
	}else
	{
		__nop();
	}
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

