/*************************************************************************************************/
/*************************************     32λƽ̨����     ***************************************/
/* �������ڣ�2015/1/13										 									 */
/*************************************************************************************************/
#include "Log_INF.h"

#include "Dev_Fram.h"

#include <stdio.h>
#include <string.h>

/*
*********************************************************************************************************
*	�� �� ��: Instru_Inf_State_Tab_State_Check
*	����˵��: ��� ���� ������Ϣ��
*	��    �Σ���
*	�� �� ֵ: ��
*	˵��	������ϵͳ�м��100MS���һ�Ρ�
*********************************************************************************************************
*/
void Instru_Inf_State_Tab_State_Check(INSTRU_CONFIG_INF_TAB* Instru_data_update)
{
//	
//	/* �ж� ϵͳ ״̬��Ϣ ���ݸ��� */
//	if(Instru_data_update->Inf_State.Instru_System.Inf.Data_Update_Flag)
//	{
//		/* ��� ���ݸ��±�־ */
//		Instru_data_update->Inf_State.Instru_System.Inf.Data_Update_Flag = 0;
//		/* ϵͳ ״̬��Ϣ 				�� �������� */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_System.Inf_Space,
//						  Instru_Inf_Tab_Mem_System_adder,
//						  Instru_System_Inf_Space_Size);
//		
//	}else if(Instru_Inf_State_Tab.Inf_State.Instru_Power.Inf_State.Data_Update_Flag)
//	{
//		/* ��� ���ݸ��±�־ */
//		Instru_data_update->Inf_State.Instru_Power.Inf_State.Data_Update_Flag = 0;
//		/* ��Դ ״̬��Ϣ 				�� �������� */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Power.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Power_adder,
//						  Instru_Power_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_Fram.Inf_State.Data_Update_Flag)
//	{
//		/* ��� ���ݸ��±�־ */
//		Instru_data_update->Inf_State.Instru_Fram.Inf_State.Data_Update_Flag = 0;
//		/* �ⲿ�洢�� FRAM ״̬��Ϣ 	�� �������� */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Fram.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Fram_adder,
//						  Instru_Fram_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_Flash.Inf_State.Data_Update_Flag)
//	{
//		/* ��� ���ݸ��±�־ */
//		Instru_data_update->Inf_State.Instru_Flash.Inf_State.Data_Update_Flag = 0;
//		/* �ⲿ�洢�� Flash ״̬��Ϣ 	�� �������� */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Flash.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Flash_adder,
//						  Instru_Flash_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_Display_Device.Inf_State.Data_Update_Flag)
//	{
//		/* ��� ���ݸ��±�־ */	
//		Instru_data_update->Inf_State.Instru_Display_Device.Inf_State.Data_Update_Flag = 0;
//		/* ��ʾ�豸 ״̬ ��Ϣ 		�� �������� */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Display_Device.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Disp_Dev_adder,
//						  Instru_Disp_Dev_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_RTC.Inf_State.Data_Update_Flag)
//	{
//		/* ��� ���ݸ��±�־ */
//		Instru_data_update->Inf_State.Instru_RTC.Inf_State.Data_Update_Flag = 0;
//		/* ʵʱʱ�� ״̬ ��Ϣ 		�� �������� */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_RTC.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_RTC_adder,
//						  Instru_RTC_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_GSM.Inf_State.Data_Update_Flag)
//	{
//		/* ��� ���ݸ��±�־ */
//		Instru_data_update->Inf_State.Instru_GSM.Inf_State.Data_Update_Flag = 0;
//		/* GSM ״̬ ��Ϣ 			�� �������� */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_GSM.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_GSM_adder,
//						  Instru_GSM_Inf_State_Space_Size);
//	}else if(Instru_data_update->Inf_State.Instru_Sensor.Inf_State.Data_Update_Flag)
//	{
//		/* ��� ���ݸ��±�־ */
//		Instru_data_update->Inf_State.Instru_Sensor.Inf_State.Data_Update_Flag = 0;
//		/* ������ ״̬ ��Ϣ 			�� �������� */
//		I2C_Write_INF_TAB(Instru_data_update->Inf_State.Instru_Sensor.Inf_State_Space,
//						  Instru_Inf_Tab_Mem_Sensor_adder,
//						  Instru_Sensor_Inf_State_Space_Size);
//	}
//	
//	

}

