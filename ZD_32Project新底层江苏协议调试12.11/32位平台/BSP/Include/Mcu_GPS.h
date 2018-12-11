#ifndef __Mcu_GPS_H
#define	__Mcu_GPS_H

#include "stm32f10x.h"



/************************************************************************************************/
/* GPRMC 消息 接收状态 */
extern BOOL	GPSRMC_Data_Rec_State;
/* GPGSA 消息 接收状态 */
extern BOOL	GPSGSA_Data_Rec_State;


/* GPRMC消息 的数据缓存大小 */
#define 	GPRMC_MSG_Buf_Size	(76u)
/* GPGSA消息 的数据缓存大小 */
#define 	GPGSA_MSG_Buf_Size	(70u)


/* GPRMC消息，包含经度、纬度、速度、海拔等，实际最大长度约为71字节 */
extern char	GPRMC_MSG_Buf[GPRMC_MSG_Buf_Size+1];
/* GPGSA消息，包含三种DOP信息、定位模式、用于定位的卫星编号(最多12颗)等，实际最大长度为64字节 */
extern char	GPGSA_MSG_Buf[GPGSA_MSG_Buf_Size+1];



/****************************************************  函数声明  ********************************/

void GPS_Mcu_Init(void);
void GPS_Rec_Data_Inf_Flow(char data);




#endif /* __GPS_H */


