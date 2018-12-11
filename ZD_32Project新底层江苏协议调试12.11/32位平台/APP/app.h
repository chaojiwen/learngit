#ifndef _APP_H_    
#define _APP_H_    
#include "stm32f10x.h" 
#include "ucos_ii.h"
#include "System_Config.h"
/************************************************************************************************/
/* Save �¼���־ */
extern OS_FLAG_GRP *Str_Save_Flag;
#if (Firmware_Urerade_Function_Enable == 0x01)
/* �������� �¼���־ */
extern OS_FLAG_GRP *Fwe_Ure_Flag;
#endif
/* GPRS ������� �����������ݰ� �¼���־ */
extern OS_FLAG_GRP *Str_GPRS_Upg_RXC_Data_Flag;
///* RTC8025 WakeUp �¼���־ */
//extern OS_FLAG_GRP *Str_RTC8025_WakeUp_Flag;
/* GSM��Ϣ 	���еĳ��� */
#define GSM_EVENT_Q_Len				((u8)OS_MAX_QS)
/* Beep 		���еĳ��� */
#define Beep_EVENT_Q_Len			((u8)OS_MAX_QS)
/* Light 	���еĳ��� */
#define Light_EVENT_Q_Len			((u8)OS_MAX_QS)
/* ������У׼ͨ���¼� 	���еĳ��� */
#define COM_Sensor_Cal_EVENT_Q_Len	((u8)OS_MAX_QS)
/* ���߿�Ƭ �ź��� */
extern OS_EVENT   *RF_Card_QUE_Sem;
/* ��ӡ���� �ź��� */
extern OS_EVENT   *Print_QUE_Sem;
/* GSM��Ϣ�����¼����ƿ� */
extern OS_EVENT* GSM_EVENT_Str_Q;
/* GSM��Ϣ�����¼����ƿ����� */
extern void    * GSM_EVENT_Grp[GSM_EVENT_Q_Len];
/* ����������¼����ƿ� */
extern OS_EVENT* Beep_EVENT_Str_Q;
/* ����������¼����ƿ����� */
extern void    * Beep_EVENT_Grp[Beep_EVENT_Q_Len];
/* ����LED�¼����ƿ� */
extern OS_EVENT* Light_EVENT_Str_Q;
/* ����LED�¼����ƿ����� */
extern void    * Light_EVENT_Grp[Light_EVENT_Q_Len];
/* ����Comͨ���¼����ƿ� */
extern OS_EVENT* COM_Sensor_Cal_EVENT_Q;
/* ����Comͨ���¼����ƿ����� */
extern void 	* COM_Sensor_Cal_EVENT_Grp[COM_Sensor_Cal_EVENT_Q_Len];
#if (GPS_Config_Enable == 0x01)
/* ����GPS��Ϣ�����¼����ƿ�ָ��  */
extern OS_EVENT *GPS_Box;
#endif
/**************** �û��������� *******************/   
void Task_Start(void *p_arg);  //������������ʼ����
void AppTaskRFCard(void *p_arg);//���߿�Ƭ����
#if (GPS_Config_Enable == 0x01)
void AppTaskGPS(void * p_arg);//GPS���� 
#endif
#if (Gateway_NJSY_Enable == 0x01)
void AppTaskGateway(void * p_arg);/*�Ͼ�ʢ����������*/
#endif
void AppTaskSave       (void *p_arg);//���������ݴ洢����
void AppTaskDis        (void *p_arg);//������ʾ����
void AppTaskKey        (void *p_arg);//��������ɨ������
void AppTaskTime       (void *p_arg);//���Ź�ι������
void AppTaskUpg        (void *p_arg);//�����ֶ�����Զ�̳����������
void AppTaskGSM        (void *p_arg);//GSMģ������ͨ������
void AppTaskState      (void *p_arg);//����״̬��Ϣ��������
void AppTaskADC        (void *p_arg);//ADC�ɼ�������ģ���ѹ��Ϣ����
void AppTaskCOM        (void *p_arg);//�����ù���ͨ�Ŷ�һ����ز���������������
void AppTaskBeep       (void *p_arg);//��������(������)����
void AppTaskLight      (void *p_arg);//�ⱨ��(ָʾ��)����
void AppTaskPrint      (void *p_arg);//��ӡ����ӡ����
void AppTaskEasyPrint  (void *p_arg);//��ӡ��һ����ӡ����
void GSM_Config_Task   (void *p_arg);//GSMģ�������������
#endif  //_APP_H_  
