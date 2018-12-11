#ifndef __Dev_GPS_H
#define	__Dev_GPS_H

#include "stm32f10x.h"


/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>GPSģ�������Ϣѡ��

//	<e>����GPSģ��RMC��Ϣ
#define GPS_CMD_RMC_ON_OFF					(0x01)
//  </e>

//	<e>����GPSģ��GSA��Ϣ
#define GPS_CMD_GSA_ON_OFF					(0x01)
//  </e>

//	<e>����GPSģ��GGA��Ϣ
#define GPS_CMD_GGA_ON_OFF					(0x00)
//  </e>

//	<e>����GPSģ��GLL��Ϣ
#define GPS_CMD_GLL_ON_OFF					(0x00)
//  </e>

//	<e>����GPSģ��GSV��Ϣ
#define GPS_CMD_GSV_ON_OFF					(0x00)
//  </e>

//	<e>����GPSģ��VTG��Ϣ
#define GPS_CMD_VTG_ON_OFF					(0x00)
//  </e>

// </h>
//	<<< end of configuration section >>>




/****************************************************  ��������  ********************************/
void GPS_Dev_Init(void);
void GPS_CMD_Config(void);




#endif /* __Dev_GPS_H */



