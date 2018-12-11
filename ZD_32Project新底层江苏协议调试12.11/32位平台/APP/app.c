/************************************************************************************************/
/** @file              : App.c																	*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��6��28��															*/
/** @brief             : c file																	*/
/** @description       : 32Ϊƽ̨Ӧ���ļ�															*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "interface.h"
#include "app.h"
#include "System_Config.h"
#include "App_GPS.h"
#include "App_Print.h"          
#include "App_Save.h"
#include "BSP.h"
#include "Mcu_ADC.h"
#include "Mcu_button.h"
#include "Mcu_Beep.h"
#include "Mcu_Config.h"
#include "Mcu_GPS.h"
#include "Mcu_led.h"
#include "Log_Adc1.h"
#include "Log_Alarm.h"
#include "Log_Battery.h"
#include "Log_COM.h"
#include "Log_GSM.h"
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
	#include "Log_ConfigService.h"
#elif (AppGPRSComProtocol == 0x01)
	#include "Log_ConfigService_Http.h"
#endif
#endif
#include "Log_Download.h"
#if (Gateway_NJSY_Enable == 0x01)
#include "App_Gateway_NJSY.h"
#endif
#if (Gateway_HZZH_Enable == 0x01)
#include "App_Gateway_HZZH.h"
#endif

#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	
#include "Log_GSM_Config.h"  
#endif



#include "Log_GPS.h"
#include "Log_Hum_Cal.h"
#include "Log_IAP.h"
#include "Log_Power.h"
#include "Log_Print.h"
#include "Log_RF_Card.h"
#include "Log_Save.h"
#include "Log_Sensor.h"
#include "Dev_GPS.h"
#include "Dev_Flash.h"
#include "Dev_RTC.h"
#include "Psl_AI2C.h"
#include "Menu.H"
#include <string.h>
/***************************************************************************/
/*****************************ģ����ȫ�ֱ���**********************************/
/***************************************************************************/
/* ����ÿ������Ķ�ջ�ռ䣬app_cfg.h�ļ��к궨��ջ��С */
#if (Wireless_RF_Card_Config_Enable == 0x01)
static OS_STK AppTaskRFCardStk	[APP_TASK_RFCard_STK_SIZE];
#endif

#if (GPS_Config_Enable == 0x01)
/* ����GPS����ջ */
static OS_STK AppTaskGPSStk		      [APP_TASK_GPS_STK_SIZE];
/* ����GPS��Ϣ�����¼����ƿ�ָ�� */
OS_EVENT *GPS_Box;
#endif

static OS_STK AppTaskSaveStk	      [APP_TASK_Save_STK_SIZE];
static OS_STK AppTaskDisStk 	      [APP_TASK_Dis_STK_SIZE];
static OS_STK AppTaskKeyStk 	      [APP_TASK_Key_STK_SIZE];
#if (Firmware_Urerade_Function_Enable == 0x01)
//static OS_STK AppTaskUpgStk 	[APP_TASK_Upg_STK_SIZE];
#endif
static OS_STK AppTaskCOMStk		      [APP_TASK_COM_STK_SIZE];
static OS_STK AppTaskBeepStk	      [APP_TASK_Beep_STK_SIZE];
static OS_STK AppTaskLightStk	      [APP_TASK_Light_STK_SIZE];
static OS_STK AppTaskADCStk		      [APP_TASK_ADC_STK_SIZE];
OS_STK AppTaskGSMStk 	[APP_TASK_GSM_STK_SIZE];
static OS_STK AppTaskStateStk 	    [APP_TASK_State_STK_SIZE];
static OS_STK AppTaskTimeStk 	      [APP_TASK_Time_STK_SIZE];

//#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	
//static OS_STK   AGsmConfigStk		      [GSM_Config_STK_SIZE];

///* GSMģ�����������Ϣ�����¼����ƿ� */
//OS_EVENT    *GSM_Config_EVENT_Str_Q;
///* GSMģ�����������Ϣ�����¼����ƿ����� */
//void        *GSM_Config_EVENT_Grp[((u8)OS_MAX_QS)];
//#endif


//static OS_STK AppTaskUpgStk     [APP_TASK_Upg_STK_SIZE];  
#if (Print_Function_Enable == 0x01)
static OS_STK AppTaskPrintStk 	    [APP_TASK_Print_STK_SIZE];
static OS_STK AppTaskEasyPrintStk 	[APP_TASK_Easy_Print_STK_SIZE];
#endif


/************************************************************************************************/
/* RF��Ƭ �ź���*/
OS_EVENT *RF_Card_QUE_Sem;
#if (Print_Function_Enable == 0x01)
/* ��ӡ���� �ź���*/
OS_EVENT *Print_QUE_Sem;
#endif
/* Save �¼���־ */
OS_FLAG_GRP *Str_Save_Flag;

#if (Firmware_Urerade_Function_Enable == 0x01)
/* �̼����� �¼���־ */
OS_FLAG_GRP *Fwe_Ure_Flag;
#endif

/* GPRS ������� �����������ݰ� �¼���־ */
OS_FLAG_GRP *Str_GPRS_Upg_RXC_Data_Flag;
///* RTC8025 WakeUp �¼���־ */
//OS_FLAG_GRP *Str_RTC8025_WakeUp_Flag;
/* GSM��Ϣ�����¼����ƿ� */
OS_EVENT* GSM_EVENT_Str_Q;
/* GSM��Ϣ�����¼����ƿ����� */
void    * GSM_EVENT_Grp[GSM_EVENT_Q_Len];
/* ����������¼����ƿ� */
OS_EVENT* Beep_EVENT_Str_Q;
/* ����������¼����ƿ����� */
void 	* Beep_EVENT_Grp[GSM_EVENT_Q_Len];
/* ����LED�¼����ƿ� */
OS_EVENT* Light_EVENT_Str_Q;
/* ����LED�¼����ƿ����� */
void 	* Light_EVENT_Grp[GSM_EVENT_Q_Len];
/* ����Comͨ���¼����ƿ� */
OS_EVENT* COM_Sensor_Cal_EVENT_Q;
/* ����Comͨ���¼����ƿ����� */
void 	* COM_Sensor_Cal_EVENT_Grp[COM_Sensor_Cal_EVENT_Q_Len];
/************************************************************************************************/

