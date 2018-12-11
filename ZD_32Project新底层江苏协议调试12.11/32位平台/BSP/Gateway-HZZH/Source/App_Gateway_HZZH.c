/************************************************************************************************/
/** @file              : App_Gateway_HZZH.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��12��15��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--���ݴ�������Ӧ�ò��ļ�							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��12��15��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "App_Gateway_HZZH.h"
#if (Gateway_HZZH_Enable == 0x01)
#include "Log_Sensor.h"
/********************************* �궨�� *********************************/
/* ���ݴ������������������ص�ʱ�� */
#define AppGW_RestartTimeValue		(20u)

/* ���ݴ������ز���ָ�ʱʱ�� */
#define AppGW_CmdOvertimeValue		(25u)

/* ���ݴ������ؽ������ݼ�ʱʱ��Ƶ�� */
#define AppGWHZZH_TimeOutValue		(100u)

/* ���ݴ������ؿ��Ź�ʱ�� */
#define AppGWHZZH_WatchdogTimeValue	(30000ul)

/* ���ݴ�����������״̬��ѯʱ�� */
#define AppGWHZZH_NStateTimeValue	(60000ul)

/* ���ݴ��������쳣״̬��ѯʱ�� */
#define AppGWHZZH_AStateTimeValue	(1000ul)

/***********************************************************************************/

/* ���ݴ������ع��������� */
typedef enum 
{
	/* ��ʼ������ */
	AppGWHZZHCmd_Init 					= (0u),
	/* �������ع��� */
	AppGWHZZHCmd_Restart				= (1u),
	/* ��������״̬ */
	AppGWHZZHCmd_UpdateState			= (2u),	
	/* ��̽ͷ��Ӧʱ���̽ͷ�������� */
	AppGWHZZHCmd_ReadWkupTimeProbeNum 	= (3u),
	/* ����̽ͷ��Ӧʱ�� */
	AppGWHZZHCmd_SetWkupTime 			= (4u),
	/* ����̽ͷ���� */
	AppGWHZZHCmd_SetProbeNum 			= (5u),

}AppGWHZZHCmdType;

/* ���ݴ������ع��������� */
typedef struct
{
	/* �������� */
	AppGWHZZHCmdType Type;
	/* ������� */
    void* Para;
}AppGWHZZHCmdStruct;

/* ��ʼ�����ع��� ��Ϣ���� ���ݽṹ */
typedef struct
{	
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
}AppGWHZZHInitMboxStruct;

/* �������ع��� ��Ϣ���� ���ݽṹ */
typedef struct
{	
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
}AppGWHZZHRestartMboxStruct;

/* �����ع���״̬���� ��Ϣ���� ���ݽṹ */
typedef struct
{	
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
}AppGWHZZHStateMboxStruct;

/* ��������Ӧʱ���̽ͷ�������� ��Ϣ���� ���ݽṹ */
typedef struct
{	
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
}AppGWHZZHRWTPNMboxStruct;

/* ����������Ӧʱ�� ��Ϣ���� ���ݽṹ */
typedef struct
{	
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ��Ӧʱ�� */
	uint8_t WkupTime;
	/* ̽ͷ���� */
	uint8_t ProbeNum;
}AppGWHZZHSWTMboxStruct;

/* ��������̽ͷ�������� ��Ϣ���� ���ݽṹ */
typedef struct
{	
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ��Ӧʱ�� */
	uint8_t WkupTime;
	/* ̽ͷ���� */
	uint8_t ProbeNum;
}AppGWHZZHSPNMboxStruct;
/* ���ݴ�����Ӧ�ò��������ݽṹ */
typedef struct
{
	

	/* ״̬�Ĵ��� */
	struct
	{
		/* ���ع���״̬ */
		GWHZZH_Result WorkState;

	}SR;
	
	/* ���ݼĴ��� */
	struct
	{
		/* ̽ͷ��������ָ�� */
		uint8_t* ProbeTypep;
		/* ̽ͷID����ָ�� */
		uint8_t* ProbeIDp;
		/* ̽ͷ����ָ�� */
		float* ProbeDatap;
		/* ̽ͷ��������ָ�� */
		uint8_t* ProbeLevelp;
		
		/* ̽ͷ��Ӧʱ�� */
		uint8_t WkupTime;
		/* ̽ͷ���� */
		uint8_t ProbeNum;
		
		/* ��������ʱ�� */
		uint32_t ReceTimeOut[32];
	}DR;

}AppGWHZZHRunParaStruct;

/***********************************************************************************/




/********************************* ���ݴ������� Ӧ�ò��� *********************************/
/********************************* ����������� *********************************/
/* �����ջ */
OS_STK AppTaskGatewayHZZHStk[AppTask_GatewayHZZH_StkSize];


