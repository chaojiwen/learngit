/************************************************************************************************/
/** @file              : Dev_Flash.h															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年7月25日															*/
/** @brief             : c file																	*/
/** @description       : FLASH驱动代码（按照8Mflash32通道1分钟存储一条数据，能够存85.2天数据）		*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#ifndef __Dev_Flash_H
#define __Dev_Flash_H

#include "stm32f10x.h"
#include "Log_Save.h"

/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>Flash配置相关的宏设置
//	<e>Flash配置设定使能
#define Flash_Config_Enable			(0x01)

//	<e>Flash页大小
//  <o.0..8>Flash页大小设置			<0x100-0x200:0x100>
//  <i>这里只是示范性的给出两种,现在只有256字节大小的页.
#define Flash_Page_Size				(0x0100)
//  </e>

//	<e>Flash扇区大小
//  <o.0..13>Flash扇区大小设置		<0x1000-0x2000:0x1000>
//  <i>这里只是示范性的给出两种,现在只有4096字节大小的扇区.
#define Flash_Sector_Size			(0x1000)
//  </e>

//	<e>Flash容量大小			
//  <o.0..23>Flash容量大小设置		
//  <0x200000=> 2M Bytes
//  <0x400000=> 4M Bytes
//  <0x800000=> 8M Bytes
//  <0x5000=> 20K Bytes(测试)
//  <i>这里有2M、4M、8M，3种容量可选.(并且这里的容量大小要是扇区的整数倍。)
#define Flash_Capacity_Size			(0x200000)
//  </e>

//  </e>
//  </h>
//	<<< end of configuration section >>>



/* Flash 数据读写缓存大小 */
#define Flash_Buffer_Size  	(0X100)
///* Flash 读写数据缓存 */
//extern  u8 Flash_Buffer[Flash_Buffer_Size];

#if (Flash_Capacity_Size < Flash_Sector_Size*5)
#error "Flash设置的存储空间小于4个扇区大小，会导致打印数据丢失。"
#endif

/* 检测设置的芯片容量大小是否为扇区的整数倍 */
#if ((Flash_Capacity_Size % Flash_Sector_Size) > 0)
#error "Flash设置的芯片容量大小不是为扇区的整数倍，这样会导致打印数据出错。"
#endif


/* Flash Model */
#define Flash_Model_W25X16



void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

u8*  Flash_Read_Sensor_Data(u32 adder,u32 count);
void Flash_Write_Sensor_Data(u8* data_p,u32 adder,u32 count);

#endif /* __SPI_FLASH_H */

