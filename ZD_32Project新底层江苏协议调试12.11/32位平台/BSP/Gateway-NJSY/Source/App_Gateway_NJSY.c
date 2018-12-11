/************************************************************************************************/
/** @file              : App_Gateway_NJSY.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��18��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--�Ͼ�ʢ������Ӧ�ò��ļ�							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��11��18��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "App_Gateway_NJSY.h"
#if (Gateway_NJSY_Enable == 0x01)

/********************************* �궨�� *********************************/
/* ���ؽ��ճ�ʱʱ�� */
#define AppGW_CmdOvertimeValue	(LogGW_ReceOvertimeValue*10+100)
/* ��������ʱ�� */
#define AppGW_RestartTimeValue	(LogGW_RestartTimeValue*10)

/********************************* �Ͼ�ʢ������ �������� *********************************/
#if (1)
/* ���ع��������� */
typedef enum 
{
	/* ��ʼ������ */
	GWCMD_Init 			= (0u), 
	/* �������ع��� */
	GWCMD_Restart		= (1u),
	/* �������ع��� */
	GWCMD_ConfigFunc	= (2u),
	/* ������ */
	GWCMD_ReadData 		= (3u), 
	/* ������ */
	GWCMD_ReadLevel 	= (4u), 
	/* ������������ */
	GWCMD_UpdataProbeData = (5u), 
	/* ��������״̬ */
	GWCMD_UpdataState   = (6u),
	/* �������ص�ַ */
	GWCMD_SetAdder 		= (7u),
	/* ���������ŵ�(����) */
	GWCMD_SetChannel 	= (8u),
	/* ��������������Чʱ�� */
	GWCMD_SetActiveTime = (9u),
	/* �������ع���ģʽ */
	GWCMD_SetMode 		= (10u),
	/* ��������ͨ�Ų����� */
	GWCMD_SetBaudrate 	= (11u),
}GWCMDType;

typedef struct
{
	/* �������� */
	GWCMDType Type;
	/* ������� */
    void* Para;
}GWCMDStruct;

/* ��ʼ������ ��Ϣ���� ���ݽṹ */
typedef struct
{	
	/* ���ص�ַ */
	uint8_t Adder;
	/* �ŵ�(����) */
	uint8_t Channel;
	/* ������Чʱ�� */
	uint8_t ActiveTime;
	/* ����ģʽ������/���ԣ� */
	GW_Mode Mode;
	/* ͨ�Ų����� */
	uint32_t Baudrate;
}GWInitStruct;

/* �������ع��� ��Ϣ���� ���ݽṹ */
typedef struct
{	
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
}GWRestartStruct;

/* �������ع��� ��Ϣ���� ���ݽṹ */
typedef struct
{	
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
}GWConfigFuncStruct;

/* ��̽ͷ���� ��Ϣ���� ���ݽṹ */
typedef struct
{
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ����ָ�� */
	float* Data;
	/* ����̽ͷ���� */
	GW_ProbeType Type;
	/* ̽ͷ��� */
	uint8_t No;
}GWReadProbeStruct;

/* ��̽ͷ����״̬ ��Ϣ���� ���ݽṹ */
typedef struct
{
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ����ָ�� */
	GW_Level* Level;
	/* ̽ͷ��� */
	uint8_t No;
}GWReadLevelStruct;

/* ��������״̬ ��Ϣ���� ���ݽṹ */
typedef struct
{
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ���ص�ַ */
	uint8_t* Adder;
}GWUpdataStateStruct;

/* ������������ ��Ϣ���� ���ݽṹ */
typedef struct
{
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
}GWUpdataProbeDataStruct;

/* �������ص�ַ ��Ϣ���� ���ݽṹ */
typedef struct
{
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ���ص�ַ */
	uint8_t Adder;
	
}GWSetAdderStruct;

/* ���������ŵ�(����) ��Ϣ���� ���ݽṹ */
typedef struct
{
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ����ͨ���ŵ� */
	uint8_t Channel;
	
}GWSetChannelStruct;

