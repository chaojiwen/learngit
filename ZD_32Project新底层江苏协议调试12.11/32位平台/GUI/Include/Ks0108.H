#ifndef __Ks0108_H
#define __Ks0108_H

#include "stm32f10x.h"  

#include "Log_Time.h"




#define Ks0108_CMD_Disp_ON          0x3F
#define Ks0108_CMD_Disp_OFF         0x3E
#define Ks0108_CMD_Set_Addr         0x40
#define Ks0108_CMD_Set_Page         0xB8
#define Ks0108_CMD_Disp_Start_Line  0xC0

//显示模式
#define GUI_TEXTMODE_NORMAL      (0)
#define GUI_TEXTMODE_REVERSE     (1<<0)
#define GUI_TEXTMODE_UNDERLINE   (1<<1)
#define GUI_TEXTMODE_XOR         (1<<2)
//对齐方式
#define GUI_TA_LEFT              (0<<4)
#define GUI_TA_HCENTER           (1<<4)
#define GUI_TA_RIGHT             (2<<4)
#define GUI_JUST_TEXT            (1<<7)

//英文的两种字体
#define En_8x16      	  (0)
#define En_6x12       	(1)
#define En_5x8       	  (2)
#define En_12x24       	(3)
#define En_16x48       	(4)

//字体宽度定义
#define En_8x16_XSIZE  8
#define En_8x16_YSIZE  16

#define En_6x12_XSIZE  6
#define En_6x12_YSIZE  12

#define En_5x8_XSIZE   5
#define En_5x8_YSIZE   8

#define En_12x24_XSIZE 12
#define En_12x24_YSIZE 24

#define En_16x48_XSIZE 16
#define En_16x48_YSIZE 48

//中文字体宽度定义
#define Chinese_XSIZE  16
#define Chinese_YSIZE  2

//英文字体宽度定义
extern u8 Char_XSIZE;
extern u8 Char_YSIZE;

/********************************* 参数寄出厂设置值 **********************************/
void GUI_Init(void);
void GUI_Clear(void);
void GUI_SetTextMode(u8 TextMode);
void GUI_SetEnFont(u8 EnFont);
void GUI_DispCharAt(u8 c, u8 x, u8 y);
void GUI_DispStringAt(u8 *s,u8 X,u8 Y);
void GUI_DispDecAt(u16 v, u16 x, u16 y, u8 Len);
void GUI_DispStringAtBar(u8 *s,u8 x0, u8 y0, u8 x1,u8 Mode);
void GUI_Delay(u16 Period);
void GUI_DrawIcon(u8 const *Ico,u8 X,u8 Y);
void GUI_Draw_Bar_BMP(u8 const *Ico,u8 x0,u8 y0);
void GUI_Draw_CH_NO(u8 const *Ico,u8 x0,u8 y0);
void GUI_Draw_Card_SN_NO(u32 sn_no,u8 x0,u8 y0);
void GUI_Draw_Sensor_ICO(u8 const *Ico,u8 x0,u8 y0);
void GUI_Disp_Headline(u8 *String);
void HBar(u8 y, u8 x0, u8 x1,u8 percent);
void Bar(u8 Item_,u8 ItemNum_,u8 BarPosX,u8 BarNumPosX);
void TipClr( u8 x0, u8 y0, u8 x1, u8 y1);


#endif
