/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.cpp
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include "stm32glibc_rcc.h"
#include "stm32glibc_nvic.h"
#include "stm32glibc_systick.h"
#include "stm32glibc_tim.h"
#include "stm32glibc_afio.h"
#include "stm32glibc_exti.h"
#include "stm32glibc_gpio.h"
#include "stm32glibc_usart.h"
#include "stm32glibc_trace.h"
#include "stm32glibc_stream.h"
#include "app_gsm.h"
#include "dev_gsm.h"

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


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define APP_DEBUG_CUR				DEBUG_ALL
#define APP_DEBUG_SIZE				0x200
#define GSM_SMBUF_SIZE				160
/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef 					MCU_Clocks;
//! 周期：微秒
uint32_t			 				nPeriod_MicroSeconds;
//!
uint8_t								LED_Pin0 = GPIO_Pin_0;
//!
char 								str[APP_DEBUG_SIZE];
//!
char 								SMS[GSM_SMBUF_SIZE];
//!
DEV_GSM_Struct 						DeviceGSM;
RTC8025_Typedef						DeviceRTC;

/* Private functions ---------------------------------------------------------*/
using namespace device;
using namespace periph_c;
using namespace mcucore;

/**
  * @brief  Description "软件延时函数"
  * @param  None
  * @retval None
  */



/**
  * @brief  Description "Debug 初始化"
  * @param  None
  * @retval None
  */
void Debug_Configuration(void)
{
    union mcu_info chip_info;
#if (DEBUG_OUTPUT_EN == 1)
#if (DEBUG_OUTPUT_USART1_EN == 1)

#else
    MCUTRACE.Init();
#endif
#endif
    MCUDBG.GetDebugMcuInfo(chip_info);
    MCUDBG.DebugMcuPeriph(DBGMCU_DBG_TIM7_STOP);
    DebugOutPrintf(DEBUG_NED, "\r\n DebugMCUInfo\tDEV:%X,REV:%X.",
                   chip_info.st.DEV_ID, chip_info.st.REV_ID);
    DebugOutPrintf(DEBUG_NED, "\r\n DeviceInfo\tFlash:%dK,UID0:%X,UID1:%X,UID2:%X.\r\n",
                   MCUINFO.F_SIZE,
                   MCUINFO.U_ID0, MCUINFO.U_ID1, MCUINFO.U_ID2);
}

/**
  * @brief  Description "RCC 时钟初始化"
  * @param  None
  * @retval None
  */
void Clock_Configuration(void)
{
    CLOCK.GetClocksFreq(MCU_Clocks);
    //! Timer
    CLOCK.APB1PeriphClockCmd(RCC_APB1Periph_TIM7, 	ENABLE);
    //! GPIO
//    CLOCK.APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  	ENABLE);
    CLOCK.APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, 	ENABLE);
    CLOCK.APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, 	ENABLE);
    CLOCK.APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, 	ENABLE);
    CLOCK.APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, 	ENABLE);
    CLOCK.APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, 	ENABLE);
    //! USART
    CLOCK.APB2PeriphClockCmd(RCC_APB2Periph_USART1,	ENABLE);
    CLOCK.APB1PeriphClockCmd(RCC_APB1Periph_USART3,	ENABLE);
    // 周期：微秒
//    nPeriod_MicroSeconds = 1000;
//    SYSTICK.Config(MCU_Clocks.SYSCLK_Frequency / nPeriod_MicroSeconds, SYSTICK_CLKSRC_HCLK_Div8);
}

/**
  * @brief  Description "NVIC 中断优先级初始化"
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    MCUSCB.SetPriorityGrouping(mcucore::GRP4_SUB4);
    MCUNVIC.IRQConfig(mcucore::USART1_IRQn);
    MCUNVIC.IRQConfig(mcucore::USART3_IRQn);
    MCUNVIC.IRQConfig(mcucore::TIM7_IRQn);
//    MCUNVIC.IRQConfig(mcucore::EXTI9_5_IRQn);
}

/**
  * @brief  Description "USART 初始化"
  * @param  None
  * @retval None
  */