/* ��������������Чʱ�� ��Ϣ���� ���ݽṹ */
typedef struct
{
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ����������Чʱ�� */
	uint8_t ActiveTime;
	
}GWSetActiveTimeStruct;

/* �������ع���ģʽ ��Ϣ���� ���ݽṹ */
typedef struct
{
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ���ع���ģʽ */
	GW_Mode Mode;
	
}GWSetModeStruct;

/* ��������ͨ�Ų����� ��Ϣ���� ���ݽṹ */
typedef struct
{
	/* ��Ϣ����ָ�� */
	OS_EVENT* MboxHandle;
	/* ����ͨ�Ų����� */
	uint32_t Baudrate;
}GWSetBaudrateStruct;


#endif
/********************************* �Ͼ�ʢ������ Ӧ�ò��� *********************************/
/********************************* ����������� *********************************/
/* �����ջ */
OS_STK AppTaskGatewayNJSYStk[AppTask_GatewayNJSY_StkSize];


/********************************* ��Ϣ���� *********************************/
/* ��Ϣ���� */
static  OS_EVENT* GW_NJSY_Q;
/* ��Ϣ���г��� */
#define GW_NJSY_Q_Len		((u8)OS_MAX_QS)
/* ���ƿ����� */
static void* GW_NJSY_Grp[GW_NJSY_Q_Len];

/********************************* ���в��� *********************************/
static AppGWInitPara* AppGWRunPara = 
(AppGWInitPara*)&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway;

/********************************* Ӧ�ýӿڶ��� *********************************/
AppGatewayAPIStruct AppGW_API;

#if (1)//��ʱ��������̽ͷ���ݻص�����
/* ��ʱ��������̽ͷ���������ʱ�� ���ָ�� */
static OS_TMR* AppGW_UpdataProbeData_ptmr;
	
/************************************************************************************************/
/* ������	: AppGW_UpdataProbeData_CallBack													*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: ��ʱ��������̽ͷ����						 										*/
/* ��������	: 2016/11/21																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGW_UpdataProbeData_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* ��������̽ͷ���� */
	AppGW_API.UpdataProbeData();
}

#endif

#if (1)//��ʱ�������ع���״̬�ص�����
/* ��ʱ�������ع���״̬�����ʱ�� ���ָ�� */
static OS_TMR* AppGW_WorkState_ptmr;
	
/************************************************************************************************/
/* ������	: AppGW_UpdataState_CallBack										 		 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: ��ʱ����ȡ���ع���״̬						 										*/
/* ��������	: 2016/11/21																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGW_UpdataState_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* ��������̽ͷ���� */
	AppGW_API.UpdataState();
}

#endif

#if (1)//��ʱ�������ع��ܻص�����
/* ��ʱ�������ع��������ʱ�� ���ָ�� */
static OS_TMR* AppGW_ReConfigFunc_ptmr;
	
/************************************************************************************************/
/* ������	: AppGW_ReConfigFunc_CallBack										 		 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: ��ʱ��ȡ���ع���״̬						 										*/
/* ��������	: 2016/11/21																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGW_ReConfigFunc_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* �������أ���ʼ������ */
	AppGW_API.ConfigFunc();
}

