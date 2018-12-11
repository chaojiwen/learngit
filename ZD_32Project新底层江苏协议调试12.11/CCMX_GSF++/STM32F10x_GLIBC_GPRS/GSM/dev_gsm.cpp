/**
  ******************************************************************************
  * @file              : dev_gsm.c
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
#ifndef DEV_GSM_C
#define DEV_GSM_C

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32glibc_gpio.h"
#include "stm32glibc_tim.h"
#include "stm32glibc_usart.h"
#include "stm32glibc_stream.h"
#include "dev_gsm.h"
#include "sms_utils.h"
//! @}

/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<e>开启串口接收
//	<i>开启/关闭串口接收功能
#define USART_RECEIVE_EN		1
//	</e>
//
//	<e>使用input/output文本输入输出流
//	<i>不使用输出流的话就无法使用输入流了。
#define	OUTPUT_EN				1
//	<o>文本输出流发送缓冲区大小
#define TEXT_OUT_BUFF_SIZE		0x0200
//		<e>使用input文本输入流
//		<i>使用input文本输入流会占用一定的内存，
//		<i>注意input输入流的缓冲区需要配合其它模块一起使用
//		<i>如果这里不使用input输入流，那么其它模块就无法使用输入缓冲区
#define INPUT_EN				1
//		<o>文本输入流接收缓冲区大小
#define TEXT_IN_BUFF_SIZE		0x0200
//		</e>
//	</e>
//
//	<e>开启调试串口跟踪输出
//	<i>开启/关闭串口1调试输出
#define USART_DEBUG_EN			1
//	</e>
//
//	<e>开启GSM调试跟踪输出
//	<i>开启/关闭GSM调试输出
#define GSM_DEBUG_EN			1
//	</e>
//
//	<e>开启GSM错误跟踪输出
//	<i>开启/关闭GSM错误输出
#define GSM_ERROR_EN			1
//	</e>
//
//	<e>GSM SMS 初始化
//	<i>开启/关闭GSM SMS 初始化
#define GSM_SMS_EN				0
//	</e>
//
//	<e>GSM WAKEUP/SLEEP 使能
//	<i>开启/关闭GSM 休眠和唤醒功能
#define GSM_SLEEP_EN			0
//	</e>
//
//	<<< end of configuration section >>>

/*============================ TYPES =========================================*/

/*============================ LOCAL VARIABLES ===============================*/


/*============================ GLOBAL VARIABLES ==============================*/


/*============================ EXTERN FUNCTIONS ==============================*/
//#ifdef __cplusplus
//}
//#endif

namespace device
{
    using namespace periph_c;

#if(OUTPUT_EN != 0)
    char TextOutBuffer[TEXT_OUT_BUFF_SIZE];
    FIFO<char> textOutFIFO(TextOutBuffer, TEXT_OUT_BUFF_SIZE);
    TextOutStream<periph_c::USARTx_PUTCHAR> output(textOutFIFO);

    //!
#if(INPUT_EN != 0)
    char TextInBuffer[TEXT_IN_BUFF_SIZE];
    char CmdBuffer[TEXT_IN_BUFF_SIZE];
    FIFO<char> textInFIFO( TextInBuffer, TEXT_IN_BUFF_SIZE);
    TextInStream<periph_c::USARTx_PUTCHAR> input(textInFIFO, output);
#endif
#endif

    /**
      * @brief  Description "初始化"
      * @param  None
      * @retval None
      */
    bool 	DEV_GSM_Struct::DEV_GSM_Init(Func_Delay func1, Func_Systick func2,
                                         int baudrate, int &err)
    {
        int 	pos;			// 字符串定位
        int 	ret, times;
        int		rssi = -1;
        char	str[64 + 1];
        __SMS_READY = 0;
        __DEV_Delay = func1;
        __DEV_SysTick = func2;
        __DEV_StartupTime = DEV_GSM_Timing();

        sts_gsm_power                     	= false;
        // 初始化gsm硬件前将Global.is_gsm_ready设置为FALSE
        is_gsm_ready                      	= false;
        // 初始化gsm硬件前将Global.is_gsm_calling设置为FALSE
        is_gsm_calling                    	= false;
        is_gsmring_pending 				  	= false;
        cnt_gsmring_asserted 				= 0;

        // 初始化dtmf命令相关变量
        sts_dtmf_command 					= STS_DTMF_COMMAND_CALL_WAITING;
        times_input_dtmf_password 			= 0;
        is_dtmf_detection_enabled 			= false;

        cnt_gsm_recovered 					= 0;

        __strcpy(de_gsm_pnin, "");
        __strcpy(de_gsm_dtmf, "");
        __strcpy(gsm_sca, "");

        // GPRS网络参数
        __strcpy(gsm_apn,                 	(char *)(GPRS_APN));
        // GPRS/GSM运行参数
        __strcpy(gsm_telecode,            	(char *)(GSM_TELECODE));
        // 地图服务器的连接参数(TBD)
        __strcpy(tcp_conn[GPRS_ID0].dn,   	(char *)(GPRS_ID0_DN));
        __strcpy(tcp_conn[GPRS_ID0].ip,   	(char *)(GPRS_ID0_IP));
        __strcpy(tcp_conn[GPRS_ID0].port, 	(char *)(GPRS_ID0_PORT));

        // 应用服务器的连接参数
        __strcpy(tcp_conn[GPRS_ID1].dn,   	(char *)(GPRS_ID1_DN));
        __strcpy(tcp_conn[GPRS_ID1].ip,   	(char *)(GPRS_ID1_IP));
        __strcpy(tcp_conn[GPRS_ID1].port, 	(char *)(GPRS_ID1_PORT));

        // 星历服务器的连接参数
        __strcpy(tcp_conn[GPRS_ID2].ip,   	(char *)(GPRS_ID2_IP));
        __strcpy(tcp_conn[GPRS_ID2].port, 	(char *)(GPRS_ID2_PORT));

        // 设置默认的授权操作密码
        __strcpy(cmd_pwd_sms, 				DEF_CMD_PWD_SMS);
        // 恢复DTMF操作密码
        // 系统中会统一手机号码为包含国家代码前缀的格式，
        // 因此保存在Flash中的默认手机号码都统一包含国家代码前缀
        __strcpy(dtmf_password, 			DEF_CMD_PWD_DTMF);

        input.Init(1000);
        GSMPWRCTRL.Config(BSP_GSM_PWRCTRL, GPIO_MODE_OUT_PP);
        GSMONOFF.Config(BSP_GSM_ONOFF, GPIO_MODE_OUT_PP);
        GSMWAKEUP.Config(BSP_GSM_WAKEUP, GPIO_MODE_OUT_PP);
        GSMWAKEUP.SetBits(BSP_GSM_WAKEUP);

        DebugOutPrintf(DEBUG_INF, "##<%d>to initialize GSM module.\r\n", DEV_GSM_Timing());

        // 先强行关断GSM电源，以便在系统异常重启而未能在关机前关断GSM电源时清除GSM工作状态
        DEV_GSM_PowerDown();

        // 打开GSM电源
        DEV_GSM_PowerUp();

        // 给GSM模块上电
        DEV_GSM_OnOff();

        // 延时3秒等待GSM硬件初始化
        DEV_GSM_Delay(1000);

        // 等待GSM模块搜网、注册
        DebugOutPrintf(DEBUG_INF, "##<%d>to wait for GSM registration...\r\n", DEV_GSM_Timing());
        // 不管SIM卡是否插入，模块上电后都会输出+EIND特征字符串，
        // 以此检测模块是否基本正常(M660+模块的注册网络时间一般固定为10秒)。
//		if(DEV_FindPattern("+CPIN:", TO_GPRS_FIND_MODULE, pos, err) == false)
        if(DEV_FindPattern("+CPIN: READY", TO_GPRS_FIND_MODULE, pos, err) == false)
        {
            DebugOutPrintf(DEBUG_ERR, "+CPIN: READY not found.\r\n");

            DEV_GSM_Exit();

            err = ER_GSM_INIT_REGISTRATION;		// 网络注册异常
            goto dev_gsm_init_err;
        }

        // 检查模块波特率
        if(GSM_CheckBaudRate(baudrate) == false)
        {
            DebugOutPrintf(DEBUG_ERR, "Module BaudRate error.\r\n");

            DEV_GSM_Exit();

            err = ER_GSM_UART_RECV_CHAOS;		// 模块波特率
            goto dev_gsm_init_err;
        }


        // 等待GSM模块搜网、注册
        DebugOutPrintf(DEBUG_INF, "##<%d>to detect SIM card...\r\n", DEV_GSM_Timing());

        // 检查SIM卡是否插上
        if(GSM_CheckSIM() == false)
        {
            DebugOutPrintf(DEBUG_ERR, "SIM card not detected.\r\n");

            DEV_GSM_Exit();

            err = ER_GSM_INIT_SIM;		// sim卡未检测到
            goto dev_gsm_init_err;
        }


        // 查询GSM固件版本
        DebugOutPrintf(DEBUG_INF, "##<%d>to get for gsm firmware version...\r\n", DEV_GSM_Timing());
        if(GSM_GetSWVersion(str, 64) == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to get gsm firmware version.\r\n");

            DEV_GSM_Exit();

            err = ER_GSM_INIT_SIM;		// sim卡未检测到
            goto dev_gsm_init_err;
        }


        // 查询IMEI串码
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to get GSM IMEI...\r\n", DEV_GSM_Timing());
            // 查询IMEI串(GSM模块刚上电后查询IMEI经常失败，原因不详)
            ret = GSM_GetIMEI((char *)gsm_imei, STD_LEN_GSM_IMEI);
            if(ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to get GSM IMEI.\r\n");
            }
            else
            {
                // 检查imei长度是否为15位
                if(__strlen(gsm_imei) != 15)
                {
                    DebugOutPrintf(DEBUG_WRN, "length of imei is not 15!\r\n");
                    err = ER_GSM_INIT_IMEI2;
                    break;
                }
                else
                {
                    DebugOutPrintf(DEBUG_INF, "GSM IMEI: %s\r\n", gsm_imei);
                    break;
                }
            }
            DEV_GSM_Delay(1000);
        }
        while (times <= 5);
        if (!ret)
        {
            DEV_GSM_Exit();
            err = ER_GSM_INIT_IMEI1;
            goto dev_gsm_init_err;
        }


