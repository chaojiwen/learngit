/************************************************************************************************/
/** @file              : Log_Gateway_HZZH.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��12��14��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--���ݴ��������߼����ļ�							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��12��14��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_Gateway_HZZH.h"
#if (Gateway_HZZH_Enable == 0x01)
#include "Dev_Gateway_HZZH.h"
#include <ucos_ii.h>


/********************************* �궨�� *********************************/
/* ���ݴ�������ͨ�����ݰ�ͷ */
#define LogGW_ComPackHeader			(0X9BB9)

/* ���ݴ����������뷢�������ź����ȴ�ʱ�� */
#define LogGW_SemOvertimeValue		(20u)

/* ���ݴ������صȴ�������Ӧ�ĳ�ʱʱ��(50ms) */
#define LogGWHZZH_ReceCmdAckOvertimeValue	(50u)

/* ���ݴ������������������ص�ʱ�� */
#define LogGW_RestartTimeValue		(100u)

/* ���ݴ������ش���ͨ�Ų����� */
#define LogGW_Baudrate				(115200ul)

/* ���ݴ������ؽ�������֡״̬(������������֡) */
#define LogGW_ReceDataNull			(0XFF)

/**************************************************************************/
/************************** ���ݴ�����������ṹ ***************************/
/* ���ݴ������ع��������� */
typedef struct
{
	/* ��ͷ */
	uint8_t	PackHeader[2];
	/* ���� */
	uint8_t		Len;
	/* ID�� */
	uint8_t		ID[4];
	/* �������� */
	GWHZZHCmdType	CmdType;
}LogGWCmdStruct;

/* ���ݴ�����������������󳤶�(���ݡ�·���ϱ���ʽ_������1014����֪��󳤶�Ϊ34�ֽ�) */
#define LogGWCmd_DataLenMax			(4u)
/* ���ݴ��������������ݽṹ */
typedef struct
{
	uint8_t Buf[LogGWCmd_DataLenMax+10];
}LogGWCmdReceBufStruct;

/* ���ݴ�����������Ӧ���������ݽṹ */
typedef struct
{
	/* �̶������ݽṹ */
	LogGWCmdStruct LogGWCmdFix;
	/* ���� */
	uint8_t		Data[LogGWCmd_DataLenMax];
	/* У��CKS */
	uint16_t	Check;
}LogGWCmdDataACKStruct;

/* ���ݴ�������������Ӧʱ���������ݽṹ */
typedef struct
{
	/* �̶������ݽṹ */
	LogGWCmdStruct LogGWCmdFix;
	/* ��Ӧʱ�� */
	uint8_t	WkupTime;
	/* ̽ͷ���� */
	uint8_t	ProbeNum;
	/* У��CKS */
	uint16_t	Check;
}LogGWCmdSetWkupTimeStruct;

/* ���ݴ�������������Ӧʱ���Ӧ���������ݽṹ */
typedef struct
{
	/* �̶������ݽṹ */
	LogGWCmdStruct LogGWCmdFix;
	/* У��CKS */
	uint16_t	Check;
}LogGWCmdSetWkupTimeACKStruct;

/* ���ݴ������ض���Ӧʱ���������ݽṹ */
typedef struct
{
	/* �̶������ݽṹ */
	LogGWCmdStruct LogGWCmdFix;
	/* У��CKS */
	uint16_t	Check;
}LogGWCmdReadWkupTimeStruct;

/* ���ݴ������ض���Ӧʱ���Ӧ���������ݽṹ */
typedef struct
{
	/* �̶������ݽṹ */
	LogGWCmdStruct LogGWCmdFix;
	/* ��Ӧʱ�� */
	uint8_t	WkupTime;
	/* ̽ͷ���� */
	uint8_t	ProbeNum;
	/* У��CKS */
	uint16_t	Check;
}LogGWCmdReadWkupTimeACKStruct;

