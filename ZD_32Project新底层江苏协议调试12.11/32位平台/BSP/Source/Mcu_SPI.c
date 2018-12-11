/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/09/15																			 */
/*************************************************************************************************/
/*************************************************************************************************
 * 硬件连接 ----------------------------
 *         | PB12-SPI2-NSS  : W25X16-CS  |
 *         | PB13-SPI2-SCK  : W25X16-CLK |
 *         | PB14-SPI2-MISO : W25X16-DO  |
 *         | PB15-SPI2-MOSI : W25X16-DIO |
 *         | PD2 			: W25X16-WP  |
 *         ----------------------------
 * 库版本  ：ST3.5.0
 * 说明    ：在本程序中Flash操作都是 页读 页写 的  
**************************************************************************************************/
#include "Mcu_SPI.h"
/************************************************************************************************/
/* 函数名	: Mcu_SPI1_Init																	 	*/
/* 输入		: 无 																				*/
/* 输出		: 无 																				*/
/* 作用		: SPI1端口配置													 					*/
/* 创建日期	: 2015/04/10																		*/
/* 修改日期	: 2015/11/11																		*/
/************************************************************************************************/
void Mcu_SPI1_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable SPI1 and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(SPI_WP_GPIO_CLK, ENABLE);

  /* SPI2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
 
  
  /*!< Configure SPI_FLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  /*!< Configure  W25X16-WP */
  GPIO_InitStructure.GPIO_Pin = SPI_WP_GPIO_PIN;
  GPIO_Init(SPI_WP_GPIO_PORT, &GPIO_InitStructure);
  GPIO_ResetBits( SPI_WP_GPIO_PORT,  SPI_WP_GPIO_PIN );

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* SPI1 configuration */
  // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI2, ENABLE);
}
 

/******************************END OF FILE*****************************/