/* ��ӡ���Ա�־ */
extern BOOL GSM_DEBUG_Flag;// TRUE
/************************************************************************************************/
/* ������	: Task_Start(ϵͳ�����Ŀ�ʼ����)														*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��������壬���Դ�����ɾ���������������͵��¼� 									 	*/
/* �������� 	: 2012/12/20																		*/
/************************************************************************************************/
void Task_Start(void *p_arg)
{
	INT8U      err;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ���� 
	/* ���CPU������ͳ��ģ���ʼ�����ú�����������CPUռ���� ע�⣺�˺������ӳ�100ms�ٷ��� */
	#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
	#endif
	/* �弶��ʼ�� */
	BSP_Init();
	/* ģ���·��Դ�ȶ� */
	OSTimeDly(1000);
	/* �ϵ����ص�ѹ */
	ADC_Sam_Pwr_Sta();
	/* ��ʼ��(���I2C����IOģ���) */
	Psl_AI2C_Init();	
	/* ϵͳ�������� */
	System_Para_Init();
	/* RTC ��ʼ�� */
	if(RTC_Init(FALSE) == FALSE)
	{
		/* ʱ�����ݼĴ��� */
		Times time  = {0};
		time.Year	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year;
		time.Month	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month;
		time.Day	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day;
		time.Hour	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour;
		time.Min	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min;
		time.Sec	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec;
		/* ���� ʱ�� */
		Write_System_Date_Time_Data(&time);
	}
	
	/* ���� Save 	�¼���־ */
	Str_Save_Flag 	= OSFlagCreate (0, &err);

#if (Firmware_Urerade_Function_Enable == 0x01)
  	/* ���� �������� �ź�����־ */
	Fwe_Ure_Flag 	= OSFlagCreate (0, &err);
#endif
	
  	/* ���� �������� �ź�����־ */
	Str_GPRS_Upg_RXC_Data_Flag = OSFlagCreate (0, &err);
	/* ���� RF��Ƭ �ź��� */
	RF_Card_QUE_Sem	= OSSemCreate(0);
	
	/* ���� GSM�¼� 					��Ϣ���� */
	GSM_EVENT_Str_Q   			= OSQCreate(GSM_EVENT_Grp,				GSM_EVENT_Q_Len);
	/* ���� ������У׼�¼�����			��Ϣ���� */
	COM_Sensor_Cal_EVENT_Q   = OSQCreate(COM_Sensor_Cal_EVENT_Grp,	COM_Sensor_Cal_EVENT_Q_Len);	
	/* ���� Beep�¼� 				��Ϣ���� */
	Beep_EVENT_Str_Q  			= OSQCreate(Beep_EVENT_Grp,				Beep_EVENT_Q_Len);
	/* ���� Light�¼� 				��Ϣ���� */
	Light_EVENT_Str_Q 			= OSQCreate(Light_EVENT_Grp,			Light_EVENT_Q_Len);
	
////	#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	
////   //����GSMģ�������¼���Ϣ����
////	 GSM_Config_EVENT_Str_Q = OSQCreate(GSM_Config_EVENT_Grp,((u8)OS_MAX_QS));
////	 
////	 	/*  ����GSMģ��������� ���� */
////	OSTaskCreate(GSM_Config_Task,(void *)0,
////       			(OS_STK *)&AGsmConfigStk[GSM_Config_STK_SIZE - 1],
////				     APP_TASK_GSM_Config_PRIO); 
////  #endif
	
	
//  	/* ����  �ź�����־ */
//	Str_RTC8025_WakeUp_Flag    = OSFlagCreate (0, &err);

#if (Wireless_RF_Card_Config_Enable == 0x01)
	/*  ���� RF��Ƭ ���� */
	OSTaskCreate(AppTaskRFCard,(void *)0,
       			(OS_STK *)&AppTaskRFCardStk[APP_TASK_RFCard_STK_SIZE - 1],
				 APP_TASK_RFCard_PRIO); 
#endif

#if (GPS_Config_Enable == 0x01)

	/*  ���� GPS ���� */
	OSTaskCreate(AppTaskGPS,(void *)0,    
       			(OS_STK *)&AppTaskGPSStk[APP_TASK_GPS_STK_SIZE - 1],
				 APP_TASK_GPS_PRIO); 
#endif
	/*  ���� ��Ϣ���� ���� */
	OSTaskCreate(AppTaskSave,(void *)0,    
       			(OS_STK *)&AppTaskSaveStk[APP_TASK_Save_STK_SIZE - 1],
				 APP_TASK_Save_PRIO); 

	/*  ���� ��ʾ ���� */
	OSTaskCreate(AppTaskDis,(void *)0,    
       			(OS_STK *)&AppTaskDisStk[APP_TASK_Dis_STK_SIZE - 1],
				 APP_TASK_Dis_PRIO); 

	/*  ���� ���� ���� */
	OSTaskCreate(AppTaskKey,(void *)0,    
       			(OS_STK *)&AppTaskKeyStk[APP_TASK_Key_STK_SIZE - 1],
				 APP_TASK_Key_PRIO);

	/*  ���� ʱ�� ���� */
	OSTaskCreate(AppTaskTime,(void *)0,    
       			(OS_STK *)&AppTaskTimeStk[APP_TASK_Time_STK_SIZE - 1],
				 APP_TASK_Time_PRIO);
	
#if (Firmware_Urerade_Function_Enable == 0x01)

	/*  ���� ������������ ���� */
//	OSTaskCreate(AppTaskUpg,(void *)0,    
//       			(OS_STK *)&AppTaskUpgStk[APP_TASK_Upg_STK_SIZE - 1],
//				 APP_TASK_Upg_PRIO);

#endif
	/*  ���� ״̬��� ���� */
	OSTaskCreate(AppTaskState,(void *)0,    
       			(OS_STK *)&AppTaskStateStk[APP_TASK_State_STK_SIZE - 1],
				 APP_TASK_State_PRIO);
	
	/*  ���� ADC���� ���� */
	OSTaskCreate(AppTaskADC,(void *)0,    
       			(OS_STK *)&AppTaskADCStk[APP_TASK_ADC_STK_SIZE - 1],
				 APP_TASK_ADC_PRIO); 
	
	/*  ��������ͨ�� ���� */
	OSTaskCreate(AppTaskCOM,(void *)0,    
       			(OS_STK *)&AppTaskCOMStk[APP_TASK_COM_STK_SIZE - 1],
				 APP_TASK_COM_PRIO);	
	/*  ���� ������ ���� */
	OSTaskCreate(AppTaskBeep,(void *)0,    
       			(OS_STK *)&AppTaskBeepStk[APP_TASK_Beep_STK_SIZE - 1],
				 APP_TASK_Beep_PRIO);
	
	/*  ���� ����ʾ ���� */
	OSTaskCreate(AppTaskLight,(void *)0,    
       			(OS_STK *)&AppTaskLightStk[APP_TASK_Light_STK_SIZE - 1],
				 APP_TASK_Light_PRIO);
	
	/*  ���� GPRS ���� */
	OSTaskCreate(AppTaskGSM,(void *)0,    
       			(OS_STK *)&AppTaskGSMStk[APP_TASK_GSM_STK_SIZE - 1],
				 APP_TASK_GSM_PRIO);
	
#if (Print_Function_Enable == 0x01)
	/*  ���� ��ӡ ���� */
	OSTaskCreate(AppTaskPrint,(void *)0,   
       			(OS_STK *)&AppTaskPrintStk[APP_TASK_Print_STK_SIZE - 1],
				 APP_TASK_Print_PRIO);
	
	/*  ���� һ����ӡ ���� */
	OSTaskCreate(AppTaskEasyPrint,(void *)0,   
       			(OS_STK *)&AppTaskEasyPrintStk[APP_TASK_Easy_Print_STK_SIZE - 1],
				 APP_TASK_Easy_Print_PRIO);
#endif
	
#if (Gateway_NJSY_Enable == 0x01)
	/*  ���� ���� ���� */
	OSTaskCreate(AppTaskGatewayNJSY,(void *)0,
       			(OS_STK *)&AppTaskGatewayNJSYStk[AppTask_GatewayNJSY_StkSize - 1],
				 APP_TASK_Gateway_PRIO);
#endif
#if (Gateway_HZZH_Enable == 0x01)
	/*  ���� ���ݴ������� ���� */
	OSTaskCreate(AppTaskGatewayHZZH,(void *)0,
       			(OS_STK *)&AppTaskGatewayHZZHStk[AppTask_GatewayHZZH_StkSize - 1],
				 APP_TASK_GatewayHZZH_PRIO);
#endif
	while (1)  
	{    
		/* �����Լ� */
		OSTaskSuspend (OS_PRIO_SELF);
	} 
}  

