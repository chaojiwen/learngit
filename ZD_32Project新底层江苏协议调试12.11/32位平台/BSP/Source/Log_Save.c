/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/10/27																			 */
/*************************************************************************************************/

#include "interface.h"
#include  "app.h"

#include "Log_Adc1.h"
#include "Log_Battery.h"
#include "Log_RF_Card.h"
#include "Log_Save.h"
#include "Log_Sensor.h"

#include "Dev_Flash.h"
#include "Dev_Fram.h"
#include "Dev_RTC.h"

#include "Mcu_Usart.h"
#include "string.h"


/* �洢���������ݼĴ��� */
static Save_Sensor_Data_Struct Save_Sensor_Reg;
/* �洢����ʱ��� */
static DATETIME_Storage_Format Save_Timestamp;

/* �������� */
static void Sort_Out_Save_Data(INSTRU_SENSOR_FIX* sensor_f);


/*
*********************************************************************************************************
*	�� �� ��: Cycle_Save_Data
*	����˵��: �����Ե��� ���� ������ʱ�䡱�������������ݡ�����GPS���ݡ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* ��ɾ�����ݵ�FLASH�������� */
	u32 flash_sector_erase;
	
	
	//����д��Ŀ����ӦΪ����FLASH�ڱ߽��ʱ����ǰ������������д����������ʱ�ں�һ���������ݾ�д���쳣�������ͻᵼ������ʱ���޷��ϴ������ˡ�
	/* �ж�Flashдָ�� ���� 1������ ��Χ,�� ������Ҫд��Flashҳ */
	if((((fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer + Save_Sensor_Data_Len)%Flash_Capacity_Size) >= fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer)&&
	   (((fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer + Save_Sensor_Data_Len)%Flash_Capacity_Size) < (fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer+Flash_Sector_Size)))
	{
		/* ������������ */
		OSSchedLock();
		/* д�����ݳ���һ��������С����Ҫ����һ�������������Ժ������ */
		SPI_FLASH_SectorErase(fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer);
		/* ������������ */
		OSSchedUnlock();
		/* ������ָ�붨λ����һ������ */
		fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer = (fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer+Flash_Sector_Size)%Flash_Capacity_Size;
	}
	
	/* ������Ҫ��������� */
	Sort_Out_Save_Data(&fix_tab->Inf_State.Instru_Sensor);
	/* ����������ת�Ƶ�Flash�� */
	Flash_Write_Sensor_Data((u8*)&Save_Sensor_Reg,
							fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer%Flash_Capacity_Size,
							Save_Sensor_Data_Len);
	
	/* ����Flashдָ�� */
	fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer 	= (fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer+Save_Sensor_Data_Len)%Flash_Capacity_Size;
	/* ����Flash���ݴ洢���� */
	fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	+= Save_Sensor_Data_Len;
	
	/* ���� ǰһ�� ��ɾ�����ݵ� FLASH���� */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer == 0)
	{
		/* ��ɾ�������������һ�� */
		flash_sector_erase = Flash_Capacity_Size - Flash_Sector_Size;
	}else
	{
		/* ��ɾ���������������һ�� */
		flash_sector_erase = fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer - Flash_Sector_Size;
	}
	
	/* �ж϶����ݵ�ַָ���Ƿ��ڱ�ɾ����FLASH�����ڡ� */
	if((fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer >= flash_sector_erase)&&
	   (fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer < (flash_sector_erase+Flash_Sector_Size))&&
	   ((fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer-(s32)Save_Sensor_Data_Len) < fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer))
	{
		
		/* �жϴ�ӡ����ʼ���ݵ�ַ С�� Flash������ַ */
		while(fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer < (flash_sector_erase+Flash_Sector_Size))
		{
			/* ��ӡ����ʼ���ݵ�ַ С�� Flash������ַ */
			/* ���� ��ӡ��ʼ���ݵ�ַ */
			fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer += Save_Sensor_Data_Len;
		}
		/* ���� ��ӡ��ʼ���ݵ�ַ */
		fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer = fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size;
	}
	
	/* ����Flash���ñ���� */
	Write_Config_Tab(fix_tab->Inf_State.Instru_Flash.Inf_State_Space,sizeof(INSTRU_FLASH_FIX));
	
}

