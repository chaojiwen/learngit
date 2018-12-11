/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/10/22																			 */
/*************************************************************************************************/
#include "Log_Power.h"






/*
*********************************************************************************************************
*	�� �� ��: Power_Link_State_Check
*	����˵��: ����ϵͳ����
*	��    �Σ���
*	�� �� ֵ: ��
*	˵��	������ϵͳ�м��50MS���һ�Ρ�
*********************************************************************************************************
*/
void Power_ET_Link_State_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
#define ET_Link_On		(Bit_RESET)
#define ET_Link_Off		(Bit_SET)	
	/* ������Ӷ˿�״̬ */
	u8 power_pin_static;
	/* ���״̬�ۼƼ����� */
	static u16 et_link_cnt = 0;

	/* ��ȡGPIO�ڵ�״̬ */
	power_pin_static = Ext_Power_ON_OFF_Check();

	/* �ж� ��ǰ ��ӵ������״̬ */
	if(power_t->Inf_State.ET_Link_State == TRUE)
	{
		/* ��ӵ㴦������״̬ */
		/* �ж������˿�״̬���е�/�޵磩  */
		if(power_pin_static == ET_Link_Off)
		{
			/* ���� ���״̬�ۼƼ����� */
			et_link_cnt++;
			/* �ж� ��ǰ�� ����״̬��ʱ�� �Ƿ� �趨��ֵ��Ĭ��1S�� */
			if(et_link_cnt > power_f->Inf.ET_Status_Update_Time)
			{
				/* ������� */
				et_link_cnt = 0;	
				/* ����״̬ */
				power_t->Inf_State.ET_Link_State = FALSE;
			}
		}else
		{
			/* ������� */
			/* ���� ��Դ���˿ڿ��� ��������źţ������Ҫ�������� */
			if(et_link_cnt)
			{
				/* ���� ���״̬�ۼƼ����� ��ֵ */
				et_link_cnt--;
			}	
		}
	}else
	{
		/* ��ӵ㴦��δ����״̬ */
		/* �ж������˿�״̬���е�/�޵磩  */
		if(power_pin_static == ET_Link_On)
		{
			/* ���� ���״̬�ۼƼ����� */
			et_link_cnt++;
			/* �ж� ��ǰ�� �ϵ�״̬��ʱ�� �Ƿ� �趨��ֵ��Ĭ��1S�� */
			if(et_link_cnt >  power_f->Inf.ET_Status_Update_Time)
			{
				/* ������� */
				et_link_cnt = 0;	
				/* ����״̬ */
				power_t->Inf_State.ET_Link_State = TRUE;
			}
		}else
		{
			/* ���� ��Դ���˿ڿ��� ��������źţ������Ҫ�������� */
			if(et_link_cnt)
			{
				/* ���� ���״̬�ۼƼ����� ��ֵ */
				et_link_cnt--;
			}	
		}
	}
	
#undef ET_Link_On
#undef ET_Link_Off
}

/*
*********************************************************************************************************
*	�� �� ��: Power_ET_SET_Alarm_State
*	����˵��: ��ӵ�Դ�ı���״̬����
*	��    �Σ�power_t: ������ӱ���״̬;power_f:��ʱû��ʹ�ã�
*	�� �� ֵ: ��
*	˵��	������ϵͳ�м��50MS���һ�Ρ�
*********************************************************************************************************
*/
void Power_ET_SET_Alarm_State(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* �жϵ�ǰ�������״̬ */
	if(power_t->Inf_State.ET_Link_State == FALSE)
	{
		/* ���δ���� */
		/* ���� ���籨�� */
		power_t->Inf_State.ET_Alarm_State =	TRUE; 
	}else
	{
		/* ��� ������ */
		/* ���� ������ */
		power_t->Inf_State.ET_Alarm_State =	FALSE; 
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Power_ET_Alarm_Check
*	����˵��: ��籨��״̬���
*	��    �Σ�power_t: ������ӱ���״̬;power_f:��ʱû��ʹ�ã�
*	�� �� ֵ: ��
*	˵��	������ϵͳ�м��50MS���һ�Ρ�
*********************************************************************************************************
*/
u8 Power_ET_Alarm_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{

	/* ͨ������״̬ */
	u8 ch_alarm_sr = 0;

	/* �ж� ������ӵı���״̬ */
	if(power_t->Inf_State.ET_Alarm_State  == TRUE)
	{
		/* ��ǰ�������Ϊ����״̬ */
		/* ���ݵ�ǰ��Ϣ���ã����ñ������ݡ� */
		ch_alarm_sr |= ((power_t->Inf_State.ET_Sound_Alarm_Switch == TRUE) ? 0X1:0X0);
		ch_alarm_sr |= ((power_t->Inf_State.ET_Light_Alarm_Switch == TRUE) ? 0X2:0X0);
		ch_alarm_sr |= ((power_t->Inf_State.ET_SMS_Alarm_Switch	  == TRUE) ? 0X4:0X0);

	}

	return ch_alarm_sr;
}


/************************************************************************************************/
/* ������	: Power_ET_Para_Init														 	 	*/
/* ����  	: *bt_state: ���״ָ̬�롣															*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��������ӵ�Դ״̬����							 								*/
/* �������� 	: 2015/08/13																		*/
/************************************************************************************************/
void Power_ET_Para_Init(INSTRU_POWER *power_state)
{

	/* �����������״̬������ */
	power_state->Inf_State.ET_Link_State = TRUE;
	/* ������籨��״̬��δ���� */
	power_state->Inf_State.ET_Alarm_State = FALSE;
	
}






/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