#if (Wireless_RF_Card_Config_Enable == 0x01)
/************************************************************************************************/
/* ������	: AppTaskRFCard(���߿�Ƭ����)													*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��������壬���Դ�����ɾ���������������͵��¼�											*/
/* �������� 	: 2015/04/14																		*/
/************************************************************************************************/
void AppTaskRFCard(void *p_arg)
{
	INT8U      err;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	while (1)
	{
		/* �ȴ� ���� ��Ϣ */
		OSSemPend((OS_EVENT*)RF_Card_QUE_Sem,0,&err);
		
		/* ������Ϣ ״̬ �ж� */
		if(err == OS_ERR_NONE)
		{
			/* RF_Card ����ɸѡ ִ�ж�Ӧ�Ķ��� */
			RF_Card_Screen(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor);
		}
	}
} 
#endif


#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	
/************************************************************************************************/
/* ������	: GSM_Config_Task(GSMģ�������������)													 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ����GSMģ�����							 									*/
/* �������� 	: 2018/11/12																		*/
/************************************************************************************************/
//////////void GSM_Config_Task(void *p_arg)
//////////{
//////////	INT8U                                   err;
//////////	GSM_Config_QUE*          GSM_Config_QUE_inf;
//////////	OS_Q_DATA                         	 Q_data; 
//////////	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
//////////	while (1)
//////////	{
//////////		/* �ȴ� ���� ��Ϣ */
//////////		GSM_Config_QUE_inf = (GSM_Config_QUE*)OSQPend((OS_EVENT*)GSM_Config_EVENT_Str_Q,0,&err);
//////////		/* ������Ϣ ״̬ �ж� */
//////////		if(err == OS_ERR_NONE)
//////////		{
//////////			/* ��Ϣ����ȷ�Ľ��� */
//////////			/* GSMģ���������ִ�ж�Ӧ�Ķ��� */
//////////			GSM_Config_Function(GSM_Config_QUE_inf,&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
//////////		}	
//////////		/* ��ѯ GSM ��Ϣ���� */
//////////		err = OSQQuery((OS_EVENT*)GSM_Config_EVENT_Str_Q,&Q_data);
//////////	
//////////		/* �жϲ�ѯ��� */
//////////		if(err == OS_ERR_NONE)
//////////		{
//////////			/* ��ѯ�ɹ� */
//////////			/* Ѱ����һ�����ͳɹ�����Ϣ */
//////////			if((((OS_Q *)(GSM_Config_EVENT_Str_Q->OSEventPtr))->OSQOut) == (((OS_Q *)(GSM_Config_EVENT_Str_Q->OSEventPtr))->OSQStart))
//////////			{
//////////				/* ���GSMģ�����������Ϣ������һ����Ϣָ�� */
//////////				*((u32*)(((OS_Q *)(GSM_Config_EVENT_Str_Q->OSEventPtr))->OSQEnd)-1) = 0;
//////////			}else
//////////			{
//////////				/* ���GSMģ�����������Ϣ������һ����Ϣָ�� */
//////////				*((u32*)(((OS_Q *)(GSM_Config_EVENT_Str_Q->OSEventPtr))->OSQOut)-1) = 0;
//////////			}
//////////		}
//////////	 } 
//////////	}



	 
	 
#endif



