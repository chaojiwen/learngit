#ifndef __DataLink_Cfg_Tab_H
#define __DataLink_Cfg_Tab_H
			  	 
#include "stm32f10x.h"	     



















void DataLink_Cfg_Tab_Inf_Read(char *Data_Src_Add,int Data_Offset_Add,int Data_Size);
void DataLink_Cfg_Tab_Inf_Write(char *Data_Src_Add,int Data_Offset_Add,int Data_Size);
void DataLink_Cfg_Tab_Para_Read(char *Data_Src_Add,int Data_Offset_Add,int Data_Size);
void DataLink_Cfg_Tab_Para_Write(char *Data_Src_Add,int Data_Offset_Add,int Data_Size);




#endif  






