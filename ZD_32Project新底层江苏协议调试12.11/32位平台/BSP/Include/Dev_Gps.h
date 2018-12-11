#ifndef __Dev_GPS_H
#define	__Dev_GPS_H

#include "stm32f10x.h"


/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>GPS模块输出消息选择

//	<e>开关GPS模块RMC消息
#define GPS_CMD_RMC_ON_OFF					(0x01)
//  </e>

//	<e>开关GPS模块GSA消息
#define GPS_CMD_GSA_ON_OFF					(0x01)
//  </e>

//	<e>开关GPS模块GGA消息
#define GPS_CMD_GGA_ON_OFF					(0x00)
//  </e>

//	<e>开关GPS模块GLL消息
#define GPS_CMD_GLL_ON_OFF					(0x00)
//  </e>

//	<e>开关GPS模块GSV消息
#define GPS_CMD_GSV_ON_OFF					(0x00)
//  </e>

//	<e>开关GPS模块VTG消息
#define GPS_CMD_VTG_ON_OFF					(0x00)
//  </e>

// </h>
//	<<< end of configuration section >>>




/****************************************************  函数声明  ********************************/
void GPS_Dev_Init(void);
void GPS_CMD_Config(void);




#endif /* __Dev_GPS_H */



