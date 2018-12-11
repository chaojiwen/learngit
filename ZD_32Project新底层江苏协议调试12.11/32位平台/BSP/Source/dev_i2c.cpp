/**
  ******************************************************************************
  * @file              : dev_i2c.cpp
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 27/09/2014 17:41:56
  * @brief             : c file
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
#ifndef DEV_I2C_C
#define DEV_I2C_C

/*============================ INCLUDES ======================================*/
/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include "app_gsm.h"
#include "dev_i2c.h"

//	<<< Use Configuration Wizard in Context Menu >>>
//	<e>使用printf文本输出
//	<i>通过系统函数printf输出打印调试信息
#define	DEBUG_OUTPUT_EN				0
//		<e>USART1映射printf
//		<i>使用USART1文本输出打印调试信息
//		<i>或者使用ITM Trace输出打印调试信息
#define DEBUG_OUTPUT_USART1_EN		0
//		</e>
//	</e>
//	<<< end of configuration section >>>

/*============================ MACRO =========================================*/

/*============================ TYPES =========================================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ EXTERN FUNCTIONS ==============================*/
using periph_c::CAI2C;
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Description "模拟I2C写函数"
  * @param  pdata  		写入的数据缓冲区指针
  * @param  addr		写入的器件内存地址
  * @param  count		写入的器件数据大小
  * @retval bool		I2C写函数是否成功
  */
bool AI2C_Write(char *pData, int addr, int count)
{
    
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
    CAI2C		theAI2C(0xA0);
	/*            从机ID   + 器件选择 + 读写选择  */
	device_addr = theAI2C + 0x0		+ 0;
    addr_msb = ((addr / 256)&0X3F);
    addr_lsb = (addr % 256);

    if (count == 0)
    {
        return true;
    }

    if (!theAI2C.Start())
        return false;
    theAI2C.SendByte(device_addr);	// I2C_WRITE
    if (!theAI2C.WaitAck())
    {
        theAI2C.Stop();
        return false;
    }
    theAI2C.SendByte(addr_msb);
    theAI2C.WaitAck();
    theAI2C.SendByte(addr_lsb);
    theAI2C.WaitAck();
    for (i = 0; i < count; i ++)
    {
        theAI2C.SendByte(*(pData + i));
        theAI2C.WaitAck();
    }
    theAI2C.Stop();
    return true;
}

/**
  * @brief  Description "模拟I2C读函数"
  * @param  pdata  		读取的数据缓冲区指针
  * @param  addr		读取的器件内存地址
  * @param  count		读取的器件数据大小
  * @retval bool		I2C读函数是否成功
  */
bool AI2C_Read(char *pData, int addr, int count)
{
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
    CAI2C		theAI2C(0xA0);
	/*            从机ID   + 器件选择 + 读写选择  */
	device_addr = theAI2C + 0x0		+ 0;
    addr_msb = ((addr / 256)&0X3F);
    addr_lsb = (addr % 256);
    if (count == 0)
    {
        return true;
    }
    if (!theAI2C.Start())
        return false;
    theAI2C.SendByte(device_addr);
    if (!theAI2C.WaitAck())
    {
        theAI2C.Stop();
        return false;
    }
    theAI2C.SendByte(addr_msb);
    theAI2C.WaitAck();
    theAI2C.SendByte(addr_lsb);
    theAI2C.WaitAck();
    theAI2C.Start();
    theAI2C.SendByte(device_addr|0X01);
    theAI2C.WaitAck();
    for (i = 0; i < count - 1; i ++)
    {
        theAI2C.RecvByte((pData + i));
        theAI2C.Ack(false);
    }
    theAI2C.RecvByte(pData + count - 1);
    theAI2C.Ack(true);//接收结束,发送非应答信号
    theAI2C.Stop();
	return true;
}


