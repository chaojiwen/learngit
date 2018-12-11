#ifndef __Mcu_GPS_H
#define	__Mcu_GPS_H

#include "stm32f10x.h"



/************************************************************************************************/
/* GPRMC ��Ϣ ����״̬ */
extern BOOL	GPSRMC_Data_Rec_State;
/* GPGSA ��Ϣ ����״̬ */
extern BOOL	GPSGSA_Data_Rec_State;


/* GPRMC��Ϣ �����ݻ����С */
#define 	GPRMC_MSG_Buf_Size	(76u)
/* GPGSA��Ϣ �����ݻ����С */
#define 	GPGSA_MSG_Buf_Size	(70u)


/* GPRMC��Ϣ���������ȡ�γ�ȡ��ٶȡ����εȣ�ʵ����󳤶�ԼΪ71�ֽ� */
extern char	GPRMC_MSG_Buf[GPRMC_MSG_Buf_Size+1];
/* GPGSA��Ϣ����������DOP��Ϣ����λģʽ�����ڶ�λ�����Ǳ��(���12��)�ȣ�ʵ����󳤶�Ϊ64�ֽ� */
extern char	GPGSA_MSG_Buf[GPGSA_MSG_Buf_Size+1];



/****************************************************  ��������  ********************************/

void GPS_Mcu_Init(void);
void GPS_Rec_Data_Inf_Flow(char data);




#endif /* __GPS_H */