void USART_Configuration(uint32_t uBaudrate)
{
    //!
//    COM1.Config(uBaudrate);
//    COM1.InterruptConfig(RXNEIT);
    //!
    COM3.Config(uBaudrate);
    COM3.InterruptConfig(RXNEIT);
}

/**
  * @brief  Description "GPIO 初始化"
  * @param  None
  * @retval None
  */
void EXTI_Configuration(void)
{
    //!
    IOAF.EXTILineConfig(BSP_PORTSRC_RING, BSP_PPINSRC_RING);

    IOEXTI.FallingTrigger(BSP_EXTILINE_RING);
    IOEXTI.Interrupt(BSP_EXTILINE_RING);
    IOEXTI.ClearPending(BSP_EXTILINE_RING);
    //	IOEXTI.GenerateSWInterrupt(BSP_EXTILINE_RING);
}

/**
  * @brief  Description "GPIO 初始化"
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
//    int GPIO_Pin = GPIO_Pin_All;

//    GPIO_Pin = GPIO_Pin_All & (~(GPIO_Pin_13 | GPIO_Pin_14));
//    IOPA.Config(GPIO_Pin);

//    GPIO_Pin = GPIO_Pin_All & (~(GPIO_Pin_3));
//    IOPB.Config(GPIO_Pin);

//    GPIO_Pin = GPIO_Pin_All;
//    IOPC.Config(GPIO_Pin);
//    IOPD.Config(GPIO_Pin);
//    IOPE.Config(GPIO_Pin);

//    IOAF.PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable);

//    BEEPIO.Config(BSP_BEEP_CH0, GPIO_MODE_OUT_PP);
//    LEDIO1.Config(BSP_LED_CH1,  GPIO_MODE_OUT_PP);
//    LEDIO2.Config(BSP_LED_CH2,  GPIO_MODE_OUT_PP);
//    LEDIO3.Config(BSP_LED_CH3,  GPIO_MODE_OUT_PP);
//    LEDIO4.Config(BSP_LED_CH4,  GPIO_MODE_OUT_PP);
//    LEDIO5.Config(BSP_LED_CH5,  GPIO_MODE_OUT_PP);
    //!
//    COM1IO.Config(BSP_COM1_TX, GPIO_MODE_AF_PP);
//    COM1IO.Config(BSP_COM1_RX, GPIO_MODE_IN_FLOATING);
    //!
    COM3IO.Config(BSP_COM3_TX, GPIO_MODE_AF_PP);
    COM3IO.Config(BSP_COM3_RX, GPIO_MODE_IN_FLOATING);
    //!
    I2CIO.Config(BSP_I2C_SCL, GPIO_MODE_OUT_OD);
    I2CIO.Config(BSP_I2C_SDA, GPIO_MODE_OUT_OD);
    I2CIO.SetBits(BSP_I2C_SCL);
    I2CIO.SetBits(BSP_I2C_SDA);
}


/**
  * @brief  Description "GSM 库函数环境初始化"
  * @param  None
  * @retval None
  */
bool gsm_env_init(GPRS_Connection* ip_com,_SCSStruct * state)
{
    int		uBaudrate = 9600;
    int 	err = 0;
    int 	ret;

    // TODO: Configuration
    Clock_Configuration();
    GPIO_Configuration();
    Debug_Configuration();
    // TODO: periph init
    USART_Configuration(uBaudrate);
//    EXTI_Configuration();
    NVIC_Configuration();
    /* Retarget the C library printf function to the USARTx, can be USART1 or USART2
       depending on the EVAL board you are using ********************************/
	
    ret = DeviceGSM.DEV_GSM_Init(&Delay, &GetSysTick, uBaudrate, err ,(T_GPRS_CONNECTION*)ip_com,state);
    if(ret == 1)
	{
		/* 模块 配置 成功 */
		return true;
	}else
	{
		return false;
	}
}

/**
  * @brief  Description "GSM库函数配置数据服务器网络"
  * @param  None
  * @retval None
  */
void GSM_Config_Data_Network(void * ip_com)
{
	DeviceGSM.DEV_GSM_Config_Data_Network((T_GPRS_CONNECTION*)ip_com);
}


