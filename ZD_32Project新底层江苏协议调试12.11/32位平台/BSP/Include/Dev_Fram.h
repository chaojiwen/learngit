#ifndef __Dev_Fram_H
#define __Dev_Fram_H

#include "stm32f10x.h"

#include "Log_Save.h"


/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>Fram������صĺ�����
//	<e>Fram�����趨ʹ��
#define Fram_Config_Enable			(0x01)

//	<e>Framҳ��С
//  <o.0..8>Framҳ��С����			<0x100-0x200:0x100>
//  <i>����ֻ��ʾ���Եĸ�������,����ֻ��256�ֽڴ�С��ҳ.
#define Fram_Page_Size				(0x0100)
//  </e>

//	<e>Fram������С			
//  <o.0..13>Fram������С����		
//  <0x800=> 2K Bytes
//  <0x1000=> 4K Bytes
//  <0x2000=> 8K Bytes
//  <i>������2K��4K��8K��3��������ѡ.
#define Fram_Capacity_Size			(0x1000)
//  </e>

//  </e>
// </h>
//	<<< end of configuration section >>>





/* Fram Slave ID */
#define Fram_Slave_ID 0xA0 



/******************** �������� **********************/
void Sve_I2C_Test(void);
/* д ���� ������Ϣ�� */
void Write_Config_Tab(u8* buffer_p,u16 count);
/* �� ���� ������Ϣ�� */
void Read_Config_Tab (u8* buffer_p,u16 count);

#endif /* __Fram_Model_H */




