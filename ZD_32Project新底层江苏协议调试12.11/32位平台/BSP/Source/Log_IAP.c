/************************************************************************************************/
/** @file              : Log_IAP.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��08��24��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--�����̼�Ӧ�ò��ļ�								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��08��24��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_IAP.h"
#include "Mcu_Config.h"
#include "Dev_Fram.h"
#include "stdio.h"
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)
#include "Mcu_Flash.h"
#include "app.h"
#include <string.h>
/* �̼���������Դ */
ConfigService_Para_Update* IAP_ParaSrc;

/* �̼�����ÿ�����������������������ֽ��� */
#define UploadUpdataBytes	       (0x800)     //ÿ����������2KBytes������

/* ����״̬��Ϣ��Ϣ*/
#define ConfigService_UploadUpdataStatus	       (0x08)
extern void ConfigService_Cmd(uint8_t item,uint8_t type);

/* ���ݼĴ��� */
/* �������ݼĴ��� ��������С */
#define IAP_TXDR_Size		(3000u)
/* �������ݼĴ��� ��������С */
#define IAP_RXDR_Size		(3000u)
/* �������ݼĴ��� */
static uint8_t IAP_TXDR[IAP_TXDR_Size];
/* �������ݼĴ��� */
static uint8_t IAP_RXDR[IAP_RXDR_Size];

/* �̼�����ͨ��Э��ṹ */
typedef struct
{
	//��·���䳵��ͨѶЭ Э����ô��ģʽ(big-endian)�������ֽ����������ֺ�˫�֡�
	//������ݽṹֻ����Ӧ2ͨ���Ĵ�����GPRS���ݷ��ͣ����Ҳ��ܴ���״̬��Ϣ��
	/* ͷ ��ʶλ */ //(1 Byte)
	uint8_t			Start_Iden;
	/* ͨ�� ��Ϣͷ */
	/* ��Ϣ ID(����) *///(2 Byte)
	uint8_t			ID_Type_HSB;
	uint8_t			ID_Type_LSB;
	/* ��Ϣ�� ���� *///(2 Byte)
	//��Ϣ������ ��Ϣ�峤��
	uint8_t	 		Attribute_HSB_Length:2;		
	//��Ϣ������ ���ݼ��ܷ�ʽ
	uint8_t	 		Attribute_HSB_Enc_Mode:3;
	//��Ϣ������ �ְ�
	uint8_t	 		Attribute_HSB_Sub_Flag:1;		
	//��Ϣ������ ����
	uint8_t 		Attribute_HSB_Reserve:2;
	//��Ϣ������ ��Ϣ�峤��
	uint8_t	 		Attribute_LSB_Length:8;
	/* �ն����к�(SN��): *///(6 Byte)
	uint8_t			SN_NO[6];
	/* ��Ϣ ��ˮ�� *///(2 Byte)
	uint8_t			Serial_NO_HSB;
	uint8_t			Serial_NO_LSB;

	/* ͨ�� ��Ϣ�� */
	/* ��Ϣ����:0XFA(�̼�����) *///(1 Byte)
	uint8_t			Body_Type;
	/* ��Ϣ������ */
	uint8_t			Data;
}
IAPComStruct;

/* �̼�����ͨ��Э��Ĭ������ */
static const IAPComStruct  IAPComDefault = 
{
	/* ͷ ��ʶλ */
	0X7E,
/****************************************************/
	/* ͨ�� ��Ϣͷ */
	/* ��Ϣ ID(����) */
	0X09,
	0X00,
	/* ��Ϣ�� ���� */
	/* ��Ϣ������ ��Ϣ�峤�� */
	0X00,	
	/* ��Ϣ������ ���ݼ��ܷ�ʽ */
	0X00,
	/* ��Ϣ������ �ְ� */
	0X00,		
	/* ��Ϣ������ ���� */
	0X00,
	/* ��Ϣ������ ��Ϣ�峤�� */
	0X00,
	/* �ն����к�(SN��): */
	0X00,0X00,0X00,0X00,0X00,0X00,
	/* ��Ϣ ��ˮ�� */
	0X00,
	0X00,
/****************************************************/
	/* ͨ�� ��Ϣ�� */
	/* ��Ϣ����: */
	0XFA,

};
/****************************************************/
/* �̼������ϴ����ݽṹ */
typedef struct
{
	/* �ļ����� */
	char DocumentName[32];
	/* ����� */
	uint32_t Packet;
	/* ����С */
	uint32_t Size;
	
}
IAP_UploadStruct;

