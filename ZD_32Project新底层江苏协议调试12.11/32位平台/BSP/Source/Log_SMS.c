/**
  ******************************************************************************
  * @file              : Log_SMS.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015��3��19�� 15:09:47
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
  */
#include "System_Config.h"

#include "Log_SMS.h"

#include "Log_Alarm.h"
#include "Log_GSM.h"
#include "Log_RF_Card.h"
#include "Log_Sensor.h"


#include <string.h>
#include <stdio.h>

#include "Unicode.c"
#include "app.h"

/* ���ڷ��Ͷ������õ�Unicode�룬������������ʹ�õ���GBK�룬
   ��˱������޷�Unicode�룬������ܶ಻��Ҫ�ľ��档
   ��� ��#pragma diag_suppress 870������ȡ����Щ���档   */
#pragma diag_suppress 870

///////////////* ���ű���ʱ�� */
//////////////#define SMS_Alarm_Time_String			"����ʱ��:%04d��%02d��%02d��,%02dʱ%02d��%02d��;\r\n"
///////////////* ������ */
////////////////#define SMS_SN_NO_String				"������:%X%X%X%X%05d;\r\n"
//////////////#define SMS_SN_NO_String				"������:%04X%05d;\r\n"
///////////////* �¶ȴ�����������ʾ */
//////////////#define SMS_Sensor_Temp_Para_String		"��ǰ����:%c%d.%01d��(����ֵ:%c%d.%01d��,����ֵ:%c%d.%01d��);\r\n"
///////////////* ʪ�ȴ�����������ʾ */
//////////////#define SMS_Sensor_Humi_Para_String		"��ǰ����:%d.%01d%%RH(����ֵ:%d.%01d%%RH,����ֵ:%d.%01d%%RH);\r\n"
///////////////* ��Ƭ�¶ȴ��������ճ�ʱ������ʾ */
//////////////#define SMS_RF_Card_Data_Rece_Timeout_Temp_Para_String	"��ǰ����:������(����ֵ:%c%d.%01d��,����ֵ:%c%d.%01d��);\r\n"
///////////////* ��Ƭʪ�ȴ��������ճ�ʱ������ʾ */
//////////////#define SMS_RF_Card_Data_Rece_Timeout_Humi_Para_String	"��ǰ����:������(����ֵ:%d.%01d%%RH,����ֵ:%d.%01d%%RH);\r\n"
///////////////* ������ͨ���� */
//////////////#define SMS_Sensor_CH_NO_String			"���������:%01d;\r\n"
///////////////* ������ͨ������ */
//////////////#define SMS_Sensor_CH_Type_String		"ͨ��%01d����:%s;\r\n"
///////////////* �¶ȴ����� */
//////////////#define SMS_Sensor_Temp_String			"�¶ȴ�����"
///////////////* ʪ�ȴ����� */
//////////////#define SMS_Sensor_Humi_String			"ʪ�ȴ�����"


///////////////* �Ͼ�ʢ�ڴ������¶� */
//////////////#define SMS_ShengYi_Sensor_Temp_String			"ʢ���¶ȴ�����"

///////////////* �Ͼ�ʢ�ڴ�����ʪ�� */
//////////////#define SMS_ShengYi_Sensor_Humi_String			"ʢ��ʪ�ȴ�����"



///////////////* ���¿�Ƭ������ */
//////////////#define SMS_Sensor_RF_Card_Single_Temp_String	"���¿�Ƭ������"
///////////////* ��ʪ�ȿ�Ƭ�¶ȴ����� */
//////////////#define SMS_Sensor_RF_Card_Humiture_Temp_String	"��ʪ�ȿ�Ƭ�¶ȴ�����"
///////////////* ��ʪ�ȿ�Ƭʪ�ȴ����� */
//////////////#define SMS_Sensor_RF_Card_Humiture_Humi_String	"��ʪ�ȿ�Ƭʪ�ȴ�����"
///////////////* ����������״̬ */
//////////////#define SMS_Sensor_Alarm_State_String	"����״̬:%s;\r\n"
///////////////* ���������� */
//////////////#define SMS_Sensor_Alarm_String			"����"
///////////////* ������δ���� */
//////////////#define SMS_Sensor_Normal_String		"δ����"
///////////////* �������������� */
//////////////#define SMS_Sensor_Alarm_Type_String	"ͨ��%01d��������:%s;\r\n"
///////////////* ���������ޱ��� */
//////////////#define SMS_Sensor_Alarm_Up_String		"���ޱ���"
///////////////* ���������ޱ��� */
//////////////#define SMS_Sensor_Alarm_Down_String	"���ޱ���"
///////////////* ��Ƭ���������ճ�ʱ�������� */
//////////////#define SMS_RF_Card_Data_Rece_Timeout_Alarm_String	"��������:���ݽ��ճ�ʱ;\r\n"
///////////////* ���籨����Ϣ */
//////////////#define SMS_Power_Alarm_Type_String		"�����¼�:��Դδ��."
///////////////* ��ص����ͱ�����Ϣ */
//////////////#define SMS_Battery_Alarm_Type_String	"�����¼�:��ص�����."







