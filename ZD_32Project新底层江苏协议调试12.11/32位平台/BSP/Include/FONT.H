#ifndef __FONT_H_
#define __FONT_H_

#include "stm32f10x.h"	 

/* 中文字模的存储数量 */
#define MAX_Chinese_NUM 					(150u)

/* 中文 内码 和 字模 的数据结构 */
typedef struct {
	u16			Code;
	const u8* 	Font;     
}Chinese;

//中文字模的存储数量
extern const Chinese Han_Zi[MAX_Chinese_NUM];
//字体 Font
extern const unsigned char asc2_0805[28][5];
extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[240][16];
extern const unsigned char asc2_2412[240][36];
extern const unsigned char asc2_4816[][96];



/******************* (C) ZHOU *****END OF FILE****/




     
#endif
