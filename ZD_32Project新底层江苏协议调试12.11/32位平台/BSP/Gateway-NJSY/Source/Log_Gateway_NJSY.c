/************************************************************************************************/
/** @file              : Log_Gateway_NJSY.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��15��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--�����߼����ļ�									*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��11��15��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_Gateway_NJSY.h"
#if (Gateway_NJSY_Enable == 0x01)
#include "Dev_Gateway_NJSY.h"
#include "Log_CRC16.h"


#include <ucos_ii.h>
#include <string.h>



/********************************* �궨�� *********************************/
/* ���ؽ������� �洢�Ĵ�����С */
#define GW_ReceDataBufSize		(264u)
/* ����Э��ʹ�õ��Ǵ��ģʽ��STM32ʹ�õ���С��ģʽ�����������Ҫת��Ϊ��ˡ� */
#define BigToLittle_Endian_HalfWord(data)	(((((uint16_t)data) & 0xff00) >> 8) |\
											 ((((uint16_t)data) & 0x00ff) << 8))
#define BigToLittle_Endian_Word(data)		(((((uint32_t)data) & 0xff000000) >> 24) |\
											 ((((uint32_t)data) & 0x00ff0000) >> 8)  |\
											 ((((uint32_t)data) & 0x0000ff00) << 8)  |\
											 ((((uint32_t)data) & 0x000000ff) << 24))
/* ���ؽ������ݳ�ʱ���� */
#define GW_ReceTimeoutPara		(-9990.0f)

/********************************* �������в������� *********************************/
/* ���ؽ���Ӧ��״̬ */
typedef enum {GW_ReceFail = (0u), GW_ReceSuccess = (1u)} GW_ReceState;
typedef enum {GW_Idle = (0u), GW_Busy = (1u)} GW_Run;

/* ���ع��������� */
typedef enum {
				/* ������ */
				GW_ReadData = (3u), 
				/* ���ò��� */
				GW_Config 	 = (6u),
				/* �����ص�ַ */
				GW_ReadAdder = (83u)
			  }GWCMDType;
/* �������ò�����ַ */
typedef enum {
				/* ���õ�ַ */
				GC_SetAdder 	= (0u), 
				/* �����ŵ� */
				GC_SetChannel 	= (3u),
				/* �������ݶ�ʧʱ�� */
				GC_SetActiveTime = (4u),
				/* ����ͨ�Ų����� */
				GC_SetBaudrate 	= (6u),
				/* ���ù���ģʽ */
				GC_SetMode 		= (7u),
/* ���ض����ݵ�ַ��Ĭ���Ǵ�0x09��ַ��ʼ��һ��̽ͷ��2����[1���¶ȣ�2��ʪ��]����
   ÿ��������4���ֽ�[float]���ܹ�32��̽ͷ����˶�Ӧͨ����ַ�������ϵ�����
   ����:̽ͷ2�ĵ�ַΪ0x19[0x09+8*2]��	 */
				/* ������ */
				GC_ReadData		= (9u),
				/* �����ص�ַ���� */
				GC_ReadAdder 	= (83u)
			 }GWConfigAdderType; 
/********************************* �������в����ṹ *********************************/
typedef struct
{
	/* ���üĴ��� */
	struct CR
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
	}CR;
	/* ״̬�Ĵ��� */
	struct SR
	{
		/* ���ع���״̬ */
		GW_Result Work;
		/* ��̽ͷ����״̬ */
		GW_Result ReadData;
		/* �����ص�ַ״̬ */
		GW_Result ReadAdder;
		/* �������ص�ַ״̬ */
		GW_Result SetAdder;
		/* ���ò����� */
		GW_Result SetBaudrate;
		/* �����ŵ�״̬ */
		GW_Result SetChannel;
		/* �������ݶ�ʧʱ��״̬ */
		GW_Result SetInvalidTime;
		/* �������ع���ģʽ״̬ */
		GW_Result SetMode;
		/* ��������״̬ */
		GW_Run	 Run;
	}SR;
	/* ���ݼĴ��� */
	struct DR
	{
		/* ̽ͷͨ������ */
		float Data[32][2];
		/* ̽ͷ��ص���״̬ */
		GW_Level Level[32];
		/* ��ȡ�����ص�ַ */
		uint8_t Adder;
	}DR;
	
}GWRegStruct;

/********************************* ��������ṹ *********************************/
typedef struct
{
	/* ���ص�ַ */
	uint8_t	GW_Adder;
	/* ������ */
	GWCMDType	Func;
	/* Ԥ����1(��֪�����ʲô��) */
	uint8_t	Reserve1;
	/* ���ʵ�ַ */
	GWConfigAdderType	Visit_Adder;
	/* Ԥ����2(��֪�����ʲô��) */
	uint8_t	Reserve2;
	/* ���� */
	uint8_t	Data;
	/* У��(Modbus RTU) */
	uint16_t Check;
}GWCMDStruct;

/********************************* ��������Ӧ�����ݽṹ *********************************/
typedef struct
{
	/* ���ص�ַ */
	uint8_t	GW_Adder;
	/* ������ */
	GWCMDType	Func;
	/* Ԥ����1(��֪�����ʲô��) */
	uint8_t	Reserve1;
	/* ���ʵ�ַ */
	GWConfigAdderType	Visit_Adder;
	/* Ԥ����2(��֪�����ʲô��) */
	uint8_t	Reserve2;
	/* ���� */
	uint8_t* Data;
	/* У��(Modbus RTU) */
	uint16_t* Check;
}GWConfigACKStruct;