/************************** ���ݴ������ؽ�������״̬�ṹ ********************/
typedef struct
{
	
	/* ״̬�Ĵ��� */
	struct
	{
		/* ״̬�Ĵ��� */
		struct
		{
			/* ��ʼ�������ݱ�־ */
			uint8_t StartFlag;
			/* ��������Ӧʱ���̽ͷ����������Ӧ״̬ */
			GWHZZH_Result CmdAckRWTPNState;
			/* ����������Ӧʱ���̽ͷ����������Ӧ״̬ */
			GWHZZH_Result CmdAckSWTPNState;
		}SR;
		
		/* ���ݼĴ��� */
		struct
		{
			/* ��������/���ݼĴ��� */
			LogGWCmdReceBufStruct Reg[2];

		}DR;
		
		/* ��������ָ�� */
		uint8_t Index;	
		/* �������ݼĴ������ */
		uint8_t DRNo;
		/* ������ɼĴ������ */
		uint8_t FinishDRNo;
	}ReceReg;

}LogGWRunParaStruct;



/* ���ؽ���Ӧ��״̬ */
/********************************* ���ݴ������ؽ���Ӧ��״̬ ****************/
typedef enum {GWHZZH_ReceCmdAckFail = (0u), 
			  GWHZZH_ReceCmdAckSuccess = (1u)} GWHZZH_ReceCmdAckState;

/**************************************************************************/

/********************************* �����������ź��� **********************/
static OS_EVENT* LogGW_Sem;

/********************************* ���ݴ������ؽ��ն���Ӧʱ���̽ͷ����������Ӧ
								   ��Ϣ����ָ�� ****/
static OS_EVENT*  GWHZZH_RRWTPNCAMp = (OS_EVENT *)0;

/********************************* ���ݴ������ؽ���������Ӧʱ���̽ͷ����������
								   Ӧ��Ϣ����ָ�� ****/
static OS_EVENT*  GWHZZH_RSWTPNCAMp = (OS_EVENT *)0;			  
			  

/********************************* ���ݴ�������API ************************/
LogGatewayHZZHAPIStruct LogGWHZZHAPI;
/********************************* ���ݴ��������߼����������״̬�Ĵ��� ****/
static LogGWRunParaStruct LogGWRunPara = 
{	
	
	/* ��ʼ�������ݱ�־ */
	0,
	/* ��������Ӧʱ���̽ͷ����������Ӧ״̬ */
	GWHZZH_Fail,
	/* ����������Ӧʱ���̽ͷ����������Ӧ״̬ */
	GWHZZH_Fail,
	
	
	/* ���ݼĴ��� */
	0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,
	0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,
	
	
	/* ��������ָ�� */
	0,
	/* �������ݼĴ������ */
	0,
	/* ������ɼĴ������ */
	LogGW_ReceDataNull,
};

/*************************************************************************/

/********************************* ���ݴ���������Ӧ������ָ�� ************/
/* ̽ͷ������Ӧָ�� */
void (*LogGW_ProbeDataACKp)(uint8_t* id,uint8_t* data);
/* ����������Ӧʱ���̽ͷ����ָ�� */
void (*LogGW_SetWkupTimeProbeNumACKp)(void);
/* ��������Ӧʱ���̽ͷ����ָ�� */
void (*LogGW_ReadWkupTimeProbeNumACKp)(uint8_t wkupTime,uint8_t ProbeNum);

/*************************************************************************/

#if (1)//�������������ʱ���ص�����
/* �������������ʱ�� ���ָ�� */
static OS_TMR* LogGW_Restart_ptmr;
/************************************************************************************************/
/* ������	: LogGW_Restart_CallBack										 	 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: �������������ʱ���ص�����						 									*/
/* ��������	: 2016/12/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void LogGW_Restart_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* �������������ʱ����Ϣ */
	INT8U tmrErr;
	/* �������������ʱ��״̬ */
	BOOLEAN tmrState;
	
	
	(void) p_arg;

	/* �������ص�Դ */
	DevGWHZZHAPI.OpenPower();
	
	/* ����������ʱ�� */
	tmrState = OSTmrStop(ptmr,
                         OS_TMR_OPT_NONE,
                         (void*)0,
                         &tmrErr);
	/* �жϹر������ʱ��״̬ */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* �ر������ʱ��ʧ�� */
		__nop();
	}
}

