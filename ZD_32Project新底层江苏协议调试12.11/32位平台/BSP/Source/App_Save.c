/************************************************************************************************/
/** @file              : App_Save.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��8��27��															*/
/** @brief             : c file																	*/
/** @description       : Ӧ�����ݴ洢�ļ�														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "System_Config.h"

#include "App_Save.h"

#include "Dev_Fram.h"
#include "Dev_Flash.h"
#include "Dev_RTC.h"

#include "ucos_ii.h"
#include <string.h>
#include "app.h"

#include "stdlib.h"



/********************************* ̽ͷ���ݴ洢Ӧ�ýӿڼĴ������� *********************************/

/********************************* ̽ͷ���ݴ洢Ӧ�ýӿڼĴ������� *********************************/
AppSaveAPIStruct AppSaveAPI;

/************************************************************************************************/

#if (Menu_Save_Data_Func == 0x01)
/* ʵʱ���ݴ洢���� ����ָ�� */
static FunctionalState *Save_Data_Switch_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch;
/* ʵʱ���ݴ洢���� ���ݴ�С */
#define Save_Data_Switch_size (sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch))
	
/* ж����ʼ ����ָ�� */
#define Save_Data_Start_Upload_P    (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_upload_Adder)
/* ж����ʼ ���ݴ�С */
#define Save_Data_Start_Upload_size (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_upload_Adder))
/* ж������ ����ָ�� */
#define Save_Data_Stop_Upload_P     (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[48])
/* ж������ ���ݴ�С */
#define Save_Data_Stop_Upload_size  (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[48]))

#endif


/************************************************************************************************/
/* ������	: AppSave_ReadNum													 	 			*/
/* ����  	: ��.																				*/
/* ���		: ��. 																				*/
/* ����	 	: ��̽ͷ���ݴ洢����																	*/
/* ��������	: 2016/12/22																		*/
/************************************************************************************************/
static uint32_t AppSave_ReadNum(void)
{
	/* ����̽ͷ���ݴ洢���� */
	return Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM;
}


/************************************************************************************************/
/* ������	: AppSave_ReadSaveDataFrame													 	 	*/
/* ����  	: dataFrame:����������ָ֡��.														*/
/* ���		: ���洢���ݽ�� 																	*/
/* ����	 	: ���洢����.																		*/
/* ��������	: 2016/12/22																		*/
/************************************************************************************************/
Save_ReadState AppSave_ReadSaveDataFrame(AppSaveFrameStruct* dataFrame)
{
	/* ������������ָ�� */
	Save_Sensor_Data_Struct* sReadDatap;
	
	
	/* �ж�̽ͷ�洢�������� */
	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM < Save_Sensor_Data_Len)
	{
		/* û��̽ͷ���� */
		__nop();
		/* ��̽ͷ����ʧ�� */
		return Save_ReadFail;
	}
	
	/* �д洢���ݣ���ȡ���ݡ� */
	sReadDatap = (Save_Sensor_Data_Struct*)Flash_Read_Sensor_Data(
				 (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size),
				 Save_Sensor_Data_Len);

	//���� Flash ��ָ��
	/* ���� ��ȡ�����ݳ��� */
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Read_Pointer 		+= Save_Sensor_Data_Len; 
	/* �洢������ ��*/
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	-= Save_Sensor_Data_Len;
	
	/* ���òɼ�ʱ�� */
	dataFrame->Timestamp.Year 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS + 2000;
	dataFrame->Timestamp.Month	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS;
	dataFrame->Timestamp.Day 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS;
	dataFrame->Timestamp.Hour 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS;
	dataFrame->Timestamp.Min 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS;
	dataFrame->Timestamp.Sec 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS;
	/* ���ƴ��������� */
	memcpy(dataFrame->Sensor,sReadDatap->GPRS_IR.Sensor_Data,sizeof(dataFrame->Sensor));
	
	/* ��̽ͷ���ݳɹ� */
	return Save_ReadSuccess;
}

/************************************************************************************************/
/* ������	: AppSave_InitAPI										 	 						*/
/* ����  	: ��																				*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ��̽ͷ�洢�ӿڹ���					 											*/
/* ��������	: 2016/12/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void AppSave_InitAPI(void)
{

	/* ��̽ͷ���ݴ洢������ */
	AppSaveAPI.ReadSaveNum = AppSave_ReadNum;

	/* ��̽ͷ���ݴ洢������ */
	AppSaveAPI.ReadSaveDataFrame = AppSave_ReadSaveDataFrame;

}