/********************************* ��Ϣ���� *********************************/
/* ��Ϣ���� */
static  OS_EVENT* AppGWHZZH_Q;
/* ��Ϣ���г��� */
#define AppGWHZZH_Q_Len		((u8)OS_MAX_QS)
/* ���ƿ����� */
static void* AppGWHZZH_Grp[AppGWHZZH_Q_Len];

/********************************* ���в��� *********************************/


/********************************* ���ݴ�����Ӧ�ò��������ݶ��� **************/
AppGWHZZHRunParaStruct	AppGWHZZHRunPara =
{	
	
	
	/* ���ع���״̬ */
	GWHZZH_Fail,
	
	
	/* ���ݼĴ��� */
	/* ̽ͷ��������ָ�� */
	(uint8_t*)&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1.Type,
	/* ̽ͷID����ָ�� */
	(uint8_t*)&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,
	/* ̽ͷ����ָ�� */
	&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
	/* ̽ͷ��������ָ�� */
	&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State,
	
	/* ̽ͷ��Ӧʱ�� */
	1u,
	/* ̽ͷ���� */
	50u,
	
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	
};
	
/********************************* Ӧ�ýӿڶ��� *****************************/
AppGatewayHZZHAPIStruct AppGWHZZH_API;

/***********************************************************************************/




#if (1)//���ؿ��Ź������ʱ���ص�����
/* ���ؿ��Ź������ʱ�� ���ָ�� */
static OS_TMR* AppGWHZZH_Watchdog_ptmr;

/************************************************************************************************/
/* ������	: AppGWHZZH_Watchdog_CallBack										 	 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: ���ؿ��Ź������ʱ���ص�����						 								*/
/* ��������	: 2016/12/19																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_Watchdog_CallBack(OS_TMR *ptmr,void *p_arg)
{	
	/* �ر������ʱ��״̬ */
	BOOLEAN tmrState;
	/* ���������ʱ������Ĵ��� */
	INT8U tmrErr;
	
	
	/* �������� */
	AppGWHZZH_API.Restart();
	
	/* �������ؿ��Ź������ʱ��(��ʱ5���Ӻ����) */
	tmrState = OSTmrStart (AppGWHZZH_Watchdog_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
}

/************************************************************************************************/
/* ������	: AppGWHZZH_Restart_CallBack										 	 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: �������������ʱ���ص�����						 									*/
/* ��������	: 2016/12/19																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_FeedWatchdog(void)
{
	/* �ر������ʱ��״̬ */
	BOOLEAN tmrState;
	/* ���������ʱ������Ĵ��� */
	INT8U tmrErr;
	
	
	/* ֹͣ��ʱ��ʼ�����ض�ʱ�� */
	tmrState = OSTmrStop (AppGWHZZH_Watchdog_ptmr,
						  OS_TMR_OPT_NONE,
						  (OS_TMR_CALLBACK)AppGWHZZH_Watchdog_CallBack,
						  &tmrErr);
					
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((OS_ERR_NONE != tmrErr)||(OS_FALSE == tmrState))
	{
		/* ֹͣ��ʱ��ʼ�����ض�ʱ��:ʧ�� */
		__nop();
	}
#endif
	
	/* ɾ�������ʱ�� */
	tmrState = OSTmrDel (AppGWHZZH_Watchdog_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* ���� ���ؿ��Ź������ʱ�� */
	AppGWHZZH_Watchdog_ptmr = 
	OSTmrCreate (
					AppGWHZZH_WatchdogTimeValue,	//�����ʱ����������ʱ����ʱ����ûص�����
					AppGWHZZH_WatchdogTimeValue,	//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_ONE_SHOT,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)AppGWHZZH_Watchdog_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"���ؿ��Ź���ʱ��",				//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);
	
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}	
	
	
	/* ������ʱ��ʼ�����ض�ʱ�� */
	/* �������ؿ��Ź������ʱ��(��ʱ5���Ӻ����) */
	tmrState = OSTmrStart (AppGWHZZH_Watchdog_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
}

#endif

#if (1)//����״̬�����ʱ���ص�����
/* ����״̬�����ʱ�� ���ָ�� */
static OS_TMR* AppGWHZZH_State_ptmr;

/************************************************************************************************/
/* ������	: AppGWHZZH_State_CallBack										 	 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: ����״̬�����ʱ���ص�����						 								*/
/* ��������	: 2016/12/19																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_State_CallBack(OS_TMR *ptmr,void *p_arg)
{	
	
	/* �����ص�ǰ״̬ */
	AppGWHZZH_API.UpdateState();
}

#endif

