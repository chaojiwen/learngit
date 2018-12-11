/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/10/24																			 */
/*************************************************************************************************/
#include "Log_Protocol_ Analysis.h"

#include "common.h"
#include "Ks0108.h"	 

#include "Mcu_Beep.h"
#include "Mcu_led.h"

#include "Log_GSM.h"

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �������ݻ��棬�ж����ñ��Ƿ����޸ģ����޸ĵĻ��������ñ���Ķ�Ӧ����
*	���ݲ�����*buf���������黺�棬�ǿ�(������Ϣ)
					  len�����黺�� ����
						* Config_Content�����ñ� ��Ϣ �ṹ��
						Packet_num�����黺���� ������������!
						* tab�����ݻ����޸ĵ����ݣ��޸��ڴ��� �������ñ�
*	�� �� ֵ: �� __nop(); 
*********************************************************************************************************
*/
void RECBUF_config_change_Deal(char *buf,int len,CONFIG_TAB_CHANGE* Config_Content[],u16 Packet_num,INSTRU_CONFIG_INF_TAB* TAB)
{	
  u32 data_Length=0;
	u16 j=0,i=0;
	
	//�����õ������� Ҫ������������������⣡
	for(i=0;i<Packet_num;i++)//������ �м�������(С������) ������
	{
		Config_Content[i]->Change_Type     = buf[0+data_Length];//����
		Config_Content[i]->Change_Adder    = buf[1+data_Length];//��ַ
		Config_Content[i]->Change_Data_Len = buf[2+data_Length];//���ݳ���(С��)
		
		data_Length=data_Length+3;//���� ��ַ ���ݳ��� 3�ֽ�	
		
		if(0x01==Config_Content[i]->Change_Type)//ϵͳ��Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_System_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }
			data_Length=data_Length+j;
		}
		else if(0x02==Config_Content[i]->Change_Type)//��Դ״̬��Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Power_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }	  
			data_Length=data_Length+j;			
		}
		else if(0x03==Config_Content[i]->Change_Type)//ϵͳʱ����Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_RTC_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		else if(0x04==Config_Content[i]->Change_Type)//GSMģ����Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_GSM_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }			
			data_Length=data_Length+j;			
		}
		else if(0x05==Config_Content[i]->Change_Type)//��������Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Sensor_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		else if(0x06==Config_Content[i]->Change_Type)//�˵�������Ϣ,�������ܿ�����Ϣ��ַ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Alarm_Master_Switch_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		else if(0x07==Config_Content[i]->Change_Type)//������У׼���ݱ���Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				(*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Cal_adder+Config_Content[i]->Change_Adder+j))) = buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		
	}
  
}
/*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �������ñ���Ϣ�ṹ��(���������)���ж����ñ��Ƿ����޸ģ����޸ĵĻ���ȡTAB��������ݣ�����ֵ��BUF
*	���ݲ�����*buf���ջ��棬�洢������Ϣ������+��ַ+���ݳ���+��Ӧ����������
					  len��buf[] ���黺�� �ܳ���
						* Config_Content�����ñ� ��Ϣ �ṹ��(���������)
						Packet_num�����黺���� ������������!
						* tab���������ñ�
*	�� �� ֵ: �� __nop(); 
*********************************************************************************************************
*/
void RECTAB_config_change_Deal(char *buf,int len,CONFIG_TAB_CHANGE* Config_Content[],u16 Packet_num,INSTRU_CONFIG_INF_TAB* TAB)
{
	//len ��� 1024
	u32 data_Length=0;
	u16 j=0,i=0;
	
	//�����õ������� Ҫ������������������⣡
	for(i=0;i<Packet_num;i++)//������ �м�������(С������) ������
	{
		buf[0+data_Length] = Config_Content[i]->Change_Type;//����
		buf[1+data_Length] = Config_Content[i]->Change_Adder;//��ַ
		buf[2+data_Length] = Config_Content[i]->Change_Data_Len;//���ݳ���(С��)
		
		data_Length=data_Length+3;//���� ��ַ ���ݳ��� 3�ֽ�	
		
		if(0x01==Config_Content[i]->Change_Type)//ϵͳ��Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_System_adder+Config_Content[i]->Change_Adder+j)));
      }
			data_Length=data_Length+j;
		}
		else if(0x02==Config_Content[i]->Change_Type)//��Դ״̬��Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Power_adder+Config_Content[i]->Change_Adder+j)));
      }	  
			data_Length=data_Length+j;			
		}
		else if(0x03==Config_Content[i]->Change_Type)//ϵͳʱ����Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_RTC_adder+Config_Content[i]->Change_Adder+j)));
      }				
			data_Length=data_Length+j;
		}
		else if(0x04==Config_Content[i]->Change_Type)//GSMģ����Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_GSM_adder+Config_Content[i]->Change_Adder+j)));
      }			
			data_Length=data_Length+j;			
		}
		else if(0x05==Config_Content[i]->Change_Type)//��������Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Sensor_adder+Config_Content[i]->Change_Adder+j)));
      }				
			data_Length=data_Length+j;
		}
		else if(0x06==Config_Content[i]->Change_Type)//�˵�������Ϣ,�������ܿ�����Ϣ��ַ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Alarm_Master_Switch_adder+Config_Content[i]->Change_Adder+j)));
      }				
			data_Length=data_Length+j;
		}
		else if(0x07==Config_Content[i]->Change_Type)//������У׼���ݱ���Ϣ
		{
			for(j=0;j<(Config_Content[i]->Change_Data_Len);j++)
			{
				buf[data_Length+j] = (*((u32*)(TAB->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number_Code.Code_1+
				Instru_Inf_Tab_Mem_Cal_adder+Config_Content[i]->Change_Adder+j)));
      }				
			data_Length=data_Length+j;
		}
	}
	
}



