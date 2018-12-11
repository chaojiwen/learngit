/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**�ļ���     : ComProtocol_JS.c																*/
/**����       : ���ݽ���ʡ���߹滮���������ṩ��ͨ��Э���ʽ��̽ͷ�����ϴ�����ƽ̨							*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1     ZWC         2018��10��19��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "ComProtocol_JS.h"
#if (AppGPRSComProtocol == 0x04)
#include <string.h>
#include "App_Save.h"
#include "Dev_Flash.h"
#include <stdio.h>
#include <stdlib.h>
#include "String_Utility.h"
#include "GSM_app.h"
/* ��ӡ���Ա�־ */
extern BOOL GSM_DEBUG_Flag;// TRUE

/*******************************************************************************/
/********************************* ȫ�ֱ������� *****************************/
ComProtocolZDEAppStruct    ComProtocolZDEApp; 	
/* û�в鵽ƥ���ͨ�� */
#define Sensors_CH_Null (0xFF)
/* �� Fram ���ݵĻ���ָ�� */
Save_Sensor_Data_Struct *read_sensor_data_p;

#define SEC_PER_8HOUR       (28800u)//8��Сʱ������


//CRC16_CCITT(1021)У��ֵ��
const int crc16_ccitt_table[] = { 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a,
            0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b,
            0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528,
            0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719,
            0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e,
            0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf,
            0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec,
            0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe, 0xdfdd,
            0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2,
            0x20e3, 0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3,
            0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0,
            0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x0691,
            0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806,
            0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37,
            0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64,
            0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55,
            0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 };
//У���׼  CRC16_CCITT(1021)�㷨
 int docrc16(uint8_t *pSrcData,int length) {
        int crc_reg = 0x0000;
        int i=0;
        for (i = 0; i <length; i++) {
            crc_reg =  crc16_ccitt_table[(crc_reg ^ pSrcData[i]) & 0xFF] ^ (crc_reg >> 8);
        }
        return crc_reg;
    }
 
		
		
uint32_t mktimev(RTC_T tm)
{
	if(0 >= (int)(tm.Mon -= 2) )
	{	/* 1..12 -> 11,12,1..10 */
		tm.Mon += 12;		/* Puts Feb last since it has leap day */
		tm.Year -= 1;
	}

	//�Ȼ�������ͷ�����õ�days����
	//�ٵó��ж��ٸ�Сʱ
	//�ٵó��ж��ٸ�����
	//���ó��ж��ٸ���
	return ( ( ( (uint32_t)(tm.Year/4 - tm.Year/100 + tm.Year/400 + 367*tm.Mon/12 + tm.Day) + \
		         tm.Year * 365 - 719499 													  \
			   ) * 24 + tm.Hour 															  \
			 ) * 60 + tm.Min 	 															  \
		   ) * 60 + tm.Sec; 	 
}
	
		

//����ص���ת��Ϊ�ٷֱ�
uint8_t   Power_BT_State(INSTRU_POWER* power_t)
{
	 uint8_t  Power_BT_Percentage=0;//��ص����ٷֱ�
	
	/* �жϵ�ǰ��ص���״̬ */
	if(power_t->Inf_State.BT_Quantity_State <10)
	{
		/* ��ص������� */
		
		 Power_BT_Percentage=0;//��ص����ٷֱ�
		
		
	}else if((power_t->Inf_State.BT_Quantity_State >=10)&&(power_t->Inf_State.BT_Quantity_State<30))
	{
		/* ��ص������� */
	
		 Power_BT_Percentage=40;//��ص����ٷֱ�
		
	}
	else if((power_t->Inf_State.BT_Quantity_State >=30)&&(power_t->Inf_State.BT_Quantity_State<70))
	{
		/* ��ص������� */
	
		 Power_BT_Percentage=80;//��ص����ٷֱ�
		
	}
	else if((power_t->Inf_State.BT_Quantity_State >=70)&&(power_t->Inf_State.BT_Quantity_State<100))
	{
		/* ��ص������� */
	
		 Power_BT_Percentage=100;//��ص����ٷֱ�
		
	}
	else
	{
	  /* ��ص������� */
		 Power_BT_Percentage=255;//��ص����ɼ��쳣
	}

  return	Power_BT_Percentage;//���ص�ص����ٷֱ�
}		