/********************************* ���ض�����Ӧ�����ݽṹ *********************************/
typedef struct
{
	/* ���ص�ַ */
	uint8_t	GW_Adder;
	/* ������ */
	GWCMDType	Func;
	/* ���ݳ��� */
	uint8_t	Len;
	/* ���� */
	uint8_t* Data;
	/* У��(Modbus RTU) */
	uint16_t* Check;
}GWReadDateACKStruct;



/********************************* �������в��� *********************************/
static GWRegStruct GWReg;
/********************************* ����������� *********************************/
static GWCMDStruct GWCMD;

/********************************* ���ؽ�����Ϣ���� *********************************/
/* ����״̬  ��Ϣ����ָ�� */
static OS_EVENT*  GW_ReceStateMboxp = (OS_EVENT *)0;
/* �������� �洢�Ĵ��� */
static uint8_t GW_ReceDataBuf[GW_ReceDataBufSize];
/* �������� �洢�Ĵ���λ��ָ�� */
static uint16_t GW_ReceDataIndex = 0;
/********************************* ���ؽ��ź������� *********************************/
static OS_EVENT* LogGW_Sem;
/********************************* ����API *********************************/
LogGatewayAPIStruct LogGW_API;

/********************************* ���ؽ������ݺ������� *********************************/
static void GWLog_ReceData_Enable(void);
static void GWLog_ReceData_Disable(void);

static GW_Result GWLog_ReadProbe(uint8_t probeNo,GW_ProbeType probetype,float* probe);
static GW_Result GWLog_ReadLevel(uint8_t probeNo,GW_Level* level);
static GW_Result GWLog_ReadAdder(uint8_t* adder);
static GW_Result GWLog_SetAdder(uint8_t adder);
static GW_Result GWLog_SetChannel(uint8_t channel);
static GW_Result GWLog_SetActiveTime(uint8_t time);
static GW_Result GWLog_SetMode(GW_Mode mode);
static GW_Result GWLog_SetBaudrate(uint32_t baudrate);
/***********************************************************************************/
#if (1)//���ݽ��ճ�ʱ�����ʱ���ص�����
/* ���ݽ��ճ�ʱ�����ʱ�� ���ָ�� */
static OS_TMR* GWLog_Overtime_ptmr;

/************************************************************************************************/
/* ������	: GWLog_Overtime_CallBack										 	 					*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: ���ݽ��ճ�ʱ�����ʱ���ص�����						 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void GWLog_Overtime_CallBack(OS_TMR *ptmr,void *p_arg)
{	
	/* �������������ʱ����Ϣ */
	INT8U MboxErr;
	(void) p_arg;
	
	
	/* �رս��չ��� */
	GWLog_ReceData_Disable();
	
	/* ����ʧ����Ϣ */
	MboxErr = OSMboxPost (GW_ReceStateMboxp, (void*)0);
	/* �жϷ�����Ϣ״̬ */
	if(MboxErr == OS_ERR_NONE)
	{__nop();}
	
}

#endif

#if (1)//����״̬�����ʱ���ص�����
/* ���ݽ��ճ�ʱ�����ʱ�� ���ָ�� */
static OS_TMR* GWLog_ReceData_ptmr;

