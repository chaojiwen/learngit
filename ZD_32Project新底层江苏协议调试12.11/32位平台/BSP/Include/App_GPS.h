#ifndef __App_GPS_H
#define	__App_GPS_H

#include "stm32f10x.h"
#include "Log_GPS.h"





/************************************************************************************************/
/* GPS��λ Ӧ������ �ṹ */
typedef struct
{		
	/* γ�� */
	float			Lat;
	/* ���� */
	float			Lon;
	/* �ٶ� */
	float			Spd;
}INSTRU_GPS_POS_App_DR;

/* GPS��λ Ӧ��״̬ �ṹ */
typedef struct
{		
	/* γ�����ݸ���״̬ */
	BOOL			Lat_Updata_State;
	/* �������ݸ���״̬ */
	BOOL			Lon_Updata_State;
	/* �ٶ����ݸ���״̬ */
	BOOL			Spd_Updata_State;

}INSTRU_GPS_POS_App_SR;


/* GPS��λӦ�����ݼĴ��� */
extern INSTRU_GPS_POS_App_DR GPS_POS_App_DR;
/* GPS��λӦ��״̬�Ĵ��� */
extern INSTRU_GPS_POS_App_SR GPS_POS_App_SR;



/************************************************************************************************/



/****************************************************  ��������  ********************************/
void GPS_App_Get_Data(INSTRU_GPS_POS_Log_SR *state,INSTRU_GPS_POS *data);
void GPS_App_Set_Rece_Timeout_Para(void);

float GPS_App_Send_Lat_Data(void);
float GPS_App_Send_Lon_Data(void);
float GPS_App_Send_Spd_Data(void);






#endif /* __App_GPS_H */