#if (1)//�������ݼ�ʱ�����ʱ���ص�����
/* �������ݼ�ʱ�����ʱ�� ���ָ�� */
static OS_TMR* AppGWHZZH_TimeOut_ptmr;
/************************************************************************************************/
/* ������	: AppGWHZZH_TimeOut_CallBack										 	 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: �������ݼ�ʱ�����ʱ���ص�����						 									*/
/* ��������	: 2016/12/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_TimeOut_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* ������ */
	uint8_t num;
	
	/* ��ʱʱ��Ĵ��� */
	for(num=0;num<32;num++)
	{
		/* �жϵ�ǰ��ͨ������ */
		if((Sensor_GW_HZZH_Tmp != *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL)))&&
		   (Sensor_GW_HZZH_Hum != *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL))))
		{
			/* �Ǻ��ݴ����������� */
			continue;
		}
		/* �ж�ʱ��Ĵ��������Ƿ񳬳� */
		if(Gateway_HZZH_ReceTimeout > AppGWHZZHRunPara.DR.ReceTimeOut[num])
		{
			/* ��������δ��ʱ������ʱ��Ĵ����� */
			AppGWHZZHRunPara.DR.ReceTimeOut[num]++;
		}else
		{
			/* �������ݳ�ʱ���������ݼĴ���ֵΪ��ʱֵ�� */
			*((float*)(((uint8_t*)AppGWHZZHRunPara.DR.ProbeDatap)+num*sizeof(INSTRU_SENSOR_CHANNEL_FIX))) = 
			Gateway_HZZH_TimeoutPara;
		}
	}
}

#endif

#if (1)//���ݴ�������Ӧ�ú���
/************************************************************************************************/
/* ������	: AppGW_GetProbeDataTemp										 	 				*/
/* ����  	: data:̽ͷ����							 											*/
/* ���		: ̽ͷ�¶�����																		*/
/* ����	 	: ��̽ͷ�����е��¶����ݽ�������						 								*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static float AppGW_GetProbeDataTemp(AppGatewayHZZHProbeDataStruct* data)
{

	/* �������е�̽ͷ�¶����ݽ������� */
	return ((data->TempPM ? -1 : 1)*((float)data->TempInteger+((float)data->TempDecimal)*0.1));
}

/************************************************************************************************/
/* ������	: AppGW_GetProbeDataHum										 	 					*/
/* ����  	: data:̽ͷ����							 											*/
/* ���		: ̽ͷʪ������																		*/
/* ����	 	: ��̽ͷ�����е�ʪ�����ݽ�������						 								*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static float AppGW_GetProbeDataHum(AppGatewayHZZHProbeDataStruct* data)
{
	/* �������е�̽ͷ�¶����ݽ������� */
	return ((float)data->HumiInteger+((float)data->HumiDecimal)*0.1);
}

/************************************************************************************************/
/* ������	: AppGW_GetProbeLevel										 	 					*/
/* ����  	: data:̽ͷ����							 											*/
/* ���		: ̽ͷ��������																		*/
/* ����	 	: ��̽ͷ�����еĵ������ݽ�������						 								*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static uint8_t AppGW_GetProbeLevel(AppGatewayHZZHProbeDataStruct* data)
{
	/* ��Ƭ��ص�ѹֵ */
	float gVoltage;
	/* �����ٷֱ� */
	uint8_t gLevel;
	
	
	/* �������е�̽ͷ�¶����ݽ������� */
	gVoltage =  ((float)data->PowerInteger+((float)data->PowerDecimal)*0.1);
	
	/* ��������ٷֱ� */
	if((3.0) < gVoltage)
	{
		/* ����Ϊ100% */
		gLevel = 100;
	}else if(((3.0) >= gVoltage)&&((2.6) < gVoltage))
	{
		/* ��������ٷֱ� */
		gLevel = ((gVoltage-2.6)*100*5)/2;
	}else if((2.6) >= gVoltage)
	{
		/* ��������ٷֱ� */
		gLevel = 0;
	}
	
	return gLevel;
}

#endif

