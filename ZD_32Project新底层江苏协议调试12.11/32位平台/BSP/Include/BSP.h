#ifndef __BSP_H_    
#define __BSP_H_   
   
#include "stm32f10x.h"
#include "Mcu_Config.h"

/************************************************************************************************/
void float_to_string(float f, char r[]);

void BSP_Init(void); 

void System_Para_Init(void);

void System_Serial_Init(void);

void Serial_Number_Init(SERIAL_NUMBER *serial_number);
   
#endif