/* Ĭ�Ϲ̼������ϴ����� */
static const IAP_UploadStruct IAP_UploadDefault = 
{
	/* �ļ����� */
	'\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', 
	'\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
	'\0','\0','\0','\0', '\0','\0','\0','\0', 
	/* ����� */
	1,
	/* ����С */
	0x4B0,
};
/****************************************************/
/* �̼����� �ļ������ṹ */
typedef struct
{
	/* ������ */
	uint32_t Total;
	/* ����� */
	uint32_t Num;
	/* ����С */
	uint32_t Size;
	/* ���� */
	uint8_t	 Data;
}
IAP_UpdateFileParaStruct;
/* �̼������ļ����� */
//static IAP_UpdateFileParaStruct IAP_UpdateFilePara;

/****************************************************/
/* �̼����������Ĵ��� */
static IAP_UploadStruct IAP_UploadParaCR;

///* �̼�����Ӧ�ò����ṹ */
//typedef struct
//{
//	/* ����״̬ */
//	BOOL RunState;
//	/* ������־ */
//	uint8_t* UpdateFlag;
//	/* ����״̬ */
//	BOOL UpdateState;
//}
//IAP_AppStruct;

/* �̼�����Ӧ�ò��� */
IAP_AppStruct IAP_App = 
{
	FALSE,
	(uint8_t*)0,
	FALSE,
     0,	
};

/****************************************************/


#define htonl(A)     ((((u32)(A) & 0xff000000) >> 24) | \
(((u32)(A) & 0x00ff0000) >> 8) | \
(((u32)(A) & 0x0000ff00) << 8) | \
(((u32)(A) & 0x000000ff) << 24))
/************************************************************************************************/
/* ������	: IAP_SetUploadPara																		*/
/* ����  	: documentname:�ļ����ַ�����packet:����ţ�size:����С			 					*/
/* ���		: �� 																				*/
/* ����	 	: �����ϴ����ݲ���								 									*/
/* �������� 	: 2016/08/25																		*/
/* �޸����� 	: 2016/08/25																		*/
/************************************************************************************************/
static void IAP_SetUploadPara(char* documentname,uint32_t packet,uint32_t size)
{
	/* �ļ������� */
	uint32_t documentnamelen;

	
	/* ��ʼ���̼������Ĵ��� */
	IAP_UploadParaCR = IAP_UploadDefault;
	
	/* �����ļ������� */
	documentnamelen = strlen(documentname);
	
	if(documentnamelen > sizeof(IAP_UploadParaCR.DocumentName))
	{
		documentnamelen = sizeof(IAP_UploadParaCR.DocumentName);
	}
	
	/* ���������ļ��� */
	memcpy(IAP_UploadParaCR.DocumentName,documentname,documentnamelen);
	
	/* ��С�θ�ʽת��Ϊ��˸�ʽ */
	packet = htonl(packet);
	/* ����� */
	IAP_UploadParaCR.Packet = packet;
	/* ��С�θ�ʽת��Ϊ��˸�ʽ */
	size = htonl(size);
	/* ����С */
	IAP_UploadParaCR.Size = size;

}

