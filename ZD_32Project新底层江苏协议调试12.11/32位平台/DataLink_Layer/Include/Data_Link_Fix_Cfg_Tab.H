#ifndef __Data_Link_Fix_Cfg_Tab_H
#define __Data_Link_Fix_Cfg_Tab_H
			  	 
#include "stm32f10x.h"	     


void Data_Link_Fix_Cfg_Tab_Inf_Read(char *Data_Src_Add,int Data_Offset_Add,int Data_Size);
void Data_Link_Fix_Cfg_Tab_Inf_Write(char *Data_Src_Add,int Data_Offset_Add,int Data_Size);
void Data_Link_Fix_Cfg_Tab_Para_Read(char *Data_Src_Add,int Data_Offset_Add,int Data_Size);
void Data_Link_Fix_Cfg_Tab_Para_Write(char *Data_Src_Add,int Data_Offset_Add,int Data_Size);


#endif  