#endif
/***********************************************************************************/
#if (1)//���ع��ܺ���
/************************************************************************************************/
/* ������	: AppGW_Init										 	 							*/
/* ����  	: initPara:��ʼ�����ز���															*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ������						 										*/
/* ��������	: 2016/11/24																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGW_Init(AppGWInitPara* initPara)
{	
	/* ������Ϣ����״̬��Ϣ */
	INT8U gQerr;
	/* ��̽ͷ���� ��Ϣ���� ���� */
	static GWInitStruct gInit;
	/* ��������ṹ */
	static GWCMDStruct gCMD;
	
	
	/* �������� */
	gCMD.Type = GWCMD_Init;
	gCMD.Para = &gInit;
	
	/* ���ص�ַ */
	gInit.Adder 		= initPara->Inf_State.Adder;
	/* �ŵ�(����) */
	gInit.Channel 		= initPara->Inf_State.Channel;
	/* ���ݶ�ʧʱ�� */
	gInit.ActiveTime 	= initPara->Inf_State.ActiveTime;
	/* ����ģʽ������/���ԣ� */
	if(gInit.Mode == 0)
	{
		/* ����ģʽ */
		initPara->Inf_State.Mode = GW_Release;
	}else
	{
		/* ����ģʽ */
		initPara->Inf_State.Mode = GW_Debug;
	}
	/* ͨ�Ų����� */
	gInit.Baudrate 		= initPara->Inf_State.Baudrate;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPostFront (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* ������	: AppGW_Restart										 	 							*/
/* ����  	: ��																				*/
/* ���		: �� 																				*/
/* ����	 	: ��������						 													*/
/* ��������	: 2016/12/01																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_Restart(void)
{
	/* ������Ϣ����״̬��Ϣ */
	INT8U gQerr;
	/* ��̽ͷ���� ��Ϣ���� ���� */
	static GWRestartStruct gRestartFunc;
	/* ��������ṹ */
	static GWCMDStruct gCMD;
	
	
	/* �������� */
	gCMD.Type = GWCMD_Restart;
	gCMD.Para = &gRestartFunc;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPostFront (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	return GW_Success;
}

/************************************************************************************************/
/* ������	: AppGW_ConfigFunc										 	 						*/
/* ����  	: ��																				*/
/* ���		: �� 																				*/
/* ����	 	: �������ع���						 										*/
/* ��������	: 2016/11/28																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGW_ConfigFunc(void)
{
	/* ������Ϣ����״̬��Ϣ */
	INT8U gQerr;
	/* ��̽ͷ���� ��Ϣ���� ���� */
	static GWConfigFuncStruct gConfigFunc;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �������� */
	gCMD.Type = GWCMD_ConfigFunc;
	gCMD.Para = &gConfigFunc;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPostFront (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* ������	: AppGW_ReadProbe										 	 						*/
/* ����  	: no:̽ͷ���[1-32]��type:̽ͷ����[Temp/Hum]��data:̽ͷ����							*/
/* ���		: �� 																				*/
/* ����	 	: ���Ͼ�ʢ������̽ͷ����						 										*/
/* ��������	: 2016/11/18																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_ReadProbe(uint8_t no,GW_ProbeType type,float* data)
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
	static GWReadProbeStruct gReadProbePara;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �ж����ع���״̬ */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GW_Fail;
	}
	
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* �ڴ�����ɹ� */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
	}
#endif
	
	/* �������� */
	gCMD.Type = GWCMD_ReadData;
	gCMD.Para = &gReadProbePara;
	/* ����������� */
	gReadProbePara.MboxHandle 	= gMbox_Handle;
	gReadProbePara.Data 		= data;
	gReadProbePara.Type 		= type;
	gReadProbePara.No 			= no;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGW_ReadLevel										 	 						*/
/* ����  	: no:̽ͷ���[1-32]��level:̽ͷ����״̬												*/
/* ���		: �� 																				*/
/* ����	 	: ���Ͼ�ʢ�����ص���״̬						 										*/
/* ��������	: 2016/11/21																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_ReadLevel(uint8_t no,GW_Level* level)
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
	static GWReadLevelStruct gReadLevel;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �ж����ع���״̬ */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GW_Fail;
	}
	
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* �ڴ�����ɹ� */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
	}
#endif
	
	/* �������� */
	gCMD.Type = GWCMD_ReadLevel;
	gCMD.Para = &gReadLevel;
	/* ����������� */
	gReadLevel.MboxHandle 	= gMbox_Handle;
	gReadLevel.Level 		= level;
	gReadLevel.No 			= no;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGW_UpdataProbeData										 	 					*/
