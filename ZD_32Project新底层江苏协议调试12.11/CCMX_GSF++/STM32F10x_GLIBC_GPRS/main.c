/**
  ******************************************************************************
  * @file              : main.c
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
#ifndef MAIN_C
#define MAIN_C

#ifdef __cplusplus
extern "C" {
#endif
//}
/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "interface.h"
//! @}

#ifdef USE_STM32100B_EVAL
#include "stm32100b_eval_lcd.h"
#elif defined USE_STM3210B_EVAL
#include "stm3210b_eval_lcd.h"
#elif defined USE_STM3210E_EVAL
#include "stm3210e_eval_lcd.h"
#elif defined USE_STM3210C_EVAL
#include "stm3210c_eval_lcd.h"
#elif defined USE_STM32100E_EVAL
#include "stm32100e_eval_lcd.h"
#endif

/*============================ MACRO =========================================*/
#define GSM_GPRS_COMM_BUF_SIZE		512

// 用于测试目的的短信接收手机号码，一般为用户手机号码
#define TEST_PHONE_NUMBER			"18072868677" 		//"15824400095"

/*============================ TYPES =========================================*/

/*============================ LOCAL VARIABLES ===============================*/
USART_InitTypeDef USART_InitStructure;

//! 单位：毫秒（溢出周期为24天）
vu32			 	nSystemTimingSystick = 0;
vs32				TimingDelay;

char				TxBuffer[GSM_GPRS_COMM_BUF_SIZE];
char				RxBuffer[GSM_GPRS_COMM_BUF_SIZE];
char				SMSBuffer[GSM_GPRS_COMM_BUF_SIZE];
/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/
//!
//!
// "关闭 / 打开"
const char info_switch[2][7] = 
{
	"\xE5\x85\xB3\xE9\x97\xAD", 
	"\xE6\x89\x93\xE5\xBC\x80"
};
// "参数非法！"
const char info_invalid_argument[] = 
	"\xE5\x8F\x82\xE6\x95\xB0\xE9\x9D\x9E\xE6\xB3\x95\xEF\xBC\x81";
// "。"
const char info_fullstop[] = "\xE3\x80\x82";
// "，"
const char info_commma[] = "\xEF\xBC\x8C";
// "命令不支持"
const char info_unsupported_command[] = 
	"\xE5\x91\xBD\xE4\xBB\xA4\xE4\xB8\x8D\xE6\x94\xAF\xE6\x8C\x81\xEF\xBC\x81";
// "参数个数错误！"
const char info_incorrect_argc[] = 
	"\xE5\x8F\x82\xE6\x95\xB0\xE4\xB8\xAA\xE6\x95\xB0\xE9\x94\x99\xE8\xAF\xAF\xEF\xBC\x81";
// "命令格式错误！"
const char info_patern_err[] = 
	"\xE5\x91\xBD\xE4\xBB\xA4\xE6\xA0\xBC\xE5\xBC\x8F\xE9\x94\x99\xE8\xAF\xAF\xEF\xBC\x81";		
// "未授权！"
const char info_not_authorized[] = 
	"\xE6\x9C\xAA\xE6\x8E\x88\xE6\x9D\x83\xEF\xBC\x81";
// 请输入11位手机号码作为命令执行结果返回号码！
const char info_phonum_tip[] = 
	"\xE8\xAF\xB7\xE8\xBE\x93\xE5\x85\xA5\x31\x31\xE4\xBD\x8D\xE6\x89\x8B\xE6\x9C\xBA\xE5\x8F\xB7\xE7\xA0\x81\xE4\xBD\x9C\xE4\xB8\xBA\xE5\x91\xBD\xE4\xBB\xA4\xE6\x89\xA7\xE8\xA1\x8C\xE7\xBB\x93\xE6\x9E\x9C\xE8\xBF\x94\xE5\x9B\x9E\xE5\x8F\xB7\xE7\xA0\x81\xEF\xBC\x81";

//!
const char reply_sw_dev_led1[3][7]=
{
	"\xE7\xBA\xA2\xE8\x89\xB2\0",		// R: 红色
	"\xE7\xBB\xBF\xE8\x89\xB2\0",		// G: 绿色
	"\xE8\x93\x9D\xE8\x89\xB2\0"		// B: 蓝色
};
//!
const char reply_sw_dev_led2[2][10]=
{
	"\xE7\x86\x84\xE7\x81\xAD\xE3\x80\x82\0",		// 熄灭。
	"\xE7\x82\xB9\xE4\xBA\xAE\xE3\x80\x82\0",		// 点亮。
};

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*============================ EXTERN FUNCTIONS ==============================*/
/**
  * @brief  Description "软件延时函数"
  * @param  None
  * @retval None
  */