#endif

#if (1)//���ݴ��������߼��㺯��
/************************************************************************************************/
/* ������	: LogGW_GetCheck										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: У�������																		*/
/* ����	 	: ���㺼�ݴ���ͨ������У��ֵ					 										*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static uint16_t LogGW_GetCheck(LogGWCmdStruct* dataFrame)
{	
	uint16_t nResult = 0;
	uint8_t index;
	/* ��У�� */
	uint8_t gCheckSum;
	/* ���У�� */
	uint8_t gCheckXor;
	
	
	gCheckSum = dataFrame->Len;
	
	gCheckXor = dataFrame->Len;
	
	for(index = 3;index < (dataFrame->Len+1);index ++)
	{
		gCheckSum += *((dataFrame->PackHeader)+index);
		
		gCheckXor ^= *((dataFrame->PackHeader)+index);
		
		gCheckXor ++ ;
	}
	
	/* �����ߵ�λ */
	nResult = gCheckSum+gCheckXor*256;
	
	return nResult;
}

/************************************************************************************************/
/* ������	: LogGW_ParseData										 	 						*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ��ʼ�����ݴ��������߼������в���					 								*/
/* ��������	: 2016/12/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void LogGW_ParseData(LogGWCmdStruct* dataFrame)
{	
	/* ����֡У�� */
	uint16_t gCheck;
	/* �������������ʱ����Ϣ */
	INT8U MboxErr;
	

	/* ����У�� */
	gCheck = LogGW_GetCheck(dataFrame);
	
	switch(dataFrame->CmdType)
	{
		/* ̽ͷ��Ӧ���� */
 		case GWHZZHCmd_ProbeDataACK:
		{
			/* �ж�У��ֵ�Ƿ���ȷ */
			if(gCheck != ((LogGWCmdDataACKStruct*)dataFrame)->Check)
			{
				/* У�鲻��ȷ */
				__nop();
				return;
			}else
			{
				/* ����̽ͷ������Ӧ */
				LogGW_ProbeDataACKp(((LogGWCmdDataACKStruct*)dataFrame)->LogGWCmdFix.ID,
									((LogGWCmdDataACKStruct*)dataFrame)->Data);
			}
			break;
		}
		
		/* ���û���ʱ����Ӧ���� */
 		case GWHZZHCmd_SetWkupTimeProbeNumACK:
		{
			/* �ж�У��ֵ�Ƿ���ȷ */
			if(gCheck != ((LogGWCmdSetWkupTimeACKStruct*)dataFrame)->Check)
			{
				/* У�鲻��ȷ */
				__nop();
				return;
			}else
			{
				/* ����������Ӧʱ���̽ͷ���� */
				LogGW_SetWkupTimeProbeNumACKp();
				
				/* ����������Ӧʱ���̽ͷ����������Ӧ״̬ */
				LogGWRunPara.ReceReg.SR.CmdAckSWTPNState = GWHZZH_Success;
				
				/* ������Ӧʱ���̽ͷ�����ɹ� */
				/* ������Ϣ */
				MboxErr = OSMboxPost (GWHZZH_RSWTPNCAMp, 
									 (void*)&LogGWRunPara.ReceReg.SR.CmdAckSWTPNState);
				
				/* �жϽ�������״̬��Ϣ����״̬ */
				if(MboxErr != OS_ERR_NONE)
				{__nop();/* ʧ�� */}
			}
			break;
		}
		
		/* ������ʱ����Ӧ���� */
 		case GWHZZHCmd_ReadWkupTimeProbeNumACK:
		{
			/* �ж�У��ֵ�Ƿ���ȷ */
			if(gCheck != ((LogGWCmdReadWkupTimeACKStruct*)dataFrame)->Check)
			{
				/* У�鲻��ȷ */
				__nop();
				return;
			}else
			{
				/* �������Ӧʱ���̽ͷ���� */
				LogGW_ReadWkupTimeProbeNumACKp(((LogGWCmdReadWkupTimeACKStruct*)dataFrame)->WkupTime,
											   ((LogGWCmdReadWkupTimeACKStruct*)dataFrame)->ProbeNum);
				
				/* ��������Ӧʱ���̽ͷ����������Ӧ״̬ */
				LogGWRunPara.ReceReg.SR.CmdAckRWTPNState = GWHZZH_Success;
				
				/* ������Ӧʱ���̽ͷ�����ɹ� */
				/* ������Ϣ */
				MboxErr = OSMboxPost (GWHZZH_RRWTPNCAMp, 
									 (void*)&LogGWRunPara.ReceReg.SR.CmdAckRWTPNState);
				
				/* �жϽ�������״̬��Ϣ����״̬ */
				if(MboxErr != OS_ERR_NONE)
				{__nop();/* ʧ�� */}
			}
			break;
		}
		
		default:
		{
			/* δ�趨���ͣ��������� */
			__nop();
			return;
		}
	}
	
}