#if (1)//���ݴ���������Ӧ���ܺ���
/************************************************************************************************/
/* ������	: AppGWHZZH_ProbeDataACKHandle										 	 			*/
/* ����  	: id:̽ͷID��data:̽ͷ����							 								*/
/* ���		: ��																				*/
/* ����	 	: ����̽ͷ������Ӧ��						 											*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_ProbeDataACKHandle(uint8_t* id,uint8_t* data)
{	
	/* ������ */
	uint8_t num;
	
	
	/* �������õ�̽ͷID�� */
	for(num = 0;num < Sensor_CH_Config_Num;num++)
	{
		/* �жϵ�ǰͨ������ */
		if((Sensor_GW_HZZH_Tmp != *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL)))&&
		   (Sensor_GW_HZZH_Hum != *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL))))
		{
			/* ̽ͷ���Ͳ��Ǵ���̽ͷ������������һ��̽ͷ�� */
			continue;
		}
		
		/* �жϵ�ǰͨ����ID���Ƿ�����յ�̽ͷID�Ƿ�һ�¡� */
		if(((AppGWHZZHRunPara.DR.ProbeIDp+num*sizeof(INSTRU_SENSOR_CHANNEL))[0] != id[3])||
		   ((AppGWHZZHRunPara.DR.ProbeIDp+num*sizeof(INSTRU_SENSOR_CHANNEL))[1] != id[2])||
		   ((AppGWHZZHRunPara.DR.ProbeIDp+num*sizeof(INSTRU_SENSOR_CHANNEL))[2] != id[1])||
		   ((AppGWHZZHRunPara.DR.ProbeIDp+num*sizeof(INSTRU_SENSOR_CHANNEL))[3] != id[0]))
		{
			/* ��ǰ̽ͷID������յ�̽ͷID����ͬ������������һ��̽ͷID�� */
			continue;
		}
		
		/* ��ǰ̽ͷID������յ�̽ͷID��ͬ��������Ӧͨ�������ݺ͵����� */
		/* �жϵ�ǰͨ������(�¶�/ʪ��) */
		if(Sensor_GW_HZZH_Tmp == *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL)))
		{
			/* �¶�̽ͷ�������ݵ��е��¶�����д��Ӧ�üĴ��� */
			*((float*)(((uint8_t*)AppGWHZZHRunPara.DR.ProbeDatap)+num*sizeof(INSTRU_SENSOR_CHANNEL_FIX))) = 
			AppGW_GetProbeDataTemp((AppGatewayHZZHProbeDataStruct*)data);
		}else if(Sensor_GW_HZZH_Hum == *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL)))
		{
			/* ʪ��̽ͷ�������ݵ��е��¶�����д��Ӧ�üĴ��� */
			*((float*)(((uint8_t*)AppGWHZZHRunPara.DR.ProbeDatap)+num*sizeof(INSTRU_SENSOR_CHANNEL_FIX))) = 
			AppGW_GetProbeDataHum((AppGatewayHZZHProbeDataStruct*)data);
		}
		/* �����ݵ��еĵ�������д��Ӧ�üĴ��� */
		*(AppGWHZZHRunPara.DR.ProbeLevelp+num*sizeof(INSTRU_SENSOR_CHANNEL_FIX)) = 
		AppGW_GetProbeLevel((AppGatewayHZZHProbeDataStruct*)data);
		
		/* �����ǰ̽ͷ�������ݳ�ʱʱ�� */
		AppGWHZZHRunPara.DR.ReceTimeOut[num] = 0;
	}
	
	/* �ɹ����յ�һ֡���ݣ�������Ź���ʱ��ֵ(ι��) */
	AppGWHZZH_FeedWatchdog();
}

/************************************************************************************************/
/* ������	: AppGWHZZH_SetWkupTimeProbeNumACKHandle											*/
/* ����  	: ��														 						*/
/* ���		: ��																				*/
/* ����	 	: ��������̽ͷ��Ӧʱ���̽ͷ��������Ӧ��						 						*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_SetWkupTimeProbeNumACKHandle(void)
{	
	/* Ŀǰû�ж�Ӧ���� */
	__nop();
}

/************************************************************************************************/
/* ������	: AppGWHZZH_ReadWkupTimeProbeNumACKHandle											*/
/* ����  	: wkupTime:��Ӧʱ�䣬data:̽ͷ����							 						*/
/* ���		: ��																				*/
/* ����	 	: �����̽ͷ��Ӧʱ���̽ͷ��������Ӧ��						 							*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_ReadWkupTimeProbeNumACKHandle(uint8_t wkupTime,uint8_t probeNum)
{	
	
	/* ����̽ͷ��Ӧʱ���̽ͷ���� */
	AppGWHZZHRunPara.DR.WkupTime = wkupTime;
	/* ����̽ͷ���� */
	AppGWHZZHRunPara.DR.ProbeNum = probeNum;
}

#endif

#if (1)//���ݴ������ؽӿں���
/************************************************************************************************/
/* ������	: AppGWHZZH_Restart										 	 						*/
/* ����  	: ��							 													*/
/* ���		: ��																				*/
/* ����	 	: ��������									 										*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void	AppGWHZZH_Restart(void)
{
	/* ������Ϣ����״̬��Ϣ */
	INT8U gQerr;
	/* ��̽ͷ���� ��Ϣ���� ���� */
	static AppGWHZZHRestartMboxStruct gRestartMbox;
	/* ��������ṹ */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* �������� */
	gGWHZZHCmd.Type = AppGWHZZHCmd_Restart;
	/* ���ò���ָ�� */
	gGWHZZHCmd.Para = &gRestartMbox;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPostFront (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();

	}
#endif
}