//��GSMģ���ź�ǿ��ת��Ϊ�ٷֱ�
uint8_t   GSM_SINGLE_State(void)
{
	 uint8_t  GSM_SINGLE_Percentage=0;//GSMģ���źŰٷֱ�
	/* �ж�GSMģ���ź�ǿ��״̬ */
	if(GPRS_SR_Reg.GPRS_Sign_Stre<10)
	{
		/* ��ص������� */
		
		 GSM_SINGLE_Percentage=0;//GSMģ���źŰٷֱ�
		
		
	}else if((GPRS_SR_Reg.GPRS_Sign_Stre >=10)&&(GPRS_SR_Reg.GPRS_Sign_Stre<12))
	{
		/* ��ص������� */
	
		 GSM_SINGLE_Percentage=40;//GSMģ���źŰٷֱ�
		
	}
	else if((GPRS_SR_Reg.GPRS_Sign_Stre>=12)&&(GPRS_SR_Reg.GPRS_Sign_Stre<15))
	{
		/* ��ص������� */
	
		 GSM_SINGLE_Percentage=80;//GSMģ���źŰٷֱ�
		
	}
	else if((GPRS_SR_Reg.GPRS_Sign_Stre>=15)&&(GPRS_SR_Reg.GPRS_Sign_Stre<32))
	{
		/* ��ص������� */
	
		 GSM_SINGLE_Percentage=100;//GSMģ���źŰٷֱ�
		
	}
	else
	{
	  /* ��ص������� */
		 GSM_SINGLE_Percentage=255;//��ص����ɼ��쳣
	}

  return	GSM_SINGLE_Percentage;//���ص�ص����ٷֱ�
}

void   fun1(char *SN)	
{

      ComProtocolJS_MH1* COMReg1 = (ComProtocolJS_MH1*)&ComProtocolZDEApp.DR.TXDR;
	    COMReg1->JS_SYNC_Flag    = JS_UL_SYNC_Identify;//����ͬ����־���� 
	    COMReg1->JS_STX_Flag     = JS_UL_STX_Identify ;//������ʼ��־����
	    COMReg1->JS_DATA_BODY_Len=SN_LEN_Bytes+strlen(SN)+Time_Stamp_Bytes+SENSOR_CH_NUM_LEN_Bytes+Sensor_CH_Num*Single_Sensor_Bytes+COM_END_Bytes; //�������ܳ��� 
	    COMReg1->JS_SN_DATA_Len=strlen(SN); //SN�����ݳ��� 
	    memcpy(COMReg1->JS_SN_DATA,SN,strlen(SN)) ;
}

