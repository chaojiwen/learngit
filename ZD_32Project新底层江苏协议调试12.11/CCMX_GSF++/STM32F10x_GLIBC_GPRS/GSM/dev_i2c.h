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
// ��������Ż��ٶȣ�����Ƶ�뽵Ƶ���ԣ�����Ϊ�պ���ʱ������д��
			uint32_t i=time; 
			while(i)
			{
				i--;
			}
        }
/*
********************************************************************************
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:     ����I2C����,������I2C��ʼ����.  
********************************************************************************
*/
        static bool Start(void)
        {
//��ʼ�ź� SCL�ڸߵ�ƽ�ڼ䣬SDAһ���½������ʾ�����źţ�������Ҳ���������ظ���ʼ����
//��ǰֻ��һ��IIC�豸�������������豸ռ�õ���������Բ���������æ��������֣����Կ���û�з���ֵ
			EM_SDA_H();			///* ������ʼ�����������ź�*/�ͷ�SDA����
			EM_SCL_H();
			Delay(100);			/* ��ʼ��������ʱ�����4.7us,��ʱ*/
			if(!I2CIO.ReadInputDataBit(BSP_I2C_SDA))
				return false;	//SDA��Ϊ�͵�ƽ������æ,�˳�
			EM_SDA_L();			/* ������ʼ�ź�*/
			Delay(100);			/* ��ʼ��������ʱ�����4��s*/
			if(I2CIO.ReadInputDataBit(BSP_I2C_SDA))
				return false;	//SDA��Ϊ�ߵ�ƽ������æ,�˳�
			EM_SCL_L();			/* ǯסI2C���ߣ�׼�����ͻ�������� */
			Delay(100);
			return true;
        }
/*
********************************************************************************
                      �������ߺ���               
  ����ԭ��	: void  Stop_I2c();  
  ����		:     ����I2C����,������I2C��������.  
********************************************************************************
*/
        static bool Stop(void)
        {
			EM_SCL_L();
			Delay(100);	
//ֹͣ SCL�ڸߵ�ƽ�ڼ䣬SDAһ�����������ʾֹͣ�ź�
			EM_SDA_L();			/*���ͽ���������ʱ��/�����ź�*/
			Delay(100);	
			EM_SCL_H();
			Delay(100);	/*������������ʱ�����4��s*/
			EM_SDA_H();			/*����I2C���߽����ź�*/
			Delay(100);
			return true;
        }
/*
********************************************************************************
  ����ԭ��	: bit I2C_Get_Ack_From_Slave(void)
  ����		: Get_Ack_From_Slave
  ����ֵ	: Get the Ack or lose
********************************************************************************
*/
        static bool WaitAck(void)
        {
			int i = 0;			//WaitAck
			EM_SCL_L();
			Delay(100);
			EM_SDA_H();   		/*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
			Delay(100);
//Ӧ�� SCL�ڸߵ�ƽ�ڼ䣬SDA�����豸��Ϊ�͵�ƽ��ʾӦ��
			EM_SCL_H();  		
			Delay(100);
			while((I2CIO.ReadInputDataBit(BSP_I2C_SDA)==1)&&(i<250))i++;  
			if((I2CIO.ReadInputDataBit(BSP_I2C_SDA)==1))
			{		
				EM_SCL_L();  
				Delay(100);
				Delay(100);

				return false;
			} else				/*�ж��Ƿ���յ�Ӧ���ź� (ack == 0) <ACK>*/
			{
				Delay(100);
				EM_SCL_L();  
				Delay(100);

				return true;
			}
        }

/******************************************************************************
                     Ӧ���Ӻ���
����ԭ��:  void Ack_I2c(bit a);
����:      ����������Ӧ���ź�(������Ӧ����Ӧ���źţ���λ����a����)
******************************************************************************/
/******************************************************************************
������I2C_PutAck()
���ܣ���������Ӧ��λ��Ӧ����Ӧ�𣩣�����֪ͨ�ӻ��������Ƿ�ɹ����մӻ�����
������Ack = 0������Ӧ��
      Ack = 1��������Ӧ��
˵�����������յ�ÿһ���ӻ��ֽں�Ҫ��ӻ�����Ӧ�����յ����һ���ֽ�ʱ��Ӧ��������Ӧ��֪ͨ�ӻ���Ҫ��Ӧ��
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
				EM_SDA_L();		/*�ڴ˷���Ӧ����Ӧ���ź� *///I2C_Ack()
			}
			Delay(100);
			EM_SCL_H();
			Delay(100);			/*ʱ�ӵ͵�ƽ���ڴ���4��s*/
			EM_SCL_L();			/*��ʱ���ߣ�ǯסI2C�����Ա��������*/
			Delay(100);
        }
/*
********************************************************************************
                 �ֽ����ݷ��ͺ���               
����ԭ��: u8 I2C_LLV_SendByte(u8 SendByte);
����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
          ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)     
          ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************************
*/
		static void SendByte(char chData)
		{
			uint8_t i = 0;
			for(i=0; i < 8; i++)
			{
				///����SCL����Ϊֻ����ʱ���ź�Ϊ�͵�ƽ�ڼ䣬�������ϵĸߵ͵�ƽ״̬
				///������仯;���ڴ�ʱ����һ��ѭ����SDA_Hһ���γ�һ��������
				EM_SCL_L();
				Delay(100);
				if(chData&0x80)	/*�жϷ���λ*/
				{
					EM_SDA_H(); 
				}       
				else 
				{
					EM_SDA_L();
				}    
				chData<<=1;
				Delay(100);
				EM_SCL_H();		/*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
				Delay(100);		/*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/
			}
			EM_SCL_L();
		}
/*
********************************************************************************
                 �ֽ����ݽ��պ���               
����ԭ��: u8 I2C_LLV_ReceiveByte(void);
����:     �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
          ���������Ӧ����Ӧ��ӻ���  
********************************************************************************
*/
		static void RecvByte(char *pData)
		{
			uint8_t i;
			*pData = 0;
			//���ݴӸ�λ����λ
			//EM_SCL_L();  
			//Delay();
			EM_SDA_H();  				/*��������Ϊ���뷽ʽ*/ 
			Delay(100);
			for(i=0; i < 8; i++)
			{
				(*pData)<<=1;
				EM_SCL_L();  
				Delay(100);
				EM_SCL_H();				//������ʱ��IIC�豸�����ݷ���sda���ϣ�
										//���ڸߵ�ƽ�ڼ������Ѿ��ȶ������Խ�����  
										/*��ʱ����Ϊ��ʹ��������������Ч*/		
				Delay(100);
				if (I2CIO.ReadInputDataBit(BSP_I2C_SDA))
				{
					(*pData)|=0x01;
										//������λ,���յ�����λ����pdata��
				}
				//EM_SCL_L();			//����SCL��ʹ���Ͷ˿��԰����ݷ���SDA��   
										/*��ʱ����Ϊ�ͣ�׼����������λ*/
				//Delay(100);			/*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/
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