/************************************************************************************************/
/* ������	: LogGW_ReceData										 	 						*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ��ʼ�����ݴ��������߼������в���					 								*/
/* ��������	: 2016/12/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void LogGW_ReceData(uint8_t data)
{
	/* �жϿ�ʼ�������ݱ�־ */
	if(LogGWRunPara.ReceReg.SR.StartFlag == (1u))
	{
		/* ��ʼ��������֡ */
		LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo].\
		Buf[LogGWRunPara.ReceReg.Index] = data;
		
		/* ���ý������ݼĴ���ָ�� */
		LogGWRunPara.ReceReg.Index++;
		
		/* �ж������Ƿ������� */
		if(((((LogGWCmdStruct*)(&LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo])))->Len +3) ==\
			LogGWRunPara.ReceReg.Index)
		{
			/* ���ݽ�����ɣ���������֡������ɱ�š� */
			LogGWRunPara.ReceReg.FinishDRNo = LogGWRunPara.ReceReg.DRNo;
			/* �޸Ľ������ݼĴ������ */
			if(LogGWRunPara.ReceReg.DRNo == (0u))
			{LogGWRunPara.ReceReg.DRNo = (1u);}
			else
			{LogGWRunPara.ReceReg.DRNo = (0u);}
			
			/* ����������ݼĴ���ָ�� */
			LogGWRunPara.ReceReg.Index = (0u);
			/* �����ʼ��������֡��־ */
			LogGWRunPara.ReceReg.SR.StartFlag = (0u);
		}else if((((LogGWCmdStruct*)(&LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo])+3))->Len ==\
				(LogGWCmd_DataLenMax+(10u)))
		{
			/* ���յĳ��ȳ�����ǰЭ��������ݳ��ȡ� */
			/* ����������ݼĴ���ָ�� */
			LogGWRunPara.ReceReg.Index = (0u);
			/* �����ʼ�������ݱ�־ */
			LogGWRunPara.ReceReg.SR.StartFlag = (0u);
		}
	}else
	{
		/* δ��ʼ���գ��жϵ�ǰ�����Ƿ�Ϊ��ʼ���ձ�־���� */
		if(((LogGW_ComPackHeader>>8) == data)&&((0u) == LogGWRunPara.ReceReg.Index))
		{
			/* ��ǰ����Ϊ��ʼ���ձ�־�ĵ�һ������ */
			/* ���浱ǰ���� */
			LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo].\
			Buf[LogGWRunPara.ReceReg.Index] = data;
			/* ���ý������ݼĴ���ָ�� */
			LogGWRunPara.ReceReg.Index++;
			
		}else if(((LogGW_ComPackHeader&0xFF) == data)&&((1u) == LogGWRunPara.ReceReg.Index))
		{
			/* ��ǰ����Ϊ��ʼ���ձ�־�ĵ�һ������ */
			/* ���浱ǰ���� */
			LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo].\
			Buf[LogGWRunPara.ReceReg.Index] = data;
			/* ���ý������ݼĴ���ָ�� */
			LogGWRunPara.ReceReg.Index++;
			/* ���ÿ�ʼ�������ݱ�־ */
			LogGWRunPara.ReceReg.SR.StartFlag = (1u);
		}else
		{
			/* ��ǰ���ݲ��ǿ�ʼ���ձ�־�ĵ�һ������ */
			/* ����������ݼĴ���ָ�� */
			LogGWRunPara.ReceReg.Index = (0u);
			/* �����ʼ�������ݱ�־ */
			LogGWRunPara.ReceReg.SR.StartFlag = (0u);
		}
	}
	
	/* �ж�����֡�������״̬ */
	if(LogGW_ReceDataNull != LogGWRunPara.ReceReg.FinishDRNo)
	{
		/* ����֡������ɣ�������ǰ���ݡ� */
		LogGW_ParseData((void*)&LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.FinishDRNo]);
		/* ���������ɼĴ������ */
		LogGWRunPara.ReceReg.FinishDRNo = LogGW_ReceDataNull;
	}
	
}

