/************************************************************************************************/
/** @file              : Dev_Flash.h															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��7��25��															*/
/** @brief             : c file																	*/
/** @description       : FLASH�������루����8Mflash32ͨ��1���Ӵ洢һ�����ݣ��ܹ���85.2�����ݣ�		*/
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
//	<h>Flash������صĺ�����
//	<e>Flash�����趨ʹ��
#define Flash_Config_Enable			(0x01)

//	<e>Flashҳ��С
//  <o.0..8>Flashҳ��С����			<0x100-0x200:0x100>
//  <i>����ֻ��ʾ���Եĸ�������,����ֻ��256�ֽڴ�С��ҳ.
#define Flash_Page_Size				(0x0100)
//  </e>

//	<e>Flash������С
//  <o.0..13>Flash������С����		<0x1000-0x2000:0x1000>
//  <i>����ֻ��ʾ���Եĸ�������,����ֻ��4096�ֽڴ�С������.
#define Flash_Sector_Size			(0x1000)
//  </e>

//	<e>Flash������С			
//  <o.0..23>Flash������С����		
//  <0x200000=> 2M Bytes
//  <0x400000=> 4M Bytes
//  <0x800000=> 8M Bytes
//  <0x5000=> 20K Bytes(����)
//  <i>������2M��4M��8M��3��������ѡ.(���������������СҪ����������������)
#define Flash_Capacity_Size			(0x200000)
//  </e>

//  </e>
//  </h>
//	<<< end of configuration section >>>



/* Flash ���ݶ�д�����С */
#define Flash_Buffer_Size  	(0X100)
///* Flash ��д���ݻ��� */
//extern  u8 Flash_Buffer[Flash_Buffer_Size];

#if (Flash_Capacity_Size < Flash_Sector_Size*5)
#error "Flash���õĴ洢�ռ�С��4��������С���ᵼ�´�ӡ���ݶ�ʧ��"
#endif

/* ������õ�оƬ������С�Ƿ�Ϊ������������ */
#if ((Flash_Capacity_Size % Flash_Sector_Size) > 0)
#error "Flash���õ�оƬ������С����Ϊ�������������������ᵼ�´�ӡ���ݳ���"
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