/**
  * @brief  Description "获得GSM的信号强度"
  * @param  None
  * @retval int			GSM的信号强度(5~32为正常数值)
  */
int gsm_get_signal(void)
{
    int rssi;
    bool ret;
    ret = DeviceGSM.GSM_GetRSSI((int *) &rssi);
    if (ret)
    {
        return rssi;
    }
    return -1;
}

/**
  * @brief  Description "获得GSM的GPRS的附着状态"
  * @param  None
  * @retval bool		GPRS通讯是否正常
  */
int gsm_get_gprs_state(void)
{
    bool ret;
    ret = DeviceGSM.GSM_CheckGPRSActive();
    return ret;
}

/**
  * @brief  Description "GPRS通讯"
  * @param  wbuf		发送缓冲区
  * @param  wlen		发送缓冲区字节数大小
  * @param  rbuf		接收缓冲区
  * @param  rlen		接收缓冲区字节数大小
  * @retval bool		GPRS通讯是否成功
  */
bool gsm_gprs_comm(char *wbuf, int wlen, char *rbuf, int* rlen)
{
    // "未授权！"
    const char info_not_authorized[] =
        "\xE6\x9C\xAA\xE6\x8E\x88\xE6\x9D\x83\xEF\xBC\x81";
    static int tx_size = 0, rx_size = 0, errors = 0, delay_time = 5000, times = 0;
    bool ret;
    int rssi = 1;
    int err = 0;
    do
    {
        // 查询RSSI
        rssi = gsm_get_signal();
        // GPRS通讯
        ret = DeviceGSM.GSM_ConnnectServerExample(wbuf, wlen, rbuf, *rlen, err,GSM_GPRS_MODE_TCP,0);
        times++;
        tx_size += wlen;
        rx_size += *rlen;
        if (!ret)
        {
            DebugOutPrintf(DEBUG_ERR, "GSM_Example:Err<%d>,ECode[%d]\r\n", errors++, err);
//            LEDIO1.SetBits(BSP_LED_CH1);
            delay_time = 60000;
            goto app_gsm_comm_rtn;
        }
//        LEDIO1.ResetBits(BSP_LED_CH1);
        delay_time = (5000);
    }while (0);
		
app_gsm_comm_rtn:
#if 0
    RTC8025_Read(DeviceRTC.REGS, 0, SYS_RTC_8025_BUF_SIZE);
    printf("RTC8025_Read:\r\n");
    for (i = 0; i < SYS_RTC_8025_BUF_SIZE; i ++)
    {
        printf("%02X,", DeviceRTC.REGS[i]);
    }
    printf("\r\n");
    printf("YYYY-MM-DD HH:MM:SS W\r\n");
	DebugOutPrintf(DEBUG_INF, "20%02X-%02X-%02X %02X:%02X:%02X %01X\r\n",
           DeviceRTC.STREG.Years,
           DeviceRTC.STREG.Months,
           DeviceRTC.STREG.Days,
           DeviceRTC.STREG.Hours,
           DeviceRTC.STREG.Minutes,
           DeviceRTC.STREG.Seconds,
           DeviceRTC.STREG.Weekdays);
    while ('\0' != *ptr)
	{
		COM1.PutChar(*ptr++);
	}
#endif
    if (!ret || rssi < 10)
    {
        sprintf(SMS, "TIME:%s; TIM:(%d/%d); ERR:%d; RSSI:%d; Tx:%d; Rx:%d;",
                str, errors, times, err, rssi, tx_size, rx_size);
        strcat(SMS, info_not_authorized);
        // 发送错误短信
        //gsm_send_sms((char *)TEST_PHONE_NUMBER, SMS);
    }
    memset(SMS, 0, GSM_SMBUF_SIZE);
    // 接收短信
    //	do {
    //		ret = DeviceGSM.GSM_RecvSMS(SMS, GSM_SMBUF_SIZE, err);
    //		if (ret == false)
    //			break;
    //		DebugOutPrintf(DEBUG_INF, SMS);
    //	} while (ret);
	
    DebugOutPrintf(DEBUG_ERR, "GSM_Example:\t<%s>Err(%d)\r\n\t\tTransmitBytes(%d)\r\n\t\tReceiveBytes (%d)\r\n",
                   ret ? "true" : "false",
                   errors, tx_size, rx_size);
    Delay(delay_time / 1000);
    return ret;
}

