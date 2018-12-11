#ifndef __Log_Download_H_
#define __Log_Download_H_

#include "stm32f10x.h"

/************************************************************************************************/
/* IAP程序升级 数据缓存结构（16字节 数据信息） */
typedef struct {
	/* 头 标识位 */		//(4 Byte:正常是4个#)
	u8	Block_Iden[4];	
	/* 数据块 总数量 */	//现在一个块是（1040字节，有16字节信息+1k数据）
	u8	Block_Total;
	/* 保留 */			//还没使用	
	u8  Reserve_0;
	/* 数据块 序号 */	//也就是说最大也就只能存放255K数据块
	u8  Block_No;
	/* 保留 */			//还没使用		
	u8  Reserve_1;
	/* 数据块 长度 */	//固定1040字节
	u16 Block_Len;
	/* 数据块 校验 */	//考虑，使用CRC16
	u16 Block_Check;	
	/* 保留 */			//还没使用		
	u32  Reserve_2;
}IAP_Data_Inf_Structure;

/* IAP程序升级 控制寄存器 */
typedef struct {
	
	/* 读取 GPRS 数据 失败 次数 */
	u8 Data_Download_fail_Num;
	
}IAP_CR;

/* IAP程序升级 信息结构 */
typedef struct {
	//IAP_SR
	/* 数据 读取 状态 */
	BOOL RX_COM_State;
	//IAP_CR
	IAP_CR	CR; 
	//IAP_DR
	/* 数据缓存 指针 */
	char* 	Data_Buffer;
	/* 数据缓存 大小 */	
	int 	Data_Buffer_Size;
	/* 写指针 */
	u32		Write_Pointer;
	/* 读指针 */
	u32		Read_Pointer;
	/* 数据空间 大小(Flash的空间) *///有250K（1代表1K）
	u32		Space_Size;	
	/* 数据块 总数 */
	int 	Data_Block_Total;
	/* 当前数据块 编号 */
	int 	Data_Block_No;
	/* 校验 */
	u32 	Data_Block_Check;
#if 0	
	u16 	Page_Size;
	u16 	Page_NUM;
#endif
}IAP_SR_Structure;
/************************************************************************************************/
/* IAP 数据页大小 */
#define IAP_Page_Inf_Size				(16u)
#define IAP_Page_Data_Size				(1024u)
#define IAP_PageSize					(IAP_Page_Inf_Size+IAP_Page_Data_Size)
/* MCU 在IAP程序升级阶段 存放数据的flash数据 */
//STM32F103VE的页大小为2K，但是，考虑GPRS传输数据的稳定性，
//暂时先将数据页设置成512字节，每次往flash中写512字节数据。
//extern char IAP_Data_Buffer[IAP_PageSize];		
//extern IAP_SR_Structure Mcu_IAP_Flash_Data;


/************************************************************************************************/
/************************ 函数声明 **************************/
BOOL Remote_Upgrade		(void);
void Flash_Write_SN_NO	(unsigned char* sn_ptr);
void Flash_Read_SN_NO	(unsigned char* sn_ptr);
void Flash_Write_Pgm_Ude_State	(unsigned char* state_ptr);
void Flash_Read_Pgm_Ude_State	(unsigned char* sn_ptr);





#endif /* Log_Download_H */