BOOL   fun2(AppSaveFrameStruct* dataFrame,char *SN)	
{
//	    time_t  t=0;
	
	
////      struct tm stm;
		   RTC_T    t;
	
	
   	  ComProtocolJS_MH2* COMReg2 = (ComProtocolJS_MH2*)&ComProtocolZDEApp.DR.TXDR[COMReg1ExceptSN_Len+strlen(SN)];
	
	
	if((dataFrame->Timestamp.Year<=0)\
		|| (dataFrame->Timestamp.Month<=0)\
		|| (dataFrame->Timestamp.Day<=0))
	{
		return FALSE;
	}
	



		       /* ̽ͷ��Ϣ */
  	       /* ���òɼ�ʱ��  */
	         Log_printf("���ݲɼ�ʱ��:%d��%d��%d�� %dʱ%d��%d��\n",dataFrame->Timestamp.Year,
	                                                        dataFrame->Timestamp.Month,
	                                                        dataFrame->Timestamp.Day,
	                                                        dataFrame->Timestamp.Hour,
	                                                        dataFrame->Timestamp.Min,
	                                                        dataFrame->Timestamp.Sec);
	                                                        
	          Log_printf("ͨ��1����:%d  ͨ��2����:%d\n",(int)(dataFrame->Sensor[0]*100),(int)(dataFrame->Sensor[1]*100));
				 

	
	
/***************ʱ���**************/
////////      stm.tm_year =dataFrame->Timestamp.Year; //��
////////      stm.tm_mon  =dataFrame->Timestamp.Month;//��
////////      stm.tm_mday =dataFrame->Timestamp.Day;//��
////////      stm.tm_hour =dataFrame->Timestamp.Day;//ʱ
////////      stm.tm_min  =dataFrame->Timestamp.Min;//��
////////      stm.tm_sec  =dataFrame->Timestamp.Sec;//��
////////      stm.tm_year-=1900; /*���ֵ��ȥ1900���õ�tm�ṹ�б�����������*/ 
////////      stm.tm_mon-=1;    /*�·�ֵ��ȥ1���õ�tm�ṹ�б�����·�����*/
////////			stm.tm_mday-=1;    /*�շ�ֵ��ȥ1���õ�tm�ṹ�б�����շ�����*/
////////      t=mktime(&stm);  /* ���û����������ʱ��������������ֵΪ-1*/
////////	    t=(t-SEC_PER_16HOUR); //������������ʱ���Ǳ���ʱ��Ҫת��Ϊ��ʱ����ʱ����Ҫ��ȥ16Сʱʱ��/
				   t.Year=dataFrame->Timestamp.Year; //��
			     t.Mon=dataFrame->Timestamp.Month; //��
			     t.Day=dataFrame->Timestamp.Day;   //��
			     t.Hour=dataFrame->Timestamp.Hour; //ʱ
			     t.Min=dataFrame->Timestamp.Min;   //��
				   t.Sec=dataFrame->Timestamp.Sec;   //��

/******************************************/

				    Log_printf("���ݲɼ�ʱ��ʮ������:%d\n",(mktimev(t)-28800));
				 
	    COMReg2->JS_SN_Time_Stamp=(mktimev(t)-28800);//����ʱ���   //������������ʱ���Ǳ���ʱ��Ҫת��Ϊ��ʱ����ʱ����Ҫ��ȥ8Сʱʱ��/
	    
	    COMReg2->JS_SENSOR_CH_Num=Sensor_CH_Config_Num;//������ͨ������
			
			
		return TRUE;	
}
void   fun3(AppSaveFrameStruct* dataFrame,char *SN)
{
	    int i=0;
      ComProtocolJS_MH3* COMReg3 = (ComProtocolJS_MH3*)&ComProtocolZDEApp.DR.TXDR[COMReg1ExceptSN_Len+strlen(SN)+COMReg2_Len];
    

	   //��������ͨ������������ 
     for(i=0;i<Sensor_CH_Config_Num;i++)
	   {
	     	COMReg3->JS_SENSOR_Data[i]=(int)(dataFrame->Sensor[i]*100);	//����������ʪ������ת��Ϊ��������
	   }
}	