bool gsm_gprs_config(char *wbuf, int wlen, char *rbuf, int* rlen)
{
    // "未授权！"
    const char info_not_authorized[] =
        "\xE6\x9C\xAA\xE6\x8E\x88\xE6\x9D\x83\xEF\xBC\x81";
    static int tx_size = 0, rx_size = 0, errors = 0, delay_time = 5000, times = 0;
    bool ret;
    int rssi = 1;
    int err = 0;
//	int i;
//	char *ptr = str;
    do
    {
        // 查询RSSI
        rssi = gsm_get_signal();
        // GPRS通讯
        ret = DeviceGSM.GSM_ConnnectServerExample(wbuf, wlen, rbuf, *rlen, err,GSM_GPRS_MODE_TCP,1);
        times++;
        tx_size += wlen;
        rx_size += *rlen;
        if (!ret)
        {
            DebugOutPrintf(DEBUG_ERR, "GSM_Example:Err<%d>,ECode[%d]\r\n", errors++, err);
//            LEDIO1.SetBits(BSP_LED_CH1);
            delay_time = 60000;
            goto app_gsm_comm_rtn;
        }
//        LEDIO1.ResetBits(BSP_LED_CH1);
        delay_time = (5000);
    }
    while (0);
app_gsm_comm_rtn:
#if 0
    RTC8025_Read(DeviceRTC.REGS, 0, SYS_RTC_8025_BUF_SIZE);
    printf("RTC8025_Read:\r\n");
    for (i = 0; i < SYS_RTC_8025_BUF_SIZE; i ++)
    {
        printf("%02X,", DeviceRTC.REGS[i]);
    }
    printf("\r\n");
    printf("YYYY-MM-DD HH:MM:SS W\r\n");
	DebugOutPrintf(DEBUG_INF, "20%02X-%02X-%02X %02X:%02X:%02X %01X\r\n",
           DeviceRTC.STREG.Years,
           DeviceRTC.STREG.Months,
           DeviceRTC.STREG.Days,
           DeviceRTC.STREG.Hours,
           DeviceRTC.STREG.Minutes,
           DeviceRTC.STREG.Seconds,
           DeviceRTC.STREG.Weekdays);
    while ('\0' != *ptr)
	{
		COM1.PutChar(*ptr++);
	}
#endif
    if (!ret || rssi < 10)
    {
        sprintf(SMS, "TIME:%s; TIM:(%d/%d); ERR:%d; RSSI:%d; Tx:%d; Rx:%d;",
                str, errors, times, err, rssi, tx_size, rx_size);
        strcat(SMS, info_not_authorized);
        // 发送错误短信
        //gsm_send_sms((char *)TEST_PHONE_NUMBER, SMS);
    }
    memset(SMS, 0, GSM_SMBUF_SIZE);
    // 接收短信
    //	do {
    //		ret = DeviceGSM.GSM_RecvSMS(SMS, GSM_SMBUF_SIZE, err);
    //		if (ret == false)
    //			break;
    //		DebugOutPrintf(DEBUG_INF, SMS);
    //	} while (ret);
	
    DebugOutPrintf(DEBUG_ERR, "GSM_Example:\t<%s>Err(%d)\r\n\t\tTransmitBytes(%d)\r\n\t\tReceiveBytes (%d)\r\n",
                   ret ? "true" : "false",
                   errors, tx_size, rx_size);
    Delay(delay_time / 1000);
    return ret;
}

/**
  * @brief  Description "GPRS通讯"
  * @param  wbuf		发送缓冲区
  * @param  wlen		发送缓冲区字节数大小
  * @param  rbuf		接收缓冲区
  * @param  rlen		接收缓冲区字节数大小
  * @retval bool		GPRS通讯是否成功
  */
