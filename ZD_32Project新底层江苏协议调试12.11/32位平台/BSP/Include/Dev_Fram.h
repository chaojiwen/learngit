#ifndef __Dev_Fram_H
#define __Dev_Fram_H

#include "stm32f10x.h"

#include "Log_Save.h"


/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>Fram配置相关的宏设置
//	<e>Fram配置设定使能
#define Fram_Config_Enable			(0x01)

//	<e>Fram页大小
//  <o.0..8>Fram页大小设置			<0x100-0x200:0x100>
//  <i>这里只是示范性的给出两种,现在只有256字节大小的页.
#define Fram_Page_Size				(0x0100)
//  </e>

//	<e>Fram容量大小			
//  <o.0..13>Fram容量大小设置		
//  <0x800=> 2K Bytes
//  <0x1000=> 4K Bytes
//  <0x2000=> 8K Bytes
//  <i>这里有2K、4K、8K，3种容量可选.
#define Fram_Capacity_Size			(0x1000)
//  </e>

//  </e>
// </h>
//	<<< end of configuration section >>>





/* Fram Slave ID */
#define Fram_Slave_ID 0xA0 



/******************** 函数声明 **********************/
void Sve_I2C_Test(void);
/* 写 仪器 配置信息表 */
void Write_Config_Tab(u8* buffer_p,u16 count);
/* 读 仪器 配置信息表 */
void Read_Config_Tab (u8* buffer_p,u16 count);

#endif /* __Fram_Model_H */