/************************************************************************************************/
/* ������	: GWLog_ReceData_CallBack										 	 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: ����״̬�����ʱ���ص�����						 									*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void GWLog_ReceData_CallBack(OS_TMR *ptmr,void *p_arg)
{
	(void) p_arg;

	/* �жϵ�ǰ�����Ӧ������ */
	
	/* �жϵ�ǰ���� ������ */
	if(GWCMD.Func == GW_ReadData)
	{
		/* �ж������Ƿ�������� */
		if((GWCMD.Data > (0u))&&
		   (GWCMD.Data <=(128u))&&
		   (GW_ReceDataIndex == (5u+((uint16_t)GWCMD.Data)*2)))
		{
			/* ���ݽ��ճɹ� */
			/* ��ϢӦ��ָ�� */
			GWReadDateACKStruct* gACKp = 
			(GWReadDateACKStruct*)GW_ReceDataBuf;
			GWReadDateACKStruct gACK;
			/* �������������ʱ����Ϣ */
			INT8U MboxErr;
			
			
			/*  */
			gACK = *gACKp;
			/* �������ݵ�ַ */
			gACK.Data = &GW_ReceDataBuf[3];
			/* �ж�������Ϣ */
			if((gACK.GW_Adder 	== GWCMD.GW_Adder)&&
			   (gACK.Func 		== GWCMD.Func)&&
			   (gACK.Len		==((((uint16_t)GWCMD.Data)*2)%0X100)))
			{
				/* У������ */
				uint16_t check;
					
				
				/* �������ݵ�ַ */
				gACK.Check = (uint16_t*)&GW_ReceDataBuf[3u+((uint16_t)GWCMD.Data)*2];
				/* ����У�� */
				check = BigToLittle_Endian_HalfWord(CRC16_GetValue(GW_ReceDataBuf,(3u+((uint16_t)GWCMD.Data)*2)));
				/* �ж�У��ֵ�Ƿ����� */
				if(*gACK.Check == check)
				{
					/* ������ */
					uint8_t num;
					/* ����ָ�� */
					uint32_t* gProbeDatap;
					
					
					/* У����ȷ�����ö����ص�ַ���� */
					for(num = 0;num < (GWCMD.Data/2);num++)
					{
						/* ��������ָ�� */
						gProbeDatap = (((uint32_t*)gACK.Data)+num);
						
						/* ���ݴ�С��ת�� */
						*gProbeDatap = BigToLittle_Endian_Word(*gProbeDatap);
						
						/* ��������ֵ���ж�̽ͷ������ */
						/* ��ĳ�����������ֵ�������ʱ���ô��������������ݵĵ�һ��
						   ������+9000���г�һ������ֵ�� */
						if((*((float*)gProbeDatap)) >= (9000.0f))
						{
							/* ��ǰ̽ͷ��ص������㡣 */
							GWReg.DR.Level[(num/2)] = GW_Level_FALSE;
							/* �������� */
							GWReg.DR.Data[num/2][num%2] = ((*((float*)gProbeDatap))-(9000.0f));
						}else
						{
							/* ��ǰ̽ͷ��ص��������� */
							GWReg.DR.Level[(num/2)] = GW_Level_TRUE;
							/* �������� */
							GWReg.DR.Data[num/2][num%2] = (*((float*)gProbeDatap));
						}
						
						/* �жϵ�ǰ���صĹ���ģʽ */
						if(GWReg.CR.Mode == GW_Debug)
						{
							/* ���ش��ڵ���ģʽ��̽ͷʪ�����ݽ���0ֵ��
							   ����������ʾ��������Ƚϲ��ˣ�����
							   ���ź�ǿ�����ݿ���һ�ݵ�ʪ�ȼĴ����С� */
							if((num%2) == (1u))
							{
								/* �����¶ȼĴ����е��ź�ǿ�����ݵ�ʪ�ȼĴ����� */
								GWReg.DR.Data[num/2][num%2] = GWReg.DR.Data[num/2][0];
							}
						}
					}
					
					/* ���ö����ص�ַʧ��״̬ */
					GWReg.SR.ReadData = GW_Success;
				}else
				{
					/* У����� */
					/* ���ö����ص�ַʧ��״̬ */
					GWReg.SR.ReadData = GW_Fail;
				}
			}else
			{
				/* �رս��չ��̣����ͽ���ʧ����Ϣ */
				/* ���ö����ص�ַʧ��״̬ */
				GWReg.SR.ReadData = GW_Fail;
			}
			/* �رս��չ��� */
			GWLog_ReceData_Disable();
			
			/* ������Ϣ */
			MboxErr = OSMboxPost (GW_ReceStateMboxp, (void*)&GWReg.SR.ReadData);
			/* �жϷ�����Ϣ״̬ */
			if(MboxErr == OS_ERR_NONE)
			{__nop();}
		}else
		{
			/* ����û�н����������������� */
			__nop();
		}
	}
	else if(GWCMD.Func == GW_Config)
	{
		/* �ж������Ƿ�������� */
		if(GW_ReceDataIndex == (8u))
		{
			/* ���ݽ��ճɹ� */
			/* ��ϢӦ��ָ�� */
			GWConfigACKStruct* gACKp = (GWConfigACKStruct*)GW_ReceDataBuf;
			/* �������������ʱ����Ϣ */
			INT8U MboxErr;
			/* ����״ָ̬�� */
			GW_Result* gStatep;
			
			
			/* ���� ����״ָ̬�� */
			switch(GWCMD.Visit_Adder)
			{
				/* ���õ�ַ */
				case GC_SetAdder:
				{
					gStatep = &GWReg.SR.SetAdder;
					break;
				}

				/* �����ŵ� */
				case GC_SetChannel:
				{
					gStatep = &GWReg.SR.SetChannel;
					break;
				}
				
				/* �������ݶ�ʧʱ�� */
				case GC_SetActiveTime:
				{
					gStatep = &GWReg.SR.SetInvalidTime;
					break;
				}
				
				/* ����ͨ�Ų����� */
				case GC_SetBaudrate:
				{
					gStatep = &GWReg.SR.SetBaudrate;
					break;
				}
				
				/* ���ù���ģʽ */
				case GC_SetMode:
				{
					gStatep = &GWReg.SR.SetMode;
					break;
				}
				
				default:
				{
					/* δ�趨���� �������� */
					/* �����ؽ�������״̬���ȴ���ʱ */
					__nop();
					return;
				}
			}
			/* �������ݵ�ַ */
			gACKp->Data = &GW_ReceDataBuf[5];
			/* �ж�������Ϣ */
			if((gACKp->GW_Adder 	== (GWCMD.GW_Adder))&&
			   (gACKp->Func 			== (GWCMD.Func))&&
			   (gACKp->Reserve1			== (GWCMD.Reserve1))&&
			   (gACKp->Visit_Adder		== (GWCMD.Visit_Adder))&&
			   (gACKp->Reserve2			== (GWCMD.Reserve2)))
			{
				/* У������ */
				uint16_t check;
			
				
				/* �������ݵ�ַ */
				gACKp->Check = (uint16_t*)&GW_ReceDataBuf[6];
				/* ����У�� */
				check = BigToLittle_Endian_HalfWord(CRC16_GetValue(GW_ReceDataBuf,(6u)));
				/* �ж�У��ֵ�Ƿ����� */
				if(*gACKp->Check == check)
				{
					/* ���ö����ص�ַʧ��״̬ */
					*gStatep = GW_Success;
				}else
				{
					/* У����� */
					/* ���ö����ص�ַʧ��״̬ */
					*gStatep = GW_Fail;
				}
			}else
			{
				/* �رս��չ��̣����ͽ���ʧ����Ϣ */
				/* ���ö����ص�ַʧ��״̬ */
				*gStatep = GW_Fail;
			}
			/* �رս��չ��� */
			GWLog_ReceData_Disable();
			
			/* ����ʧ����Ϣ */
			MboxErr = OSMboxPost (GW_ReceStateMboxp, (void*)gStatep);
			/* �жϷ�����Ϣ״̬ */
			if(MboxErr == OS_ERR_NONE)
			{__nop();}
		}else
		{
			/* ����û�н����������������� */
			__nop();
		}
	}
	else if(GWCMD.Func == GW_ReadAdder)
	{
		/* �ж������Ƿ�������� */
		if(GW_ReceDataIndex == (8u))
		{
			/* ���ݽ��ճɹ� */
			/* ��ϢӦ��ָ�� */
			GWConfigACKStruct* gACKp = (GWConfigACKStruct*)GW_ReceDataBuf;
			/* �������������ʱ����Ϣ */
			INT8U MboxErr;
			
			
			/* �������ݵ�ַ */
			gACKp->Data = &GW_ReceDataBuf[5];
			/* �ж�������Ϣ */
			if((gACKp->GW_Adder 	== (0x53))&&
			   (gACKp->Func 			== (0x53))&&
			   (gACKp->Reserve1			== (0x53))&&
			   (gACKp->Visit_Adder		== (0x53))&&
			   (gACKp->Reserve2			== (0x53)))
			{
				/* У������ */
				uint16_t check;
					
				
				/* �������ݵ�ַ */
				gACKp->Check = (uint16_t*)&GW_ReceDataBuf[6];
				/* ����У�� */
				check = BigToLittle_Endian_HalfWord(CRC16_GetValue(GW_ReceDataBuf,(6u)));
				/* �ж�У��ֵ�Ƿ����� */
				if((*gACKp->Check == check)||(*gACKp->Check == 0xEC15))
				{
					/* У����ȷ�����ö����ص�ַ���� */
					GWReg.DR.Adder = *gACKp->Data;
					/* ���ö����ص�ַʧ��״̬ */
					GWReg.SR.ReadAdder = GW_Success;
				}else
				{
					/* У����� */
					/* ���ö����ص�ַʧ��״̬ */
					GWReg.SR.ReadAdder = GW_Fail;
				}
			}
			else
			{
				/* �رս��չ��̣����ͽ���ʧ����Ϣ */
				/* ���ö����ص�ַʧ��״̬ */
				GWReg.SR.ReadAdder = GW_Fail;
			}
			/* �رս��չ��� */
			GWLog_ReceData_Disable();
			
			/* ����ʧ����Ϣ */
			MboxErr = OSMboxPost (GW_ReceStateMboxp, (void*)&GWReg.SR.ReadAdder);
			/* �жϷ�����Ϣ״̬ */
			if(MboxErr == OS_ERR_NONE)
			{__nop();}
		}else
		{
			/* ����û�н����������������� */
			__nop();
		}
	}
}

