/**
  ******************************************************************************
  * @file              : Log_COM.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015��3��12�� 09:55
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
  
#include "BSP.h" 
 
#include "Log_COM.h"

#include "Log_Download.h"
#include "Log_GSM.h"
#include "Log_Sensor.h"

#include "Dev_Fram.h"

#include "Mcu_Config.h"
#include "Mcu_Usart.h"

#include <string.h>
#include "app.h"
#include "Mcu_Beep.h"

#include "GSM_app.h"

#include "String_Utility.h"

#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	
#include "Log_GSM_Config.h"
#endif

/************************GSMģ���ź�ǿ�Ƚṹ��*****************ZWC********/
extern  GSM_GPRS_SR_Struct GPRS_SR_Reg;
extern void DispMenuDeskTopDown(void);
//static void Com_Config_SMSAamPoeNOSet (void);
/************************************************************************************************/
extern void Instru_Rte_Fty_Set(void);
extern void Parameter_Setting_Successful(void);

/* ��ӡ���Ա�־ *///ZWC
extern BOOL GSM_DEBUG_Flag;// TRUE




/**********************************���ڿ������ݼ�¼��������ֹͣ*********************************/

extern void App_Save_Config(FunctionalState New_state);
/**********************************���ڶ�ȡ���ݼ�¼��״̬***************************************/
/* ʵʱ���ݴ洢���� ����ָ�� SaveDataZWC */
static FunctionalState *Save_Data_Switch_A = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch;

/********************************ͨ�����ڵ������״̬�͵�ص���************************************/
extern u8 Power_ET_Alarm_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);

/*************************RTCʱ������******************************************/
extern u8 HEX_To_BCD(u8 hex_data);    //HEXתΪBCD�ӳ���   
extern u8 BCD_To_HEX(u8 hex_data);    //BCDתΪHEX�ӳ��� 

extern u8 HEX_Hi4(u8 Data);//ȡ��һ���ֽ����ݵĸ�4λ
extern u8 HEX_Lo4(u8 Data);//ȡ��һ���ֽ����ݵĵ�4λ

#define System_Year_Start		(2000u)//��������ʱ�䣻��ݿ�ʼ��ʼʱ��



/* ������У׼ͨ�����ݾ�֡ ���� �Ĵ��� ���� */
#define Com_Sensor_Cal_RX_Reg_Size			(COM_Sensor_Cal_EVENT_Q_Len)
/* ������У׼ͨ�����ݾ�֡�洢�Ĵ��� */
COM_SENSOR_CAL_TYPEDEF	Com_Sensor_Cal_RX_Reg[Com_Sensor_Cal_RX_Reg_Size];

/************************************************************************************************/
//ԭʼ�汾�Ĵ������ṹ
//�������ù�����ʱû���˽��ָ��£��������ֻ����������һ�´���
/* ������ ͨ��ͨ�����ݽṹ */
typedef struct {
	/* ������ ���� */						//	0:û���趨�Ĵ���������	1:�¶ȴ�����		2:ʪ�ȴ�����		3:���¿�Ƭ	4����ʪ��Ƭ	����:��Ч
	u32				Type						:8;
	/* ������ �������� */						//	1:������			0:������
	u32				SENSOR_Alarm_Switch			:1;	
	/* ������ ������������ */					//	1:������			0:������
	u32				SENSOR_Sound_Alarm_Switch	:1;
	/* ������ �ⱨ������ */					//	1:������			0:������
	u32				SENSOR_Light_Alarm_Switch	:1;
	/* ������ ���ű������� */					//	1:������			0:������
	u32				SENSOR_SMS_Alarm_Switch		:1;
	/* ������ ���ر��� */						//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
	u32				SENSOR_Alarm_Switch_Reserve	:4;	
	/* ������ ����״̬ */						//	00:������δ����	01:����������
	u32				SENSOR_Alarm_State			:2;
	/* ������ ����״̬���� */					//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
	u32 			SENSOR_Alarm_State_Reserve	:6;	
	/* ������ ��Ϣ �����ռ� */				//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
	u32				SENSOR_State_Inf_Reserve	:8;
	/* ��ƬID�� */							//	0:δ���ã�Ĭ�����ã�
	u32				SENSOR_RF_Card_ID_NO;
	/* ������ SN�� */						//	0:δ���ã�Ĭ�����ã�
	u8				SENSOR_SN_NO[SENSOR_SN_NO_Size];
	/* ������ �������� */						//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
	float			SENSOR_Up_Limit;
	/* ������ �������� */						//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
	float			SENSOR_Low_Limit;
	
}INSTRU_SENSOR_Old;

/************************************************************************************************/

/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Send_Q																*/
/**	����˵��: ���ʹ�����У׼������Ϣ																*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void Com_Sensor_Cal_Send_Q(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	INT8U      	err;
	OS_Q_DATA	com_sensor_cal_q;

	/* ��� ������У׼���� ��Ϣ */
	err = OSQQuery (COM_Sensor_Cal_EVENT_Q,&com_sensor_cal_q);
	/* �жϲ�ѯ���гɹ� */
	if(err == OS_ERR_NONE)
	{
		/* �ж� ��������Ϣ�����Ƿ񳬳����� */
		if(com_sensor_cal_q.OSNMsgs < COM_Sensor_Cal_EVENT_Q_Len)
		{
			/* ���� δ���� */
			/* ����������У׼�������� */
			Com_Sensor_Cal_RX_Reg[com_sensor_cal_q.OSNMsgs] = *Cal_Inf;
			/* ������з�����Ϣ */
			err = OSQPost(COM_Sensor_Cal_EVENT_Q,&Com_Sensor_Cal_RX_Reg[com_sensor_cal_q.OSNMsgs]);
		}else
		{
			/* ���� ���� */
			/* ��������Ϣ */
			__nop();
		}	
	}
}

/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Get_Check															*/
/**	����˵��: ���㴫����ͨ��֡��У��																*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
u8 Com_Sensor_Cal_Get_Check(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	u8 check = 0;
	u8 num;
	
	/* ���㴫����ͨ��֡��У�� */
	check += Cal_Inf->Head_Code;
	check += Cal_Inf->Instru_NO;
	check += Cal_Inf->Cmd_Code;
	check += Cal_Inf->Chanel_Num;
	check += Cal_Inf->Data_Len;
	
	for(num = 0;num < (Cal_Inf->Data_Len+1);num++)
	{
		check += Cal_Inf->Com_Data[num];
	}
	
	return check;
}

/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Deleted_Buffer_Data													*/
/**	����˵��: ��� ���ݻ��������																	*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Deleted_Buffer_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* ��� ���ݻ�������� */
	memset(Cal_Inf,0X00,sizeof(COM_SENSOR_CAL_TYPEDEF));
}

/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Deleted_Buffer_Data													*/
/**	����˵��: ��� ���ݻ��������																*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static BOOL Log_Com_Sensor_CH_Check(u8 ch)
{

	/* ��ȡ��Ӧͨ������ʵֵ��У׼ֵ */
	switch(ch)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			return TRUE;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			return TRUE;
		}
#endif	
		
		default:
		{
			/* ��ȡ��ͨ��Ŀ�겻���� */

			return FALSE;
		}
	}

}

#if (1)

#if (GSM_Module_Type==2) //ESP8266Wifiģ��
/************************************************************************************************/
/**	�� �� ��: Com_Config_Read_Write_Wifi_Name	*/
/**	����˵��: ����дWifi����***************/
/**	��    �Σ�������Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_Name(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	/* дWifi���� Ӧ������ݳ��� */
#define Com_Read_Write_Wifi_Name_Data_Len	(1u) //���1����������
/* дWifi���� Ӧ��Э�鳤�� */
//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
#define Com_Read_Write_Wifi_Name_Len		       (Com_Read_Write_Wifi_Name_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* дWifi���Ƶ����ݵ�ַ */
#define  Read_Write_Wifi_Name_Data_P				   	(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name)
COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_Name;//���մ������� 
/*Cal_Inf->Chanel_Num=0x55���������    Read_Write_Wifi_Name.Chanel_Num=0xAA����д����*/
	    if((Cal_Inf->Chanel_Num==0x55)&&(Cal_Inf->Data_Len<=Wifi_Name_Size))//��
			{	
	        /* ���� ʵʱ���� ���� */
					Read_Write_Wifi_Name.Head_Code 	   = Com_Sensor_Cal_Rece_Head_Code;
					Read_Write_Wifi_Name.Instru_NO 	   = Cal_Inf->Instru_NO;//�������
					Read_Write_Wifi_Name.Cmd_Code	     = Com_Read_Write_Wifi_Name_Cmd_Code;
					Read_Write_Wifi_Name.Chanel_Num    = Cal_Inf->Chanel_Num;
					Read_Write_Wifi_Name.Data_Len	     = strlen(Read_Write_Wifi_Name_Data_P);
					/* �����ݿ��������ͼĴ����� */
					memcpy(Read_Write_Wifi_Name.Com_Data,Read_Write_Wifi_Name_Data_P,Read_Write_Wifi_Name.Data_Len);
					/* ���� ���� */
					*((u8*)&(Read_Write_Wifi_Name.Com_Data[Read_Write_Wifi_Name.Data_Len])) = Com_Sensor_Cal_Constant;
					/* ���� У�� */
					*((u8*)&(Read_Write_Wifi_Name.Com_Data[Read_Write_Wifi_Name.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Name);
					/* ��Ӧ ʵʱ���� ������Ϣ */
					USART_printf(USART1,(u8*)&Read_Write_Wifi_Name,(Read_Write_Wifi_Name.Data_Len + Com_Sensor_Cal_Fix_Data_Len));		
			
			}
			else if((Cal_Inf->Chanel_Num==0xAA)&&(Cal_Inf->Data_Len<=Wifi_Name_Size))//д
			{
				/* ������������� *//* дWifi���� */
				memset(Read_Write_Wifi_Name_Data_P,'\0',Wifi_Name_Size);
				/* �����ݿ��������ͼĴ����� */
				memcpy(Read_Write_Wifi_Name_Data_P,Cal_Inf->Com_Data,(Cal_Inf->Data_Len > (Wifi_Name_Size - 1)) ? (Wifi_Name_Size - 1):Cal_Inf->Data_Len);
				Write_Config_Tab((u8*)Read_Write_Wifi_Name_Data_P,sizeof(Read_Write_Wifi_Name_Data_P));/* ����Wifi���� */
				/* �ظ���λ��ָ�������װ */
				Read_Write_Wifi_Name.Head_Code       	= Com_Sensor_Cal_Rece_Head_Code;
				Read_Write_Wifi_Name.Instru_NO 	      = Cal_Inf->Instru_NO;//�������
				Read_Write_Wifi_Name.Cmd_Code		      = Com_Read_Write_Wifi_Name_Cmd_Code;
				Read_Write_Wifi_Name.Chanel_Num	      = Cal_Inf->Chanel_Num;
				Read_Write_Wifi_Name.Data_Len		      = Com_Read_Write_Wifi_Name_Data_Len;
				/* �����ݿ��������ͼĴ����� */
				Read_Write_Wifi_Name.Com_Data[0]	    = Com_Sensor_Cal_Set_Para_Succeed;
				/* ���� ���� */
				Read_Write_Wifi_Name.Com_Data[1]	    = Com_Sensor_Cal_Constant;
				/* ���� У�� */
				Read_Write_Wifi_Name.Com_Data[2] 	    = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Name);
				/* ��Ӧ ʵʱ���� ������Ϣ */
				USART_printf(USART1,(u8*)&Read_Write_Wifi_Name,Com_Read_Write_Wifi_Name_Len);	
				
				
			
				
			}
			
   #undef Com_Read_Write_Wifi_Name_Data_Len
   #undef Com_Read_Write_Wifi_Name_Len
   #undef Write_Wifi_Name_Data_P				
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Read_Write_Wifi_WSKEY*/
/**	����˵��: ����дWifi_WSKEY(���ܷ�ʽ������)***************/
/**	��    �Σ�������Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_WSKEY(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	/*дWifi_WSKEY(���ܷ�ʽ������) Ӧ������ݳ��� */
#define Com_Read_Write_Wifi_WSKEY_Data_Len	  (1u) //���1����������
/* дWifi_WSKEY(���ܷ�ʽ������) Ӧ��Э�鳤�� */
//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
#define Com_Read_Write_Wifi_WSKEY_Len		       (Com_Read_Write_Wifi_WSKEY_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* дWifi_WSKEY(���ܷ�ʽ������)�����ݵ�ַ */
#define  Read_Write_Wifi_WSKEY_Data_P				   (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password)	
COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_WSKEY;//���մ������� 
/*Cal_Inf->Chanel_Num=0x55���������    Cal_Inf->Chanel_Num=0xAA����д����*/
	    if((Cal_Inf->Chanel_Num==0x55)&&(Cal_Inf->Data_Len<=Wifi_Passward_Size))//��
			{				
	        /* ���� ʵʱ���� ���� */
					Read_Write_Wifi_WSKEY.Head_Code 	   = Com_Sensor_Cal_Rece_Head_Code;
					Read_Write_Wifi_WSKEY.Instru_NO 	   = Cal_Inf->Instru_NO;//�������
					Read_Write_Wifi_WSKEY.Cmd_Code	     = Com_Read_Write_Wifi_WSKEY_Cmd_Code;
					Read_Write_Wifi_WSKEY.Chanel_Num     = Cal_Inf->Chanel_Num;
					Read_Write_Wifi_WSKEY.Data_Len	     = strlen(&Read_Write_Wifi_WSKEY_Data_P[1])+2;
					Read_Write_Wifi_WSKEY.Com_Data[0]    =0;//Ĭ���޼��ܷ�ʽ
					/* �����ݿ��������ͼĴ����� */
					memcpy(&Read_Write_Wifi_WSKEY.Com_Data[1],Read_Write_Wifi_WSKEY_Data_P,Read_Write_Wifi_WSKEY.Data_Len);
				
					/* ���� ���� */
					*((u8*)&(Read_Write_Wifi_WSKEY.Com_Data[Read_Write_Wifi_WSKEY.Data_Len])) = Com_Sensor_Cal_Constant;
					/* ���� У�� */
					*((u8*)&(Read_Write_Wifi_WSKEY.Com_Data[Read_Write_Wifi_WSKEY.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_WSKEY);
				
				
					/* ��Ӧ ʵʱ���� ������Ϣ */
					USART_printf(USART1,(u8*)&Read_Write_Wifi_WSKEY,(Read_Write_Wifi_WSKEY.Data_Len + Com_Sensor_Cal_Fix_Data_Len));		
			
			}
			else if((Cal_Inf->Chanel_Num==0xAA)&&(Cal_Inf->Data_Len<=Wifi_Passward_Size))//д
			{
				/* ������������� *//* дWifi���� */
				memset(Read_Write_Wifi_WSKEY_Data_P,'\0',Wifi_Passward_Size);
				/* �����ݿ��������ͼĴ����� */
				memcpy(Read_Write_Wifi_WSKEY_Data_P,&Cal_Inf->Com_Data[1],(Cal_Inf->Data_Len > (Wifi_Passward_Size - 1)) ? (Wifi_Passward_Size - 1):Cal_Inf->Data_Len-1);
				
				Write_Config_Tab((u8*)Read_Write_Wifi_WSKEY_Data_P,sizeof(Read_Write_Wifi_WSKEY_Data_P));/* ����Wifi����  */
				/* �ظ���λ��ָ�������װ */
				Read_Write_Wifi_WSKEY.Head_Code       	= Com_Sensor_Cal_Rece_Head_Code;
				Read_Write_Wifi_WSKEY.Instru_NO 	      = Cal_Inf->Instru_NO;//�������
				Read_Write_Wifi_WSKEY.Cmd_Code		      = Com_Read_Write_Wifi_WSKEY_Cmd_Code;
				Read_Write_Wifi_WSKEY.Chanel_Num	      = Cal_Inf->Chanel_Num;
				Read_Write_Wifi_WSKEY.Data_Len		      = Com_Read_Write_Wifi_WSKEY_Data_Len;
				/* �����ݿ��������ͼĴ����� */
				Read_Write_Wifi_WSKEY.Com_Data[0]	      = Com_Sensor_Cal_Set_Para_Succeed;
				/* ���� ���� */
				Read_Write_Wifi_WSKEY.Com_Data[1]	      = Com_Sensor_Cal_Constant;
				/* ���� У�� */
				Read_Write_Wifi_WSKEY.Com_Data[2] 	    = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_WSKEY);
//////////						OSTimeDly(100);//��ʱ100ms
				/* ��Ӧ ʵʱ���� ������Ϣ */
				USART_printf(USART1,(u8*)&Read_Write_Wifi_WSKEY,Com_Read_Write_Wifi_WSKEY_Len);	
				
				
				
					GSM_Config_Reg.WifiNamePWD_Config_State=TRUE;//��λ���·�������Wifiģ������ּ���������
//////				GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,SetWifiNamePWD_EVENT);//��������Wifiģ�����ֺ�������Ϣ
				
							
			}
#undef Com_Read_Write_Wifi_Name_Data_Len
#undef Com_Read_Write_Wifi_Name_Len
#undef Write_Wifi_Name_Data_P	
			
}
/************************************************************************************************/
/**	�� �� ��: Com_Config_Read_Write_Wifi_Wlan*/
/**	����˵��: ����дWifi_WANN(LOCALIP:����IP��ַ��  MASK(��������) ��GATEWAY(���ص�ַ))***************/
/**	��    �Σ�������Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_Wlan(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
   /* Ӧ��Э��Ĵ��� */
   COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_Wlan;//���մ������� 
/*Cal_Inf->Chanel_Num=0x55���������    Cal_Inf->Chanel_Num=0xAA����д����*/
	    if(Cal_Inf->Chanel_Num==0x55)//��
			{	
	      	memset((u8*)&Read_Write_Wifi_Wlan,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF)); 
				  ConvertIP_StringToArray(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,&Read_Write_Wifi_Wlan.Com_Data[0],3,".");//����IP��ַ  4���ֽ�  3��  .
				  ConvertIP_StringToArray(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,&Read_Write_Wifi_Wlan.Com_Data[4],3,".");//�������� 
				  ConvertIP_StringToArray(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,&Read_Write_Wifi_Wlan.Com_Data[8],3,"."); //����IP��ַ 
				
				  Read_Write_Wifi_Wlan.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	        Read_Write_Wifi_Wlan.Instru_NO 	= Cal_Inf->Instru_NO;
	        Read_Write_Wifi_Wlan.Cmd_Code		= Com_Read_Write_Wifi_WALN_Cmd_Code;
	        Read_Write_Wifi_Wlan.Chanel_Num	= Cal_Inf->Chanel_Num;
	        Read_Write_Wifi_Wlan.Data_Len		= 12u;//4���ֽ�LOCALIP+4���ֽ�MASK+4���ֽ�GATEWAY
				  Read_Write_Wifi_Wlan.Com_Data[Read_Write_Wifi_Wlan.Data_Len] = Com_Sensor_Cal_Constant;
	        Read_Write_Wifi_Wlan.Com_Data[Read_Write_Wifi_Wlan.Data_Len+1] = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Wlan);
	        USART_printf(USART1,(u8*)&Read_Write_Wifi_Wlan,(Com_Sensor_Cal_Fix_Data_Len+Read_Write_Wifi_Wlan.Data_Len));	
			}
			else if(Cal_Inf->Chanel_Num==0xAA)//д
			{
				
				

		    sprintf(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,"%d.%d.%d.%d",
		                               Cal_Inf->Com_Data[0],
		                               Cal_Inf->Com_Data[1],
		                               Cal_Inf->Com_Data[2],
		                               Cal_Inf->Com_Data[3]);	//����IP��ַ
				
				
			  sprintf(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,"%d.%d.%d.%d",
		                               Cal_Inf->Com_Data[4],
		                               Cal_Inf->Com_Data[5],
		                               Cal_Inf->Com_Data[6],
		                               Cal_Inf->Com_Data[7]);//�������� 
				
			  sprintf(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,"%d.%d.%d.%d",
		                               Cal_Inf->Com_Data[8],
		                               Cal_Inf->Com_Data[9],
		                               Cal_Inf->Com_Data[10],
		                               Cal_Inf->Com_Data[11]);//����IP��ַ

				Read_Write_Wifi_Wlan.Head_Code 	   = Com_Sensor_Cal_Rece_Head_Code;
	      Read_Write_Wifi_Wlan.Instru_NO 	   = Cal_Inf->Instru_NO;
	      Read_Write_Wifi_Wlan.Cmd_Code	     = ComConfigDataNetworkWrite;
	      Read_Write_Wifi_Wlan.Chanel_Num    = Cal_Inf->Chanel_Num;
	      Read_Write_Wifi_Wlan.Data_Len	     = (1u);	
				Read_Write_Wifi_Wlan.Com_Data[0]	 = Com_Sensor_Cal_Set_Para_Succeed;
	      Read_Write_Wifi_Wlan.Com_Data[1]	 = Com_Sensor_Cal_Constant;
	      Read_Write_Wifi_Wlan.Com_Data[2]   = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Wlan);
				