/*************************************************************************************************/
/* ������	: Sort_Out_Save_Data												 	 			 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: ������Ҫ���浽Fram�е����� 									 	 	 			 */
/* ��������  : 2015/03/30																		 */
/*************************************************************************************************/
static void Sort_Out_Save_Data(INSTRU_SENSOR_FIX* sensor_f)
{
	/* ���������ݴ�����ݵĿ�ʼλ�� */
	u8 sensor_data_offset = 0;
	
	/* ������ݻ��� */
	memset(Save_Sensor_Reg.Save_Sensor_Data_Buffer,0X00,sizeof(Save_Sensor_Data_Struct));

	/* �� �� ��������ݱ��浽������ */
	Save_Sensor_Reg.GPRS_IR.Timestamp = Save_Timestamp;

#if (Sensor_Function_Config_Enable != 0x00)
	//2�����ô���������
	
/* �ж� ͨ��1 */
#if	(Sensor_CH1_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data;
#endif
	
/* �ж� ͨ��2 */
#if	(Sensor_CH2_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data;
#endif
	
/* �ж� ͨ��3 */
#if	(Sensor_CH3_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data;	
#endif	
	
/* �ж� ͨ��4 */
#if	(Sensor_CH4_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data;	
#endif	
	
/* �ж� ͨ��5 */
#if	(Sensor_CH5_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��6 */
#if	(Sensor_CH6_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��7 */
#if	(Sensor_CH7_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��8 */
#if	(Sensor_CH8_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��9 */
#if	(Sensor_CH9_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��10 */
#if	(Sensor_CH10_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��11 */
#if	(Sensor_CH11_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��12 */
#if	(Sensor_CH12_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��13 */
#if	(Sensor_CH13_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��14 */
#if	(Sensor_CH14_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��15 */
#if	(Sensor_CH15_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��16 */
#if	(Sensor_CH16_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��17 */
#if	(Sensor_CH17_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��18 */
#if	(Sensor_CH18_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��19 */
#if	(Sensor_CH19_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��20 */
#if	(Sensor_CH20_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��21 */
#if	(Sensor_CH21_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��22 */
#if	(Sensor_CH22_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��23 */
#if	(Sensor_CH23_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��24 */
#if	(Sensor_CH24_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��25 */
#if	(Sensor_CH25_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��26 */
#if	(Sensor_CH26_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��27 */
#if	(Sensor_CH27_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��28 */
#if	(Sensor_CH28_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data;	
#endif
	
/* �ж� ͨ��29 */
#if	(Sensor_CH29_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��30 */
#if	(Sensor_CH30_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��31 */
#if	(Sensor_CH31_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data;	
#endif

/* �ж� ͨ��32 */
#if	(Sensor_CH32_Config_Type != Sensor_Null)
	Save_Sensor_Reg.GPRS_IR.Sensor_Data[sensor_data_offset++] = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data;	
#endif


	
#endif
	
	//3���������ܻ�����������
	{
		/* �������� */
	}


}


/*************************************************************************************************/
/* ������	: Save_Get_Timestamp												 	 			 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: ��������壬���Դ�����ɾ���������������͵��¼� 									 	 	 */
/* ��������  : 2015/03/30																		 */
/*************************************************************************************************/
void Save_Get_Timestamp(DATETIME_Storage_Format* Timestamp)
{
	/* ��ȡ ʱ��� */
	Save_Timestamp = *Timestamp;
}

/*************************************************************************************************/
/* ������	: Save_Tmp_To_Float												 	 			 	 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: ���洢���¶�����ת���ɸ����͵�����		 									 	 	 */
/* ��������  : 2015/04/28																		 */
/*************************************************************************************************/
float Save_Tmp_To_Float(float tmp)
{
	
	if(tmp == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		return Wireless_RF_Card_Data_Rece_Timeout_Value;
	}else
	{
		return tmp;
	}

}

