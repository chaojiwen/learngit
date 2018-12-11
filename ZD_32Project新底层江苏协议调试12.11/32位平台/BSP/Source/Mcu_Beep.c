#include "Mcu_Beep.h"
#include "app.h"



/* ���� ���ź� ���� ��Ϣ */
//static Beep_QUE Beep_QUE_Null_Alarm_Signal_Module		= {1u,1000u,TRUE};
/* ������ ���� ���� ��Ϣ */
static Beep_QUE Beep_QUE_Sensor_Alarm_Signal_Module		= {500u,500u,TRUE};
/* ���� ���� ���� ��Ϣ */
static Beep_QUE Beep_QUE_Power_Alarm_Signal_Module		= {100u,100u,TRUE};
/* ��ص��� ���� ��Ϣ */
static Beep_QUE Beep_QUE_Battery_Alarm_Signal_Module	= {200u,200u,TRUE};
/* ��ص��� ���� ��Ϣ */
static Beep_QUE Beep_QUE_Menu_Key_Signal_Module			= {15u,5u,TRUE};

/*
 * ��������Beep_Init
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Beep_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= Beep_PIN;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(Beep_PORT, &GPIO_InitStructure);
	
	Beep(Beep_OFF);	 
}

/*
*********************************************************************************************************
*	�� �� ��: Battery_Alarm_Sound
*	����˵��: ���� ��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Battery_Alarm_Sound(void)
{					
	Beep_Send_Q((OS_EVENT*)Beep_EVENT_Str_Q,Battery_Signal_Beep_EVENT);
}

/*
*********************************************************************************************************
*	�� �� ��: Power_Alarm_Sound
*	����˵��: ���� ��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Power_Alarm_Sound(void)
{					
	Beep_Send_Q((OS_EVENT*)Beep_EVENT_Str_Q,Power_Signal_Beep_EVENT);
}

/*
*********************************************************************************************************
*	�� �� ��: Sensor_Alarm_Sound
*	����˵��: ������ ��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Sensor_Alarm_Sound(void)
{					
	Beep_Send_Q((OS_EVENT*)Beep_EVENT_Str_Q,Sensor_Signal_Beep_EVENT);
	 
}

/************************************************************************************************/
/* ������	: Beep_Menu_Key_Sound																*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ���Ͳ˵�����İ�������																*/
/* ��������	: 2015/6/4																			*/
/************************************************************************************************/
void Beep_Menu_Key_Sound(void)
{					
	Beep_Send_Q((OS_EVENT*)Beep_EVENT_Str_Q,Beep_Menu_Key_Sound_EVENT);
}


/*
*********************************************************************************************************
*	�� �� ��: Audible_Alarm
*	����˵��: ��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Alarm_Audible(void)
{

//	static BOOL state = FALSE;

//	if(state == TRUE)
//	{
//		Beep(Beep_ON);
//		state = FALSE;
//	}else
//	{
//		Beep(Beep_OFF);
//		state = TRUE;
//	}						
//	Beep_Send_Q((OS_EVENT*)Beep_EVENT_Str_Q,Alarm_Signal_EVENT);
}

/*
*********************************************************************************************************
*	�� �� ��: Light_Alarm
*	����˵��: �ⱨ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
BOOL Beep_Send_Q(OS_EVENT* pevent,BEEP_CMD_TYPE EVENT_Type)
	{
	INT8U     	err;
	OS_Q_DATA	Q_data;
	Beep_QUE*	EVENT_p;	
	u16 		num;
	u32 		Q_p;

	
	/* ���͵� �¼� ����  */
	if(EVENT_Type == Null_Signal_Beep_EVENT)
	{
		/* ���ź� ���� */
		/* ��ʱ û���� */
		return FALSE;
		
	}else if(EVENT_Type == Sensor_Signal_Beep_EVENT)
	{
		/* ������ ���� ���� */
		EVENT_p = &Beep_QUE_Sensor_Alarm_Signal_Module;	
	}else if(EVENT_Type == Power_Signal_Beep_EVENT)
	{
		/* ������ ���� ���� */
		EVENT_p = &Beep_QUE_Power_Alarm_Signal_Module;	
	}else if(EVENT_Type == Battery_Signal_Beep_EVENT)
	{
		/* ������ ���� ���� */
		EVENT_p = &Beep_QUE_Battery_Alarm_Signal_Module;	
	}
	else if(EVENT_Type == Beep_Menu_Key_Sound_EVENT)
	{
		/* ��ص��� ���� ��Ϣ */
		EVENT_p = &Beep_QUE_Menu_Key_Signal_Module;
	}
	
	
	/* ���򱨵����� ˵�������в����� �ϴ�������Ϣ */
	/* �ж� �ϴ����� ִ�н�� */
	if(EVENT_p->EVENT_Getarg == FALSE)
	{
		/* �ϴ�������Ϣ ���� ��ִ�У��ȴ�ִ�н��� */
		return FALSE;
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
				return FALSE;
			}
			/* ����ָ�뵽��һ��λ�� */
			Q_p += sizeof(Beep_QUE);
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
		return FALSE;	
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
		/* ��Ϣ ���� �ɹ� */
		return TRUE;
	}else
	{
		/* ��Ϣ ���� ʧ�� */		
		return FALSE;		
	}
}



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