/************************************************************************************************/
/* ������	: AppTaskSave(���������ݴ洢����)													 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��������壬���Դ�����ɾ���������������͵��¼�											*/
/* �������� 	: 2014/9/11																			*/
/************************************************************************************************/
void AppTaskSave(void *p_arg)
{
	INT8U      err;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	/* �ȴ������������� */
	OSTimeDly(1000);
	while (1)  
	{    
		/* �ȴ����� */
		OSFlagPend(								  //�����־�ź�
		Str_Save_Flag,							//�����־�ź�ָ��
		(OS_FLAGS)0xff,							//������ �����0�͵�1λ�ź� 0011 �����ǰ�����3ǿ��ת��ΪOS_FLAGS���͵����ݣ�
												        //��Ϊ�������ͱ�־�ź��е��źŶ���OS_FLAGS���͵�����
												        //OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,   
												        //�ź�ȫ��1��ʾ�ź���Ч ����OS_FLAG_CONSUME��ʾ��
												        //����ȴ����¼������������Ӧ���¼���־λ
		OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,	//�ź�ȫ��1��ʾ�ź���Ч  û�мӲ���OS_FLAG_CONSUME�����Բ��������־λ
		0,										      //�ȴ�ʱ�ޣ�0��ʾ���޵ȴ�
		&err									      //������Ϣ
		);

		/* �����Ե��� ���� ������ʱ�䡱�������������ݡ�����GPS���ݡ����� */
		Save_Sensor_Data(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
		
#if (Print_Function_Enable == 0x01)
		/* ��¼��ǰ�������ݵ�ϵͳʱ���Flash�洢��ַ */
		Print_Index_Save_Sensor_Data(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
#endif		
	}
}         

/************************************************************************************************/
/* ������	: AppTaskDis(������ʾ����)															 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��������壬���Դ�����ɾ���������������͵��¼� 									 		*/
/* �������� 	: 2014/9/12																			*/
/************************************************************************************************/

void AppTaskDis(void *p_arg)
{
	/* ����״̬�Ĵ��� */
	u8 Key_State_REG=0;
	/* ϵͳʱ��Ĵ��� */
	u32 Cur_Time;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	while (1)
	{
		/* ��ȡ��ǰϵͳʱ�� */
		Cur_Time = OSTimeGet();
		
		/* �˵�����״̬���. */
		Menu_Idle_State_Check(Cur_Time);
		
		/* �˵��׽�����Ŀѭ����ʾ */
		Menu_DeskTop_Switch_Disp(Cur_Time);
		
	   #if (AppGPRSComProtocolEnable == 0x01)
     #if (AppGPRSComProtocol == 0x00)//����Э��		
		if(*IAP_App.UpdateFlag != TRUE)//���豸����Զ����������ʱ����ʧЧ
		{
		  /* ��ȡ����״̬ */
		  Key_State_REG = Get_Key_State();
		}
    #endif
    #endif	
		
	   #if (AppGPRSComProtocolEnable == 0x01)
     #if (AppGPRSComProtocol == 0x01)	//HttpЭ��	
		  /* ��ȡ����״̬ */
		  Key_State_REG = Get_Key_State();

    #endif
    #endif
		/* �жϰ���״̬�� */
		if(Key_State_REG != KEY_NONE)
		{
			/* ����������. */
			/* ����˵�����״̬ʱ��Ĵ������� */
			Menu_Idle_State_Reset(Cur_Time);
			/* ����׽�����Ŀ�л�ʱ��Ĵ������� */
			Menu_DeskTop_Switch_Reset(Cur_Time);
		}
		/* ����ɨ�� */
		Menu_Display(Key_State_REG);
		/* �ȴ�100mS������ȴ�������CPUʹ��Ȩ�� */
		OSTimeDlyHMSM(0,0,0,100); 
	}
}

/************************************************************************************************/
/* ������	: AppTaskKey(����״̬��Ϣɨ���ӡ����)													 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ����ʱʱɨ������								 									*/
/* �������� 	: 2014/11/14																		*/
/************************************************************************************************/
void AppTaskKey(void *p_arg)
{
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	/* �� ���� ��Դ */
	KEY_Power_Open;
	
	while (1)  
	{
		/* ����ɨ�� */
		Key_State_Check_All();
		/* �ȴ�10mS������ȴ�������CPUʹ��Ȩ�� */
		OSTimeDlyHMSM(0,0,0,10); 
	}
}

/************************************************************************************************/
/* ������	: AppTaskTime(���Ź���ʱι��ʱ������)															 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��������壬���Դ�����ɾ���������������͵��¼�											*/
/* �������� 	: 2014/10/08																		*/
/************************************************************************************************/
void AppTaskTime(void *p_arg)
{
	static u16 wdg_state;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
//////	/* ι�� ���� ���� */
//////	MAX823_WDI_GPIO_Config();	
	while (1)
	{
////		/* ι�� �ź���� */
////		if(wdg_state == TRUE)
////		{
////			wdg_state = FALSE;
////			MAX823_WDI_High;
////		}else
////		{
////			wdg_state = TRUE;
////			MAX823_WDI_Low;			
////		}	
		
		/* ��ʱ500MSι��һ�� */
		OSTimeDly(500);
	}
}

#if (Firmware_Urerade_Function_Enable == 0x01)
/************************************************************************************************/
/* ������	��AppTaskUpg(���ش���Զ����������)													 	 				*/
/* ����		��(void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����		����������									 									    */
/* ��������	��2014/12/07																		*/
/************************************************************************************************/
void AppTaskUpg(void *p_arg)
{
	INT8U err;
	u16 Upgrade_Result;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	/* �������� ���� */
	while (1)  
	{
		/* ��ʼ�ȴ� ϵͳ���� �ź� */
		OSFlagPend(								//�����־�ź�
		Fwe_Ure_Flag,							//�����־�ź�ָ��
		(OS_FLAGS)0xff,							//������ �����0�͵�1λ�ź� 0011 �����ǰ�����3ǿ��ת��ΪOS_FLAGS���͵����ݣ�
												//��Ϊ�������ͱ�־�ź��е��źŶ���OS_FLAGS���͵�����
												//OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,   
												//�ź�ȫ��1��ʾ�ź���Ч ����OS_FLAG_CONSUME��ʾ��
												//����ȴ����¼������������Ӧ���¼���־λ
		OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME,	//�ź�ȫ��1��ʾ�ź���Ч  û�мӲ���OS_FLAG_CONSUME�����Բ��������־λ
		0,										//�ȴ�ʱ�ޣ�0��ʾ���޵ȴ�
		&err									//������Ϣ
		);
		/* �ж��ж� �ź����� ״̬ */
		if(err == OS_ERR_NONE)
		{
			/* �յ� �������� �ź� */
			/* ���� ���� ���� 	*/
			Upgrade_Result = Remote_Upgrade();
			/* ��ʾ��ǰ �������� ��� */
			if(Upgrade_Result == FALSE)
			{
				/* �������� ʧ��... */
				__nop();
			}else
			{
				/* �������� �ɹ�*/
				__nop();
			}
		}
	}
}
#endif

/************************************************************************************************/
/* ������	: AppTaskGPRS(GSMģ������)													 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: �ϴ����������ݡ�Զ�����á�Զ��������(�й���������ͨ��)						 									 	*/
/* �������� 	: 2014/11/19																		*/
/************************************************************************************************/
void AppTaskGSM(void *p_arg)
{
	INT8U               err;
	GSM_QUE*            GSM_QUE_inf;
	OS_Q_DATA	          Q_data;
	(void)p_arg;      // 'p_arg' ��û���õ�����ֹ��������ʾ����
	while (1)
	{
		/* �ȴ� ���� ��Ϣ */
		GSM_QUE_inf = (GSM_QUE*)OSQPend((OS_EVENT*)GSM_EVENT_Str_Q,0,&err);
		/* ������Ϣ ״̬ �ж� */
		if(err == OS_ERR_NONE)
		{
			/* ��Ϣ����ȷ�Ľ��� */
			/* GSM ִ�ж�Ӧ�Ķ��� */
			GSM_Function(GSM_QUE_inf,&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
		}	
		/* ��ѯ GSM ��Ϣ���� */
		err = OSQQuery((OS_EVENT*)GSM_EVENT_Str_Q,&Q_data);
	
		/* �жϲ�ѯ��� */
		if(err == OS_ERR_NONE)
		{
			/* ��ѯ�ɹ� */
			/* Ѱ����һ�����ͳɹ�����Ϣ */
			if((((OS_Q *)(GSM_EVENT_Str_Q->OSEventPtr))->OSQOut) == (((OS_Q *)(GSM_EVENT_Str_Q->OSEventPtr))->OSQStart))
			{
				/* ���GSM��Ϣ������һ����Ϣָ�� */
				*((u32*)(((OS_Q *)(GSM_EVENT_Str_Q->OSEventPtr))->OSQEnd)-1) = 0;
			}else
			{
				/* ���GSM��Ϣ������һ����Ϣָ�� */
				*((u32*)(((OS_Q *)(GSM_EVENT_Str_Q->OSEventPtr))->OSQOut)-1) = 0;
			}
		}
	}
}

/************************************************************************************************/
/* ������	: AppTaskState(����״̬����)													 	 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ���������еĸ���״̬��Ϣ						 									*/
/* �������� 	: 2014/12/19																		*/
/************************************************************************************************/
void AppTaskState(void *p_arg)
{
/* ״̬ ���� �ĸ���ʱ��Ƶ�� */
#define Task_State_Update_Time					  (10ul)
#define Task_State_Base_time_50ms				  (50u)
#define Task_State_Base_time_100ms				(100u)
#define Task_State_Base_time_500ms				(500u)
#define Task_State_Base_time_1000ms				(1000u)
#define Task_State_Base_time_10000ms			(10000u)
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)	//����Э��
#define Task_State_Base_time_60000ms			(60000u) //1����
	
#define Task_State_Base_time_300000ms			(300000u)//5����	
#endif
#endif	
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x01)	//HttpЭ��
#define Task_State_Base_time_120000ms			(120000u) //2����
#endif
#endif
	
	static u32 Base_time_50ms_Reg            = 0;
	static u32 Base_time_100ms_Reg           = 0;
	static u32 Base_time_500ms_Reg           = 0;
	static u32 Base_time_1000ms_Reg          = 0;
	static u32 Base_time_10000ms_Reg         = 0;
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)	//����Э��	
	static u32 Base_time_60000ms_Reg         = 0;
	static u32 Base_time_300000ms_Reg        = 0;
#endif
#endif
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x01)//HttpЭ��		
	static u32 Base_time_120000ms_Reg         = 0;
