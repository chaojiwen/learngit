#ifndef __Mcu_Flash_H_
#define __Mcu_Flash_H_
#include "stm32f10x.h"




BOOL Write_Para(uint32_t adder,uint8_t * data,uint32_t Len);
BOOL Read_Para(uint32_t adder,uint8_t * data,uint32_t Len);
void Erase_Para(void);

BOOL Write_Data(uint32_t adder,uint8_t * data,uint32_t Len);
void Erase_Data(void);



#endif