void sw_delay(int32_t time)
{
    while(time -- );
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
    if (TimingDelay > 0x00)
    {
        TimingDelay--;
    }
}

/**
  * @brief  Description "PWM GPIO Init"
  * @param  None
  * @retval None
  */
static  void  BSP_PWM_Init (void)
{
    GPIO_InitTypeDef  gpio_init;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio_init.GPIO_Pin   = BSP_GPIOF_PWM_GRP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &gpio_init);
}

/**
  * @brief  Description "任务函数"
  * @param  None
  * @retval None
  */
int CommTask(bool bReturn)
{
	gsm_env_init();
    return 0;
}

void gsm_main(void)
{
	int		TX_SEND = 50;
	int 	rssi = 1;
	bool 	state = false;
	
	int 	i;
	
	memset(TxBuffer, 0, GSM_GPRS_COMM_BUF_SIZE);
	srand(nSystemTimingSystick);
	for (i = 0; i < TX_SEND; i ++)
	{
		TxBuffer[i] = 0x16 + rand() % TX_SEND;
	}
	AI2C_Write(TxBuffer, 0, 8);
	sw_delay(0x3FFF);
	memset(RxBuffer, 0, GSM_GPRS_COMM_BUF_SIZE);
	AI2C_Read(RxBuffer, 0, 8);
	
	while (1)
	{
		memset(TxBuffer, 0, GSM_GPRS_COMM_BUF_SIZE);
		for (i = 0; i < TX_SEND*4; i ++)
		{
			TxBuffer[i] = 0x16 + i%TX_SEND;
		}
        // 查询RSSI
		rssi = gsm_get_signal();
		state = gsm_get_gprs_state();
        if(state == true && rssi > 5 && rssi < 32)
        {
            printf("RSSI = %d\r\n", rssi);
			gsm_gprs_comm(TxBuffer, TX_SEND*4, 
				RxBuffer, GSM_GPRS_COMM_BUF_SIZE);
        }
        else
        {
			printf("\n\r failed to get rssi{%d}.\r\n", rssi);
			printf("       file  %s\r\n", __FILE__);
			printf("       line  %d\r\n", __LINE__);
        }
	}
}

/**
  * @brief  Description "主函数"
  * @param  None
  * @retval None
  */
int main(void)
{
    /*!< At this stage the microcontroller clock setting is already configured,
         this is done through SystemInit() function which is called from startup
         file (startup_stm32f10x_xx.s) before to branch to application main.
         To reconfigure the default setting of SystemInit() function, refer to
         system_stm32f10x.c file
       */
    GPIO_InitTypeDef GPIOA_InitStructure;

    /* Initialize LEDs, Key Button, LCD and COM port(USART) available on
       STM3210X-EVAL board ******************************************************/
    STM_EVAL_LEDInit(LED1);
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);

    /* USARTx configured as follow:
          - BaudRate = 115200 baud
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Hardware flow control disabled (RTS and CTS signals)
          - Receive and transmit enabled
    * /
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    STM_EVAL_COMInit(COM1, &USART_InitStructure);
    */
    /* Turn on leds available on STM3210X-EVAL **********************************/
    STM_EVAL_LEDOn(LED1);
    STM_EVAL_LEDOn(LED2);
    STM_EVAL_LEDOn(LED3);
    STM_EVAL_LEDOn(LED4);

	// cpp initialization
    CommTask(true);

    printf("\r\n STM32F10x Firmware Library compiled with FULL ASSERT function... \n\r");
    printf("...Run-time checking enabled  \n\r");

    /* Simulate wrong parameter passed to library function ---------------------*/
    /* To enable SPI1 clock, RCC_APB2PeriphClockCmd function must be used and
       not RCC_APB1PeriphClockCmd */
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* Some member of GPIOA_InitStructure structure are not initialized */
    GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /* GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; */
    GPIO_Init(GPIOA, &GPIOA_InitStructure);

	// cpp main loop
    gsm_main();
    return 0;
}


/**
  * @brief  Description "秒中断函数"
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    nSystemTimingSystick ++;
    TimingDelay_Decrement();
//	LEDIO1.ToggleBits(BSP_LED_CH1);
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	DebugOutChar(ch);
    return ch;
}

#ifdef __cplusplus
}
#endif

#endif /* MAIN_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

