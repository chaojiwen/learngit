/**
  ******************************************************************************
  * @file              : dev_i2c.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 27/09/2014 17:41:57
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2014 STMicroelectronics
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEV_I2C_H
#define DEV_I2C_H

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32f10x.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32glibc_rcc.h"
#include "stm32glibc_nvic.h"
#include "stm32glibc_systick.h"
#include "stm32glibc_tim.h"
#include "stm32glibc_afio.h"
#include "stm32glibc_gpio.h"
#include "stm32glibc_usart.h"
#include "stm32glibc_trace.h"
#include "stm32glibc_stream.h"
#include "interface.h"
//! @}

/*============================ MACROS ========================================*/
//! @{
//! \brief ai2c_address_define 
//!
#define AI2C_ADDRESS			0xA0
#define AI2C_PAGE_SIZE			128
#define AI2C_G_SDA			    0
#define AI2C_G_SCL			    0
#define AI2C_G_SDA_STR			"G_SDA"
#define AI2C_G_SCL_STR			"G_SCL"
//!
#define EM_SCL_H()         		I2CIO.SetBits(BSP_I2C_SCL)
#define EM_SCL_L()         		I2CIO.ResetBits(BSP_I2C_SCL)
//!
#define EM_SDA_H()         		I2CIO.SetBits(BSP_I2C_SDA)
#define EM_SDA_L()         		I2CIO.ResetBits(BSP_I2C_SDA)
//! @}

/*============================ TYPES =========================================*/
//!
namespace periph_c
{
    class CAI2C
    {
    public:
        static const uint8_t uAddress = AI2C_ADDRESS;
	
