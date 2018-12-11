/************************************************************************************************/
/** @file              : Log_Print.h															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��6��28��															*/
/** @brief             : c file																	*/
/** @description       : ��ӡ�߼�																*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 

#ifndef __Log_Print_H
#define __Log_Print_H

#include "System_Config.h"

#include "stm32f10x.h"

#include "Log_Sensor.h"

#include "Mcu_Config.h"




//�ж��Ƿ�ʹ�ܴ�ӡ����
#if (Print_Function_Enable == 0x01)
//ʹ�ܴ�ӡ����

	/* ������ͨ���������Ϊ4�� */
	#define Print_Sensor_CH_Num_Max 	(0x04)

	#if (Print_Sensor_CH_Num > Print_Sensor_CH_Num_Max)
	#error	"���õĴ�������������4��������ͨ����"
	#endif

	#if (Print_Sensor_CH_Num > Sensor_CH_Config_Num)
	#error	"���õĴ������������ڴ�����ͨ������������"
	#endif


	//�ж����õĴ�ӡ��ͨ������ �Ƿ����1
	#if (Print_Sensor_CH_Num >= 0x01)
		//�ж� ͨ��һ �������Ƿ�ɴ�ӡ�Ĵ���������
		#if ((Sensor_CH1_Config_Type != Sensor_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_Hum) &&\
			 (Sensor_CH1_Config_Type != Sensor_RF_Card_Single_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_RF_Card_Humiture_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_RF_Card_Humiture_Hum) &&\
			 (Sensor_CH1_Config_Type != Sensor_PT1000) &&\
			 (Sensor_CH1_Config_Type != Sensor_GW_NJSY_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_GW_NJSY_Hum) &&\
			 (Sensor_CH1_Config_Type != Sensor_GW_HZZH_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_GW_HZZH_Hum))
			 
		#error "������ͨ��<1>�����趨Ϊ���ɴ�ӡ�Ĵ��������͡�"
		#endif
	#endif

	//�ж����õĴ�ӡ��ͨ������ �Ƿ����2
	#if (Print_Sensor_CH_Num >= 0x02)
		//�ж� ͨ���� �������Ƿ�ɴ�ӡ�Ĵ���������
		#if ((Sensor_CH2_Config_Type != Sensor_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_Hum) &&\
			 (Sensor_CH2_Config_Type != Sensor_RF_Card_Single_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_RF_Card_Humiture_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_RF_Card_Humiture_Hum) &&\
			 (Sensor_CH2_Config_Type != Sensor_PT1000) &&\
			 (Sensor_CH2_Config_Type != Sensor_GW_NJSY_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_GW_NJSY_Hum) &&\
			 (Sensor_CH2_Config_Type != Sensor_GW_HZZH_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_GW_HZZH_Hum))
		#error "������ͨ��<2>�����趨Ϊ���ɴ�ӡ�Ĵ��������͡�"
		#endif
	#endif

	//�ж����õĴ�ӡ��ͨ������ �Ƿ����3
	#if (Print_Sensor_CH_Num >= 0x03)
		//�ж� ͨ���� �������Ƿ�ɴ�ӡ�Ĵ���������
		#if ((Sensor_CH3_Config_Type != Sensor_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_Hum) &&\
			 (Sensor_CH3_Config_Type != Sensor_RF_Card_Single_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_RF_Card_Humiture_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_RF_Card_Humiture_Hum) &&\
			 (Sensor_CH3_Config_Type != Sensor_PT1000) &&\
			 (Sensor_CH3_Config_Type != Sensor_GW_NJSY_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_GW_NJSY_Hum) &&\
			 (Sensor_CH3_Config_Type != Sensor_GW_HZZH_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_GW_HZZH_Hum))
		#error "������ͨ��<3>�����趨Ϊ���ɴ�ӡ�Ĵ��������͡�"
		#endif
	#endif

	//�ж����õĴ�ӡ��ͨ������ �Ƿ����4
	#if (Print_Sensor_CH_Num == 0x04)
		//�ж� ͨ���� �������Ƿ�ɴ�ӡ�Ĵ���������
		#if ((Sensor_CH4_Config_Type != Sensor_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_Hum) &&\
			 (Sensor_CH4_Config_Type != Sensor_RF_Card_Single_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_RF_Card_Humiture_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_RF_Card_Humiture_Hum) &&\
			 (Sensor_CH4_Config_Type != Sensor_PT1000) &&\
			 (Sensor_CH4_Config_Type != Sensor_GW_NJSY_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_GW_NJSY_Hum) &&\
			 (Sensor_CH4_Config_Type != Sensor_GW_HZZH_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_GW_HZZH_Hum))
		#error "������ͨ��<4>�����趨Ϊ���ɴ�ӡ�Ĵ��������͡�"
		#endif
	#endif

#endif


/************************************************************************************************/


/************************************************************************************************/
/* �������� */
void Print_Index_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
/* ��ӡ������� */
void Print_Data_Output(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf);
/* ��ݴ�ӡ������� */
void Easy_Print_Process_Flow(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf);
/* ��ݴ�ӡ״̬ */
BOOL Easy_Print_Get_State(void);
/* ��ݴ�ӡʣ��ʱ�� */
u32 Easy_Print_Get_Out_Time(void);


#endif  
/******************* (C) COPYRIGHT 2015 HZ Zeda Team ****END OF FILE****/ 