void   fun4(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,char *SN)
{
	
   	 ComProtocolJS_MH4* COMReg4 = (ComProtocolJS_MH4*)&ComProtocolZDEApp.DR.TXDR[COMReg1ExceptSN_Len+strlen(SN)+COMReg2_Len+Sensor_CH_Config_Num*sizeof(uint16_t)];
	   COMReg4->JS_RUN_STATE_Flag =JS_YUN_STATE_Identify;//����״̬��־λ 
	    if(config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State==TRUE)
			{
			       COMReg4->JS_RUN_State=0x00;//������������ӵ�   (��������״̬����)
			}
			else
			{
			      COMReg4->JS_RUN_State=0x01;//����δ������ӵ�   (��������״̬����)
			}
	   COMReg4->JS_POWER_STATE_Flag=JS_POWER_STATE_Identify;//��ص�����־λ 
	   COMReg4->JS_POWER_State= Power_BT_State(&Instru_Config_Inf_Tab.Inf_State.Instru_Power);//��ص���ֵ  0%--40%--80%--100%
	   COMReg4->JS_SIGNLE_STATE_Flag=JS_SIGNLE_STATE_Identify;//�ź�ǿ�ȱ�־λ 
	   COMReg4->JS_SIGNLE_State= GSM_SINGLE_State();//GSMģ���ź�ǿ��ֵ 
	   COMReg4->JS_DATA_CRC_CheckCode=docrc16(ComProtocolZDEApp.DR.TXDR,COMReg1ExceptSN_Len+strlen(SN)+COMReg2_Len+Sensor_CH_Config_Num*sizeof(uint16_t)+(COM_END_Bytes-sizeof(uint16_t)));
}
/************************************************************************************************/
/* ������	: ComProtocol_JS_CreateData										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ���ݽ���ʡ���߹滮��������ͨ��Э�飬�������ݡ�					 							*/
/* ��������	: 2018/10/23																		*/
/* ����汾	: V1.0																				*/
/************************************************************************************************/
//BOOL ComProtocol_JS_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,
//							                  uint8_t* destBuf,int* dataLen)
////BOOL ComProtocol_JS_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
BOOL ComProtocol_JS_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab, uint8_t* destBuf,int* dataLen)
{
//////	     int i=0;
	    uint16_t  Instrument_Serial_Number=0;//�������
//config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[]  ǰ2���ֽڱ�ʾ��ݺ��� ��3�͵�4���ֽ�Ϊ���(��������Ϊ����)
/******************SN��********************/
	   char SN[50]={0};
		 	/* Ӧ�ô洢���� */
	   AppSaveFrameStruct cSaveFrameReg;
		 	/* �洢����״̬ */
	   Save_ReadState SaveDataState;
		  
		 HexToStr((u8*)SN,config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number,2);
	
		 Instrument_Serial_Number=(config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[2]<<8)+ \
		                           config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[3];//��ȡ�������к�
 
		 sprintf(&SN[4],"%05d",Instrument_Serial_Number);//��ʽ���������SN��
		 
	    /* ������ǰ�洢����״̬ */
	   SaveDataState = AppSaveAPI.ReadSaveDataFrame(&cSaveFrameReg);
	   /* �ж϶����洢����״̬ */
	   if(Save_ReadFail == SaveDataState)
	   {
			 
			  *dataLen=0;
//////////			 if(GSM_DEBUG_Flag ==TRUE)
//////////			 {
//////////			 			 printf("û����ʷ����\n");
//////////			 }

	   	/* û�д洢���� */
	   	return FALSE;
	   }
		 
	   memset(ComProtocolZDEApp.DR.TXDR,0x00,sizeof(ComProtocolZDEApp.DR.TXDR));
/***********************��������*******************************/	

	     fun1(SN);
			 fun2(&cSaveFrameReg,SN);
			 fun3(&cSaveFrameReg,SN);
			 fun4(config_tab,fix_tab,SN);
		 
		 
	  //���ݳ��� 
	   ComProtocolZDEApp.DR.TXLen=COMReg1ExceptSN_Len+strlen(SN)+COMReg2_Len+Sensor_CH_Config_Num*sizeof(uint16_t)+COM_END_Bytes; 
/*********************************************************************/	
   //������װ��Ҫ���͵ĵ�����
     memcpy(destBuf,ComProtocolZDEApp.DR.TXDR, ComProtocolZDEApp.DR.TXLen);

     *dataLen=ComProtocolZDEApp.DR.TXLen;
		 
////		 printf("Ҫ�������ݵ�����:\n"); 
////    for(i=0;i<ComProtocolZDEApp.DR.TXLen;i++)
////    {
////    	printf("%02X ",ComProtocolZDEApp.DR.TXDR[i]);	
////	  }


	   	return TRUE;		
  } 

#endif



/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