////////							OSTimeDly(100);//��ʱ100ms
	      USART_printf(USART1,(u8*)&Read_Write_Wifi_Wlan,(Com_Sensor_Cal_Fix_Data_Len+1));	
				
				
				GSM_Config_Reg.WifiWLAN_Config_State=TRUE;//��λ���·�������Wifiģ���WLAN��������
				

				  
				GSM_Config_Reg.GSM_Config_State=TRUE;//��ʼ����Wifiģ�����
				GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);//�·���ʼ��Wifiģ����Ϣ
	
				
				
				
////////				
////////				//			/* Wifi����WifiWALN����*/
////////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,SetWifiWLAN_EVENT);
				
				
			}		
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Wifi_NetP_Read														*/
/**	����˵��: ��Wifi���ݷ����������ò���																*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Wifi_NetP_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
//	/* ��������ָ�� */
//	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;

	/* ���þֲ�������ʼֵ */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* �������ݷ��������� */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= Com_Read_Write_Wifi_NETP_Cmd_Code;
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 6u;


   ackDataNetworkRead.Com_Data[0]=(0xFF00&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1)>>8;
   ackDataNetworkRead.Com_Data[1]=(0xFF&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1);
	 ackDataNetworkRead.Com_Data[2]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A;
   ackDataNetworkRead.Com_Data[3]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B;
	 ackDataNetworkRead.Com_Data[4]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C;
   ackDataNetworkRead.Com_Data[5]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D;
	 

		   
	/* ���� ���� */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* ���� У�� */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Wifi_NetP_Write														*/
/**	����˵��: д���ݷ����������ò���																*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Wifi_NetP_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	  /* Ӧ��Э��Ĵ��� */
	  COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	  /* ���ݺϷ��Ա�־ */
	  BOOL data_vldy = TRUE;

////////	
////////		/* ��ȡ�������� */
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A = Cal_Inf->Com_Data[2];
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B = Cal_Inf->Com_Data[3];
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C = Cal_Inf->Com_Data[4];
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D = Cal_Inf->Com_Data[5];
////////	
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1 = (Cal_Inf->Com_Data[0]<<8)|(Cal_Inf->Com_Data[1]&0xff);
	
	
	
////		/* ������������Ĵ��� */
////		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,'\0',32);


////		/* �޸��������ñ��ж�ӦIP������ */
////		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address,
////						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address));
////		/* �޸��������ñ��ж�Ӧ�˿ڵ����� */
////		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1,
////						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1));
////		/* �޸��������ñ��ж�Ӧ���������� */
////		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
////						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));	

	
	/* ���� ʵʱ���� ���� */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	  = Com_Read_Write_Wifi_NETP_Cmd_Code;
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	  = (1u);
	
	/* �����ݿ��������ͼĴ����� */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ���� ���� */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Read_Write_Wifi_NetP*/
/**	����˵��: ����дWifi��NETP��ز���IP��ַ�Ͷ˿�***************/
/**	��    �Σ�������Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_NetP(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{ 
/*Cal_Inf->Chanel_Num=0x55���������    Cal_Inf->Chanel_Num=0xAA����д����*/
	         if(Cal_Inf->Chanel_Num==0x55)//��
			     {	
						 
						 Com_Config_Wifi_NetP_Read( Cal_Inf);

			     }
			    else if(Cal_Inf->Chanel_Num==0xAA)//д
			    {
	           Com_Config_Wifi_NetP_Write( Cal_Inf); 
					 }						

}
/************************************************************************************************/
/**	�� �� ��: Com_Config_Read_Write_Wifi_DHCP*/
/**	����˵��: ����дWifiģ���ȡIP��ַ��ʽ***************/
/**	��    �Σ�������Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_DHCP(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
   /* Ӧ��Э��Ĵ��� */
   COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_DHCP;//���մ������� 
/*Cal_Inf->Chanel_Num=0x55���������    Cal_Inf->Chanel_Num=0xAA����д����*/
	    if(Cal_Inf->Chanel_Num==0x55)//��
			{	
	      	memset((u8*)&Read_Write_Wifi_DHCP,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF)); 
				  Read_Write_Wifi_DHCP.Head_Code 	  = Com_Sensor_Cal_Rece_Head_Code;
	        Read_Write_Wifi_DHCP.Instru_NO 	  = Cal_Inf->Instru_NO;
	        Read_Write_Wifi_DHCP.Cmd_Code		  = Com_Read_Write_Wifi_DHCP_Cmd_Code;
	        Read_Write_Wifi_DHCP.Chanel_Num	  = Cal_Inf->Chanel_Num;
	        Read_Write_Wifi_DHCP.Data_Len		  = 1u;//1���ֽ�      
				  Read_Write_Wifi_DHCP.Com_Data[0]  = Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way  ;//0x00�����ȡIP��ַ�ķ�ʽΪSTATIC   0x01�����ȡIP��ַ�ķ�ʽΪDHCP 
				  Read_Write_Wifi_DHCP.Com_Data[Read_Write_Wifi_DHCP.Data_Len] = Com_Sensor_Cal_Constant;
	        Read_Write_Wifi_DHCP.Com_Data[Read_Write_Wifi_DHCP.Data_Len+1] = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_DHCP);
	        USART_printf(USART1,(u8*)&Read_Write_Wifi_DHCP,(Com_Sensor_Cal_Fix_Data_Len+Read_Write_Wifi_DHCP.Data_Len));	
			}
			else if(Cal_Inf->Chanel_Num==0xAA)//д
			{
				 Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way = Cal_Inf->Com_Data[0];//0x00�����ȡIP��ַ�ķ�ʽΪSTATIC   0x01�����ȡIP��ַ�ķ�ʽΪDHCP 
				 Read_Write_Wifi_DHCP.Head_Code 	       = Com_Sensor_Cal_Rece_Head_Code;
	       Read_Write_Wifi_DHCP.Instru_NO 	       = Cal_Inf->Instru_NO;
	       Read_Write_Wifi_DHCP.Cmd_Code	         = Com_Read_Write_Wifi_DHCP_Cmd_Code;
	       Read_Write_Wifi_DHCP.Chanel_Num         = Cal_Inf->Chanel_Num;
	       Read_Write_Wifi_DHCP.Data_Len	         = (1u);	
				 Read_Write_Wifi_DHCP.Com_Data[0]	       = Com_Sensor_Cal_Set_Para_Succeed;
	       Read_Write_Wifi_DHCP.Com_Data[1]	       = Com_Sensor_Cal_Constant;
	       Read_Write_Wifi_DHCP.Com_Data[2]        = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_DHCP);
				
////////////							OSTimeDly(100);//��ʱ100ms
	       USART_printf(USART1,(u8*)&Read_Write_Wifi_DHCP,(Com_Sensor_Cal_Fix_Data_Len+1));	
				
				
				GSM_Config_Reg.WifiWLAN_GET_IP_WAY_Config_State=TRUE;//��λ���·�������Wifiģ��Ļ�ȡIP��ַ��ʽ����
				
				if((GSM_Config_Reg.WifiWLAN_Config_State==FALSE)&&(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==0x01))
				{
				     GSM_Config_Reg.GSM_Config_State=TRUE;//��ʼ����Wifiģ�����
				   GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);//�·���ʼ��Wifiģ����Ϣ
				}
				
				
				
				
////////				 /* Wifiģ���ȡIP��ַ��ʽ����*/
////////			   GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,SetWifiWLAN_GET_IP_WAY_EVENT);

			}		
}
/************************************************************************************************/
/**	�� �� ��: Com_Config_Read_Write_Wifi_MAC*/
/**	����˵��: ����дWifiģ���MAC��ַ***************/
/**	��    �Σ�������Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_MAC(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
   /* Ӧ��Э��Ĵ��� */
   COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_MAC;//���մ������� 
/*Cal_Inf->Chanel_Num=0x55���������    Cal_Inf->Chanel_Num=0xAA����д����*/
	    if(Cal_Inf->Chanel_Num==0x55)//��
			{	
	      	memset((u8*)&Read_Write_Wifi_MAC,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF)); 
				  Read_Write_Wifi_MAC.Head_Code 	  = Com_Sensor_Cal_Rece_Head_Code;
	        Read_Write_Wifi_MAC.Instru_NO 	  = Cal_Inf->Instru_NO;
	        Read_Write_Wifi_MAC.Cmd_Code		  = Com_Read_Write_Wifi_MAC_Cmd_Code;
	        Read_Write_Wifi_MAC.Chanel_Num	  = Cal_Inf->Chanel_Num;
	        Read_Write_Wifi_MAC.Data_Len		  = strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address);//MAC��ַ�����ֽ� 
				  memcpy(Read_Write_Wifi_MAC.Com_Data,Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address, Read_Write_Wifi_MAC.Data_Len);
				  Read_Write_Wifi_MAC.Com_Data[Read_Write_Wifi_MAC.Data_Len]    = Com_Sensor_Cal_Constant;
	        Read_Write_Wifi_MAC.Com_Data[Read_Write_Wifi_MAC.Data_Len+1]  = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_MAC);
	        USART_printf(USART1,(u8*)&Read_Write_Wifi_MAC,(Com_Sensor_Cal_Fix_Data_Len+Read_Write_Wifi_MAC.Data_Len));

				
			}
			else if(Cal_Inf->Chanel_Num==0xAA)//д
			{
				
////////				 memcpy(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address,Cal_Inf->Com_Data,Cal_Inf->Data_Len);

				 Read_Write_Wifi_MAC.Head_Code 	       = Com_Sensor_Cal_Rece_Head_Code;
	       Read_Write_Wifi_MAC.Instru_NO 	       = Cal_Inf->Instru_NO;
	       Read_Write_Wifi_MAC.Cmd_Code	         = Com_Read_Write_Wifi_MAC_Cmd_Code;
	       Read_Write_Wifi_MAC.Chanel_Num        = Cal_Inf->Chanel_Num;
	       Read_Write_Wifi_MAC.Data_Len	         = (1u);	
				 Read_Write_Wifi_MAC.Com_Data[0]	     = Com_Sensor_Cal_Set_Para_Succeed;
	       Read_Write_Wifi_MAC.Com_Data[1]	     = Com_Sensor_Cal_Constant;
	       Read_Write_Wifi_MAC.Com_Data[2]       = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_MAC);
	       USART_printf(USART1,(u8*)&Read_Write_Wifi_MAC,(Com_Sensor_Cal_Fix_Data_Len+1));	

			}		
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Rest_Wifi*/
/**	����˵��:��λ���߻ָ���������Wifiģ��***************/
/**	��    �Σ�������Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Rest_Wifi(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
   /* Ӧ��Э��Ĵ��� */
   COM_SENSOR_CAL_TYPEDEF                          Read_Rest_Wifi;//���մ������� 
/*Cal_Inf->Chanel_Num=0x55���������    Cal_Inf->Chanel_Num=0xAA����д����*/
      if(Cal_Inf->Chanel_Num==0xAA)//д
			{
				  if(Cal_Inf->Com_Data[0]==1)
					{//���WIFI�����޸ģ��˳�����ģʽ���������ú�����Զ��ϴ�ģʽ��

					}
					else if(Cal_Inf->Com_Data[0]==2)
					{
									OSTimeDly(100);//��ʱ100ms
						//ǿ�Ƹ�λWIFIģ��
					  /* ��λWifiģ��*/
             GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,RestWifi_EVENT);
					}
					else if(Cal_Inf->Com_Data[0]==3)
					{
									OSTimeDly(100);//��ʱ100ms
						//ǿ�ƻָ���������
		   	    /* �ָ���������Wifiģ��*/
      		 	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Factory_Data_Reset_Wifi_EVENT);
					}
					
				 Read_Rest_Wifi.Head_Code 	           = Com_Sensor_Cal_Rece_Head_Code;
	       Read_Rest_Wifi.Instru_NO 	           = Cal_Inf->Instru_NO;
	       Read_Rest_Wifi.Cmd_Code	         = Com_Set_Rest_Wifi_Cmd_Code;
	       Read_Rest_Wifi.Chanel_Num        = Cal_Inf->Chanel_Num;
	       Read_Rest_Wifi.Data_Len	         = (1u);	
				 Read_Rest_Wifi.Com_Data[0]	     = Com_Sensor_Cal_Set_Para_Succeed;
	       Read_Rest_Wifi.Com_Data[1]	     = Com_Sensor_Cal_Constant;
	       Read_Rest_Wifi.Com_Data[2]       = Com_Sensor_Cal_Get_Check(&Read_Rest_Wifi);
	       USART_printf(USART1,(u8*)&Read_Rest_Wifi,(Com_Sensor_Cal_Fix_Data_Len+1));		
			}		
}
#endif
/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Read_Real_Data														*/
/**	����˵��: ��ȡ������ʵʱ����																	*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Real_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Real_Data_Len	 	(11u)//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+4(����)+1(����)+1(У���)
#define Com_Sensor_Cal_Read_data_buffer_Size	(sizeof(float))//�����ȸ��������ݳ���
/* ������õĴ��������� */
	COM_SENSOR_CAL_TYPEDEF read_real_data_ack;
	float com_data_buffer;

	/* ���� ʵʱ���� ���� */
	read_real_data_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;	
	read_real_data_ack.Instru_NO 	= Cal_Inf->Instru_NO;
	read_real_data_ack.Cmd_Code	 	= Com_Sensor_Cal_Cmd_Code_Read_Real_Data;
	read_real_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_real_data_ack.Data_Len		= Com_Sensor_Cal_Read_data_buffer_Size;
	
	
	/* �жϻ�ȡ���� �ڼ�ͨ�� �Ĵ������������� */
	/* ��ȡ��Ӧͨ������ʵֵ��У׼ֵ */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
		default:
		{
			/* ��ȡ��ͨ��Ŀ�겻���� */
			__nop();
			return;
		}
	}

	memcpy(read_real_data_ack.Com_Data,(u8*)(&com_data_buffer),Com_Sensor_Cal_Read_data_buffer_Size);
	/* ���� ���� */
	read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size] = Com_Sensor_Cal_Constant;
	/* ���� У�� */
	read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size + 1] 	= Com_Sensor_Cal_Get_Check(&read_real_data_ack);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&read_real_data_ack,Com_Sensor_Cal_Read_Real_Data_Len);
#undef Com_Sensor_Cal_Read_Real_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
}

