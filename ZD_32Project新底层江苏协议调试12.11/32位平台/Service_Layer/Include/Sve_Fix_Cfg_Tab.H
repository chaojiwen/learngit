#ifndef __Sve_Fix_Cfg_Tab_H
#define __Sve_Fix_Cfg_Tab_H
			  	 
#include "stm32f10x.h"	     




/* 配置表在外部存储器中存储信息的成员数据结构 */
typedef struct {
	/* 成员名称(编号) */
	u8	Mbr_NO;
	/* 成员版本 */
	u8  Mbr_Ver;	
	/* 成员数据存储空间大小(单位:字节) */
	u16 Mbr_Size;
	/* 成员存储的相对地址 */
	u32 Mbr_Sre_Add; 
}Fix_Cfg_Tab_Mbr_Def;



void Sve_Fix_Cfg_Tab_Init(void);




#endif  