bool gsm_gprs_update(char *wbuf, int wlen, char *rbuf, int* rlen)
{
    static int delay_time = 5000;
    bool ret;
//    int rssi = 1;
    int err = 0;
    do
    {
        // 查询RSSI
		    gsm_get_signal();
        // GPRS通讯
        ret = DeviceGSM.GSM_ConnnectServerExample(wbuf, wlen, rbuf, *rlen, err,GSM_GPRS_MODE_TCP,2);

        if (!ret)
        {
            delay_time = 60000;
            goto app_gsm_update_rtn;
        }
        delay_time = (5000);
    }
    while (0);
app_gsm_update_rtn:

    Delay(delay_time / 1000);
    return ret;
}


/**
  * @brief  Description "GSM发送短信"
  * @param  pn			发送手机号码
  * @param  ascii_utf8	发送缓冲区
  * @retval bool		GSM发送短信是否成功
  */
bool gsm_send_sms(char *pn, char *ascii_utf8)
{
    bool ret;
    int err = 0;
    do
    {
        ret = DeviceGSM.GSM_SendSMS(pn, ascii_utf8, err);
    }
    while (0);
		
    return ret;
		
		
}

/**
  * @brief  Description "GSM接收短信"
  * @param  pn			接收手机号码
  * @param  ascii_utf8	接收缓冲区
  * @retval bool		GSM接收短信是否成功
  */
bool gsm_recv_sms(char *pn, RTC_Times *time, char *ascii_utf8, int *len)
{
    bool ret;
	char time_buf[32];
	char buf[16];
    int err = 0, argc, buf_size;
	
#define 			ARGS_MAX_COUNT 8
	const char 		sepm[] = "/-,: ";
	char*			argv[ARGS_MAX_COUNT];
	
    do
    {
        ret = DeviceGSM.GSM_RecvSMS(pn, time_buf, ascii_utf8, *len, err);
        if (ret == false)
            break;
        DebugOutPrintf(DEBUG_INF, "TeleNum:\t%s\r\n", pn);
        DebugOutPrintf(DEBUG_INF, "SMSTime:\t%s\r\n", time_buf);
        DebugOutPrintf(DEBUG_INF, "SMSCont:\t%s\r\n", ascii_utf8);
		do {
			buf_size = __strlen(ascii_utf8);
			argc = split_str((char *)ascii_utf8, buf_size, ".#", argv, ARGS_MAX_COUNT);
			if (argc != 2)
			{
				return false;
			}
			__trim_sep(buf, __toUpper(argv[0]), " .#");
			if (__strcmp(buf, "SET") == 0)
			{
				__trim_sep(buf, __toUpper(argv[1]), " .#");
				if (__strcmp(buf, "DATETIME") == 0)
				{
					buf_size = __strlen(time_buf);
					argc = split_str((char *)time_buf, buf_size, sepm, argv, ARGS_MAX_COUNT);
					if (argc == 6)
					{
						time->Year = __atoi(argv[0]);
						time->Month = __atoi(argv[1]);
						time->Day = __atoi(argv[2]);
						time->Hour = __atoi(argv[3]);
						time->Min = __atoi(argv[4]);
						time->Sec = __atoi(argv[5]);
						time->Zone = __atoi(&argv[5][2]);
						time->Week = __calc_weekday(time->Year, time->Month, time->Day);
						DebugOutPrintf(DEBUG_INF, "SETTING:%04d-%02d-%02d %02d:%02d:%02d %01d\r\n",
							   time->Year,
							   time->Month,
							   time->Day,
							   time->Hour,
							   time->Min,
							   time->Sec,
							   time->Week);
						return true;
					}
				}
			}
		} while (0);
    }
    while (0);
//	DebugOutPrintf(DEBUG_INF, "gsm_recv_sms over!\r\n");
    return false;
}