#else
/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Read_Real_Data														*/
/**	����˵��: ��ȡ������ʵʱ����																	*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Real_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Real_Data_Len	 	(9u)//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+2(����)+1(����)+1(У���)
#define Com_Sensor_Cal_Read_data_buffer_Size	(2u)//���ݳ��ȣ�2�ֽ�
#define Com_Sensor_Cal_Data_Ratio_K				(10u)//���ݷŴ���
#define Com_Sensor_Cal_Temp_Data_Offset_Value	(2731u)//�¶�ֵ���ݵ�ƫ��ֵ


	/* ������õĴ��������� */
		COM_SENSOR_CAL_TYPEDEF read_real_data_ack;
		u16 com_data_buffer;
	
		/* ���� ʵʱ���� ���� */
		read_real_data_ack.Head_Code	= Com_Sensor_Cal_Rece_Head_Code;	
		read_real_data_ack.Instru_NO	= Cal_Inf->Instru_NO;
		read_real_data_ack.Cmd_Code 	= Com_Sensor_Cal_Cmd_Code_Read_Real_Data;
		read_real_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
		read_real_data_ack.Data_Len 	= Com_Sensor_Cal_Read_data_buffer_Size;
		
		
		/* �жϻ�ȡ���� �ڼ�ͨ�� �Ĵ������������� */
		/* ��ȡ��Ӧͨ������ʵֵ��У׼ֵ */
		switch(Cal_Inf->Chanel_Num)
		{
#if	(Sensor_CH1_Config_Type != Sensor_Null)
			case Sensors_CH_1:
			{
#if	((Sensor_CH1_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}	
			
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
			case Sensors_CH_2:
			{
#if	((Sensor_CH2_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
			case Sensors_CH_3:
			{
#if	((Sensor_CH3_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
			case Sensors_CH_4:
			{
#if	((Sensor_CH4_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif
				break;
			}
#endif	
			
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
			case Sensors_CH_5:
			{
#if	((Sensor_CH5_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH5_Config_Type == Sensor_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
			case Sensors_CH_6:
			{
#if	((Sensor_CH6_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH6_Config_Type == Sensor_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
			case Sensors_CH_7:
			{
#if	((Sensor_CH7_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH7_Config_Type == Sensor_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
			case Sensors_CH_8:
			{
#if	((Sensor_CH8_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH8_Config_Type == Sensor_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
			case Sensors_CH_9:
			{
#if	((Sensor_CH9_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH9_Config_Type == Sensor_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}				
#endif	
			
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
			case Sensors_CH_10:
			{
#if	((Sensor_CH10_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH10_Config_Type == Sensor_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
			case Sensors_CH_11:
			{
#if	((Sensor_CH11_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH11_Config_Type == Sensor_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
			case Sensors_CH_12:
			{
#if	((Sensor_CH12_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH12_Config_Type == Sensor_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
			case Sensors_CH_13:
			{
#if	((Sensor_CH13_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH13_Config_Type == Sensor_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
			case Sensors_CH_14:
		{
#if	((Sensor_CH14_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH14_Config_Type == Sensor_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
			case Sensors_CH_15:
			{
#if	((Sensor_CH15_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH15_Config_Type == Sensor_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
			case Sensors_CH_16:
			{
#if	((Sensor_CH16_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH16_Config_Type == Sensor_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
			case Sensors_CH_17:
			{
#if	((Sensor_CH17_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH17_Config_Type == Sensor_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
			case Sensors_CH_18:
			{
#if	((Sensor_CH18_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH18_Config_Type == Sensor_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
			case Sensors_CH_19:
			{
#if	((Sensor_CH19_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH19_Config_Type == Sensor_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
			case Sensors_CH_20:
			{
#if	((Sensor_CH20_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH20_Config_Type == Sensor_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
			case Sensors_CH_21:
			{
#if	((Sensor_CH21_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH21_Config_Type == Sensor_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
			case Sensors_CH_22:
			{
#if	((Sensor_CH22_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH22_Config_Type == Sensor_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
			case Sensors_CH_23:
			{
#if	((Sensor_CH23_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH23_Config_Type == Sensor_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
			case Sensors_CH_24:
			{
#if	((Sensor_CH24_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH24_Config_Type == Sensor_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
			case Sensors_CH_25:
			{
#if	((Sensor_CH25_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH25_Config_Type == Sensor_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
			case Sensors_CH_26:
			{
#if	((Sensor_CH26_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH26_Config_Type == Sensor_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
			case Sensors_CH_27:
			{
#if	((Sensor_CH27_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH27_Config_Type == Sensor_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
			case Sensors_CH_28:
			{
#if	((Sensor_CH28_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH28_Config_Type == Sensor_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
			case Sensors_CH_29:
			{
#if	((Sensor_CH29_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH29_Config_Type == Sensor_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
			case Sensors_CH_30:
			{
#if	((Sensor_CH30_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH30_Config_Type == Sensor_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
			case Sensors_CH_31:
			{
#if	((Sensor_CH31_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH31_Config_Type == Sensor_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
			case Sensors_CH_32:
			{
#if	((Sensor_CH32_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH32_Config_Type == Sensor_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
			default:
			{
				/* ��ȡ��ͨ��Ŀ�겻���� */
				__nop();
				return;
			}
		}
#endif
		memcpy(read_real_data_ack.Com_Data,(u8*)(&com_data_buffer),Com_Sensor_Cal_Read_data_buffer_Size);
		/* ���� ���� */
		read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size] = Com_Sensor_Cal_Constant;
		/* ���� У�� */
		read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size + 1]	= Com_Sensor_Cal_Get_Check(&read_real_data_ack);
		/* ��Ӧ ʵʱ���� ������Ϣ */
		USART_printf(USART1,(u8*)&read_real_data_ack,Com_Sensor_Cal_Read_Real_Data_Len);

		
#undef Com_Sensor_Cal_Read_Real_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
#undef Com_Sensor_Cal_Data_Ratio_K
#undef Com_Sensor_Cal_Temp_Data_Offset_Value
}
#endif


#if (1)
/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Read_Orig_Data														*/
/**	����˵��: ��ȡ������ԭʼ����																	*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Orig_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Orig_Data_Len		(11u)//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+2(����)+1(����)+1(У���)
#define Com_Sensor_Cal_Read_data_buffer_Size	(sizeof(float))//�����ȸ��������ݳ���
	COM_SENSOR_CAL_TYPEDEF read_Orig_data_ack;
	float com_data_buffer;

	/* ���� ʵʱ���� ���� */
	read_Orig_data_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_Orig_data_ack.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	read_Orig_data_ack.Cmd_Code	 	= Com_Sensor_Cal_Cmd_Code_Read_Orig_Data;
	read_Orig_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_Orig_data_ack.Data_Len		= Com_Sensor_Cal_Read_data_buffer_Size;
	
	
	/* �жϻ�ȡ���� �ڼ�ͨ�� �Ĵ������������� */
	/* ��ȡ��Ӧͨ������ʵֵ��У׼ֵ */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
			break;
		}
#endif	
		
		default:
		{
			/* ��ȡ��ͨ��Ŀ�겻���� */
			__nop();
			return;
		}
	}
	
	memcpy(read_Orig_data_ack.Com_Data,(u8*)&com_data_buffer,Com_Sensor_Cal_Read_data_buffer_Size);
	/* ���� ���� */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size] 	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size+1] = Com_Sensor_Cal_Get_Check(&read_Orig_data_ack);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&read_Orig_data_ack,Com_Sensor_Cal_Read_Orig_Data_Len);
#undef Com_Sensor_Cal_Read_Orig_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
}
#else
/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Read_Orig_Data														*/
/**	����˵��: ��ȡ������ԭʼ����																	*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Orig_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Orig_Data_Len		(9u)//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+2(����)+1(����)+1(У���)
#define Com_Sensor_Cal_Read_data_buffer_Size	(2u)//���ݳ��ȣ�2�ֽ�
#define Com_Sensor_Cal_Data_Ratio_K				(10u)//���ݷŴ���
#define Com_Sensor_Cal_Temp_Data_Offset_Value	(2731u)//�¶�ֵ���ݵ�ƫ��ֵ


	COM_SENSOR_CAL_TYPEDEF read_Orig_data_ack;
	u16 com_data_buffer;

	/* ���� ʵʱ���� ���� */
	read_Orig_data_ack.Head_Code	= Com_Sensor_Cal_Rece_Head_Code;
	read_Orig_data_ack.Instru_NO	= Cal_Inf->Instru_NO;//�������
	read_Orig_data_ack.Cmd_Code 	= Com_Sensor_Cal_Cmd_Code_Read_Orig_Data;
	read_Orig_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_Orig_data_ack.Data_Len 	= Com_Sensor_Cal_Read_data_buffer_Size;
	
			
			/* �жϻ�ȡ���� �ڼ�ͨ�� �Ĵ������������� */
			/* ��ȡ��Ӧͨ������ʵֵ��У׼ֵ */
			switch(Cal_Inf->Chanel_Num)
			{
#if	(Sensor_CH1_Config_Type != Sensor_Null)
				case Sensors_CH_1:
				{
#if	((Sensor_CH1_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
				case Sensors_CH_2:
				{
#if	((Sensor_CH2_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
				case Sensors_CH_3:
				{
#if	((Sensor_CH3_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
				case Sensors_CH_4:
				{
#if	((Sensor_CH4_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif
					break;
				}
#endif	
				
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
				case Sensors_CH_5:
				{
#if	((Sensor_CH5_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH5_Config_Type == Sensor_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
				case Sensors_CH_6:
				{
#if	((Sensor_CH6_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH6_Config_Type == Sensor_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
				case Sensors_CH_7:
				{
#if	((Sensor_CH7_Config_Type == Sensor_Tmp)||\
					(Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH7_Config_Type == Sensor_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
				case Sensors_CH_8:
				{
#if	((Sensor_CH8_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH8_Config_Type == Sensor_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
				case Sensors_CH_9:
				{
#if	((Sensor_CH9_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH9_Config_Type == Sensor_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}				
#endif	
				
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
				case Sensors_CH_10:
				{
#if	((Sensor_CH10_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH10_Config_Type == Sensor_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
				case Sensors_CH_11:
				{
#if	((Sensor_CH11_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH11_Config_Type == Sensor_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
				case Sensors_CH_12:
				{
#if	((Sensor_CH12_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH12_Config_Type == Sensor_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
				case Sensors_CH_13:
				{
#if	((Sensor_CH13_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH13_Config_Type == Sensor_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
				case Sensors_CH_14:
			{
#if	((Sensor_CH14_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH14_Config_Type == Sensor_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
				case Sensors_CH_15:
				{
#if	((Sensor_CH15_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH15_Config_Type == Sensor_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
				case Sensors_CH_16:
				{
#if	((Sensor_CH16_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH16_Config_Type == Sensor_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
				case Sensors_CH_17:
				{
#if	((Sensor_CH17_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH17_Config_Type == Sensor_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
				case Sensors_CH_18:
				{
#if	((Sensor_CH18_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH18_Config_Type == Sensor_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
				case Sensors_CH_19:
				{
#if	((Sensor_CH19_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH19_Config_Type == Sensor_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
				case Sensors_CH_20:
				{
#if	((Sensor_CH20_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH20_Config_Type == Sensor_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
				case Sensors_CH_21:
				{
#if	((Sensor_CH21_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH21_Config_Type == Sensor_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
				case Sensors_CH_22:
				{
#if	((Sensor_CH22_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH22_Config_Type == Sensor_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
				case Sensors_CH_23:
				{
#if	((Sensor_CH23_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH23_Config_Type == Sensor_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
				case Sensors_CH_24:
				{
#if	((Sensor_CH24_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH24_Config_Type == Sensor_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
				case Sensors_CH_25:
				{
#if	((Sensor_CH25_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH25_Config_Type == Sensor_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
				case Sensors_CH_26:
				{
#if	((Sensor_CH26_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH26_Config_Type == Sensor_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
				case Sensors_CH_27:
				{
#if	((Sensor_CH27_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH27_Config_Type == Sensor_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
				case Sensors_CH_28:
				{
#if	((Sensor_CH28_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH28_Config_Type == Sensor_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
				case Sensors_CH_29:
				{
#if	((Sensor_CH29_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH29_Config_Type == Sensor_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
				case Sensors_CH_30:
				{
#if	((Sensor_CH30_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH30_Config_Type == Sensor_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
				case Sensors_CH_31:
				{
#if	((Sensor_CH31_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH31_Config_Type == Sensor_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
				case Sensors_CH_32:
				{
#if	((Sensor_CH32_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH32_Config_Type == Sensor_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
				default:
				{
					/* ��ȡ��ͨ��Ŀ�겻���� */
					__nop();
					return;
				}
			}

	
	memcpy(read_Orig_data_ack.Com_Data,(u8*)&com_data_buffer,Com_Sensor_Cal_Read_data_buffer_Size);
	/* ���� ���� */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size+1] = Com_Sensor_Cal_Get_Check(&read_Orig_data_ack);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&read_Orig_data_ack,Com_Sensor_Cal_Read_Orig_Data_Len);
#undef Com_Sensor_Cal_Read_Orig_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
#undef Com_Sensor_Cal_Data_Ratio_K
#undef Com_Sensor_Cal_Temp_Data_Offset_Value
}
#endif


/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Write_Curve_Data													*/
/**	����˵��: �޸Ĵ�������Ӧͨ����������															*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Write_Curve_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* ������У׼ д���� ����֡���� */
#define Com_Sensor_Cal_Write_Curve_Data_Len			(1u)//���103����������
/* ������У׼ д���� ����֡���� *///���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
#define Com_Sensor_Cal_Write_Curve_Packet_Len		(Com_Sensor_Cal_Write_Curve_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//���103����������	
	
	/* ��������ʵֵ����ָ�� */
	u16* sensor_act_curve_data_p;
	/* ��������ʵֵ����ָ�� */
	u16* sensor_cal_curve_data_p;
	/* ���� ������д���� ����֡ �Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF write_curve_ack;
	
	
	/* �жϻ�ȡ���� �ڼ�ͨ�� �Ĵ������������� */
	/* ��ȡ��Ӧͨ������ʵֵ��У׼ֵ */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value;
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value;
			break;
		}
#endif	
		
		default:
		{
			/* ��ȡ��ͨ��Ŀ�겻���� */
			__nop();
			return;
		}
	}
	

	/* ���¶�Ӧ������ͨ����У׼���� */
	memcpy((u8*)sensor_cal_curve_data_p,Cal_Inf->Com_Data,Com_Sensor_Cal_Curve_Data_Len);
	memcpy((u8*)sensor_act_curve_data_p,&Cal_Inf->Com_Data[Com_Sensor_Cal_Curve_Data_Len],Com_Sensor_Cal_Curve_Data_Len);

	/* �޸� ���� ��Ϣ�� ���ñ�־ */
	Write_Config_Tab((u8*)sensor_act_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	
	/* �޸� ���� ��Ϣ�� ���ñ�־ */
	Write_Config_Tab((u8*)sensor_cal_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	
	/* ���� ��Ӧ����֡ ���� */
	write_curve_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_curve_ack.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	write_curve_ack.Cmd_Code	= Com_Sensor_Cal_Cmd_Code_Write_Curve_Data;
	write_curve_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	write_curve_ack.Data_Len	= Com_Sensor_Cal_Write_Curve_Data_Len;
	/* �����ݿ��������ͼĴ����� */
	write_curve_ack.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ���� ���� */
	write_curve_ack.Com_Data[1]	= Com_Sensor_Cal_Constant;	
	/* ���� У�� */
	write_curve_ack.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_curve_ack);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&write_curve_ack,Com_Sensor_Cal_Write_Curve_Packet_Len);
	
/* ������У׼ д���� ��Ӧ���ݳ��� *///���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+96(����)+1(����)+1(У���)
#undef Com_Sensor_Cal_Write_Curve_Data_Len
/* ������У׼ д���� ����֡���� */
#undef Com_Sensor_Cal_Write_Curve_Packet_Len
}

/************************************************************************************************/
/**	�� �� ��: Com_Sensor_Cal_Read_Curve_Data														*/
/**	����˵��: ����������Ӧͨ����������																*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Curve_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* ��������ʵֵ����ָ�� */
	u16* sensor_act_curve_data_p;
	/* ��������ʵֵ����ָ�� */
	u16* sensor_cal_curve_data_p;
	/* ���� ������������ ����֡ �Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF read_curve_ack;
	
/* ������У׼ ������ ��Ӧ���ݳ��� *///���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+32(����)+1(����)+1(У���)
#define Com_Sensor_Cal_Read_Curve_Data_Len		(Com_Sensor_Cal_Data_Buffer_Size)
/* ������У׼ ������ ����֡���� */
#define Com_Sensor_Cal_Read_Curve_Packet_Len	(Com_Sensor_Cal_Read_Curve_Data_Len+Com_Sensor_Cal_Fix_Data_Len)


	/* �жϻ�ȡ���� �ڼ�ͨ�� �Ĵ������������� */
	/* ��ȡ��Ӧͨ������ʵֵ��У׼ֵ */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			/* ��ȡ������ͨ��1��ʵֵ�������� */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value;
			/* ��ȡ������ͨ��1У׼ֵ�������� */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value;
			break;
		}
#endif	
		
		default:
		{
			/* ��ȡ��ͨ��Ŀ�겻���� */
			__nop();
			return;
		}
	}
	
	/* ���� ��Ӧ����֡ ���� */
	read_curve_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_curve_ack.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	read_curve_ack.Cmd_Code	 	= Com_Sensor_Cal_Cmd_Code_Read_Curve_Data;
	read_curve_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_curve_ack.Data_Len		= Com_Sensor_Cal_Read_Curve_Data_Len;

	/* �����ݿ��������ͼĴ����� */
	/* ����У׼ֵ��ǰ�棨16�ֽڣ�����ʵֵ�ź��棨16�ֽڣ� */
	memcpy(read_curve_ack.Com_Data,(u8*)sensor_cal_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	memcpy(&read_curve_ack.Com_Data[Com_Sensor_Cal_Curve_Data_Len],(u8*)sensor_act_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	/* ���� ���� */
	read_curve_ack.Com_Data[read_curve_ack.Data_Len]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	read_curve_ack.Com_Data[read_curve_ack.Data_Len+1]	= Com_Sensor_Cal_Get_Check(&read_curve_ack);
	/* ��Ӧ ʵʱ���� ������Ϣ *///���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+16(����)+1(����)+1(У���)
	USART_printf(USART1,(u8*)&read_curve_ack,Com_Sensor_Cal_Read_Curve_Packet_Len);
	

/* ������У׼ ������ ��Ӧ���ݳ��� *///���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+96(����)+1(����)+1(У���)
#undef Com_Sensor_Cal_Read_Curve_Data_Len
/* ������У׼ ������ ����֡���� */
#undef Com_Sensor_Cal_Read_Curve_Packet_Len
}


/************************************************************************************************/
/**	�� �� ��: Com_Config_SN_Write																*/
/**	����˵��: дSN��																				*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void static Com_Config_SN_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* дSN�� Ӧ������ݳ��� */
#define Com_Write_SN_NO_Data_Len	(1u) //���1����������
/* дSN�� Ӧ��Э�鳤�� */
#define Com_Write_SN_NO_Len			(Com_Write_SN_NO_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
	/* дSN�� Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF write_sn_no;
	

	/* ����SN�� */
	Flash_Write_SN_NO(Cal_Inf->Com_Data);
	
	memcpy(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number,Cal_Inf->Com_Data,sizeof(SERIAL_NUMBER));
				/* �޸� ���� ��Ϣ�� ���ñ�־ */
 	Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number,sizeof(SERIAL_NUMBER));
	
	

	/* ���� ʵʱ���� ���� */
	write_sn_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_sn_no.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	write_sn_no.Cmd_Code	= Com_Config_SN_Cmd_Code_Write;
	write_sn_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	write_sn_no.Data_Len	= Com_Write_SN_NO_Data_Len;
	/* �����ݿ��������ͼĴ����� */
	write_sn_no.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ���� ���� */
	write_sn_no.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	write_sn_no.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_sn_no);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&write_sn_no,Com_Write_SN_NO_Len);
	
	
#undef Com_Write_SN_NO_Data_Len	
#undef Com_Read_SN_NO_Len
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_SN_Read																	*/
/**	����˵��: ��SN��																				*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void static Com_Config_SN_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* �� SN ��Ӧ��Э�鳤�� */
#define Com_Read_SN_NO_Len	(ENV_SN_NO_SIZE+Com_Sensor_Cal_Fix_Data_Len)//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+6(����)+1(����)+1(У���)
	/* �� SN ��Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF read_sn_no;
	

	/* ���� ʵʱ���� ���� */
	read_sn_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_sn_no.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	read_sn_no.Cmd_Code	 	= Com_Config_SN_Cmd_Code_Read;
	read_sn_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	read_sn_no.Data_Len		= ENV_SN_NO_SIZE;
////	/* ��ȡSN�� */
////	Flash_Read_SN_NO(read_sn_no.Com_Data);
	
	memcpy(read_sn_no.Com_Data,&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number,sizeof(SERIAL_NUMBER));
	
	
	
	/* ���� ���� */
	read_sn_no.Com_Data[ENV_SN_NO_SIZE] 		= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	read_sn_no.Com_Data[ENV_SN_NO_SIZE + 1] 	= Com_Sensor_Cal_Get_Check(&read_sn_no);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&read_sn_no,Com_Read_SN_NO_Len);
#undef Com_Read_SN_NO_Len
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Sensor_Write															*/
/**	����˵��: д����������																			*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Sensor_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* д���������� ���ݳ��� */
#define Com_Write_Sensor_Data_Len		(1u) //���1����������
/* д���������� ����֡���� */
#define Com_Write_Sensor_Len			(Com_Write_Sensor_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
/* ������ */
#define Write_Sensor_Data_Len			(sizeof(INSTRU_SENSOR_Old))
/* ������ */
#define Write_Sensor_Para_Data_Len		(sizeof(INSTRU_SENSOR_CHANNEL_FIX))
/* ������ͨ��ָ�� */
#define Sensor_Cha_P(cha)				(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[(cha-1)*sizeof(INSTRU_SENSOR_CHANNEL)])
	
	/* д���������üĴ��� */
	COM_SENSOR_CAL_TYPEDEF write_sensor_config;
	/*  */
	INSTRU_SENSOR_CHANNEL_FIX	*sensor_para_p;
	/* ������ͨ�����ݼĴ���. */
	INSTRU_SENSOR_Old			sensor_data;


	/* ��ȡ��Ӧͨ������ʵֵ��У׼ֵ */
	switch(Cal_Inf->Chanel_Num-1)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			break;
		}
#endif	
		
		default:
		{
			/* ��ȡ��ͨ��Ŀ�겻���� */
			__nop();
			return;
		}
	}
	
	/* ����������� */
	memset((u8*)&sensor_data,0x00,sizeof(INSTRU_SENSOR_Old));

	/* �����ݿ��������ͼĴ����� */
	memcpy((u8*)&sensor_data,Cal_Inf->Com_Data,Write_Sensor_Data_Len);
	
	/* ����õĴ���������ת�Ƶ����ñ��� */
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_RF_Card_ID_NO = sensor_data.SENSOR_RF_Card_ID_NO;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[0] 	= sensor_data.SENSOR_SN_NO[0];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[1] 	= sensor_data.SENSOR_SN_NO[1];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[2] 	= sensor_data.SENSOR_SN_NO[2];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[3] 	= sensor_data.SENSOR_SN_NO[3];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Up_Limit 	= sensor_data.SENSOR_Up_Limit;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Low_Limit 	= sensor_data.SENSOR_Low_Limit;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->Type 				= sensor_data.Type;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_Switch 		= (sensor_data.SENSOR_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Sound_Alarm_Switch = (sensor_data.SENSOR_Sound_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Light_Alarm_Switch = (sensor_data.SENSOR_Light_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SMS_Alarm_Switch 	= (sensor_data.SENSOR_SMS_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_State =  0;
			
	/* �޸� ���� ��Ϣ�� ���ñ�־ */
	Write_Config_Tab((u8*)Sensor_Cha_P(Cal_Inf->Chanel_Num),sizeof(INSTRU_SENSOR_CHANNEL));

	/* ���ô�������صĲ���ָ�� */
	sensor_para_p = (INSTRU_SENSOR_CHANNEL_FIX*)&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[(Cal_Inf->Chanel_Num-1)*Write_Sensor_Para_Data_Len];

	/* �������ָ�� */
	sensor_para_p->Sensor_Up_Alarm_Cnt = 0;	
	sensor_para_p->Sensor_Low_Alarm_Cnt = 0;
	sensor_para_p->Sensor_Suspend_Alarm_Flag = FALSE;

	/* ���� ʵʱ���� ���� */
	write_sensor_config.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_sensor_config.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	write_sensor_config.Cmd_Code	= Com_Config_Sensor_Cmd_Code_Write;
	write_sensor_config.Chanel_Num	= Cal_Inf->Chanel_Num;
	write_sensor_config.Data_Len	= 1;
	/* �����ݿ��������ͼĴ����� */
	write_sensor_config.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ���� ���� */
	write_sensor_config.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	write_sensor_config.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_sensor_config);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&write_sensor_config,Com_Write_Sensor_Len);
	
	
#undef Com_Write_Sensor_Data_Len
#undef Com_Write_Sensor_Len
#undef Write_Sensor_Data_Len
#undef Sensor_Cha_P
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Sensor_Read																*/
/**	����˵��: ������������																			*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Sensor_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* ���������� ���ݳ��� */
#define Com_Read_Sensor_Data_Len	(sizeof(INSTRU_SENSOR_Old))
/* ���������� ����֡���� */
#define Com_Read_Sensor_Len			  (Com_Read_Sensor_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
/* ������ͨ��ָ�� */
#define Sensor_Cha_P(cha)			    (&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[(cha-1)*sizeof(INSTRU_SENSOR_CHANNEL)])

	
	/* д���������üĴ��� */
	COM_SENSOR_CAL_TYPEDEF read_sensor_config;
	/* ������ͨ�����ݼĴ���. */
	INSTRU_SENSOR_Old			sensor_data;


	/* ��ȡ��Ӧͨ������ʵֵ��У׼ֵ */
	switch(Cal_Inf->Chanel_Num-1)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			break;
		}
#endif	
		
		default:
		{
			/* ��ȡ��ͨ��Ŀ�겻���� */
			__nop();
			return;
		}
	}
	
	/* ����������� */
	memset((u8*)&sensor_data,0x00,sizeof(INSTRU_SENSOR_Old));
	
	/* ���� ʵʱ���� ���� */
	read_sensor_config.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_sensor_config.Instru_NO 	= Cal_Inf->Instru_NO;
	read_sensor_config.Cmd_Code	 	= Com_Config_Sensor_Cmd_Code_Read;
	read_sensor_config.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_sensor_config.Data_Len		= Com_Read_Sensor_Data_Len;
	
	/* ����õĴ���������ת�Ƶ����ñ��� */	
	sensor_data.SENSOR_RF_Card_ID_NO 	= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_RF_Card_ID_NO;
	sensor_data.SENSOR_SN_NO[0] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[0];
	sensor_data.SENSOR_SN_NO[1] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[1];
	sensor_data.SENSOR_SN_NO[2] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[2];
	sensor_data.SENSOR_SN_NO[3] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[3];
	sensor_data.SENSOR_Up_Limit 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Up_Limit;
	sensor_data.SENSOR_Low_Limit 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Low_Limit;
	sensor_data.Type 					= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->Type;
	sensor_data.SENSOR_Alarm_Switch 	= (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_Sound_Alarm_Switch = (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Sound_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_Light_Alarm_Switch = (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Light_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_SMS_Alarm_Switch   = (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SMS_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_Alarm_State 		  = ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_State;
	
	/* �����ݿ��������ͼĴ����� */
	memcpy(read_sensor_config.Com_Data,(u8 *)&sensor_data,Com_Read_Sensor_Data_Len);
	/* ���� ���� */
	read_sensor_config.Com_Data[Com_Read_Sensor_Data_Len]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	read_sensor_config.Com_Data[Com_Read_Sensor_Data_Len+1]	= Com_Sensor_Cal_Get_Check(&read_sensor_config);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&read_sensor_config,Com_Read_Sensor_Len);

#undef Com_Read_Sensor_Data_Len
#undef Com_Read_Sensor_Len
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Card_NO_Write															*/
/**	����˵��: д��ӡ���ƺ�																		*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF											*/
/**	�� �� ֵ: ��																				*/
/************************************************************************************************/
static void Com_Config_Card_NO_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* д��ӡ���ƺ� Ӧ������ݳ��� */
#define Com_Write_Card_NO_Data_Len	(1u) //���1����������
/* д��ӡ���ƺ� Ӧ��Э�鳤�� */
//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
#define Com_Write_Card_NO_Len		(Com_Write_Card_NO_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* ��ӡ���ƺŵ����ݵ�ַ */
#define Card_NO_Data_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO)

	/* д��ӡ���ƺ� Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF write_card_no;
	

	/* ���ô�ӡ���ƺ� */
	/* ������������� */
	memset(Card_NO_Data_P,'\0',Print_Card_NO_Size);
	/* �����ݿ��������ͼĴ����� */
	memcpy(Card_NO_Data_P,Cal_Inf->Com_Data,(Cal_Inf->Data_Len >(9u))? (9u):Cal_Inf->Data_Len);
	/* ���� ��ӡ���ƺ� �������� */
	Write_Config_Tab((u8*)Card_NO_Data_P,sizeof(Card_NO_Data_P));

	/* ���� ʵʱ���� ���� */
	write_card_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_card_no.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	write_card_no.Cmd_Code		= Com_Config_Card_NO_Cmd_Code_Write;
	write_card_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	write_card_no.Data_Len		= Com_Write_Card_NO_Data_Len;
	/* �����ݿ��������ͼĴ����� */
	write_card_no.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ���� ���� */
	write_card_no.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	write_card_no.Com_Data[2] 	= Com_Sensor_Cal_Get_Check(&write_card_no);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&write_card_no,Com_Write_Card_NO_Len);
	
	
#undef Com_Write_Card_NO_Data_Len	
#undef Com_Write_Card_NO_Len
#undef Card_NO_Data_P
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Card_NO_Read																	*/
/**	����˵��: ��SN��																				*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Card_NO_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* ��ӡ���ƺŵ����ݵ�ַ */
#define Card_NO_Data_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO)
	/* ����ӡ���ƺ�Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF read_card_no;
	

	/* ���� ʵʱ���� ���� */
	read_card_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_card_no.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	read_card_no.Cmd_Code	= Com_Config_Card_NO_Cmd_Code_Read;
	read_card_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	read_card_no.Data_Len	= strlen(Card_NO_Data_P);
	/* �����ݿ��������ͼĴ����� */
	memcpy(read_card_no.Com_Data,Card_NO_Data_P,read_card_no.Data_Len);
 
	/* ���� ���� */
	*((u8*)&(read_card_no.Com_Data[read_card_no.Data_Len])) = Com_Sensor_Cal_Constant;
	/* ���� У�� */
	*((u8*)&(read_card_no.Com_Data[read_card_no.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&read_card_no);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&read_card_no,(read_card_no.Data_Len + Com_Sensor_Cal_Fix_Data_Len));
	

#undef Card_NO_Data_P
}


/************************************************************************************************/
/**	�� �� ��: Com_Config_Company_Name_Read														*/
/**	����˵��: ������ ��ӡ��˾����																*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF											*/
/**	�� �� ֵ: ��																				*/
/************************************************************************************************/
static void Com_Config_Company_Name_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* ��ӡ ��˾���� �����ݵ�ַ */
#define Card_Cpy_Name_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name)
	/* ����ӡ���ƺ�Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF read_cpy_name;
	

	/* ���� ʵʱ���� ���� */
	read_cpy_name.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_cpy_name.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	read_cpy_name.Cmd_Code		= Com_Config_Company_Name_Cmd_Code_Read;
	read_cpy_name.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	read_cpy_name.Data_Len		= strlen(Card_Cpy_Name_P);
	/* �����ݿ��������ͼĴ����� */
	memcpy(read_cpy_name.Com_Data,Card_Cpy_Name_P,read_cpy_name.Data_Len);
	
	/* ���� ���� */
	*((u8*)&(read_cpy_name.Com_Data[read_cpy_name.Data_Len])) = Com_Sensor_Cal_Constant;
	
	/* ���� У�� */
	*((u8*)&(read_cpy_name.Com_Data[read_cpy_name.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&read_cpy_name);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&read_cpy_name,read_cpy_name.Data_Len + Com_Sensor_Cal_Fix_Data_Len);
	

#undef Card_Cpy_Name_P
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Company_Name_Write														*/
/**	����˵��: д���� ��ӡ��˾����																*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF											*/
/**	�� �� ֵ: ��																				*/
/************************************************************************************************/
static void Com_Config_Company_Name_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* д��˾���� Ӧ������ݳ��� */
#define Com_Write_Cpy_Name_Data_Len	(1u) //���1����������
/* д��˾���� Ӧ��Э�鳤�� */
//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
#define Com_Write_Cpy_Name_Len		(Com_Write_Cpy_Name_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* ��ӡ��˾���Ƶ����ݵ�ַ */
#define Cpy_Name_Data_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name)

	
	/* д��ӡ��˾���� Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF write_cpy_name;
	

	/* ���ô�ӡ��˾���� */
	/* ������������� */
	memset(Cpy_Name_Data_P,'\0',Print_Company_Name_Size);
	/* �����ݿ��������ͼĴ����� */
	memcpy(Cpy_Name_Data_P,Cal_Inf->Com_Data,(Cal_Inf->Data_Len > (Print_Company_Name_Size - 1)) ? (Print_Company_Name_Size - 1):Cal_Inf->Data_Len);
	/* ���湫˾�������� */
	Write_Config_Tab((u8*)Cpy_Name_Data_P,sizeof(Cpy_Name_Data_P));

	/* ���� ʵʱ���� ���� */
	write_cpy_name.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_cpy_name.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	write_cpy_name.Cmd_Code		= Com_Config_Company_Name_Cmd_Code_Write;
	write_cpy_name.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	write_cpy_name.Data_Len		= Com_Write_Cpy_Name_Data_Len;
	/* �����ݿ��������ͼĴ����� */
	write_cpy_name.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ���� ���� */
	write_cpy_name.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	write_cpy_name.Com_Data[2] 	= Com_Sensor_Cal_Get_Check(&write_cpy_name);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&write_cpy_name,Com_Write_Cpy_Name_Len);
	
	
#undef Com_Write_Cpy_Name_Data_Len
#undef Com_Write_Cpy_Name_Len
#undef Cpy_Name_Data_P
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Fad_Data_Write															*/
/**	����˵��: д������ͨ������������ֵ																*/
/**	��    �Σ�������ͨ������������Ϣ�壺COM_SENSOR_CAL_TYPEDEF										*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Fad_Data_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* ������ͨ����������ָ�� */
#define Fad_Up_Lmt_Data_P(ch)			(&(((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Fraud_Up_Limit))
/* ������ͨ����������ָ�� */
#define Fad_Low_Lmt_Data_P(ch)			(&(((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Fraud_Low_Limit))
/* ���ݳ��� */
#define Com_Write_Fad_Data_Len			(1u)
/* Ӧ��Э�鳤�� */
//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
#define Com_Write_Fad_Data_Func_Len		(Com_Write_Fad_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* ��Ϣ�����ݳ��� */
#define Com_Inf_Data_Len				(8u)
/* ������ͨ�����޲��� */
#define Sen_Up_Lmt_Data(ch)				(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Up_Limit)
/* ������ͨ�����޲��� */
#define Sen_Low_Lmt_Data(ch)			(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Low_Limit)


	/* ������ͨ���������� Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF write_fad_data_func;

	/* ���ݺϷ��Ա�־ */
	BOOL data_vldy = TRUE;
	/* �������������ݲ��� */
	float* fad_up_lmt = (float*)Cal_Inf->Com_Data;
	float* fad_low_lmt = (float*)(&Cal_Inf->Com_Data[sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)))]);

	/* �ж����õ������Ƿ�Ϸ� */
	/* �ж�ͨ���Ƿ�Ϸ� */
	/* ��ȡͨ��������� */
	data_vldy = Log_Com_Sensor_CH_Check((Cal_Inf->Chanel_Num-1));
	
	/* �ж��Ƿ�Ϸ� */
	if((data_vldy == TRUE)&&(Cal_Inf->Data_Len != Com_Inf_Data_Len))
	{
		/* ���ݳ��Ȳ��Ϸ� */
		data_vldy = FALSE;
	}
	
	/* �жϲ����Ϸ��� */
	if(((data_vldy == TRUE)&&(*fad_up_lmt < *fad_low_lmt))||
	   (*fad_up_lmt > 1000)||(*fad_low_lmt < -1000))
	{
		/* �������Ϸ� */
		data_vldy = FALSE;
		
	}else
	{
		/* ���ݳ��ȺϷ� */
		/* �������޴������� */
		if((*fad_low_lmt > Sen_Up_Lmt_Data((Cal_Inf->Chanel_Num-1)))||(*fad_up_lmt < Sen_Low_Lmt_Data((Cal_Inf->Chanel_Num-1))))
		{
			/* ���������޲��ڴ����������޷�Χ�� */
			/* �������Ϸ� */
			data_vldy = FALSE;
		}
	}
		
	/* �ж����ݺϷ��Ա�־ */
	if(data_vldy == TRUE)
	{
		/* �����Ϸ� */
		/* �������״����������޲��� */
		/* ��������	*/
		memcpy(Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),Cal_Inf->Com_Data,sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));
		/* �������� */
		memcpy(Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),&Cal_Inf->Com_Data[sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)))],sizeof(*Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));

		/* ������� */
		/* �޸� ���� ��Ϣ�� ���ñ�־ */
		Write_Config_Tab((u8*)Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));
		
		/* �޸� ���� ��Ϣ�� ���ñ�־ */
		Write_Config_Tab((u8*)Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),sizeof(*Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));
	}


	/* ���� ʵʱ���� ���� */
	write_fad_data_func.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_fad_data_func.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	write_fad_data_func.Cmd_Code	= Com_Config_Fad_Data_Cmd_Code_Write;
	write_fad_data_func.Chanel_Num	= Cal_Inf->Chanel_Num;
	write_fad_data_func.Data_Len	= Com_Write_Fad_Data_Len;
	
	/* �����ݿ��������ͼĴ����� */
	write_fad_data_func.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ���� ���� */
	write_fad_data_func.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	write_fad_data_func.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_fad_data_func);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&write_fad_data_func,Com_Write_Fad_Data_Func_Len);
	

#undef Fad_Up_Lmt_Data_P
#undef Fad_Low_Lmt_Data_P
#undef Com_Write_Fad_Data_Len
#undef Com_Write_Fad_Data_Func_Len
#undef Com_Inf_Data_Len
#undef Sen_Up_Lmt_Data
#undef Sen_Low_Lmt_Data
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Fad_Data_Read															*/
/**	����˵��: ��������ͨ������������ֵ																*/
/**	��    �Σ�������ͨ������������Ϣ�壺COM_SENSOR_CAL_TYPEDEF										*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Fad_Data_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* ������ͨ������ָ�� */
#define Fad_Data_P(ch)			((char*)(&(((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Fraud_Up_Limit)))

/* ���ݳ��� */
#define Com_Read_Fad_Data_Len	(8u)

/* ��Ϣ�����ݳ��� */
#define Com_Inf_Data_Len		(0u)

/* Ӧ��Э�鳤�� */
//���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+8(���ݳ���)+1(����)+1(����)+1(У���)
#define Com_Read_Fad_Len		(Com_Read_Fad_Data_Len+Com_Sensor_Cal_Fix_Data_Len)


	/* ������ͨ���������� Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF 		read_fad_data_func;

	/* ���ݺϷ��Ա�־ */
	BOOL data_vldy = TRUE;

	/* �ж����õ������Ƿ�Ϸ� */
	/* �ж�ͨ���Ƿ�Ϸ� */
	/* ��ȡͨ��������� */
	data_vldy = Log_Com_Sensor_CH_Check((Cal_Inf->Chanel_Num-1));

	/* �ж��Ƿ�Ϸ� */
	if((data_vldy == TRUE)&&(Cal_Inf->Data_Len != Com_Inf_Data_Len))
	{
		/* ���ݳ��Ȳ��Ϸ� */
		data_vldy = FALSE;
	}

	/* ���� ʵʱ���� ���� */
	read_fad_data_func.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_fad_data_func.Instru_NO 	= Cal_Inf->Instru_NO;//�������
	read_fad_data_func.Cmd_Code		= Com_Config_Fad_Data_Cmd_Code_Read;
	read_fad_data_func.Chanel_Num	= Cal_Inf->Chanel_Num;
	
	if(data_vldy == TRUE)
	{
		/* �������ݳ��� */
		read_fad_data_func.Data_Len		= Com_Read_Fad_Data_Len;

		/* �����ݿ��������ͼĴ����� */
		memcpy(read_fad_data_func.Com_Data,Fad_Data_P((Cal_Inf->Chanel_Num-1)),Com_Read_Fad_Data_Len);
		
		/* ���� ���� */
		read_fad_data_func.Com_Data[Com_Read_Fad_Data_Len]		= Com_Sensor_Cal_Constant;
		
		/* ���� У�� */
		read_fad_data_func.Com_Data[Com_Read_Fad_Data_Len+1]	= Com_Sensor_Cal_Get_Check(&read_fad_data_func);	
	}else
	{
		/* �������ݳ��� */
		read_fad_data_func.Data_Len		= 1;
		
		/* ���� ���� */
		read_fad_data_func.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Fail;
		
		/* ���� ���� */
		read_fad_data_func.Com_Data[1]	= Com_Sensor_Cal_Constant;
		
		/* ���� У�� */
		read_fad_data_func.Com_Data[1]	= Com_Sensor_Cal_Get_Check(&read_fad_data_func);
	}

	
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&read_fad_data_func,Com_Read_Fad_Len);


#undef Fad_Data_P
#undef Com_Read_Fad_Data_Len
#undef Com_Inf_Data_Len
#undef Com_Read_Fad_Len
}


/************************************************************************************************/
/**	�� �� ��: Com_Config_DataNetwork_Read														*/
/**	����˵��: �����ݷ����������ò���																*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_DataNetwork_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
	/* ��������ָ�� */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;
	/* �����ַ������� */
	u32	domainNameLen;
	
	
	/* ���þֲ�������ʼֵ */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* ���������ַ������� */
	domainNameLen = strlen((char*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1);
	/* �ж����������ݳ��� */
	if(domainNameLen > 31u)
	{
		/* ����31��������Ϊ���31 */
		domainNameLen = 31u;
	}
	
	/* �������ݷ��������� */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= ComConfigDataNetworkRead;
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 38u;

	/* ���÷����������� */
	/* ����IP */
	networkDatap->IP.Server_Address = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address;
	/* ���ö˿� */
	networkDatap->Port = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1;
	/* �������� */
	memcpy(networkDatap->DomainName,
		   Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
		   domainNameLen);
		   
	/* ���� ���� */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* ���� У�� */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}



/************************************************************************************************/
/**	�� �� ��: Com_Config_DataNetwork_Read														*/
/**	����˵��: �����÷����������ò���																*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Network_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
	/* ��������ָ�� */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;
	/* �����ַ������� */
	u32	domainNameLen;
	
	
	/* ���þֲ�������ʼֵ */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* ���������ַ������� */
	domainNameLen = strlen((char*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2);
	/* �ж����������ݳ��� */
	if(domainNameLen > 31u)
	{
		/* ����31��������Ϊ���31 */
		domainNameLen = 31u;
	}
	
	/* �������ݷ��������� */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= ComConfigNetworkRead;//�����÷�����������
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 38u;

	/* ���÷����������� */
	/* ����IP */
	networkDatap->IP.Server_Address = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Server_Address;
	/* ���ö˿� */
	networkDatap->Port = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2;
	/* �������� */
	memcpy(networkDatap->DomainName,
		   Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
		   domainNameLen);
		   
	/* ���� ���� */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* ���� У�� */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}
/************************************************************************************************/
/**	�� �� ��: Com_Config_Upgrade_Network_Read											*/
/**	����˵��: �����������������ò���																*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Upgrade_Network_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
	/* ��������ָ�� */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;
	/* �����ַ������� */
	u32	domainNameLen;
	
	
	/* ���þֲ�������ʼֵ */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* ���������ַ������� */
	domainNameLen = strlen((char*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3);
	/* �ж����������ݳ��� */
	if(domainNameLen > 31u)
	{
		/* ����31��������Ϊ���31 */
		domainNameLen = 31u;
	}
	
	/* �������ݷ��������� */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= ComConfigUpgradeNetworkRead;//������������������
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 38u;

	/* ���÷����������� */
	/* ����IP */
	networkDatap->IP.Server_Address = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Server_Address;
	/* ���ö˿� */
	networkDatap->Port = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3;
	/* �������� */
	memcpy(networkDatap->DomainName,
		   Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
		   domainNameLen);
		   
	/* ���� ���� */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* ���� У�� */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}

/************************************************************************************************/
/**	�� �� ��: Com_Config_DataNetwork_Write														*/
/**	����˵��: д���ݷ����������ò���																*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_DataNetwork_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* ��������ָ�� */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)Cal_Inf->Com_Data;
	/* Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	/* ���ݺϷ��Ա�־ */
	BOOL data_vldy = TRUE;
	/* �����ַ������� */
	u32	domainNameLen;
	
	/* �Ϸ��Լ�� */
	if(data_vldy == TRUE)
	{
		/* ����������ݳ��� */
		domainNameLen = strlen((char*)networkDatap->DomainName);
		
		/* �ж����������Ƿ���� */
		if(domainNameLen > 31u)
		{
			/* �������ݳ��Ȳ����� */
			data_vldy = FALSE;
		}
	}
	
	
	/* �ж����ݺϷ��Ա�־ */
	if(data_vldy == TRUE)
	{
		/* ��ȡ�������� */
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1 = networkDatap->IP;
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1 = networkDatap->Port;
		/* ������������Ĵ��� */
		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,'\0',32);
		memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,networkDatap->DomainName,domainNameLen);

		/* �޸��������ñ��ж�ӦIP������ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address));
		/* �޸��������ñ��ж�Ӧ�˿ڵ����� */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1));
		/* �޸��������ñ��ж�Ӧ���������� */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));	
	}
	
	/* ���� ʵʱ���� ���� */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	= ComConfigDataNetworkWrite;
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	= (1u);
	
	/* �����ݿ��������ͼĴ����� */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ���� ���� */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}



/************************************************************************************************/
/**	�� �� ��: Com_Config_DataNetwork_Write														*/
/**	����˵��: д���÷����������ò���																*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Network_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* ��������ָ�� */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)Cal_Inf->Com_Data;
	/* Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	/* ���ݺϷ��Ա�־ */
	BOOL data_vldy = TRUE;
	/* �����ַ������� */
	u32	domainNameLen;
	
	/* �Ϸ��Լ�� */
	if(data_vldy == TRUE)
	{
		/* ����������ݳ��� */
		domainNameLen = strlen((char*)networkDatap->DomainName);
		
		/* �ж����������Ƿ���� */
		if(domainNameLen > 31u)
		{
			/* �������ݳ��Ȳ����� */
			data_vldy = FALSE;
		}
	}
	
	
	/* �ж����ݺϷ��Ա�־ */
	if(data_vldy == TRUE)
	{
		/* ��ȡ�������� */
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2 = networkDatap->IP;
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2 = networkDatap->Port;
		/* ������������Ĵ��� */
		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,'\0',32);
		memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,networkDatap->DomainName,domainNameLen);

		/* �޸��������ñ��ж�ӦIP������ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Server_Address,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Server_Address));
		/* �޸��������ñ��ж�Ӧ�˿ڵ����� */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2));
		/* �޸��������ñ��ж�Ӧ���������� */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));	
	}
	
	/* ���� ʵʱ���� ���� */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	= ComConfigNetworkWrite;//���÷�����������
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	= (1u);
	
	/* �����ݿ��������ͼĴ����� */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ���� ���� */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}


/************************************************************************************************/
/**	�� �� ��: Com_Config_Upgrade_Network_Write												*/
/**	����˵��: д���������������ò���																*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Upgrade_Network_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* ��������ָ�� */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)Cal_Inf->Com_Data;
	/* Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	/* ���ݺϷ��Ա�־ */
	BOOL data_vldy = TRUE;
	/* �����ַ������� */
	u32	domainNameLen;
	
	/* �Ϸ��Լ�� */
	if(data_vldy == TRUE)
	{
		/* ����������ݳ��� */
		domainNameLen = strlen((char*)networkDatap->DomainName);
		
		/* �ж����������Ƿ���� */
		if(domainNameLen > 31u)
		{
			/* �������ݳ��Ȳ����� */
			data_vldy = FALSE;
		}
	}
	
	
	/* �ж����ݺϷ��Ա�־ */
	if(data_vldy == TRUE)
	{
		/* ��ȡ�������� */
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3 = networkDatap->IP;
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3 = networkDatap->Port;
		/* ������������Ĵ��� */
		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,'\0',32);
		memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,networkDatap->DomainName,domainNameLen);

		/* �޸��������ñ��ж�ӦIP������ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Server_Address,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Server_Address));
		/* �޸��������ñ��ж�Ӧ�˿ڵ����� */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3));
		/* �޸��������ñ��ж�Ӧ���������� */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));	
	}
	
	/* ���� ʵʱ���� ���� */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	= ComConfigUpgradeNetworkWrite;//д����������������
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	= (1u);
	
	/* �����ݿ��������ͼĴ����� */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ���� ���� */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ���� У�� */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* ��Ӧ ʵʱ���� ������Ϣ */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}












































/************************************************************************************************/
/**	�� �� ��: Com_Config_SMSAamPoeNOSet												*/
/**	����˵��: ���ö��ű�������															*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
///************************************************************************************************/
//static void Com_Config_SMSAamPoeNOSet (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
/* ���ű�����������ָ��. */
#define SMS_Aam_Poe_NO_P_1(num) (((MOBILE_NO*)(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1))+num)
static void Com_Config_SMSAamPoeNOSet (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	/* д�����ֻ��� Ӧ������ݳ��� */
  #define Com_Write_SMSAamPoeNOSet_Data_Len	(1u) //���1����������
	
	#define Com_Write_SMSAamPoeNOSet_Len	(Com_Write_SMSAamPoeNOSet_Data_Len+Com_Config_SMSAamPoeNO_Fix_Data_Len) //���ݳ��ȣ�[1(ͷ��)+1(�豸��ַ)+1(������)+1(ͨ����)]+1(���ݳ���)+1(����)+1(����)+1(У���)
	
	
	   MOBILE_NO *sms_no_p;//�ֻ� ���� �ṹ��
		/* д�����ֻ��� Ӧ��Э��Ĵ��� */
	COM_SENSOR_CAL_TYPEDEF      write_SMSAamPoeNOSet;//���մ�������
/**********************������λ��ָ��*****************************************/
	sms_no_p = SMS_Aam_Poe_NO_P_1(Cal_Inf->Chanel_Num-1);//�����ֻ�����λ�õ�ַ������0:�������һ,����1:������� ��,����2:���������;
	if(Cal_Inf->Data_Len==Check_SMSAamPhone_Len)
	{/* �����õĶ��ź���д�����ݼĴ��� */
		sms_no_p->Serial_Number_Number.Number_1  = (Cal_Inf->Com_Data[0]-0x30);
		sms_no_p->Serial_Number_Number.Number_2  = (Cal_Inf->Com_Data[1]-0x30);
		sms_no_p->Serial_Number_Number.Number_3  = (Cal_Inf->Com_Data[2]-0x30);
		sms_no_p->Serial_Number_Number.Number_4  = (Cal_Inf->Com_Data[3]-0x30);
		sms_no_p->Serial_Number_Number.Number_5  = (Cal_Inf->Com_Data[4]-0x30);
		sms_no_p->Serial_Number_Number.Number_6  = (Cal_Inf->Com_Data[5]-0x30);
		sms_no_p->Serial_Number_Number.Number_7  = (Cal_Inf->Com_Data[6]-0x30);
		sms_no_p->Serial_Number_Number.Number_8  = (Cal_Inf->Com_Data[7]-0x30);
		sms_no_p->Serial_Number_Number.Number_9  = (Cal_Inf->Com_Data[8]-0x30);
		sms_no_p->Serial_Number_Number.Number_10 = (Cal_Inf->Com_Data[9]-0x30);
		sms_no_p->Serial_Number_Number.Number_11 = (Cal_Inf->Com_Data[10]-0x30);
		/* �޸� ���� ��Ϣ�� ���ñ�־ */
		Write_Config_Tab((u8*)sms_no_p,sizeof(MOBILE_NO));
/***********************��Ӧ��λ��ָ��****************************************/
   /* ���� ʵʱ���� ���� */
    write_SMSAamPoeNOSet.Head_Code =Com_Sensor_Cal_Rece_Head_Code;//ͷ��
    write_SMSAamPoeNOSet.Instru_NO =Cal_Inf->Instru_NO;//�������
    write_SMSAamPoeNOSet.Cmd_Code  =Com_Config_SMSAamPoeNO_Cmd_Code_Write;//д�����ֻ����������
		write_SMSAamPoeNOSet.Chanel_Num=Cal_Inf->Chanel_Num;//��Ӧ�ĺ������
		write_SMSAamPoeNOSet.Data_Len  =0x01;
		write_SMSAamPoeNOSet.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/
		write_SMSAamPoeNOSet.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	  write_SMSAamPoeNOSet.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_SMSAamPoeNOSet);/* ���� У�� */
   /* ��Ӧ ʵʱ���� ������Ϣ */
  	USART_printf(USART1,(u8*)&write_SMSAamPoeNOSet,Com_Write_SMSAamPoeNOSet_Len);
	}
	else
	{
		
	/***********************��Ӧ��λ��ָ��****************************************/
   /* ���� ʵʱ���� ���� */
    write_SMSAamPoeNOSet.Head_Code =Com_Sensor_Cal_Rece_Head_Code;//ͷ��
    write_SMSAamPoeNOSet.Instru_NO =Cal_Inf->Instru_NO;//�������
    write_SMSAamPoeNOSet.Cmd_Code  =Com_Config_SMSAamPoeNO_Cmd_Code_Write;//д�����ֻ����������
		write_SMSAamPoeNOSet.Chanel_Num=Cal_Inf->Chanel_Num;//��Ӧ�ĺ������
		write_SMSAamPoeNOSet.Data_Len  =0x01;
		write_SMSAamPoeNOSet.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*����ʧ�ܱ�־*/
		write_SMSAamPoeNOSet.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	  write_SMSAamPoeNOSet.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_SMSAamPoeNOSet);/* ���� У�� */
   /* ��Ӧ ʵʱ���� ������Ϣ */
  	USART_printf(USART1,(u8*)&write_SMSAamPoeNOSet,Com_Write_SMSAamPoeNOSet_Len);
	
	}

}
/************************************************************************************************/
/**	�� �� ��: Com_Config_SMSAamPoeNORead												*/
/**	����˵��:��ȡ���ű�������															*/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
///************************************************************************************************/
static void Com_Config_SMSAamPoeNORead (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	
       MOBILE_NO *sms_no_p;//�ֻ� ���� �ṹ�� 
       COM_SENSOR_CAL_TYPEDEF      read_SMSAamPoeNOSet;//���ʹ�������(�����ֻ�����Ϣ)
/***********************��Ӧ��λ��ָ��****************************************/
     /* ���� ʵʱ���� ���� */
sms_no_p = SMS_Aam_Poe_NO_P_1(Cal_Inf->Chanel_Num-1);//��λ��Ҫ�����ֻ�����λ�õ�ַ������0:�������һ,����1:������� ��,����2:���������;	

	    read_SMSAamPoeNOSet.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	    read_SMSAamPoeNOSet.Instru_NO=Cal_Inf->Instru_NO;//�������
	    read_SMSAamPoeNOSet.Cmd_Code =Com_Config_SMSAamPoeNO_Cmd_Code_Read;//�������ֻ����������
			read_SMSAamPoeNOSet.Chanel_Num=Cal_Inf->Chanel_Num;//��Ӧ�ĺ������
	    read_SMSAamPoeNOSet.Data_Len=Check_SMSAamPhone_Len_Read;//�ֻ����볤��;Ĭ��Ϊ11λ
	                                     
	
      read_SMSAamPoeNOSet.Com_Data[0]=(sms_no_p->Serial_Number_Number.Number_1+0x30);
	    read_SMSAamPoeNOSet.Com_Data[1]=(sms_no_p->Serial_Number_Number.Number_2+0x30);
      read_SMSAamPoeNOSet.Com_Data[2]=(sms_no_p->Serial_Number_Number.Number_3+0x30);
	    read_SMSAamPoeNOSet.Com_Data[3]=(sms_no_p->Serial_Number_Number.Number_4+0x30);
	    read_SMSAamPoeNOSet.Com_Data[4]=(sms_no_p->Serial_Number_Number.Number_5+0x30);
	    read_SMSAamPoeNOSet.Com_Data[5]=(sms_no_p->Serial_Number_Number.Number_6+0x30);
      read_SMSAamPoeNOSet.Com_Data[6]=(sms_no_p->Serial_Number_Number.Number_7+0x30);
	    read_SMSAamPoeNOSet.Com_Data[7]=(sms_no_p->Serial_Number_Number.Number_8+0x30);
	    read_SMSAamPoeNOSet.Com_Data[8]=(sms_no_p->Serial_Number_Number.Number_9+0x30);
      read_SMSAamPoeNOSet.Com_Data[9]=(sms_no_p->Serial_Number_Number.Number_10+0x30);
	    read_SMSAamPoeNOSet.Com_Data[10]=(sms_no_p->Serial_Number_Number.Number_11+0x30);
	    read_SMSAamPoeNOSet.Com_Data[11]=0;
	    read_SMSAamPoeNOSet.Com_Data[12]=0;
	    read_SMSAamPoeNOSet.Com_Data[13]=0;
	    read_SMSAamPoeNOSet.Com_Data[14]=0;
			read_SMSAamPoeNOSet.Com_Data[15]=Com_Sensor_Cal_Constant;/* ���� ���� */;
			read_SMSAamPoeNOSet.Com_Data[16]=Com_Sensor_Cal_Get_Check(&read_SMSAamPoeNOSet);/* ���� У�� */;

	  /* ��Ӧ ʵʱ���� ������Ϣ */
  	USART_printf(USART1,(u8*)&read_SMSAamPoeNOSet,22);
	                                               
}
/************************************************************************************************/
/**	�� �� ��: Com_Config_Time_Interval_Set											*/
/**	����˵��:����ʱ�������ϴ�ʱ��������ʷ��¼ʱ��������ӡʱ������ ������¼�����****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
/* �ǳ���״̬��ӡʱ��������ָ�� */
#define Pit_Time_Irl_Data_P_1  (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
/* " �����洢���"����ָ�� */
#define Menu_Nml_Save_Irl_Data_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval)
/* "�����洢���"����ָ�� */
#define Menu_Aam_Save_Irl_Data_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval)
static void Com_Config_Time_Interval_Set (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	
		COM_SENSOR_CAL_TYPEDEF      write_Time_Interval_Set;//���մ�������
     /* ���õ�ʱ����� */
     u32 normalsavetime;//��ӡ���
	 //  u32 uploadtime;//�ϴ�������
	   u32 alarm_intervaltime;//������¼���
	   u32 normalsavetHHH;//��ʷ��¼���
    /* ��ȡ���õ�ʱ��ֵ */
    normalsavetime =((Cal_Inf->Com_Data[5]<<8)+Cal_Inf->Com_Data[4])*60;//��ӡ���
	//  uploadtime     =((Cal_Inf->Com_Data[1]<<8)+Cal_Inf->Com_Data[0])*60;//�ϴ����
    alarm_intervaltime =((Cal_Inf->Com_Data[7]<<8)+Cal_Inf->Com_Data[6])*60;//������¼���
  	normalsavetHHH=((Cal_Inf->Com_Data[3]<<8)+Cal_Inf->Com_Data[2])*60;//��ʷ��¼���
	
    	/* �ж����õ�ʱ��ֵ�Ƿ��ڿ������õķ�Χ�� */
	if( ((normalsavetime == 0)||(normalsavetime > Com_Config_Time_Interval_Set_Max_Second_Num))||
		  ((normalsavetHHH == 0)||(normalsavetHHH > Com_Config_Time_Interval_Set_Max_Second_Num))||
	    ((alarm_intervaltime == 0)||(alarm_intervaltime > Com_Config_Time_Interval_Set_Max_Second_Num))
	)//����ʱ����ʧ��
	
	{
		/***********************��Ӧ��λ��ָ��****************************************/
	/* ���� ʵʱ���� ���� */
	write_Time_Interval_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
  write_Time_Interval_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	write_Time_Interval_Set.Cmd_Code =Com_Config_Time_Interval_Cmd_Code_Write;//д�� ʱ����������	
	write_Time_Interval_Set.Chanel_Num=0xFF;	
	write_Time_Interval_Set.Data_Len=0X00;
	write_Time_Interval_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*����ʧ�ܱ�־*/
	write_Time_Interval_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	write_Time_Interval_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_Time_Interval_Set);/* ���� У�� */	
	 /* ��Ӧ ʵʱ���� ������Ϣ */
  USART_printf(USART1,(u8*)&write_Time_Interval_Set,8);
	//	return;
	}
	else//����ʱ�����ɹ�
	{
/***********���ô�ӡʱ����***************************************/		
		//ʱ������뷽ʽӦ�ø������Ի���ȽϺá�
	*Pit_Time_Irl_Data_P_1	= normalsavetime;	
	/* �޸� ���� ��Ϣ�� ���ñ�־ */
	 Write_Config_Tab((u8*)Pit_Time_Irl_Data_P_1,sizeof(*Pit_Time_Irl_Data_P_1));
/******************************************************************/
///***********�����ϴ�ʱ����***************************************/	
//			//ʱ������뷽ʽӦ�ø������Ի���ȽϺá�
//		*Menu_Nml_Save_Irl_Data_A	= uploadtime;
/***********������ʷ��¼ʱ����***************************************/	
			//ʱ������뷽ʽӦ�ø������Ի���ȽϺá�
		*Menu_Nml_Save_Irl_Data_A	= normalsavetHHH;		
		/* �޸� ���� ��Ϣ�� ���ñ�־ */
		Write_Config_Tab((u8*)Menu_Nml_Save_Irl_Data_A,sizeof(*Menu_Nml_Save_Irl_Data_A));
/*******************************************************************/
/*************���ñ�����¼���************************************/
		/* ���ñ���״̬�ı���ʱ���� */
		*Menu_Aam_Save_Irl_Data_A	= alarm_intervaltime;
		
		/* �޸� ���� ��Ϣ�� ���ñ�־ */
		Write_Config_Tab((u8*)Menu_Aam_Save_Irl_Data_A,sizeof(*Menu_Aam_Save_Irl_Data_A));
			
/***********************��Ӧ��λ��ָ��****************************************/
	/* ���� ʵʱ���� ���� */
	write_Time_Interval_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
  write_Time_Interval_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	write_Time_Interval_Set.Cmd_Code =Com_Config_Time_Interval_Cmd_Code_Write;//д�� ʱ����������	
	write_Time_Interval_Set.Chanel_Num=0xFF;	
	write_Time_Interval_Set.Data_Len=0x01;
	write_Time_Interval_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	write_Time_Interval_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	write_Time_Interval_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_Time_Interval_Set);/* ���� У�� */	
	 /* ��Ӧ ʵʱ���� ������Ϣ */
  USART_printf(USART1,(u8*)&write_Time_Interval_Set,8);	
	}

}