        // 查询GSM 时钟管理
        DebugOutPrintf(DEBUG_INF, "##<%d>to get cclk.\r\n", DEV_GSM_Timing());
        if(GSM_GetCCLK((char *)str, 64) == true)
        {
            DebugOutPrintf(DEBUG_INF, "GSM CCLK: %s.\r\n", str);
        }
        else
        {
            DebugOutPrintf(DEBUG_WRN, "failed to get GSM CCLK.\r\n");
        }


        // Call环境初始化
        DebugOutPrintf(DEBUG_INF, "##<%d>to enable phone number\r\n", DEV_GSM_Timing());

        // 使能来电号码自动输出
        if(DEV_SendAT("AT+CLIP=1\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
        {
            DEV_GSM_Exit();
            DebugOutPrintf(DEBUG_WRN, "failed to enable phone number of incoming call output automatically.\r\n");
            return ER_GSM_INIT_CLIP;
        }


        // Network初始化
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to get rssi[%d].\r\n", DEV_GSM_Timing(), times++);

            rssi = 1;
            ret = GSM_GetRSSI((int *) &rssi);
            // 查询RSSI
            if(ret == true)
            {
                DebugOutPrintf(DEBUG_INF, "RSSI = %d\r\n", rssi);
                break;
            }
            else
            {
                DebugOutPrintf(DEBUG_WRN, "failed to get rssi{%d}.\r\n", rssi);

                err = ER_GSM_INIT_RSSI;
            }
            DEV_GSM_Delay(1000);
        }
        while (times <= 5);
        if (!ret)
        {
            DEV_GSM_Exit();
        }


        // GPRS环境初始化
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to check reg state[%d].\r\n", DEV_GSM_Timing(), times++);

            ret = GSM_CheckNetwork();
            if (ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "unregistered to network.\r\n");
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "already registered local network.\r\n");
                break;
            }
            DEV_GSM_Delay(5 * 1000);
        }
        while (times <= TO_GSM_REGISTRATION / 5);


        // GPRS环境初始化
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to check gprs attach[%d].\r\n", DEV_GSM_Timing(), times++);

            ret = GSM_CheckGPRSActive();
            if (ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "unattached to gprs.\r\n");
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "already attached to gprs.\r\n");
                break;
            }
            DEV_GSM_Delay(2000);
        }
        while (times <= 5);


        /*
        // 设置数据收发都为ASCII模式(即传统的二进制模式)
        if(gsm_send_at("AT+DATAFORMAT=1,1\r\n", "OK", 2, false) != OK)
        {
        	DEV_GSM_Exit();
        	DebugOutPrintf(DEBUG_WRN, "failed to set data mode to ASCII.\r\n");

        	return ER_GSM_INIT_DATAFORMAT;
        }
        */

        // 设置APN
        DebugOutPrintf(DEBUG_INF, "##<%d>to set gprs apn...\r\n", DEV_GSM_Timing());

        if (GSM_SetGPRSAPN() == true)
        {
            DebugOutPrintf(DEBUG_INF, "APN set.\r\n");
        }

        // SMS环境初始化