#endif

#if (1)//�������������ʱ���ص�����
/* �������������ʱ�� ���ָ�� */
static OS_TMR* GWLog_Restart_ptmr;
/************************************************************************************************/
/* ������	: GWLog_Restart_CallBack										 	 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: �������������ʱ���ص�����						 									*/
/* ��������	: 2016/11/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void GWLog_Restart_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* �������������ʱ����Ϣ */
	INT8U tmrErr;
	/* �������������ʱ��״̬ */
	BOOLEAN tmrState;
	
	
	(void) p_arg;

	/* �������ص�Դ */
	DevGW_OpenPower();
	
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

/************************************************************************************************/
/* ������	: GWLog_ReceData_Enable										 	 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: ������������Ӧ�����ݡ�����Ӧ��ʱ�����ʱ���������������������ʱ��					*/
/* ��������	: 2016/11/17																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void GWLog_ReceData_Enable(void)
{
	/* �������������ʱ����Ϣ */
	INT8U tmrErr;
	/* �������������ʱ��״̬ */
	BOOLEAN tmrState;

	
	/* ʹ�ܽ�������״̬ */
	GWReg.SR.Run = GW_Busy;
	
	/* ֹͣ��ʱ�����ʱ�� */
	tmrState = OSTmrStart(GWLog_Overtime_ptmr,&tmrErr);
	/* �жϹر������ʱ��״̬ */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* �ر������ʱ��ʧ�� */
		__nop();
	}
	
	/* ֹͣ�������ݶ�ʱ�� */
	tmrState = OSTmrStart(GWLog_ReceData_ptmr,&tmrErr);
	/* �жϹرս������ݶ�ʱ��״̬ */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* �ر������ʱ��ʧ�� */
		__nop();
	}

	/* ����洢�Ĵ���λ��ָ�� */
	GW_ReceDataIndex = 0;
	
	/* ����洢�Ĵ��� */
	memset(GW_ReceDataBuf,0x00,GW_ReceDataBufSize);
}

/************************************************************************************************/
/* ������	: GWLog_ReceData_Disable										 	 			*/
/* ����  	: ptmr:�����ʱ�����ָ��,p_arg:�ص���������(��ʱû��)			 						*/
/* ���		: �� 																				*/
/* ����	 	: �رս�������Ӧ�����ݡ��ر�Ӧ��ʱ�����ʱ�����رս������������ʱ��					*/
/* ��������	: 2016/11/17																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void GWLog_ReceData_Disable(void)
{
	/* �������������ʱ����Ϣ */
	INT8U tmrErr;
	/* �������������ʱ��״̬ */
	BOOLEAN tmrState;
	/* ���������ź��� */
	INT8U gSemErr;
	
	
	/* ������������ */
	/* ʹ�ܽ�������״̬ */
	GWReg.SR.Run = GW_Idle;
	
	/* ֹͣ��ʱ�����ʱ�� */
	tmrState = OSTmrStop(GWLog_Overtime_ptmr,
						 OS_TMR_OPT_NONE,
						 (void*)0,
						 &tmrErr);
	/* �жϹر������ʱ��״̬ */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* �ر������ʱ��ʧ�� */
		__nop();
	}
	
	/* ֹͣ�������ݶ�ʱ�� */
	tmrState = OSTmrStop(GWLog_ReceData_ptmr,
						 OS_TMR_OPT_PERIODIC,
						 (void*)0,
						 &tmrErr);
	/* �жϹرս������ݶ�ʱ��״̬ */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* �ر������ʱ��ʧ�� */
		__nop();
	}
	
	/* ����洢�Ĵ���λ��ָ�� */
	GW_ReceDataIndex = 0;
	
	/* ����洢�Ĵ��� */
	memset(GW_ReceDataBuf,0x00,GW_ReceDataBufSize);
	
	/* ���������ź��� */
	gSemErr = OSSemPost (LogGW_Sem);
	if(gSemErr != OS_ERR_NONE)
	{__nop();}
}