/* ����  	: ��																				*/
/* ���		: ���ز������ 																		*/
/* ����	 	: ��������̽ͷ����						 															*/
/* ��������	: 2016/11/21																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_UpdataProbeData(void)
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
	static GWUpdataProbeDataStruct gUpdataProbeData;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �ж����ع���״̬ */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GW_Fail;
	}
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* �ڴ�����ɹ� */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
	}
#endif
	
	/* �������� */
	gCMD.Type = GWCMD_UpdataProbeData;
	gCMD.Para = &gUpdataProbeData;
	/* ����������� */
	gUpdataProbeData.MboxHandle 	= gMbox_Handle;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GW_Result*)gMbox_p;

}

/************************************************************************************************/
/* ������	: AppGW_UpdataState										 	 						*/
/* ����  	: ��							 										*/
/* ���		: �� 																			*/
/* ����	 	: �����Ͼ�ʢ�����ص�ַ						 										*/
/* ��������	: 2016/11/18																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGW_UpdataState(void)
{
	/* ������Ϣ����״̬��Ϣ */
	INT8U gQerr;
	/* ��Ϣ������ */
	OS_EVENT* gMbox_Handle;
	/* ��̽ͷ���� ��Ϣ���� ���� */
	static GWUpdataStateStruct gUpdataState;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �ж����ع���״̬ */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return;
	}
	
	/* �������� */
	gCMD.Type = GWCMD_UpdataState;
	gCMD.Para = &gUpdataState;
	/* ����������� */
	gUpdataState.MboxHandle 	= gMbox_Handle;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* ������	: AppGW_SetAdder										 	 						*/
/* ����  	: adder:��ַ[00-255]							 										*/
/* ���		: ���ý�� 																			*/
/* ����	 	: �����Ͼ�ʢ�����ص�ַ						 										*/
/* ��������	: 2016/11/18																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetAdder(uint8_t adder)
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
	static GWSetAdderStruct gSetAdder;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �ж����ع���״̬ */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GW_Fail;
	}
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* �ڴ�����ɹ� */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
	}
#endif
	
	/* �������� */
	gCMD.Type = GWCMD_SetAdder;
	gCMD.Para = &gSetAdder;
	/* ����������� */
	gSetAdder.MboxHandle 	= gMbox_Handle;
	gSetAdder.Adder 		= adder;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGW_SetChannel										 	 						*/
/* ����  	: channel:�ŵ�							 											*/
/* ���		: ���ý�� 																			*/
/* ����	 	: �����Ͼ�ʢ�������ŵ������Σ�						 								*/
/* ��������	: 2016/11/18																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetChannel(uint8_t channel)
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
	static GWSetChannelStruct gSetChannel;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �ж����ع���״̬ */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GW_Fail;
	}
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* �ڴ�����ɹ� */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
	}
#endif
	
	/* �������� */
	gCMD.Type = GWCMD_SetChannel;
	gCMD.Para = &gSetChannel;
	/* ����������� */
	gSetChannel.MboxHandle 	= gMbox_Handle;
	gSetChannel.Channel 	= channel;

	/* ������Ҫ�������������� */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGW_SetActiveTime										 	 					*/
/* ����  	: activeTime:��Чʱ��							 									*/
/* ���		: ���ý�� 																			*/
/* ����	 	: �����Ͼ�ʢ������������Чʱ��						 								*/
/* ��������	: 2016/11/18																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetActiveTime(uint8_t activeTime)
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
	static GWSetActiveTimeStruct gSetActiveTime;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �ж����ع���״̬ */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GW_Fail;
	}
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* �ڴ�����ɹ� */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
	}
#endif
	
	/* �������� */
	gCMD.Type = GWCMD_SetActiveTime;
	gCMD.Para = &gSetActiveTime;
	/* ����������� */
	gSetActiveTime.MboxHandle 	= gMbox_Handle;
	gSetActiveTime.ActiveTime 	= activeTime;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGW_SetMode										 	 						*/