/************************************************************************************************/
/* ������	: IAP_ParseData																		*/
/* ����  	: data:���ݣ�len:���ݳ���			 												*/
/* ���		: ���ݽ���״̬ 																		*/
/* ����	 	: �������յ�������								 									*/
/* �������� 	: 2016/08/25																		*/
/* �޸����� 	: 2016/08/25																		*/
/************************************************************************************************/
static BOOL IAP_ParseData(uint8_t* data,uint32_t* len)
{
	/* ��Ϣͷָ�� */
	IAPComStruct * iapcomparap;
	
	/* У������ */
	uint8_t  check = 0;
	/* ������ */
	uint32_t num;
	
	
	/* �Ƚ��������������(��ʡ���ݿռ�) */
	memset(IAP_TXDR,0X00, sizeof(IAP_TXDR));
	
	/* 1������ת������ */
	/* ��ת�������ݴ浽������ */
	*len = GPRS_Network_To_Host_Translator_Data(IAP_TXDR,data,*len);
	/* ��ս���������(��ʡ���ݿռ�) */
	memset(data,0X00,*len);
	/* ��������������ת�Ƶ����������� */
	memcpy(data,IAP_TXDR,*len);
	
	/* 2���ж����ݺϷ��� */
	/* ��������ָ�� */
	iapcomparap = (IAPComStruct *)IAP_TXDR;
	
	/* a���Ƚ�SN���Ƿ�һ�� */
	if(memcmp(iapcomparap->SN_NO,
			 (uint8_t*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),
			  6) != 0)
	{
		/* SN�Ų�һ�� */
		return FALSE;
	}
	
	/* b������У�� */
	for(num = 1 ; num < (*len - 2); num++)
	{
		/* �� ��Ϣͷ �� ��Ϣ�� ���һ�� ���� ��� */
		check ^= IAP_TXDR[num];
	}
	
	/* �ж�У���Ƿ���ͬ */
	if(check != IAP_TXDR[*len - 2])
	{
		/* У�鲻��ͬ */
		return FALSE;
	}
	
	/* c����Ϣ������ */
	if(iapcomparap->Body_Type != 0xFA)
	{
		/* ��Ϣ�����Ͳ������÷��������� */
		return FALSE;
	}

	/* ���ݽ������ */
	return TRUE;
}