/************************************************************************************************/
/* ������	: GWLog_ReceData										 	 					*/
/* ����  	: data:����			 																*/
/* ���		: �� 																				*/
/* ����	 	: �������ݹ���						 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void GWLog_ReceData(uint8_t data)
{
	
	/* �жϽ�������״̬ */	
	if(GWReg.SR.Run == GW_Idle)
	{
		/* ���ش��ڿ���״̬������Ҫ�������� */
		if(GW_ReceDataIndex)
		{
			/* ����洢�Ĵ���λ��ָ�� */
			GW_ReceDataIndex = 0;
		}
		return;
	}
	
	/* ������д��������ݴ洢�Ĵ��� */
	GW_ReceDataBuf[GW_ReceDataIndex] = data;
	/* �洢�Ĵ���λ��ָ�� ָ����һ��λ�� */
	GW_ReceDataIndex = (GW_ReceDataIndex+1)%GW_ReceDataBufSize;
}

/************************************************************************************************/
/* ������	: GWLog_Restart										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �������ع���					 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_Restart(void)
{
	/* �������������ʱ����Ϣ */
	INT8U tmrErr;
	/* �������������ʱ��״̬ */
	BOOLEAN tmrState;

	
	/* �ر����ص�Դ */
	DevGW_ClosePower();
	
	/* ����������ʱ������ʱ1S���� ���������ص������� */
	tmrState = OSTmrStart (GWLog_Restart_ptmr,&tmrErr);
	if((tmrErr != OS_ERR_NONE)||(tmrState != OS_FALSE))
	{__nop();}
	
	return GW_Success;
}

/************************************************************************************************/
/* ������	: GWLog_ReadProbe										 	 					*/
/* ����  	: probeNo:̽ͷ��ţ�probe:̽ͷ����			 										*/
/* ���		: �� 																				*/
/* ����	 	: ��ȡ̽ͷ����					 													*/
/* ��������	: 2016/11/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_ReadProbe(uint8_t probeNo,GW_ProbeType probetype,float* probe)
{	
	
	/* �ж�̽ͷ����Ƿ񳬳�ѡ��Χ */
	if(probeNo == 0)
	{
		/* δ���õ�̽ͷID�����������ݸ������ݡ� */
		*probe = GW_ReceTimeoutPara;
		return GW_Fail;
	}
	if(probeNo >= 33)
	{
		/* ������Χ */
		*probe = GW_ReceTimeoutPara;
		return GW_Fail;
	}
	
	/* ���û�ȡ������ */
	*probe = GWReg.DR.Data[(probeNo-1)][probetype];
	return GW_Success;
}

/************************************************************************************************/
/* ������	: GWLog_ReadLevel										 	 						*/
/* ����  	: probeNo:̽ͷ��ţ�probe:̽ͷ����			 										*/
/* ���		: �� 																				*/
/* ����	 	: ��ȡ̽ͷ����					 													*/
/* ��������	: 2016/11/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_ReadLevel(uint8_t probeNo,GW_Level* level)
{
	/* �ж�̽ͷ����Ƿ񳬳�ѡ��Χ */
	if(probeNo == 0)
	{
		/* δ���õ�̽ͷID�����������ݸ������ݡ� */
		*level = GW_Level_FALSE;
		return GW_Fail;
	}
	if(probeNo >= 33)
	{
		/* ������Χ */
		*level = GW_Level_FALSE;
		return GW_Fail;
	}
	
	/* ���û�ȡ������ */
	*level = GWReg.DR.Level[(probeNo-1)];
	return GW_Success;
}

/************************************************************************************************/
/* ������	: GWLog_UpdataProbeData										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �����������ݹ���					 													*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/*   ע��	: Ŀǰ1�ζ�ȡ���ݣ���32��̽ͷ������ȫ����ȡ������										*/
/************************************************************************************************/
static GW_Result GWLog_UpdataProbeData(void)
{	
	/* ��Ϣ����  ��Ϣָ�� */
	void* mbox_Mgs;
	/* �ź���������Ϣ */
	INT8U mSemErr;
	
	
	/* ��ѯ�����ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* �ж��ź���״̬ */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GW_Fail;
	}
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* ����ж�ûʲô��Ҫ */
//	/* �ж���Ϣ״̬ */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* �������ؽ������� */
	GWLog_ReceData_Enable();
	
	/* ����������� */
	GWCMD.GW_Adder 		= (GWReg.CR.Adder);
	GWCMD.Func 		 	= (GW_ReadData);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder 	= (GC_ReadData);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (0x80);//��ȡ32��ͨ������
	
	/* ����У�� */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* ���Ͷ���ַ���� */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));

	(void) mbox_Mgs;
	/* ����Ӧ��ɹ� */
	return GW_Success;
}