#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
/************************************************************************************************/
/* ������	: App_Save_Write														 	 		*/
/* ����  	: New_state:�µĴ洢״̬(TRUE:�������ݴ洢����;FALSE:�ر����ݴ洢����).				*/
/* ���		: ��. 																				*/
/* ����	 	: д�洢����.																		*/
/* ��������	: 2015/08/27																		*/
/************************************************************************************************/
void App_Save_Config(FunctionalState New_state)
{

	/* �ж� ʵʱ���ݴ洢���� �� ���õ�״̬�Ƿ�һ�� */
	if(*Save_Data_Switch_P != New_state)
	{
		/* ��һ��,�޸�ʵʱ���ݴ洢����״̬. */
		*Save_Data_Switch_P = New_state;
		/* ���µ�ʵʱ���ݴ洢״̬��������. */
		Write_Config_Tab((u8*)Save_Data_Switch_P,Save_Data_Switch_size);
	}else
	{
		/* ���õ�״̬��ʵʱ���ݴ洢������ͬ,�������״̬. */
	}


}

/************************************************************************************************
*������     : App_Save_Print_Start_upload_Adder															
*�������  	: �� 									
*�������	  : �� 																				
*����	 	    : �洢ж����ʼʱ���ݼ�¼�ĵ�ַ�Ա��ڸ��������ַ��ȷ��ж����ʼ�ĵ�ַ															
*��������	  : 2018/05/23																		
************************************************************************************************/
void App_Save_Print_Start_upload_Adder(void)
{
	
   u32  Temp_Print_Start_Upload_Counter=0;//��ȡ��ǰж����ʼ���¼λ��
	
	 Temp_Print_Start_Upload_Counter=Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter;
	
	
			 if(((Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter-  \
		     Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter)==1) || \
			 ( (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter > \
			   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter))
		   ) 
			  //ʹж����ʼ����ж��������ɶԳ���   �粻��ͬ��������ò���
		  {
		     return ;
		  }
	
	  if(Temp_Print_Start_Upload_Counter>99)//��ж����ʼ�㳬����100��,����Ϣ��ֵ���ͷ��ʼ��¼  ʵ��ѭ���洢
		{
		   Temp_Print_Start_Upload_Counter=0;
		}
		
	
  *(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp_Print_Start_Upload_Counter])=   \
	  Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_End_Adder;
		
	/* �洢ж�����ַ������ڴ�ӡж����־���̵� */
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp_Print_Start_Upload_Counter]), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp_Print_Start_Upload_Counter])));
	
	 Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter++;//ÿ����һ�ο�ʼж������ ����ֵ��һ
	/* �洢ж����ʼ������ �Ա���ڴ�ӡ���ж����*/
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter)));
	
	/*��ж����ʼ��־λ��Ϊ1 ���ݴ˱�־�Ƿ���ʾж����ʼͼ�� 1����ʾ  0������ʾ */
	  Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag=1;
	
	
	
	/* �洢ж����ʼ������ �Ա���ڴ�ӡ���ж����*/
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag)));
	
	
	
//	 printf("ж����ʼ  ��ַ:%d\n", *(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp_Print_Start_Upload_Counter]));
}
/************************************************************************************************
*������     : App_Save_Print_Start_upload_Adder															
*�������  	: �� 									
*�������	  : �� 																				
*����	 	    : �洢ж������ʱ���ݼ�¼�ĵ�ַ�Ա��ڸ��������ַ��ȷ��ж����ʼ�ĵ�ַ															
*��������	  : 2018/05/23																		
************************************************************************************************/
void App_Save_Print_Stop_upload_Adder(void)
{

  u32  Temp_Print_Stop_Upload_Counter=0;//��ȡ��ǰж���������¼λ��
	
	 Temp_Print_Stop_Upload_Counter=Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter;
	
			 if(((Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter-  \
		     Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter)==1) || \
			 ( (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter > \
			   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter))
		   ) 
			  //ʹж����ʼ����ж��������ɶԳ���   �粻��ͬ��������ò���
		  {
		     return ;
		  }
			
			
		if(Temp_Print_Stop_Upload_Counter>99)//��ж�������㳬����100��,����Ϣ��ֵ���ͷ��ʼ��¼  ʵ��ѭ���洢
		{
		   Temp_Print_Stop_Upload_Counter=0;
		}
  *(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp_Print_Stop_Upload_Counter])=   \
	  Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_End_Adder;
		
	/* �洢ж���������ַ������ڴ�ӡж����־���̵� */
	Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp_Print_Stop_Upload_Counter]), \
	sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp_Print_Stop_Upload_Counter])));
	
	 Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter++;//ÿ����һ�ν���ж������ ����ֵ��һ
	/* �洢ж������������ �Ա���ڴ�ӡ���ж����*/
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter)));
	
	/*��ж����ʼ��־λ��Ϊ0 ���ݴ˱�־�Ƿ���ʾж����ʼͼ�� 1����ʾ  0������ʾ  */
	  Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag=0;
	
	
	/* �洢��ʾж����ʼͼ���־λ��Ϣ*/
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag)));
	


//	printf("ж������   ��ַ:%d\n", *(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp_Print_Stop_Upload_Counter]));
}


#endif


/******************* (C) ZHOU *****END OF FILE****/

