/************************************************************************************************/
/* ������	: IAP_Cmd																		 	*/
/* ����  	: para:�̼���������			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��������״ָ̬��								 									*/
/* �������� 	: 2016/08/24																		*/
/* �޸����� 	: 2016/08/24																		*/
/************************************************************************************************/
static void IAP_Cmd(IAP_UploadStruct* para)
{
	/* ͨ��Э��ָ�� */
	IAPComStruct* iapcomp = (IAPComStruct*)IAP_TXDR;
	/* ��Ϣ��ָ�� */
	IAP_UploadStruct * iapcombodyp;
	/* ���ݳ��� */
	uint32_t len;
	/* У��λָ�� */
	uint8_t * check;
	/* β ��ʶλ */
	uint8_t * end_Iden;
	/* ������ */
	uint32_t num;
	
	
	/* ������������� */
	memset(IAP_TXDR,0x00,sizeof(IAP_TXDR));
	
	/* ����ͨ����Ϣͷ */
	*iapcomp = IAPComDefault;
	
	/* ����SN�� */
	memcpy(iapcomp->SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),ENV_SN_NO_SIZE);
	
	/* ������Ϣ��ָ�� */
	iapcombodyp = ((IAP_UploadStruct *)&iapcomp->Data);
	
	/* ������Ϣ������ */
	*iapcombodyp = *para;
	
	/* �������ݽ�����ַ */
	end_Iden = (((uint8_t*)(&iapcombodyp->Size+1)));
	
	/* �������ݳ��� */
	len = ((uint32_t)(end_Iden - ((uint8_t*)&iapcomp->Body_Type)));
	
	/* ������Ϣ�峤�� */
	iapcomp->Attribute_HSB_Length = (len>>8);
	iapcomp->Attribute_LSB_Length = (len%256);
	
	/* ����У��λָ�� */
	check = end_Iden;
	
	/* �������ݳ��� */
	len = len + 13 - 1 ;//(14��ͨ��Э��̶����ݳ���-1ͷ��ʶλ)
	
	/* ����У���� */
	for(num = 1 ; num <= len ; num++)
	{
		/* �� ��Ϣͷ �� ��Ϣ�� ���һ�� ���� ��� */
		*check ^= IAP_TXDR[num];
	}
	
	/* �������ݳ��� */
	len = len + 1;//(+У��)
	
	//1��ת�� ����
	/* ת�� *//* ���GPRS�������ݳ��� */
	num = GPRS_Host_To_Network_Translator_Data(
		 &IAP_RXDR[1],&IAP_TXDR[1],len);
	
	/* ����ת��������� */
	memcpy(&IAP_TXDR[1],&IAP_RXDR[1],num);
	
	/* ����������ݼĴ��� */
	memset(IAP_RXDR,0x00,sizeof(IAP_RXDR));
	
	/* �������ݳ��� */
	len = num+1;//+1ͷ��ʶλ
	
	/* ����β��ʶλָ�� */
	end_Iden = &IAP_TXDR[len];
	/* ����β��ʶ */
	*end_Iden = 0X7E;

	/* ����GPRS�������ݲ��� */
	GPRS.IAP.Para.TXDR	= IAP_TXDR;
	GPRS.IAP.Para.TXLen	= len+1;//(����1�ֽڽ���������)
	GPRS.IAP.Para.RXDR	= IAP_RXDR;
	GPRS.IAP.Para.RXLen	= IAP_RXDR_Size;

	/* ���������¼� */	
	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,IAP_EVENT);
	
	
}
/************************************************************************************************/
/* ������	: IAP_UpdateProcess																	*/
/* ����  	: data:���ݣ�len:���ݳ���			 												*/
/* ���		: �� 																				*/
/* ����	 	: ������������								 										*/
/* �������� 	: 2016/08/25																		*/
/* �޸����� 	: 2016/08/25																		*/
/************************************************************************************************/
static void IAP_UpdateProcess(uint8_t* data,uint32_t len)
{
	/* ��Ϣͷָ�� */
	IAPComStruct * iapcomparap;
	/* ��Ϣ������ */
	IAP_UpdateFileParaStruct* iapcomfilep;
	IAP_UpdateFileParaStruct  iapcomfile;
	/* ����ָ�� */
	uint8_t* updatefiledata;
	/*��¼��ȡ��������ʧ�ܵĴ���;����ȡʧ�ܺ�ᷢ�����»�ȡ����������3��*/
static 	uint8_t  GetUpdateContentErrors=0;
	/* д�̼��ļ�����״̬ */
	BOOL download;
	/* �������� */ 
	if(IAP_ParseData(data,&len)==FALSE)
	{

		if(GetUpdateContentErrors<=20)
		{
			GetUpdateContentErrors++;
		/*��ȡ��������ʧ�� �������»�ȡ �����ļ���Ϣ ���� */
			IAP_Cmd(&IAP_UploadParaCR);
		}else//������20�ν�����������������ʧ��,������������������״̬����Ϊδ��ִ����������״̬,�����÷������Ǳ�δ����ȡ�����������Ļ����豸���ڴ˽�����������
		{
			IAP_App.RunState   = FALSE;
			GetUpdateContentErrors=0;
		}
		return;
	}

	/* ������Ϣͷָ�� */
	iapcomparap = (IAPComStruct *)data;
	
	/* ���ò�������ָ�� */
	iapcomfilep = (IAP_UpdateFileParaStruct*)(&iapcomparap->Data);
	
	/* ���������ļ�������С�� */
	iapcomfile.Total = htonl(iapcomfilep->Total);
	iapcomfile.Num 	 = htonl(iapcomfilep->Num);
	iapcomfile.Size  = htonl(iapcomfilep->Size);
	
	
	/*�����������Ȱٷֱ�*/
	IAP_App.UpdatePercent=(u8)((((float)iapcomfile.Num)/(iapcomfile.Total))*100);
	
	
	/* ��������ָ�� */
	updatefiledata = &iapcomfilep->Data;

	/* �жϰ���� */
	if(iapcomfilep->Num > 0)
	{
		/* ������д��Mcu��(Flash) */
		download = Write_Data((iapcomfile.Num-1)*UploadUpdataBytes,updatefiledata,iapcomfile.Size);
	}else
	{
		download = FALSE;
	}
	
	/* �ж�����д��״̬ */
	if(TRUE == download)
	{
		/* ����д��ɹ� */
		/* �жϵ�ǰ���ݰ���� */
		if(iapcomfile.Num == iapcomfile.Total)
		{
			/* �̼�������־ */
			uint8_t updateflag = TRUE;		
			/* ��ǰΪ���һ�����ݣ����ù̼������ɹ�״̬. */
			IAP_App.UpdateState = TRUE;
			/* ���ó�����³ɹ���־(�ṩ��Bootloaderʶ��). */
			Write_Para(ENV_APP_UPDATED_START,&updateflag,ENV_APP_UPDATED_SIZE);
			/* ɾ������״̬ */
			IAP_App.RunState = FALSE;
			/* ɾ��������־ */
			*IAP_App.UpdateFlag = FALSE;
			
	     OSTimeDlyHMSM(0,0,2,0);//��ʱ2���Ӹ���ƽ̨���������ɹ�
			ConfigService_Cmd(ConfigService_UploadUpdataStatus,0x01);			/* ��������ƽ̨�������*/

			printf("�������ݽ������......\r\n���͸������÷������豸�Ѿ�����������ݵĽ�����Ϣ......\r\n");

				
		}else
		{
		  printf("�ѻ�ȡ����%d����������\n",iapcomfile.Num);	

			/* δ��ɹ̼�����������������һ������ */
			IAP_SetUploadPara(IAP_ParaSrc->DocumentName,(iapcomfile.Num+1),UploadUpdataBytes);
			/* ���� ��ȡ �����ļ���Ϣ ���� */
			IAP_Cmd(&IAP_UploadParaCR);
		}
	}else//����ǰ���ݰ�д��ʧ��,������ڴ�����ð�����
	{
		/* δ��ɹ̼�����������������һ������ */
			IAP_SetUploadPara(IAP_ParaSrc->DocumentName,(iapcomfile.Num),UploadUpdataBytes);
			/* ���� ��ȡ �����ļ���Ϣ ���� */
			IAP_Cmd(&IAP_UploadParaCR);
		//��ĩǰΪֹ��δ����д����ʧ�ܵ������
		/* �������� */
	}
	
}