/* �ȴ����͵Ķ����������� */
char SMS_UTF8[SMS_UTF8_Data_Buffer_Size];

static void SMS_Set_CH_Inf		(u8 CH_num,char* sms_string,INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f);
static void SMS_Set_Power_Inf	(char* sms_string,INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
static void SMS_Set_Battery_Inf	(char* sms_string,INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
	
static void SMS_Set_Sensor_Alarm_Inf	(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,u8 alarm_ch_no);
static void SMS_Set_Power_Alarm_Inf		(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
static void SMS_Set_Battery_Alarm_Inf	(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);

/**********************************************************************************************************/

/* �����ظ�����ʱ���� ����ָ�� */
static u32 *SMS_Repeat_Alarm_Interval_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval;
/* ʵʱ���ݴ洢���� ���ݴ�С */
#define SMS_Repeat_Alarm_Interval_Data_Size (sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval))




/*
*********************************************************************************************************
*	�� �� ��: SMS_Set_Power_Inf
*	����˵��: ���ݵ�Դ�ı���״̬��Ϣ���ö�������
*	��    �Σ�UTF8_p:UTF-8����洢�ռ�ָ�룻String_p:�ַ�������ָ�룻�ַ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SMS_Set_Power_Inf(char* sms_string,INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* ���õ�Դ������Ϣ */
	sprintf(&sms_string[strlen(sms_string)],SMS_Power_Alarm_Type_String);
}

/*
*********************************************************************************************************
*	�� �� ��: SMS_Set_Battery_Inf
*	����˵��: ���ݵ�ص����ı���״̬��Ϣ���ö�������
*	��    �Σ�UTF8_p:UTF-8����洢�ռ�ָ�룻String_p:�ַ�������ָ�룻�ַ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SMS_Set_Battery_Inf(char* sms_string,INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* ���õ�Դ������Ϣ */
	sprintf(&sms_string[strlen(sms_string)],SMS_Battery_Alarm_Type_String);
}

/************************************************************************************************/
/* ������	: SMS_Set_CH_Inf															 	 	*/
/* ����  	: sms_string:�ַ���Ϣ��sensor_t:���������ñ���Ϣ��sensor_f:���������ñ���Ϣ				*/
/* ���		: �� 																				*/
/* ����	 	: ����ÿ��ͨ����������Ϣ��״̬���ö�������						 						*/
/* ��������	: 2015/04/20																		*/
/************************************************************************************************/
static void SMS_Set_CH_Inf(u8 CH_num,char* sms_string,INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	/* �жϴ����� ͨ��һ �Ƿ����ù� */
	if(sensor_t->Type != 0)
	{
		/* ͨ��һ Ϊ������ͨ�� */
		/* ����ͨ��������Ϣ */
		/* ������ͨ���� *///����ͨ����� ��1 ��ԭ����ͨ��1��ֵΪ0����֤����Ľ�������õ�һ�¡�
		sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_NO_String,(CH_num+1));
		/* ������ͨ������ */
		if(sensor_t->Type == Sensor_Tmp)
		{
			/* �����¶ȴ����� */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Temp_String);
		}else if(sensor_t->Type == Sensor_Hum)
		{
			/* ����ʪ�ȴ����� */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Humi_String);
		}else if(sensor_t->Type == Sensor_RF_Card_Single_Tmp)
		{
			/* ���¿�Ƭ������ */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_RF_Card_Single_Temp_String);
		}else if(sensor_t->Type == Sensor_RF_Card_Humiture_Tmp)
		{
			/* ��ʪ�ȿ�Ƭ�¶ȴ����� */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_RF_Card_Humiture_Temp_String);		
		}else if(sensor_t->Type == Sensor_RF_Card_Humiture_Hum)
		{
			/* ��ʪ�ȿ�Ƭʪ�ȴ����� */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_RF_Card_Humiture_Humi_String);	
		}else if(sensor_t->Type == Sensor_PT1000)
		{
			/* PT1000�¶ȴ����� */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Temp_String);
		}else if(sensor_t->Type == Sensor_GW_NJSY_Tmp	)
		{
			/* �Ͼ�ʢ�������¶�̽ͷ���� */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Temp_String);//�¶ȴ�����
		}else if(sensor_t->Type == Sensor_GW_NJSY_Hum	)
		{
			/* �Ͼ�ʢ������ʪ��̽ͷ���� */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Humi_String);//ʢ��ʪ�ȴ�����
		}