#endif
#endif
	u32 Cur_time;	
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	/* ��ʼ�� ״̬������ */
	/* ��ʼ�����õ��״̬���� */
	Battery_Para_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_Power);
	/* ��ʼ��������ӵ�Դ״̬���� */
	Power_ET_Para_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_Power);
	
#if (Print_Function_Enable == 0x01)
	/* �Զ����´�ӡ"��ʼʱ��"��"����ʱ��". */
	App_Print_Date_Init();
#endif

	/* �ȴ�1S�����������񶼾���󣬿�ʼ״̬���¡� */
	OSTimeDlyHMSM(0,0,2,0);
	while (1)
	{
		/* ���� ����״̬ �ĸ������� */
		Cur_time = OSTimeGet();
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x00)//����Э��
		/* Ĭ��5���Ӹ���һ��,���ڽ���Զ������ʱ�޸�Ϊ1��������һ�� */
		if((Cur_time-Base_time_300000ms_Reg) >= Task_State_Base_time_300000ms)	
		{
	    		/* ���� ��ǰ ʱ��Ϊ300000ms��׼ʱ�� */
	    		  Base_time_300000ms_Reg = Cur_time;
	    			/* ������������ */
	    	   	OSSchedLock();
	    	if(IAP_App.RunState==FALSE)//�ж����32�豸��δ����Զ������ʱ���������÷�������ʱ��������ΪĬ��5����
	    	{
	    			/* �������÷���Ӧ�� */
	    		 ConfigService_TXLog(&Instru_Config_Inf_Tab);
	    		/* ���̼�����״̬ */
	    	   IAP_CheckFlag();	
	    	}
	    		/* ������������ */
	    		  OSSchedUnlock();
		}
	
     /* 1���Ӹ���һ��*//* Ĭ��5���Ӹ���һ��,���ڽ���Զ������ʱ�޸�Ϊ1��������һ�� */
     if((Cur_time-Base_time_60000ms_Reg) >= Task_State_Base_time_60000ms)
     {
     	  /* ���� ��ǰ ʱ��Ϊ60000ms��׼ʱ�� */
     	  Base_time_60000ms_Reg = Cur_time;	
     	  /* ������������ */
     	  OSSchedLock();
/**********************�ϵ����һ���豸��Ϣͬ������*****************************************/
			 /* 1���ж������Ƿ�ͬ����� ϵͳÿ�θ�λ�������һ�ν�ϵͳ�����в���ͬ�������÷�������*/
	     if((ConfigService_App.SYNC_State == FALSE))	/* ����δ����ͬ������ͬ��ʧ�� */
	     {
	     		if(GSM_DEBUG_Flag ==TRUE)
	     		{
	     			printf("1����ϵͳ������Ϣͬ�������÷�������\n");
	     		}
	     	  ConfigService_Cmd(0xFF,0x01);			/* �ϴ����豸������Ϣ(ͬ��ƽ̨����) */
	     }
/*******************************************************************************************/			 
     	  if(IAP_App.RunState==TRUE)//�ж����32�豸�ڽ���Զ������ʱ���������÷�������ʱ��������Ϊ1����
     	  {
     	  	/* �������÷���Ӧ�� */
     	    ConfigService_TXLog(&Instru_Config_Inf_Tab);
					/* ���̼�����״̬ */
		      IAP_CheckFlag();	
     	  }
     	  /* ������������ */
     	  OSSchedUnlock();
     }
		  #endif
		  #endif
		    
		   #if (AppGPRSComProtocolEnable == 0x01)
       #if (AppGPRSComProtocol == 0x01) //HttpЭ��
		      /* 1����������ŷ���������һ���豸״̬��Ϣ*/
     if((Cur_time-Base_time_120000ms_Reg) >= Task_State_Base_time_120000ms)
     {
     	  /* ���� ��ǰ ʱ��Ϊ60000ms��׼ʱ�� */
     	  Base_time_120000ms_Reg = Cur_time;	
     	  /* ������������ */
     	  OSSchedLock();
     	  	/* �������÷���Ӧ�� */
     	    ConfigService_TXLog(&Instru_Config_Inf_Tab);
		
     	  /* ������������ */
     	  OSSchedUnlock();
     }
		 	  #endif
		    #endif
		
		/* 10000MS ���� */
		if((Cur_time-Base_time_10000ms_Reg) >= Task_State_Base_time_10000ms)
		{
			/* ���� ��ǰ ʱ��Ϊ1000ms��׼ʱ�� */
			Base_time_10000ms_Reg = Cur_time;
			
			/* ������������ */
			OSSchedLock();
		  #if (AppGPRSComProtocolEnable == 0x01)
      #if (AppGPRSComProtocol == 0x00)//����Э��
			if(IAP_App.RunState == FALSE)
			{
						/* GSM �ź�ǿ�� */
			  GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Update_SGL_EVENT);
			  /* GSM ����״̬ */
			  GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Update_Link_EVENT);
			
			}
			#endif
		  #endif