/**
  * @brief  Description "模拟I2C写RTC8025函数"
  * @param  pdata  		写入的数据缓冲区指针
  * @param  addr		写入的器件内存地址
  * @param  count		写入的器件数据大小
  * @retval bool		RTC8025写函数是否成功
  */
bool RTC8025_Write(char *pData, int addr, int count)
{
    uint8_t 	addr_h = 0, addr_l = 0;
    uint16_t 	i;
    CAI2C		theAI2C(0x64);

    addr_l = ((addr % 16) << 4);

    addr_h = theAI2C + addr_h;
    if (count == 0)
    {
        return true;
    }
	//! START condition sent by Master
    if (!theAI2C.Start())
        return false;
	//! Slave address + write specification
    theAI2C.SendByte(addr_h);	// I2C_WRITE
	//! Confirmation response from Master
    if (!theAI2C.WaitAck())
    {
        theAI2C.Stop();
        return false;
    }
    //theAI2C.SendByte(addr_h);
    //theAI2C.WaitAck();
    theAI2C.SendByte(addr_l);
    theAI2C.WaitAck();
    for (i = 0; i < count; i ++)
    {
        theAI2C.SendByte(*(pData + i));
        theAI2C.WaitAck();
    }
    theAI2C.Stop();
    return true;
}

/**
  * @brief  Description "模拟I2C读RTC8025函数"
  * @param  pdata  		读取的数据缓冲区指针
  * @param  addr		读取的器件内存地址
  * @param  count		读取的器件数据大小
  * @retval bool		RTC8025读函数是否成功
  */
bool RTC8025_Read(char *pData, int addr, int count)
{
    uint8_t 	addr_h = 0, addr_l = 0;
    uint16_t 	i;
    CAI2C		theAI2C(0x64);

    addr_l = ((addr % 16) << 4);

    addr_h = theAI2C + addr_h;
    if (count == 0)
    {
        return true;
    }
	//! START condition sent by Master
    if (!theAI2C.Start())
        return false;
	//! Slave address + write specification
    theAI2C.SendByte(addr_h);
	//! Confirmation response from Master
    if (!theAI2C.WaitAck())
    {
        theAI2C.Stop();
        return false;
    }
//	theAI2C.SendByte(addr_h);
//	theAI2C.WaitAck();
    theAI2C.SendByte(addr_l);
    theAI2C.WaitAck();
	//! RESTART condition sent by Master
    theAI2C.Start();
	//! Indicates next byte will be read
    theAI2C.SendByte((addr_h) | 0x01);
    theAI2C.WaitAck();
	//! Data is read from the specified start address
    for (i = 0; i < count - 1; i ++)
    {
        theAI2C.RecvByte((pData + i));
        theAI2C.Ack(false);
    }
    theAI2C.RecvByte(pData + count - 1);
	//! Master does not respond
    theAI2C.Ack(true);//接收结束,发送非应答信号
	//! STOP condition sent by Master
    theAI2C.Stop();
	return true;
}


/**
  * @brief  Description "RTC8025状态位复位函数"
  * @param  None
  * @retval bool		RTC8025读函数是否成功
  */