//		/* ����������״̬ */
//		sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Alarm_State_String,
//		(sensor_t->SENSOR_Alarm_State == Alarm_Null) ? SMS_Sensor_Normal_String : SMS_Sensor_Alarm_String);
		/* �������������� */
		if(sensor_t->SENSOR_Alarm_State != Alarm_Null)
		{
			/* ����ͨ��������Ϣ */
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x01)
			/* ���� ����RF��Ƭ���ݽ��ճ�ʱ���� */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_RF_Card_Data_Rece_Timeout_Alarm_String);
			}else			
#endif
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Alarm_Type_String,(CH_num+1),
				(sensor_t->SENSOR_Alarm_State == Alarm_Up) ? SMS_Sensor_Alarm_Up_String : SMS_Sensor_Alarm_Down_String);
			}

		}
		/* �¶ȴ�����������ʾ */
		if(sensor_t->Type == Sensor_Tmp)
		{
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
			((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
			(((u16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
			((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Up_Limit),
			((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
			((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Low_Limit),
			((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
		}else if(sensor_t->Type == Sensor_Hum)/* ʪ�ȴ�����������ʾ */
		{
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Humi_Para_String,
			(((u16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
			((u16) sensor_t->SENSOR_Up_Limit),
			((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
			((u16) sensor_t->SENSOR_Low_Limit),
			((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
		}
		else if(sensor_t->Type == Sensor_RF_Card_Single_Tmp)/* ���¿�Ƭ������ʾ */
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x01)
			/* ���� ����RF��Ƭ���ݽ��ճ�ʱ���� */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_RF_Card_Data_Rece_Timeout_Temp_Para_String,
				((sensor_f->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((sensor_f->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));	
			}else			
#endif
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
				((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
				(((u16)sensor_f->Sensor_Real_Time_Data)),
				((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
				((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
			}
		}
		else if(sensor_t->Type == Sensor_RF_Card_Humiture_Tmp)/* ��ʪ��Ƭ ���¶����� ������ʾ */
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x01)
			/* ���� ����RF��Ƭ���ݽ��ճ�ʱ���� */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_RF_Card_Data_Rece_Timeout_Temp_Para_String,
				((sensor_f->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((sensor_f->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));	
			}else			
#endif
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
				((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
				(((u16)sensor_f->Sensor_Real_Time_Data)),
				((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
				((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));		
			}
			
		}
		else if(sensor_t->Type == Sensor_RF_Card_Humiture_Hum)/* ��ʪ��Ƭ ��ʪ������ ������ʾ */
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x01)
			/* ���� ����RF��Ƭ���ݽ��ճ�ʱ���� */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_RF_Card_Data_Rece_Timeout_Humi_Para_String,
				((s16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((s16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));	
			}else			
#endif
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Humi_Para_String,
			(((s16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
				((s16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((s16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));		
			}
		}else if(sensor_t->Type == Sensor_PT1000)/* PT1000�¶ȴ�����������ʾ */
		{
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
			((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
			(((u16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
			((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Up_Limit),
			((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
			((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Low_Limit),
			((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
			
			
			
		}else if(sensor_t->Type == Sensor_GW_NJSY_Tmp)/* �Ͼ�ʢ�������¶�̽ͷ���� */
		{	
			
			
		
			
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
			((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
			(((u16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
			((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Up_Limit),
			((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
			((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
			((u16)sensor_t->SENSOR_Low_Limit),
			((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
			
			
		
			

			
		}else if(sensor_t->Type == Sensor_GW_NJSY_Hum)/* �Ͼ�ʢ������ʪ��̽ͷ���� */
		{
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Humi_Para_String,
			(((s16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
				((s16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((s16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));		
		}

	}
	
}

/*
*********************************************************************************************************
*	�� �� ��: SMS_Set_Sensor_Alarm_Inf
*	����˵��: ���ô�����������������
*	��    �Σ�config_tab:������Ϣ��;fix_tab:����������;
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SMS_Set_Sensor_Alarm_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,u8 alarm_ch_no)
{
	
	/* ����Ĵ������� */
	memset(SMS_UTF8,0X00,sizeof(SMS_UTF8));
	
	/* ���÷��Ͷ������� */
	/* ��ȡ��ǰ����ʱ��������ʱ������ */
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_Alarm_Time_String,
	config_tab->Inf_State.Instru_RTC.Inf_State.Year,
	config_tab->Inf_State.Instru_RTC.Inf_State.Month,
	config_tab->Inf_State.Instru_RTC.Inf_State.Day,
	config_tab->Inf_State.Instru_RTC.Inf_State.Hour,
	config_tab->Inf_State.Instru_RTC.Inf_State.Min,
	config_tab->Inf_State.Instru_RTC.Inf_State.Sec
	);
	
	switch(alarm_ch_no)
	{	
#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_Hum)||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000))
		case Sensors_CH_1:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_1,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH1,&fix_tab->Inf_State.Instru_Sensor.Inf.CH1);	
			break;
		}
#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum))
		case Sensors_CH_1:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_1,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH1,&fix_tab->Inf_State.Instru_Sensor.Inf.CH1);	
			break;
		}
		
#endif
		
#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum)||(Sensor_CH2_Config_Type == Sensor_PT1000))
		case Sensors_CH_2:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_2,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH2,&fix_tab->Inf_State.Instru_Sensor.Inf.CH2);	
			break;
		}
#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum))
		case Sensors_CH_2:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_2,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH2,&fix_tab->Inf_State.Instru_Sensor.Inf.CH2);
			break;
		}
#endif	
		
#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum)||(Sensor_CH3_Config_Type == Sensor_PT1000))
		case Sensors_CH_3:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_3,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH3,&fix_tab->Inf_State.Instru_Sensor.Inf.CH3);	
			break;
		}
#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_3:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_3,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH3,&fix_tab->Inf_State.Instru_Sensor.Inf.CH3);
			break;
		}
#endif			
		
#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||(Sensor_CH4_Config_Type == Sensor_Hum)||(Sensor_CH4_Config_Type == Sensor_PT1000))
		case Sensors_CH_4:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_4,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH4,&fix_tab->Inf_State.Instru_Sensor.Inf.CH4);	
			break;
		}
#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum))
				
		case Sensors_CH_4:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_4,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH4,&fix_tab->Inf_State.Instru_Sensor.Inf.CH4);
			break;
		}
#endif		
		
#if ((Sensor_CH5_Config_Type == Sensor_Tmp)||(Sensor_CH5_Config_Type == Sensor_Hum)||(Sensor_CH5_Config_Type == Sensor_PT1000))
		case Sensors_CH_5:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_5,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH5,&fix_tab->Inf_State.Instru_Sensor.Inf.CH5);	
			break;
		}
#elif ((Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_5:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_5,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH5,&fix_tab->Inf_State.Instru_Sensor.Inf.CH5);
			break;
		}
#endif	
		
#if ((Sensor_CH6_Config_Type == Sensor_Tmp)||(Sensor_CH6_Config_Type == Sensor_Hum)||(Sensor_CH6_Config_Type == Sensor_PT1000))
		case Sensors_CH_6:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_6,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH6,&fix_tab->Inf_State.Instru_Sensor.Inf.CH6);	
			break;
		}
#elif ((Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_6:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_6,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH6,&fix_tab->Inf_State.Instru_Sensor.Inf.CH6);
			break;
		}
#endif	
		
#if ((Sensor_CH7_Config_Type == Sensor_Tmp)||(Sensor_CH7_Config_Type == Sensor_Hum)||(Sensor_CH7_Config_Type == Sensor_PT1000)|| (Sensor_CH7_Config_Type == Sensor_DoorContact))
		case Sensors_CH_7:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_7,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH7,&fix_tab->Inf_State.Instru_Sensor.Inf.CH7);	
			break;
		}
#elif ((Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum))
				
		case Sensors_CH_7:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_7,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH7,&fix_tab->Inf_State.Instru_Sensor.Inf.CH7);	
			break;
		}
#endif	
		
#if ((Sensor_CH8_Config_Type == Sensor_Tmp)||(Sensor_CH8_Config_Type == Sensor_Hum)||(Sensor_CH8_Config_Type == Sensor_PT1000))
		case Sensors_CH_8:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_8,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH8,&fix_tab->Inf_State.Instru_Sensor.Inf.CH8);	
			break;
		}
#elif ((Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_8:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_8,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH8,&fix_tab->Inf_State.Instru_Sensor.Inf.CH8);
			break;
		}
#endif	

#if ((Sensor_CH9_Config_Type == Sensor_Tmp)||(Sensor_CH9_Config_Type == Sensor_Hum)||(Sensor_CH9_Config_Type == Sensor_PT1000))
		case Sensors_CH_9:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_9,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH9,&fix_tab->Inf_State.Instru_Sensor.Inf.CH9);	
			break;
		}
#elif ((Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_9:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_9,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH9,&fix_tab->Inf_State.Instru_Sensor.Inf.CH9);
			break;
		}
#endif		
		
#if ((Sensor_CH10_Config_Type == Sensor_Tmp)||(Sensor_CH10_Config_Type == Sensor_Hum)||(Sensor_CH10_Config_Type == Sensor_PT1000))
		case Sensors_CH_10:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_10,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH10,&fix_tab->Inf_State.Instru_Sensor.Inf.CH10);	
			break;
		}
#elif ((Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_10:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_10,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH10,&fix_tab->Inf_State.Instru_Sensor.Inf.CH10);
			break;
		}
#endif	
		
#if ((Sensor_CH11_Config_Type == Sensor_Tmp)||(Sensor_CH11_Config_Type == Sensor_Hum)||(Sensor_CH11_Config_Type == Sensor_PT1000))
		case Sensors_CH_11:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_11,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH11,&fix_tab->Inf_State.Instru_Sensor.Inf.CH11);	
			break;
		}
#elif ((Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_11:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_11,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH11,&fix_tab->Inf_State.Instru_Sensor.Inf.CH11);
			break;
		}
#endif	
		
#if ((Sensor_CH12_Config_Type == Sensor_Tmp)||(Sensor_CH12_Config_Type == Sensor_Hum)||(Sensor_CH12_Config_Type == Sensor_PT1000))
		case Sensors_CH_12:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_12,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH12,&fix_tab->Inf_State.Instru_Sensor.Inf.CH12);	
			break;
		}
#elif ((Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_12:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_12,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH12,&fix_tab->Inf_State.Instru_Sensor.Inf.CH12);
			break;
		}
#endif		
		
#if ((Sensor_CH13_Config_Type == Sensor_Tmp)||(Sensor_CH13_Config_Type == Sensor_Hum)||(Sensor_CH13_Config_Type == Sensor_PT1000))
		case Sensors_CH_13:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_13,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH13,&fix_tab->Inf_State.Instru_Sensor.Inf.CH13);	
			break;
		}
#elif ((Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_13:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_13,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH13,&fix_tab->Inf_State.Instru_Sensor.Inf.CH13);
			break;
		}
#endif		
		
#if ((Sensor_CH14_Config_Type == Sensor_Tmp)||(Sensor_CH14_Config_Type == Sensor_Hum)||(Sensor_CH14_Config_Type == Sensor_PT1000))
		case Sensors_CH_14:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_14,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH14,&fix_tab->Inf_State.Instru_Sensor.Inf.CH14);	
			break;
		}
#elif ((Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_14:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_14,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH14,&fix_tab->Inf_State.Instru_Sensor.Inf.CH14);
			break;
		}
#endif	
		
#if ((Sensor_CH15_Config_Type == Sensor_Tmp)||(Sensor_CH15_Config_Type == Sensor_Hum)||(Sensor_CH15_Config_Type == Sensor_PT1000))
		case Sensors_CH_15:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_15,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH15,&fix_tab->Inf_State.Instru_Sensor.Inf.CH15);	
			break;
		}
#elif ((Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_15:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_15,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH15,&fix_tab->Inf_State.Instru_Sensor.Inf.CH15);
			break;
		}
#endif	
		
#if ((Sensor_CH16_Config_Type == Sensor_Tmp)||(Sensor_CH16_Config_Type == Sensor_Hum)||(Sensor_CH16_Config_Type == Sensor_PT1000))
		case Sensors_CH_16:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_16,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH16,&fix_tab->Inf_State.Instru_Sensor.Inf.CH16);	
			break;
		}
#elif ((Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_16:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_16,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH16,&fix_tab->Inf_State.Instru_Sensor.Inf.CH16);
			break;
		}
#endif
		
#if ((Sensor_CH17_Config_Type == Sensor_Tmp)||(Sensor_CH17_Config_Type == Sensor_Hum)||(Sensor_CH17_Config_Type == Sensor_PT1000))
		case Sensors_CH_17:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_17,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH17,&fix_tab->Inf_State.Instru_Sensor.Inf.CH17);	
			break;
		}