        static void Delay(uint32_t time)
        {
// 这里可以优化速度，经超频与降频测试，保持为空函数时还可以写入
			uint32_t i=time; 
			while(i)
			{
				i--;
			}
        }
/*
********************************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************************
*/
        static bool Start(void)
        {
//开始信号 SCL在高电平期间，SDA一个下降沿则表示启动信号，本函数也用来产生重复起始条件
//当前只有一个IIC设备，不会有其它设备占用的情况，所以不会有总线忙的情况出现，所以可以没有返回值
			EM_SDA_H();			///* 发送起始条件的数据信号*/释放SDA总线
			EM_SCL_H();
			Delay(100);			/* 起始条件建立时间大于4.7us,延时*/
			if(!I2CIO.ReadInputDataBit(BSP_I2C_SDA))
				return false;	//SDA线为低电平则总线忙,退出
			EM_SDA_L();			/* 发送起始信号*/
			Delay(100);			/* 起始条件锁定时间大于4μs*/
			if(I2CIO.ReadInputDataBit(BSP_I2C_SDA))
				return false;	//SDA线为高电平则总线忙,退出
			EM_SCL_L();			/* 钳住I2C总线，准备发送或接收数据 */
			Delay(100);
			return true;
        }
/*
********************************************************************************
                      结束总线函数               
  函数原型	: void  Stop_I2c();  
  功能		:     结束I2C总线,即发送I2C结束条件.  
********************************************************************************
*/
        static bool Stop(void)
        {
			EM_SCL_L();
			Delay(100);	
//停止 SCL在高电平期间，SDA一个上升沿则表示停止信号
			EM_SDA_L();			/*发送结束条件的时钟/数据信号*/
			Delay(100);	
			EM_SCL_H();
			Delay(100);	/*结束条件建立时间大于4μs*/
			EM_SDA_H();			/*发送I2C总线结束信号*/
			Delay(100);
			return true;
        }
/*
********************************************************************************
  函数原型	: bit I2C_Get_Ack_From_Slave(void)
  功能		: Get_Ack_From_Slave
  返回值	: Get the Ack or lose
********************************************************************************
*/
        static bool WaitAck(void)
        {
			int i = 0;			//WaitAck
			EM_SCL_L();
			Delay(100);
			EM_SDA_H();   		/*8位发送完后释放数据线，准备接收应答位*/
			Delay(100);
//应答 SCL在高电平期间，SDA被从设备拉为低电平表示应答
			EM_SCL_H();  		
			Delay(100);
			while((I2CIO.ReadInputDataBit(BSP_I2C_SDA)==1)&&(i<250))i++;  
			if((I2CIO.ReadInputDataBit(BSP_I2C_SDA)==1))
			{		
				EM_SCL_L();  
				Delay(100);
				Delay(100);

				return false;
			} else				/*判断是否接收到应答信号 (ack == 0) <ACK>*/
			{
				Delay(100);
				EM_SCL_L();  
				Delay(100);

				return true;
			}
        }

/******************************************************************************
                     应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
******************************************************************************/
/******************************************************************************
函数：I2C_PutAck()
功能：主机产生应答位（应答或非应答），用于通知从机：主机是否成功接收从机数据
参数：Ack = 0：主机应答
      Ack = 1：主机非应答
说明：主机在收到每一个从机字节后都要求从机产生应答，在收到最后一个字节时，应当产生非应答，通知从机不要再应答
******************************************************************************/
        static void Ack(bool ack)
        {
			EM_SCL_L();
			Delay(100);	
			/*(ack == 0) : ACK : An acknowledge(ACK) by pulling down the SDA line 
								 During the 9th clock cycle*/
			if(ack)				
			{
				EM_SDA_H();		//I2C_NoAck()
			}
			else
			{
				EM_SDA_L();		/*在此发出应答或非应答信号 *///I2C_Ack()
			}
			Delay(100);
			EM_SCL_H();
			Delay(100);			/*时钟低电平周期大于4μs*/
			EM_SCL_L();			/*清时钟线，钳住I2C总线以便继续接收*/
			Delay(100);
        }
/*
********************************************************************************
                 字节数据发送函数               
函数原型: u8 I2C_LLV_SendByte(u8 SendByte);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)     
          发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************************
*/
		static void SendByte(char chData)
		{
			uint8_t i = 0;
			for(i=0; i < 8; i++)
			{
				///拉低SCL，因为只有在时钟信号为低电平期间，数据线上的高低电平状态
				///才允许变化;并在此时和上一个循环的SDA_H一起形成一个上升沿
				EM_SCL_L();
				Delay(100);
				if(chData&0x80)	/*判断发送位*/
				{
					EM_SDA_H(); 
				}       
				else 
				{
					EM_SDA_L();
				}    
				chData<<=1;
				Delay(100);
				EM_SCL_H();		/*置时钟线为高，通知被控器开始接收数据位*/
				Delay(100);		/*保证时钟高电平周期大于4μs*/
			}
			EM_SCL_L();
		}
/*
********************************************************************************
                 字节数据接收函数               
函数原型: u8 I2C_LLV_ReceiveByte(void);
功能:     用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。  
********************************************************************************
*/
		static void RecvByte(char *pData)
		{
			uint8_t i;
			*pData = 0;
			//数据从高位到低位
			//EM_SCL_L();  
			//Delay();
			EM_SDA_H();  				/*置数据线为输入方式*/ 
			Delay(100);
			for(i=0; i < 8; i++)
			{
				(*pData)<<=1;
				EM_SCL_L();  
				Delay(100);
				EM_SCL_H();				//上升沿时，IIC设备将数据放在sda线上，
										//并在高电平期间数据已经稳定，可以接收啦  
										/*置时钟线为高使数据线上数据有效*/		
				Delay(100);
				if (I2CIO.ReadInputDataBit(BSP_I2C_SDA))
				{
					(*pData)|=0x01;
										//读数据位,接收的数据位放入pdata中
				}
				//EM_SCL_L();			//拉低SCL，使发送端可以把数据放在SDA上   
										/*置时钟线为低，准备接收数据位*/
				//Delay(100);			/*时钟低电平周期大于4.7μs*/
			}
			EM_SCL_L();
		}
    };
}
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:STM32F103C(8-B)Tx
extern vs32 			TimingDelay;
extern vu32			 	nSystemTimingSystick;
/*============================ EXTERN FUNCTIONS ==============================*/


//#ifdef __cplusplus
//}
//#endif

#endif /* DEV_I2C_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