bool RTC8025_Reset(bool need_reset)
{

	RTC8025_Ctrl_Typedef ctrl = {0};
	RTC8025_Typedef RTC8025_CR = {0};
	
	
	RTC8025_Read((char*)&RTC8025_CR, 0x08, 5);
	RTC8025_Read((char*)&ctrl.Control, 0x0E, 2);
	if (ctrl.STREG.PON)
	{
		DebugOutPrintf(DEBUG_INF, "power-on reset was detected, ");
		DebugOutPrintf(DEBUG_INF, "you may have to readjust the clock.\r\n");
		need_reset = true;
	}
	if (ctrl.STREG.VDET)
	{
		DebugOutPrintf(DEBUG_INF, "a power voltage drop was detected, ");
		DebugOutPrintf(DEBUG_INF, "you may have to readjust the clock.\r\n");
		need_reset = true;
	}
	if (!ctrl.STREG.XST)
	{
		DebugOutPrintf(DEBUG_INF, "Oscillation stop was detected,");
		DebugOutPrintf(DEBUG_INF, "you may have to readjust the clock.\r\n");
		need_reset = true;
	}
	
	if((ctrl.STREG.CT != 0)||
	   (ctrl.STREG.H12_24 != 1)||
	   (ctrl.STREG.DALE != 0)||
	   (ctrl.STREG.WALE != 0)||
	   (ctrl.STREG.DAFG != 0)||
	   (ctrl.STREG.WAFG != 0)||	
	   (ctrl.STREG.CTFG != 0)||		
	   (ctrl.STREG.PON != 0)||			
	   (ctrl.STREG.VDSL != 0)	
	   )
	{
		need_reset = true;
	}
	
	if (need_reset)
	{
		RTC8025_CR.STREG.Seconds 	= 0;
		RTC8025_CR.STREG.Resv1 		= 0;

		RTC8025_CR.STREG.Minutes 	= 0;
		RTC8025_CR.STREG.Resv2 		= 0;

		RTC8025_CR.STREG.Hours 		= 0;
		RTC8025_CR.STREG.Resv3 		= 0;		
		
		RTC8025_CR.STREG.Weekdays 	= 0;
		RTC8025_CR.STREG.Resv4 		= 0;		

		RTC8025_CR.STREG.Days 		= 0;
		RTC8025_CR.STREG.Resv5 		= 0;	
		
		RTC8025_CR.STREG.Months 	= 0;
		RTC8025_CR.STREG.Resv6 		= 0;
		RTC8025_CR.STREG.YearCarry 	= 0;
		
		RTC8025_CR.STREG.Years 		= 0;
	
		RTC8025_CR.STREG.DigitalOffset 	= 0;
		
		RTC8025_CR.STREG.Alarm_W_Minute = 0;
		RTC8025_CR.STREG.Resv7 			= 0;

		RTC8025_CR.STREG.Alarm_W_Hour 	= 0;
		RTC8025_CR.STREG.Resv8 			= 0;

		RTC8025_CR.STREG.Alarm_W_day 	= 0;
		RTC8025_CR.STREG.Resv9 			= 0;

		RTC8025_CR.STREG.Alarm_D_Minute = 0;
		RTC8025_CR.STREG.Resv10 		= 0;

		RTC8025_CR.STREG.Alarm_D_Hour 	= 0;
		RTC8025_CR.STREG.Resv11 		= 0;
		
		/* 设置 RTC8025 固定周期中断功能 */
		ctrl.STREG.CT 		= 0;	//
		ctrl.STREG.Resv1 	= 0;	//TEST for manufacturer
		ctrl.STREG.CLEN2 	= 0;	//
		ctrl.STREG.H12_24 	= 1;	//
		ctrl.STREG.DALE 	= 0;	//
		ctrl.STREG.WALE 	= 0;	//
		
		ctrl.STREG.DAFG 	= 0;	//
		ctrl.STREG.WAFG 	= 0;	//
		ctrl.STREG.CTFG 	= 0;	//
		ctrl.STREG.CLEN1 	= 0;	//
		ctrl.STREG.PON 		= 0;	//
		ctrl.STREG.XST 		= 1;	//
		ctrl.STREG.VDET 	= 0;	//
		ctrl.STREG.VDSL 	= 0;	//

		RTC8025_Write	((char*)&RTC8025_CR, 	0x08, 5);
		RTC8025_Read	((char*)&RTC8025_CR, 	0x08, 5);
		RTC8025_Write	((char*)&ctrl.Control, 	0x0E, 2);
		RTC8025_Read	((char*)&ctrl.Control, 	0x0E, 2);
	}

	if (need_reset)
	{
		return false;
	}
	
	return true;
}

#endif /* DEV_I2C_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