///*************************************************************************************************/
///* ��������Instru_Inf_State_Tab_Config															 */
///* ����  ������ ��Ϣ�� ����																	 	 */
///* ����  ����																					 */
///* ����  ����																					 */
///* ע��  ��																						 */
///*************************************************************************************************/
//void Instru_Inf_State_Tab_Config(void)
//{
//	/* �ж� FRAM ����״̬ */
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram.Inf_State.Chip_Work_State == 0)
//	{
////		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
////		{
////			/* FRAM ���������� */
////			printf("\rFRAM���в���������.\r\n");
////		}		
//		/* ֹͣ�������� */		
//		while(1);
//	}else if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Fram.Inf_State.Chip_Work_State == 1)
//	{
//		/* FRAM �������� */
//		/* ��ȡ ���� ��Ϣ�� ���ñ�־ */
////		Read_Config_Tab(&Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag,sizeof(Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag));
//		/* �ж� ���� ��Ϣ�� ���ñ�־ */
//		if(Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag == 1)
//		{
//			/* ���� ��Ϣ�� �Ѿ����ù� */
//			/* ���ⲿ�Ĵ洢���л�ȡ ���� ��Ϣ��. */
//			Read_Config_Tab(Instru_Config_Inf_Tab.Inf_State_Space,sizeof(INSTRU_CONFIG_INF_TAB));
//		}else
//		{
////			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
////			{
////				/* ���� ��Ϣ�� δ���ù� */
////				printf("\r������Ϣ��δ���ù�.\r\n");				
////			}
//			/* ʹ�� Ĭ�ϵ� ������Ϣ */
//			/* ���� ���� ��Ϣ�� ��־ */
//			Instru_Config_Inf_Tab = Default_Instru_Config_Inf_State_Tab;
////			/* ���� SN�� */
////			memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf_Space,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),ENV_SN_NO_SIZE);
//			/* д ���� ��Ϣ�� ���ñ�־ */
//			Write_Config_Tab(Instru_Config_Inf_Tab.Inf_State_Space,sizeof(INSTRU_CONFIG_INF_TAB));
//		}
//		
//		/* ��ȡ ���� �̶���Ϣ�� ���ñ�־ */
////		Read_Config_Tab(&Instru_Fix_Inf_State_Tab.Inf_State.Data_Update_Flag,sizeof(Instru_Config_Inf_Tab.Inf_State.Data_Update_Flag));
//		/* �ж� ���� �̶���Ϣ�� ���ñ�־ */
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Data_Update_Flag == 1)
//		{
//			/* ���� �̶���Ϣ�� �Ѿ����ù� */
//			/* ���ⲿ�Ĵ洢���л�ȡ ���� ��Ϣ��. */
//			Read_Config_Tab(Instru_Fix_Inf_State_Tab.Inf_State_Space,sizeof(INSTRU_FIX_INF_TAB));
//		}else
//		{
////			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
////			{
////				/* ���� ��Ϣ�� δ���ù� */
////				printf("\r������Ϣ��δ���ù�.\r\n");
////			}	
//			/* ʹ�� Ĭ�ϵ� ������Ϣ */
//			/* ���� ���� ��Ϣ�� ��־ */
//			Instru_Fix_Inf_State_Tab = Default_Instru_Fix_Inf_State_Tab;
//			/* д ���� ��Ϣ�� ���ñ�־ */
//			Write_Config_Tab(Instru_Fix_Inf_State_Tab.Inf_State_Space,sizeof(INSTRU_FIX_INF_TAB));
//		}
//		
//	}else
//	{
//		/* ֹͣ�������� *///��������²������е��ĵط���		
//		while(1);
//	}
//}






/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