/************************************************************************************************/
/* ������	: AppGWHZZH_UpdateState										 	 						*/
/* ����  	: ��							 													*/
/* ���		: ��																				*/
/* ����	 	: �����ص�ǰ����״̬									 								*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_UpdateState(void)
{
	/* ������Ϣ����״̬��Ϣ */
	INT8U gQerr;
	/* ��Ϣ������ */
	OS_EVENT* gMbox_Handle;
	/* ɾ����Ϣ����ָ�� */
	OS_EVENT* gDelMbox;
	/* ��Ϣ����ָ�� */
	void* gMbox_p;
	/* �����ڴ�״̬��Ϣ */
	INT8U gMboxerr;
	/* ��̽ͷ���� ��Ϣ���� ���� */
	static AppGWHZZHStateMboxStruct gStateMbox;
	/* ��������ṹ */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	
	/* �ڴ�����ɹ� */
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* �������� */
	gGWHZZHCmd.Type = AppGWHZZHCmd_UpdateState;
	/* ���ò���ָ�� */
	gGWHZZHCmd.Para = &gStateMbox;
	/* ����������� */
	gStateMbox.MboxHandle = gMbox_Handle;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GWHZZH_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGWHZZH_GetState										 	 					*/
/* ����  	: ��							 													*/
/* ���		: ��ǰ���ݴ�������״̬																*/
/* ����	 	: �����ݴ�������״̬																	*/
/* ��������	: 2017/01/11																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_GetState(void)
{

	/* ��������״̬ */
	return AppGWHZZHRunPara.SR.WorkState;
}