/************************************************************************************************/
/* ������	: LogGWCmd_Load										 	 							*/
/* ����  	: ��			 																	*/
/* ���		: У�������																		*/
/* ����	 	: ���㺼�ݴ���ͨ������У��ֵ					 										*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void LogGWCmd_Load(uint8_t* dest,LogGWCmdStruct* src)
{
	/* ������ */
	uint8_t num;
	/* ����ָ�� */
	uint8_t* gData;
	/* У��ָ�� */
	uint16_t* gCheck;
	
	/* �ж����ݳ��� */
	if(src->Len < 7)
	{
		/* ���ݳ���С��ͨ��������С���ȣ�˵������ȷ�� */
		return;
	}
		
	/* ��ȡ��ͷ */
	dest[0] = src->PackHeader[0];
	dest[1] = src->PackHeader[1];
	
	/* ��ȡ���ݳ��� */
	dest[2] = src->Len;
	
	/* ��ȡID */
	dest[3] = src->ID[0];
	dest[4] = src->ID[1];
	dest[5] = src->ID[2];
	dest[6] = src->ID[3];
	
	/* ��ȡ���� */
	dest[7] = src->CmdType;
	
	switch(src->CmdType)
	{
		/* ���û���ʱ������ */
 		case GWHZZHCmd_SetWkupTimeProbeNum:
		{
			/* ��������ָ�� */
			gData = &((LogGWCmdSetWkupTimeStruct*)src)->WkupTime;
			/* ����У��ָ�� */
			gCheck = &((LogGWCmdSetWkupTimeStruct*)src)->Check;
			break;
		}
		
		/* ������ʱ������ */
 		case GWHZZHCmd_ReadWkupTimeProbeNum:
		{
			/* ��������ָ�� */
			gData = (uint8_t*)src;
			/* ����У��ָ�� */
			gCheck = &((LogGWCmdReadWkupTimeStruct*)src)->Check;
			break;
		}

		default:
		{
			/* δ�趨���ͣ��������� */
			__nop();
			return;
		}
	}
	
	for(num = 0;num < (src->Len-7);num++)
	{
		/* ��ȡ���� */
		dest[num+8] = gData[num];
	}
	
	/* ��ȡУ��ֵ */
	dest[num+8] = *gCheck;
	/* ��ȡУ��ֵ */
	dest[num+9] = ((*gCheck)>>8);
	
}