#if (0)

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int cpp_main(void)
{
    char	message1[25] = "AAAAAA>BBBB:>GPRS TEST\r\n";							// 定位包
    char	message2[] = "user AAAAAA pass 12345 vers GPRS 0425 Filter r/0 \r\n";	// 登录包

    int err = 0, errors = 0;
    int rx_len = 25;
    using periph_c::USARTx_PUTCHAR;
	
	 T_GPRS_CONNECTION ip_com;
    //	USARTx_PUTCHAR::TimoutInit(15000);
    //	USARTx_PUTCHAR::TimoutEnable();
    DeviceGSM.DEV_GSM_Init(&Delay, &GetSysTick, 9600, err ,&ip_com);
    /* Infinite loop */
    while (0)
    {
        //		output 	<< "\r\n请输入一个单词吧 :";
        //		output.Flush();
        //		input  	>> str;
        //		output	<< "\r\n你输入的单词是：" << str << "\r\n";
        err = 0;
        DeviceGSM.GSM_ConnnectServerExample(message1, 25, message2, rx_len, err);
        if (err < 0)
        {
            DebugOutPrintf(DEBUG_ERR, "GSM_Example:Err<%d>,ECode[%d]\r\n", errors++, err);
        }
        Delay(5000);
        str[0] = '\0';
//        LEDIO1.ToggleBits(BSP_LED_CH1);
    }
    while (0)
    {
        //		input  	>> str;
        DebugOutPrintf(DEBUG_NED, "\r\n你输入的单词是：[%d]%s\r\n", __strlen(str), str);
        Delay(5);
        str[0] = '\0';
//        LEDIO1.ToggleBits(BSP_LED_CH1);
    }
    DeviceGSM.DEV_GSM_Exit(true);
    return 0;
}

#endif

/**
  * @brief  Description "调试信息输出--字符输出函数"
  * @param  ch			输出字符
  * @retval None
  */
void DebugOutChar(int ch)
{
#if (DEBUG_OUTPUT_EN == 1)
#if (DEBUG_OUTPUT_USART1_EN == 1)
    COM1.PutChar(ch);
#else
    MCUTRACE.PutChar(ch);
#endif
#endif
}

/**
  * @brief  Description "调试信息输出--字符串输出函数"
  * @param  ch			输出字符
  * @retval None
  */
finline void DebugOutString(const char *string, int len)
{
    do
    {
#if (DEBUG_OUTPUT_EN == 1)
#if (DEBUG_OUTPUT_USART1_EN == 1)
        COM1.PutChar(*string);
#else
        MCUTRACE.PutChar(*string);
#endif
#endif
    }
    while (*string++ != '\0');
}

/**
  * @brief  Description "调试信息输出--字符串输出函数"
  * @param  ch			输出字符
  * @retval None
  */
int DebugOutPrintf(enum debug_degrees deg, const char *format, ...)
{
    int done = 0;
    char *ptr;
    ptr = str;
    if(deg & (APP_DEBUG_CUR - 1))
    {
        if (IS_FORMAT_STRING(format))
        {
            va_list arg;
            va_start(arg, format);
            done = vsprintf(ptr, format, arg);
            done = done >= APP_DEBUG_SIZE ? APP_DEBUG_SIZE - 1 : done;
            str[done] = '\0';
            va_end(arg);
            DebugOutString(ptr, done);
        }
        else
        {
            done = __strlen(format);
            DebugOutString(format, done);
        }
    }
    return done;
}


/**
  * @brief  Description "RING中断函数"
  * @param  None
  * @retval None
  */
ARMAPI void EXTI9_5_IRQHandler(void)
{
    if (IOEXTI.ReadPending(BSP_EXTILINE_RING))
    {
        // gsm模块的ring中断事件是否待处理
        DeviceGSM.is_gsmring_pending = true;
        // GSM模块RING中断产生的次数(GSM模块上电后会自动产生一个RING中断)
        DeviceGSM.cnt_gsmring_asserted ++;

        COM1.WriteByte('*');
        IOEXTI.ClearPending(BSP_EXTILINE_RING);
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number */

    DebugOutPrintf(DEBUG_ERR, "\n\r Wrong parameter value detected on\r\n");
    DebugOutPrintf(DEBUG_ERR, "       file  %s\r\n", file);
    DebugOutPrintf(DEBUG_ERR, "       line  %d\r\n", line);

    /* Infinite loop */
    while (0)
    {
    }
}

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
