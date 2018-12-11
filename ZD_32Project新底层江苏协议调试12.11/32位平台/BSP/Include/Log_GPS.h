#ifndef __Log_GPS_H
#define	__Log_GPS_H

#include "stm32f10x.h"



/************************************************************************************************/
/* GPS定位 数据结构 */
typedef struct
{		
	/* 纬度 */
	float			Lat;
	/* 经度 */
	float			Lon;
	/* 磁变量 */
	float			Alt;
	/* 速度 */
	float			Spd;
	/* 方位 */
	float			Cog;
	/* 水平精度  */
	float			HDOP;
	/* UTC时间 */
	unsigned char	UTC[6];
	/* 定位状态 */
	BOOL			Pos_State;
}INSTRU_GPS_POS;

/* GPS定位 应用状态 结构 */
typedef struct
{		
	/* 信息解析状态 */
	BOOL			Ana_State;
	/* 数据接收超时寄存器 */
	u16				Rece_Timeout_Reg;
}INSTRU_GPS_POS_Log_SR;



/************************************************************************************************/

/* GPS定位 数据寄存器 */
extern INSTRU_GPS_POS 	GPS_POS_Inf;

extern INSTRU_GPS_POS_Log_SR GPS_POS_Log_SR;


/****************************************************  函数声明  ********************************/
void GPS_Log_Init(void);
BOOL GPS_Log_Ana_Inf(void);

void GPS_Log_Rece_Data_State_Check(void);
void GPS_Log_RESET_POS_Inf(void);
void GPS_Log_Set_Rece_Timeout_Para(void);





#endif /* __Log_GPS_H */



