/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/10/28																			 */
/*************************************************************************************************/
#include "Mcu_oled.h"


/*************************************************************************************
 * ��������OLED_Config
 * ����  ��OLED CHUSHI ��ʼ������
 * ����  : �� 
 * ���  ����
***************************************************************************************/ 
void OLED_Config(void)
{
	/* GPIO ���� */
	OLED_GPIO_Config();
	/* FSMC �������� */
	OLED_FSMC_Config();
}

/*************************************************************************************
 * ��������OLED_GPIO_Config
 * ����  ��OLED FSMC ��������
 * ����  : �� 
 * ���  ����
***************************************************************************************/ 			    
void OLED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
#if 0    
    /* ʹ��FSMCʱ��*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* ����LCD��λ���ƹܽ�*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 	 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  		   
    
    /* ����FSMC���Ӧ��������,FSMC-D0~D7: PD 14 15 0 1,PE 7 8 9 10*/	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_14| GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* ����FSMC���Ӧ�Ŀ�����
	 * PD4-FSMC_NOE  :LCD-RD
   * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
   * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
#else	
	//ʹ��PORTBʱ�� 
	//ʹ��PORTCʱ�� 	  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE );	
	
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_0|
								  		GPIO_Pin_1|
								  		GPIO_Pin_14|
								  		GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_0);
	GPIO_SetBits(GPIOD, GPIO_Pin_1);
	GPIO_SetBits(GPIOD, GPIO_Pin_14);
	GPIO_SetBits(GPIOD, GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_4|
										GPIO_Pin_5|
										GPIO_Pin_7|
										GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_SetBits(GPIOD, GPIO_Pin_4);
	GPIO_SetBits(GPIOD, GPIO_Pin_5);
	GPIO_SetBits(GPIOD, GPIO_Pin_7);
	GPIO_SetBits(GPIOD, GPIO_Pin_11);

	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_7|
								  		GPIO_Pin_8|
								  		GPIO_Pin_9|
								  		GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
#endif
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_10;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}  


/*******************************************
 * ��������LCD_FSMC_Config
 * ����  ��LCD  FSMC ģʽ����
 * ����  : �� 
 * ���  ����
*********************************************/ 
void OLED_FSMC_Config(void)
{
//    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
//    FSMC_NORSRAMTimingInitTypeDef  p; 
//    
//    
//    p.FSMC_AddressSetupTime 		= 0x02;	 //��ַ����ʱ��
//    p.FSMC_AddressHoldTime 			= 0x00;	 //��ַ����ʱ��
//    p.FSMC_DataSetupTime 			= 0x05;	 //���ݽ���ʱ��
//    p.FSMC_BusTurnAroundDuration 	= 0x00;
//    p.FSMC_CLKDivision 				= 0x00;
//    p.FSMC_DataLatency 				= 0x00;
//    p.FSMC_AccessMode 				= FSMC_AccessMode_B;// һ��ʹ��ģʽB������LCD
//    
//    FSMC_NORSRAMInitStructure.FSMC_Bank 					= FSMC_Bank1_NORSRAM1;
//    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux 			= FSMC_DataAddressMux_Disable;
//    FSMC_NORSRAMInitStructure.FSMC_MemoryType 				= FSMC_MemoryType_NOR;
//    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth 			= FSMC_MemoryDataWidth_8b;
//    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode 			= FSMC_BurstAccessMode_Disable;
//    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity 		= FSMC_WaitSignalPolarity_Low;
//    FSMC_NORSRAMInitStructure.FSMC_WrapMode 				= FSMC_WrapMode_Disable;
//    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive 		= FSMC_WaitSignalActive_BeforeWaitState;
//    FSMC_NORSRAMInitStructure.FSMC_WriteOperation 			= FSMC_WriteOperation_Enable;
//    FSMC_NORSRAMInitStructure.FSMC_WaitSignal 				= FSMC_WaitSignal_Disable;
//    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode 			= FSMC_ExtendedMode_Disable;
//    FSMC_NORSRAMInitStructure.FSMC_WriteBurst 				= FSMC_WriteBurst_Disable;
//    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct 	= &p;
//    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct 		= &p; 
//   
//    
//    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
//    
//    /* ʹ�� FSMC Bank1_SRAM Bank */
//    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