/************************************************************************************************/
/**	�� �� ��: Com_Config_Time_Interval_Read											*/
/**	����˵��:��ȡʱ�������ϴ�ʱ��������ʷ��¼ʱ��������ӡʱ������ ������¼�����****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
//u32 mmn=0;
static void Com_Config_Time_Interval_Read	(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
     COM_SENSOR_CAL_TYPEDEF      read_Time_Interval_Set;//���մ�������
	
	       /* ���õ�ʱ����� */
     u32 normalsavetime;//��ӡ���
	 //  u32 uploadtime;//�ϴ�������
	   u32 alarm_intervaltime;//������¼���
	   u32 normalsavetHHH;//��ʷ��¼���
	
	   normalsavetHHH=(*Menu_Nml_Save_Irl_Data_A)/60;//��ʷ��¼���
	   alarm_intervaltime=(*Pit_Time_Irl_Data_P_1)/60;//������¼���
	   normalsavetime=(*Menu_Aam_Save_Irl_Data_A)/60;//��ӡ���
	
	
	   read_Time_Interval_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Time_Interval_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Time_Interval_Set.Cmd_Code =Com_Config_Time_Interval_Cmd_Code_Read;//�� ʱ���� ������
	   read_Time_Interval_Set.Chanel_Num=0xFF;
	   read_Time_Interval_Set.Data_Len=0X08;
	   read_Time_Interval_Set.Com_Data[0]=0x78;
	   read_Time_Interval_Set.Com_Data[1]=0x00;
	   read_Time_Interval_Set.Com_Data[2]=(normalsavetHHH&0xFF);/**/
	   read_Time_Interval_Set.Com_Data[3]=(normalsavetHHH>>8);
	   read_Time_Interval_Set.Com_Data[4]=(alarm_intervaltime&0xFF);/**/ 
	   read_Time_Interval_Set.Com_Data[5]=(alarm_intervaltime>>8);
	   read_Time_Interval_Set.Com_Data[6]=(normalsavetime&0xFF);/**/;
	   read_Time_Interval_Set.Com_Data[7]=(normalsavetime>>8);;
	   read_Time_Interval_Set.Com_Data[8]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Time_Interval_Set.Com_Data[9]=Com_Sensor_Cal_Get_Check(&read_Time_Interval_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Time_Interval_Set,15);	