/* ����  	: mode:����ģʽ							 											*/
/* ���		: ���ý�� 																			*/
/* ����	 	: �����Ͼ�ʢ�����ع���ģʽ						 									*/
/* ��������	: 2016/11/18																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetMode(GW_Mode mode)
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
	static GWSetModeStruct gSetMode;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �ж����ع���״̬ */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GW_Fail;
	}
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* �ڴ�����ɹ� */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
	}
#endif
	
	/* �������� */
	gCMD.Type = GWCMD_SetMode;
	gCMD.Para = &gSetMode;
	/* ����������� */
	gSetMode.MboxHandle 	= gMbox_Handle;
	gSetMode.Mode 			= mode;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: AppGW_SetBaudrate										 	 							*/
/* ����  	: baudrate:������							 													*/
/* ���		: ���ý��																				*/
/* ����	 	: �����Ͼ�ʢ������ͨ�Ų�����						 									*/
/* ��������	: 2016/11/18																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetBaudrate(uint32_t baudrate)
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
	static GWSetBaudrateStruct gSetBaudrate;
	/* ��������ṹ */ 
	static GWCMDStruct gCMD;
	
	
	/* �ж����ع���״̬ */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* ��������ִ��ʧ�ܽ�� */
		return GW_Fail;
	}
	/* ����һ����Ϣ���� */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* �ڴ�����ɹ� */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϴ�����Ϣ����״̬ */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* ������Ϣ����ʧ�� */
		__nop();
	}
#endif
	
	/* �������� */
	gCMD.Type = GWCMD_SetBaudrate;
	gCMD.Para = &gSetBaudrate;
	/* ����������� */
	gSetBaudrate.MboxHandle 	= gMbox_Handle;
	gSetBaudrate.Baudrate 		= baudrate;
	
	/* ������Ҫ�������������� */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gQerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	/* �ȴ�������ɷ��ص���Ϣ�� */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ɾ����Ϣ���� */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ���ݷ��ص���Ϣ�����÷��������������� */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* ������	: GW_NJSY_Handle										 	 						*/