////////			/* ��⵱ǰ GSM�ź� */
////////			GSM_Signal_Check();		
/* �ж��¶Ȳ��������Ƿ����� */	
#if ((Hum_Sensor_Function_Config_Enable == 0x01) &&\
	 (Hum_Sensor_Tmp_Comp_Enable == 0x01) &&\
	 (Hum_Sensor_Tmp_Comp_CH > 0))
			/* ���ʪ�ȴ������¶Ȳ������� */
			Humi_Get_Temp_Comp_Value();
#endif

			/* ������������ */
			OSSchedUnlock();		
		}
		/* 1000MS ���� */
		if((Cur_time-Base_time_1000ms_Reg) >= Task_State_Base_time_1000ms)
		{
			/* ��⵱ǰ GSM�ź� */
			GSM_Signal_Check();	//ÿ1���Ӹ���һ���ź�ֵ
			
			Heart_Runing_Led();//��������ָʾ��  ÿ��1����˸һ��
			/* ���� ��ǰ ʱ��Ϊ1000ms��׼ʱ�� */
			Base_time_1000ms_Reg = Cur_time;

			/* ���´��������� */
			Sensor_Updata_Data(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
			
			/* ������������ */
			OSSchedLock();
			
			/* ������״̬��� */
			Sensor_State_Check_ALL(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
			
			/* ��ص������ */
			Power_BT_Quantity_Check(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);
			
			/* ��� �Ƿ����µ� �洢�� ��ʪ�� */
			GPRS_Save_Sensor_Data_Check(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
			
			/* ��� GPRS���ݽ���״̬ */
			GPRSSendData_ParseStateCheck();
/***************�̼�����ʱ��ִ�������ϴ���������**********************/
			/* GPRS ���ʹ��������� */
			 GPRS_Send_Sensor_Data();
/***************************END***************************************/					
#if (Wireless_RF_Card_Config_Enable == 0x01)
			/* ��⿨Ƭ���������Ƿ�ʱ */
			RF_Card_Rece_Data_State_Check();
#endif

#if (GPS_Config_Enable == 0x01)
			GPS_Log_Rece_Data_State_Check();
#endif
			/* ���ϵͳ������ʱ���Ƿ�У׼ */
			RTC_System_Clock_Calibration_State_Check();	
			/* ��ʱ���£�1s�� �˵� ���� */
			Flash = FlashMode_DeskTop;	
			/* ������������ */
			OSSchedUnlock();
		}
		
		/* 500MS ���� */
		if((Cur_time-Base_time_500ms_Reg) >= Task_State_Base_time_500ms)
		{
		
			/* ���� ��ǰ ʱ��Ϊ100ms��׼ʱ�� */
			Base_time_500ms_Reg = Cur_time;
			/* ������������ */
			OSSchedLock();
			/* ��ȡ RTC8025 ��ʱ������ */
			Read_System_Date_Time_Data((Times*)&System_Time);
			
			/* ����ǰ��ʱ������ѹ���� */
			Compression_Times_Data((Times*)&System_Time,&Instru_Config_Inf_Tab);
			
			/* ������ ���ݴ洢 ��� */
			Log_Save_Flag_Check(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu,&System_Time);
#if (Print_Function_Enable == 0x01)
			/* �Զ����´�ӡ"��ʼʱ��"��"����ʱ��". */
			App_Print_Updata_Date(System_Time);
#endif

			RTC_System_Clock_Calibration(System_Time); //zwc
			
			/* ������������ */
			OSSchedUnlock();
		}
		
		/* 100MS ���� */
		if((Cur_time-Base_time_100ms_Reg) >= Task_State_Base_time_100ms)
		{
			/* ���� ��ǰ ʱ��Ϊ100ms��׼ʱ�� */
			Base_time_100ms_Reg = Cur_time;
			/* ������������ */
			OSSchedLock();
			
			/* ���� ������ʾ */
			Alarm_Reminding(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);

			
			/* ������������ */
			OSSchedUnlock();		
			/* ��� �������� ״̬:�У���������;��:ѭ�����  */
//			Instru_Config_Inf_Check((GSM_SR_DR*)&GSM_Tab,&GSM_Config_Inf_Data,(void*)GSM_Config_Tab,&Instru_Config_Inf_Tab);
		}
		/* 50MS ���� */
		if((Cur_time-Base_time_50ms_Reg) >= Task_State_Base_time_50ms)
		{
			/* ������������ */
			OSSchedLock();

			/* ������У׼ ͨ�ų�ʱ��� */
			COM_Sensor_Cal_Timeout_Check(&Instru_Fix_Inf_State_Tab,Cur_time-Base_time_50ms_Reg);
			
			/* ���� ��ǰ ʱ��Ϊ50ms��׼ʱ�� */
			Base_time_50ms_Reg = Cur_time;
			
			/* ��Դ����״̬��� */
			Power_ET_Link_State_Check(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);
			/* ���� ��籨��״̬ */
			Power_ET_SET_Alarm_State(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);
			/* ��س��״̬��� */
			Power_BT_Charge_State_Check(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);
			/* ���� ��ص�������״̬ */
			Power_BT_SET_Alarm_State(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);


			/* ������������ */
			OSSchedUnlock();
		}
		/* �ȴ�1MS������ȴ�������CPUʹ��Ȩ�� */
		OSTimeDly(Task_State_Update_Time);
	}
	
#undef Task_State_Update_Time
#undef Task_State_Base_time_50ms
#undef Task_State_Base_time_100ms
#undef Task_State_Base_time_500ms
#undef Task_State_Base_time_1000ms
#undef Task_State_Base_time_10000ms
#undef Task_State_Base_time_60000ms
}

/************************************************************************************************/
/* ������	: AppTaskCOM(�����������������Ϣ����)														*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ͨ�����ù���������������SN�š����ݷ�����IP��ַ���˿ڵ���Ϣ										*/
/* �������� 	: 2014/9/11																			*/
/************************************************************************************************/
void AppTaskCOM(void *p_arg)
{
	INT8U      err;
	COM_SENSOR_CAL_TYPEDEF*   com_sensor_cal_q;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	while (1)  
	{    
		/* �ȴ� ������У׼���� ��Ϣ */
		com_sensor_cal_q = (COM_SENSOR_CAL_TYPEDEF*)OSQPend(COM_Sensor_Cal_EVENT_Q,0,&err);
		
		/* ������Ϣ ״̬ �ж� */
		if(err == OS_ERR_NONE)
		{
			/* ��Ϣ����ȷ�Ľ��� */
			/* ���� ������У׼���� */
			COM_Sensor_Cal_Deal(com_sensor_cal_q);
		}
	} 
} 

/************************************************************************************************/
/* ������	: AppTaskBeep(����������������������)													 	 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����		: ���ݱ������ͷ�����ͬ�ı�������						 									*/
/* �������� 	: 2015/2/27																			*/
/************************************************************************************************/
void AppTaskBeep(void *p_arg)
{
	INT8U      err;
	Beep_QUE*   Beep_QUE_inf;
	OS_Q_DATA	Beep_Q_data;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	while (1)
	{
		/* �ȴ� ���� ��Ϣ */
		Beep_QUE_inf = (Beep_QUE*)OSQPend((OS_EVENT*)Beep_EVENT_Str_Q,0,&err);
		
		/* ������Ϣ ״̬ �ж� */
		if(err == OS_ERR_NONE)
		{
			/* �� ������ */
			Beep(Beep_ON);
			/* �ȴ� ���� ʱ�䡣 */
			OSTimeDly(Beep_QUE_inf->Beep_Open_Time);
			/* �� ������ */
			Beep(Beep_OFF);
			/* �ȴ� ���� ʱ�䡣 */
			OSTimeDly(Beep_QUE_inf->Beep_Close_Time); 
		}
		/* ���� ��Ϣ���� ������ɱ�־ */
		Beep_QUE_inf->EVENT_Getarg = TRUE;
		
		/* ��ѯ ������ ��Ϣ���� */
		err = OSQQuery((OS_EVENT*)Beep_EVENT_Str_Q,&Beep_Q_data);
	
		/* �жϲ�ѯ��� */
		if(err == OS_ERR_NONE)
		{
			/* ��ѯ�ɹ� */
			/* Ѱ����һ�����ͳɹ�����Ϣ */
			if((((OS_Q *)(Beep_EVENT_Str_Q->OSEventPtr))->OSQOut) == (((OS_Q *)(Beep_EVENT_Str_Q->OSEventPtr))->OSQStart))
			{
				/* ���GSM��Ϣ������һ����Ϣָ�� */
				*((u32*)(((OS_Q *)(Beep_EVENT_Str_Q->OSEventPtr))->OSQEnd)-1) = 0;
			}else
			{
				/* ���GSM��Ϣ������һ����Ϣָ�� */
				*((u32*)(((OS_Q *)(Beep_EVENT_Str_Q->OSEventPtr))->OSQOut)-1) = 0;
			}
		}
	}
}

/************************************************************************************************/
/* ������	: AppTaskBeep(�ⱨ��״ָ̬ʾ������)													 	 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����		: ���ݱ��������ñ���ָʾ�ƽ�����˸						 									*/
/* �������� 	: 2015/2/27																			*/
/************************************************************************************************/
void AppTaskLight(void *p_arg)
{
	INT8U      err;
	LIGHT_QUE*   Light_QUE_inf;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	while (1)
	{
		/* �ȴ� ���� ��Ϣ */
		Light_QUE_inf = (LIGHT_QUE*)OSQPend((OS_EVENT*)Light_EVENT_Str_Q,0,&err);
		/* ������Ϣ ״̬ �ж� */
		if(err == OS_ERR_NONE)
		{
			/* �� ����ָʾ�� */
			Light(Light_ON);
			/* �ȴ� ���� ʱ�䡣 */
			OSTimeDly(Light_QUE_inf->Light_Open_Time);
			/* �� ����ָʾ�� */
			Light(Light_OFF);
			/* �ȴ� ���� ʱ�䡣 */
			OSTimeDly(Light_QUE_inf->Light_Close_Time); 
		}
		/* ���� ��Ϣ���� ������ɱ�־ */
		Light_QUE_inf->EVENT_Getarg = TRUE;
	}

}

/************************************************************************************************/
/* ������	: AppTaskADC(ADC���ݲɼ�����)														*/
/* ����		: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����		: ͨ��ADC�ɼ�������ģ���ѹ��Ϣ									*/
/* ��������	: 2014/9/11																			*/
/************************************************************************************************/
void AppTaskADC(void *p_arg)
{
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	
/* �ж��¶Ȳ��������Ƿ����� */
#if ((Hum_Sensor_Function_Config_Enable == 0x01) &&\
	 (Hum_Sensor_Tmp_Comp_Enable == 0x01) &&\
	 (Hum_Sensor_Tmp_Comp_CH > 0))
	/* ���ʪ�ȴ������¶Ȳ������� */
	Humi_Get_Temp_Comp_Value();
#endif
	while (1)
	{    
		/* ����ģ�⿴�Ź� */
		ADC_AWD_Run_Init();
		
		/* �ɼ�ADC���� */
		ADC_Get_Value();
		
		/*����ȴ���	����CPUʹ��Ȩ*/
		OSTimeDly(ADC1_Collect_Interval); //ADC1_Collect_Interval���������ɼ�ʱ��(�ǿɵ���)
		
	}
}