/************************************************************************************************/
/* ������	: GWLog_UpdataState										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��������״̬					 												*/
/* ��������	: 2016/11/28																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_UpdataState(void)
{
	/* ��Ϣ����  ��Ϣָ�� */
	void* mbox_Mgs;
	/* �ź���������Ϣ */
	INT8U mSemErr;
	/* ���������Ϣ */
	INT8U mBoxErr;
	
	
	/* ��ѯ�����ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* �ж��ź���״̬ */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GW_Fail;
	}
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* ����ж�ûʲô��Ҫ */
//	/* �ж���Ϣ״̬ */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* �������ؽ������� */
	GWLog_ReceData_Enable();
	
	/* ����������� */
	GWCMD.GW_Adder 		= (GWReg.CR.Adder);
	GWCMD.Func 		 	= (GW_ReadData);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder 	= (GC_ReadData);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (0x02);
	
	/* ����У�� */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* ���Ͷ���ַ���� */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* �ȴ�����������Ϣ */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* ��ȡ��Ϣ */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* ����Ӧ��ɹ� */
		return GW_Success;
	}
	
	/* ���ز������ */
	return GW_Fail;
}

/************************************************************************************************/
/* ������	: GWLog_ReadAdder										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �����ص�ַ����					 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_ReadAdder(uint8_t* adder)
{	
	/* ��Ϣ����  ��Ϣָ�� */
	void* mbox_Mgs;
	/* �ź���������Ϣ */
	INT8U mSemErr;
	/* ���������Ϣ */
	INT8U mBoxErr;
	
	
	/* ��ѯ�����ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* �ж��ź���״̬ */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GW_Fail;
	}
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* ����ж�ûʲô��Ҫ */
//	/* �ж���Ϣ״̬ */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* �������ؽ������� */
	GWLog_ReceData_Enable();
	
	/* ����������� */
	GWCMD.GW_Adder 		= 0x53;
	GWCMD.Func 		 	= GW_ReadAdder;
	GWCMD.Reserve1 	 	= 0x53;
	GWCMD.Visit_Adder 	= (GC_ReadAdder);
	GWCMD.Reserve2 	 	= 0x53;
	GWCMD.Data 	 	 	= 0x53;
	
	/* ���������ֵȫ��Ϊ���������У��Ҳ�ǳ��������ü��㡣 */
	GWCMD.Check 	 	= BigToLittle_Endian_HalfWord(0x15EC);
	
	/* ���Ͷ���ַ���� */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* �ȴ�����������Ϣ */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* ��ȡ��Ϣ */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* ���õ�ַ���� */
		*adder = GWReg.DR.Adder;
		/* ����Ӧ��ɹ� */
		return GW_Success;
	}
	
	/* ���ز������ */
	return GW_Fail;
}

/************************************************************************************************/
/* ������	: GWLog_SetAdder										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �������ص�ַ����				 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetAdder(uint8_t adder)
{
	/* ��Ϣ����  ��Ϣָ�� */
	void* mbox_Mgs;
	/* �ź���������Ϣ */
	INT8U mSemErr;
	/* ���������Ϣ */
	INT8U mBoxErr;
	
	
	/* ��ѯ�����ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* �ж��ź���״̬ */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GW_Fail;
	}
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* ����ж�ûʲô��Ҫ */
//	/* �ж���Ϣ״̬ */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* �������ؽ������� */
	GWLog_ReceData_Enable();
	
	/* ����������� */
	GWCMD.GW_Adder 	 	= (GWReg.DR.Adder);
	GWCMD.Func 		 	= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetAdder);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (adder);
	
	/* ����У�� */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* ���Ͷ���ַ���� */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* �ȴ�����������Ϣ */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* ��ȡ��Ϣ */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* ����Ӧ��ɹ� */
		/* �������ص�ַΪ���õ�ַ */
		GWReg.CR.Adder = GWReg.DR.Adder = adder;
		return GW_Success;
	}
	
	/* ���ز������ */
	return GW_Fail;
}

/************************************************************************************************/
/* ������	: GWLog_SetChannel										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �����ŵ�����					 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetChannel(uint8_t channel)
{
	/* ��Ϣ����  ��Ϣָ�� */
	void* mbox_Mgs;
	/* �ź���������Ϣ */
	INT8U mSemErr;
	/* ���������Ϣ */
	INT8U mBoxErr;
	
	
	/* ��ѯ�����ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* �ж��ź���״̬ */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GW_Fail;
	}
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* ����ж�ûʲô��Ҫ */
//	/* �ж���Ϣ״̬ */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* �������ؽ������� */
	GWLog_ReceData_Enable();
	
	/* ����������� */
	GWCMD.GW_Adder		= (GWReg.CR.Adder);
	GWCMD.Func 		 	= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetChannel);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (channel);
	
	/* ����У�� */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* ���Ͷ���ַ���� */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* �ȴ�����������Ϣ */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* ��ȡ��Ϣ */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* ����Ӧ��ɹ� */
		/* �����ŵ����� */
		GWReg.CR.Channel = channel;
		/* ���ز����ɹ� */
		return GW_Success;
	}
	
	/* ���ز������ */
	return GW_Fail;
}

/************************************************************************************************/
/* ������	: GWLog_SetActiveTime										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �������ݶ�ʧʱ�书��					 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetActiveTime(uint8_t time)
{
	/* ��Ϣ����  ��Ϣָ�� */
	void* mbox_Mgs;
	/* �ź���������Ϣ */
	INT8U mSemErr;
	/* ���������Ϣ */
	INT8U mBoxErr;
	
	
	/* ��ѯ�����ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* �ж��ź���״̬ */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GW_Fail;
	}
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* ����ж�ûʲô��Ҫ */
//	/* �ж���Ϣ״̬ */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* �������ؽ������� */
	GWLog_ReceData_Enable();
	
	/* ����������� */
	GWCMD.GW_Adder		= (GWReg.CR.Adder);
	GWCMD.Func 		 	= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetActiveTime);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (time);
	
	/* ����У�� */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* ���Ͷ���ַ���� */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* �ȴ�����������Ϣ */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* ��ȡ��Ϣ */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* ����Ӧ��ɹ� */
		/* ����������Чʱ�� */
		GWReg.CR.ActiveTime = time;
		/* ���ز����ɹ� */
		return GW_Success;
	}
	
	/* ���ز������ */
	return GW_Fail;
}