#elif ((Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_17:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_17,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH17,&fix_tab->Inf_State.Instru_Sensor.Inf.CH17);
			break;
		}
#endif		
		
#if ((Sensor_CH18_Config_Type == Sensor_Tmp)||(Sensor_CH18_Config_Type == Sensor_Hum)||(Sensor_CH18_Config_Type == Sensor_PT1000))
		case Sensors_CH_18:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_18,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH18,&fix_tab->Inf_State.Instru_Sensor.Inf.CH18);	
			break;
		}
#elif ((Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_18:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_18,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH18,&fix_tab->Inf_State.Instru_Sensor.Inf.CH18);
			break;
		}
#endif	
		
#if ((Sensor_CH19_Config_Type == Sensor_Tmp)||(Sensor_CH19_Config_Type == Sensor_Hum)||(Sensor_CH19_Config_Type == Sensor_PT1000))
		case Sensors_CH_19:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_19,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH19,&fix_tab->Inf_State.Instru_Sensor.Inf.CH19);	
			break;
		}
#elif ((Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum))
		 	
		case Sensors_CH_19:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_19,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH19,&fix_tab->Inf_State.Instru_Sensor.Inf.CH19);
			break;
		}
#endif	
		
#if ((Sensor_CH20_Config_Type == Sensor_Tmp)||(Sensor_CH20_Config_Type == Sensor_Hum)||(Sensor_CH20_Config_Type == Sensor_PT1000))
		case Sensors_CH_20:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_20,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH20,&fix_tab->Inf_State.Instru_Sensor.Inf.CH20);	
			break;
		}