//     mmn=*Pit_Time_Irl_Data_P_1;//��ȡ��ӡʱ��������



}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Alarm_time_delay_Write										*/
/**	����˵��:д������ʱ���****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
/* ͨ��������ʱ����ָ��. */
//u32 bbk=0;
#define Cha_Aam_Dly_Data_A(cha)		(&((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[cha*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Alarm_Delay)
static void Com_Config_Alarm_time_delay_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{

     	/* ���õ�ʱ����� */
	     u32 alarmsavetime;
	  	/* �������� */
	     u8 num;
	   COM_SENSOR_CAL_TYPEDEF    Alarm_time_delay_Set;//���մ�������
	
	   alarmsavetime =(Cal_Inf->Com_Data[1]<<8)+Cal_Inf->Com_Data[0];//�����ڽ��յ��������ݺϳ�һ���������ݼ�Ϊ������ʱ���ʱ��
//	   bbk=alarmsavetime;
	   if((alarmsavetime==0)||(alarmsavetime>Com_Config_Alarm_time_delay_SetMaxSecond))//�������ı���ʱ����3600�루1��Сʱ�
		 {
		 
			 	/***********************��Ӧ��λ��ָ��****************************************/
	     /* ���� ʵʱ���� ���� */
	    Alarm_time_delay_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
      Alarm_time_delay_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	    Alarm_time_delay_Set.Cmd_Code =Com_Config_Alarm_time_delay_Cmd_Code_Write;//д������ʱ���������	
	    Alarm_time_delay_Set.Chanel_Num=0xFF;	
	    Alarm_time_delay_Set.Data_Len=0X02;
	    Alarm_time_delay_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*����ʧ�ܱ�־*/
	    Alarm_time_delay_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	    Alarm_time_delay_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Alarm_time_delay_Set);/* ���� У�� */	
	     /* ��Ӧ ʵʱ���� ������Ϣ */
      USART_printf(USART1,(u8*)&Alarm_time_delay_Set,8);
			
			 
		 }
		 else//ִ�����ñ�����ʱ�������
		 {
		 
		       /* ���� ����ͨ���� ������ʱ���� */
		    for(num = 0; num < Sensor_CH_Config_Num; num++)
		  {
			  /* ��������״̬�ı���ʱ���� */
			   *Cha_Aam_Dly_Data_A(num) = alarmsavetime;
			  /* �޸� ���� ��Ϣ�� ���ñ�־ */
		   	Write_Config_Tab((u8*)Cha_Aam_Dly_Data_A(num),sizeof(*Cha_Aam_Dly_Data_A(num)));
				
			}
				
			/***********************��Ӧ��λ��ָ��****************************************/
	/* ���� ʵʱ���� ���� */
	Alarm_time_delay_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
  Alarm_time_delay_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	Alarm_time_delay_Set.Cmd_Code =Com_Config_Alarm_time_delay_Cmd_Code_Write;//д������ʱ���������	
	Alarm_time_delay_Set.Chanel_Num=0xFF;	
	Alarm_time_delay_Set.Data_Len=0x02;
	Alarm_time_delay_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	Alarm_time_delay_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	Alarm_time_delay_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Alarm_time_delay_Set);/* ���� У�� */			
	 /* ��Ӧ ʵʱ���� ������Ϣ */
  USART_printf(USART1,(u8*)&Alarm_time_delay_Set,8);						
	}
}
/************************************************************************************************/
/**	�� �� ��: Com_Config_Alarm_time_delay_Read									*/
/**	����˵��:��������ʱ���****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Alarm_time_delay_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	
   COM_SENSOR_CAL_TYPEDEF      read_Alarm_time_delay_Set;//���մ�������


	   read_Alarm_time_delay_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Alarm_time_delay_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Alarm_time_delay_Set.Cmd_Code =Com_Config_Alarm_time_delay_Cmd_Code_Read;//�� ��ʱ������� ������
	   read_Alarm_time_delay_Set.Chanel_Num=0xFF;
	   read_Alarm_time_delay_Set.Data_Len=0X02;
	   read_Alarm_time_delay_Set.Com_Data[1]=((*Cha_Aam_Dly_Data_A(Sensors_CH_1))>>8);         
	   read_Alarm_time_delay_Set.Com_Data[0]=((*Cha_Aam_Dly_Data_A(Sensors_CH_1))&0xFF); 
	   read_Alarm_time_delay_Set.Com_Data[2]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Alarm_time_delay_Set.Com_Data[3]=Com_Sensor_Cal_Get_Check(&read_Alarm_time_delay_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Alarm_time_delay_Set,9);	
}

/* �����ܿ��� "��" ״ָ̬�� */
#define Sud_Aam_Mtr_Sth_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)
/* �����ܿ��� "��" ״ָ̬�� */
#define Lgt_Aam_Mtr_Sth_A	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch)
/************************************************************************************************/
/**	�� �� ��: Com_Config_Sound_lightSwitch_Read									*/
/**	����˵��:��ȡ���ⱨ���ܿ���״̬****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Sound_lightSwitch_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Sound_lightSwitch_Set;//���մ�������
	    
	
	   read_Sound_lightSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Sound_lightSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Sound_lightSwitch_Set.Cmd_Code =Com_ConfigTotal_audible_light_alarm_Cmd_Code_Read;//�� ���ⱨ���ܿ��� ������
	   read_Sound_lightSwitch_Set.Chanel_Num=0xFF;
	   read_Sound_lightSwitch_Set.Data_Len=0X01;      
	   read_Sound_lightSwitch_Set.Com_Data[0]=((*Sud_Aam_Mtr_Sth_A)&&(*Lgt_Aam_Mtr_Sth_A)); 
	   read_Sound_lightSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Sound_lightSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Sound_lightSwitch_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Sound_lightSwitch_Set,8);	
	
     




}
/************************************************************************************************/
/**	�� �� ��: Com_Config_Sound_lightSwitch_Write									*/
/**	����˵��:д���ⱨ���ܿ���״̬****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Sound_lightSwitch_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	      COM_SENSOR_CAL_TYPEDEF      read_Sound_lightSwitch_Set;//���մ�������
	
          if((Cal_Inf->Com_Data[0]==Switch_ON)||(Cal_Inf->Com_Data[0]==Switch_OFF))
					{
					   	/* ���ⱨ���ܿ���״̬Ϊ:��,���� ͨ�� ���ⱨ��. */
				        *Sud_Aam_Mtr_Sth_A =(BOOL)(Cal_Inf->Com_Data[0]);
				        *Lgt_Aam_Mtr_Sth_A =(BOOL)(Cal_Inf->Com_Data[0]);
				      /* �޸� ���� ��Ϣ�� ���ñ�־ */
				    Write_Config_Tab((u8*)Sud_Aam_Mtr_Sth_A,sizeof(*Sud_Aam_Mtr_Sth_A));
				    Write_Config_Tab((u8*)Lgt_Aam_Mtr_Sth_A,sizeof(*Lgt_Aam_Mtr_Sth_A));
					
	/***********************��Ӧ��λ��ָ��****************************************/
	     /* ���� ʵʱ���� ���� */
	    read_Sound_lightSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
      read_Sound_lightSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	    read_Sound_lightSwitch_Set.Cmd_Code =Com_ConfigTotal_audible_light_alarm_Cmd_Code_Write;//д���ⱨ���ܿ��ز�����	
	    read_Sound_lightSwitch_Set.Chanel_Num=0xFF;	
	    read_Sound_lightSwitch_Set.Data_Len=0x01;
	    read_Sound_lightSwitch_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	    read_Sound_lightSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	    read_Sound_lightSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Sound_lightSwitch_Set);/* ���� У�� */			
	    /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Sound_lightSwitch_Set,8);
								
					}





}
/************************************************************************************************/
/**	�� �� ��: Com_Config_SMSalarmSwitch_Read									*/
/**	����˵��:��ȡ���ű����ܿ���״̬****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
/* "���ű���"�ܿ���״ָ̬�� */
#define SMS_Aam_Mtr_Sth_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch)
static void Com_Config_SMSalarmSwitch_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_SMSalarmSwitch_Set;//���մ������� 

     read_SMSalarmSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_SMSalarmSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_SMSalarmSwitch_Set.Cmd_Code =Com_Config_SMS_alarm_total_switch_Cmd_Code_Read;//�� ���ű����ܿ��� ������
	   read_SMSalarmSwitch_Set.Chanel_Num=0xFF;
	   read_SMSalarmSwitch_Set.Data_Len=0X01;      
	   read_SMSalarmSwitch_Set.Com_Data[0]=(*SMS_Aam_Mtr_Sth_A); 
	   read_SMSalarmSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_SMSalarmSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_SMSalarmSwitch_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_SMSalarmSwitch_Set,8);	
}
/************************************************************************************************/
/**	�� �� ��: Com_Config_SMSalarmSwitch_Write									*/
/**	����˵��:д���ű����ܿ���״̬****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_SMSalarmSwitch_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
     COM_SENSOR_CAL_TYPEDEF      Write_SMSalarmSwitch_Set;//���մ�������

     if((Cal_Inf->Com_Data[0]==Switch_ON)||(Cal_Inf->Com_Data[0]==Switch_OFF))
					{
					   	/* ���ű����ܿ���״̬Ϊ:��,����*/
				        *SMS_Aam_Mtr_Sth_A =(BOOL)(Cal_Inf->Com_Data[0]);
				    /* �޸� ���� ��Ϣ�� ���ñ�־ */
				     Write_Config_Tab((u8*)SMS_Aam_Mtr_Sth_A,sizeof(*SMS_Aam_Mtr_Sth_A));
	/***********************��Ӧ��λ��ָ��****************************************/
	     /* ���� ʵʱ���� ���� */
	    Write_SMSalarmSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
      Write_SMSalarmSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	    Write_SMSalarmSwitch_Set.Cmd_Code =Com_Config_SMS_alarm_total_switch_Cmd_Code_Write;//д���ű����ܿ��ز�����	
	    Write_SMSalarmSwitch_Set.Chanel_Num=0xFF;	
	    Write_SMSalarmSwitch_Set.Data_Len=0x01;
	    Write_SMSalarmSwitch_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	    Write_SMSalarmSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	    Write_SMSalarmSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Write_SMSalarmSwitch_Set);/* ���� У�� */			
	    /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&Write_SMSalarmSwitch_Set,8);
								
					}

}

