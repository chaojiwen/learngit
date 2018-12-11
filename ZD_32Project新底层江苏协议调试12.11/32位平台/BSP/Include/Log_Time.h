#ifndef __Log_Time_H
#define __Log_Time_H
			  	 
#include "stm32f10x.h"	     



/* 时间数据结构体->类型 */
typedef struct 
{
  unsigned int  Year;
  unsigned char Month;
  unsigned char Day;
  unsigned char Hour;
  unsigned char Min;
  unsigned char Sec;
  unsigned char Week;
  char			Zone;
}Times; 
	
/* 系统的时间数据结构体 */
extern Times System_Time;

#endif  
	 

