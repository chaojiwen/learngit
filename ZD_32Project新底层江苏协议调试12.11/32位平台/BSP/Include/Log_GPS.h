#ifndef __Log_GPS_H
#define	__Log_GPS_H

#include "stm32f10x.h"



/************************************************************************************************/
/* GPS��λ ���ݽṹ */
typedef struct
{		
	/* γ�� */
	float			Lat;
	/* ���� */
	float			Lon;
	/* �ű��� */
	float			Alt;
	/* �ٶ� */
	float			Spd;
	/* ��λ */
	float			Cog;
	/* ˮƽ����  */
	float			HDOP;
	/* UTCʱ�� */
	unsigned char	UTC[6];
	/* ��λ״̬ */
	BOOL			Pos_State;
}INSTRU_GPS_POS;

/* GPS��λ Ӧ��״̬ �ṹ */
typedef struct
{		
	/* ��Ϣ����״̬ */
	BOOL			Ana_State;
	/* ���ݽ��ճ�ʱ�Ĵ��� */
	u16				Rece_Timeout_Reg;
}INSTRU_GPS_POS_Log_SR;



/************************************************************************************************/

/* GPS��λ ���ݼĴ��� */
extern INSTRU_GPS_POS 	GPS_POS_Inf;

extern INSTRU_GPS_POS_Log_SR GPS_POS_Log_SR;


/****************************************************  ��������  ********************************/
void GPS_Log_Init(void);
BOOL GPS_Log_Ana_Inf(void);

void GPS_Log_Rece_Data_State_Check(void);
void GPS_Log_RESET_POS_Inf(void);
void GPS_Log_Set_Rece_Timeout_Para(void);





#endif /* __Log_GPS_H */