/************************************************************************************************/
/**	�� �� ��: Com_Config_SystemTime_Write									*/
/**	����˵��:ͨ����λ������ϵͳʱ��****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
/* ϵͳʱ�����ݼĴ��� */
static Times System_Cok_DR;
extern BOOL RTC_Init(BOOL reset_state);
extern void Write_System_Set(Times* time);
static void Com_Config_SystemTime_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	  COM_SENSOR_CAL_TYPEDEF      read_GSM_SystemTime_Set;//���մ������� 
   // COM_SENSOR_CAL_TYPEDEF      Write_SystemTime_Set;//���մ�������

          if((Cal_Inf->Chanel_Num==0xBB)&&
						 ((Cal_Inf->Com_Data[0]>=0x17)&&(Cal_Inf->Com_Data[0]<=0x34))&&
					   ((Cal_Inf->Com_Data[1]>=0x01)&&(Cal_Inf->Com_Data[1]<=0x12))&&
					   ((Cal_Inf->Com_Data[2]>=0x01)&&(Cal_Inf->Com_Data[2]<=0x31))&&
					   ((Cal_Inf->Com_Data[3]>0x00)&&(Cal_Inf->Com_Data[3]<=0x23))&&
					   ((Cal_Inf->Com_Data[4]>0x00)&&(Cal_Inf->Com_Data[4]<=0x59))&&
					   ((Cal_Inf->Com_Data[5]>0x00)&&(Cal_Inf->Com_Data[2]<=0x59))
					
					)
					{    
           
					     System_Cok_DR.Year= System_Year_Start+(Cal_Inf->Com_Data[0]);//������
					     System_Cok_DR.Month=(Cal_Inf->Com_Data[1]);//������
						   System_Cok_DR.Day=(Cal_Inf->Com_Data[2]);//������
						   System_Cok_DR.Hour=(Cal_Inf->Com_Data[3]);//����ʱ
						   System_Cok_DR.Min=(Cal_Inf->Com_Data[4]);//���÷�
						   System_Cok_DR.Sec=(Cal_Inf->Com_Data[5]);//������
						
						
						   /* ��λ RTC8025 */
		           RTC_Init(TRUE);	
						   /* ���� ʱ�� */ 
	        	 Write_System_Set(&System_Cok_DR);

					  Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year 	=System_Cok_DR.Year;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month 	= System_Cok_DR.Month;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day 	= System_Cok_DR.Day;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour 	= System_Cok_DR.Hour;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min 	= System_Cok_DR.Min;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec 	= System_Cok_DR.Sec;
		
	       	//Ϊ�˽��RTCʱ��оƬ���ⱻд���ϣ������ȱ���һ�ε�ǰ��ʵʱʱ������Ϊ\
		     ���ϵ�������⵽RTC���쳣�����ʹ�����ʱ��������ʵʱʱ�䡣
		     /* �޸� ���� ��Ϣ�� ���ñ�־ */
		       Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_RTC,
					    sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC));




					}
			else if(Cal_Inf->Chanel_Num==0xCC)
			   {
				 
		 read_GSM_SystemTime_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_GSM_SystemTime_Set.Instru_NO=0x01;//�������
	   read_GSM_SystemTime_Set.Cmd_Code =Com_Config_SystemTime_Cmd_Code_Read_Write;//��ϵͳʱ�������
	   read_GSM_SystemTime_Set.Chanel_Num=0xFF;
	   read_GSM_SystemTime_Set.Data_Len=0X06;      
	   read_GSM_SystemTime_Set.Com_Data[0]=HEX_To_BCD((Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year-0X30));//��
		 read_GSM_SystemTime_Set.Com_Data[1]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month);//��
	   read_GSM_SystemTime_Set.Com_Data[2]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day);//��
		 read_GSM_SystemTime_Set.Com_Data[3]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour);//ʱ
	   read_GSM_SystemTime_Set.Com_Data[4]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min);//��
		 read_GSM_SystemTime_Set.Com_Data[5]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec);//��
	   read_GSM_SystemTime_Set.Com_Data[6]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_GSM_SystemTime_Set.Com_Data[7]=Com_Sensor_Cal_Get_Check(&read_GSM_SystemTime_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_GSM_SystemTime_Set,13);	
					 
					 
				 }				
					
					

}