/************************************************************************************************/
/* ������	: AppGWHZZH_ReadWkupTimeProbeNum										 	 		*/
/* ����  	: ��							 													*/
/* ���		: ��																				*/
/* ����	 	: �����ݴ�������̽ͷ��Ӧʱ���̽ͷ����													*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_ReadWkupTimeProbeNum(uint16_t* time,uint16_t* probeNum)
{
	/* ������Ϣ����״̬��Ϣ */
	INT8U gQerr;
	/* ��Ϣ������ */
	OS_EVENT* gMbox_Handle;
	/* ɾ����Ϣ����ָ�� */
	OS_EVENT* gDelMbox;
	/* ��Ϣ����ָ�� */
	void* gMbox_p;
	/* �����ڴ�״̬��Ϣ */
	INT8U gMboxerr;
	/* ��̽ͷ���� ��Ϣ���� ���� */
	static AppGWHZZHRWTPNMboxStruct gRWTPNMbox;
	/* ��������ṹ */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* �ж����ع���״̬ */
	if(GWHZZH_Fail == AppGWHZZHRunPara.SR.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
	
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	
	/* �ڴ�����ɹ� */
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* �������� */
	gGWHZZHCmd.Type = AppGWHZZHCmd_ReadWkupTimeProbeNum;
	/* ���ò���ָ�� */
	gGWHZZHCmd.Para = &gRWTPNMbox;
	/* ����������� */
	gRWTPNMbox.MboxHandle 	= gMbox_Handle;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* �жϻ�ȡ��Ϣ״̬	*/
	if (GWHZZH_Success == (*(GWHZZH_Result*)gMbox_p))
	{
		/* ���û���ʱ��ͱ�ǩ���� */
		*time = AppGWHZZHRunPara.DR.WkupTime*15;
		*probeNum = AppGWHZZHRunPara.DR.ProbeNum*10;
	}else
	{
		/* ���û���ʱ��ͱ�ǩ����Ϊ0 */
		*time = 0;
		*probeNum = 0; 
	}

	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GWHZZH_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGWHZZH_SetWkupTime										 	 					*/
/* ����  	: time:							 													*/
/* ���		: ��																				*/
/* ����	 	: ���ú��ݴ�������̽ͷ��Ӧʱ��														*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_SetWkupTime(uint16_t time,uint16_t probeNum)
{
	/* ������Ϣ����״̬��Ϣ */
	INT8U gQerr;
	/* ��Ϣ������ */
	OS_EVENT* gMbox_Handle;
	/* ɾ����Ϣ����ָ�� */
	OS_EVENT* gDelMbox;
	/* ��Ϣ����ָ�� */
	void* gMbox_p;
	/* �����ڴ�״̬��Ϣ */
	INT8U gMboxerr;
	/* ����̽ͷ��Ӧʱ������ ��Ϣ���� ���� */
	static AppGWHZZHSWTMboxStruct gSWTMbox;
	/* ��������ṹ */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* �ж����ع���״̬ */
	if(GWHZZH_Fail == AppGWHZZHRunPara.SR.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
	
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	
	/* �ڴ�����ɹ� */
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* �������� */
	gGWHZZHCmd.Type = AppGWHZZHCmd_SetWkupTime;
	/* ���ò���ָ�� */
	gGWHZZHCmd.Para = &gSWTMbox;
	
	/* ����������� */
	gSWTMbox.MboxHandle = gMbox_Handle;
	/* ������Ӧʱ����� */
	gSWTMbox.WkupTime = (time/15);
	/* ����̽ͷ�������� */
	gSWTMbox.ProbeNum = (probeNum/10);
	
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GWHZZH_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGWHZZH_SetProbeNum										 	 					*/
/* ����  	: time:							 													*/
/* ���		: ��																				*/
/* ����	 	: ���ú��ݴ�������̽ͷ����															*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_SetProbeNum(uint16_t time,uint16_t probeNum)
{
	/* ������Ϣ����״̬��Ϣ */
	INT8U gQerr;
	/* ��Ϣ������ */
	OS_EVENT* gMbox_Handle;
	/* ɾ����Ϣ����ָ�� */
	OS_EVENT* gDelMbox;
	/* ��Ϣ����ָ�� */
	void* gMbox_p;
	/* �����ڴ�״̬��Ϣ */
	INT8U gMboxerr;
	/* ����̽ͷ�������� ��Ϣ���� ���� */
	static AppGWHZZHSPNMboxStruct gSPNMbox;
	/* ��������ṹ */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* �ж����ع���״̬ */
	if(GWHZZH_Fail == AppGWHZZHRunPara.SR.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
	
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	
	/* �ڴ�����ɹ� */
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* �������� */
	gGWHZZHCmd.Type = AppGWHZZHCmd_SetWkupTime;
	/* ���ò���ָ�� */
	gGWHZZHCmd.Para = &gSPNMbox;
	
	/* ����������� */
	gSPNMbox.MboxHandle = gMbox_Handle;
	/* ������Ӧʱ����� */
	gSPNMbox.WkupTime = time/15;
	/* ����̽ͷ�������� */
	gSPNMbox.ProbeNum = probeNum/10;
	
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
		/* ��������ִ��ʧ�ܽ�� */
		return GWHZZH_Fail;
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GWHZZH_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGW_Init										 	 							*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ�����ݴ������ع���					 											*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_Init(void)
{
	/* ���������ʱ������Ĵ��� */
	INT8U tmrErr;
	/* �������������ʱ��״̬ */
	BOOLEAN tmrState;
	
	
	/* ��ʼ�����ݴ��������߼���ӿ� */
	LogGW_InitAPI(AppGWHZZH_ProbeDataACKHandle,
				  AppGWHZZH_SetWkupTimeProbeNumACKHandle,
				  AppGWHZZH_ReadWkupTimeProbeNumACKHandle);

	/* �������ؿ��Ź������ʱ��(��ʱ5���Ӻ����) */
	tmrState = OSTmrStart (AppGWHZZH_Watchdog_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* �����������ݼ�ʱ�����ʱ�� */
	tmrState = OSTmrStart (AppGWHZZH_TimeOut_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* ��������״̬�����ʱ�� */
	tmrState = OSTmrStart (AppGWHZZH_State_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	
	
	LogGWHZZHAPI.Init();
	
}


#endif

/************************************************************************************************/
/* ������	: AppGW_Init										 	 							*/
/* ����  	: ��							 													*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��Ӧ�ò����������Ϣ						 									*/
/* ��������	: 2016/12/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_EnvironInit(void)
{
	/* ���������ʱ������Ĵ��� */
	INT8U tmrErr;
	
	
	/* �ȳ�ʼ�� ϵͳ�ȹصĲ��� */
	/* ���� �Ͼ�ʢ��������Ϣ���� */
	AppGWHZZH_Q = OSQCreate(AppGWHZZH_Grp,AppGWHZZH_Q_Len);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if(AppGWHZZH_Q == (OS_EVENT *)0)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	
	/* ���� ���ؿ��Ź������ʱ�� */
	AppGWHZZH_Watchdog_ptmr = 
	OSTmrCreate (
					AppGWHZZH_WatchdogTimeValue,	//�����ʱ����������ʱ����ʱ����ûص�����
					AppGWHZZH_WatchdogTimeValue,	//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_ONE_SHOT,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)AppGWHZZH_Watchdog_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"���ؿ��Ź���ʱ��",				//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);
	
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}
	
	/* �����������ݼ�ʱ�����ʱ�� */
	AppGWHZZH_TimeOut_ptmr = 
	OSTmrCreate (
					AppGWHZZH_TimeOutValue,			//�����ʱ����������ʱ����ʱ����ûص�����
					AppGWHZZH_TimeOutValue,			//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_PERIODIC,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)AppGWHZZH_TimeOut_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"�������ݼ�ʱ��ʱ��",				//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);
	
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}
	
	/* ���� ����״̬�����ʱ�� */
	AppGWHZZH_State_ptmr = 
	OSTmrCreate (
					(10u),							//�����ʱ����������ʱ����ʱ����ûص�����
					AppGWHZZH_AStateTimeValue,		//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_PERIODIC,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)AppGWHZZH_State_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"����״̬��ʱ��",				//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);
							
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}
	
	
	
}

/************************************************************************************************/
/* ������	: AppGWHZZH_InitAPI										 	 							*/
/* ����  	: initPara:��ʼ�����ز���															*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ������						 													*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_InitAPI(void)
{

	
	/* ��ʼ������������� */
	AppGWHZZH_EnvironInit();
	
	/* ��ʼ���ӿ� */
	AppGWHZZH_API.Init		 = AppGWHZZH_Init;
	
	/* �����ӿ� */
	AppGWHZZH_API.Restart	 = AppGWHZZH_Restart;
	
	/* �������ص�ǰ����״̬ */
	AppGWHZZH_API.UpdateState= AppGWHZZH_UpdateState;
	
	/* ��ȡ���ص�ǰ����״̬ */
	AppGWHZZH_API.GetState	 = AppGWHZZH_GetState;
	
	/* ����Ӧʱ���̽ͷ�����ӿ� */
	AppGWHZZH_API.ReadWkupTimeProbeNum = AppGWHZZH_ReadWkupTimeProbeNum;
	
	/* ������Ӧʱ�� */
	AppGWHZZH_API.SetWkupTime = AppGWHZZH_SetWkupTime;
	
	/* ����̽ͷ���� */
	AppGWHZZH_API.SetProbeNum = AppGWHZZH_SetProbeNum;
	
}

#if (1)//���ݴ�������������
/************************************************************************************************/
/* ������	: AppGWHZZH_CmdHandle																*/
/* ����  	: cmd:Ӧ������ 																		*/
/* ���		: �� 																				*/
/* ����	 	: �Ͼ�ʢ�����ش�������																*/
/* �������� 	: 2016/12/16																		*/
/************************************************************************************************/
static void AppGWHZZH_CmdHandle(AppGWHZZHCmdStruct* cmd)
{
	/* ����״̬�Ĵ��� */
	const static GWHZZH_Result gSuccess = GWHZZH_Success;
	const static GWHZZH_Result gFail 	= GWHZZH_Fail;
				 GWHZZH_Result gState;
	/* ��Ϣ����״̬��Ϣ */
	INT8U gMboxerr;
	/* ������ָ�� */
	OS_EVENT* gMboxp;
	
	
	/* �ж��������� */
	switch(cmd->Type)
	{
		/* ��ʼ�����ݴ������� */
 		case AppGWHZZHCmd_Init:
 		{
			/* ��ʼ�����ݴ��������߼��� */
			LogGWHZZHAPI.Init();
			return;
 		}
		
		/* �������ݴ������� */
 		case AppGWHZZHCmd_Restart:
 		{
			/* �������ݴ��������߼��� */
			LogGWHZZHAPI.Restart();
			/* �ȴ�ϵͳ�����ȶ� */
			OSTimeDly(AppGW_RestartTimeValue);
			return;
 		}
		
		/* �����ݴ������ع���״̬ */
 		case AppGWHZZHCmd_UpdateState:
 		{
			/* �����ݴ�����������ָ�� */
			AppGWHZZHStateMboxStruct* gStatep = cmd->Para;
			
			
			/* ������Ϣ������ */
			gMboxp = gStatep->MboxHandle;
			/* �������ݴ��������߼��� */
			gState = LogGWHZZHAPI.State();
			
			/* �жϹ���״̬�Ƿ�仯 */
			if(AppGWHZZHRunPara.SR.WorkState != gState)
			{
				/* ֹͣ״̬�����ʱ�� */
				/* �ر������ʱ��״̬ */
				BOOLEAN tmrState;
				/* ���������ʱ������Ĵ��� */
				INT8U tmrErr;
				
				
				/* ֹͣ��ʱ��ʼ�����ض�ʱ�� */
				tmrState = OSTmrStop (AppGWHZZH_State_ptmr,
									  OS_TMR_OPT_NONE,
									  (OS_TMR_CALLBACK)AppGWHZZH_State_CallBack,
									  &tmrErr);
				
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
				if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
				{__nop();}
#endif
				
				/* ɾ�������ʱ�� */
				tmrState = OSTmrDel (AppGWHZZH_State_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
				if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
				{__nop();}
#endif
				
				/* ����״̬��ʲô�仯���ж����صĹ���״̬�� */
				if(GWHZZH_Success == gState)
				{
					/* ����״̬���������������Ĳ�ѡʱ�� */
					/* ���� ����״̬�����ʱ�� */
					AppGWHZZH_State_ptmr = 
					OSTmrCreate (
									AppGWHZZH_NStateTimeValue,		//�����ʱ����������ʱ����ʱ����ûص�����
									AppGWHZZH_NStateTimeValue,		//�����ڶ�ʱģʽ�£����ûص�����������
									OS_TMR_OPT_PERIODIC,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
									(OS_TMR_CALLBACK)AppGWHZZH_State_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
									(void*)0,						//�ص���������
									"����״̬��ʱ��",				//�ص���������
									&tmrErr							//���������ʱ������ִ�н���Ĵ���
								);
											
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))					
					/* �ж������ʱ������״̬ */
					if(tmrErr != OS_ERR_NONE)
					{__nop();/* ʧ�� */}
#endif
					
				}else
				{
					/* ����״̬���������������Ĳ�ѡʱ�� */
					/* ���� ����״̬�����ʱ�� */
					AppGWHZZH_State_ptmr = 
					OSTmrCreate (
									AppGWHZZH_NStateTimeValue,		//�����ʱ����������ʱ����ʱ����ûص�����
									AppGWHZZH_AStateTimeValue,		//�����ڶ�ʱģʽ�£����ûص�����������
									OS_TMR_OPT_PERIODIC,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
									(OS_TMR_CALLBACK)AppGWHZZH_State_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
									(void*)0,						//�ص���������
									"����״̬��ʱ��",				//�ص���������
									&tmrErr							//���������ʱ������ִ�н���Ĵ���
								);
					
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))					
					/* �ж������ʱ������״̬ */
					if(tmrErr != OS_ERR_NONE)
					{__nop();/* ʧ�� */}
#endif
				}
				
				/* �������ع���״̬ */
				AppGWHZZHRunPara.SR.WorkState = gState;
				/* ������ʱ��ʼ�����ض�ʱ�� */
				tmrState = OSTmrStart (AppGWHZZH_State_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
				if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
				{__nop();}
#endif
			}
			break;
 		}
		
		/* �����ݴ���������Ӧʱ���̽ͷ���� */
 		case AppGWHZZHCmd_ReadWkupTimeProbeNum:
 		{
			/* �����ݴ�����������ָ�� */
			AppGWHZZHRWTPNMboxStruct* gRWTPNp = cmd->Para;

			
			/* ������Ϣ������ */
			gMboxp = gRWTPNp->MboxHandle;
			/* �����ݴ���������Ӧʱ���̽ͷ���� */
			gState = LogGWHZZHAPI.ReadWkupTimeProbeNum();
			break;
 		}
		
		/* ���ú��ݴ���������Ӧʱ�� */
 		case AppGWHZZHCmd_SetWkupTime:
 		{
			/* �����ݴ�����������ָ�� */
			AppGWHZZHSWTMboxStruct* gSWTp = cmd->Para;
			
			
			/* ������Ϣ������ */
			gMboxp = gSWTp->MboxHandle;
			/* �����ݴ���������Ӧʱ���̽ͷ���� */
			gState = LogGWHZZHAPI.SetWkupTime(gSWTp->WkupTime,gSWTp->ProbeNum);
			break;
 		}
		
		/* ���ú��ݴ�������̽ͷ���� */
 		case AppGWHZZHCmd_SetProbeNum:
 		{	
			/* �����ݴ�����������ָ�� */
			AppGWHZZHSPNMboxStruct* gSPNp = cmd->Para;
			
			
			/* ������Ϣ������ */
			gMboxp = gSPNp->MboxHandle;
			/* �����ݴ���������Ӧʱ���̽ͷ���� */
			gState = LogGWHZZHAPI.SetWkupTime(gSPNp->WkupTime,gSPNp->ProbeNum);
			break;
 		}
		
		default:
		{
			/* δ�趨���� �������� */
			__nop();
			return;
		}
	}
	
	/* �ж�����ִ��״̬ */
	if(gState == GWHZZH_Success)
	{
		/* ����ִ�гɹ���������Ϣ���䡣 */
		gMboxerr = OSMboxPost(gMboxp,((void*)&gSuccess));
	}else
	{
		/* ����ִ��ʧ�� */
		gMboxerr = OSMboxPost(gMboxp,((void*)&gFail));
	}
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gMboxerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* ������	: AppTaskGatewayHZZH																*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: �Ͼ�ʢ�����ش�������																*/
/* �������� 	: 2016/12/16																		*/
/************************************************************************************************/
void AppTaskGatewayHZZH(void *p_arg)
{
	/* ��������ṹ */
	AppGWHZZHCmdStruct* gAppGWHZZHCmdDatap;
	/* �������״̬��Ϣ */
	INT8U gQerr;
	(void)p_arg;
	
	
	/* ��ʼ��Ӧ�ò�����API���� */
	AppGWHZZH_InitAPI();
	
	/* �������أ���ʼ������ */
	AppGWHZZH_API.Init();
	
	while (1)
	{
		/* �ȴ� ���� ��Ϣ */
		gAppGWHZZHCmdDatap = (AppGWHZZHCmdStruct*)OSQPend(AppGWHZZH_Q,0,&gQerr);
		
		/* ������Ϣ ״̬ �ж� */
		if(gQerr == OS_ERR_NONE)
		{
			/* �������ز������� */
			AppGWHZZH_CmdHandle(gAppGWHZZHCmdDatap);
		}
	}

}
#endif
/***********************************************************************************/



#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
