#ifndef __Dev_Oled_H
#define __Dev_Oled_H			  	 
#include "stm32f10x.h"	 
#include "stdlib.h"	    
//SSD1306 OLED 驱动IC驱动代码
//驱动方式:8080并口/4线串口
//版本:V1.1

/* 选择BANK1-BORSRAM1 连接 TFT，地址范围为0X60000000~0X63FFFFFF
 * FSMC_A16 接LCD的DC(寄存器/数据选择)脚
 * 16 bit => FSMC[24:0]对应HADDR[25:1]
 * 寄存器基地址 = 0X60000000
 * RAM基地址 = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
 * 当选择不同的地址线时，地址要重新计算。
 */
#define Bank1_LCD_D    ((u32)0x60020000)       //Disp Data ADDR
#define Bank1_LCD_C    ((u32)0x60000000)	   //Disp Reg ADDR

/*选定LCD指定寄存器*/
#define OLED_WR_REG(index)    	((*(__IO u16 *) (Bank1_LCD_C)) = ((u16)index))
/*将LCD中的index寄存器设置为val值*/
#define OLED_WR_CMD(index,val)  do{(*(__IO u16 *) (Bank1_LCD_C)) = ((u16)index);(*(__IO u16 *) (Bank1_LCD_D)) = ((u16)(val));}while(0)
/*往LCD GRAM写入数据*/
#define OLED_WR_Data(val)       ((*(__IO u16 *) (Bank1_LCD_D)) = ((u16)(val)))
		
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Font,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);	 
#endif  
	 