/************************************************************************************************/
/**	�� �� ��: Com_Config_GSM_Signal_Strength_Read									*/
/**	����˵��:��ȡ����GSMģ���ź�ǿ��****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_GSM_Signal_Strength_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_GSM_Signal_Strength_Set;//���մ������� 

     read_GSM_Signal_Strength_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_GSM_Signal_Strength_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_GSM_Signal_Strength_Set.Cmd_Code =Com_Config_GSM_Signal_Strength_Cmd_Code_Read;//�� GSMģ���ź�ǿ�Ȳ�����
	   read_GSM_Signal_Strength_Set.Chanel_Num=0xFF;
	   read_GSM_Signal_Strength_Set.Data_Len=0X01;      
	   read_GSM_Signal_Strength_Set.Com_Data[0]=(u8)GPRS_SR_Reg.GPRS_Sign_Stre;//GSMģ���ź�ǿ��ֵ
	   read_GSM_Signal_Strength_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_GSM_Signal_Strength_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_GSM_Signal_Strength_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_GSM_Signal_Strength_Set,8);	
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Cancel_Sound_Light_Alarm		*/
/**	����˵��:����λ������ȡ�����ⱨ���ɹ�****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Cancel_Sound_Light_Alarm(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Cancel_Sound_Light_Alarm_Set;//���մ������� 

     read_Cancel_Sound_Light_Alarm_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Cancel_Sound_Light_Alarm_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Cancel_Sound_Light_Alarm_Set.Cmd_Code =Cancel_Sound_Light_Alarm_Cmd_Code;//ȡ�����ⱨ��
	   read_Cancel_Sound_Light_Alarm_Set.Chanel_Num=0xFF;
	   read_Cancel_Sound_Light_Alarm_Set.Data_Len=0X01;      
	   read_Cancel_Sound_Light_Alarm_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	   read_Cancel_Sound_Light_Alarm_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Cancel_Sound_Light_Alarm_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Cancel_Sound_Light_Alarm_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Cancel_Sound_Light_Alarm_Set,8);	
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Cancel_Sound_Light_Alarm		*/
/**	����˵��:����λ������ȡ�����ⱨ���ɹ�****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Clear_Historical_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Clear_Historical_Data_Set;//���մ������� 

     read_Clear_Historical_Data_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Clear_Historical_Data_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Clear_Historical_Data_Set.Cmd_Code =Clear_Historical_Data_Cmd_Code;//�����ʷ���ݲ�����
	   read_Clear_Historical_Data_Set.Chanel_Num=0xFF;
	   read_Clear_Historical_Data_Set.Data_Len=0X01;      
	   read_Clear_Historical_Data_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	   read_Clear_Historical_Data_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Clear_Historical_Data_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Clear_Historical_Data_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Clear_Historical_Data_Set,8);	
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Open_GSM_Debug_Information	*/
/**	����˵��:����λ��������GSMģ�鴮�ڵ�����Ϣ�ɹ�****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Open_GSM_Debug_Information(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Open_GSM_Debug_Information_Set;//���մ������� 

     read_Com_Config_Open_GSM_Debug_Information_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Com_Config_Open_GSM_Debug_Information_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Com_Config_Open_GSM_Debug_Information_Set.Cmd_Code =Open_GSM_Debug_Information_Cmd_Code;//��GSMģ�������Ϣ
	   read_Com_Config_Open_GSM_Debug_Information_Set.Chanel_Num=0xFF;
	   read_Com_Config_Open_GSM_Debug_Information_Set.Data_Len=0X01;      
	   read_Com_Config_Open_GSM_Debug_Information_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	   read_Com_Config_Open_GSM_Debug_Information_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Com_Config_Open_GSM_Debug_Information_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Open_GSM_Debug_Information_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Com_Config_Open_GSM_Debug_Information_Set,8);	
}


/************************************************************************************************/
/**	�� �� ��: Com_Config_Close_GSM_Debug_Information	*/
/**	����˵��:����λ�������ر�GSMģ�鴮�ڵ�����Ϣ�ɹ�****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Close_GSM_Debug_Information(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Close_GSM_Debug_Information_Set;//���մ������� 

     read_Com_Config_Close_GSM_Debug_Information_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Com_Config_Close_GSM_Debug_Information_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Com_Config_Close_GSM_Debug_Information_Set.Cmd_Code =Open_GSM_Debug_Information_Cmd_Code;//��GSMģ�������Ϣ
	   read_Com_Config_Close_GSM_Debug_Information_Set.Chanel_Num=0xFF;
	   read_Com_Config_Close_GSM_Debug_Information_Set.Data_Len=0X01;      
	   read_Com_Config_Close_GSM_Debug_Information_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	   read_Com_Config_Close_GSM_Debug_Information_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Com_Config_Close_GSM_Debug_Information_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Close_GSM_Debug_Information_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Com_Config_Close_GSM_Debug_Information_Set,8);	
}


/************************************************************************************************/
/**	�� �� ��: Com_Config_PowerLink_And_Battery_Voltage_Information	*/
/**	����˵��:����λ�������ر�GSMģ�鴮�ڵ�����Ϣ�ɹ�****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static INSTRU_POWER* Battery=&Instru_Config_Inf_Tab.Inf_State.Instru_Power;//��Դ�����Ϣ�ṹ��ָ��
static void Com_Config_PowerLink_And_Battery_Voltage_Information(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set;//���մ������� 

     read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Cmd_Code =Check_PowerLink_And_Battery_Voltage_Cmd_Code;//����32λ������ӵ缰��ص�ѹ״̬������
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Chanel_Num=0xFF;
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Data_Len=0X02;      
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[0]=Battery->Inf_State.ET_Link_State;//������ӵ�״̬
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[1]=Battery->Inf_State.BT_Quantity_State;//32λ�����ص���                                                                                    
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[2]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[3]=Com_Sensor_Cal_Get_Check(&read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set,9);	
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Open_Data_Save_Record	*/
/**	����˵��:����λ�����������ݼ�¼�ɹ�****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Open_Data_Save_Record(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Open_Data_Save_Record_Set;//���մ������� 

     read_Com_Config_Open_Data_Save_Record_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Com_Config_Open_Data_Save_Record_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Com_Config_Open_Data_Save_Record_Set.Cmd_Code =Open_DataSave_Cmd_Code;//�����ݼ�¼����
	   read_Com_Config_Open_Data_Save_Record_Set.Chanel_Num=0xFF;
	   read_Com_Config_Open_Data_Save_Record_Set.Data_Len=0X01;      
	   read_Com_Config_Open_Data_Save_Record_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	   read_Com_Config_Open_Data_Save_Record_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Com_Config_Open_Data_Save_Record_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Open_Data_Save_Record_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Com_Config_Open_Data_Save_Record_Set,8);	
}
/************************************************************************************************/
/**	�� �� ��: Com_Config_Open_Data_Save_Record	*/
/**	����˵��:����λ�������ر����ݼ�¼�ɹ�****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Close_Data_Save_Record(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Close_Data_Save_Record_Set;//���մ������� 

     read_Com_Config_Close_Data_Save_Record_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Com_Config_Close_Data_Save_Record_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Com_Config_Close_Data_Save_Record_Set.Cmd_Code =Open_DataSave_Cmd_Code;//�����ݼ�¼����
	   read_Com_Config_Close_Data_Save_Record_Set.Chanel_Num=0xFF;
	   read_Com_Config_Close_Data_Save_Record_Set.Data_Len=0X01;      
	   read_Com_Config_Close_Data_Save_Record_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*�����ɹ���־*/	
	   read_Com_Config_Close_Data_Save_Record_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Com_Config_Close_Data_Save_Record_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Close_Data_Save_Record_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Com_Config_Close_Data_Save_Record_Set,8);	
}


/************************************************************************************************/
/**	�� �� ��: Com_Config_Open_Data_Save_Record	*/
/**	����˵��:����λ�������ر����ݼ�¼�ɹ�****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_DataSave_Status_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_DataSave_Status_Set;//���մ������� 

     read_Com_Config_DataSave_Status_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   read_Com_Config_DataSave_Status_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   read_Com_Config_DataSave_Status_Set.Cmd_Code =Check_DataSave_Status_Cmd_Code;//��ѯ���ݼ�¼״̬������
	   read_Com_Config_DataSave_Status_Set.Chanel_Num=0xFF;
	   read_Com_Config_DataSave_Status_Set.Data_Len=0X01;      
	   read_Com_Config_DataSave_Status_Set.Com_Data[0]=*Save_Data_Switch_A;/*���ݼ�¼״̬��0��δ������1�����������ݼ�¼*/	
	   read_Com_Config_DataSave_Status_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   read_Com_Config_DataSave_Status_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_DataSave_Status_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&read_Com_Config_DataSave_Status_Set,8);	
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Software_Version_NO_Major_Read	*/
/**	����˵��:��ȡ32λƽ̨������汾��Ӳ���汾****************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Software_Version_NO_Major_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Software_Version_NO_Major_Set;//���մ������� 

	
	   if(Cal_Inf->Chanel_Num==0xAA)
		 {
				 read_Com_Config_Software_Version_NO_Major_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
				 read_Com_Config_Software_Version_NO_Major_Set.Instru_NO=0x01;//�������
				 read_Com_Config_Software_Version_NO_Major_Set.Cmd_Code =Com_Config_Software_Version_NO_Major_Cmd_Code_Read;//��ѯ32λƽ̨����汾��Ӳ���汾
				 read_Com_Config_Software_Version_NO_Major_Set.Chanel_Num=0xAA;
				 read_Com_Config_Software_Version_NO_Major_Set.Data_Len=0X08;      
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[4]=(Software_Version_NO_Major+0x30);/*��� ���汾��� */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[5]=0X2E;/* .��ASCII��*/
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[6]=(Software_Version_NO_Child+0x30);/*��� �Ӱ汾��� */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[7]=(Software_Version_NO_Stage+0x30);/*��� �׶ΰ汾��� */
			
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[0]=(Hardware_Version_NO_Major+0x30);/*Ӳ�� ���汾��� */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[1]=0X2E;/* .��ASCII��*/
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[2]=(Hardware_Version_NO_Child+0x30);/*Ӳ�� �Ӱ汾��� */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[3]=(Hardware_Version_NO_Stage+0x30);/*Ӳ�� �׶ΰ汾��� */
			
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[8]=Com_Sensor_Cal_Constant;/* ���� ���� */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[9]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Software_Version_NO_Major_Set);/* ���� У�� */	
				 /* ��Ӧ ʵʱ���� ������Ϣ */
				 USART_printf(USART1,(u8*)&read_Com_Config_Software_Version_NO_Major_Set,15);	
		 }
}