/************************************************************************************************/
/* ������	: IAP_UploadLog																		*/
/* ����  	: documentname:�ļ����ַ�����packet:����ţ�size:����С			 					*/
/* ���		: �� 																				*/
/* ����	 	: �����ϴ����ݲ���								 									*/
/* �������� 	: 2016/08/25																		*/
/* �޸����� 	: 2016/08/25																		*/
/************************************************************************************************/
void IAP_UploadLog(GPRS_Struct* gprs)
{

//	/* �ж�������־ */
	if(FALSE == *IAP_App.UpdateFlag)
	{
		/* ֹͣ���� */
		IAP_App.RunState = FALSE;
		return;
	}	
	/* �ж�ͨ��״̬ */
	if(TRUE == gprs->IAP.State.Result)
	{
		/* ͨ�ųɹ� */
		/* �Ѿ����������ļ���������ʼ���� */
		IAP_UpdateProcess(gprs->IAP.Para.RXDR,gprs->IAP.Para.RXLen);
	}else
	{
		/* ͨ��ʧ�ܣ����»�ȡ��ǰ���ݰ��� */
		IAP_Cmd(&IAP_UploadParaCR);
	}

}

/************************************************************************************************/
/* ������	: IAP_StopRun																	*/
/* ����  	: ��		 																		*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ����								 									*/
/* �������� 	: 2016/08/30																		*/
/* �޸����� 	: 2016/08/30																		*/
/************************************************************************************************/
void IAP_StartRun(void)
{
	/* ������������״̬ */
	IAP_App.RunState = TRUE;
}

