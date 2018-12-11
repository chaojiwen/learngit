#ifndef __App_GPS_H
#define	__App_GPS_H

#include "stm32f10x.h"
#include "Log_GPS.h"





/************************************************************************************************/
/* GPS定位 应用数据 结构 */
typedef struct
{		
	/* 纬度 */
	float			Lat;
	/* 经度 */
	float			Lon;
	/* 速度 */
	float			Spd;
}INSTRU_GPS_POS_App_DR;

/* GPS定位 应用状态 结构 */
typedef struct
{		
	/* 纬度数据更新状态 */
	BOOL			Lat_Updata_State;
	/* 经度数据更新状态 */
	BOOL			Lon_Updata_State;
	/* 速度数据更新状态 */
	BOOL			Spd_Updata_State;

}INSTRU_GPS_POS_App_SR;


/* GPS定位应用数据寄存器 */
extern INSTRU_GPS_POS_App_DR GPS_POS_App_DR;
/* GPS定位应用状态寄存器 */
extern INSTRU_GPS_POS_App_SR GPS_POS_App_SR;



/************************************************************************************************/



/****************************************************  函数声明  ********************************/
void GPS_App_Get_Data(INSTRU_GPS_POS_Log_SR *state,INSTRU_GPS_POS *data);
void GPS_App_Set_Rece_Timeout_Para(void);

float GPS_App_Send_Lat_Data(void);
float GPS_App_Send_Lon_Data(void);
float GPS_App_Send_Spd_Data(void);






#endif /* __App_GPS_H */