#elif ((Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_20:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_20,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH20,&fix_tab->Inf_State.Instru_Sensor.Inf.CH20);
			break;
		}
#endif		
		
#if ((Sensor_CH21_Config_Type == Sensor_Tmp)||(Sensor_CH21_Config_Type == Sensor_Hum)||(Sensor_CH21_Config_Type == Sensor_PT1000))
		case Sensors_CH_21:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_21,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH21,&fix_tab->Inf_State.Instru_Sensor.Inf.CH21);	
			break;
		}
#elif ((Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_21:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_21,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH21,&fix_tab->Inf_State.Instru_Sensor.Inf.CH21);
			break;
		}
#endif		
		
#if ((Sensor_CH22_Config_Type == Sensor_Tmp)||(Sensor_CH22_Config_Type == Sensor_Hum)||(Sensor_CH22_Config_Type == Sensor_PT1000))
		case Sensors_CH_22:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_22,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH22,&fix_tab->Inf_State.Instru_Sensor.Inf.CH22);	
			break;
		}
#elif ((Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_22:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_22,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH22,&fix_tab->Inf_State.Instru_Sensor.Inf.CH22);
			break;
		}
#endif	
		
#if ((Sensor_CH23_Config_Type == Sensor_Tmp)||(Sensor_CH23_Config_Type == Sensor_Hum)||(Sensor_CH23_Config_Type == Sensor_PT1000))
		case Sensors_CH_23:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_23,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH23,&fix_tab->Inf_State.Instru_Sensor.Inf.CH23);	
			break;
		}