/************************************************************************************************/
/**	�� �� ��: Com_Config_Factory_Setting	*/
/**	����˵��:�ָ���������***************/
/**	��    �Σ������������ò�����Ϣָ�룺Cal_Inf													*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Com_Config_Factory_Setting(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF         Factory_Setting_Set;//���մ������� 

	    if(Cal_Inf->Com_Data[0]==0x01)
			{	
		 Factory_Setting_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   Factory_Setting_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   Factory_Setting_Set.Cmd_Code =Com_Config_System_Factory_Setting;//�ָ��������ò�����
	   Factory_Setting_Set.Chanel_Num=0xFF;
	   Factory_Setting_Set.Data_Len=0X01;      
	   Factory_Setting_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*ִ�лָ�������������ɹ�*/	
	   Factory_Setting_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   Factory_Setting_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Factory_Setting_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&Factory_Setting_Set,8);	
			/* �ָ��������� */
		 Instru_Rte_Fty_Set();
			}
			else
			{
		 Factory_Setting_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//ͷ��
	   Factory_Setting_Set.Instru_NO=Cal_Inf->Instru_NO;//�������
	   Factory_Setting_Set.Cmd_Code =Com_Config_System_Factory_Setting;//�ָ��������ò�����
	   Factory_Setting_Set.Chanel_Num=0xFF;
	   Factory_Setting_Set.Data_Len=0X01;      
	   Factory_Setting_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*ִ�лָ�������������ʧ��*/			
	   Factory_Setting_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ���� ���� */
	   Factory_Setting_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Factory_Setting_Set);/* ���� У�� */	
		 /* ��Ӧ ʵʱ���� ������Ϣ */
     USART_printf(USART1,(u8*)&Factory_Setting_Set,8);
			}
}

/************************************************************************************************/
/**	�� �� ��: COM_Sensor_Cal_Deal																*/
/**	����˵��: ���� ������У׼����																	*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void COM_Sensor_Cal_Deal(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	
	/* ���� ������ ����Ӧ������ */
	switch(Cal_Inf->Cmd_Code)
	{
#if (GSM_Module_Type==2) //ESP8266Wifiģ��		
	/*************Wifiģ���������**********************/
		case Com_Read_Write_Wifi_Name_Cmd_Code:
		{
		  /* ����дWifi���� */
		 Com_Config_Read_Write_Wifi_Name(Cal_Inf);
			break;
		}
		case Com_Read_Write_Wifi_WSKEY_Cmd_Code:
		{
	   /*����дWifi��WSKEY��ز���(���ܷ�ʽ������)*/
     Com_Config_Read_Write_Wifi_WSKEY(Cal_Inf);
			break;
		}
	 case Com_Read_Write_Wifi_WALN_Cmd_Code:
		{
	  /*����дWifi��WANN��������ز���(����IP��ַ���������롢����IP��ַ )*/
     Com_Config_Read_Write_Wifi_Wlan(Cal_Inf);
			break;
		}
		case Com_Read_Write_Wifi_NETP_Cmd_Code:
		{
   /* ����дWifi��NETP��������ز���(Ҫ���ӵ����ݷ�������IP��ַ���˿ں�)*/
     Com_Config_Read_Write_Wifi_NetP(Cal_Inf);
			break;
		}	
		
		case Com_Read_Write_Wifi_DHCP_Cmd_Code:
		{
   /* ����дWifi��ȡIP��ַ�ķ�ʽ*/
     Com_Config_Read_Write_Wifi_DHCP(Cal_Inf);
			break;
		}
		case Com_Read_Write_Wifi_MAC_Cmd_Code:
		{
      /* ����дWifiģ���MAC��ַ*/
     Com_Config_Read_Write_Wifi_MAC(Cal_Inf);
			break;
		}	
		case Com_Set_Rest_Wifi_Cmd_Code:
 		{
		  /*��λ���߻ָ���������*/
		 Com_Config_Rest_Wifi(Cal_Inf);
 		 break;
 		}			
#endif
/**************************************************/		
		case Com_Sensor_Cal_Cmd_Code_Read_Real_Data:
		{
			/* ��ʵʱ���� */
			Com_Sensor_Cal_Read_Real_Data(Cal_Inf);
			break;
		}
		case Com_Sensor_Cal_Cmd_Code_Read_Orig_Data:
		{
			/* ��ԭʼֵ */
			Com_Sensor_Cal_Read_Orig_Data(Cal_Inf);
			break;
		}
		case Com_Sensor_Cal_Cmd_Code_Write_Curve_Data:
		{
			/* д���� */
			Com_Sensor_Cal_Write_Curve_Data(Cal_Inf);
			break;
		}
		case Com_Sensor_Cal_Cmd_Code_Read_Curve_Data:
		{
			/* ������ */
			Com_Sensor_Cal_Read_Curve_Data(Cal_Inf);
			break;
		}
		case Com_Config_SN_Cmd_Code_Write:
		{
			
			/* д�����ţ�SN�ţ� */
			Com_Config_SN_Write(Cal_Inf);
			/* ��������SN�� */
			Serial_Number_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number);
			/* ��λ��ʪ�����ݼĴ��� */
			GPRS_Reset_Sensor_Data_Reg();
			break;
		}
		case Com_Config_SN_Cmd_Code_Read:
		{
			/* �������ţ�SN�ţ� */
			Com_Config_SN_Read(Cal_Inf);
			break;
		}
		case Com_Config_Sensor_Cmd_Code_Write:
		{
			/* д���������� */
			Com_Config_Sensor_Write(Cal_Inf);
			
#if (Wireless_RF_Card_Config_Enable == 0x01)
			/* ��λ��ʪ�����ݼĴ��� */
			GPRS_Reset_Sensor_Data_Reg();
#endif
			break;
		}
		case Com_Config_Sensor_Cmd_Code_Read:
		{
			/* ������������ */
			Com_Config_Sensor_Read(Cal_Inf);
			break;
		}
		
 		/* ����ӡ���ƺ� */
 		case Com_Config_Card_NO_Cmd_Code_Read:
 		{
 			Com_Config_Card_NO_Read(Cal_Inf);
 			break;
 		}
		
 		/* д��ӡ���ƺ� */
 		case Com_Config_Card_NO_Cmd_Code_Write:
 		{
 			Com_Config_Card_NO_Write(Cal_Inf);
 			break;
 		}
		
		 /* �� ��ӡ��˾���� */
 		case Com_Config_Company_Name_Cmd_Code_Read:
 		{
 			Com_Config_Company_Name_Read(Cal_Inf);
 			break;
 		}
		
 		/* д ��ӡ��˾���� */
 		case Com_Config_Company_Name_Cmd_Code_Write:
 		{
 			Com_Config_Company_Name_Write(Cal_Inf);
 			break;
 		}
		
		/* д �������ٹ��� */
 		case Com_Config_Fad_Data_Cmd_Code_Write:
 		{
 			Com_Config_Fad_Data_Write(Cal_Inf);
 			break;
 		}
		
		/* �� �������ٹ��� */
 		case Com_Config_Fad_Data_Cmd_Code_Read:
 		{
 			Com_Config_Fad_Data_Read(Cal_Inf);
 			break;
 		}
		
		/* д���ݷ�����������IP���˿� */
 		case ComConfigDataNetworkWrite:
 		{	
 			Com_Config_DataNetwork_Write(Cal_Inf);
			
////			/* ��������GSMģ�� */
////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
 			break;
 		}	
		/* �����ݷ�����������IP���˿� */
 		case ComConfigDataNetworkRead:
 		{
 			Com_Config_DataNetwork_Read(Cal_Inf);
 			break;
 		}
			/* д���÷�����������IP���˿� */
 		case ComConfigNetworkWrite:
 		{	
 			Com_Config_Network_Write(Cal_Inf);
			
//////			/* ��������GSMģ�� */
//////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
 			break;
 		}	
		/* �����÷�����������IP���˿� */
 		case ComConfigNetworkRead:
 		{
 			 Com_Config_Network_Read(Cal_Inf);
 			break;
 		}
					/* д����������������IP���˿� */
 		case ComConfigUpgradeNetworkWrite:
 		{	
 			Com_Config_Upgrade_Network_Write(Cal_Inf);
			
//			/* ��������GSMģ�� */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
 			break;
 		}	
		/* ������������������IP���˿� */
 		case ComConfigUpgradeNetworkRead:
 		{
 			Com_Config_Upgrade_Network_Read(Cal_Inf);
 			break;
 		}
			/* ����Ϣ��������*/
 		case Com_Config_SMSAamPoeNO_Cmd_Code_Read:
 		{
 			Com_Config_SMSAamPoeNORead(Cal_Inf);
 			break;
 		}
			/* д��Ϣ��������*/
 		case Com_Config_SMSAamPoeNO_Cmd_Code_Write:
 		{
 			Com_Config_SMSAamPoeNOSet(Cal_Inf);
 			break;
 		}
		 /*��ʱ����*/
		case Com_Config_Time_Interval_Cmd_Code_Read:
 		{
 	  	Com_Config_Time_Interval_Read	(Cal_Inf);
 			break;
 		}
/*����ʱ�������ϴ�ʱ��������ʷ��¼ʱ��������ӡʱ������ ������¼�����*/
		/**************дʱ����***************************/
		case Com_Config_Time_Interval_Cmd_Code_Write:
 		{
 			 Com_Config_Time_Interval_Set(Cal_Inf);
 			break;
 		}
/***********************���ñ�����ʱ (����Ϊ��λ) *********************************/	

		 /************��������ʱ���****************************/
		case Com_Config_Alarm_time_delay_Cmd_Code_Read  :
 		{
			
 			Com_Config_Alarm_time_delay_Read(Cal_Inf);
 			break;
 		}	
			
	 /************д������ʱ���****************************/
		case Com_Config_Alarm_time_delay_Cmd_Code_Write :
 		{
 			Com_Config_Alarm_time_delay_Write(Cal_Inf);
 			break;
 		}

  /************�����ⱨ���ܿ���״̬*************************/
		case Com_ConfigTotal_audible_light_alarm_Cmd_Code_Read :
 		{
			
			
 			Com_Config_Sound_lightSwitch_Read(Cal_Inf);
 			break;
 		}			
 /************д���ⱨ���ܿ���״̬*************************/
		case Com_ConfigTotal_audible_light_alarm_Cmd_Code_Write :
 		{
 			Com_Config_Sound_lightSwitch_Write(Cal_Inf);
 			break;
 		}	
/************�����ű����ܿ���״̬*************************/
		case Com_Config_SMS_alarm_total_switch_Cmd_Code_Read :
 		{
 			Com_Config_SMSalarmSwitch_Read(Cal_Inf);
 			break;
 		}				
/************д���ű����ܿ���״̬*************************/
		case Com_Config_SMS_alarm_total_switch_Cmd_Code_Write :
 		{
 			Com_Config_SMSalarmSwitch_Write(Cal_Inf);
 			break;
 		}	

/************��ȡGSMģ���ź�ǿ��*************************/
		case Com_Config_GSM_Signal_Strength_Cmd_Code_Read :
 		{

			Com_Config_GSM_Signal_Strength_Read(Cal_Inf);
 			break;
 		}	 
/************ȡ�����ⱨ�������ڰ����ؼ�ȡ�����ⱨ����������ͬ*************************/
		case Cancel_Sound_Light_Alarm_Cmd_Code:
 		{
			
			DispMenuDeskTopDown();//ȡ�����ⱨ��
			Beep(Beep_OFF);
			Com_Config_Cancel_Sound_Light_Alarm(Cal_Inf);//����λ������ȡ�����ⱨ���ɹ�
			
 			break;
 		}	
/************�����ʷ����0X52************************/
		case Clear_Historical_Data_Cmd_Code:
 		{

			Mcu_Cer_Htc_Data();//�����ʷ���ݺ���
		  Com_Config_Clear_Historical_Data(Cal_Inf);//����λ�����������ʷ���ݳɹ�
 			break;
 		}	

/************��GSMģ�鴮�ڵ�����Ϣ************************/
		case Open_GSM_Debug_Information_Cmd_Code:
 		{
       
			   if(Cal_Inf->Com_Data[0]==1)
				 {
				 	/* ��GSM������Ϣ��� */
//					GSM_DEBUG_Flag = TRUE;
					 
					DEBUG_Flag = TRUE;
			 Com_Config_Open_GSM_Debug_Information(Cal_Inf);//����λ�����ش�GSMģ�鴮�ڵ�����Ϣ�ɹ� 
				 }
				 else if(Cal_Inf->Com_Data[0]==0)
				 {
				 	/* �ر�GSM������Ϣ��� */
//					GSM_DEBUG_Flag = FALSE;
						DEBUG_Flag = FALSE; 
			 Com_Config_Close_GSM_Debug_Information(Cal_Inf);//����λ�����عر�GSMģ�鴮�ڵ�����Ϣ�ɹ�
				 
				 }
 			break;
 		}
/************���32������ӵ�״̬�͵�ص�ѹֵ��Ϣ************************/
		case Check_PowerLink_And_Battery_Voltage_Cmd_Code:
 		{
       
		Com_Config_PowerLink_And_Battery_Voltage_Information(Cal_Inf);//����λ������32λ������ӵ�״̬����ص�ѹ
 			break;
 		}		
/************�������ݼ�¼************************/
		case Open_DataSave_Cmd_Code:
 		{
			  if(Cal_Inf->Com_Data[0]==1)
				{	
       	/* ���� ���ݴ洢����. */
			App_Save_Config(ENABLE);
		  Com_Config_Open_Data_Save_Record(Cal_Inf);//����λ�����ش����ݼ�¼�ɹ�
				}
				else if(Cal_Inf->Com_Data[0]==0)
				{
				
				    /* �ر� ���ݴ洢����. */
			App_Save_Config(DISABLE);
			
		Com_Config_Close_Data_Save_Record(Cal_Inf);//����λ�����عر����ݼ�¼�ɹ�	
				
				}
				
 			break;
 		}			
///************ֹͣ���ݼ�¼************************/
//		case Close_DataSave_Cmd_Code:
// 		{
//    
// 			break;
// 		}	
/************��ѯ���ݼ�¼��״̬************************/
		case Check_DataSave_Status_Cmd_Code:
 		{
			
	   Com_Config_DataSave_Status_Read(Cal_Inf);//����λ���������ݼ�¼״̬			
			 
 			break;
 		}

	case Com_Config_SystemTime_Cmd_Code_Read_Write://��λ��������� �Ͷ�ȡϵͳʱ��
 		{
      Com_Config_SystemTime_Write(Cal_Inf);//��дϵͳʱ��
 			break;
 		}	
 case Com_Config_Software_Version_NO_Major_Cmd_Code_Read://��λ�����32λ��������汾��Ӳ���汾
 		{

			Com_Config_Software_Version_NO_Major_Read(Cal_Inf);//��ȡ32λƽ̨������汾��Ӳ���汾
		 
 			break;
 		}	
 case Com_Config_System_Factory_Setting://�ָ���������ָ��
 		{
			
		 Com_Config_Factory_Setting(Cal_Inf);//�ָ���������ָ��
 			break;
 		}
 case  0x4F://���ڲ��Թ�����
 {
	 
////	 /* ���¿�Ƭ������ */
////#define SMS_Sensor_String	
//// char SMS_GBK[50];
////	 
////	 /* ���õ�Դ������Ϣ */
////	sprintf(SMS_GBK,SMS_Sensor_String);
	 
//////////      if(gsm_handle.sim800c.pGSM_Send_Sms("18738738517","���¿�Ƭ������123!!!"))
//////////			{
//////////			   printf("���Ͷ������ݳɹ�������\n");
//////////			
//////////			}else{
//////////			
//////////			printf("���Ͷ�������ʧ�ܣ�����\n");
//////////			}
    break;
 }

		
		default:
		{
			/* δ�趨���� �������� */
			__nop();
			break;
		}

	}	
	/* �¼�ִ����� ��� ���ݻ�������� */
	Com_Sensor_Cal_Deleted_Buffer_Data(Cal_Inf);
}

/************************************************************************************************/
/**	�� �� ��: COM_Sensor_Cal_Timeout_Check														*/
/**	����˵��: ������У׼ ͨ�ų�ʱ���																*/
/**	��    �Σ�������У׼���ݣ�COM_SENSOR_CAL_TYPEDEF												*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
void COM_Sensor_Cal_Timeout_Check(INSTRU_FIX_INF_TAB* fix_tab,u16 check_time)
{
	
	/* �жϵ�ǰ�Ƿ��ڽ�������֡״̬ */
	if(fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout > 0)
	{
		/* ��ǰ���� ������У׼����֡����״̬ */
		fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout -= check_time;
		/* �ж� ���ݽ����Ƿ�ʱ */
		if(fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout <= 0)
		{
			/* ���ݽ��ճ�ʱ���������ݽ��� */
			/* ������վ������� */
			fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size = 0;
			/* �������֡���� */
			fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len = 0;
			/* �����ʱʱ�� */
			fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout = 0;
		}else
		{
			/* ���ݽ���δ��ʱ���������ݽ��� */
			__nop();
		}
	}else
	{
		/* ��ǰû�н������� */
		__nop();
	}
}

/******************* (C) COPYRIGHT 2015 HZ Zeda Team ****END OF FILE****/