/************************************************************************************************/
/* ������	: LogGWCmd_InitPara										 	 						*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ��ʼ�����ݴ��������߼������в���					 								*/
/* ��������	: 2016/12/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void LogGWCmd_InitPara(void)
{
	/* ���������ʱ������Ĵ��� */
	INT8U tmrErr;
	
	
	/* �����������������ʱ�� */
	LogGW_Restart_ptmr = 
	OSTmrCreate (
					LogGW_RestartTimeValue,			//�����ʱ����������ʱ����ʱ����ûص�����
					LogGW_RestartTimeValue,			//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_ONE_SHOT,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)LogGW_Restart_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"����������ʱ��",				//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);					
							
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}
	
					
	/* ���� ���ն�������Ӧʱ���̽ͷ���� ��Ϣ���� */
	GWHZZH_RRWTPNCAMp = OSMboxCreate ((void*)0);
	/* �жϽ�������״̬��Ϣ����״̬ */
	if(GWHZZH_RRWTPNCAMp == ((OS_EVENT *)0))
	{__nop();/* ʧ�� */}
	
	/* ���� ���ն�������Ӧʱ���̽ͷ���� ��Ϣ���� */
	GWHZZH_RSWTPNCAMp = OSMboxCreate ((void*)0);
	/* �жϽ�������״̬��Ϣ����״̬ */
	if(GWHZZH_RSWTPNCAMp == ((OS_EVENT *)0))
	{__nop();/* ʧ�� */}
	
}

#endif

#if (1)//���ݴ������ع��ܽӿں���
/************************************************************************************************/
/* ������	: LogGW_Restart										 	 							*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ��������					 														*/
/* ��������	: 2016/12/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void LogGW_Restart(void)
{
	/* �������������ʱ����Ϣ */
	INT8U tmrErr;
	/* �������������ʱ��״̬ */
	BOOLEAN tmrState;
	
	
	/* �ر����ص�Դ */
	DevGWHZZHAPI.ClosePower();
	
	/* ����������ʱ������ʱ1S���� ���������ص������� */
	tmrState = OSTmrStart (LogGW_Restart_ptmr,&tmrErr);
	if((tmrErr != OS_ERR_NONE)||(tmrState != OS_FALSE))
	{__nop();}
}

/************************************************************************************************/
/* ������	: LogGW_ReadWkupTime_ProbeNum										 	 			*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ������ʱ��ͱ�ǩ����					 											*/
/* ��������	: 2016/12/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result LogGW_ReadWkupTimeProbeNum(void)
{
	/* ��Ϣ����  ��Ϣָ�� */
	void* mbox_Mgs;
	/* �ź���������Ϣ */
	INT8U mSemErr;
	/* ���������Ϣ */
	INT8U mBoxErr;
	/* ������ʱ��ͱ�ǩ�����Ĵ��� */
	LogGWCmdReadWkupTimeStruct LogGWCmdReadWkupTime = {0};
	/* �������ݼĴ��� */
	static uint8_t gSendReg[10];
	
	
	/* ���������ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_SemOvertimeValue*10),
			  &mSemErr);
	
	/* �ж��ź���״̬ */
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GWHZZH_Fail;
	}

	/* ����������� */
	LogGWCmdReadWkupTime.LogGWCmdFix.PackHeader[0]	= (LogGW_ComPackHeader>>8);
	LogGWCmdReadWkupTime.LogGWCmdFix.PackHeader[1]	= (LogGW_ComPackHeader&0xFF);
	LogGWCmdReadWkupTime.LogGWCmdFix.Len 		= (7u);
	LogGWCmdReadWkupTime.LogGWCmdFix.ID[0] 	 	= (0x0);
	LogGWCmdReadWkupTime.LogGWCmdFix.ID[1] 	 	= (0x0);
	LogGWCmdReadWkupTime.LogGWCmdFix.ID[2]	 	= (0x0);
	LogGWCmdReadWkupTime.LogGWCmdFix.ID[3]	 	= (0x0);
	LogGWCmdReadWkupTime.LogGWCmdFix.CmdType	= (GWHZZHCmd_ReadWkupTimeProbeNum);
	
	/* ����У�� */
	LogGWCmdReadWkupTime.Check = LogGW_GetCheck(&LogGWCmdReadWkupTime.LogGWCmdFix);
	
	/* �����úõĲ���ת�Ƶ����ͼĴ����� */
	LogGWCmd_Load(gSendReg,&LogGWCmdReadWkupTime.LogGWCmdFix);
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GWHZZH_RRWTPNCAMp);
	
	/* ���Ͷ���ַ���� */
	DevGWHZZHAPI.SendData(gSendReg,sizeof(gSendReg));
	
	/* �ȴ�����������Ϣ */
	mbox_Mgs = OSMboxPend ((GWHZZH_RRWTPNCAMp),
						   (LogGWHZZH_ReceCmdAckOvertimeValue),
						   (&mBoxErr));
	
	/* ���������ź��� */
	mSemErr = OSSemPost (LogGW_Sem);
	if(mSemErr != OS_ERR_NONE)
	{__nop();}
	
	/* ��ȡ��Ϣ */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GWHZZH_ReceCmdAckState*)mbox_Mgs) == GWHZZH_ReceCmdAckSuccess))
	{
		/* ����Ӧ��ɹ� */
		return GWHZZH_Success;
	}
	
	/* ���ز������ */
	return GWHZZH_Fail;
}

