#ifndef __Log_Time_H
#define __Log_Time_H
			  	 
#include "stm32f10x.h"	     



/* ʱ�����ݽṹ��->���� */
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
	
/* ϵͳ��ʱ�����ݽṹ�� */
extern Times System_Time;

#endif  
	 

