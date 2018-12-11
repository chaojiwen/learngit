#ifndef __Dev_AI2C_H
#define __Dev_AI2C_H
			  	 
#include "stm32f10x.h"	     
#include "ucos_ii.h"


/* Ä£ÄâI2CÐÅºÅÁ¿*/
extern OS_EVENT *Dev_AI2C_Sem;













void Dev_AI2C_Read(char *dest,int src,int n);
void Dev_AI2C_Write(char *dest,int src,int n);




#endif  































