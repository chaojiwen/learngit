#ifndef __Mcu_Oled_H
#define __Mcu_Oled_H			  	 
#include "stm32f10x.h"	 
#include "stdlib.h"	    
//SSD1306 OLED ����IC��������
//������ʽ:8080����/4�ߴ���
				  
//-----------------OLED�˿ڶ���----------------  

//#define OLED_Power 
#define OLED_Power_PORT            GPIOE
#define OLED_Power_CLK             RCC_APB2Periph_GPIOE 
#define OLED_Power_PIN             GPIO_Pin_6

//#define OLED_RST  PBout(14)//��MINISTM32��ֱ�ӽӵ���STM32�ĸ�λ�ţ�
#define OLED_RST_PORT              GPIOD
#define OLED_RST_CLK               RCC_APB2Periph_GPIOD
#define OLED_RST_PIN               GPIO_Pin_10

#define Set_OLED_RST  {GPIO_SetBits(OLED_RST_PORT,OLED_RST_PIN);}
#define Clr_OLED_RST  {GPIO_ResetBits(OLED_RST_PORT,OLED_RST_PIN);}

//#define OLED_CS PDout(7)
#define OLED_CS_PORT              GPIOD
#define OLED_CS_CLK               RCC_APB2Periph_GPIOD
#define OLED_CS_PIN               GPIO_Pin_7

#define Set_OLED_CS  {GPIO_SetBits(OLED_CS_PORT,OLED_CS_PIN);}
#define Clr_OLED_CS  {GPIO_ResetBits(OLED_CS_PORT,OLED_CS_PIN);} 

//#define OLED_RS PDout(11)	  //OLED_DC
#define OLED_RS_PORT              GPIOD
#define OLED_RS_CLK               RCC_APB2Periph_GPIOD
#define OLED_RS_PIN               GPIO_Pin_11

#define Set_OLED_RS  {GPIO_SetBits(OLED_RS_PORT,OLED_RS_PIN);}
#define Clr_OLED_RS  {GPIO_ResetBits(OLED_RS_PORT,OLED_RS_PIN);}

//#define OLED_WR PDout(5)		
#define OLED_WR_PORT              GPIOD
#define OLED_WR_CLK               RCC_APB2Periph_GPIOD  
#define OLED_WR_PIN               GPIO_Pin_5

#define Set_OLED_WR  {GPIO_SetBits(OLED_WR_PORT,OLED_WR_PIN);}
#define Clr_OLED_WR  {GPIO_ResetBits(OLED_WR_PORT,OLED_WR_PIN);}
  
//#define OLED_RD PDout(1)
#define OLED_RD_PORT              GPIOD
#define OLED_RD_CLK               RCC_APB2Periph_GPIOD 
#define OLED_RD_PIN               GPIO_Pin_4

#define Set_OLED_RD  {GPIO_SetBits(OLED_RD_PORT,OLED_RD_PIN);}
#define Clr_OLED_RD  {GPIO_ResetBits(OLED_RD_PORT,OLED_RD_PIN);}

//PB0~7,��Ϊ������
//#define DATAOUT(x) GPIOB->ODR=(GPIOB->ODR&0xff00)|(x&0x00FF); //���,ԭ��ԭ�ӵĴ���
#define OLED_GPIO_DATA_PORT	 GPIOE
#define DATAOUT(DataValue) {GPIO_Write(GPIOD,(GPIO_ReadOutputData(GPIOD)&0x3FFC)|((DataValue&0x0003)<<14)|((DataValue&0x000C)>>2));\
							GPIO_Write(GPIOE,(GPIO_ReadOutputData(GPIOE)&0xF87F)|((DataValue&0x00F0)<<3));}  
  


void OLED_Config(void);
void OLED_GPIO_Config(void);
void OLED_FSMC_Config(void);
 
#endif  
	 



