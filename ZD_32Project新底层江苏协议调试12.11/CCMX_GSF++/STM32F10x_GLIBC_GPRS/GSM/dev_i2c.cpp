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
#define	DEBUG_OUTPUT_EN				1
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
    uint8_t 	addr_h = 0, addr_l = 0;
    uint16_t 	i;
    CAI2C		theAI2C;

    addr_h = ((addr / 256) & 0x7) << 1;
    addr_l = ((addr % 256));

    addr_h = theAI2C.uAddress + addr_h;
    if (count == 0)
    {
        return true;
    }

    if (!theAI2C.Start())
        return false;
    theAI2C.SendByte(addr_h);	// I2C_WRITE
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
  * @brief  Description "模拟I2C读函数"
  * @param  pdata  		读取的数据缓冲区指针
  * @param  addr		读取的器件内存地址
  * @param  count		读取的器件数据大小
  * @retval bool		I2C读函数是否成功
  */
bool AI2C_Read(char *pData, int addr, int count)
{
    uint8_t 	addr_h = 0, addr_l = 0;
    uint16_t 	i;
    CAI2C		theAI2C;

    addr_h = ((addr / 256) & 0x7) << 1;
    addr_l = ((addr % 256));

    addr_h = theAI2C.uAddress + addr_h;
    if (count == 0)
    {
        return true;
    }
    if (!theAI2C.Start())
        return false;
    theAI2C.SendByte(addr_h);
    if (!theAI2C.WaitAck())
    {
        theAI2C.Stop();
        return false;
    }
    //theAI2C.SendByte(addr_h);
    //theAI2C.WaitAck();
    theAI2C.SendByte(addr_l);
    theAI2C.WaitAck();
    theAI2C.Start();
    theAI2C.SendByte((addr_h) | 0x01);
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

#endif /* DEV_I2C_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