/************************************************************************************************/
/* ������	: GWLog_SetMode										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ����ģʽ����					 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetMode(GW_Mode mode)
{
	/* ��Ϣ����  ��Ϣָ�� */
	/* �ź���������Ϣ */
	INT8U mSemErr;
	/* ���������Ϣ */
	INT8U mBoxErr;
	/* ��Ϣ�������ݵ�ָ�� */
	void* mbox_Mgs;
	
	
	/* ��ѯ�����ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* �ж��ź���״̬ */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GW_Fail;
	}
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* ����ж�ûʲô��Ҫ */
//	/* �ж���Ϣ״̬ */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* �������ؽ������� */
	GWLog_ReceData_Enable();
	
	/* ����������� */
	GWCMD.GW_Adder		= (GWReg.CR.Adder);
	GWCMD.Func			= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetMode);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= ((uint8_t)mode);
	
	/* ����У�� */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* ���Ͷ���ַ���� */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* �ȴ�����������Ϣ */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* ��ȡ��Ϣ */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* ����Ӧ��ɹ� */
		/* ��������ģʽ���� */
		GWReg.CR.Mode = mode;
		/* ���ز����ɹ� */
		return GW_Success;
	}
	
	/* ���ز������ */
	return GW_Fail;
}

/************************************************************************************************/
/* ������	: GWLog_SetBaudrate										 	 				*/
/* ����  	: baudrate:����ͨ�Ų�����			 												*/
/* ���		: �� 																				*/
/* ����	 	: ���ò����ʹ���					 													*/
/* ��������	: 2016/11/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetBaudrate(uint32_t baudrate)
{
	/* ��Ϣ����  ��Ϣָ�� */
	void* mbox_Mgs;
	/* �ź���������Ϣ */
	INT8U mSemErr;
	/* ���������Ϣ */
	INT8U mBoxErr;
	
	
	/* ��ѯ�����ź��� */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* �ж��ź���״̬ */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* û���ź��������ز���ʧ�ܡ� */
		return GW_Fail;
	}
	
	/* �����������״̬��Ϣ������֮ǰ�յ�����ϢΪ���� */
	/* ��������һ����Ϣ����������Ϣ�Ϳ��Խ������Ϣ���������������� */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* ����ж�ûʲô��Ҫ */
//	/* �ж���Ϣ״̬ */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* �������ؽ������� */
	GWLog_ReceData_Enable();
	
	/* ����������� */
	GWCMD.GW_Adder		= (GWReg.CR.Adder);
	GWCMD.Func 		   	= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetBaudrate);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	   	= ((baudrate==19200)?1:0);
	
	/* ����У�� */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* ���Ͷ���ַ���� */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* �ȴ�����������Ϣ */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* ��ȡ��Ϣ */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* ����Ӧ��ɹ� */
		/* ��������ͨ�Ų��������� */
		GWReg.CR.Baudrate = baudrate;
		/* ���ز����ɹ� */
		return GW_Success;
	}
	
	/* ���ز������ */
	return GW_Fail;
}

/************************************************************************************************/
/* ������	: GWLog_Init										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ�����ع���					 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void GWLog_Init(LogGatewayInitParaStruct initPara)
{
	/* ���������ʱ������Ĵ��� */
	INT8U tmrErr;
	
	
	/* ��ʼ�����в��� */
	GWReg.CR.Adder 			= initPara.Adder;
	GWReg.CR.Channel 		= initPara.Channel;
	GWReg.CR.ActiveTime 	= initPara.ActiveTime;
	GWReg.CR.Mode 			= initPara.Mode;
	GWReg.CR.Baudrate 		= initPara.Baudrate;
	
	//�����ʱ����ʱ�ӽ������ó�1ms
	/* �����������ݳ�ʱ�����ʱ�� */
	GWLog_Overtime_ptmr = 
	OSTmrCreate (
					LogGW_ReceOvertimeValue,		//�����ʱ����������ʱ����ʱ����ûص�����
					LogGW_ReceOvertimeValue,		//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_ONE_SHOT,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)GWLog_Overtime_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"���ؽ��ճ�ʱ��ʱ��",				//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);
							
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}	
							
	/* ��������״̬�����ʱ�� */
	GWLog_ReceData_ptmr = 
	OSTmrCreate (
					1,								//�����ʱ����������ʱ����ʱ����ûص�����
					1,								//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_PERIODIC,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)GWLog_ReceData_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"���ؽ���״̬��ʱ��",				//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);					
							
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}
	
	/* �����������������ʱ�� */
	GWLog_Restart_ptmr = 
	OSTmrCreate (
					LogGW_RestartTimeValue,			//�����ʱ����������ʱ����ʱ����ûص�����
					LogGW_RestartTimeValue,			//�����ڶ�ʱģʽ�£����ûص�����������
					OS_TMR_OPT_ONE_SHOT,			//�����ʱ��ģʽѡ��:OS_TMR_OPT_ONE_SHOT->����������OS_TMR_OPT_PERIODIC->��������
					(OS_TMR_CALLBACK)GWLog_Restart_CallBack,	//�ص�����	��ʽ:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//�ص���������
					"����������ʱ��",				//�ص���������
					&tmrErr							//���������ʱ������ִ�н���Ĵ���
				);					
							
	/* �ж������ʱ������״̬ */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* ʧ�� */}
	
	
	/* ������������״̬��Ϣ */
	GW_ReceStateMboxp = OSMboxCreate ((void*)0);
	/* �жϽ�������״̬��Ϣ����״̬ */
	if(GW_ReceStateMboxp == ((OS_EVENT *)0))
	{__nop();/* ʧ�� */}
	
	/* ���������ź��� */
	LogGW_Sem = OSSemCreate (1u);
	
	/* ��ʼ������ */
	DevGW_Init(GWReg.CR.Baudrate,GWLog_ReceData);
	
	/* �ȴ�������������Դ�ȶ��� */
	OSTimeDly(10);
}

