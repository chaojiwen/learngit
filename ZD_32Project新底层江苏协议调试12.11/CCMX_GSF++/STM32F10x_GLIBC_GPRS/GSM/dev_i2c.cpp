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
//	<e>ʹ��printf�ı����
//	<i>ͨ��ϵͳ����printf�����ӡ������Ϣ
#define	DEBUG_OUTPUT_EN				1
//		<e>USART1ӳ��printf
//		<i>ʹ��USART1�ı������ӡ������Ϣ
//		<i>����ʹ��ITM Trace�����ӡ������Ϣ
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
  * @brief  Description "ģ��I2Cд����"
  * @param  pdata  		д������ݻ�����ָ��
  * @param  addr		д��������ڴ��ַ
  * @param  count		д����������ݴ�С
  * @retval bool		I2Cд�����Ƿ�ɹ�
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
  * @brief  Description "ģ��I2C������"
  * @param  pdata  		��ȡ�����ݻ�����ָ��
  * @param  addr		��ȡ�������ڴ��ַ
  * @param  count		��ȡ���������ݴ�С
  * @retval bool		I2C�������Ƿ�ɹ�
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
    theAI2C.Ack(true);//���ս���,���ͷ�Ӧ���ź�
    theAI2C.Stop();
	return true;
}

#endif /* DEV_I2C_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