/*************************************************************************************************/
/* ������	: Save_Hum_To_Float												 	 			 	 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: ���洢���¶�����ת���ɸ����͵�����		 									 	 	 */
/* ��������  : 2015/04/28																		 */
/*************************************************************************************************/
float Save_Hum_To_Float(float hum)
{
	if(hum == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		return Wireless_RF_Card_Data_Rece_Timeout_Value;
	}else
	{
		return hum;	
	}
}

#if (Gateway_NJSY_Enable == 0x01)
/************************************************************************************************/
/* ������	: SaveGw_TimeoutParaCheck												 	 		*/
/* ����		: data:����																 		 	*/
/* ���		: �������ݲ�Ϊ�����ź����ݣ���ô����ԭʼ���ݣ����������ź����ݣ��������ź�ֵ�� 			*/
/* ����		: �ж����ص��¶������Ƿ�Ϊ���źŵ�����	(��ʱ����)	 									*/
/* ��������  : 2016/12/05																		*/
/************************************************************************************************/
float SaveGw_TimeoutParaCheck(float data)
{
	if(data == Gateway_NJSY_TimeoutPara)
	{
		return Gateway_NJSY_TimeoutValue;
	}else
	{
		return data;	
	}
}
#elif (Gateway_HZZH_Enable == 0x01)
/************************************************************************************************/
/* ������	: SaveGw_TimeoutParaCheck												 	 		*/
/* ����		: data:����																 		 	*/
/* ���		: �������ݲ�Ϊ�����ź����ݣ���ô����ԭʼ���ݣ����������ź����ݣ��������ź�ֵ�� 			*/
/* ����		: �ж����ص��¶������Ƿ�Ϊ���źŵ�����	(��ʱ����)	 									*/
/* ��������  : 2016/12/05																		*/
/************************************************************************************************/
float SaveGw_TimeoutParaCheck(float data)
{
	if(data == Gateway_HZZH_TimeoutPara)
	{
		return Gateway_HZZH_TimeoutValue;
	}else
	{
		return data;	
	}
}
#endif