/************************************************************************************************/
/* ������	: LogGW_State										 	 							*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: �����ع���״̬					 													*/
/* ��������	: 2016/12/19																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result LogGW_State(void)
{
	/* ���Ͷ���Ӧʱ���̽ͷ���� */
	return LogGW_ReadWkupTimeProbeNum();
}

/************************************************************************************************/
/* ������	: LogGW_SetWkupTime										 	 						*/
/* ����  	: time:����ʱ������,probeNum:̽ͷ����			 										*/
/* ���		: �������																			*/
/* ����	 	: ���ú��ݴ������ػ���ʱ�书��					 									*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result LogGW_SetWkupTime(uint8_t time,uint8_t probeNum)
{
	/* ��Ϣ����  ��Ϣָ�� */
	void* mbox_Mgs;
	/* �ź���������Ϣ */
	INT8U mSemErr;
	/* ���������Ϣ */
	INT8U mBoxErr;
	/* ������ʱ��ͱ�ǩ�����Ĵ��� */
	LogGWCmdSetWkupTimeStruct LogGWCmdSetWkupTime = {0};
	/* �������ݼĴ��� */
	static uint8_t gSendReg[12];
	
	
	/* ��ѯ�����ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_SemOvertimeValue*10),
			  &mSemErr);
	
	/* �ж��ź���״̬ */
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GWHZZH_Fail;
	}
	
	/* ����������� */
	LogGWCmdSetWkupTime.LogGWCmdFix.PackHeader[0]	= (LogGW_ComPackHeader>>8);
	LogGWCmdSetWkupTime.LogGWCmdFix.PackHeader[1]	= (LogGW_ComPackHeader&0xFF);
	LogGWCmdSetWkupTime.LogGWCmdFix.Len 		= (9u);
	LogGWCmdSetWkupTime.LogGWCmdFix.ID[0] 	 	= (0x0);
	LogGWCmdSetWkupTime.LogGWCmdFix.ID[1] 	 	= (0x0);
	LogGWCmdSetWkupTime.LogGWCmdFix.ID[2]	 	= (0x0);
	LogGWCmdSetWkupTime.LogGWCmdFix.ID[3]	 	= (0x0);
	LogGWCmdSetWkupTime.LogGWCmdFix.CmdType		= (GWHZZHCmd_SetWkupTimeProbeNum);
	LogGWCmdSetWkupTime.WkupTime    			= (time);
	LogGWCmdSetWkupTime.ProbeNum 				= (probeNum);
	
	/* ����У�� */
	LogGWCmdSetWkupTime.Check 					= LogGW_GetCheck(&LogGWCmdSetWkupTime.LogGWCmdFix);
	
	/* �����úõĲ���ת�Ƶ����ͼĴ����� */
	LogGWCmd_Load(gSendReg,&LogGWCmdSetWkupTime.LogGWCmdFix);
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GWHZZH_RSWTPNCAMp);
	
	/* ���Ͷ���ַ���� */
	DevGWHZZHAPI.SendData(gSendReg,sizeof(gSendReg));
	
	/* �ȴ�����������Ϣ */
	mbox_Mgs = OSMboxPend ((GWHZZH_RSWTPNCAMp),
						   (LogGWHZZH_ReceCmdAckOvertimeValue),
						   (&mBoxErr));
	
	/* ���������ź��� */
	mSemErr = OSSemPost (LogGW_Sem);
	if(mSemErr != OS_ERR_NONE)
	{__nop();}
	
	/* ��ȡ��Ϣ */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GWHZZH_ReceCmdAckState*)mbox_Mgs) == GWHZZH_ReceCmdAckSuccess))
	{
		/* ����Ӧ��ɹ� */
		return GWHZZH_Success;
	}
	
	/* ���ز������ */
	return GWHZZH_Fail;
}