/* ����  	: ��							 													*/
/* ���		: ��																				*/
/* ����	 	: �Ͼ�ʢ�����ش��������						 									*/
/* ��������	: 2016/11/18																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void GW_NJSY_Handle(GWCMDStruct* cmd)
{	
	/* ����״̬�Ĵ��� */
	const static GW_Result gSuccess = GW_Success;
	const static GW_Result gFail 	= GW_Fail;
				 GW_Result gState;
	/* ��Ϣ����״̬��Ϣ */
	INT8U gMboxerr;
	/* ������ָ�� */
	OS_EVENT* gMboxp;
	
	
	/* �ж��������� */
	switch(cmd->Type)
	{
		/* ��ʼ������ */
 		case GWCMD_Init:
 		{
			/* ��ʼ����������ָ�� */
			GWInitStruct* gInitp = cmd->Para;
			/* ��ʼ�����ز��� */
			LogGatewayInitParaStruct logGWInitPara;
			
			
			/* �������س�ʼ������ */
			logGWInitPara.Adder 		= gInitp->Adder;
			logGWInitPara.Channel 		= gInitp->Channel;
			logGWInitPara.ActiveTime 	= gInitp->ActiveTime;
			logGWInitPara.Mode 			= gInitp->Mode;
			logGWInitPara.Baudrate 		= gInitp->Baudrate;
			
			/* ��ʼ������ */
			LogGW_API.Init(logGWInitPara);
			
			return;
 		}
		
		/* �������ع��� */
		case GWCMD_Restart:
		{
//			/* ��ʼ����������ָ�� */
//			GWRestartStruct* gRestartp = cmd->Para;
//			
//			
//			/* ������Ϣ������ */
//			gMboxp = gRestartp->MboxHandle;
			/* ���õ�ǰ��������״̬ */
			gState = LogGW_API.Restart();
			/* �ȴ�ϵͳ�����ȶ� */
			OSTimeDly(AppGW_RestartTimeValue+1000);
			return;
		}
		
		/* �������ع��� */
		case GWCMD_ConfigFunc:
		{
			/* ��ʼ����������ָ�� */
			GWConfigFuncStruct* gConfigFuncp = cmd->Para;
			
			
			/* ������Ϣ������ */
			gMboxp = gConfigFuncp->MboxHandle;
			/* ���õ�ǰ��������״̬ */
			gState = LogGW_API.ConfigFunc();
			/* ���ò���״̬ */
			AppGWRunPara->Inf_State.WorkState = gState;
			/* �ж��������ع���״̬ */
			if(AppGWRunPara->Inf_State.WorkState == GW_Success)
			{
				/* ���������ʱ������Ĵ��� */
				INT8U tmrErr;
				/* �ر������ʱ��״̬ */
				BOOLEAN gTmrStopState;
				
				
				/* ֹͣ��ʱ��ʼ�����ض�ʱ�� */
				gTmrStopState = OSTmrStop (AppGW_ReConfigFunc_ptmr,
										   OS_TMR_OPT_NONE,
										   (OS_TMR_CALLBACK)AppGW_ReConfigFunc_CallBack,
										   &tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
				if((OS_ERR_NONE != tmrErr)||(OS_FALSE == gTmrStopState))
				{
					/* ֹͣ��ʱ��ʼ�����ض�ʱ��:ʧ�� */
					__nop();
				}
#endif
				(void) gTmrStopState;
			}
			break;
		}
		
		/* ������ */
 		case GWCMD_ReadData:
 		{
			/* ��ʼ����������ָ�� */
			GWReadProbeStruct* gReadProbep = cmd->Para;

			
			/* ������Ϣ������ */
			gMboxp = gReadProbep->MboxHandle;
			/* ��̽ͷ���� */
			gState = LogGW_API.ReadProbe(gReadProbep->No,
											 gReadProbep->Type,
											 gReadProbep->Data);
			break;
 		}
		
		/* ������ */
 		case GWCMD_ReadLevel:
 		{
			/* ��ʼ����������ָ�� */
			GWReadLevelStruct* gReadLevelp = cmd->Para;

			
			/* ������Ϣ������ */
			gMboxp = gReadLevelp->MboxHandle;
			/* ��̽ͷ���� */
			gState = LogGW_API.ReadLevel(gReadLevelp->No,gReadLevelp->Level);
			break;
 		}
		
		/* ������������ */
 		case GWCMD_UpdataProbeData:
 		{
			/* ��ʼ����������ָ�� */
			GWUpdataProbeDataStruct* gUpdataProbeDatap = cmd->Para;

			
			/* ������Ϣ������ */
			gMboxp = gUpdataProbeDatap->MboxHandle;
			/* ������������ */
			gState = LogGW_API.UpdataProbeData();
			break;
 		}
		/* ��������״̬ */
		case GWCMD_UpdataState:
		{
			/* ��ʼ����������ָ�� */
			GWUpdataStateStruct* gUpdataStatep = cmd->Para;
			
			
			/* ������Ϣ������ */
			gMboxp = gUpdataStatep->MboxHandle;
			/* ������������ */
			gState = LogGW_API.UpdataState();
			/* �ж����ع���״̬ */
			if(GW_Fail == gState)
			{
				/* ���������ʱ������Ĵ��� */
				INT8U tmrErr;
				/* �������������ʱ��״̬ */
				BOOLEAN tmrState;
				
				
				/* ���ع����쳣�����ñ�־ */
				AppGWRunPara->Inf_State.WorkState = GW_Fail;
				
				/* ������ʼ������ */
				tmrState = OSTmrStart (AppGW_ReConfigFunc_ptmr,&tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
				if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
				{__nop();}
#endif
				(void)tmrErr;(void)tmrState;
			}
			
			return;
		}
		
		/* �������ص�ַ */
 		case GWCMD_SetAdder:
 		{
			/* ��ʼ����������ָ�� */
			GWSetAdderStruct* gSetAdderp = cmd->Para;

			
			/* ������Ϣ������ */
			gMboxp = gSetAdderp->MboxHandle;
			/* ������������ */
			gState = LogGW_API.SetAdder(gSetAdderp->Adder);
			break;
 		}
		
		/* ���������ŵ�(����) */
 		case GWCMD_SetChannel:
 		{
			/* ��ʼ����������ָ�� */
			GWSetChannelStruct* gSetChannelp = cmd->Para;

			
			/* ������Ϣ������ */
			gMboxp = gSetChannelp->MboxHandle;
			/* ������������ */
			gState = LogGW_API.SetChannel(gSetChannelp->Channel);
			break;
 		}
		
		/* ��������������Чʱ�� */
 		case GWCMD_SetActiveTime:
 		{
			/* ��ʼ����������ָ�� */
			GWSetActiveTimeStruct* gSetActiveTimep = cmd->Para;

			
			/* ������Ϣ������ */
			gMboxp = gSetActiveTimep->MboxHandle;
			/* ������������ */
			gState = LogGW_API.SetActiveTime(gSetActiveTimep->ActiveTime);
			break;
 		}
		
		/* �������ع���ģʽ */
 		case GWCMD_SetMode:
 		{
			/* ��ʼ����������ָ�� */
			GWSetModeStruct* gSetModep = cmd->Para;
			
			
			/* ������Ϣ������ */
			gMboxp = gSetModep->MboxHandle;
			/* ������������ */
			gState = LogGW_API.SetMode(gSetModep->Mode);
			break;
 		}
		
		/* �������ع���ģʽ */
 		case GWCMD_SetBaudrate:
 		{	
			/* ��ʼ����������ָ�� */
			GWSetBaudrateStruct* gSetBaudratep = cmd->Para;
			
			
			/* ������Ϣ������ */
			gMboxp = gSetBaudratep->MboxHandle;
			/* ������������ */
			gState = LogGW_API.SetBaudrate(gSetBaudratep->Baudrate);
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
	if(gState == GW_Success)
	{
		/* ����ִ�гɹ���������Ϣ���䡣 */
		gMboxerr = OSMboxPost(gMboxp,((void*)&gSuccess));
	}else
	{
		/* ����ִ��ʧ�� */
		gMboxerr = OSMboxPost(gMboxp,((void*)&gFail));
	}
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* �жϷ�������״̬ */
	if(gMboxerr != OS_ERR_NONE)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* ������	: AppGW_Init										 	 							*/
/* ����  	: ��							 													*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��Ӧ�ò����������Ϣ						 									*/
/* ��������	: 2016/11/18																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void AppGW_EnvironInit(void)
{
	/* ���������ʱ������Ĵ��� */
	INT8U tmrErr;
	/* �������������ʱ��״̬ */
	BOOLEAN tmrState;
	
	
	/* ��ʼ�����ع���״̬ */
	AppGWRunPara->Inf_State.WorkState = GW_Fail;
	
	/* �ȳ�ʼ�� ϵͳ�ȹصĲ��� */
	/* ���� �Ͼ�ʢ��������Ϣ���� */
	GW_NJSY_Q = OSQCreate(GW_NJSY_Grp,GW_NJSY_Q_Len);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	if(GW_NJSY_Q == (OS_EVENT *)0)
	{
		/* ��������ʧ�� */
		__nop();
	}
#endif
	
	/* ������ʱ��������̽ͷ���������ʱ�� */
	AppGW_UpdataProbeData_ptmr = 
	OSTmrCreate (
					300,							//�����ʱ����������ʱ����ʱ����ûص�����
					500,							//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_PERIODIC,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)AppGW_UpdataProbeData_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"������̽ͷ���ݶ�ʱ��",			//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);					

#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}
#endif				
	
	/* ������ʱ��ȡ���ع���״̬�����ʱ�� */
	AppGW_WorkState_ptmr = 
	OSTmrCreate (
					1000,							//�����ʱ����������ʱ����ʱ����ûص�����
					1000,							//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_PERIODIC,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)AppGW_UpdataState_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"�����ع���״̬��ʱ��",			//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);					
				
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}
#endif
	
	/* �����������ع��������ʱ�� */
	AppGW_ReConfigFunc_ptmr = 
	OSTmrCreate (
					10,								//�����ʱ����������ʱ����ʱ����ûص�����
					1000,							//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_PERIODIC,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)AppGW_ReConfigFunc_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"�������ع��ܶ�ʱ��",				//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);		
	
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}
#endif				
	
	/* ������ʱ��������̽ͷ���ݶ�ʱ������ʱ5S���� ���������ص������� */
	tmrState = OSTmrStart (AppGW_UpdataProbeData_ptmr,&tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* ������ʱ��ȡ���ع���״̬��ʱ������ʱ60S���� ���������ص������� */
	tmrState = OSTmrStart (AppGW_WorkState_ptmr,&tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* ������ʱ��ʼ�����ض�ʱ������ʱ60S���� ���������ص������� */
	tmrState = OSTmrStart (AppGW_ReConfigFunc_ptmr,&tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* ���ýӿں��� */
	/* ��ʼ���ӿ� */
	AppGW_API.Init 			= AppGW_Init;
	/* �������ؽӿ� */
	AppGW_API.Restart		= AppGW_Restart;
	/* �������ع��� */
	AppGW_API.ConfigFunc 	= AppGW_ConfigFunc;
	/* ��̽ͷ���ݽӿ� */
	AppGW_API.ReadProbe 	= AppGW_ReadProbe;
	/* ��̽ͷ����״̬ */
	AppGW_API.ReadLevel		= AppGW_ReadLevel;
	/* ��������̽ͷ���� */
	AppGW_API.UpdataProbeData	= AppGW_UpdataProbeData;
	/* ��������״̬ */
	AppGW_API.UpdataState	= AppGW_UpdataState;
	/* �������ؽӿ� */
	AppGW_API.SetAdder	 	= AppGW_SetAdder;
	/* ���������ŵ�(����) */
	AppGW_API.SetChannel 	= AppGW_SetChannel;
	/* ��������������Чʱ�� */
	AppGW_API.SetActiveTime = AppGW_SetActiveTime;
	/* ��������������Чʱ�� */
	AppGW_API.SetMode 		= AppGW_SetMode;
	/* ��������������Чʱ�� */
	AppGW_API.SetBaudrate 	= AppGW_SetBaudrate;
}

#endif
/***********************************************************************************/


/************************************************************************************************/
/* ������	: AppTaskGatewayNJSY																*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: �Ͼ�ʢ�����ش�������																*/
/* �������� 	: 2016/11/18																		*/
/************************************************************************************************/
void AppTaskGatewayNJSY(void *p_arg)
{
	/* �������״̬��Ϣ */
	INT8U gQerr;
	/* ��������ṹ */
	GWCMDStruct* gCMDDatap;

	(void)p_arg;
	
	
	/* ��ʼ������������� */
	AppGW_EnvironInit();
	
	/* ��ʼ���߼�������API���� */
	BSP_LogGW_API_Init();
	
	/* �������أ���ʼ������ */
	AppGW_API.Init(AppGWRunPara);

	while (1)
	{
		/* �ȴ� ���� ��Ϣ */
		gCMDDatap = (GWCMDStruct*)OSQPend(GW_NJSY_Q,0,&gQerr);
		
		/* ������Ϣ ״̬ �ж� */
		if(gQerr == OS_ERR_NONE)
		{
			/* �������ز������� */
			GW_NJSY_Handle(gCMDDatap);
		}
	}
}



#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