#elif ((Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_23:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_23,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH23,&fix_tab->Inf_State.Instru_Sensor.Inf.CH23);
			break;
		}
#endif	
		
#if ((Sensor_CH24_Config_Type == Sensor_Tmp)||(Sensor_CH24_Config_Type == Sensor_Hum)||(Sensor_CH24_Config_Type == Sensor_PT1000))
		case Sensors_CH_24:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_24,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH24,&fix_tab->Inf_State.Instru_Sensor.Inf.CH24);	
			break;
		}
#elif ((Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_24:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_24,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH24,&fix_tab->Inf_State.Instru_Sensor.Inf.CH24);
			break;
		}
#endif		
		
#if ((Sensor_CH25_Config_Type == Sensor_Tmp)||(Sensor_CH25_Config_Type == Sensor_Hum)||(Sensor_CH25_Config_Type == Sensor_PT1000))
		case Sensors_CH_25:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_25,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH25,&fix_tab->Inf_State.Instru_Sensor.Inf.CH25);	
			break;
		}
#elif ((Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_25:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_25,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH25,&fix_tab->Inf_State.Instru_Sensor.Inf.CH25);
			break;
		}
#endif		
		
#if ((Sensor_CH26_Config_Type == Sensor_Tmp)||(Sensor_CH26_Config_Type == Sensor_Hum)||(Sensor_CH26_Config_Type == Sensor_PT1000))
		case Sensors_CH_26:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_26,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH26,&fix_tab->Inf_State.Instru_Sensor.Inf.CH26);	
			break;
		}
#elif ((Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_26:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_26,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH26,&fix_tab->Inf_State.Instru_Sensor.Inf.CH26);
			break;
		}
#endif	
		
#if ((Sensor_CH27_Config_Type == Sensor_Tmp)||(Sensor_CH27_Config_Type == Sensor_Hum)||(Sensor_CH27_Config_Type == Sensor_PT1000))
		case Sensors_CH_27:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_27,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH27,&fix_tab->Inf_State.Instru_Sensor.Inf.CH27);	
			break;
		}
#elif ((Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_27:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_27,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH27,&fix_tab->Inf_State.Instru_Sensor.Inf.CH27);
			break;
		}
#endif	
		
#if ((Sensor_CH28_Config_Type == Sensor_Tmp)||(Sensor_CH28_Config_Type == Sensor_Hum)||(Sensor_CH28_Config_Type == Sensor_PT1000))
		case Sensors_CH_28:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_28,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH28,&fix_tab->Inf_State.Instru_Sensor.Inf.CH28);	
			break;
		}
#elif ((Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_28:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_28,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH28,&fix_tab->Inf_State.Instru_Sensor.Inf.CH28);
			break;
		}
#endif		
		
#if ((Sensor_CH29_Config_Type == Sensor_Tmp)||(Sensor_CH29_Config_Type == Sensor_Hum)||(Sensor_CH29_Config_Type == Sensor_PT1000))
		case Sensors_CH_29:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_29,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH29,&fix_tab->Inf_State.Instru_Sensor.Inf.CH29);	
			break;
		}
#elif ((Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_29:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_29,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH29,&fix_tab->Inf_State.Instru_Sensor.Inf.CH29);
			break;
		}
#endif		
		