/************************************************************************************************/
/* ������	: GWLog_ConfigFunc										 	 							*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �������ع���					 													*/
/* ��������	: 2016/11/28																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_ConfigFunc(void)
{

	/* ���ܲ���״̬ */
	GW_Result gState;
	/* �����ص�ַ */
	uint8_t gAdder;
	/* ͨ�Ų���������״̬ */
	GW_Result gSetBaudrateState = GW_Fail;
	
	
	/* ������״̬ */
	GWReg.SR.Work = GWLog_ReadAdder(&gAdder);
	/* �ж�����״̬�Ƿ����� */
	if(GWReg.SR.Work == GW_Fail)
	{
		/* �жϵ�ǰ�Ĳ����� */
		if(GWReg.CR.Baudrate == (115200ul))
		{
			/* ��ʼ������ */
			DevGW_Init((19200ul),GWLog_ReceData);
		}else if(GWReg.CR.Baudrate == (19200ul))
		{
			/* ��ʼ������ */
			DevGW_Init((115200ul),GWLog_ReceData);
		}else
		{return GW_Fail;}
		
		/* �ȴ�������������Դ�ȶ��� */
		OSTimeDly(10);
		
		/* ������״̬ */
		GWReg.SR.Work = GWLog_ReadAdder(&gAdder);
		
		/* �ж�����״̬�Ƿ����� */
		if(GWReg.SR.Work == GW_Fail)
		{
			/* ģ��ͨ��ʧ��(Ӧ����û����������) */
			/* ��ʼ��32λ������ͨ�Ų����� */
			DevGW_Init(GWReg.CR.Baudrate,GWLog_ReceData);
			/* �������� */
			GWLog_Restart();
			return GW_Fail;
		}
	}else
	{
		/* ����ͨ�Ų�����״̬ */
		gSetBaudrateState = GW_Success;
	}
	
	/* �ж϶�ȡ�����ص�ַ��Ĭ�ϵĵ�ַ�Ƿ���ͬ */
	if(gAdder != GWReg.CR.Adder)
	{
		/* ����ͬ������Ĭ�ϵĵ�ַΪ��ǰ���ص�ַ */
		gState = GWLog_SetAdder(GWReg.CR.Adder);
		/* �ж�����״̬�Ƿ����� */
		if(gState == GW_Fail)
		{__nop();}
	}
	
	/* �����ŵ�(����) */
	gState = GWLog_SetChannel(GWReg.CR.Channel);
	/* �ж�����״̬�Ƿ����� */
	if(gState == GW_Fail)
	{__nop();}
	
	/* �������ݶ�ʧʱ�� */
	gState = GWLog_SetActiveTime(GWReg.CR.ActiveTime);
	/* �ж�����״̬�Ƿ����� */
	if(gState == GW_Fail)
	{__nop();}
	
	/* ���ù���ģʽ */
	gState = GWLog_SetMode(GWReg.CR.Mode);
	/* �ж�����״̬�Ƿ����� */
	if(gState == GW_Fail)
	{__nop();}
	
	/* �ж�ͨ�Ų������Ƿ���Ҫ��� */
	if(gSetBaudrateState == GW_Fail)
	{
		/* ģ��ͨ�ųɹ�(ģ��ĳ�ʼ�����������õĲ����ʲ���ͬ) */
		/* ����ģ��ͨ�Ų����� */
		GWLog_SetBaudrate(GWReg.CR.Baudrate);
		/* ��ʼ��32λ������ͨ�Ų����� */
		DevGW_Init(GWReg.CR.Baudrate,GWLog_ReceData);
	}
	
	/* �������� */
	GWLog_Restart();

	return GW_Success;
}

/************************************************************************************************/
/* ������	: BSP_LogGW_API_Init										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ���߼���API����				 												*/
/* ��������	: 2016/11/15																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void BSP_LogGW_API_Init(void)
{
	/* ����ϵͳ�ӿں��� */
	
	/* ��ʼ���ӿ� */
	LogGW_API.Init 			= GWLog_Init;
	/* �������ؽӿ� */
	LogGW_API.Restart 		= GWLog_Restart;
	/* �������ع��ܽӿ� */
	LogGW_API.ConfigFunc 	= GWLog_ConfigFunc;
	/* ��̽ͷ���ݽӿ� */
	LogGW_API.ReadProbe 	= GWLog_ReadProbe;
	/* ��̽ͷ���ݽӿ� */
	LogGW_API.ReadLevel 	= GWLog_ReadLevel;
	/* ������������ */
	LogGW_API.UpdataProbeData 	= GWLog_UpdataProbeData;
	/* �����ص�ַ */
	LogGW_API.ReadAdder 	= GWLog_ReadAdder;
	/* ������״̬ */
	LogGW_API.UpdataState 	= GWLog_UpdataState;
	/* �������ؽӿ� */
	LogGW_API.SetAdder	 	= GWLog_SetAdder;
	/* ���������ŵ�(����) */
	LogGW_API.SetChannel 	= GWLog_SetChannel;
	/* ��������������Чʱ�� */
	LogGW_API.SetActiveTime = GWLog_SetActiveTime;
	/* ��������������Чʱ�� */
	LogGW_API.SetMode 		= GWLog_SetMode;
	/* ��������������Чʱ�� */
	LogGW_API.SetBaudrate 	= GWLog_SetBaudrate;
}




#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