/************************************************************************************************/
/* ������	: Log_Save_Flag_Check													 	 		*/
/* ����  	: *save_inf:�洢������Ϣָ��;*system_time:ϵͳʱ��ָ��.									*/
/* ���		: ��. 																				*/
/* ����	 	: �����趨�Ĵ洢ʱ����(�����洢ʱ��,�����洢ʱ��,ʵʱ���ݴ洢����),���ͱ����־.			*/
/* ��������	: 2015/08/27																		*/
/************************************************************************************************/
void Log_Save_Flag_Check(INSTRU_MENU *save_inf,Times *system_time)
{
/* ��Դ��ѹֵ����ָ�� */
#define Power_Value_P	(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power.Inf.BT_Quantity_Sampl_Value)	
	
	
	/* ���ʹ洢��־�Ľ��. */
	INT8U		err;
	/* ϵͳ �� ʱ������. */
	u32 		system_second_b;
	/* ת��ϵͳʱ���ı����ݼĴ����е��·�ֵ����˽�ϵͳʱ��ŵ������ʱʫ��Ĵ�����. */
	Times 		time_b = *system_time;
	/* ���ݴ洢ʱ���¼. */
	static u32 	store_second_b = 0;

	
//	/* �жϵ�ǰ��Դ��״̬ */
//	if(Power_Record_Function_Check(*Power_Value_P) == FALSE)
//	{
//		/* ��Դ��ѹ�����趨ֵ�����������ݼ�¼����. */
//		__nop();
//		return;
//	}

#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
	/* �ж� ʵʱ���ݴ洢����. */
	if(save_inf->Inf_State.Menu_Save_Data_Switch == TRUE)
#endif
	{

		/* ��ϵͳ��ʱ��ת������������. */
		system_second_b = RTC_Date_Time_To_Second(&time_b);
		/* �ж����ݴ洢�Ƿ���ͬһʱ������ظ��洢. */
		if(store_second_b == system_second_b)
		{
			/* ʱ����ͬ���ٴ洢. */
			return;
		}
		/* ʱ�䲻��ͬ,��¼��ǰʱ��. */
		store_second_b = system_second_b;
		
		/* ��ǰ������:δ����. */
		if((system_second_b%save_inf->Inf_State.Normal_Save_Data_Interval)==0)
		{
			/* ʱ�� �ﵽ ����ʱ�䣬����һ�α���. */
			/* ���� ������� �����������ݰ� ��־. */
			OSFlagPost( 				//���־���ź�
			Str_Save_Flag,				//���ͱ�־��ָ��
			(OS_FLAGS)0XFF, 			//ѡ��Ҫ���͵��ź� ����1λ���ź� 0010  ͬ����2ǿ��ת��ΪOS_FLAGS�͵�����
										//��Ϊ�ź�ΪOS_FLAGS�͵�
			OS_FLAG_SET,				//�ź���Ч��ѡ�� �ź���1  OS_FLAG_SETΪ��1	OS_FLAG_CLRΪ��0
			&err);						//��־�ź�
		}
		/* ��ǰ������:����. */
		else if((system_second_b%save_inf->Inf_State.Alarm_Save_Data_Interval)==0)
		{
			/* ʱ�� �ﵽ ����ʱ��. */
			/* �ж� ��������ǰ����״̬. */
			if(0
#if (Sensor_CH1_Config_Type != Sensor_Null)				
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Alarm_State
#endif
#if (Sensor_CH2_Config_Type != Sensor_Null)			
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Alarm_State
#endif
#if (Sensor_CH3_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Alarm_State
#endif
#if (Sensor_CH4_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Alarm_State
#endif
#if (Sensor_CH5_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Alarm_State
#endif
#if (Sensor_CH6_Config_Type != Sensor_Null)			
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Alarm_State
#endif
#if (Sensor_CH7_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Alarm_State
#endif
#if (Sensor_CH8_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Alarm_State
#endif
#if (Sensor_CH9_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Alarm_State
#endif
#if (Sensor_CH10_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Alarm_State
#endif
#if (Sensor_CH11_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Alarm_State
#endif
#if (Sensor_CH12_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Alarm_State
#endif
#if (Sensor_CH13_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Alarm_State
#endif
#if (Sensor_CH14_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Alarm_State
#endif
#if (Sensor_CH15_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Alarm_State
#endif
#if (Sensor_CH16_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Alarm_State
#endif
#if (Sensor_CH17_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Alarm_State
#endif
#if (Sensor_CH18_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Alarm_State
#endif
#if (Sensor_CH19_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Alarm_State
#endif
#if (Sensor_CH20_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Alarm_State
#endif
#if (Sensor_CH21_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Alarm_State
#endif
#if (Sensor_CH22_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Alarm_State
#endif
#if (Sensor_CH23_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Alarm_State
#endif
#if (Sensor_CH24_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Alarm_State
#endif
#if (Sensor_CH25_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Alarm_State
#endif
#if (Sensor_CH26_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Alarm_State
#endif
#if (Sensor_CH27_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Alarm_State
#endif
#if (Sensor_CH28_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Alarm_State
#endif
#if (Sensor_CH29_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Alarm_State
#endif
#if (Sensor_CH30_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Alarm_State
#endif
#if (Sensor_CH31_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Alarm_State
#endif
#if (Sensor_CH32_Config_Type != Sensor_Null)		
			||Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Alarm_State
#endif
			)
			{
				/* ���� ������� �����������ݰ� ��־ */
				OSFlagPost( 				//���־���ź�
				Str_Save_Flag,				//���ͱ�־��ָ��
				(OS_FLAGS)0XFF, 			//ѡ��Ҫ���͵��ź� ����1λ���ź� 0010  ͬ����2ǿ��ת��ΪOS_FLAGS�͵�����
											//��Ϊ�ź�ΪOS_FLAGS�͵�
				OS_FLAG_SET,				//�ź���Ч��ѡ�� �ź���1  OS_FLAG_SETΪ��1	OS_FLAG_CLRΪ��0
				&err);						//��־�ź�
			}
		}


	}
	
}



/******************* (C) ZHOU *****END OF FILE****/


