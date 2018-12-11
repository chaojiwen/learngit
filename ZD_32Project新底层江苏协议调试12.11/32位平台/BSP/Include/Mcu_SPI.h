#ifndef __Mcu_SPI_H
#define __Mcu_SPI_H
#include "stm32f10x.h"
#if (1)
/********************************* 按键 端口定义 *********************************/
#define SPI_WP_GPIO_PORT		(GPIOD)
#define SPI_WP_GPIO_CLK			(RCC_APB2Periph_GPIOD)
#define SPI_WP_GPIO_PIN			(GPIO_Pin_2)
/* 外部Fram写保护引脚 */
#define SPI_WP_GPIO_Enable()	{GPIO_SetBits(SPI_WP_GPIO_PORT,SPI_WP_GPIO_PIN);}
#define SPI_WP_GPIO_DISABLE()	{GPIO_ResetBits(SPI_WP_GPIO_PORT,SPI_WP_GPIO_PIN);}
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)
#endif
void Mcu_SPI1_Init(void);
#endif /* __SPI_FLASH_H */