#if (Print_Function_Enable == 0x01)
/************************************************************************************************/
/* ������	: AppTaskPrint(��ӡ������)												*/
/* ����		: (void *p_arg) 																	*/
/* ���		: �� 																				      */
/* ����		: ͨ����ӡ������¼�����ݴ�ӡ����									*/
/* ��������	: 2015/7/6																			*/
/************************************************************************************************/
void AppTaskPrint(void *p_arg)
{
	INT8U      err;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	/* ���� ��ӡ �ź��� */
	Print_QUE_Sem	= OSSemCreate(0);	
	
	while (1)
	{ 
		/* �ȴ� ���� ��Ϣ */
		OSSemPend((OS_EVENT*)Print_QUE_Sem,0,&err);		
		
		/* ������Ϣ ״̬ �ж� */
		if(err == OS_ERR_NONE)
		{
			/* RF_Card ����ɸѡ ִ�ж�Ӧ�Ķ��� */
			Print_Data_Output(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print);
		}

	}
}

/************************************************************************************************/
/* ������	: AppTaskEasyPrint(һ����ӡ����)														*/
/* ����		: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����		: ͨ��������������ϵ�"���"��ʵ��һ����ӡ����										*/
/* ��������	: 2016/04/08																		*/
/************************************************************************************************/
void AppTaskEasyPrint(void *p_arg)
{
	(void)p_arg;
	while (1)
	{ 
		/*ִ��һ����ӡ��������*/
		Easy_Print_Process_Flow(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print);
		/* ������ʱ */
		OSTimeDly(10);
	}
}
#endif