/************************************************************************************************/
/* ������	: LogGW_SetProbeNum										 	 						*/
/* ����  	: time:����ʱ������,probeNum:̽ͷ����			 										*/
/* ���		: �������																			*/
/* ����	 	: ���ú��ݴ������ر�ǩ��������					 									*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result LogGW_SetProbeNum(uint8_t time,uint8_t probeNum)
{	
	
	/* ���ز������ */
	return LogGW_SetWkupTime(time,probeNum);
}

/************************************************************************************************/
/* ������	: LogGW_Init										 	 							*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ�����ع���					 													*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void LogGW_Init(void)
{
	
	/* ��ʼ��������ӿ� */
	DevGW_InitAPI();
	
	/* ��ʼ������ */
	DevGWHZZHAPI.Init(LogGW_Baudrate,LogGW_ReceData);
	
	/* ���������ź��� */
	LogGW_Sem = OSSemCreate (1u);
	
	/* �ȴ�������������Դ�ȶ��� */
	OSTimeDly(10);
	
}

#endif

/************************************************************************************************/
/* ������	: LogGW_InitAPI										 	 							*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ���߼��㹦�ܽӿ�				 												*/
/* ��������	: 2016/12/14																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void LogGW_InitAPI(void (*probeDataACKp)(uint8_t* id,uint8_t* data),
				   void (*setWkupTimeProbeNumACKp)(void),
				   void (*readWkupTimeProbeNumACKp)(uint8_t wkupTime,uint8_t ProbeNum))
{	
	
	
	/* ̽ͷ������Ӧָ�� */
	LogGW_ProbeDataACKp = probeDataACKp;
	/* ����������Ӧʱ���̽ͷ����ָ�� */
	LogGW_SetWkupTimeProbeNumACKp = setWkupTimeProbeNumACKp;
	/* ��������Ӧʱ���̽ͷ����ָ�� */
	LogGW_ReadWkupTimeProbeNumACKp = readWkupTimeProbeNumACKp;
	
	/* ��ʼ�����ݴ��������߼������в��� */
	LogGWCmd_InitPara();
	
	/* ����ϵͳ�ӿں��� */
	/* ��ʼ���ӿ� */
	LogGWHZZHAPI.Init 			= LogGW_Init;
	
	/* �����������ؽӿ� */
	LogGWHZZHAPI.Restart		= LogGW_Restart;
	
	/* �����������ؽӿ� */
	LogGWHZZHAPI.State			= LogGW_State;
	
	/* �������ػ���ʱ�� */
	LogGWHZZHAPI.SetWkupTime	= LogGW_SetWkupTime;
	
	/* �������ر�ǩ���� */
	LogGWHZZHAPI.SetProbeNum	= LogGW_SetProbeNum;
	
	/* �����ػ���ʱ��ͱ�ǩ���� */
	LogGWHZZHAPI.ReadWkupTimeProbeNum	= LogGW_ReadWkupTimeProbeNum;
	
}




#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
