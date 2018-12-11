#ifndef __Log_Download_H_
#define __Log_Download_H_

#include "stm32f10x.h"

/************************************************************************************************/
/* IAP�������� ���ݻ���ṹ��16�ֽ� ������Ϣ�� */
typedef struct {
	/* ͷ ��ʶλ */		//(4 Byte:������4��#)
	u8	Block_Iden[4];	
	/* ���ݿ� ������ */	//����һ�����ǣ�1040�ֽڣ���16�ֽ���Ϣ+1k���ݣ�
	u8	Block_Total;
	/* ���� */			//��ûʹ��	
	u8  Reserve_0;
	/* ���ݿ� ��� */	//Ҳ����˵���Ҳ��ֻ�ܴ��255K���ݿ�
	u8  Block_No;
	/* ���� */			//��ûʹ��		
	u8  Reserve_1;
	/* ���ݿ� ���� */	//�̶�1040�ֽ�
	u16 Block_Len;
	/* ���ݿ� У�� */	//���ǣ�ʹ��CRC16
	u16 Block_Check;	
	/* ���� */			//��ûʹ��		
	u32  Reserve_2;
}IAP_Data_Inf_Structure;

/* IAP�������� ���ƼĴ��� */
typedef struct {
	
	/* ��ȡ GPRS ���� ʧ�� ���� */
	u8 Data_Download_fail_Num;
	
}IAP_CR;

/* IAP�������� ��Ϣ�ṹ */
typedef struct {
	//IAP_SR
	/* ���� ��ȡ ״̬ */
	BOOL RX_COM_State;
	//IAP_CR
	IAP_CR	CR; 
	//IAP_DR
	/* ���ݻ��� ָ�� */
	char* 	Data_Buffer;
	/* ���ݻ��� ��С */	
	int 	Data_Buffer_Size;
	/* дָ�� */
	u32		Write_Pointer;
	/* ��ָ�� */
	u32		Read_Pointer;
	/* ���ݿռ� ��С(Flash�Ŀռ�) *///��250K��1����1K��
	u32		Space_Size;	
	/* ���ݿ� ���� */
	int 	Data_Block_Total;
	/* ��ǰ���ݿ� ��� */
	int 	Data_Block_No;
	/* У�� */
	u32 	Data_Block_Check;
#if 0	
	u16 	Page_Size;
	u16 	Page_NUM;
#endif
}IAP_SR_Structure;
/************************************************************************************************/
/* IAP ����ҳ��С */
#define IAP_Page_Inf_Size				(16u)
#define IAP_Page_Data_Size				(1024u)
#define IAP_PageSize					(IAP_Page_Inf_Size+IAP_Page_Data_Size)
/* MCU ��IAP���������׶� ������ݵ�flash���� */
//STM32F103VE��ҳ��СΪ2K�����ǣ�����GPRS�������ݵ��ȶ��ԣ�
//��ʱ�Ƚ�����ҳ���ó�512�ֽڣ�ÿ����flash��д512�ֽ����ݡ�
//extern char IAP_Data_Buffer[IAP_PageSize];		
//extern IAP_SR_Structure Mcu_IAP_Flash_Data;


/************************************************************************************************/
/************************ �������� **************************/
BOOL Remote_Upgrade		(void);
void Flash_Write_SN_NO	(unsigned char* sn_ptr);
void Flash_Read_SN_NO	(unsigned char* sn_ptr);
void Flash_Write_Pgm_Ude_State	(unsigned char* state_ptr);
void Flash_Read_Pgm_Ude_State	(unsigned char* sn_ptr);





#endif /* Log_Download_H */