/************************************************************************************************/
/* ������	: IAP_StopRun																	*/
/* ����  	: ��		 																		*/
/* ���		: �� 																				*/
/* ����	 	: ֹͣ����								 									*/
/* �������� 	: 2016/08/30																		*/
/* �޸����� 	: 2016/08/30																		*/
/************************************************************************************************/
void IAP_StopRun(void)
{
	/* ������������״̬ */
	IAP_App.RunState = FALSE;
}

/************************************************************************************************/
/* ������	: IAP_CheckFlag																		 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �̼�������־��ѯ								 									*/
/* �������� 	: 2016/08/24																		*/
/* �޸����� 	: 2016/08/24																		*/
/************************************************************************************************/
extern ConfigService_AppDef ConfigService_App;
void IAP_CheckFlag(void)
{
	/* ��ѯ�̼�������־ */
	if((*IAP_App.UpdateFlag == TRUE)&&(IAP_App.RunState == FALSE))
	{	
		   OSTaskSuspend(APP_TASK_Save_PRIO);//�رմ洢����
		   OSTaskSuspend(APP_TASK_Key_PRIO);//�رհ�������AppTaskADC
			 OSTaskSuspend(APP_TASK_ADC_PRIO);//�ر�ADC�������
		   OSTaskSuspend(APP_TASK_COM_PRIO);//�ر����ù�������               
		   OSTaskSuspend(APP_TASK_Beep_PRIO);//�رշ���������                 
		   OSTaskSuspend(APP_TASK_Light_PRIO);//�ر�LED����������                  
	                     
		
			 printf("�Ѿ��������ļ�����Ϣ���Ƶ�������Ϣ������\n��ʼ�����������ݰ�\n");

		
		
		
		
		
		
		
		/* ��ʼ���� */
		IAP_StartRun();
		/* ����̼������������� */
		Erase_Para();
		/* ���ù̼��������� */
		IAP_SetUploadPara(IAP_ParaSrc->DocumentName,1,UploadUpdataBytes);
		/* ���� ��ȡ �����ļ���Ϣ ���� */
		IAP_Cmd(&IAP_UploadParaCR);
	}
}

/************************************************************************************************/
/* ������	: IAP_ParaInit																		*/
/* ����  	: updateparap:�̼���������ָ��		 												*/
/* ���		: �� 																				*/
/* ����	 	: �̼�����������ʼ��								 									*/
/* �������� 	: 2016/08/28																		*/
/* �޸����� 	: 2016/08/28																		*/
/************************************************************************************************/
void IAP_ParaInit(ConfigService_Para_Update* updateparap,uint8_t* updateflag)
{
	
	/* ��ʼ���̼���������ָ�� */
	IAP_ParaSrc = updateparap;
	/* ��ʼ���̼�������־ָ�� */
	IAP_App.UpdateFlag = updateflag;
}

/************************************************************************************************/
/* ������	: IAP_UpdateSatte																	*/
/* ����  	: ��					 															*/
/* ���		: �̼�����״̬ 																		*/
/* ����	 	: ��ȡ�̼�����״̬								 									*/
/* �������� 	: 2016/08/28																		*/
/* �޸����� 	: 2016/08/28																		*/
/************************************************************************************************/
BOOL IAP_UpdateSatte(void)
{
	/* ���ص�ǰ�̼�����״̬ */
	return IAP_App.UpdateState;
}
/************************************************************************************************/
/**	�� �� ��:mcu_reset															*/
/**	����˵��: �����豸  ��λ��Ƭ��																	*/
/**	��    �Σ���																	*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/	
void mcu_reset(void)//�����豸  ��λ��Ƭ��
{
	  __set_FAULTMASK(1);//�ر����ж�
    NVIC_SystemReset();//����Ƭ������
}
#endif
#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