#if ((Sensor_CH30_Config_Type == Sensor_Tmp)||(Sensor_CH30_Config_Type == Sensor_Hum)||(Sensor_CH30_Config_Type == Sensor_PT1000))
		case Sensors_CH_30:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_30,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH30,&fix_tab->Inf_State.Instru_Sensor.Inf.CH30);	
			break;
		}
#elif ((Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_30:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_30,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH30,&fix_tab->Inf_State.Instru_Sensor.Inf.CH30);
			break;
		}
#endif	
		
#if ((Sensor_CH31_Config_Type == Sensor_Tmp)||(Sensor_CH31_Config_Type == Sensor_Hum)||(Sensor_CH31_Config_Type == Sensor_PT1000))
		case Sensors_CH_31:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_31,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH31,&fix_tab->Inf_State.Instru_Sensor.Inf.CH31);	
			break;
		}
#elif ((Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_31:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_31,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH31,&fix_tab->Inf_State.Instru_Sensor.Inf.CH31);
			break;
		}
#endif	
		
#if ((Sensor_CH32_Config_Type == Sensor_Tmp)||(Sensor_CH32_Config_Type == Sensor_Hum)||(Sensor_CH32_Config_Type == Sensor_PT1000))
		case Sensors_CH_32:
		{	
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_32,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH32,&fix_tab->Inf_State.Instru_Sensor.Inf.CH32);	
			break;
		}
#elif ((Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_32:
		{
			/* ��ȡ��ȡ������ */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[3]));
			/* ���ö��ű������� */
			SMS_Set_CH_Inf(Sensors_CH_32,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH32,&fix_tab->Inf_State.Instru_Sensor.Inf.CH32);
			break;
		}
#endif		
		
		
		default:
		{

			break;
		}
	}
	
}

/*
*********************************************************************************************************
*	�� �� ��: SMS_Send_Sensor_Inf
*	����˵��: ���ʹ������������ű���
*	��    �Σ�State��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
BOOL SMS_Send_Sensor_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,u8 alarm_ch_no)
{
	/* ������Ϣ����״̬ */
	BOOL sms_state;
	
	
	/* ���Ͷ��� */
	sms_state = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Send_SMS_EVENT);
	/* �ж϶��ŷ��Ͷ���״̬ *///ע�⣺�����״̬��������ȼ�Ҫ��GSM����ߣ���������ɶ��к��ִ��GSM�����ˣ���������û�б����á�
	if(sms_state == TRUE)
	{
		/* ���ͳɹ� */
		/* ���÷��Ͷ������� */
		SMS_Set_Sensor_Alarm_Inf(config_tab,fix_tab,alarm_ch_no);
		/* ���ö��ŷ���״̬ */
		GSM_SMS_SR_Reg.GSM_SMS_Send_State = TRUE;
	}
	/* ���� ������Ϣ ������� */
	return sms_state;
}

/*
*********************************************************************************************************
*	�� �� ��: SMS_Set_Power_Alarm_Inf
*	����˵��: ���õ��籨����������
*	��    �Σ�config_tab:������Ϣ��;fix_tab:����������;
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SMS_Set_Power_Alarm_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	
	/* ����Ĵ������� */
	memset(SMS_UTF8,0X00,sizeof(SMS_UTF8));
	
	/* ���÷��Ͷ������� */
	/* ��ȡ��ǰ����ʱ��������ʱ������ */
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_Alarm_Time_String,
	config_tab->Inf_State.Instru_RTC.Inf_State.Year,
	config_tab->Inf_State.Instru_RTC.Inf_State.Month,
	config_tab->Inf_State.Instru_RTC.Inf_State.Day,
	config_tab->Inf_State.Instru_RTC.Inf_State.Hour,
	config_tab->Inf_State.Instru_RTC.Inf_State.Min,
	config_tab->Inf_State.Instru_RTC.Inf_State.Sec
	);

	/* ��ȡ��ȡ������ *///"������:%X%X%X%X%05d;\r\n"
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) | \
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) | \
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));

	/* ���ݵ�Դ�ı���״̬��Ϣ���ö������� */
	SMS_Set_Power_Inf(&SMS_UTF8[strlen(SMS_UTF8)],&config_tab->Inf_State.Instru_Power,&fix_tab->Inf_State.Instru_Power);
	

}