#if (GPS_Config_Enable == 0x01)
/************************************************************************************************/
/* ������	: AppTaskGPS(GPS����)																*/
/* ����		: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����		: ��ȡGPS��λ��Ϣ*/
/* ��������	: 2015/7/30																			*/
/************************************************************************************************/
void AppTaskGPS(void *p_arg)
{
	/* ������Ϣ����״̬ */
	INT8U   err;
	(void)p_arg;   // 'p_arg' ��û���õ�����ֹ��������ʾ����
	/* ����GPS��Ϣ���� */
	GPS_Box = OSMboxCreate((void *)0);
	/* ��ʼ��GPS_Mcu */
	GPS_Mcu_Init();
	/* ��ʼ��GPS_Log */
	GPS_Log_Init();
	/* ��ʼ��GPS_Dev */
	GPS_Dev_Init();
	//���ʱ���ǵ�Դ���ϵ磬ģ�鿪ʼ���У������ģ���ȶ�ǰ������ģ�鷢�����
	//��������ʧ�ܣ�����֪����
	/* ������ʱ�ȴ�GPSģ���ȶ� */
	OSTimeDly(500);
	/* ѡ��GPSģ�������Ϣ���ݡ� */
	GPS_CMD_Config();
	while (1)
	{
		/* �ȴ���Ϣ���� */
		OSMboxPend((OS_EVENT*)GPS_Box,0,&err);	
		/* �ж���Ϣ����״̬�Ƿ���ճɹ��� */
		if(err == OS_ERR_NONE)
		{
			/* GPS��Ϣ������ճɹ���*/
			/* ����GPS��λ��Ϣ��(�������Ϣ������ת������������) */
			GPS_POS_Log_SR.Ana_State = GPS_Log_Ana_Inf();
			
			/* GPS��λ��Ϣ�����ɹ�������GPS��Ϣ����. */
			GPS_App_Get_Data(&GPS_POS_Log_SR,&GPS_POS_Inf);
				
		}
	}
}
#endif
/******************* (C) ZHOU *****END OF FILE****/