#if (GSM_SMS_EN != 0)
        DebugOutPrintf(DEBUG_INF, "##<%d>to set for sms mode to text...\r\n", DEV_GSM_Timing());

        do
        {
            times = 0;
            while (times <= 10)
            {
                // 将GSM模块的短消息模式设置为文本模式
                ret = GSM_SetSMSMode(GSM_SMS_MODE_TXT);
                if((ret == true) || (times >= 10))
                {
                    break;
                }
                DEV_GSM_Delay(2500);
                DebugOutPrintf(DEBUG_INF, "##<%d>to try set SMS mode.\r\n", ++times);
            };

            if (!ret)
            {
                DEV_GSM_Exit();
                DebugOutPrintf(DEBUG_WRN, "failed to set SMS mode to text.\r\n");

                err = ER_GSM_INIT_SMS_MODE;
                goto dev_gsm_init_err;
            }
            // 短消息输出模式采用默认设置(以便GSM模块接收到短信时能输出+SMSFLAG提示消息)，
            // 采用其他设置反而不能正常产生ring中断
            DebugOutPrintf(DEBUG_INF, "##<%d>to set for cnmi...\r\n", DEV_GSM_Timing());

            // 禁止接收到的短信自动输出，以使上层应用在输出短信前低RING引脚
            if(DEV_SendAT("AT+CNMI=3,1,0,0,0\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to set CNMI.\r\n");

                DEV_GSM_Exit();

                err = ER_GSM_INIT_SMS_SETTING;
                goto dev_gsm_init_err;
            }
            DebugOutPrintf(DEBUG_INF, "enable outputing received SMS automatically.\r\n");

            DebugOutPrintf(DEBUG_WRN, "##<%d>to delete all sms in current memory\r\n", DEV_GSM_Timing());

            // 删除当前短信存储器中的所有短信，以免短信接收异常
            if(DEV_SendAT("AT+CMGD=1,4\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
            {
                DEV_GSM_Exit();

                DebugOutPrintf(DEBUG_WRN, "failed to delete all sms in current memory.\r\n");

                err = ER_GSM_INIT_SMS_DELETE;
                goto dev_gsm_init_err;
            }
            DebugOutPrintf(DEBUG_INF, "##<%d>to set data mode to ASCII.\r\n", DEV_GSM_Timing());
        }
        while (false);
#endif	/* defined(GSM_SMS_EN) */


        // 查询GSM 时钟管理
        DebugOutPrintf(DEBUG_INF, "##<%d>to get cclk.\r\n", DEV_GSM_Timing());

        if(GSM_GetCCLK((char *)str, 64) == true)
        {
            DebugOutPrintf(DEBUG_INF, "GSM CCLK: %s.\r\n", str);
        }
        else
        {
            DebugOutPrintf(DEBUG_WRN, "failed to get GSM CCLK.\r\n");
        }

        DebugOutPrintf(DEBUG_INF, "##<%d>GSM initialized successfully.\r\n", DEV_GSM_Timing());

        // 初始化gsm成功后将Global.is_gsm_ready设置为TRUE
        is_gsm_ready = true;
        return true;
dev_gsm_init_err:
        return false;
    }

    void   	DEV_GSM_Struct::DEV_GSM_PowerUp(void)
    {
        GSMPWRCTRL.SetBits(BSP_GSM_PWRCTRL);
        DEV_GSM_Delay(300);
        DebugOutPrintf(DEBUG_INF, "GSM powered up.\r\n");
    }

    void   	DEV_GSM_Struct::DEV_GSM_PowerDown(void)
    {
        GSMPWRCTRL.ResetBits(BSP_GSM_PWRCTRL);
        DEV_GSM_Delay(300);
        DebugOutPrintf(DEBUG_INF, "GSM powered down.\r\n");
    }

    void   	DEV_GSM_Struct::DEV_GSM_Sleep(void)
    {
#if (GSM_SLEEP_EN != 0)
        int err, pos = -1;
        // 拉高DTR引脚
        GSMWAKEUP.SetBits(BSP_GSM_WAKEUP);

        // 允许模块进入休眠模式
        if(DEV_SendAT("AT+ZDSLEEP=1\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
        {
            return;
        }

        // 拉低DTR引脚
        GSMWAKEUP.ResetBits(BSP_GSM_WAKEUP);

        // 模块只有在空闲时才会进入待机模式，如果有数据交互未结束，不会进入待机。
#endif
        DebugOutPrintf(DEBUG_INF, "GSM sleep.\r\n");
    }

    void   	DEV_GSM_Struct::DEV_GSM_Wakeup(void)
    {
#if (GSM_SLEEP_EN != 0)
        // 检查当前DTR是否被拉低，被拉低则说明之前模块被手动置于休眠状态
        if (GSMWAKEUP.ReadOutputDataBit(BSP_GSM_WAKEUP) == 0)
        {
            // 拉高DTR引脚
            GSMWAKEUP.SetBits(BSP_GSM_WAKEUP);

            DEV_GSM_Delay(2000);
        }
#endif
        DebugOutPrintf(DEBUG_INF, "GSM wake up.\r\n");

    }

    void   	DEV_GSM_Struct::DEV_GSM_OnOff(void)
    {
        GSMONOFF.SetBits(BSP_GSM_ONOFF);
        DEV_GSM_Delay(3000);
        GSMONOFF.ResetBits(BSP_GSM_ONOFF);

        DebugOutPrintf(DEBUG_INF, "GSM On/OFF.\r\n");
    }

    /**
      * @brief  Description "按正常流程关断GSM模块"
      * @param  None
      * @retval None
      */
    void   	DEV_GSM_Struct::DEV_GSM_Exit(bool param)
    {
        if (!param)
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>GSM initialized failed.\r\n", DEV_GSM_Timing());
        }
        input.DeInit();

        DEV_GSM_OnOff();

        DEV_GSM_PowerDown();

        while(1)
        {
            if (param)
            {
                LEDIO4.ToggleBits(BSP_LED_CH4);
            }
            else
            {
                LEDIO5.ToggleBits(BSP_LED_CH5);
            }
            DEV_GSM_Delay(250);
        }
    }

    bool	DEV_GSM_Struct::DEV_FindPattern(char *ptn, int to, int &pos, int &err)
    {
        vs32 	len = 0;
        // 检查输入参数的合法性
        len = __strlen(ptn);

        if(len <= 0)
        {
            err = ER_GSM_PATTERN_NULL;
            return false;
        }
        input.TimoutInit(to);
        CmdBuffer[0] = '\0';
        do
        {
            input  	>> CmdBuffer;

            len = __strlen(CmdBuffer);
            if (len > 0)
            {
                DebugOutPrintf(DEBUG_INF, ">>[%d]%s\r\n", len, CmdBuffer);

                pos = KMPMatch((char *)CmdBuffer, ptn, __strlen(CmdBuffer));
                if(pos >= 0)
                {
                    pos = pos + __strlen(ptn);
                    break;
                }
            }
        }
        while (!input.IsTimout());

        if (input.IsTimout())
        {
            err = ER_GSM_UART_RECV_TIMEOUT;
            return false;
        }
        else
        {
            return true;
        }
    }

    bool	DEV_GSM_Struct::DEV_FindFlag(char ptn, int to, int &err)
    {
        char 	CmdChar = '\0';

        input.TimoutInit(to);
        do
        {
            input  	>> CmdChar;

            if (CmdChar != '\0')
            {
                DebugOutPrintf(DEBUG_INF, ">>[%d]0x%X\r\n", 1, CmdChar);

                if(CmdChar == ptn)
                {
                    break;
                }
            }
        }
        while (!input.IsTimout());

        if (input.IsTimout())
        {
            err = ER_GSM_UART_RECV_TIMEOUT;
            return false;
        }
        else
        {
            return true;
        }
    }

    bool	DEV_GSM_Struct::DEV_SendAT(char *at, char *ptn, int to, int &pos, int &err, bool ok)
    {
        int len;
        input.TimoutInit(to);
        CmdBuffer[0] = '\0';

        output 	<< at;
        do
        {
            input  	>> CmdBuffer;

            len = __strlen(CmdBuffer);
            if (len > 0)
            {
                DebugOutPrintf(DEBUG_INF, ">><%d>[%d]%s\r\n", DEV_GSM_Timing(), len, CmdBuffer);

                pos = KMPMatch((char *)CmdBuffer, ptn, len);
                if(pos >= 0)
                {
                    pos = pos + __strlen(ptn);
                    if (ok)
                    {
                        if (len - pos > 1)
                        {
                            int pos2 = KMPMatch((char *)&CmdBuffer[pos], "OK", len - pos);
                            if(pos2 >= 0)
                            {
                                return true;
                            }
                        }
                        err = ER_GSM_AT_MISS_OK;
                    }
                    break;
                }
            }
        }
        while (!input.IsTimout());

        if (input.IsTimout())
        {
            err = ER_GSM_UART_RECV_TIMEOUT;
            return false;
        }
        else
        {
            return true;
        }
    }

    bool	DEV_GSM_Struct::DEV_FetchPattern(char *ptn, int to, int &pos, int &err)
    {
        int rlen = __strlen(CmdBuffer);
        int plen = __strlen(ptn);
        if ((rlen > 0) && (plen > 0) && (pos >= 0) && (rlen - pos >= plen))
        {
            if (plen == 1)
            {
                if (__strstr(CmdBuffer, ptn) != NULL)
                {
                    return true;
                }
            }
            else
            {
                int new_pos = -1;
                new_pos = KMPMatch(ptn, CmdBuffer, rlen);

                if (new_pos >= pos)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool	DEV_GSM_Struct::DEV_FetchString(char *ptn, char *str, int len, int to, int &pos)
    {
        int rlen = __strlen(CmdBuffer);
        if ((pos >= 0) && (rlen > pos))
        {
            int i;
            char *ptr = CmdBuffer + pos;
            len = len > (rlen - pos) ? (rlen - pos) : len;
            for(i = 0; (i < len) && IS_TXT_COMMAND_CHARACTER(*ptr); i++)
            {
                str[i] = *(ptr ++);
            }

            str[i++] = '\0';
            return true;
        }
        return false;
    }

    bool	DEV_GSM_Struct::DEV_FetchHex(char *ptn, char *str, int &len, int to, int &pos)
    {
        int rlen = __strlen(CmdBuffer);
        if ((pos >= 0) && (rlen > pos))
        {
            int i;
            char *ptr = CmdBuffer + pos;
            len = len > (rlen - pos) ? (rlen - pos) : len;
            for(i = 0; (i < len); i++)
            {
                str[i] = *(ptr ++);
            }
            return true;
        }
        len = 0;
        return false;
    }

    bool	DEV_GSM_Struct::DEV_FetchValue(char *ptn, int *val, int to, int &pos)
    {
        char	str[MAX_DECIMAL_DIGITS_FOR_INT + 1];
        int		i = 0, rlen;

        assert_param(*val != 0);
        str[0] = '\0';
        rlen = __strlen(CmdBuffer);
        if ((pos >= 0) && (rlen > pos))
        {
            char *ptr = CmdBuffer + pos;
            do
            {
                if(*ptr == '-')
                {
                    // 未见测到任何数字字符前检测到'-'可认为是数值的正负符号
                    if(i == 0)
                    {
                        *val = -1;
                    }
                    // 否则检测到'-'可认为是数字字符末尾的后一字符
                    else
                    {
                        str[i] = '\0';		// 在数字字符数组后面添加字符串结尾符号

                        break;
                    }
                }
                else if(*ptr >= '0' && *ptr <= '9')
                {
                    // 读游标暂时不移动，因刚好指向数字字符
                    str[i++] = *ptr;

                    if(i >= MAX_DECIMAL_DIGITS_FOR_INT)
                    {
                        str[i] = '\0';		// 在数字字符数组后面添加字符串结尾符号

                        break;
                    }
                }
                else
                {
                    // 检查str的游标是否大于0，若大于，则说明str中已经填充了数字字符、
                    // 当前检测到非数字字符的话说明数字字符串已经结束。
                    if(i > 0)
                    {
                        str[i] = '\0';		// 在数字字符数组后面添加字符串结尾符号

                        break;
                    }
                    // else: 略过当前的非数字字符，检测下一个字符
                }
                pos ++;
            }
            while ((*(++ptr) != '\0'));

            str[i] = '\0';		// 在数字字符数组后面添加字符串结尾符号
            // 将检测到的数字字符会串转化为数值(注意数值的正负号)
            if(__strlen(str) > 0)
            {
                *val *= __atoi(str);

                return true;
            }
        }
        return false;
    }

    void	DEV_GSM_Struct::DEV_SEND_DATA(char *str, int size)
    {
        int nIndex = 0;
        for (nIndex = 0; nIndex < size; nIndex ++)
        {
            USARTx_PUTCHAR::Send(str[nIndex]);
        }
    }

    // 格式化gsm号码(统一为不带国家区号前缀的纯数字格式)。
    // 注: gsm号码在系统和外界的边界上就格式化，即接收到短信后、将短信命令加入命令队列前格式化发送方号码，发送短信出去前格式化目标号码。
    int		DEV_GSM_Struct::DEV_FormatPN(char *pn)
    {
        int 	len = __strlen(pn);
        int		n = -1;

        if(len <= 0)		// 串口命令
        {
            return 1;
        }
        else if(len == 1)	// TCP命令
        {
            n = __atoi(pn);

            if(n >= 0 && n < MAX_NUM_TCP_CONN)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else if(len < MAX_LEN_PN)
        {
            n = KMPMatch(pn, gsm_telecode, len);

            if(n == 0)
            {
                // 去掉国家区号
                __strcpy(pn, pn + __strlen(gsm_telecode));
            }

            return is_all_digit(pn);
        }
        else
        {
            return 0;
        }
    }
    bool	DEV_GSM_Struct::GSM_CheckBaudRate(int br)
    {
        int err, pos = -1;
        int cbr = 1;
        bool ret;
        char ptn[16] = {"+IPR:"};

        ret = DEV_SendAT("AT+IPR?\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchValue(ptn, &cbr, 0, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }

        if (cbr != br)
        {
            ret = GSM_SetBaudRate(br);
            if (!ret)
            {
                return false;
            }
            return false;
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_CheckSIM(void)
    {
        int err, pos = -1;
        bool ret;

        ret = DEV_SendAT("AT+ZGETICCID\r\n", "+ZGETICCID:", TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_CheckNetwork(void)
    {
        int err, pos = -1;
        bool ret;
        int nmode = 1, nstat = 1;
        char ptn[16] = {"+CREG:"};

        // 允许网络注册主动提供所在地讯息（CELL ID、LOCAL ID）
        ret = DEV_SendAT("AT+CREG=2\r\n", "OK", TO_GPRS_RT_OK, pos, err);
        if (!ret)
        {
            return false;
        }
        // 查询网络注册状况
        ret = DEV_SendAT("AT+CREG?\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchValue(ptn, &nmode, 0, pos);
        if (!ret)
        {
            return false;
        }
        if (nmode > 0)
        {
            ret = DEV_FetchValue(ptn, &nstat, 0, pos);
            if (!ret)
            {
                return false;
            }
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        if ((nmode > 0) && (nstat == 1 || nstat == 5))
        {
            return true;
        }
        return false;
    }

    bool	DEV_GSM_Struct::GSM_CheckGPRSActive(void)
    {
        int err, pos = -1;
        bool ret;
        int state = 1;
        char ptn[16] = {"+CGATT:"};

        // 查询GPRS是否附着
        ret = DEV_SendAT("AT+CGATT?\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchValue(ptn, &state, 0, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        DebugOutPrintf(DEBUG_INF, "to get CGATT %d.\r\n", state);

        if (state == 1)
        {
            return true;
        }
        return false;
    }

    bool	DEV_GSM_Struct::GSM_GetVender(char *vender, int len)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CGMI:"};

        ret = DEV_SendAT("AT+CGMI\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString(ptn, vender, len, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_GetSWVersion(char *version, int len)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CGMR:"};

        ret = DEV_SendAT("AT+CGMR\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString(ptn, version, len, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_GetState(int *state)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CPAS:"};

        ret = DEV_SendAT("AT+CPAS\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchValue(ptn, state, 3, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        if ((*state) == 0)
        {
            return true;
        }
        else
        {
            err = ER_GSM_AT_NOT_RDY;
            return false;
        }
    }

    bool 	DEV_GSM_Struct::GSM_GetIMEI(char *imei, int len)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CIMI:"};

        ret = DEV_SendAT("AT+CIMI\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString(ptn, imei, len, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool 	DEV_GSM_Struct::GSM_GetICCID(char *iccid, int len)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+ZGETICCID:"};

        ret = DEV_SendAT("AT+ZGETICCID\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString(ptn, iccid, len, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_GetRSSI(int *rssi)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CSQ:"};
        const int bdm_tbl[][2] =
        {
            //rssi, bdm
            {0, 	-113},
            {1, 	-111},
            {2, 	-109},
            {30, 	-53},
            {31, 	-51},
            {99, 	0},	// 无网络
        };

        *rssi = 1;
        ret = DEV_SendAT("AT+CSQ\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchValue(ptn, rssi, 3, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        if (__abs(*rssi) == 99)
        {
            err = ER_GSM_RSSI_UNKNOW;
            return false;
        }
        if (__abs(*rssi) < MIN_RSSI_FOR_COMMUNICATION)
        {
            err = ER_GSM_RSSI_TOOLOW;
            return false;
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_GetCCLK(char *cclk, int len)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CCLK:"};

        ret = DEV_SendAT("AT+CCLK?\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString(ptn, cclk, len, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    /**
      * @brief  在GPRS连接建立后，查询模块自身的IP地址。
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GSM_GetIP(char *ip, int len)
    {
        int 	err, pos = -1;
        bool 	ret;

        ret = DEV_SendAT("AT+ZIPGETIP\r\n", "+ZIPGETIP:", TO_GPRS_DO_DNS, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString("", ip, len, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        DebugOutPrintf(DEBUG_INF, "to get module ip:\"%s\".\r\n", ip);

        return true;
    }
    /**
      * @brief  将制定的域名解析成IP地址(应在APN设置后执行)。
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GSM_GetDNS(char *ip, int len_ip, char *dn)
    {
        int 	err, pos = -1;
        bool 	ret;
        char 	at[128];

        DebugOutPrintf(DEBUG_INF, "to inquire dns...\r\n");

        ret = DEV_SendAT("AT+ZDNSSERV?\r\n", "+ZDNSSERV:", TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString("", at, 128, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        DebugOutPrintf(DEBUG_INF, "dns:%s\r\n", at);

        pos = -1;
        sprintf(at, "AT+ZDNSGETIP=\"%s\"\r\n", dn);

        // OK和+DNS几乎同时出现
        ret = DEV_SendAT(at, "+ZDNSGETIP:", TO_GPRS_DO_DNS, pos, err, true);
        // M660+的AT+DNS命令超时已经设置并默认为5秒，2012-04-19 23:23
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString("", ip, len_ip, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_SetBaudRate(int br)
    {
        int		err, pos = -1;
        char	at[32];

        sprintf(at, "AT+IPR=%d\r\n", br);

        if(DEV_SendAT(at, "OK", TO_GPRS_GPCMD, pos, err) == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to set IPR.\r\n");

            return false;
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_SetGPRSAPN(void)
    {
        int		err, pos = -1;
        char	at[32];
        // 设置APN	"AT+NETAPN=%s,\"\",\"\"\r\n"
        sprintf(at, "AT+ZPNUM=\"%s\",\"\",\"\"\r\n", gsm_apn);

        if(DEV_SendAT(at, "OK", TO_GPRS_SET_APN, pos, err) == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to set APN.\r\n");

            return false;
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_SetSMSMode(enum DEV_GSM_SMS_Mode mode)
    {
        int err, pos = -1;
        bool ret;
        int nmode = 1;
        char ptn[16] = {"+CMGF:"};
        const char *at[2] =
        {
            "at+cmgf=0\r\n",	// gsm_sms_mode_pdu
            "at+cmgf=1\r\n",	// gsm_sms_mode_txt
        };

        ret = DEV_SendAT("AT+CMGF?\r\n", ptn, TO_GPRS_GPCMD, pos, err);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchValue(ptn, &nmode, 0, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        if (nmode != (int)mode)
        {
            ret = DEV_SendAT((char *)at[mode], "OK", TO_GPRS_RT_OK, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    // Encode input arguments into a complete PDU stream in ASCCI format and
    // return the PDU length excluding SCA part(sca_len + sca_fmt + sca_str).
    bool	DEV_GSM_Struct::GSM_SMS_PDUConstruct(char *pdu, char *pn, char *sca, unsigned char dcs,
            unsigned char *encoded, unsigned int size,
            unsigned char udhi, unsigned char *udh, unsigned int len_udh)
    {
        char			buf_sca[32];
        char			buf_da[32];
        unsigned int	len = 0;
        unsigned int 	len_encoded = 0;
        unsigned int 	len_fnl = 0;
        unsigned char	arg = 0;
        T_SMS_SUBMIT	smssubmit = {0x00, 0x00, "", 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, "", 0x00, 0x00, 0x00, 0x00, "" };

        // // printf("size = %d, encoded = %s\r\n", size, (char*)encoded);

        // handle SCA( SCA length counts both sca_str and sca_type, but in unsigned char format)
        while(*sca == '+')
        {
            sca++;							// ignore '+' in the front of phone number
        }

        len = __strlen(sca);

        printf("sca = %s\r\n", sca);

        if(len == 0)
        {
            smssubmit.sca_len = len;

            len = sprintf(buf_sca, "%02x", smssubmit.sca_len);
        }
        else
        {
            len = pdu_invert((char *)smssubmit.sca_str, sca, len);

            smssubmit.sca_len = len / 2 + 1;	// SCA length counts both sca_str in unsigned char and sca_fmt(1 byte)

            smssubmit.sca_fmt = 0x91;		// international numbering with "86" prefix

            len = sprintf(buf_sca, "%02x%02x%s", smssubmit.sca_len, smssubmit.sca_fmt, smssubmit.sca_str);
        }

        // // printf("buf_sca = %s\r\n", buf_sca);

        // 设置udhi位
        smssubmit.tp_udhi = udhi;

        // handle TP-MTI/VFP etc. as a complete argument
        arg =   (smssubmit.tp_mti & 3) | ((smssubmit.tp_rd & 1) << 2) | ((smssubmit.tp_vpf & 3) << 3) | \
                ((smssubmit.tp_srr & 1) << 5) | ((smssubmit.tp_udhi & 1) << 6) | ((smssubmit.tp_rp & 1) << 7);

        // // printf("arg = %02x\r\n", arg);

        // handle DA
        while(*pn == '+')
        {
            pn++;								// ignore '+' in the front of phone number
        }

        smssubmit.tp_dal = __strlen(pn);		// decimal digit number of destination adress, excluding tp_dat and attached 'F'
        pdu_invert((char *)smssubmit.tp_das, pn, smssubmit.tp_dal);

        smssubmit.tp_dat = 0x91;				// internal numbering

        len = sprintf(buf_da, "%02x%02x%s", smssubmit.tp_dal, smssubmit.tp_dat, smssubmit.tp_das);

        // // printf("buf_da = %s\r\n", buf_da);

        // 设置待发送内容采用的字符编码格式
        smssubmit.tp_dcs = dcs;

        // 设置udl
        if(dcs == SMS_PDU_ENCODING_UCS2)
        {
            smssubmit.tp_udl = size;				// 对于ucs2编码，udl记录的是未编码数据的长度
        }
        else if(dcs == SMS_PDU_ENCODING_7BIT)		// 对于7bit编码，udl记录的是未编码数据的长度
        {
            if(udhi == 1)
            {
                smssubmit.tp_udl = (size * 8 / 7) + 1;	// 对于级联短信，udl为原始内容长度+1
            }
            else
            {
                smssubmit.tp_udl = (size * 8 / 7);		// 对于级联短信，udl刚好为原始内容长度
            }
        }
        else if(dcs == SMS_PDU_ENCODING_8BIT)
        {
            smssubmit.tp_udl = size;				// 对于ucs2编码，udl记录的是未编码数据的长度
        }

        // 拷贝编码后的待发送内容到uds
        if(udhi == 1)
        {
            memcpy((unsigned char *)smssubmit.tp_uds, udh, len_udh);
            len_encoded += len_udh;

            memcpy(smssubmit.tp_uds + len_udh, encoded, size);
            len_encoded += size;

            smssubmit.tp_udl += len_udh;
        }
        else if(udhi == 0)
        {
            memcpy((unsigned char *)smssubmit.tp_uds, encoded, size);
            len_encoded += size;
        }

        // package entire PDU, which should be in ASCII format(PDU dose not include SCA part, 0x1A and \r\b).
        // 1, 0x1A(unsigned char, 1 size) aattached to PDU string;
        // 2, \r\n(0x0D, 0x0A) attached further;
        len = sprintf(pdu, "%s%02x%02x%s%02x%02x%02x%02x",	buf_sca,
                      arg,
                      smssubmit.tp_mr,
                      buf_da,
                      smssubmit.tp_pid,
                      smssubmit.tp_dcs,
                      smssubmit.tp_vp,
                      smssubmit.tp_udl);

        pdu += len;
        len_fnl += len;

        len = pdu_htoa(pdu, (unsigned char *)smssubmit.tp_uds, len_encoded);

        pdu += len;
        len_fnl += len;

        // 添加PDU字符串结尾符号0x1A
        len = sprintf(pdu, "\x1A\r\n");						// 这三个字节不计算在长度内

        len_fnl += len;

        return ((len_fnl - 3) / 2 - 1 - smssubmit.sca_len);	// 返回除SCA(包括sca_len字节的sca数据和sca_len字节本身)外外的pdu包字节数
    }
    /**
      * @brief  将指定的内容以PDU格式的SMS发送给指定目标方。
      *         ascii_utf8	- 	以ASCII编码的英文待发送短信内容或以UTF-8
      *                         编码的中文待发送短信内容。
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GSM_SMS_SendPDU(T_SMS_LANGUAGE language, char *pns, char *ascii_utf8, int &err)
    {
        bool			ret;
        // pdu的最大长度一般小于320字节
        char			pdu[320];
        // 经特定编码后的短信内容(中文采用UCS2编码、英文采用7bit编码)
        unsigned char	encoded[MAX_LEN_CNC_SMS * MAX_NUM_CNC_SMS];
        unsigned char	*pencoded = (unsigned char *)encoded;

        int				len = 0;

        int				rest = 0;
        unsigned int 	tobesent = 0;
        unsigned int	len_seg = 0;		// 每个pdu包中包含的纯用户数据长度(中文和英文不同)

        unsigned int	feed;

        unsigned char	udh[7];				// uder data header
        unsigned int	len_udh = 0;		// udh长度(中文使用6字节udh、英文使用7字节udh)
        int				segment_total = 0;	// 长短信拆分发送的总条数
        int				segment_count = 0;	// 长短信差分法送的当前序号(从1开始计数)

        // 计算待发送字符串的原始长度(未编码前)
        rest = __strlen(ascii_utf8);

        DebugOutPrintf(DEBUG_INF, "content to be sent in pdu:%s\r\n", ascii_utf8);

        // 先将待发送内容根据语言类别编码为特定的格式
        if(language == SMS_LANGUAGE_CHINESE)
        {
            rest = pdu_encode16bit(encoded, ascii_utf8, rest);

            // 检查待发送的短信内容中是否包含非utf8编码也非ascii编码的字符。
            if(rest <= 0)
            {
                DebugOutPrintf(DEBUG_ERR, "error in encoding pdu.\r\n");

                err = ER_GSM_SMS_PDU_CHAOS;
                goto gsm_sms_send_pdu_err;
            }

            DebugOutPrintf(DEBUG_INF, "to send %d Chinese characters.\r\n", rest / 2);

            len_udh = 6;
            len_seg = MAX_BYTE_SMS_PDU - len_udh;
        }
        else if(language == SMS_LANGUAGE_ENGLISH)
        {
            DebugOutPrintf(DEBUG_INF, "to send %d English characters.\r\n", rest);

            rest = pdu_encode8bit((char *)encoded, (char *)ascii_utf8, rest);

            // 检查待发送短信内容中是否包含非ASCII字符。
            if(rest <= 0)
            {
                err = ER_GSM_SMS_PDU_CHAOS;
                goto gsm_sms_send_pdu_err;
            }

            len_udh = 7;
            len_seg = MAX_BYTE_SMS_PDU - len_udh;
        }

        // 计算短信要拆分的总条数(每条拆分后的短信最大长度为MAX_BYTE_SMS_PDU减去udh的长度6字节)
        if(rest > MAX_BYTE_SMS_PDU)
        {
            if(rest % len_seg)
            {
                segment_total = rest / len_seg + 1;
            }
            else
            {
                segment_total = rest / len_seg;
            }
        }
        else	// : 发送短信内容长度小于最大pdu长度时，不以级联短信方式发送
        {
            segment_total = 1;
        }

        // 用于产生伪随机数
        feed = DEV_GSM_Timing();
        // 拆分发送长短信
        while(rest > 0)
        {
            // 检查是否需要将短信拆分发送
            if(segment_total > 1)
            {
                //计算变量
                if(rest > len_seg)
                {
                    tobesent = len_seg;
                    rest -= tobesent;
                    segment_count++;
                }
                else
                {
                    tobesent = rest;
                    rest -= tobesent;
                    segment_count++;
                }

                // 构造udh(中文使用6字节udh，以便剩余的134字节可以容纳整数个汉字，
                // 英文使用7字节udh，以便udh本身和uds一样构成7字节组)
                if(len_udh == 6)
                {
                    udh[0] = 0x05;
                    udh[1] = 0x00;
                    udh[2] = 0x03;
                    udh[3] = feed & 0xFF;			// serial number
                    udh[4] = segment_total;
                    udh[5] = segment_count;				// 分条短信从1开始计数
                }
                else if(len_udh == 7)
                {
                    udh[0] = 0x06;
                    udh[1] = 0x08;
                    udh[2] = 0x04;
                    udh[3] = feed & 0xFF;			// serial number
                    udh[4] = (feed >> 2) & 0xFF;		// serial number
                    udh[5] = segment_total;
                    udh[6] = segment_count;				// 分条短信从1开始计数
                }

                // 将编码后的短信内容打包成pdu字符串
                if(language == SMS_LANGUAGE_CHINESE)
                {
                    len = GSM_SMS_PDUConstruct((char *)pdu, pns, (char *)gsm_sca,
                                               SMS_PDU_ENCODING_UCS2, pencoded, tobesent, 1, (unsigned char *)udh, len_udh);

                    pencoded += tobesent;
                }
                else if(language == SMS_LANGUAGE_ENGLISH)
                {
                    len = GSM_SMS_PDUConstruct((char *)pdu, pns, (char *)gsm_sca,
                                               SMS_PDU_ENCODING_8BIT, pencoded, tobesent, 1, (unsigned char *)udh, len_udh);

                    pencoded += tobesent;
                }
            }
            else
            {
                tobesent = rest;
                rest -= tobesent;

                if(language == SMS_LANGUAGE_CHINESE)
                {
                    len = GSM_SMS_PDUConstruct((char *)pdu, pns, (char *)gsm_sca,
                                               SMS_PDU_ENCODING_UCS2, pencoded, tobesent, 0, (unsigned char *)NULL, len_udh);

                    pencoded += tobesent;
                }
                else if(language == SMS_LANGUAGE_ENGLISH)
                {
                    len = GSM_SMS_PDUConstruct((char *)pdu, pns, (char *)gsm_sca,
                                               SMS_PDU_ENCODING_8BIT, pencoded, tobesent, 0, (unsigned char *)NULL, len_udh);

                    pencoded += tobesent;
                }
            }
            ret = GSM_SendSMS(pdu, len, err);
            if (!ret)
            {
                goto gsm_sms_send_pdu_err;
            }
        }
        // 发送完SMS后，强行将SMS收发模式设置为TXT模式，便于接收SMS命令
        ret = GSM_SetSMSMode(GSM_SMS_MODE_TXT);
        if(!ret)
        {
            DebugOutPrintf(DEBUG_WRN, "error in setting sms to text mode.\r\n");

            goto gsm_sms_send_pdu_err;
        }
        return true;
gsm_sms_send_pdu_err:
        return false;
    }

    bool	DEV_GSM_Struct::GSM_SendSMS(char *pdu, int len, int &err)
    {
        int		pos = -1;
        bool 	ret;

        char	at[32];

        // 检查SMS模式设置命令是否执行成功，若不成功，则可能因为当前处于SMS PDU输入模式，
        // 此时需发送0x1B终止输入模式
        ret = DEV_SendAT("AT+CMGF=0\r\n", "OK", TO_GPRS_GPCMD, pos, err);
        if(!ret)
        {
            DEV_SEND_DATA("\x1B", 1);

            DebugOutPrintf(DEBUG_ERR, "error in \"AT+CMGF=0\".\r\n");

            return ret;
        }

        // 发送PDU模式短信的命令AT+CMGS=后面需要紧跟待发送PDU包的长度(不包括SCA部分的，
        // 即不包括Global.gsm_sca_len + Global.gsm_sca_fmt + Global.gsm_sca_str)
        // 此长度数值必须正确，否则SMS发送会失败。
        sprintf(at, "AT+CMGS=%d\r", len); 	// 这里用\r而非\r\n去结束AT命令，可能是MTK协议栈的Bug

        // 检查是否进入SMS PDU输入模式，若未进入，则可能因为当前正处于SMS PDU输入模式，
        // 此时需要发送0x1B终止输入模式
        ret = DEV_SendAT(at, "+CMGS", TO_GPRS_GPCMD, pos, err);
        if(ret == false) // 等待SMS PDU输入指示符'>'
        {
            DEV_SEND_DATA("\x1B", 1);

            DebugOutPrintf(DEBUG_ERR, "error in waiting for '>'.\r\n");
            return ret;
        }
        // 查询输入标志 '>' 是否粘连
        ret = DEV_FetchPattern(">", TO_GPRS_GPCMD, pos, err);
        if(!ret)
        {
            DebugOutPrintf(DEBUG_INF, "to query data send pattern \">\".\r\n");
            ret = DEV_FindPattern("> ", TO_GPRS_GPCMD, pos, err);
            if(ret == false)
            {
                DEV_SEND_DATA("\x1B", 1);

                DebugOutPrintf(DEBUG_ERR, "error in waiting for '>'.\r\n");
                return ret;
            }
        }
        //DEV_SEND_DATA((char *)pdu, len);

        // 发送短信
        ret = DEV_SendAT(pdu, "+CMGS:", TO_SMS_TX, pos, err, true);
        if(!ret)
        {
            // cancel SMS inputing mode so to accept following AT command
            DEV_SEND_DATA("\x1B", 1);

            DebugOutPrintf(DEBUG_ERR, "error in sending pdu.\r\n");

            return ret;
        }

        return true;
    }

    /**
      * @brief  检查指定GPRS数据链路的连接状态，若返回值为OK，则为连接状态，否则为未连接状态。
      *         注: GPRS连接自动保持超时时间为30分钟，即GPRS连接空闲超过30分钟，GSM基站会将其断开。
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GPRS_LINK_Status(void)
    {
        int		err, pos = -1;
        bool	ret;
        char	at[16];
        char	ptn[16];
        char	sts[16 + 1];

        DebugOutPrintf(DEBUG_INF, "to check GPRS PPP data link connection status.\r\n");

        sprintf(at, "AT+ZPPPSTATUS\r\n");

        // 先检测反馈消息的消息头
        sprintf(ptn, "+ZPPPSTATUS:");

        ret = DEV_SendAT(at, ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString(ptn, sts, 16, 0, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return ret;
            }
        }
        DebugOutPrintf(DEBUG_INF, "GPRS PPP Status = %s\r\n", sts);

        pos = KMPMatch(sts, "DISCONNECTED", __strlen(sts));
        if(pos >= 0)
        {
            DebugOutPrintf(DEBUG_INF, "gprs PPP is disconencted.\r\n");

            return false;
        }
        pos = KMPMatch(sts, "ESTABLISHED", __strlen(sts));
        if(pos >= 0)
        {
            DebugOutPrintf(DEBUG_INF, "gprs PPP is conencted.\r\n");

            return true;
        }
        else
        {
            DebugOutPrintf(DEBUG_WRN, "gprs PPP is unknowed.\r\n");

            return false;
        }
    }

    /**
      * @brief  检查指定GPRS连接的连接状态，若返回值为OK，则为连接状态，否则为未连接状态。
      *         注: GPRS连接自动保持超时时间为30分钟，即GPRS连接空闲超过30分钟，GSM基站会将其断开。
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GPRS_SOC_Status(int id, enum DEV_GPRS_Mode mode)
    {
        int		err, pos = -1;
        bool	ret;
        char	at[16];
        char	ptn[16];
        char	sts[16 + 1];

        const char *cmd_at[2] = {"AT+ZIPSTATUS=%d\r\n", "AT+ZIPSTATUSU=%d\r\n"};
        const char *cmd_ptn[2] = {"+ZIPSTATUS:", "+ZIPSTATUSU:"};

        DebugOutPrintf(DEBUG_INF, "to check GPRS #%d connection status.\r\n", id);

        sprintf(at, cmd_at[mode], id);

        // 先检测反馈消息的消息头
        sprintf(ptn, cmd_ptn[mode]);

        ret = DEV_SendAT(at, ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString(ptn, sts, 16, 0, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return ret;
            }
        }
        DebugOutPrintf(DEBUG_INF, "GPRS #%d Status = %s\r\n", id, sts);

        pos = KMPMatch(sts, "DISCONNECTED", __strlen(sts));
        if(pos >= 0)
        {
            DebugOutPrintf(DEBUG_WRN, "gprs conenction is disconencted.\r\n");

            return false;
        }
        pos = KMPMatch(sts, "ESTABLISHED", __strlen(sts));
        if(pos >= 0)
        {
            DebugOutPrintf(DEBUG_INF, "gprs conenction is conencted.\r\n");

            return true;
        }
        else
        {
            DebugOutPrintf(DEBUG_WRN, "gprs conenction is unknowed.\r\n");

            return false;
        }
    }


    bool	DEV_GSM_Struct::GPRS_LINK_Setup(void)
    {
        int		errors;

        DebugOutPrintf(DEBUG_INF, "to setup gprs PPP connection.\r\n");

        errors = 0;
        while(errors < MAX_TIMES_GPRS_SETUP_CONNECTION)
        {
            if(GPRS_LINK_Open() == false)
            {
                DebugOutPrintf(DEBUG_ERR, "failed to open GPRS PPP.\r\n");

                errors++;
                // 建立GPRS连接失败后间隔1秒再重试
                DEV_GSM_Delay(1000);
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "GPRS PPP setup.\r\n");

                break;
            }
        }

        if(errors >= MAX_TIMES_GPRS_SETUP_CONNECTION)
        {
            return false;
        }
        else
        {
            DebugOutPrintf(DEBUG_INF, "successfully setup GPRS PPP.\r\n");
        }

        return true;
    }

    // 根据指定的GPRS连接号建立相应的GPRS连接。
    // 1，M660+常出现阶段性的正?蛞斐Ｏ窒螅茨炒慰螅羰状蜧PRS连接正常的话，
    //	  则随后的连接正常的概率很大，反之亦然。因此某次开机后若发现GPRS连接不正常，
    //	  较好的办法是关闭模块供电然后上电;
    // 2，GPRS连接过程中若有来电呼入，则GPRS连接一般会失败，
    //    GPRS已经连接时若有来电呼入，则GPRS连接不会断开;
    //    通话过程中建立GPRS连接，则一般会失败;
    bool	DEV_GSM_Struct::GPRS_SOC_Setup(int id, enum DEV_GPRS_Mode mode)
    {
        int		errors;

        DebugOutPrintf(DEBUG_INF, "to setup gprs connection to %s:%s...\r\n",
                       (char *)tcp_conn[id].ip, (char *)tcp_conn[id].port);

        // 检查待建立连接的GPRS配置参数中IP地址是否为空，为空的话先执行域名解析
        if(__strlen(tcp_conn[id].ip) <= 0)
        {
            if(__strlen(tcp_conn[id].dn) > 0)
            {
                // TODO: ...
                if(GSM_GetDNS((char *)tcp_conn[id].ip, MAX_LEN_GPRS_IP,
                              (char *)tcp_conn[id].dn) == false)
                {
                    DebugOutPrintf(DEBUG_ERR, "failed to convert %s into IP.\r\n", tcp_conn[id].dn);

                    return false;
                }
                else
                {
                    DebugOutPrintf(DEBUG_INF, "resolved ip is:%s\r\n", (char *)tcp_conn[id].ip);
                }
            }
            else
            {
                DebugOutPrintf(DEBUG_ERR, "both IP and domain name are empty.\r\n");

                return false;
            }
        }

        errors = 0;
        // M660+默认的GPRS连接超时时间约为18秒，在TCP连接失败的情况下，即便程序提前返回，
        // M660+的串口仍处于忙状态，即此时尚不能执行正常的AT通讯，因此让程序等待，直至M660+
        // 返回明确的结果(不管是连接成功还是失败)
        // 建立GPRS连接失败时，会重试。
        while(errors < MAX_TIMES_GPRS_SETUP_CONNECTION)
        {
            if(GPRS_SOC_Open(id, mode) == false)
            {
                DebugOutPrintf(DEBUG_ERR, "#%d: failed to open GPRS connection to %s:%s.\r\n",
                               errors + 1, tcp_conn[id].ip, tcp_conn[id].port);

                errors++;

                // 建立GPRS连接失败后间隔1秒再重试
                DEV_GSM_Delay(1000);
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "GPRS conenction setup.\r\n");

                break;
            }
        }

        if(errors >= MAX_TIMES_GPRS_SETUP_CONNECTION)
        {
            return false;
        }
        else
        {
            DebugOutPrintf(DEBUG_INF, "successfully setup GPRS connection to %s:%s.\r\n",
                           tcp_conn[id].ip, tcp_conn[id].port);
        }

        return true;
    }

    bool	DEV_GSM_Struct::GPRS_LINK_Open(void)
    {
        int		err, pos = -1;
        bool	ret;
        char	at[16];
        char	ptn[16];
        char	sts[16 + 1];

        DebugOutPrintf(DEBUG_INF, "to open GPRS PPP connection.\r\n");

        sprintf(at, "AT+ZPPPOPEN\r\n");

        // 先检测反馈消息的消息头
        sprintf(ptn, "+ZPPPOPEN:");

        ret = DEV_SendAT(at, ptn, TO_GPRS_SETUP_CONNECTION, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString(ptn, sts, 16, 0, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        DebugOutPrintf(DEBUG_INF, "GPRS PPP Status = %s\r\n", sts);

        pos = KMPMatch(sts, "CONNECTED", __strlen(sts));
        if(pos >= 0)
        {
            DebugOutPrintf(DEBUG_INF, "gprs conenction is opened.\r\n");

            return true;
        }
        pos = KMPMatch(sts, "ESTABLISHED", __strlen(sts));
        if(pos >= 0)
        {
            DebugOutPrintf(DEBUG_INF, "gprs conenction is opened.\r\n");

            return true;
        }
        else
        {
            DebugOutPrintf(DEBUG_INF, "gprs conenction is unknowed.\r\n");

            return false;
        }
    }


    /**
      * @brief  检查指定GPRS连接的连接状态，若返回值为OK，则为连接状态，否则为未连接状态。
      *         注: GPRS连接自动保持超时时间为30分钟，即GPRS连接空闲超过30分钟，GSM基站会将其断开。
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GPRS_SOC_Open(int id, enum DEV_GPRS_Mode mode)
    {
        int		err, pos = -1;
        bool	ret;
        char	at[64];
        char	ptn[32];
        char	sts[16 + 1];

        const char *cmd_at[2] = {"AT+ZIPSETUP=%d,%s,%s\r\n", "AT+ZIPSETUPU=%d,%s,%s\r\n"};
        const char *cmd_ptn[2] = {"+ZIPSETUP:", "+ZIPSETUPU:"};

        DebugOutPrintf(DEBUG_INF, "to setup GPRS #%d connection status.\r\n", id);

        // 建立GPRS连接
        sprintf(at, cmd_at[mode], id, tcp_conn[id].ip, tcp_conn[id].port);

        // 先检测反馈消息的消息头
        sprintf(ptn, cmd_ptn[mode]);	// 加强判断，令它能区别OK与FALL

        // M660+的AT+TCPSETUP命令超时时间已经从19秒减小为10秒，2012-04-19 23:23	to = 10
        ret = DEV_SendAT(at, ptn, TO_GPRS_SETUP_CONNECTION, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchString(ptn, sts, 16, 0, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", 2, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        DebugOutPrintf(DEBUG_INF, "GPRS #%d Status = %s\r\n", id, sts);

        pos = KMPMatch(sts, "CONNECTED", __strlen(sts));
        if(pos >= 0)
        {
            DebugOutPrintf(DEBUG_INF, "gprs conenction is opened.\r\n");

            return true;
        }
        pos = KMPMatch(sts, "ESTABLISHED", __strlen(sts));
        if(pos >= 0)
        {
            DebugOutPrintf(DEBUG_INF, "gprs conenction is opened.\r\n");

            return true;
        }
        else
        {
            DebugOutPrintf(DEBUG_INF, "gprs conenction is unknowed.\r\n");

            return false;
        }
    }

    bool	DEV_GSM_Struct::GPRS_LINK_Close(void)
    {
        int 	err, pos = -1;
        bool 	ret;
        char	at[64];

        sprintf(at, "AT+ZPPPCLOSE\r\n");

        if(DEV_SendAT(at, "+ZPPPCLOSE:", TO_GPRS_CLOSE_CONNECTION, pos, err) == false)
        {
            return ret;
        }

        return true;
    }

    /**
      * @brief  根据指定的GPRS连接号关闭相应的GPRS连接。
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GPRS_SOC_Close(int id, enum DEV_GPRS_Mode mode)
    {
        int 	err, pos = -1;
        bool 	ret;
        char	at[32];

        const char *cmd_at[2] = {"AT+ZIPCLOSE=%d\r\n", "AT+ZIPCLOSEU=%d\r\n"};
        const char *cmd_ptn[2] = {"+ZIPCLOSE:", "+ZIPCLOSEU:"};

        sprintf(at, cmd_at[mode], id);

        //if(DEV_SendAT(at, "+ZIPCLOSE: OK", TO_GPRS_CLOSE_CONNECTION, pos, err) == false)
        if(DEV_SendAT(at, (char *)cmd_ptn[mode], TO_GPRS_CLOSE_CONNECTION, pos, err) == false)
        {
            return ret;
        }

        return true;
    }

    /**
    * @brief  在非透传模式下从指定的GPRS连接上发送给定长度的数据，
    *         并返回实际成功发送的数据长度(即从自动分割分批发送机制)。
    * @param  None
    * @retval None
    */
    bool	DEV_GSM_Struct::GPRS_SOC_TX(int id, char *data, int size, int &pos, int &err, enum DEV_GPRS_Mode mode)
    {
        char	*ptr = data;

        int		n_tobesent 	= 0;		// 每次待发送的字节数
        int		n_reported	= 0;		// 每次发送后GSM模块反馈的发送成功字节数
        int		n_sentout 	= 0;		// 累计发送成功的字节数

        int		repeat = MAX_TIMES_GPRS_SEND_PACKET;// repat times to sent the identical packet

        char	at[32];					// to accomodate the command header
        //      char	ptn[16];

        int		to;
        bool	ret;

        const char *cmd_at[2] = {"AT+ZIPSEND=%d,%d\r\n", "AT+ZIPSENDU=%d,%d\r\n"};
        const char *cmd_ptn[2] = {"+ZIPSEND", "+ZIPSENDU"};
        // 按照待发送字节数及发送每帧数据的超时计算总发送超时时间。
        to = ((size / MAX_LEN_GPRS_PACKET_SEND) + ((size % MAX_LEN_GPRS_PACKET_SEND) ? 1 : 0)) *
             (TO_GPRS_TX_FRAME + 1);

        DebugOutPrintf(DEBUG_INF, "to send %d bytes within %d seconds.\r\n", size, to);

        // 发送GPRS数据包之前，略过此前接收到但未读的所有数据
        // RxCnt3_rd = RxCnt3_wr;
        // gsm_buf_clear();

        to = DEV_GSM_Timing() + to * 1000;

        // 逐帧发送GPRS数据
        while(size > 0 && DEV_GSM_Timing() < to)
        {
            n_reported = 0;

            // 每次发送的数据长度不能超过过AT+SOCSEND命令允许的最大数据长度(=496字节)
            n_tobesent = size > MAX_LEN_GPRS_PACKET_SEND ? MAX_LEN_GPRS_PACKET_SEND : size;

            // 构造消息头	:加\r\n
            sprintf(at, cmd_at[mode], id, n_tobesent);

            ret = DEV_SendAT(at, (char *)cmd_ptn[mode], TO_GPRS_TX_FRAME, pos, err);
            if(ret == false)
            {
                pos = n_sentout;
                return ret;
            }
            // 查询输入标志 '>' 是否粘连
            ret = DEV_FetchPattern(">", TO_GPRS_TX_FRAME, pos, err);
            if(!ret)
            {
                DebugOutPrintf(DEBUG_INF, "to query data send pattern \">\".\r\n");
                ret = DEV_FindPattern("> ", TO_GPRS_GPCMD, pos, err);
                if(ret == false)
                {
                    pos = n_sentout;
                    return ret;
                }
            }

            DEV_SEND_DATA((char *)ptr, n_tobesent);
            /*
            sprintf(ptn, "+ZIPSEND: OK");
            ret = DEV_FindPattern(ptn, TO_GPRS_TX_FRAME + 4, pos, err);
            if (!ret)
            {
                pos = n_sentout;
                return ret;
            }*/
            n_reported = n_tobesent;

            // 比较期望发送的数据长度和实际发送的数据长度是否一致
            // 注: 仅在反馈的发送字节数不等于期望发送的字节数时才重发，因为这种情况多
            // 出现在PRS连接在传输过程中突然变得不稳定时(如GSM信号突然中断或对方服务器暂时宕机等)，
            // 在此之前若出现异常，则多为GSM硬件/软件出现重大故障，因此没必要即刻重发(会将未发送成功
            // 的数据加入到全局的重发数据队列在择机重发)。
            if(n_reported == n_tobesent)
            {
                size        -= n_reported;		// 剩余数据的长度递减
                ptr         += n_reported;		// 读指针后移
                n_sentout   += n_reported;		// 累计发送字节数递增

                repeat       = MAX_TIMES_GPRS_SEND_PACKET; 	// 为下一个数据帧的发送重置重发次数计数器
            }
            // 刚刚发送的数据长度不等于待发送数据长度时，尝试重发当前数据帧，重发次数由系统设定
            else
            {
                if(!(--repeat))
                {
                    pos = n_sentout;
                    return true;
                }
            }
        }
        pos = n_sentout;
        return true;
    }

    /* +TCPRECV: 0,8,abcd1234
       服务器发送abcd1234，模块收到abcd1234
    */
    /**
      * @brief  在指定的GPRS连接上接收数据?
      *         一般在发送数据后才调用接收数据的函数。
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GPRS_SOC_RX(int id, char *data, int &rlen, int to, int &err, enum DEV_GPRS_Mode mode)
    {
        int 	start, end;
        int		pos = -1, size = 1, len = rlen;

        char	pattern[32];

        bool  	ret;

        const char *cmd_at[2] = {"+ZIPRECV:%d,", "+ZIPRECVU:%d,"};

        assert_param(to > 5);

        DebugOutPrintf(DEBUG_INF, "to receive data on GPRS #%d...\r\n", id);

        // Find "+ZIPRECV:X"",9,0123456789"
        sprintf(pattern, cmd_at[mode], id);

        start = DEV_GSM_Timing();
        ret = DEV_FindPattern(pattern, to, pos, err);
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to find %s.\r\n", pattern);

            rlen = 0;
            err = ER_GSM_GPRS_RECV_ERROR;
            goto gsm_recv_err;
        }

        // Find "+ZIPRECV:X,9"",0123456789"
        ret = DEV_FetchValue(pattern, &size, 2, pos);

        DebugOutPrintf(DEBUG_INF, "%d bytes received.\r\n", size);

        if(ret == false || size <= 0 || size > MAX_LEN_GPRS_FRAMERX)
        {
            DebugOutPrintf(DEBUG_ERR, "length of tcp packet is invalid.\r\n");

            rlen = 0;
            err = ER_GSM_GPRS_RECV_ERROR;
            goto gsm_recv_err;
        }

        // Find "+ZIPRECV:X,9,""0123456789"
        pos += 1; // 忽略掉','
        ret = DEV_FetchHex(pattern, data, len, 2, pos);
        if ((ret == false) || (size > len))
        {
            rlen = len;
            err = ER_GSM_GPRS_RECV_ERROR;
            goto gsm_recv_err;
        }
        // 返回接收到的数据的长度
        rlen = size;
        end = DEV_GSM_Timing();

        DebugOutPrintf(DEBUG_INF, "it took %5d ms to recv %5d bytes data.\r\n",
                       (end - start), rlen);

        return true;
gsm_recv_err:
        return false;
    }

    bool	DEV_GSM_Struct::GSM_SendData(int id, char *data, int size, int &err, enum DEV_GPRS_Mode mode)
    {
        int 	start, end;
        int 	errors = 0;
        int		pos = -1;
        bool	ret;

        errors = 0;
        // 发送完整数据，发送出错(完全发送成功才算成功)最多连续重试三次。
        while(errors < MAX_TIMES_GPRS_SEND_DATA)
        {
            start = DEV_GSM_Timing();
            ret = (int)GPRS_SOC_TX(id, data, size, pos, err, mode);
            end = DEV_GSM_Timing();

            if(!ret || pos != size)
            {
                errors++;

                // 两次发送之间间隔一段时间
                DEV_GSM_Delay(500);
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "it took %5d ms to send out %5d bytes data.\r\n",
                               (end - start), size);

                // 返回发送的数据长度(实际为拷贝)
                return size;
            }
        }
        err = ER_GSM_GPRS_SEND_ERROR;
        return false;
    }

    bool	DEV_GSM_Struct::GSM_ConnnectServerExample(char *data, int len, char *recv, int &rlen, int &err, enum DEV_GPRS_Mode mode)
    {
        int 	start, end, tim = DEV_GSM_Timing();
        int		ret;
        int		id = GPRS_ID0;
        char	ip[MAX_LEN_GPRS_IP];

        // 每次使用gsm模块前强制唤醒。
        DEV_GSM_Wakeup();

        // 通话状态下不能建立gprs连接
        if(is_gsm_calling == true)
        {
            DebugOutPrintf(DEBUG_ERR, "gsm is in calling and can not send data!\r\n");

            ret = false;
            goto gsm_conn_serv_err;
        }

        ret = GSM_GetIP((char *)ip, MAX_LEN_GPRS_IP);
        if (!ret)
        {
            err = ER_GSM_INIT_MDIP;
            goto gsm_conn_serv_err;
        }
        /*
        ret = GSM_GetRSSI((int *) &rssi);
        // 查询RSSI
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to get rssi{%d}.\r\n", rssi);

            err = ER_GSM_RSSI_TOOLOW;
            goto gsm_conn_serv_err;
        }
        else
        {

            DebugOutPrintf(DEBUG_INF, "RSSI = %d\r\n", rssi);
        }*/

        /*
        if (GSM_CheckNetwork() == false)
        {
            DebugOutPrintf(DEBUG_WRN, "unregistered to network.\r\n");

            err = ER_GSM_NETWORK_UNREGISTERED;
            goto gsm_conn_serv_err;
        }
        else
        {
            DebugOutPrintf(DEBUG_INF, "already registered local network.\r\n");
        }*/
        ret = GPRS_LINK_Status();

        if(ret == false)
        {
            start = DEV_GSM_Timing();
            ret = GPRS_LINK_Setup();
            end = DEV_GSM_Timing();
            // 若建立连接失败，则将待发送数据写入Flash中以待重发
            if(ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to open GPRS PPP within %d ms.\r\n", (end - start));

                err = ER_GSM_GPRS_LINK_ERROR;		// 连接失败
                goto gsm_conn_serv_err;
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "it took %5d ms to open GPRS PPP.\r\n", (end - start));
            }
        }

        // 检查指定的GPRS连接是否有效，若无效，则建立之
        ret = GPRS_SOC_Status(id, mode);

        // 若指定GPRS连接当前处于断开状态，则尝试连接(连接函数内部会最多连续重试三次，
        // 三次都失败才返回出错结果)
        if(ret == false)
        {
            start = DEV_GSM_Timing();
            ret = GPRS_SOC_Setup(id, mode);
            end = DEV_GSM_Timing();
            // 若建立连接失败，则将待发送数据写入Flash中以待重发
            if(ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to set up GPRS connection within %d ms.\r\n", (end - start));

                err = ER_GSM_GPRS_LINK_ERROR;		// 连接失败
                goto gsm_conn_serv_err;
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "it took %5d ms to set up GPRS connection.\r\n", (end - start));
            }
        }

        // 循环发送三批数据，每批发送2条消息。
        ret = GSM_SendData(id,  (char *)data, len, err, mode);

        if(ret == false)
        {
            DebugOutPrintf(DEBUG_ERR, "failed to send message1 to server.\r\n");

            err = ER_GSM_GPRS_SEND_TIMEOUT;
            goto gsm_conn_serv_err;
        }
        else
        {
            DebugOutPrintf(DEBUG_INF, "successfully sent message1 to server.\r\n");
        }
        __strnset((char *)recv, 0, rlen);
        ret = GPRS_SOC_RX(id, (char *)recv, rlen, TO_GPRS_RX_FRAME, err, mode);
        if(ret == false && rlen == 0)
        {
            DebugOutPrintf(DEBUG_ERR, "failed to recv data from server.\r\n");
            goto gsm_conn_serv_err;
        }
        else
        {
            DebugOutPrintf(DEBUG_INF, "successfully recv \"%d\" data from server.\r\n", rlen);
            if (rlen > 5)
            {
                DebugOutPrintf(DEBUG_INF, "%X,%X,%X,%X,%X,\r\n%s",
                               recv[0], recv[1], recv[2], recv[3], recv[4], recv);
            }
        }

        ret = GPRS_SOC_Close(id, mode);
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to close gprs #%d connection.\r\n", id);
        }
        /*
        ret = GPRS_LINK_Close();
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_ERR, "failed to close gprs PPP connection.\r\n");
        }*/

        DebugOutPrintf(DEBUG_INF, "it took %dms to complete data communication.\r\n",
                       DEV_GSM_Timing() - tim);
        // 每次使用完gsm后强制休眠以节电。
        DEV_GSM_Sleep();

        return true;
gsm_conn_serv_err:
        rlen = 0;

        ret = GPRS_SOC_Close(id, mode);
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to close gprs #%d connection.\r\n", id);
        }
        /*
        ret = GPRS_LINK_Close();
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to close gprs PPP connection.\r\n");
        }*/

        DebugOutPrintf(DEBUG_INF, "it took %dms to complete data communication.\r\n",
                       DEV_GSM_Timing() - tim);
        // 每次使用完gsm后强制休眠以节电。
        DEV_GSM_Sleep();

        return false;
    }

    // 发送指定的消息到指定的号码，集成了短消息拆分发送和发送失败后自动重发功能。
    // 注: 输入的待发送消息要么以ASCII编码，要么以UTF-8编码，以便统一作为字符串处理。
    // 使用gsm_send_sms函数单次可发送的最大汉字数量为MAX_CHAR_CNC_SMS_CN定义、单次
    // 可发送的最大英文数量不受此宏定义限制，但是受程序可分配的栈空间限制(单次发送269字符没问题)。
    bool	DEV_GSM_Struct::GSM_SendSMS(char *pn, char *ascii_utf8, int &err)
    {
        bool			ret;
        char			pns[MAX_LEN_PN + 1];	// 标准化的gsm号码(包含国家区号前缀)

        int				len	 = 0;	// 待发送的字符数量
        T_SMS_LANGUAGE	language = SMS_LANGUAGE_ENGLISH;

        // 通话状态下可正常发送短信!!!
        if(__strlen(pn) <= 0)
        {
            DebugOutPrintf(DEBUG_ERR, "pn is null and redirect to uart:%s", ascii_utf8);

            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }

        // 检查待发内容是否为空
        len = __strlen(ascii_utf8);

        if(len <= 0)
        {
            DebugOutPrintf(DEBUG_WRN, "reply is null.\r\n");

            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }

        // 格式化gsm号码
        if(DEV_FormatPN(pn) == false)
        {
            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }

        // 将纯gsm号码添加国家区号前缀(发送pdu短信时默认设定号码格式需携带国家区号前缀)。
        __strcpy(pns, gsm_telecode);
        __strcat(pns, pn);

        // 检查待发送内容是否为中文
        if(is_all_ascii(ascii_utf8) == false)
        {
            language = SMS_LANGUAGE_CHINESE;
        }

        DebugOutPrintf(DEBUG_INF, "sms length = %d and is written in %d.\r\n", len, language);

        // 检查待发送的短息字符是否超过允许的最大数量(中文短信内容采用UTF8编码输入、
        // 英文短信内容采用ASCII编码输入，前者每字符占3字节、后者每字符占1字节)
        // 发送的单条短信最大长度为4条级联短信
        if(len > MAX_LEN_CNC_SMS * MAX_NUM_CNC_SMS)
        {
            DebugOutPrintf(DEBUG_WRN, "max. length of a Chinese or English SMS is %d .\r\n",
                           MAX_LEN_CNC_SMS * MAX_NUM_CNC_SMS);

            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }
        DEV_GSM_Wakeup();
        do
        {

            // 固定采用pdu格式发送短信(英文以8bit编码、中文以ucs2编码)
            ret = GSM_SMS_SendPDU(language, pns, ascii_utf8, err);
        }
        while (0);
        DEV_GSM_Sleep();

        if (!ret)
        {
            // 发送出错且错误不是短息内容乱码情况下，缓存当前短信准备重发
            if(err == ER_GSM_SMS_PDU_CHAOS)	// 待发送短息内容中包含不支持的字符编码
            {
                // 对于包含不支持的字符编码的短信，不予重发
                DebugOutPrintf(DEBUG_WRN, "unrecognized(neither ASCII nor UTF8 encoded) character found in SMS content!\r\n");

                goto gsm_send_sms_err;
            }
            else
            {
                DebugOutPrintf(DEBUG_ERR, "failed to send sms.\r\n");

                err = ER_GSM_SMS_FAILED;
                goto gsm_send_sms_err;
            }
        }
        else
        {
            printf("Sent sms to %s.\r\n", pns);

            return true;
        }
gsm_send_sms_err:
        return false;
    }
}

#if (USART_RECEIVE_EN!=0)
/**
  * @brief  Description "This function handles PPP interrupt request."
  * @param  None
  * @retval None
  */
ARMAPI void USART3_IRQHandler(void)
{
    //接收到数据
    if (COM3.Received())
    {
        char ch = COM3.ReadByte();
        textInFIFO << ch;
#if (USART_DEBUG_EN != 0)
        COM1.WriteByte(ch);
#endif
    }
}
#endif


/**
  * @brief  Description "秒中断函数"
  * @param  None
  * @retval None
  */
ARMAPI void TIM7_IRQHandler(void)
{
    if (TIMER7.GetUpdateStatus())
    {
        input.TimoutInc(1);
        LEDIO2.ToggleBits(BSP_LED_CH2);

        TIMER7.ClearUpdate();
    }
}

#endif /* DEV_GSM_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