/*
*********************************************************************************************************
*	�� �� ��: SMS_Set_Battery_Alarm_Inf
*	����˵��: ���õ�ص����ͱ�����������
*	��    �Σ�config_tab:������Ϣ��;fix_tab:����������;
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SMS_Set_Battery_Alarm_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{

	
	/* ����Ĵ������� */
	memset(SMS_UTF8,0X00,sizeof(SMS_UTF8));
	
	/* ���÷��Ͷ������� */
	/* ��ȡ��ǰ����ʱ��������ʱ������ */
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_Alarm_Time_String,
	config_tab->Inf_State.Instru_RTC.Inf_State.Year,
	config_tab->Inf_State.Instru_RTC.Inf_State.Month,
	config_tab->Inf_State.Instru_RTC.Inf_State.Day,
	config_tab->Inf_State.Instru_RTC.Inf_State.Hour,
	config_tab->Inf_State.Instru_RTC.Inf_State.Min,
	config_tab->Inf_State.Instru_RTC.Inf_State.Sec
	);
   
	/* ��ȡ��ȡ������ *///"������:%X%X%X%X%05d;\r\n"
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) | \
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) | \
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));

	/* ���ݵ�Դ�ı���״̬��Ϣ���ö������� */
	SMS_Set_Battery_Inf(&SMS_UTF8[strlen(SMS_UTF8)],&config_tab->Inf_State.Instru_Power,&fix_tab->Inf_State.Instru_Power);

}

/*
*********************************************************************************************************
*	�� �� ��: SMS_Send_Power_Inf
*	����˵��: ���͵�����ű���
*	��    �Σ�State��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
BOOL SMS_Send_Power_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* ������Ϣ����״̬ */
	BOOL sms_state;
	
	/* ���Ͷ��� */
	sms_state = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Send_SMS_EVENT);
	/* �ж϶��ŷ��Ͷ���״̬ *///ע�⣺�����״̬��������ȼ�Ҫ��GSM����ߣ���������ɶ��к��ִ��GSM�����ˣ���������û�б����á�
	if(sms_state == TRUE)
	{
		/* ���ͳɹ� */
		/* ���÷��Ͷ������� */
		SMS_Set_Power_Alarm_Inf(config_tab,fix_tab);
		/* ���ö��ŷ���״̬ */
		GSM_SMS_SR_Reg.GSM_SMS_Send_State = TRUE;
	}
	/* ���� ������Ϣ ������� */
	return sms_state;
}

/*
*********************************************************************************************************
*	�� �� ��: SMS_Send_Power_Inf
*	����˵��: ���͵�����ű���
*	��    �Σ�State��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
BOOL SMS_Send_Battery_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* ������Ϣ����״̬ */
	BOOL sms_state;
	

	/* ���Ͷ��� */
	sms_state = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Send_SMS_EVENT);
	/* �ж϶��ŷ��Ͷ���״̬ *///ע�⣺�����״̬��������ȼ�Ҫ��GSM����ߣ���������ɶ��к��ִ��GSM�����ˣ���������û�б����á�
	if(sms_state == TRUE)
	{
		/* ���ͳɹ� */
		/* ���÷��Ͷ������� */
		SMS_Set_Battery_Alarm_Inf(config_tab,fix_tab);
		/* ���ö��ŷ���״̬ */
		GSM_SMS_SR_Reg.GSM_SMS_Send_State = TRUE;
	}
	/* ���� ������Ϣ ������� */
	return sms_state;
}

/************************************************************************************************/
/* ������	: SMS_Again_Send_State_Check														*/
/* ����		: send_time:ǰ�η��Ͷ��ż�¼��ϵͳʱ��ʱ��;	alarm_state:����״̬							*/
/* ���		: ��																				*/
/* ����		: ���´���������		 									 							*/
/* ��������	: 2015/04/20																		*/
/* �޸�����	: 2015/08/28																		*/
/************************************************************************************************/
BOOL SMS_Again_Send_State_Check(u32 system_time,u32* send_time,u8 alarm_state)
{


	/* �жϱ���״̬ */
	if((alarm_state & 0x04) == 0X00)
	{
		/* û�ж��ű���,��¼��ǰʱ��. */
		*send_time = system_time;
		/* �����·��Ͷ��š� */
		return TRUE;
	}else
	{
		/* ���ű��� */

		if(*SMS_Repeat_Alarm_Interval_P != 0)
		{
			/* �ж�ǰ�η��͵ı���ʱ�䵽�����Ƿ񳬹��ط�ʱ�� */
			//���� �����ظ�����ʱ���� �Ĵ��� ����������ݣ���ϵͳʱ���õ��Ǻ��뵥λ
			//�������İ�SMS_Repeat_Alarm_Interval_P*1000ת���ɺ��롣
			if((system_time - *send_time) > (*SMS_Repeat_Alarm_Interval_P*1000))
			{
				/* ���ڶ����ط�ʱ�䡣 */
				/* ��¼��ǰʱ��Ϊ����ʱ�� */
				*send_time = system_time;
				/* ���·��Ͷ��š� */
				return FALSE;
			}else
			{
				/* С�ڶ����ط�ʱ�䡣 */
				/* �����·��Ͷ��š� */
				return TRUE;
			}
		}else
		{
			/* �����ظ�����ʱ��������0��ʾ�������ظ����š� */
			/* ��¼��ǰʱ��. */
			*send_time = system_time;
			/* �����·��Ͷ��š� */
			return TRUE;
		}

	}

}

/******************* (C) ZHOU *****END OF FILE****/


