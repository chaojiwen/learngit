#include "interface.h"
#include "BSP.h"
#include "app.h"

#include "Ks0108.h"
#include "Log_GSM.h"

/* GPRS �������ݻ��� */
static u8 GPRS_Send_Sensor_Data_Buffer1[1500];


/* GPRS �������ݻ��� */
static u8 GPRS_Rece_Sensor_Data_Buffer1[1500];

/* GPRS ������ʪ��������Ϣ ���ݼĴ��� */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_Sensor_Data_Reg = 
{
	/* GPRS ���� ������ �������� */
	0,
	/* GPRS ���� ������ �������� */
	0,
	/* GPRS ���ʹ���������ͨ��״̬��FALSE:û�з��ͣ�*/
	FALSE,
	/* GPRS �����򴫸������ݵȴ�����״̬��FALSE:û�����ݴ��� */
	FALSE,
	/* GPRS �����򴫸������ݵȴ�����״̬��FALSE:û�����ݴ��� */
	FALSE,
	/* GPRS ���ʹ��������ݽ�� */
	TRUE,
	/* GPRS ���մ��������ݽ�� */
	TRUE,
	/* GPRS ��������״̬ */
	TRUE,	
	/* GPRS ���ʹ��������� ʧ�ܴ��� */
	0,
};


/*
***�����������Զ������ָ��



*/
void Remote_Upgrade_Program(void)
{
    gsm_gprs_comm(
	  (char*)GPRS_Send_Sensor_Data_Buffer1,
		(1500),
	  (char*)GPRS_Rece_Sensor_Data_Buffer1,
	  ((int*)(&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)));
		
}
