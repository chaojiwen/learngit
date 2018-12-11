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
/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<e>�������ڽ���
//	<i>����/�رմ��ڽ��չ���
#define USART_RECEIVE_EN		1
//	</e>
//
//	<e>ʹ��input/output�ı����������
//	<i>��ʹ��������Ļ����޷�ʹ���������ˡ�
#define	OUTPUT_EN				1
//	<o>�ı���������ͻ�������С
#define TEXT_OUT_BUFF_SIZE		0x0800
//		<e>ʹ��input�ı�������
//		<i>ʹ��input�ı���������ռ��һ�����ڴ棬
//		<i>ע��input�������Ļ�������Ҫ�������ģ��һ��ʹ��
//		<i>������ﲻʹ��input����������ô����ģ����޷�ʹ�����뻺����
#define INPUT_EN				1
//		<o>�ı����������ջ�������С
#define TEXT_IN_BUFF_SIZE		0x0800
//		<o>�ı��������������ջ�������С
#define TEXT_SEC_BUFF_SIZE		0x0800
//		</e>
//	</e>
//  
//	<e>�������Դ��ڸ������
//	<i>����/�رմ���1�������
#define USART_DEBUG_EN			1
//	</e>
//
//	<e>����GSM���Ը������
//	<i>����/�ر�GSM�������
#define GSM_DEBUG_EN			1
//	</e>
//
//	<e>����GSM����������
//	<i>����/�ر�GSM�������
#define GSM_ERROR_EN			0
//	</e>
//
//	<e>GSM SMS ��ʼ��
//	<i>����/�ر�GSM SMS ��ʼ��
#define GSM_SMS_EN				0
//	</e>
//
//	<e>GSM WAKEUP/SLEEP ʹ��
//	<i>����/�ر�GSM ���ߺͻ��ѹ���
#define GSM_SLEEP_EN			1
//	</e>
//

//	<o1.0..1> ��ӦGSM[ģ��]
//  <i>ע��Ŀǰֻ��SIM800C������MG2639����ģ��ɹ�ѡ��,����1��2Ŀǰ������ѡ�񣡣��� ��
//  <0=> [SIM800C]ģ��.
//  <1=> [����MG2639]ģ��.
//  <2=> [����1]ģ��.
//  <3=> [����2]ģ��.
#define SIM800C_EN1			1
#define SIM800C_EN			0

//	<<< end of configuration section >>>	


#if (SIM800C_EN==0)

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
#include "stm32glibc_exti.h"
#include "stm32glibc_tim.h"
#include "stm32glibc_usart.h"
#include "stm32glibc_stream.h"
#include "dev_gsm.h"
#include "sms_utils.h"
#include <stdlib.h>
#include <stdio.h>
//! @}



/*============================ TYPES =========================================*/

/*============================ LOCAL VARIABLES ===============================*/


/*============================ GLOBAL VARIABLES ==============================*/


/*============================ EXTERN FUNCTIONS ==============================*/
//#ifdef __cplusplus
//}
//#endif


/* ��ӡ���Ա�־ */
BOOL GSM_DEBUG_Flag = FALSE;// TRUE
namespace device
{
    using namespace periph_c;

#if(OUTPUT_EN != 2)
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
      * @brief  Description "��ʼ��"
      * @param  None
      * @retval None
      */
    int 	DEV_GSM_Struct::DEV_GSM_Init(Func_Delay func1, Func_Systick func2,
                                         int baudrate, int &err , T_GPRS_CONNECTION* ip_com,_SCSStruct * state)
    {  
//  int 	pos;			// �ַ�����λ
        int 	ret, times;
#if (GSM_SMS_EN != 0)
//        int 	lst = 0;
#endif
        int		rssi = -1;
        char	str[64 + 1];
        __SMS_READY = 0;
        __DEV_Delay = func1;
        __DEV_SysTick = func2;
        __DEV_StartupTime = DEV_GSM_Timing();

        sts_gsm_power                     	= false;
        // ��ʼ��gsmӲ��ǰ��Global.is_gsm_ready����ΪFALSE
        is_gsm_ready                      	= false;
        // ��ʼ��gsmӲ��ǰ��Global.is_gsm_calling����ΪFALSE
        is_gsm_calling                    	= false;
        is_gsmring_pending 				  	      = false;
        cnt_gsmring_asserted 				        = 0;

        // ��ʼ��dtmf������ر���
        sts_dtmf_command 					          = STS_DTMF_COMMAND_CALL_WAITING;
        times_input_dtmf_password 		      = 0;
        is_dtmf_detection_enabled 		      = false;
                                            
        cnt_gsm_recovered 					        = 0;

        __strcpy(de_gsm_pnin, "");
        __strcpy(de_gsm_dtmf, "");
        __strcpy(gsm_sca, "");

        // GPRS�������
        __strcpy(gsm_apn,                 	(char *)(GPRS_APN));
        // GPRS/GSM���в���
        __strcpy(gsm_telecode,            	(char *)(GSM_TELECODE));
		
#if 1
        // ��ͼ�����������Ӳ���(TBD)
        __strcpy(tcp_conn[GPRS_ID0].dn,   	ip_com[GPRS_ID0].dn);
        __strcpy(tcp_conn[GPRS_ID0].ip,   	ip_com[GPRS_ID0].ip);
        __strcpy(tcp_conn[GPRS_ID0].port, 	ip_com[GPRS_ID0].port);

        // Ӧ�÷����������Ӳ���
        __strcpy(tcp_conn[GPRS_ID1].dn,   	ip_com[GPRS_ID1].dn);
        __strcpy(tcp_conn[GPRS_ID1].ip,   	ip_com[GPRS_ID1].ip);
        __strcpy(tcp_conn[GPRS_ID1].port, 	ip_com[GPRS_ID1].port);

        // ���������������Ӳ���
        __strcpy(tcp_conn[GPRS_ID2].dn,   	ip_com[GPRS_ID2].dn);
        __strcpy(tcp_conn[GPRS_ID2].ip,   	ip_com[GPRS_ID2].ip);
        __strcpy(tcp_conn[GPRS_ID2].port, 	ip_com[GPRS_ID2].port);
#else	
        // ��ͼ�����������Ӳ���(TBD)
        __strcpy(tcp_conn[GPRS_ID0].dn,   	(char *)(GPRS_ID0_DN));
        __strcpy(tcp_conn[GPRS_ID0].ip,   	(char *)(GPRS_ID0_IP));
        __strcpy(tcp_conn[GPRS_ID0].port, 	(char *)(GPRS_ID0_PORT));

        // Ӧ�÷����������Ӳ���
        __strcpy(tcp_conn[GPRS_ID1].dn,   	(char *)(GPRS_ID1_DN));
        __strcpy(tcp_conn[GPRS_ID1].ip,   	(char *)(GPRS_ID1_IP));
        __strcpy(tcp_conn[GPRS_ID1].port, 	(char *)(GPRS_ID1_PORT));

        // ���������������Ӳ���
        __strcpy(tcp_conn[GPRS_ID2].dn,   	(char *)(GPRS_ID2_DN));
        __strcpy(tcp_conn[GPRS_ID2].ip,   	(char *)(GPRS_ID2_IP));
        __strcpy(tcp_conn[GPRS_ID2].port, 	(char *)(GPRS_ID2_PORT));
#endif
        // ����Ĭ�ϵ���Ȩ��������
        __strcpy(cmd_pwd_sms, 				DEF_CMD_PWD_SMS);
        // �ָ�DTMF��������
        // ϵͳ�л�ͳһ�ֻ�����Ϊ�������Ҵ���ǰ׺�ĸ�ʽ��
        // ��˱�����Flash�е�Ĭ���ֻ����붼ͳһ�������Ҵ���ǰ׺
        __strcpy(dtmf_password, 			DEF_CMD_PWD_DTMF);

        input.Init();
        GSMPWRCTRL.Config(BSP_GSM_PWRCTRL, GPIO_MODE_OUT_PP);
        GSMONOFF.Config(BSP_GSM_ONOFF, GPIO_MODE_OUT_PP);
        GSMWAKEUP.Config(BSP_GSM_WAKEUP, GPIO_MODE_OUT_PP);
        GSMWAKEUP.SetBits(BSP_GSM_WAKEUP);
        GSMRING.Config(BSP_GSM_RING);

        DebugOutPrintf(DEBUG_INF, "##<%d>to initialize GSM module.\r\n", DEV_GSM_Timing());

        // ��ǿ�йض�GSM��Դ���Ա���ϵͳ�쳣������δ���ڹػ�ǰ�ض�GSM��Դʱ���GSM����״̬
        DEV_GSM_PowerDown();

        // ��GSM��Դ
        DEV_GSM_PowerUp();

        // ��GSMģ���ϵ�
        DEV_GSM_OnOff();

        // ��ʱ3��ȴ�GSMӲ����ʼ��
		/* �������ʱȥ�������ڸ���ԭ��ѣ���"+CPIN: READY"����ַ��������������ʱ֮ǰ������
		   ������ʱ���������֮ǰ�յ������ݣ��ַ���Ҳ�㣩������͵�����"+CPIN: READY"һֱ�޷��յ��� */

        //is_gsmring_pending 				  	= false;
        //cnt_gsmring_asserted 				= 0;

        // �ȴ�GSMģ��������ע��
        DebugOutPrintf(DEBUG_INF, "##<%d>to wait for GSM registration...\r\n", DEV_GSM_Timing());
        // ����SIM���Ƿ���룬ģ���ϵ�󶼻����+EIND�����ַ�����
        // �Դ˼��ģ���Ƿ��������(M660+ģ���ע������ʱ��һ��̶�Ϊ10��)��
        //		if(DEV_FindPattern("+CPIN:", TO_GPRS_FIND_MODULE, pos, err) == false)		
/******************************GSM_ZWC9.15����********************************************/	
         DEV_GSM_Delay(15000);	

		      /* ��ӡ������� */
	  	   if(GSM_DEBUG_Flag == TRUE)
				 {
				   printf("1->>>�������ƶ��豸(ME)������״̬��......\r\n");

				 }
				  // ��ѯSIM���Ƿ���λ
        if(GSM_CheckSIM() == false)
        {
            DebugOutPrintf(DEBUG_ERR, "SIM card not detected.\r\n");

					 if(GSM_DEBUG_Flag == TRUE)
				  {
				     printf("���ʧ�ܣ����� ������δ����SIM��......\r\n");

				  }
            DEV_GSM_Exit();
	        	/* ���ÿ�δ�����Ϣ */
			      *state	= _SIM_NotDetected;
            err = ER_GSM_INIT_SIM;		// sim��δ��⵽
            goto dev_gsm_init_err;
        }
				else{
				
				     if(GSM_DEBUG_Flag == TRUE)
				     {
				       printf("SIM����������Ҫ������\r\n");
				     }
				
				    }
				
		    //���SIM������Ӫ��
		   /* ��ӡ������� */
	  	   if(GSM_DEBUG_Flag == TRUE)
				 {
				   printf("2->>>���SIM������Ӫ����......\r\n");
				 }				
				if(GSM_CheckSIM_Carriers() == false)
				{
					 if(GSM_DEBUG_Flag == TRUE)
				  {
				     printf("���ʧ�ܣ����� ������δ����SIM��......\r\n");
				  }
					
				  DebugOutPrintf(DEBUG_ERR, "SIM card not detected.\r\n");
				  DEV_GSM_Exit();
				  goto dev_gsm_init_err;//��ѯSIM������Ӫ��ʧ����������ģ��
				}
				else
				  {
							if(GSM_DEBUG_Flag == TRUE)
				  	    printf("���SIM������Ӫ�̳ɹ�������\r\n");
					
					}

//			//��ѯSIM�����ź�ǿ��
//					/* ��ӡ������� */
//	  	   if(GSM_DEBUG_Flag == TRUE)
//				 {
//				   printf("���SIM���ź�ǿ����......\r\n");
//				 }	
//				if(GSM_CheckSIM_Signal_Strength() == false)
//				{	
//				  DebugOutPrintf(DEBUG_ERR, "SIM card not detected.\r\n");
//					 if(GSM_DEBUG_Flag == TRUE)
//				  {
//				     printf("���ʧ�ܣ����� ������δ����SIM��......\r\n");
//				  }
//				  DEV_GSM_Exit();
//			    goto dev_gsm_init_err;//��ѯSIM�����ź�ǿ��ʧ����������ģ��
//				}
				
			 // ��ѯGSM ʱ�ӹ���
					/* ��ӡ������� */
	  	   if(GSM_DEBUG_Flag == TRUE)
				 {
				   printf("3->>>���GSMʱ�ӹ�����......\r\n");
				 }
        DebugOutPrintf(DEBUG_INF, "##<%d>to get cclk.\r\n", DEV_GSM_Timing());
        if(GSM_GetCCLK((char *)str, 64) == true)
        {
            DebugOutPrintf(DEBUG_INF, "GSM CCLK: %s.\r\n", str);
					  if(GSM_DEBUG_Flag == TRUE)
					  printf("���GSMʱ�ӹ���ɹ�������\r\n");
        }
        else
        {
					printf("GSMģ���쳣���޷���ȡʱ����Ϣ\r\n");
            DebugOutPrintf(DEBUG_WRN, "failed to get GSM CCLK.\r\n");
        }	
							
/****************************************************************************************************/
    // Network��ʼ��
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to get rssi[%d].\r\n", DEV_GSM_Timing(), times);

							/* ��ӡ������� */
	  	    if(GSM_DEBUG_Flag == TRUE)
				  {
				   printf("4->>>���SIM���ź�ǿ����......\r\n");
				  }
            rssi = 1;
            ret = GSM_GetRSSI((int *) &rssi);
            // ��ѯRSSI
            if(ret == true)
            {
						  
                DebugOutPrintf(DEBUG_INF, "RSSI = %d\r\n", rssi);
									/* ��ӡ������� */
	  	        if(GSM_DEBUG_Flag == TRUE)
						   printf("SIM���ź�ǿ��ֵΪ:%d\r\n",rssi);
                break;
            }
            else
            {
										/* ��ӡ������� */
	  	        if(GSM_DEBUG_Flag == TRUE)
							printf("���SIM���ź�ǿ��ʧ�ܣ�����\r\n");	
              DebugOutPrintf(DEBUG_WRN, "failed to get rssi{%d}\r\n", rssi);
              err = ER_GSM_INIT_RSSI;
            }
            DEV_GSM_Delay(200);
        }
        while (times++ <1);
        if (!ret)
        {
            DEV_GSM_Exit();
		    	/* �����������ź�״̬ */
			   *state	= _SIM_NoSignal;
		   	goto dev_gsm_init_err;
        }
		        // GPRS������ʼ��
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to check reg state[%d].\r\n", DEV_GSM_Timing(), times);

							/* ��ӡ������� */
	  	    if(GSM_DEBUG_Flag == TRUE)
				  {
				   printf("5->>>���SIM������ע�ἰ״̬��ѯ��......\r\n");
				  }
            ret = GSM_CheckNetwork();
            if (ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "unregistered to network.\r\n");
							
							/* ��ӡ������� */
	  	       if((GSM_DEBUG_Flag == TRUE)&&(times==(TO_GSM_REGISTRATION / 5)))
				     {
				      printf("SIM������ע��ʧ�ܣ����� ԭ��:SIM��Ƿ��\r\n");
				     }

            }
            else
            {	
								   	/* ��ӡ������� */
	  	        if(GSM_DEBUG_Flag == TRUE)
				      {
				        printf("SIM������ע��ɹ�!!!\r\n");
				      }
                DebugOutPrintf(DEBUG_INF, "already registered local network.\r\n");
                break;
            }
            DEV_GSM_Delay(5 * 1000);
        }
        while (times++ <= TO_GSM_REGISTRATION / 5);

		/* ��⿨״̬ */
        if (!ret)
        {
            DEV_GSM_Exit();
			    /* �����������ź�״̬ */
		    	*state	= _SIM_NoSignal;	
			    goto dev_gsm_init_err;
        }		
				
				
/***************************************************************************************/

				
/*****************************************************************************************/		

        // GPRS������ʼ��
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to check gprs attach[%d].\r\n", DEV_GSM_Timing(), times);

								/* ��ӡ������� */
	  	    if(GSM_DEBUG_Flag == TRUE)
				  {
				   printf("6->>>���SIM�����總��״̬��......\r\n");
				  }
            ret = GSM_CheckGPRSActive();
            if (ret == false)
            {	
              if(GSM_DEBUG_Flag == TRUE)
				       printf("SIM����������ʧ�ܣ�����\r\n");
                DebugOutPrintf(DEBUG_WRN, "unattached to gprs.\r\n");
            }
            else
            {
								 if(GSM_DEBUG_Flag == TRUE)
				       printf("SIM����������ɹ�������\r\n");	
                DebugOutPrintf(DEBUG_INF, "already attached to gprs.\r\n");
                break;
            }
            DEV_GSM_Delay(2000);
        }
        while (times++ <= 1);

		/* ��⿨״̬ */
        if (!ret)
        {
            DEV_GSM_Exit();
			/* ���ÿ�Ƿ��ͣ��״̬ */
			*state	= _SIM_ShutDown;
			goto dev_gsm_init_err;
        }
        // SMS������ʼ��
#if (GSM_SMS_EN != 0)
        DebugOutPrintf(DEBUG_INF, "##<%d>to set for sms mode to text...\r\n", DEV_GSM_Timing());

        do
        {
            times = 0;
            while (times++ <= 10)
            {
                // ��GSMģ��Ķ���Ϣģʽ����Ϊ�ı�ģʽ
                ret = GSM_SetSMSMode(GSM_SMS_MODE_TXT);
                if((ret == true) || (times >= 10))
                {
                    break;
                }
                DEV_GSM_Delay(2000);
                DebugOutPrintf(DEBUG_INF, "##<%d>to try set SMS mode.\r\n", times);
            };

            if (!ret)
            {
                DEV_GSM_Exit();
                DebugOutPrintf(DEBUG_WRN, "failed to set SMS mode to text.\r\n");

                err = ER_GSM_INIT_SMS_MODE;
                goto dev_gsm_init_err;
            }

            // ����Ϣ���ģʽ����Ĭ������(�Ա�GSMģ����յ�����ʱ�����+SMSFLAG��ʾ��Ϣ)��
            // �����������÷���������������ring�ж�
            DebugOutPrintf(DEBUG_INF, "##<%d>to set for cnmi...\r\n", DEV_GSM_Timing());

            // ��ֹ���յ��Ķ����Զ��������ʹ�ϲ�Ӧ�����������ǰ��RING����
            if(DEV_SendAT("AT+CNMI=3,1,0,0,0\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to set CNMI.\r\n");

                DEV_GSM_Exit();

                err = ER_GSM_INIT_SMS_SETTING;
                goto dev_gsm_init_err;
            }
            if (GSM_SetRingPinMode(GSM_RING_MODE_CALL_SMS) == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to set Ring Pin Mode.\r\n");

                DEV_GSM_Exit();

                err = ER_GSM_INIT_SMS_SETTING;
                goto dev_gsm_init_err;
            }
            DebugOutPrintf(DEBUG_INF, "enable outputing received SMS automatically.\r\n");

            //
//            if(GSM_ListSMS(&lst) == false)
//            {
//                DebugOutPrintf(DEBUG_WRN, "failed to list all sms in current memory.\r\n");
//            }

            DebugOutPrintf(DEBUG_WRN, "##<%d>to delete all sms in current memory\r\n", DEV_GSM_Timing());
            
            // ɾ����ǰ���Ŵ洢���е����ж��ţ�������Ž����쳣
            if(DEV_SendAT("AT+CMGD=1,4\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
            {
                DEV_GSM_Exit();

                DebugOutPrintf(DEBUG_WRN, "failed to delete all sms in current memory.\r\n");

                err = ER_GSM_INIT_SMS_DELETE;
                goto dev_gsm_init_err;
            }
            DebugOutPrintf(DEBUG_INF, "##<%d>to set data mode to ASCII.\r\n", DEV_GSM_Timing());
            
            //
            if(GSM_SetSMSMemMode() == false)
            {
                DEV_GSM_Exit();

                DebugOutPrintf(DEBUG_WRN, "failed to choice for the SMS memory.\r\n");

                err = ER_GSM_INIT_SMS_SETTING;
                goto dev_gsm_init_err;
            }

            DebugOutPrintf(DEBUG_INF, "##<%d>to get GSM SMS Center...\r\n", DEV_GSM_Timing());
            // ��ѯ�������ĺ���
            ret = GSM_GetSMSCenter((char *)gsm_imei, STD_LEN_GSM_IMEI - 1);
            if(ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to get GSM SMS Center.\r\n");
            }
            else
            {
                // ���������ĺ��볤���Ƿ�Ϊ14λ
                if(__strlen(gsm_imei) != 14)
                {
                    DebugOutPrintf(DEBUG_WRN, "length of imei is not 14!\r\n");
                }
                else
                {
                    DebugOutPrintf(DEBUG_INF, "GSM SMS Center: %s\r\n", gsm_imei);
                }
            }
        }
        while (false);
#endif	/* defined(GSM_SMS_EN) */

//        // ��ѯGSM ʱ�ӹ���
//        DebugOutPrintf(DEBUG_INF, "##<%d>to get cclk.\r\n", DEV_GSM_Timing());

//        if(GSM_GetCCLK((char *)str, 64) == true)
//        {
//					 
//            DebugOutPrintf(DEBUG_INF, "GSM CCLK: %s.\r\n", str);
//        }
//        else
//        {
//					
//				
//            DebugOutPrintf(DEBUG_WRN, "failed to get GSM CCLK.\r\n");
//        }
        DebugOutPrintf(DEBUG_INF, "##<%d>GSM initialized successfully.\r\n", DEV_GSM_Timing());
	     if(GSM_DEBUG_Flag == TRUE)
			 printf("7->>>��ʼ��GSMģ��ɹ�������\r\n");
        // ��ʼ��gsm�ɹ���Global.is_gsm_ready����ΪTRUE
        is_gsm_ready = true;
		   /* ���ÿ�δ�����Ϣ */
	    	*state	= _SIM_OK;
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
        // ����DTR����
        GSMWAKEUP.SetBits(BSP_GSM_WAKEUP);

        // ����ģ���������ģʽ
        if(DEV_SendAT("AT+ZDSLEEP=1\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
        {
            return;
        }

        // ����DTR����
        GSMWAKEUP.ResetBits(BSP_GSM_WAKEUP);

        // ģ��ֻ���ڿ���ʱ�Ż�������ģʽ����������ݽ���δ������������������
#endif
        DebugOutPrintf(DEBUG_INF, "GSM sleep.\r\n");
    }

    void   	DEV_GSM_Struct::DEV_GSM_Wakeup(void)
    {
#if (GSM_SLEEP_EN != 0)
        // ��鵱ǰDTR�Ƿ����ͣ���������˵��֮ǰģ�鱻�ֶ���������״̬
        if (GSMWAKEUP.ReadOutputDataBit(BSP_GSM_WAKEUP) == 0)
        {
            // ����DTR����
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
      * @brief  Description "���� GSM ���� ���� IP��ַ �˿�"
      * @param  �µ� ���� IP��ַ �˿�
      * @retval None
      */
	void   	DEV_GSM_Struct::DEV_GSM_Config_Data_Network(T_GPRS_CONNECTION* ip_com)
	{
        // ���ݷ����������Ӳ���
        __strcpy(tcp_conn[GPRS_ID0].dn,   	ip_com->dn);
        __strcpy(tcp_conn[GPRS_ID0].ip,   	ip_com->ip);
        __strcpy(tcp_conn[GPRS_ID0].port, 	ip_com->port);

	}
    void DEV_GSM_Struct::DEV_GSM_Set_IP_COM(T_GPRS_CONNECTION* ip_com[MAX_NUM_TCP_CONN])
	{
        // ��ͼ�����������Ӳ���(TBD)
        __strcpy(tcp_conn[GPRS_ID0].dn,   	ip_com[GPRS_ID0]->dn);
        __strcpy(tcp_conn[GPRS_ID0].ip,   	ip_com[GPRS_ID0]->ip);
        __strcpy(tcp_conn[GPRS_ID0].port, 	ip_com[GPRS_ID0]->port);

        // Ӧ�÷����������Ӳ���
        __strcpy(tcp_conn[GPRS_ID1].dn,   	ip_com[GPRS_ID1]->dn);
        __strcpy(tcp_conn[GPRS_ID1].ip,   	ip_com[GPRS_ID1]->ip);
        __strcpy(tcp_conn[GPRS_ID1].port, 	ip_com[GPRS_ID1]->port);

        // ���������������Ӳ���
        __strcpy(tcp_conn[GPRS_ID2].dn,   	ip_com[GPRS_ID2]->dn);
        __strcpy(tcp_conn[GPRS_ID2].ip,   	ip_com[GPRS_ID2]->ip);
        __strcpy(tcp_conn[GPRS_ID2].port, 	ip_com[GPRS_ID2]->port);
	}
	
	
	
void  DEV_GSM_Struct::Hex2Str( const char *sSrc,  char *sDest, int nSrcLen )//��������ת��Ϊ�ַ���
{
	  int  i;  
    char szTmp[3];  
  
    for( i = 0; i < nSrcLen; i++ )  
    {  
        sprintf( szTmp, "%02X", (unsigned char) sSrc[i] );  
        memcpy( &sDest[i * 2], szTmp, 2 );  
    }  
    return ; 
}
	
    /**
      * @brief  Description "���������̹ض�GSMģ��"
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

//        while(1)
//        {
//            if (param)
//            {
////                LEDIO4.ToggleBits(BSP_LED_CH4);
//            }
//            else
//            {
////                LEDIO5.ToggleBits(BSP_LED_CH5);
//            }
//            DEV_GSM_Delay(250);
//        }
    }

    bool	DEV_GSM_Struct::DEV_FindPattern(char *ptn, int to, int &pos, int &err)
    {
        int32_t len = 0;
        // �����������ĺϷ���
        len = __strlen(ptn);

        if(len <= 0)
        {
            err = ER_GSM_PATTERN_NULL;
            return false;
        }
        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
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
		
	
 bool	DEV_GSM_Struct::DEV_FetchPattern_1(char *ptn, int to, int &pos, int &err)
    {
        int32_t len = 0;
        // �����������ĺϷ���
        len = __strlen(ptn);

        if(len <= 0)
        {
            err = ER_GSM_PATTERN_NULL;
            return false;
        }
        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
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
        }while (!input.IsTimout());
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

        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
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
        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
        CmdBuffer[0] = '\0';

        output 	<< at;
        do
        {
            input  	>> CmdBuffer;

            len = __strlen(CmdBuffer);
            if (len > 0)
            {
//               DebugOutPrintf(DEBUG_INF, ">><%d>[%d]%s\r\n", DEV_GSM_Timing(), len, CmdBuffer);

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
				
				pos = KMPMatch((char *)CmdBuffer, "ERROR", len);
                if(pos >= 0)
                {
					err = ER_GSM_AT_RTN_ERR;
					return false;
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
/*
���ܣ�����ATָ��ж�ָ����Ӧ������	
char *at����ʾ����ATָ����׵�ַ
char *ptn����ʾҪ�Ƚ�ATָ����Ӧ������
 int to����ʾATָ��ĳ�ʱʱ��		
*/
 bool	DEV_GSM_Struct::DEV_SendAT_Cmd(char *at, char *ptn, int to, int &pos, int &err, bool ok)	
 {
        int len;
        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
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
                            int pos2 = KMPMatch((char *)&CmdBuffer[pos], "STATE: CONNECT", len - pos);
                            if((pos2 >= 0)&&(pos2<2048))
                            {
                                return true;//�ҵ�STATE: CONNECT OK�����true
                            }
														else
														{
														   return false;//δ�ҵ�STATE: CONNECT OK�����false
														}
                        }
                        err = ER_GSM_AT_MISS_OK;
                    }
                    break;
                }
				
				pos = KMPMatch((char *)CmdBuffer, "ERROR", len);
                if(pos >= 0)
                {
					err = ER_GSM_AT_RTN_ERR;
					return false;
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
 /*
���ܣ�����ATָ��ж�ָ����Ӧ������	
char *at����ʾ����ATָ����׵�ַ
char *ptn����ʾҪ�Ƚ�ATָ����Ӧ������
 int to����ʾATָ��ĳ�ʱʱ��		
*/
 bool	DEV_GSM_Struct::DEV_Send_GPRSAT_Cmd(char *at, char *ptn, int to, int &pos, int &err, bool ok)
 {
      int len;
        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
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
									  return true;

                }
				
				pos = KMPMatch((char *)CmdBuffer, "ERROR", len);
                if(pos >= 0)
                {
					err = ER_GSM_AT_RTN_ERR;
					return false;
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
  /*
���ܣ�����ATָ��ж�ָ����Ӧ������	
char *at����ʾ����ATָ����׵�ַ
char *ptn����ʾҪ�Ƚ�ATָ����Ӧ������
 int to����ʾATָ��ĳ�ʱʱ��		
*/
  	bool  DEV_GSM_Struct::DEV_Send_Data_GPRSAT_Cmd(char *at, char *ptn, int to, int &pos, int &err, bool ok)//����ATָ�� ��������
		{
		
		 int len;
        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
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
									  return true;

                }
				
				pos = KMPMatch((char *)CmdBuffer, "ERROR", len);
                if(pos >= 0)
                {
					err = ER_GSM_AT_RTN_ERR;
					return false;
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
//		int rlen = __strlen(CmdBuffer);
//		if ((pos >= 0) && (rlen > pos))
		if ((str != NULL) && (pos >= 0))
        {
            int i;
            char *ptr = CmdBuffer + pos;
//			len = len > (rlen - pos) ? (rlen - pos) : len;
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
                    // δ���⵽�κ������ַ�ǰ��⵽'-'����Ϊ����ֵ����������
                    if(i == 0)
                    {
                        *val = -1;
                    }
                    // �����⵽'-'����Ϊ�������ַ�ĩβ�ĺ�һ�ַ�
                    else
                    {
                        str[i] = '\0';		// �������ַ������������ַ�����β����

                        break;
                    }
                }
                else if(*ptr >= '0' && *ptr <= '9')
                {
                    // ���α���ʱ���ƶ�����պ�ָ�������ַ�
                    str[i++] = *ptr;

                    if(i >= MAX_DECIMAL_DIGITS_FOR_INT)
                    {
                        str[i] = '\0';		// �������ַ������������ַ�����β����

                        break;
                    }
                }
                else
                {
                    // ���str���α��Ƿ����0�������ڣ���˵��str���Ѿ�����������ַ���
                    // ��ǰ��⵽�������ַ��Ļ�˵�������ַ����Ѿ�������
                    if(i > 0)
                    {
                        str[i] = '\0';		// �������ַ������������ַ�����β����

                        break;
                    }
                    // else: �Թ���ǰ�ķ������ַ��������һ���ַ�
                }
                pos ++;
            }
            while ((*(++ptr) != '\0'));

            str[i] = '\0';		// �������ַ������������ַ�����β����
            // ����⵽�������ַ��ᴮת��Ϊ��ֵ(ע����ֵ��������)
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
//			  printf("���͵�ʮ����������:\n");
        for (nIndex = 0; nIndex < size; nIndex ++)
        {
            USARTx_PUTCHAR::Send(str[nIndex]);
//					  printf("%X ",str[nIndex]);
        }
    }

    // ��ʽ��gsm����(ͳһΪ������������ǰ׺�Ĵ����ָ�ʽ)��
    // ע: gsm������ϵͳ�����ı߽��Ͼ͸�ʽ���������յ����ź󡢽�������������������ǰ��ʽ�����ͷ����룬���Ͷ��ų�ȥǰ��ʽ��Ŀ����롣
    int		DEV_GSM_Struct::DEV_FormatPN(char *pn)
    {
        int 	len = __strlen(pn);
        int		n = -1;

        if(len <= 0)		// ��������
        {
            return 1;
        }
        else if(len == 1)	// TCP����
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
                // ȥ����������
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
/**********���SIM���Ƿ�λ GSM_ZWC9.15*************/
    bool	DEV_GSM_Struct::GSM_CheckSIM(void)
    {
        int err, pos = -1;
        bool ret;

      ret = DEV_SendAT("AT+CPIN?\r\n","+CPIN: READY", TO_GPRS_GPCMD, pos, err, true);//���SIM���Ƿ����GSM_ZWC9.15
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }
/**********���SIM������Ӫ��GSM_ZWC9.15*************/		
		
	bool	DEV_GSM_Struct::GSM_CheckSIM_Carriers(void)
		{
		
			 int err, pos = -1;
        bool ret;

      ret = DEV_SendAT("AT+COPS?\r\n","+COPS:", TO_GPRS_GPCMD, pos, err, true);//���SIM������Ӫ��GSM_ZWC9.15
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;

		}	
/**********��ѯSIM�����ź�ǿ��GSM_ZWC9.15*************/				
	bool DEV_GSM_Struct::GSM_CheckSIM_Signal_Strength(void)//��ѯSIM�����ź�ǿ��
	{
	
	
	  int err, pos = -1;
        bool ret;

      ret = DEV_SendAT("AT+CSQ\r\n","+CSQ:", TO_GPRS_GPCMD, pos, err, true);//��ѯSIM�����ź�ǿ��GSM_ZWC9.15
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
	
	}
	
 bool  DEV_GSM_Struct::GSM_Set_SIM_APN(void)//����SIM����APN 
 {
 
 
   int err, pos = -1;
   bool ret;
	 char	at[16];
	 
	 
	 sprintf(at,"AT+CSTT=\"%s\"\r\n","CMNET");
	 
   ret = DEV_SendAT(at,"", TO_GPRS_SET_APN, pos, err, true);//����SIM����APN(CMNET)	
         
   if (!ret)
   {
      return false;
   }
      return true;
				
 }	 
 bool  DEV_GSM_Struct::GSM_Set_SIM_Wireless_Link(void)//����SIM����������·
 {
	 
  int err, pos = -1;
  bool ret;
  ret = DEV_SendAT("AT+CIICR\r\n","", TO_GPRS_GPCMD, pos, err, true);//����SIM����������·

      
   if (!ret)
   {
    return false;
   }
    return true;

 }
 bool  DEV_GSM_Struct::GSM_Check_SIM_Local_IP(void)//��ñ���IP��ַ
 {
 
  int err, pos = -1;
  bool ret;
  ret = DEV_SendAT("AT+CIFSR\r\n","", TO_GPRS_GPCMD, pos, err, true);//��ñ���IP��ַ   
   if (!ret)
   {
    return false;
   }
    return true;
 
 
 
 }
 bool	DEV_GSM_Struct::GSM_Close_Move_Scenario(void)//�ر��ƶ����� 
 {
     int err, pos = -1;
     bool ret;
   ret =DEV_Send_GPRSAT_Cmd("AT+CIPSHUT\r\n","SHUT OK", TO_GPRS_GPCMD, pos, err, true);
  if (!ret)
   {
    return false;
   }
    return true;
 
 
 }
 bool	DEV_GSM_Struct::GSM_Adhere_Business(void)//����GPRSҵ��
 {
    int err, pos = -1;
     bool ret;
   ret =DEV_Send_GPRSAT_Cmd("AT+CGATT=1\r\n","OK", TO_GPRS_GPCMD, pos, err, true);
  if (!ret)
   {
    return false;
   }
	 else
    return true;
 
 
 
 
 }
 bool	DEV_GSM_Struct::GSM_Connection_Mode(void)//����ΪGPRS����ģʽ
 {
 
     int err, pos = -1;
     bool ret;
   ret =DEV_Send_GPRSAT_Cmd("AT+CIPCSGP=1\r\n","OK", TO_GPRS_GPCMD, pos, err, true);
  if (!ret)
   {
    return false;
   }
    return true;
 
 }
 	
 bool	DEV_GSM_Struct::GSM_Set_Data_Show_Ip(void)//�����ڽ�������ǰ��IPͷ
 {
 
     int err, pos = -1;
     bool ret;
   ret =DEV_Send_GPRSAT_Cmd("AT+CIPHEAD=1\r\n","OK", TO_GPRS_GPCMD, pos, err, true);
  if (!ret)
   {
    return false;
   }
    return true;
 
 }
 
 
 
 
  bool	DEV_GSM_Struct::GSM_CheckNetwork(void)
    {
        int err, pos = -1;
        bool ret;
        int nmode = 1, nstat = 1;
        char ptn[16] = {"+CREG:"};

        // ��������ע�������ṩ���ڵ�ѶϢ��CELL ID��LOCAL ID��
        ret = DEV_SendAT("AT+CREG=2\r\n", "OK", TO_GPRS_RT_OK, pos, err);
        if (!ret)
        {
            return false;
        }
        // ��ѯ����ע��״��
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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

        // ��ѯGPRS�Ƿ���
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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

    bool	DEV_GSM_Struct::GSM_CheckSMSAck(void)
    {
        int err, pos = -1;
        bool ret;

        ret = DEV_SendAT("AT+CNMA\r\n", "OK", TO_GPRS_GPCMD, pos, err);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
        char ptn[16] = {""};

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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool 	DEV_GSM_Struct::GSM_GetSMSCenter(char *smsctr, int len)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CSCA:"};

        ret = DEV_SendAT("AT+CSCA?\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        pos += 2;
        ret = DEV_FetchString(ptn, smsctr, len, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_GetRSSI(int *rssi, enum DEV_GSM_RSSI_Mode mode)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CSQ:"};

        *rssi = 1;
        ret = DEV_SendAT("AT+CSQ\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        ret = DEV_FetchValue(ptn, rssi, 3, pos);//GSM
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        if (__abs(*rssi) == 99)
        {
            err = ER_GSM_RSSI_UNKNOW;
            goto dev_gsm_rssi_err;
        }
        if (__abs(*rssi) < MIN_RSSI_FOR_COMMUNICATION)
        {
            err = ER_GSM_RSSI_TOOLOW;
            goto dev_gsm_rssi_err;
        }
        if (mode != GSM_RSSI_MODE_NONE)
        {
            if (*rssi < 32)
                *rssi = (*rssi) * 2 + (-113);
            else
                *rssi = 0;// ������
        }
        return true;
dev_gsm_rssi_err:
        if (mode != GSM_RSSI_MODE_NONE)
        {
            if (*rssi < 32)
                *rssi = (*rssi) * 2 + (-113);
            else
                *rssi = 0;// ������
        }
        return false;
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    /**
      * @brief  ��GPRS���ӽ����󣬲�ѯģ�������IP��ַ��
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        DebugOutPrintf(DEBUG_INF, "to get module ip:\"%s\".\r\n", ip);

        return true;
    }

    /**
      * @brief  ���ƶ�������������IP��ַ(Ӧ��APN���ú�ִ��)��
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GSM_GetDNS(char *ip, int len_ip, char *dn)
    {
        int 	err, pos = -1;
        bool 	ret;
        char 	at[128];

//        DebugOutPrintf(DEBUG_INF, "to inquire dns...\r\n");

//        ret = DEV_SendAT("AT+ZDNSSERV?\r\n", "+ZDNSSERV:", TO_GPRS_GPCMD, pos, err, true);
//        if (!ret)
//        {
//            return false;
//        }
//        ret = DEV_FetchString("", at, 128, 2, pos);
//        if (!ret)
//        {
//            return false;
//        }
//        if (err == ER_GSM_AT_MISS_OK)
//        {
//            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
//            if (!ret)
//            {
//                return false;
//            }
//        }
//        DebugOutPrintf(DEBUG_INF, "dns:%s\r\n", at);

        pos = -1;
        sprintf(at, "AT+CDNSGIP=\"%s\"\r\n", dn);//������ȡ����������ATָ��

        // OK��+DNS����ͬʱ����
        ret = DEV_SendAT(at, "+CDNSGIP:", TO_GPRS_DO_DNS, pos, err, true);//ƥ����յ������������־ͷ(+CDNSGIP:)
				//���緵�ؽ��Ϊ�� +CDNSGIP: 1,"zjueehome.vicp.net","125.118.70.3"
        // M660+��AT+DNS���ʱ�Ѿ����ò�Ĭ��Ϊ5�룬2012-04-19 23:23
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_ReadSMS(int id, char *sms, int len)
    {
        int 	err, pos = -1, rlen;
        bool 	ret;
        char	at[16];

        sprintf(at, "AT+CMGR=%d\r\n", id);
        ret = DEV_SendAT(at, "+CMGR:", TO_GPRS_DO_DNS, pos, err, true);
        if (!ret)
        {
            return false;
        }
        rlen = __strlen(CmdBuffer);
        if ((pos >= 0) && (rlen > pos))
        {
            int i;
            char *ptr = CmdBuffer + pos;
            len = len > (rlen - pos) ? (rlen - pos) : len;
            for(i = 0; i < len; i++)
            {
                sms[i] = *(ptr ++);
            }

            sms[i++] = '\0';
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
        // ����APN	"AT+NETAPN=%s,\"\",\"\"\r\n"
        sprintf(at, "AT+ZPNUM=\"%s\",\"\",\"\"\r\n", gsm_apn);

        if(DEV_SendAT(at, "OK", TO_GPRS_SET_APN, pos, err) == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to set APN.\r\n");

            return false;
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_SetRingPinMode(enum DEV_GSM_RING_Mode mode)
    {
        int err, pos = -1;
        bool ret;
        int nmode = 1;
        char ptn[16] = {"+ZRINGPINMODE:"};
        const char *at[2] =
        {
            "AT+ZRINGPINMODE=0\r\n",	// old mode
            "AT+ZRINGPINMODE=1\r\n",	// new mode
        };

        ret = DEV_SendAT("AT+ZRINGPINMODE?\r\n", ptn, TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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

    bool	DEV_GSM_Struct::GSM_SetSMSMemMode(int addr)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CPMS:"};

        ret = DEV_SendAT("AT+CPMS?\r\n", ptn, TO_GPRS_GPCMD, pos, err);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_ListSMS(int *no, enum DEV_GSM_SMS_Status sta)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CMGL:"};
        const char *at[5] =
        {
            "AT+CMGL=\"REC UNREAD\"\r\n",
            "AT+CMGL=\"REC READ\"\r\n",
            "AT+CMGL=\"STO UNSENT\"\r\n",
            "AT+CMGL=\"STO SENT\"\r\n",
            "AT+CMGL=\"ALL\"\r\n",
        };
        if (sta >= 5)
            return false;
        ret = DEV_SendAT((char *)at[sta], ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            *no = 0;
            return false;
        }
        *no = 1;
        ret = DEV_FetchValue(ptn, no, 3, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        if (!((*no) > 0 && (*no) < MAX_NUM_SMS_MEM))
        {
            err = ER_GSM_SMS_FAILED;
            return false;
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_DeleteSMS(int &no, int &err)
    {
        int pos = -1;
        bool ret;
        char at[16];

        if (!((no) > 0 && (no) < MAX_NUM_SMS_MEM))
        {
            err = ER_GSM_SMS_FAILED;
            return false;
        }
        // ɾ����ǰ���յ��Ķ��ţ�����洢���ж�����(����Ϊ50��)
        sprintf(at, "AT+CMGD=%d\r\n", no);
        ret = DEV_SendAT((char *)at, "OK", TO_GPRS_GPCMD, pos, err);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
//        char			buf_sca[32];
//        char			buf_da[32];
//        unsigned int	len = 0;
//        unsigned int 	len_encoded = 0;
//        unsigned int 	len_fnl = 0;
//        unsigned char	arg = 0;
//        T_SMS_SUBMIT	smssubmit = {0x00, 0x00, "", 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, "", 0x00, 0x00, 0x00, 0x00, "" };

        // // printf("size = %d, encoded = %s\r\n", size, (char*)encoded);

//        // handle SCA( SCA length counts both sca_str and sca_type, but in unsigned char format)
//        while(*sca == '+')
//        {
//            sca++;							// ignore '+' in the front of phone number
//        }

//        len = __strlen(sca);

//        printf("sca = %s\r\n", sca);

//        if(len == 0)
//        {
//            smssubmit.sca_len = len;

//            len = sprintf(buf_sca, "%02x", smssubmit.sca_len);
//        }
//        else
//        {
//            len = pdu_invert((char *)smssubmit.sca_str, sca, len);

//            smssubmit.sca_len = len / 2 + 1;	// SCA length counts both sca_str in unsigned char and sca_fmt(1 byte)

//            smssubmit.sca_fmt = 0x91;		// international numbering with "86" prefix

//            len = sprintf(buf_sca, "%02x%02x%s", smssubmit.sca_len, smssubmit.sca_fmt, smssubmit.sca_str);
//        }

//        // // printf("buf_sca = %s\r\n", buf_sca);

//        // ����udhiλ
//        smssubmit.tp_udhi = udhi;

//        // handle TP-MTI/VFP etc. as a complete argument
//        arg =   (smssubmit.tp_mti & 3) | ((smssubmit.tp_rd & 1) << 2) | ((smssubmit.tp_vpf & 3) << 3) | \
//                ((smssubmit.tp_srr & 1) << 5) | ((smssubmit.tp_udhi & 1) << 6) | ((smssubmit.tp_rp & 1) << 7);

//        // // printf("arg = %02x\r\n", arg);

//        // handle DA
//        while(*pn == '+')
//        {
//            pn++;								// ignore '+' in the front of phone number
//        }

//        smssubmit.tp_dal = __strlen(pn);		// decimal digit number of destination adress, excluding tp_dat and attached 'F'
//        pdu_invert((char *)smssubmit.tp_das, pn, smssubmit.tp_dal);

//        smssubmit.tp_dat = 0x91;				// internal numbering

//        len = sprintf(buf_da, "%02x%02x%s", smssubmit.tp_dal, smssubmit.tp_dat, smssubmit.tp_das);

//        // // printf("buf_da = %s\r\n", buf_da);

//        // ���ô��������ݲ��õ��ַ������ʽ
//        smssubmit.tp_dcs = dcs;

//        // ����udl
//        if(dcs == SMS_PDU_ENCODING_UCS2)
//        {
//            smssubmit.tp_udl = size;				// ����ucs2���룬udl��¼����δ�������ݵĳ���
//        }
//        else if(dcs == SMS_PDU_ENCODING_7BIT)		// ����7bit���룬udl��¼����δ�������ݵĳ���
//        {
//            if(udhi == 1)
//            {
//                smssubmit.tp_udl = (size * 8 / 7) + 1;	// ���ڼ������ţ�udlΪԭʼ���ݳ���+1
//            }
//            else
//            {
//                smssubmit.tp_udl = (size * 8 / 7);		// ���ڼ������ţ�udl�պ�Ϊԭʼ���ݳ���
//            }
//        }
//        else if(dcs == SMS_PDU_ENCODING_8BIT)
//        {
//            smssubmit.tp_udl = size;				// ����ucs2���룬udl��¼����δ�������ݵĳ���
//        }

//        // ���������Ĵ��������ݵ�uds
//        if(udhi == 1)
//        {
//            ///memcpy((unsigned char *)smssubmit.tp_uds, udh, len_udh);
//            //len_encoded += len_udh;

//            memcpy(smssubmit.tp_uds + len_udh, encoded, size);
//            len_encoded += size;
//            smssubmit.tp_udl += len_udh;
//        }
//        else if(udhi == 0)
//        {
//            memcpy((unsigned char *)smssubmit.tp_uds, encoded, size);
//            len_encoded += size;
//        }

//        // package entire PDU, which should be in ASCII format(PDU dose not include SCA part, 0x1A and \r\b).
//        // 1, 0x1A(unsigned char, 1 size) aattached to PDU string;
//        // 2, \r\n(0x0D, 0x0A) attached further;
////        len = sprintf(pdu, "%s%02x%02x%s%02x%02x%02x%02x",	buf_sca,
////                      arg,
////                      smssubmit.tp_mr,
////                      buf_da,
////                      smssubmit.tp_pid,
////                      smssubmit.tp_dcs,
////                      smssubmit.tp_vp,
////                      smssubmit.tp_udl);
//				
//					len = 0;

//        pdu += len;
//        len_fnl += len;

//        len = pdu_htoa(pdu, (unsigned char *)smssubmit.tp_uds, len_encoded);

//        pdu += len;
//        len_fnl += len;

//        // ���PDU�ַ�����β����0x1A
//        len = sprintf(pdu, "\x1A\r\n");						// �������ֽڲ������ڳ�����

//        len_fnl += len;

//        return ((len_fnl - 3) / 2 - 1 - smssubmit.sca_len);	// ���س�SCA(����sca_len�ֽڵ�sca���ݺ�sca_len�ֽڱ���)�����pdu���ֽ���

        return TRUE;
    }
    /**
      * @brief  ��ָ����������PDU��ʽ��SMS���͸�ָ��Ŀ�귽��
      *         ascii_utf8	- 	��ASCII�����Ӣ�Ĵ����Ͷ������ݻ���UTF-8
      *                         ��������Ĵ����Ͷ������ݡ�
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GSM_SMS_SendPDU(T_SMS_LANGUAGE language, char *pns, char *ascii_utf8, int &err)
    {
//        bool			ret=true;
        // pdu����󳤶�һ��С��320�ֽ�
 //       char			pdu[320]={0};
        // ���ض������Ķ�������(���Ĳ���UCS2���롢Ӣ�Ĳ���7bit����)
        unsigned char	encoded[MAX_LEN_CNC_SMS * MAX_NUM_CNC_SMS];//��Ŷ������ݾ������������
 //       unsigned char	*pencoded = (unsigned char *)encoded;		
				unsigned char	Pn_encoded[44];//�������뾭�����������
        int				len = 0;
        int				rest = 0;//�������ݱ����unicode���ĳ���
		 // 	int				rest_pn = 0;//�绰��������unicode���ĳ���
 //       unsigned int 	tobesent = 0;
 //       unsigned int	len_seg = 0;		// ÿ��pdu���а����Ĵ��û����ݳ���(���ĺ�Ӣ�Ĳ�ͬ)
//        unsigned int	feed;
//        unsigned char	udh[7];				// uder data header
//        unsigned int	len_udh = 0;		// udh����(����ʹ��6�ֽ�udh��Ӣ��ʹ��7�ֽ�udh)
//        int				segment_total = 0;	// �����Ų�ַ��͵�������
//        int				segment_count = 0;	// �����Ų�ַ��͵ĵ�ǰ���(��1��ʼ����)
//				int gg=0;
				char	SMS[520]={0};
				

        // ����������ַ�����ԭʼ����(δ����ǰ)
        rest = __strlen(ascii_utf8);

        DebugOutPrintf(DEBUG_INF, "content to be sent in pdu:%s\r\n", ascii_utf8);
				
				
        // �Ƚ����������ݸ�������������Ϊ�ض��ĸ�ʽ
        if(language == SMS_LANGUAGE_CHINESE)
        {
					   pdu_encode16bit(Pn_encoded, pns, 44);//�Զ��ź������unicode����	
            rest = pdu_encode16bit(encoded, ascii_utf8, rest);//���������ݽ���unicode����
            // �������͵Ķ����������Ƿ������utf8����Ҳ��ascii������ַ���
            if(rest <= 0)
            {
                DebugOutPrintf(DEBUG_ERR, "error in encoding pdu.\r\n");

                err = ER_GSM_SMS_PDU_CHAOS;
                goto gsm_sms_send_pdu_err;
            }

            DebugOutPrintf(DEBUG_INF, "to send %d Chinese characters.\r\n", rest / 2);

 //           len_udh = 6;
          //  len_seg = MAX_BYTE_SMS_PDU - len_udh;
        }
        else if(language == SMS_LANGUAGE_ENGLISH)
        {
            DebugOutPrintf(DEBUG_INF, "to send %d English characters.\r\n", rest);

            rest = pdu_encode8bit((char *)encoded, (char *)ascii_utf8, rest);

            // �������Ͷ����������Ƿ������ASCII�ַ���
            if(rest <= 0)
            {
                err = ER_GSM_SMS_PDU_CHAOS;
                goto gsm_sms_send_pdu_err;
            }

   //         len_udh = 7;
        //    len_seg = MAX_BYTE_SMS_PDU - len_udh;
        }
				
//									printf("���������Ķ�������:\r\n");
//									for(gg=0;gg<rest;gg++)
//									{
//										 printf("%02X",encoded[gg]);

//									}		

              while(rest > 0)
							{
								Hex2Str((char *)encoded,(char *)SMS,rest);		
						    DEV_GSM_Delay(15000);//��ʱ8����
                GSM_SendSMS1((char *)SMS,(char *)Pn_encoded,len);//���Ͷ��ź���								
							  DEV_GSM_Delay(200);	
						    DEV_SEND_DATA("\x1A", 1);
								rest=rest-rest;//ֻ��һ�ζ���
							}
	
//									
//        // �������Ҫ��ֵ�������(ÿ����ֺ�Ķ�����󳤶�ΪMAX_BYTE_SMS_PDU��ȥudh�ĳ���6�ֽ�)
//        if(rest > MAX_BYTE_SMS_PDU)
//        {
//            if(rest % len_seg)
//            {
//                segment_total = rest / len_seg + 1;
//            }
//            else
//            {
//                segment_total = rest / len_seg;
//            }
//        }
//        else	// : ���Ͷ������ݳ���С�����pdu����ʱ�����Լ������ŷ�ʽ����
//        {
//            segment_total = 1;
//        }
//				

//									
//										
//        // ���ڲ���α�����
//        feed = DEV_GSM_Timing();
//        // ��ַ��ͳ�����
//        while(rest > 0)
//        {
//            // ����Ƿ���Ҫ�����Ų�ַ���
//            if(segment_total > 1)
//            {
//                //�������
//                if(rest > len_seg)
//                {
//                    tobesent = len_seg;
//                    rest -= tobesent;
//                    segment_count++;
//                }
//                else
//                {
//                    tobesent = rest;
//                    rest -= tobesent;
//                    segment_count++;
//                }

//                // ����udh(����ʹ��6�ֽ�udh���Ա�ʣ���134�ֽڿ����������������֣�
//                // Ӣ��ʹ��7�ֽ�udh���Ա�udh�����udsһ������7�ֽ���)
//                if(len_udh == 6)
//                {
//                    udh[0] = 0x05;
//                    udh[1] = 0x00;
//                    udh[2] = 0x03;
//                    udh[3] = feed & 0xFF;			// serial number
//                    udh[4] = segment_total;
//                    udh[5] = segment_count;				// �������Ŵ�1��ʼ����
//                }
//                else if(len_udh == 7)
//                {
//                    udh[0] = 0x06;
//                    udh[1] = 0x08;
//                    udh[2] = 0x04;
//                    udh[3] = feed & 0xFF;			// serial number
//                    udh[4] = (feed >> 2) & 0xFF;		// serial number
//                    udh[5] = segment_total;
//                    udh[6] = segment_count;				// �������Ŵ�1��ʼ����
//                }

//                // �������Ķ������ݴ����pdu�ַ���
//                if(language == SMS_LANGUAGE_CHINESE)
//                {
//                    len = GSM_SMS_PDUConstruct((char *)pdu, pns, (char *)gsm_sca,
//                                               SMS_PDU_ENCODING_UCS2, pencoded, tobesent, 1, (unsigned char *)udh, len_udh);

//                    pencoded += tobesent;
//                }
//                else if(language == SMS_LANGUAGE_ENGLISH)
//                {
//                    len = GSM_SMS_PDUConstruct((char *)pdu, pns, (char *)gsm_sca,
//                                               SMS_PDU_ENCODING_8BIT, pencoded, tobesent, 1, (unsigned char *)udh, len_udh);

//                    pencoded += tobesent;
//                }
//            }
//            else
//            {
//                tobesent = rest;
//                rest -= tobesent;

//                if(language == SMS_LANGUAGE_CHINESE)
//                {
//                    len = GSM_SMS_PDUConstruct((char *)pdu, pns, (char *)gsm_sca,
//                                               SMS_PDU_ENCODING_UCS2, pencoded, tobesent, 0, (unsigned char *)NULL, len_udh);

//                    pencoded += tobesent;
//                }
//                else if(language == SMS_LANGUAGE_ENGLISH)
//                {
//                    len = GSM_SMS_PDUConstruct((char *)pdu, pns, (char *)gsm_sca,
//                                               SMS_PDU_ENCODING_8BIT, pencoded, tobesent, 0, (unsigned char *)NULL, len_udh);

//                    pencoded += tobesent;
//                }
//            }
//					 DEV_GSM_Delay(15000);//��ʱ8����
//            ret = GSM_SendSMS1(pdu,(char *)Pn_encoded,len);//���Ͷ��ź���
//            if (!ret)
//            {
//                goto gsm_sms_send_pdu_err;
//            }
//        }
        // ������SMS��ǿ�н�SMS�շ�ģʽ����ΪTXTģʽ�����ڽ���SMS����
//        ret = GSM_SetSMSMode(GSM_SMS_MODE_TXT);
//        if(!ret)
//        {
//            DebugOutPrintf(DEBUG_WRN, "error in setting sms to text mode.\r\n");

//            goto gsm_sms_send_pdu_err;
//        }
        return true;
gsm_sms_send_pdu_err:
        return false;
    }

		//pdu��ת��֮�����Ҫ���͵�����
		//len���͵ĳ���
    bool	DEV_GSM_Struct::GSM_SendSMS1(char* pdu,char* PhoneNumUnicode, int len)
    {
        int		pos = -1;
        bool 	ret=false;
        char	at[58]={0};
				int err=0;
				
				char string[44];//�����绰Unicode����ַ�������
				
			 DEV_SendAT("AT+CSCS=\"UCS2\"\r\n","OK", TO_GPRS_GPCMD, pos, err);//GSM_ZWC 10.11	
				
				// ���SMSģʽ���������Ƿ�ִ�гɹ��������ɹ����������Ϊ��ǰ����SMS PDU����ģʽ��
        // ��ʱ�跢��0x1B��ֹ����ģʽ
        ret = DEV_SendAT("AT+CMGF=1\r\n", "OK", TO_GPRS_GPCMD, pos, err);//GSM_ZWC 10.11
        if(!ret)
        {
            DEV_SEND_DATA("\x1B", 1);
            DebugOutPrintf(DEBUG_ERR, "error in \"AT+CMGF=0\".\r\n");
            return ret;
        }
		 	DEV_SendAT("AT+CSMP=17,167,0,8\r\n", "OK", TO_GPRS_GPCMD, pos, err);//GSM_ZWC 10.11	
				
	    Hex2Str(PhoneNumUnicode,string,sizeof(PhoneNumUnicode)+18);//��Unicode��ת��Ϊ�ַ���(�����ֻ������ Unicode����ַ���)	
		  sprintf(at,"AT+CMGS=\"%s\"\r\n",string); 
			DEV_Send_Data_GPRSAT_Cmd(at, ">", TO_GPRS_GPCMD, pos, err);//GSM_ZWC 10.11		
				
        // ��ѯ�����־ '>' �Ƿ�ճ��
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
				
        // ���Ͷ���
        ret = DEV_SendAT(pdu, "+CMGS:", TO_SMS_TX, pos, err, true);


        return ret;
    }

    /**
      * @brief  ���ָ��GPRS������·������״̬��������ֵΪOK����Ϊ����״̬������Ϊδ����״̬��
      *         ע: GPRS�����Զ����ֳ�ʱʱ��Ϊ30���ӣ���GPRS���ӿ��г���30���ӣ�GSM��վ�Ὣ��Ͽ���
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GPRS_LINK_Status(void)
    {
        int		err, pos = -1;
        bool	ret;
        char	at[16];
    //    char	ptn[16];
    //    char	sts[16 + 1];
        DebugOutPrintf(DEBUG_INF, "to check GPRS PPP data link connection status.\r\n");

        sprintf(at, "AT+CIPSTATUS\r\n");

        ret = DEV_SendAT_Cmd(at, "OK", TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
				else
				{
				   return true;
				}
				
				
				
       
    }

    /**
      * @brief  ���ָ��GPRS���ӵ�����״̬��������ֵΪOK����Ϊ����״̬������Ϊδ����״̬��
      *         ע: GPRS�����Զ����ֳ�ʱʱ��Ϊ30���ӣ���GPRS���ӿ��г���30���ӣ�GSM��վ�Ὣ��Ͽ���
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

        // �ȼ�ⷴ����Ϣ����Ϣͷ
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
                // ����GPRS����ʧ�ܺ���1��������
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

    // ����ָ����GPRS���ӺŽ�����Ӧ��GPRS���ӡ�
    // 1��M660+�����ֽ׶���
    //	  ���������������ĸ��ʺܴ󣬷�֮��Ȼ�����ĳ�ο�����������GPRS���Ӳ�������
    //	  �Ϻõİ취�ǹر�ģ�鹩��Ȼ���ϵ�;
    // 2��GPRS���ӹ���������������룬��GPRS����һ���ʧ�ܣ�
    //    GPRS�Ѿ�����ʱ����������룬��GPRS���Ӳ���Ͽ�;
    //    ͨ�������н���GPRS���ӣ���һ���ʧ��;
    bool	DEV_GSM_Struct::GPRS_SOC_Setup(int id, enum DEV_GPRS_Mode mode)
    {
        int		errors;

        DebugOutPrintf(DEBUG_INF, "to setup gprs connection to %s:%s...\r\n",
                       (char *)tcp_conn[id].ip, (char *)tcp_conn[id].port);

//        // �����������ӵ�GPRS���ò�����IP��ַ�Ƿ�Ϊ�գ�Ϊ�յĻ���ִ����������
//        if(__strlen(tcp_conn[id].ip) <= 0)
//        {
//            if(__strlen(tcp_conn[id].dn) > 0)
//            {
//                // TODO: ...
//                if(GSM_GetDNS((char *)tcp_conn[id].ip, MAX_LEN_GPRS_IP,
//                              (char *)tcp_conn[id].dn) == false)
//                {
//                    DebugOutPrintf(DEBUG_ERR, "failed to convert %s into IP.\r\n", tcp_conn[id].dn);

//                    return false;
//                }
//                else
//                {
//                    DebugOutPrintf(DEBUG_INF, "resolved ip is:%s\r\n", (char *)tcp_conn[id].ip);
//                }
//            }
//            else
//            {
//                DebugOutPrintf(DEBUG_ERR, "both IP and domain name are empty.\r\n");

//                return false;
//            }
//        }

        errors = 0;
        // M660+Ĭ�ϵ�GPRS���ӳ�ʱʱ��ԼΪ18�룬��TCP����ʧ�ܵ�����£����������ǰ���أ�
        // M660+�Ĵ����Դ���æ״̬������ʱ�в���ִ��������ATͨѶ������ó���ȴ���ֱ��M660+
        // ������ȷ�Ľ��(���������ӳɹ�����ʧ��)
        // ����GPRS����ʧ��ʱ�������ԡ�
			  while(errors < MAX_TIMES_GPRS_SETUP_CONNECTION)
        {
            if(GPRS_SOC_Open(id, mode) == false)
            {
                DebugOutPrintf(DEBUG_ERR, "#%d: failed to open GPRS connection to %s:%s.\r\n",
                               errors + 1, tcp_conn[id].ip, tcp_conn[id].port);
                errors++;
                // ����GPRS����ʧ�ܺ���1��������
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

        // �ȼ�ⷴ����Ϣ����Ϣͷ
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
      * @brief  ���ָ��GPRS���ӵ�����״̬��������ֵΪOK����Ϊ����״̬������Ϊδ����״̬��
      *         ע: GPRS�����Զ����ֳ�ʱʱ��Ϊ30���ӣ���GPRS���ӿ��г���30���ӣ�GSM��վ�Ὣ��Ͽ���
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GPRS_SOC_Open(int id, enum DEV_GPRS_Mode mode)
    {
        int		err, pos = -1;
        bool	ret;
        char	at[64];
    //    char	ptn[32];
     //   char	sts[16 + 1];
/************************************GSM_ZWC9.15����**********************************************/	
  // ����GPRS����			
//			sprintf(at,"AT+CIPSTART=\"%s\",\"%s\",\"%s\"\r\n","TCP","zjueehome.vicp.net","5661");
			  if((__strlen(tcp_conn[id].ip) <= 0)&&((__strlen(tcp_conn[id].dn) > 0)))
				{
				 sprintf(at,"AT+CIPSTART=\"%s\",\"%s\",\"%s\"\r\n","TCP",tcp_conn[id].dn,tcp_conn[id].port);//��������zwc
				}
				else
				{
				 sprintf(at,"AT+CIPSTART=\"%s\",\"%s\",\"%s\"\r\n","TCP",tcp_conn[id].ip,tcp_conn[id].port);//IP����
				
				}
		  ret = DEV_Send_GPRSAT_Cmd(at, "CONNECT OK",TO_GPRS_SETUP_CONNECTION, pos, err, true);
        if (!ret)
        {
         return false;
        }
				else
				 return true;

    }

    bool	DEV_GSM_Struct::GPRS_LINK_Close(void)
    {
        int 	err, pos = -1;
        bool 	ret;
        char	at[64];

        sprintf(at, "AT+ZPPPCLOSE\r\n");

		ret = DEV_SendAT(at, "+ZPPPCLOSE:", TO_GPRS_CLOSE_CONNECTION, pos, err);
        if(ret == false)
        {
            return ret;
        }

        return true;
    }

    /**
      * @brief  ����ָ����GPRS���ӺŹر���Ӧ��GPRS���ӡ�
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GPRS_SOC_Close(int id, enum DEV_GPRS_Mode mode)
    {
        int 	err, pos = -1;
        bool 	ret;
        char	at[32];

       sprintf(at,"AT+CIPCLOSE=1\r\n");

		ret = DEV_Send_GPRSAT_Cmd(at,"CLOSE OK", TO_GPRS_CLOSE_CONNECTION, pos, err);
        if(ret == false)
        {
            return false;
        }

        return true;
    }

    /**
    * @brief  �ڷ�͸��ģʽ�´�ָ����GPRS�����Ϸ��͸������ȵ����ݣ�
    *         ������ʵ�ʳɹ����͵����ݳ���(�����Զ��ָ�������ͻ���)��
    * @param  None
    * @retval None
    */
    bool	DEV_GSM_Struct::GPRS_SOC_TX(int id, char *data, int size, int &pos, int &err, enum DEV_GPRS_Mode mode)
    {
        char	*ptr = data;
        int		n_tobesent 	= 0;		// ÿ�δ����͵��ֽ���
        int		n_reported	= 0;		// ÿ�η��ͺ�GSMģ�鷴���ķ��ͳɹ��ֽ���
        int		n_sentout 	= 0;		// �ۼƷ��ͳɹ����ֽ���

        int		repeat = MAX_TIMES_GPRS_SEND_PACKET;// repat times to sent the identical packet

        char	at[32];					// to accomodate the command header
        char	ptn[16];

        int		to;
        bool	ret;
			
		
			
			
        // ���մ������ֽ���������ÿ֡���ݵĳ�ʱ�����ܷ��ͳ�ʱʱ�䡣
        to = ((size / MAX_LEN_GPRS_PACKET_SEND) + ((size % MAX_LEN_GPRS_PACKET_SEND) ? 1 : 0)) *
             (TO_GPRS_TX_FRAME + 1);

        DebugOutPrintf(DEBUG_INF, "to send %d bytes within %d seconds.\r\n", size, to);
        to = DEV_GSM_Timing() + to * 1000;

			  
        // ��֡����GPRS����
        while(size > 0 && DEV_GSM_Timing() < to)
        {
            n_reported = 0;

            // ÿ�η��͵����ݳ��Ȳ��ܳ�����AT+SOCSEND���������������ݳ���(=496�ֽ�)
            n_tobesent = size > MAX_LEN_GPRS_PACKET_SEND ? MAX_LEN_GPRS_PACKET_SEND : size;

            // ������Ϣͷ	:��\r\n
            sprintf(at,"AT+CIPSEND=%d\r\n",n_tobesent);
            ret = DEV_Send_Data_GPRSAT_Cmd(at,">", TO_GPRS_TX_FRAME, pos, err);
            if(ret == false)
            {
                pos = n_sentout;
                return ret;
            }
						else
						{
						 DEV_SEND_DATA((char *)ptr, n_tobesent);//��������
						 
						 DEV_GSM_Delay(200);	
						 USART3->DR=0X1A;
						}	
			/******************9.19���Ե�����********************************/			
				/* �жϷ��͵�ʣ�������Ƿ����1024(1K) */
				if(size > MAX_LEN_GPRS_PACKET_SEND)
				{
					/* ����1K */
					sprintf(ptn, "SEND OK");
					ret = DEV_FindPattern(ptn, TO_GPRS_TX_FRAME + 4, pos, err);
					if (!ret)
					{
						pos = n_sentout;
						return ret;	
					}
				}else
			 {
				/* ���ڵ���1K */
				//�������һ���Ƚ��쳣�������\
				��������·���������ݺ�ģ�齫�᷵��һ����+ZIPSEND: OK�����\
				�����ڷ������������ݽϿ�������ֱ�ӷ��ؽ������ݳ�����Ϣ��+ZIPRECV:��\
				�����������Ϣ���ǵ������Һ��潫������֡�+ZIPSEND: OK�����\
				��ˣ���������������
				
		 	}
            n_reported = n_tobesent;

            // �Ƚ��������͵����ݳ��Ⱥ�ʵ�ʷ��͵����ݳ����Ƿ�һ��
            // ע: ���ڷ����ķ����ֽ����������������͵��ֽ���ʱ���ط�����Ϊ���������
            // ������PRS�����ڴ��������ͻȻ��ò��ȶ�ʱ(��GSM�ź�ͻȻ�жϻ�Է���������ʱ崻���)��
            // �ڴ�֮ǰ�������쳣�����ΪGSMӲ��/��������ش���ϣ����û��Ҫ�����ط�(�Ὣδ���ͳɹ�
            // �����ݼ��뵽ȫ�ֵ��ط����ݶ���������ط�)��
            if(n_reported == n_tobesent)
            {
                size        -= n_reported;		// ʣ�����ݵĳ��ȵݼ�
                ptr         += n_reported;		// ��ָ�����
                n_sentout   += n_reported;		// �ۼƷ����ֽ�������

                repeat       = MAX_TIMES_GPRS_SEND_PACKET; 	// Ϊ��һ������֡�ķ��������ط�����������
            }
            // �ոշ��͵����ݳ��Ȳ����ڴ��������ݳ���ʱ�������ط���ǰ����֡���ط�������ϵͳ�趨
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
       ����������abcd1234��ģ���յ�abcd1234
    */
    /**
      * @brief  ��ָ����GPRS�����Ͻ�������?
      *         һ���ڷ������ݺ�ŵ��ý������ݵĺ�����
      * @param  None
      * @retval None
      */
bool	DEV_GSM_Struct::GPRS_SOC_RX(int id, char *data, int &lens, int to, int &err, enum DEV_GPRS_Mode mode)
{
		int 	start, end;
		int		pos = -1, recvlength = 1, fetchlength = lens;

		char	pattern[32];

		bool  	ret;


//���ڽ��յ�TCPͨ�������뷢�͵�TCP����ͨ����ͬ����������ȡ������TCPͨ�������֤��
		//const char *cmd_at[2] = {"+ZIPRECV:%d,", "+ZIPRECVU:%d,"};
//const char *cmd_at[2] = {"+ZIPRECV:", "+ZIPRECVU:"};
		assert_param(to > 5);
		DebugOutPrintf(DEBUG_INF, "to receive data on GPRS #%d...\r\n", id);
		// Find "+ZIPRECV:X"",9,0123456789"
		//sprintf(pattern, cmd_at[mode], id);
	  sprintf(pattern,"+IPD");
		start = DEV_GSM_Timing();
		ret = DEV_FetchPattern_1(pattern, to, pos, err);
		if(ret == false)
		{
				DebugOutPrintf(DEBUG_WRN, "failed to find %s.\r\n", pattern);

				lens = 0;
				err = ER_GSM_GPRS_RECV_ERROR;
				goto gsm_recv_err;
		}
//		else
//     {
//		   printf("����+IPD�ɹ�������\n");
//		 
//		 }//GSM_ZWC9.22

//pos += 2; //GSM_ZWC9.22

		// Find "+ZIPRECV:X,9"",0123456789"
		ret = DEV_FetchValue(pattern, &recvlength, 2, pos);

		DebugOutPrintf(DEBUG_INF, "%d bytes received.\r\n", recvlength);

		if(ret == false || recvlength <= 0 || recvlength > MAX_LEN_GPRS_FRAMERX)
		{
				DebugOutPrintf(DEBUG_ERR, "length of tcp packet is invalid.\r\n");

				lens = 0;
				err = ER_GSM_GPRS_RECV_ERROR;
				goto gsm_recv_err;
		}

		// Find "+IPD(data length):payload"
		pos += 1; // ���Ե�':'
    fetchlength = recvlength;
		ret = DEV_FetchHex(pattern, data, fetchlength, 2, pos);
		if ((ret == false) || (fetchlength != recvlength))
		{
				lens = fetchlength;
				err = ER_GSM_GPRS_RECV_ERROR;
				goto gsm_recv_err;
		}
		// ���ؽ��յ������ݵĳ���
		lens = fetchlength;
		end = DEV_GSM_Timing();

		DebugOutPrintf(DEBUG_INF, "it took %5d ms to recv %5d bytes data.\r\n",
									 (end - start), lens);
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
        // �����������ݣ����ͳ���(��ȫ���ͳɹ�����ɹ�)��������������Ρ�
        while(errors < MAX_TIMES_GPRS_SEND_DATA)
        {
            start = DEV_GSM_Timing();
            ret = (int)GPRS_SOC_TX(id, data, size, pos, err, mode);
            end = DEV_GSM_Timing();

            if(!ret || pos != size)
            {
                errors++;

                // ���η���֮����һ��ʱ��
              //  DEV_GSM_Delay(500);
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "it took %5d ms to send out %5d bytes data.\r\n",
                               (end - start), size);

                // ���ط��͵����ݳ���(ʵ��Ϊ����)
                return size;
            }
        }
        err = ER_GSM_GPRS_SEND_ERROR;
        return false;
    }

    bool	DEV_GSM_Struct::GSM_RecvSMS(char *pn, char* time, char *sms, int len, int &err)
    {
        int			ret, pos = -1;
        int			sms_index = 1, argc, buf_size;

#define 			ARGS_MAX_COUNT 	8
        const char 	sepr[] = ",\r\n";
        char		*argv[ARGS_MAX_COUNT];
        char		bufs[MAX_LEN_SMS_HEAD + MAX_LEN_CMD + 1];

        // gsmģ���ring�ж��¼��Ƿ������
        if (is_gsmring_pending == true)
        {
            // GSMģ��RING�жϲ����Ĵ���(GSMģ���ϵ����Զ�����һ��RING�ж�)
            if (cnt_gsmring_asserted > 0)
            {
                do
                {
                    ret = GSM_ListSMS(&sms_index, GSM_SMS_STA_ALL);
                    if(ret == false)
                    {
                        // û���յ��µĶ���Ϣ(����������)
                        if (sms_index == 0)
                        {
                            cnt_gsmring_asserted --;
                        }
                        DebugOutPrintf(DEBUG_WRN, "failed to list all sms in current memory.\r\n");
                        goto dev_gsm_recv_sms_err;
                    }
                    ret = GSM_ReadSMS(sms_index, bufs, MAX_LEN_SMS_HEAD + MAX_LEN_CMD + 1);
                    if(ret == false)
                    {
                        DebugOutPrintf(DEBUG_WRN, "failed to read sms[%d] in current memory.\r\n", sms_index);
                        goto dev_gsm_recv_sms_err;
                    }
                    // ��ȡ�ɹ�
					buf_size = __strlen(bufs);
                    pos = KMPMatch((char *)bufs, "OK", buf_size);
                    if(pos >= 0)
                    {
						cnt_gsmring_asserted --;
						argc = split_str((char *)bufs, buf_size, sepr, argv, ARGS_MAX_COUNT);
						if (argc <= 4)
						{
							goto dev_gsm_recv_sms_err;
						}
                        __trim_sms(pn,   argv[1]);
						__trim_sms(time, argv[3]);
                        __trim_sms(sms,  argv[4]);
                    }
                    ret = GSM_CheckSMSAck();
                    if(ret == false)
                    {
                        DebugOutPrintf(DEBUG_WRN, "failed to ack sms.\r\n");
                    }
                    // ɾ����ǰ���յ��Ķ��ţ�����洢���ж�����(����Ϊ50��)
                    ret = GSM_DeleteSMS(sms_index, err);
                    if(ret == false)
                    {
                        DebugOutPrintf(DEBUG_WRN, "failed to delete sms[%d] in current memory.\r\n", sms_index);
                        goto dev_gsm_recv_sms_err;
                    }
                }
                while (0);
                return true;
            }
            else
            {
                is_gsmring_pending = false;
                DebugOutPrintf(DEBUG_WRN, "no unread short message.\r\n");
            }
        }
dev_gsm_recv_sms_err:
        return false;
    }
    bool	DEV_GSM_Struct::GSM_ConnnectServerExample(char *send, int slen, char *recv, int &rlen, int &err, enum DEV_GPRS_Mode mode, int id)
    {
        int 	start, end, tim = DEV_GSM_Timing();
        int		ret;
  //      char	ip[MAX_LEN_GPRS_IP];
        __strnset((char *)recv, 0, rlen);
        // ÿ��ʹ��gsmģ��ǰǿ�ƻ��ѡ�
        DEV_GSM_Wakeup();
/************************GSM_ZWC9.18����********************************/			
				ret=GSM_Adhere_Business();//����GPRSҵ��
		  	if(!ret)
				{
					
				 DebugOutPrintf(DEBUG_ERR, "GSMģ��δ���ţ�����\r\n");
				 goto gsm_conn_serv_err;
				}			
				else
				{
			   //   GSM_Connection_Mode();//����ΪGPRS����ģʽ
					  GSM_Set_Data_Show_Ip();//�����ڽ�������ǰ��IPͷ
				}	
        ret = GPRS_LINK_Status();//��ѯSIM��GPRS����״̬	
		 // ��ָ��GPRS���ӵ�ǰ���ڶϿ�״̬����������(���Ӻ����ڲ�����������������Σ�
        // ���ζ�ʧ�ܲŷ��س�����)
        if(ret == false)
        {
            start = DEV_GSM_Timing();
            ret = GPRS_SOC_Setup(id, mode);
            end = DEV_GSM_Timing();
            // ����������ʧ�ܣ��򽫴���������д��Flash���Դ��ط�
            if(ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to set up GPRS connection within %d ms.\r\n", (end - start));

                err = ER_GSM_GPRS_LINK_ERROR;		// ����ʧ��
                goto gsm_conn_serv_err;
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "it took %5d ms to set up GPRS connection.\r\n", (end - start));
            }
        }
				
				if(ret == true)//����TCP���ӳɹ�������
				{
			   	   DebugOutPrintf(DEBUG_INF, "it took %5d ms to set up GPRS connection.\r\n", (end - start));
				    //    printf("TCP���ӳɹ�������\r\n");
									// ѭ�������������ݣ�ÿ������2����Ϣ��
							ret = GSM_SendData(id,  (char *)send, slen, err, mode);
							if(ret == false)
							{
									DebugOutPrintf(DEBUG_ERR, "failed to send message1 to server.\r\n");
								
							//	  printf("��������ʧ�ܣ�����\r\n");
								
									err = ER_GSM_GPRS_SEND_TIMEOUT;
									goto gsm_conn_serv_err;
							}
							else
							{
							//	   printf("�������ݳɹ�������\r\n");
									DebugOutPrintf(DEBUG_INF, "successfully sent message1 to server.\r\n");
							}
					
				}
				else
				{
			   //	 printf("TCP����ʧ�ܣ�����");
				}
/************************************************************************************************/
						//�������Լ���ӵ�һ���ж�
						/* �ж� ���� ���� ���� :�������0 ������ */
						if(rlen != 0)
						{
							ret = GPRS_SOC_RX(id, (char *)recv, rlen, TO_GPRS_RX_FRAME, err, mode);
							if(ret == false && rlen == 0)
							{
								DebugOutPrintf(DEBUG_ERR, "failed to recv data from server.\r\n");
								//goto gsm_conn_serv_err;
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
						}
						
        ret = GPRS_SOC_Close(id, mode);
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to close gprs #%d connection.\r\n", id);
        }
        
//        ret = GPRS_LINK_Close();
//        if(ret == false)
//        {
//            DebugOutPrintf(DEBUG_ERR, "failed to close gprs PPP connection.\r\n");
//        }
//		
//        DebugOutPrintf(DEBUG_INF, "it took %dms to complete data communication.\r\n",
//                       DEV_GSM_Timing() - tim);
        // ÿ��ʹ����gsm��ǿ�������Խڵ硣
//        DEV_GSM_Sleep();

        return true;
gsm_conn_serv_err:
        rlen = 0;
        ret = GPRS_SOC_Close(id, mode);
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to close gprs #%d connection.\r\n", id);
        }
        
//        ret = GPRS_LINK_Close();
//        if(ret == false)
//        {
//            DebugOutPrintf(DEBUG_WRN, "failed to close gprs PPP connection.\r\n");
//        }

//        DebugOutPrintf(DEBUG_INF, "it took %dms to complete data communication.\r\n",
//                       DEV_GSM_Timing() - tim);
//        // ÿ��ʹ����gsm��ǿ�������Խڵ硣
//        DEV_GSM_Sleep();

        return false;
    }

    // ����ָ������Ϣ��ָ���ĺ��룬�����˶���Ϣ��ַ��ͺͷ���ʧ�ܺ��Զ��ط����ܡ�
    // ע: ����Ĵ�������ϢҪô��ASCII���룬Ҫô��UTF-8���룬�Ա�ͳһ��Ϊ�ַ�������
    // ʹ��gsm_send_sms�������οɷ��͵����������ΪMAX_CHAR_CNC_SMS_CN���塢����
    // �ɷ��͵����Ӣ���������ܴ˺궨�����ƣ������ܳ���ɷ����ջ�ռ�����(���η���269�ַ�û����)��
    bool	DEV_GSM_Struct::GSM_SendSMS(char *pn, char *ascii_utf8, int &err)
    {
        bool			ret;
   //     char			pns[MAX_LEN_PN + 1];	// ��׼����gsm����(������������ǰ׺)
        int				len	 = 0;	// �����͵��ַ�����
        T_SMS_LANGUAGE	language = SMS_LANGUAGE_ENGLISH;

        // ͨ��״̬�¿��������Ͷ���!!!
        if(__strlen(pn) <= 0)
        {
            DebugOutPrintf(DEBUG_ERR, "pn is null and redirect to uart:%s", ascii_utf8);

            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }

        // �����������Ƿ�Ϊ��
        len = __strlen(ascii_utf8);

        if(len <= 0)
        {
            DebugOutPrintf(DEBUG_WRN, "reply is null.\r\n");

            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }

				
		/*  2017.10.12���Ե�����
				
	// ��UTF-8������ַ���(UTF8�������ASCII����)ת����UCS2(Unicode Big Endian)������ֽ�����
   finline int utf8_to_ucs2( unsigned char* ucs2, char* utf8, int len)//������ת��ΪUCS2��				
		*/
  
				// ��ʽ��gsm����
        if(DEV_FormatPN(pn) == false)
        {
            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }

        // ����gsm������ӹ�������ǰ׺(����pdu����ʱĬ���趨�����ʽ��Я����������ǰ׺)��
   //     __strcpy(pns, gsm_telecode);
   //    __strcat(pns, pn);

        // �������������Ƿ�Ϊ����
        if(is_all_ascii(ascii_utf8) == false)
        {
            language = SMS_LANGUAGE_CHINESE;
        }

        DebugOutPrintf(DEBUG_INF, "sms length = %d and is written in %d.\r\n", len, language);

        // �������͵Ķ�Ϣ�ַ��Ƿ񳬹�������������(���Ķ������ݲ���UTF8�������롢
        // Ӣ�Ķ������ݲ���ASCII�������룬ǰ��ÿ�ַ�ռ3�ֽڡ�����ÿ�ַ�ռ1�ֽ�)
        // ���͵ĵ���������󳤶�Ϊ4����������
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
            // �̶�����pdu��ʽ���Ͷ���(Ӣ����8bit���롢������ucs2����)
            ret = GSM_SMS_SendPDU(language, pn, ascii_utf8, err);
        }
        while (0);
        DEV_GSM_Sleep();

        if (!ret)
        {
            // ���ͳ����Ҵ����Ƕ�Ϣ������������£����浱ǰ����׼���ط�
            if(err == ER_GSM_SMS_PDU_CHAOS)	// �����Ͷ�Ϣ�����а�����֧�ֵ��ַ�����
            {
                // ���ڰ�����֧�ֵ��ַ�����Ķ��ţ������ط�
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
//            printf("Sent sms to %s.\r\n", pns);

            return true;
        }
gsm_send_sms_err:
        return false;
    }
}




///* ��ӡ���Ա�־ */
//BOOL GSM_DEBUG_Flag = FALSE;// TRUE

#if (USART_RECEIVE_EN!=0)
/**
  * @brief  Description "This function handles PPP interrupt request."
  * @param  None
  * @retval None
  */

//vu16 USART3_RX_STA_1=0; //GSM_ZWC9.15
////���ڽ��ջ����� 	
//u8 USART3_RX_BUF_1[2048];//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.//GSM_ZWC9.15

//ARMAPI void USART3_IRQHandler(void)
//{
//    //���յ�����
//    if (COM3.Received())
//    {
//        char ch = COM3.ReadByte();
//#if (USART_DEBUG_EN != 0)
//		
//		/* ��ӡ������� */
//		if(GSM_DEBUG_Flag == TRUE)
//		{
//			COM1.WriteByte(ch);
//		}
//#endif
//        textInFIFO << ch;
//		
////		USART3_RX_BUF_1[USART3_RX_STA_1++]=COM3.ReadByte();	//��¼���յ���ֵ //GSM_ZWC9.15	
//		
//    }
//}
#endif


/**
  * @brief  Description "���жϺ���"
  * @param  None
  * @retval None
  */
ARMAPI void TIM7_IRQHandler(void)
{
    if (TIMER7.GetUpdateStatus())
    {
        input.TimoutInc(1);
//        LEDIO2.ToggleBits(BSP_LED_CH2);

        TIMER7.ClearUpdate();
    }
}

#endif /* DEV_GSM_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
#endif

#if (SIM800C_EN==1)


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
#include "stm32glibc_exti.h"
#include "stm32glibc_tim.h"
#include "stm32glibc_usart.h"
#include "stm32glibc_stream.h"
#include "dev_gsm.h"
#include "sms_utils.h"
//! @}


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
      * @brief  Description "��ʼ��"
      * @param  None
      * @retval None
      */
    int 	DEV_GSM_Struct::DEV_GSM_Init(Func_Delay func1, Func_Systick func2,
                                         int baudrate, int &err , T_GPRS_CONNECTION* ip_com,_SCSStruct * state)
    {
        int 	pos;			// �ַ�����λ
			 int 	baud_rate;   //���ò�����
        int 	ret, times;
#if (GSM_SMS_EN != 0)
//        int 	lst = 0;
#endif
        int		rssi = -1;
        char	str[64 + 1];
        __SMS_READY = 0;
        __DEV_Delay = func1;
        __DEV_SysTick = func2;
        __DEV_StartupTime = DEV_GSM_Timing();

        sts_gsm_power                     	= false;
        // ��ʼ��gsmӲ��ǰ��Global.is_gsm_ready����ΪFALSE
        is_gsm_ready                      	= false;
        // ��ʼ��gsmӲ��ǰ��Global.is_gsm_calling����ΪFALSE
        is_gsm_calling                    	= false;
        is_gsmring_pending 				  	= false;
        cnt_gsmring_asserted 				= 0;

        // ��ʼ��dtmf������ر���
        sts_dtmf_command 					= STS_DTMF_COMMAND_CALL_WAITING;
        times_input_dtmf_password 			= 0;
        is_dtmf_detection_enabled 			= false;

        cnt_gsm_recovered 					= 0;

        __strcpy(de_gsm_pnin, "");
        __strcpy(de_gsm_dtmf, "");
        __strcpy(gsm_sca, "");

        // GPRS�������
        __strcpy(gsm_apn,                 	(char *)(GPRS_APN));
        // GPRS/GSM���в���
        __strcpy(gsm_telecode,            	(char *)(GSM_TELECODE));
		
#if 1
        // ��ͼ�����������Ӳ���(TBD)
        __strcpy(tcp_conn[GPRS_ID0].dn,   	ip_com[GPRS_ID0].dn);
        __strcpy(tcp_conn[GPRS_ID0].ip,   	ip_com[GPRS_ID0].ip);
        __strcpy(tcp_conn[GPRS_ID0].port, 	ip_com[GPRS_ID0].port);

        // Ӧ�÷����������Ӳ���
        __strcpy(tcp_conn[GPRS_ID1].dn,   	ip_com[GPRS_ID1].dn);
        __strcpy(tcp_conn[GPRS_ID1].ip,   	ip_com[GPRS_ID1].ip);
        __strcpy(tcp_conn[GPRS_ID1].port, 	ip_com[GPRS_ID1].port);

        // ���������������Ӳ���
        __strcpy(tcp_conn[GPRS_ID2].dn,   	ip_com[GPRS_ID2].dn);
        __strcpy(tcp_conn[GPRS_ID2].ip,   	ip_com[GPRS_ID2].ip);
        __strcpy(tcp_conn[GPRS_ID2].port, 	ip_com[GPRS_ID2].port);
#else	
        // ��ͼ�����������Ӳ���(TBD)
        __strcpy(tcp_conn[GPRS_ID0].dn,   	(char *)(GPRS_ID0_DN));
        __strcpy(tcp_conn[GPRS_ID0].ip,   	(char *)(GPRS_ID0_IP));
        __strcpy(tcp_conn[GPRS_ID0].port, 	(char *)(GPRS_ID0_PORT));

        // Ӧ�÷����������Ӳ���
        __strcpy(tcp_conn[GPRS_ID1].dn,   	(char *)(GPRS_ID1_DN));
        __strcpy(tcp_conn[GPRS_ID1].ip,   	(char *)(GPRS_ID1_IP));
        __strcpy(tcp_conn[GPRS_ID1].port, 	(char *)(GPRS_ID1_PORT));

        // ���������������Ӳ���
        __strcpy(tcp_conn[GPRS_ID2].dn,   	(char *)(GPRS_ID2_DN));
        __strcpy(tcp_conn[GPRS_ID2].ip,   	(char *)(GPRS_ID2_IP));
        __strcpy(tcp_conn[GPRS_ID2].port, 	(char *)(GPRS_ID2_PORT));
#endif
        // ����Ĭ�ϵ���Ȩ��������
        __strcpy(cmd_pwd_sms, 				DEF_CMD_PWD_SMS);
        // �ָ�DTMF��������
        // ϵͳ�л�ͳһ�ֻ�����Ϊ�������Ҵ���ǰ׺�ĸ�ʽ��
        // ��˱�����Flash�е�Ĭ���ֻ����붼ͳһ�������Ҵ���ǰ׺
        __strcpy(dtmf_password, 			DEF_CMD_PWD_DTMF);

        input.Init();
        GSMPWRCTRL.Config(BSP_GSM_PWRCTRL, GPIO_MODE_OUT_PP);
        GSMONOFF.Config(BSP_GSM_ONOFF, GPIO_MODE_OUT_PP);
        GSMWAKEUP.Config(BSP_GSM_WAKEUP, GPIO_MODE_OUT_PP);
        GSMWAKEUP.SetBits(BSP_GSM_WAKEUP);
        GSMRING.Config(BSP_GSM_RING);

 //       DebugOutPrintf(DEBUG_INF, "##<%d>to initialize GSM module.\r\n", DEV_GSM_Timing());

        // ��ǿ�йض�GSM��Դ���Ա���ϵͳ�쳣������δ���ڹػ�ǰ�ض�GSM��Դʱ���GSM����״̬
        DEV_GSM_PowerDown();

        // ��GSM��Դ
        DEV_GSM_PowerUp();

        // ��GSMģ���ϵ�
        DEV_GSM_OnOff();

        // ��ʱ3��ȴ�GSMӲ����ʼ��
		/* �������ʱȥ�������ڸ���ԭ��ѣ���"+CPIN: READY"����ַ��������������ʱ֮ǰ������
		   ������ʱ���������֮ǰ�յ������ݣ��ַ���Ҳ�㣩������͵�����"+CPIN: READY"һֱ�޷��յ��� */

        //is_gsmring_pending 				  	= false;
        //cnt_gsmring_asserted 				= 0;

        // �ȴ�GSMģ��������ע��
        DebugOutPrintf(DEBUG_INF, "##<%d>to wait for GSM registration...\r\n", DEV_GSM_Timing());
        // ����SIM���Ƿ���룬ģ���ϵ�󶼻����+EIND�����ַ�����
        // �Դ˼��ģ���Ƿ��������(M660+ģ���ע������ʱ��һ��̶�Ϊ10��)��
        //		if(DEV_FindPattern("+CPIN:", TO_GPRS_FIND_MODULE, pos, err) == false)
				
				
//        if(DEV_FindPattern("+CPIN: READY", TO_GPRS_FIND_MODULE, pos, err) == false)
//        {
//            DebugOutPrintf(DEBUG_ERR, "+CPIN: READY not found.\r\n");

//            DEV_GSM_Exit();
//			/* ���ÿ�δ�����Ϣ */
//			*state	= _SIM_NotDetected;
//            err = ER_GSM_INIT_REGISTRATION;		// ����ע���쳣
//            goto dev_gsm_init_err;
//        }

		
	/* ����Ӧ������ */
		baud_rate = DEV_Adaptive_BaudRate();
		/* �жϲ���������״̬ */
		if(baud_rate == 0)
		{
			/* ����������ʧ�� */
            err = ER_GSM_INIT_BaudRate;
            goto dev_gsm_init_err;
		}
		
		/* �жϵ�ǰ�Ĳ������Ƿ�Ϊ9600 */
		if(baud_rate != 9600)
		{
			/* ��ǰ�Ĳ����ʲ�Ϊ9600 */
			/* ����ģ��Ĳ�����Ϊ9600 */
			ret = GSM_SetBaudRate(9600);
			/* �жϲ���������״̬ */
			if(ret == false)
			{
				/* ����������ʧ�� */
				err = ER_GSM_INIT_BaudRate;
				goto dev_gsm_init_err;
			}
			
			/* ���ô��ڵĲ�����Ϊ9600 */
			COM3.Config(9600);
		}
		
		
        if(DEV_FindPattern("+CPIN: READY", TO_GPRS_FIND_MODULE, pos, err) == false)
        {
            DebugOutPrintf(DEBUG_ERR, "+CPIN: READY not found.\r\n");

            DEV_GSM_Exit();
			/* ���ÿ�δ�����Ϣ */
			*state	= _SIM_NotDetected;
            err = ER_GSM_INIT_REGISTRATION;		// ����ע���쳣
            goto dev_gsm_init_err;
        }		
		
				
        // ���ģ�鲨����
        if(GSM_CheckBaudRate(baudrate) == false)
        {
            DebugOutPrintf(DEBUG_ERR, "Module BaudRate error.\r\n");

            DEV_GSM_Exit();

            err = ER_GSM_UART_RECV_CHAOS;		// ģ�鲨����
            goto dev_gsm_init_err;
        }


        // �ȴ�GSMģ��������ע��
        DebugOutPrintf(DEBUG_INF, "##<%d>to detect SIM card...\r\n", DEV_GSM_Timing());

        // ���SIM���Ƿ����
        if(GSM_CheckSIM() == false)
        {
            DebugOutPrintf(DEBUG_ERR, "SIM card not detected.\r\n");

            DEV_GSM_Exit();

            err = ER_GSM_INIT_SIM;		// sim��δ��⵽
            goto dev_gsm_init_err;
        }


        // ��ѯGSM�̼��汾
        DebugOutPrintf(DEBUG_INF, "##<%d>to get for gsm firmware version...\r\n", DEV_GSM_Timing());
        if(GSM_GetSWVersion(str, 64) == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to get gsm firmware version.\r\n");

            DEV_GSM_Exit();

            err = ER_GSM_INIT_SIM;		// sim��δ��⵽
            goto dev_gsm_init_err;
        }


        // ��ѯIMEI����
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to get GSM IMEI...\r\n", DEV_GSM_Timing());
            // ��ѯIMEI��(GSMģ����ϵ���ѯIMEI����ʧ�ܣ�ԭ����)
            ret = GSM_GetIMEI((char *)gsm_imei, STD_LEN_GSM_IMEI);
            if(ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to get GSM IMEI.\r\n");
            }
            else
            {
                // ���imei�����Ƿ�Ϊ15λ
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
            DEV_GSM_Delay(2000);
        }
        while (times++ <= 1);
        if (!ret)
        {
//            DEV_GSM_Exit();
//            err = ER_GSM_INIT_IMEI1;
//            goto dev_gsm_init_err;
        }

        // ��ѯGSM ʱ�ӹ���
        DebugOutPrintf(DEBUG_INF, "##<%d>to get cclk.\r\n", DEV_GSM_Timing());
        if(GSM_GetCCLK((char *)str, 64) == true)
        {
            DebugOutPrintf(DEBUG_INF, "GSM CCLK: %s.\r\n", str);
        }
        else
        {
            DebugOutPrintf(DEBUG_WRN, "failed to get GSM CCLK.\r\n");
        }


        // Call������ʼ��
        DebugOutPrintf(DEBUG_INF, "##<%d>to enable phone number\r\n", DEV_GSM_Timing());

        // ʹ����������Զ����
        if(DEV_SendAT("AT+CLIP=1\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
        {
            DEV_GSM_Exit();
            DebugOutPrintf(DEBUG_WRN, "failed to enable phone number of incoming call output automatically.\r\n");
            return ER_GSM_INIT_CLIP;
        }


        // Network��ʼ��
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to get rssi[%d].\r\n", DEV_GSM_Timing(), times);

            rssi = 1;
            ret = GSM_GetRSSI((int *) &rssi);
            // ��ѯRSSI
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
            DEV_GSM_Delay(2000);
        }
        while (times++ <= 15);
		
        if (!ret)
        {
            DEV_GSM_Exit();
			/* �����������ź�״̬ */
			*state	= _SIM_NoSignal;
			goto dev_gsm_init_err;
        }

        // GPRS������ʼ��
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to check reg state[%d].\r\n", DEV_GSM_Timing(), times);

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
        while (times++ <= TO_GSM_REGISTRATION / 5);

		/* ��⿨״̬ */
        if (!ret)
        {
            DEV_GSM_Exit();
			/* �����������ź�״̬ */
			*state	= _SIM_NoSignal;	
			goto dev_gsm_init_err;
        }

        // GPRS������ʼ��
        times = 0;
        do
        {
            DebugOutPrintf(DEBUG_INF, "##<%d>to check gprs attach[%d].\r\n", DEV_GSM_Timing(), times);

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
        while (times++ <= 5);

		/* ��⿨״̬ */
        if (!ret)
        {
            DEV_GSM_Exit();
			/* ���ÿ�Ƿ��ͣ��״̬ */
			*state	= _SIM_ShutDown;
			goto dev_gsm_init_err;
        }
		
        /*
        // ���������շ���ΪASCIIģʽ(����ͳ�Ķ�����ģʽ)
        if(gsm_send_at("AT+DATAFORMAT=1,1\r\n", "OK", 2, false) != OK)
        {
        	DEV_GSM_Exit();
        	DebugOutPrintf(DEBUG_WRN, "failed to set data mode to ASCII.\r\n");

        	return ER_GSM_INIT_DATAFORMAT;
        }
        */

        // ����APN
        DebugOutPrintf(DEBUG_INF, "##<%d>to set gprs apn...\r\n", DEV_GSM_Timing());

        if (GSM_SetGPRSAPN() == true)
        {
            DebugOutPrintf(DEBUG_INF, "APN set.\r\n");
        }

        // SMS������ʼ��
#if (GSM_SMS_EN != 0)
        DebugOutPrintf(DEBUG_INF, "##<%d>to set for sms mode to text...\r\n", DEV_GSM_Timing());

        do
        {
            times = 0;
            while (times++ <= 10)
            {
                // ��GSMģ��Ķ���Ϣģʽ����Ϊ�ı�ģʽ
                ret = GSM_SetSMSMode(GSM_SMS_MODE_TXT);
                if((ret == true) || (times >= 10))
                {
                    break;
                }
                DEV_GSM_Delay(2000);
                DebugOutPrintf(DEBUG_INF, "##<%d>to try set SMS mode.\r\n", times);
            };

            if (!ret)
            {
                DEV_GSM_Exit();
                DebugOutPrintf(DEBUG_WRN, "failed to set SMS mode to text.\r\n");

                err = ER_GSM_INIT_SMS_MODE;
                goto dev_gsm_init_err;
            }

            // ����Ϣ���ģʽ����Ĭ������(�Ա�GSMģ����յ�����ʱ�����+SMSFLAG��ʾ��Ϣ)��
            // �����������÷���������������ring�ж�
            DebugOutPrintf(DEBUG_INF, "##<%d>to set for cnmi...\r\n", DEV_GSM_Timing());

            // ��ֹ���յ��Ķ����Զ��������ʹ�ϲ�Ӧ�����������ǰ��RING����
            if(DEV_SendAT("AT+CNMI=3,1,0,0,0\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to set CNMI.\r\n");

                DEV_GSM_Exit();

                err = ER_GSM_INIT_SMS_SETTING;
                goto dev_gsm_init_err;
            }
            if (GSM_SetRingPinMode(GSM_RING_MODE_CALL_SMS) == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to set Ring Pin Mode.\r\n");

                DEV_GSM_Exit();

                err = ER_GSM_INIT_SMS_SETTING;
                goto dev_gsm_init_err;
            }
            DebugOutPrintf(DEBUG_INF, "enable outputing received SMS automatically.\r\n");

            //
//            if(GSM_ListSMS(&lst) == false)
//            {
//                DebugOutPrintf(DEBUG_WRN, "failed to list all sms in current memory.\r\n");
//            }

            DebugOutPrintf(DEBUG_WRN, "##<%d>to delete all sms in current memory\r\n", DEV_GSM_Timing());
            
            // ɾ����ǰ���Ŵ洢���е����ж��ţ�������Ž����쳣
            if(DEV_SendAT("AT+CMGD=1,4\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
            {
                DEV_GSM_Exit();

                DebugOutPrintf(DEBUG_WRN, "failed to delete all sms in current memory.\r\n");

                err = ER_GSM_INIT_SMS_DELETE;
                goto dev_gsm_init_err;
            }
            DebugOutPrintf(DEBUG_INF, "##<%d>to set data mode to ASCII.\r\n", DEV_GSM_Timing());
            
            //
            if(GSM_SetSMSMemMode() == false)
            {
                DEV_GSM_Exit();

                DebugOutPrintf(DEBUG_WRN, "failed to choice for the SMS memory.\r\n");

                err = ER_GSM_INIT_SMS_SETTING;
                goto dev_gsm_init_err;
            }

            DebugOutPrintf(DEBUG_INF, "##<%d>to get GSM SMS Center...\r\n", DEV_GSM_Timing());
            // ��ѯ�������ĺ���
            ret = GSM_GetSMSCenter((char *)gsm_imei, STD_LEN_GSM_IMEI - 1);
            if(ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to get GSM SMS Center.\r\n");
            }
            else
            {
                // ���������ĺ��볤���Ƿ�Ϊ14λ
                if(__strlen(gsm_imei) != 14)
                {
                    DebugOutPrintf(DEBUG_WRN, "length of imei is not 14!\r\n");
                }
                else
                {
                    DebugOutPrintf(DEBUG_INF, "GSM SMS Center: %s\r\n", gsm_imei);
                }
            }
        }
        while (false);
#endif	/* defined(GSM_SMS_EN) */


        // ��ѯGSM ʱ�ӹ���
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

        // ��ʼ��gsm�ɹ���Global.is_gsm_ready����ΪTRUE
        is_gsm_ready = true;
		/* ���ÿ�δ�����Ϣ */
		*state	= _SIM_OK;
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
        // ����DTR����
        GSMWAKEUP.SetBits(BSP_GSM_WAKEUP);

        // ����ģ���������ģʽ
        if(DEV_SendAT("AT+ZDSLEEP=1\r\n", "OK", TO_GPRS_RT_OK, pos, err) == false)
        {
            return;
        }

        // ����DTR����
        GSMWAKEUP.ResetBits(BSP_GSM_WAKEUP);

        // ģ��ֻ���ڿ���ʱ�Ż�������ģʽ����������ݽ���δ������������������
#endif
        DebugOutPrintf(DEBUG_INF, "GSM sleep.\r\n");
    }

    void   	DEV_GSM_Struct::DEV_GSM_Wakeup(void)
    {
#if (GSM_SLEEP_EN != 0)
        // ��鵱ǰDTR�Ƿ����ͣ���������˵��֮ǰģ�鱻�ֶ���������״̬
        if (GSMWAKEUP.ReadOutputDataBit(BSP_GSM_WAKEUP) == 0)
        {
            // ����DTR����
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
      * @brief  Description "���� GSM ���� ���� IP��ַ �˿�"
      * @param  �µ� ���� IP��ַ �˿�
      * @retval None
      */
	void   	DEV_GSM_Struct::DEV_GSM_Config_Data_Network(T_GPRS_CONNECTION* ip_com)
	{
        // ���ݷ����������Ӳ���
        __strcpy(tcp_conn[GPRS_ID0].dn,   	ip_com->dn);
        __strcpy(tcp_conn[GPRS_ID0].ip,   	ip_com->ip);
        __strcpy(tcp_conn[GPRS_ID0].port, 	ip_com->port);

	}
    void DEV_GSM_Struct::DEV_GSM_Set_IP_COM(T_GPRS_CONNECTION* ip_com[MAX_NUM_TCP_CONN])
	{
        // ��ͼ�����������Ӳ���(TBD)
        __strcpy(tcp_conn[GPRS_ID0].dn,   	ip_com[GPRS_ID0]->dn);
        __strcpy(tcp_conn[GPRS_ID0].ip,   	ip_com[GPRS_ID0]->ip);
        __strcpy(tcp_conn[GPRS_ID0].port, 	ip_com[GPRS_ID0]->port);

        // Ӧ�÷����������Ӳ���
        __strcpy(tcp_conn[GPRS_ID1].dn,   	ip_com[GPRS_ID1]->dn);
        __strcpy(tcp_conn[GPRS_ID1].ip,   	ip_com[GPRS_ID1]->ip);
        __strcpy(tcp_conn[GPRS_ID1].port, 	ip_com[GPRS_ID1]->port);

        // ���������������Ӳ���
        __strcpy(tcp_conn[GPRS_ID2].dn,   	ip_com[GPRS_ID2]->dn);
        __strcpy(tcp_conn[GPRS_ID2].ip,   	ip_com[GPRS_ID2]->ip);
        __strcpy(tcp_conn[GPRS_ID2].port, 	ip_com[GPRS_ID2]->port);
	}

    /**
      * @brief  Description "���������̹ض�GSMģ��"
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

//        while(1)
//        {
//            if (param)
//            {
////                LEDIO4.ToggleBits(BSP_LED_CH4);
//            }
//            else
//            {
////                LEDIO5.ToggleBits(BSP_LED_CH5);
//            }
//            DEV_GSM_Delay(250);
//        }
    }

    bool	DEV_GSM_Struct::DEV_FindPattern(char *ptn, int to, int &pos, int &err)
    {
        int32_t len = 0;
        // �����������ĺϷ���
        len = __strlen(ptn);

        if(len <= 0)
        {
            err = ER_GSM_PATTERN_NULL;
            return false;
        }
        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
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

        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
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
        input.TimoutInit(to, TEXT_SEC_BUFF_SIZE);
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
				
				pos = KMPMatch((char *)CmdBuffer, "ERROR", len);
                if(pos >= 0)
                {
					err = ER_GSM_AT_RTN_ERR;
					return false;
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
//		int rlen = __strlen(CmdBuffer);
//		if ((pos >= 0) && (rlen > pos))
		if ((str != NULL) && (pos >= 0))
        {
            int i;
            char *ptr = CmdBuffer + pos;
//			len = len > (rlen - pos) ? (rlen - pos) : len;
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
                    // δ���⵽�κ������ַ�ǰ��⵽'-'����Ϊ����ֵ����������
                    if(i == 0)
                    {
                        *val = -1;
                    }
                    // �����⵽'-'����Ϊ�������ַ�ĩβ�ĺ�һ�ַ�
                    else
                    {
                        str[i] = '\0';		// �������ַ������������ַ�����β����

                        break;
                    }
                }
                else if(*ptr >= '0' && *ptr <= '9')
                {
                    // ���α���ʱ���ƶ�����պ�ָ�������ַ�
                    str[i++] = *ptr;

                    if(i >= MAX_DECIMAL_DIGITS_FOR_INT)
                    {
                        str[i] = '\0';		// �������ַ������������ַ�����β����

                        break;
                    }
                }
                else
                {
                    // ���str���α��Ƿ����0�������ڣ���˵��str���Ѿ�����������ַ���
                    // ��ǰ��⵽�������ַ��Ļ�˵�������ַ����Ѿ�������
                    if(i > 0)
                    {
                        str[i] = '\0';		// �������ַ������������ַ�����β����

                        break;
                    }
                    // else: �Թ���ǰ�ķ������ַ��������һ���ַ�
                }
                pos ++;
            }
            while ((*(++ptr) != '\0'));

            str[i] = '\0';		// �������ַ������������ַ�����β����
            // ����⵽�������ַ��ᴮת��Ϊ��ֵ(ע����ֵ��������)
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

    // ��ʽ��gsm����(ͳһΪ������������ǰ׺�Ĵ����ָ�ʽ)��
    // ע: gsm������ϵͳ�����ı߽��Ͼ͸�ʽ���������յ����ź󡢽�������������������ǰ��ʽ�����ͷ����룬���Ͷ��ų�ȥǰ��ʽ��Ŀ����롣
    int		DEV_GSM_Struct::DEV_FormatPN(char *pn)
    {
        int 	len = __strlen(pn);
        int		n = -1;

        if(len <= 0)		// ��������
        {
            return 1;
        }
        else if(len == 1)	// TCP����
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
                // ȥ����������
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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

        // ��������ע�������ṩ���ڵ�ѶϢ��CELL ID��LOCAL ID��
        ret = DEV_SendAT("AT+CREG=2\r\n", "OK", TO_GPRS_RT_OK, pos, err);
        if (!ret)
        {
            return false;
        }
        // ��ѯ����ע��״��
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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

        // ��ѯGPRS�Ƿ���
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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

    bool	DEV_GSM_Struct::GSM_CheckSMSAck(void)
    {
        int err, pos = -1;
        bool ret;

        ret = DEV_SendAT("AT+CNMA\r\n", "OK", TO_GPRS_GPCMD, pos, err);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool 	DEV_GSM_Struct::GSM_GetSMSCenter(char *smsctr, int len)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CSCA:"};

        ret = DEV_SendAT("AT+CSCA?\r\n", ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            return false;
        }
        pos += 2;
        ret = DEV_FetchString(ptn, smsctr, len, 2, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_GetRSSI(int *rssi, enum DEV_GSM_RSSI_Mode mode)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CSQ:"};

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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        if (__abs(*rssi) == 99)
        {
            err = ER_GSM_RSSI_UNKNOW;
            goto dev_gsm_rssi_err;
        }
        if (__abs(*rssi) < MIN_RSSI_FOR_COMMUNICATION)
        {
            err = ER_GSM_RSSI_TOOLOW;
            goto dev_gsm_rssi_err;
        }
        if (mode != GSM_RSSI_MODE_NONE)
        {
            if (*rssi < 32)
                *rssi = (*rssi) * 2 + (-113);
            else
                *rssi = 0;// ������
        }
        return true;
dev_gsm_rssi_err:
        if (mode != GSM_RSSI_MODE_NONE)
        {
            if (*rssi < 32)
                *rssi = (*rssi) * 2 + (-113);
            else
                *rssi = 0;// ������
        }
        return false;
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    /**
      * @brief  ��GPRS���ӽ����󣬲�ѯģ�������IP��ַ��
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        DebugOutPrintf(DEBUG_INF, "to get module ip:\"%s\".\r\n", ip);

        return true;
    }

    /**
      * @brief  ���ƶ�������������IP��ַ(Ӧ��APN���ú�ִ��)��
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        DebugOutPrintf(DEBUG_INF, "dns:%s\r\n", at);

        pos = -1;
        sprintf(at, "AT+ZDNSGETIP=\"%s\"\r\n", dn);

        // OK��+DNS����ͬʱ����
        ret = DEV_SendAT(at, "+ZDNSGETIP:", TO_GPRS_DO_DNS, pos, err, true);
        // M660+��AT+DNS���ʱ�Ѿ����ò�Ĭ��Ϊ5�룬2012-04-19 23:23
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_ReadSMS(int id, char *sms, int len)
    {
        int 	err, pos = -1, rlen;
        bool 	ret;
        char	at[16];

        sprintf(at, "AT+CMGR=%d\r\n", id);
        ret = DEV_SendAT(at, "+CMGR:", TO_GPRS_DO_DNS, pos, err, true);
        if (!ret)
        {
            return false;
        }
        rlen = __strlen(CmdBuffer);
        if ((pos >= 0) && (rlen > pos))
        {
            int i;
            char *ptr = CmdBuffer + pos;
            len = len > (rlen - pos) ? (rlen - pos) : len;
            for(i = 0; i < len; i++)
            {
                sms[i] = *(ptr ++);
            }

            sms[i++] = '\0';
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
        // ����APN	"AT+NETAPN=%s,\"\",\"\"\r\n"
        sprintf(at, "AT+ZPNUM=\"%s\",\"\",\"\"\r\n", gsm_apn);

        if(DEV_SendAT(at, "OK", TO_GPRS_SET_APN, pos, err) == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to set APN.\r\n");

            return false;
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_SetRingPinMode(enum DEV_GSM_RING_Mode mode)
    {
        int err, pos = -1;
        bool ret;
        int nmode = 1;
        char ptn[16] = {"+ZRINGPINMODE:"};
        const char *at[2] =
        {
            "AT+ZRINGPINMODE=0\r\n",	// old mode
            "AT+ZRINGPINMODE=1\r\n",	// new mode
        };

        ret = DEV_SendAT("AT+ZRINGPINMODE?\r\n", ptn, TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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

    bool	DEV_GSM_Struct::GSM_SetSMSMemMode(int addr)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CPMS:"};

        ret = DEV_SendAT("AT+CPMS?\r\n", ptn, TO_GPRS_GPCMD, pos, err);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_ListSMS(int *no, enum DEV_GSM_SMS_Status sta)
    {
        int err, pos = -1;
        bool ret;
        char ptn[16] = {"+CMGL:"};
        const char *at[5] =
        {
            "AT+CMGL=\"REC UNREAD\"\r\n",
            "AT+CMGL=\"REC READ\"\r\n",
            "AT+CMGL=\"STO UNSENT\"\r\n",
            "AT+CMGL=\"STO SENT\"\r\n",
            "AT+CMGL=\"ALL\"\r\n",
        };
        if (sta >= 5)
            return false;
        ret = DEV_SendAT((char *)at[sta], ptn, TO_GPRS_GPCMD, pos, err, true);
        if (!ret)
        {
            *no = 0;
            return false;
        }
        *no = 1;
        ret = DEV_FetchValue(ptn, no, 3, pos);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
            if (!ret)
            {
                return false;
            }
        }
        if (!((*no) > 0 && (*no) < MAX_NUM_SMS_MEM))
        {
            err = ER_GSM_SMS_FAILED;
            return false;
        }
        return true;
    }

    bool	DEV_GSM_Struct::GSM_DeleteSMS(int &no, int &err)
    {
        int pos = -1;
        bool ret;
        char at[16];

        if (!((no) > 0 && (no) < MAX_NUM_SMS_MEM))
        {
            err = ER_GSM_SMS_FAILED;
            return false;
        }
        // ɾ����ǰ���յ��Ķ��ţ�����洢���ж�����(����Ϊ50��)
        sprintf(at, "AT+CMGD=%d\r\n", no);
        ret = DEV_SendAT((char *)at, "OK", TO_GPRS_GPCMD, pos, err);
        if (!ret)
        {
            return false;
        }
        if (err == ER_GSM_AT_MISS_OK)
        {
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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

//        printf("sca = %s\r\n", sca);

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

        // ����udhiλ
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

        // ���ô��������ݲ��õ��ַ������ʽ
        smssubmit.tp_dcs = dcs;

        // ����udl
        if(dcs == SMS_PDU_ENCODING_UCS2)
        {
            smssubmit.tp_udl = size;				// ����ucs2���룬udl��¼����δ�������ݵĳ���
        }
        else if(dcs == SMS_PDU_ENCODING_7BIT)		// ����7bit���룬udl��¼����δ�������ݵĳ���
        {
            if(udhi == 1)
            {
                smssubmit.tp_udl = (size * 8 / 7) + 1;	// ���ڼ������ţ�udlΪԭʼ���ݳ���+1
            }
            else
            {
                smssubmit.tp_udl = (size * 8 / 7);		// ���ڼ������ţ�udl�պ�Ϊԭʼ���ݳ���
            }
        }
        else if(dcs == SMS_PDU_ENCODING_8BIT)
        {
            smssubmit.tp_udl = size;				// ����ucs2���룬udl��¼����δ�������ݵĳ���
        }

        // ���������Ĵ��������ݵ�uds
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

        // ���PDU�ַ�����β����0x1A
        len = sprintf(pdu, "\x1A\r\n");						// �������ֽڲ������ڳ�����

        len_fnl += len;

        return ((len_fnl - 3) / 2 - 1 - smssubmit.sca_len);	// ���س�SCA(����sca_len�ֽڵ�sca���ݺ�sca_len�ֽڱ���)�����pdu���ֽ���
    }
    /**
      * @brief  ��ָ����������PDU��ʽ��SMS���͸�ָ��Ŀ�귽��
      *         ascii_utf8	- 	��ASCII�����Ӣ�Ĵ����Ͷ������ݻ���UTF-8
      *                         ��������Ĵ����Ͷ������ݡ�
      * @param  None
      * @retval None
      */
    bool	DEV_GSM_Struct::GSM_SMS_SendPDU(T_SMS_LANGUAGE language, char *pns, char *ascii_utf8, int &err)
    {
        bool			ret;
        // pdu����󳤶�һ��С��320�ֽ�
        char			pdu[320];
        // ���ض������Ķ�������(���Ĳ���UCS2���롢Ӣ�Ĳ���7bit����)
        unsigned char	encoded[MAX_LEN_CNC_SMS * MAX_NUM_CNC_SMS];
        unsigned char	*pencoded = (unsigned char *)encoded;

        int				len = 0;

        int				rest = 0;
        unsigned int 	tobesent = 0;
        unsigned int	len_seg = 0;		// ÿ��pdu���а����Ĵ��û����ݳ���(���ĺ�Ӣ�Ĳ�ͬ)

        unsigned int	feed;

        unsigned char	udh[7];				// uder data header
        unsigned int	len_udh = 0;		// udh����(����ʹ��6�ֽ�udh��Ӣ��ʹ��7�ֽ�udh)
        int				segment_total = 0;	// �����Ų�ַ��͵�������
        int				segment_count = 0;	// �����Ų�ַ��͵ĵ�ǰ���(��1��ʼ����)

        // ����������ַ�����ԭʼ����(δ����ǰ)
        rest = __strlen(ascii_utf8);

        DebugOutPrintf(DEBUG_INF, "content to be sent in pdu:%s\r\n", ascii_utf8);

        // �Ƚ����������ݸ�������������Ϊ�ض��ĸ�ʽ
        if(language == SMS_LANGUAGE_CHINESE)
        {
            rest = pdu_encode16bit(encoded, ascii_utf8, rest);

            // �������͵Ķ����������Ƿ������utf8����Ҳ��ascii������ַ���
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

            // �������Ͷ����������Ƿ������ASCII�ַ���
            if(rest <= 0)
            {
                err = ER_GSM_SMS_PDU_CHAOS;
                goto gsm_sms_send_pdu_err;
            }

            len_udh = 7;
            len_seg = MAX_BYTE_SMS_PDU - len_udh;
        }

        // �������Ҫ��ֵ�������(ÿ����ֺ�Ķ�����󳤶�ΪMAX_BYTE_SMS_PDU��ȥudh�ĳ���6�ֽ�)
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
        else	// : ���Ͷ������ݳ���С�����pdu����ʱ�����Լ������ŷ�ʽ����
        {
            segment_total = 1;
        }

        // ���ڲ���α�����
        feed = DEV_GSM_Timing();
        // ��ַ��ͳ�����
        while(rest > 0)
        {
            // ����Ƿ���Ҫ�����Ų�ַ���
            if(segment_total > 1)
            {
                //�������
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

                // ����udh(����ʹ��6�ֽ�udh���Ա�ʣ���134�ֽڿ����������������֣�
                // Ӣ��ʹ��7�ֽ�udh���Ա�udh�����udsһ������7�ֽ���)
                if(len_udh == 6)
                {
                    udh[0] = 0x05;
                    udh[1] = 0x00;
                    udh[2] = 0x03;
                    udh[3] = feed & 0xFF;			// serial number
                    udh[4] = segment_total;
                    udh[5] = segment_count;				// �������Ŵ�1��ʼ����
                }
                else if(len_udh == 7)
                {
                    udh[0] = 0x06;
                    udh[1] = 0x08;
                    udh[2] = 0x04;
                    udh[3] = feed & 0xFF;			// serial number
                    udh[4] = (feed >> 2) & 0xFF;		// serial number
                    udh[5] = segment_total;
                    udh[6] = segment_count;				// �������Ŵ�1��ʼ����
                }

                // �������Ķ������ݴ����pdu�ַ���
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
        // ������SMS��ǿ�н�SMS�շ�ģʽ����ΪTXTģʽ�����ڽ���SMS����
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

        // ���SMSģʽ���������Ƿ�ִ�гɹ��������ɹ����������Ϊ��ǰ����SMS PDU����ģʽ��
        // ��ʱ�跢��0x1B��ֹ����ģʽ
        ret = DEV_SendAT("AT+CMGF=0\r\n", "OK", TO_GPRS_GPCMD, pos, err);
        if(!ret)
        {
            DEV_SEND_DATA("\x1B", 1);

            DebugOutPrintf(DEBUG_ERR, "error in \"AT+CMGF=0\".\r\n");

            return ret;
        }

        // ����PDUģʽ���ŵ�����AT+CMGS=������Ҫ����������PDU���ĳ���(������SCA���ֵģ�
        // ��������Global.gsm_sca_len + Global.gsm_sca_fmt + Global.gsm_sca_str)
        // �˳�����ֵ������ȷ������SMS���ͻ�ʧ�ܡ�
        sprintf(at, "AT+CMGS=%d\r", len); 	// ������\r����\r\nȥ����AT���������MTKЭ��ջ��Bug

        // ����Ƿ����SMS PDU����ģʽ����δ���룬�������Ϊ��ǰ������SMS PDU����ģʽ��
        // ��ʱ��Ҫ����0x1B��ֹ����ģʽ
        ret = DEV_SendAT(at, "+CMGS", TO_GPRS_GPCMD, pos, err);
        if(ret == false) // �ȴ�SMS PDU����ָʾ��'>'
        {
            DEV_SEND_DATA("\x1B", 1);

            DebugOutPrintf(DEBUG_ERR, "error in waiting for '>'.\r\n");
            return ret;
        }
        // ��ѯ�����־ '>' �Ƿ�ճ��
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

        // ���Ͷ���
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
      * @brief  ���ָ��GPRS������·������״̬��������ֵΪOK����Ϊ����״̬������Ϊδ����״̬��
      *         ע: GPRS�����Զ����ֳ�ʱʱ��Ϊ30���ӣ���GPRS���ӿ��г���30���ӣ�GSM��վ�Ὣ��Ͽ���
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

        // �ȼ�ⷴ����Ϣ����Ϣͷ
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
      * @brief  ���ָ��GPRS���ӵ�����״̬��������ֵΪOK����Ϊ����״̬������Ϊδ����״̬��
      *         ע: GPRS�����Զ����ֳ�ʱʱ��Ϊ30���ӣ���GPRS���ӿ��г���30���ӣ�GSM��վ�Ὣ��Ͽ���
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

        // �ȼ�ⷴ����Ϣ����Ϣͷ
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
                // ����GPRS����ʧ�ܺ���1��������
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

    // ����ָ����GPRS���ӺŽ�����Ӧ��GPRS���ӡ�
    // 1��M660+�����ֽ׶��Ե���?��쳣���� ��ĳ�ο��������״�GPRS���������Ļ���
    //	  ���������������ĸ��ʺܴ󣬷�֮��Ȼ�����ĳ�ο�����������GPRS���Ӳ�������
    //	  �Ϻõİ취�ǹر�ģ�鹩��Ȼ���ϵ�;
    // 2��GPRS���ӹ���������������룬��GPRS����һ���ʧ�ܣ�
    //    GPRS�Ѿ�����ʱ����������룬��GPRS���Ӳ���Ͽ�;
    //    ͨ�������н���GPRS���ӣ���һ���ʧ��;
    bool	DEV_GSM_Struct::GPRS_SOC_Setup(int id, enum DEV_GPRS_Mode mode)
    {
        int		errors;

        DebugOutPrintf(DEBUG_INF, "to setup gprs connection to %s:%s...\r\n",
                       (char *)tcp_conn[id].ip, (char *)tcp_conn[id].port);

        // �����������ӵ�GPRS���ò�����IP��ַ�Ƿ�Ϊ�գ�Ϊ�յĻ���ִ����������
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
        // M660+Ĭ�ϵ�GPRS���ӳ�ʱʱ��ԼΪ18�룬��TCP����ʧ�ܵ�����£����������ǰ���أ�
        // M660+�Ĵ����Դ���æ״̬������ʱ�в���ִ��������ATͨѶ������ó���ȴ���ֱ��M660+
        // ������ȷ�Ľ��(���������ӳɹ�����ʧ��)
        // ����GPRS����ʧ��ʱ�������ԡ�
        while(errors < MAX_TIMES_GPRS_SETUP_CONNECTION)
        {
            if(GPRS_SOC_Open(id, mode) == false)
            {
                DebugOutPrintf(DEBUG_ERR, "#%d: failed to open GPRS connection to %s:%s.\r\n",
                               errors + 1, tcp_conn[id].ip, tcp_conn[id].port);

                errors++;

                // ����GPRS����ʧ�ܺ���1��������
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

        // �ȼ�ⷴ����Ϣ����Ϣͷ
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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
      * @brief  ���ָ��GPRS���ӵ�����״̬��������ֵΪOK����Ϊ����״̬������Ϊδ����״̬��
      *         ע: GPRS�����Զ����ֳ�ʱʱ��Ϊ30���ӣ���GPRS���ӿ��г���30���ӣ�GSM��վ�Ὣ��Ͽ���
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

        // ����GPRS����
        sprintf(at, cmd_at[mode], id, tcp_conn[id].ip, tcp_conn[id].port);

        // �ȼ�ⷴ����Ϣ����Ϣͷ
        sprintf(ptn, cmd_ptn[mode]);	// ��ǿ�жϣ�����������OK��FALL

        // M660+��AT+TCPSETUP���ʱʱ���Ѿ���19���СΪ10�룬2012-04-19 23:23	to = 10
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
            ret = DEV_FindPattern("OK", TO_GPRS_GPCMD, pos, err);
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

		ret = DEV_SendAT(at, "+ZPPPCLOSE:", TO_GPRS_CLOSE_CONNECTION, pos, err);
        if(ret == false)
        {
            return ret;
        }

        return true;
    }

    /**
      * @brief  ����ָ����GPRS���ӺŹر���Ӧ��GPRS���ӡ�
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

		ret = DEV_SendAT(at, (char *)cmd_ptn[mode], TO_GPRS_CLOSE_CONNECTION, pos, err);
        if(ret == false)
        {
            return false;
        }

        return true;
    }

    /**
    * @brief  �ڷ�͸��ģʽ�´�ָ����GPRS�����Ϸ��͸������ȵ����ݣ�
    *         ������ʵ�ʳɹ����͵����ݳ���(�����Զ��ָ�������ͻ���)��
    * @param  None
    * @retval None
    */
    bool	DEV_GSM_Struct::GPRS_SOC_TX(int id, char *data, int size, int &pos, int &err, enum DEV_GPRS_Mode mode)
    {
        char	*ptr = data;

        int		n_tobesent 	= 0;		// ÿ�δ����͵��ֽ���
        int		n_reported	= 0;		// ÿ�η��ͺ�GSMģ�鷴���ķ��ͳɹ��ֽ���
        int		n_sentout 	= 0;		// �ۼƷ��ͳɹ����ֽ���

        int		repeat = MAX_TIMES_GPRS_SEND_PACKET;// repat times to sent the identical packet

        char	at[32];					// to accomodate the command header
        char	ptn[16];

        int		to;
        bool	ret;

        const char *cmd_at[2] = {"AT+ZIPSEND=%d,%d\r\n", "AT+ZIPSENDU=%d,%d\r\n"};
        const char *cmd_ptn[2] = {"+ZIPSEND=", "+ZIPSENDU="};
        // ���մ������ֽ���������ÿ֡���ݵĳ�ʱ�����ܷ��ͳ�ʱʱ�䡣
        to = ((size / MAX_LEN_GPRS_PACKET_SEND) + ((size % MAX_LEN_GPRS_PACKET_SEND) ? 1 : 0)) *
             (TO_GPRS_TX_FRAME + 1);

        DebugOutPrintf(DEBUG_INF, "to send %d bytes within %d seconds.\r\n", size, to);

        // ����GPRS���ݰ�֮ǰ���Թ���ǰ���յ���δ������������
        // RxCnt3_rd = RxCnt3_wr;
        // gsm_buf_clear();

        to = DEV_GSM_Timing() + to * 1000;

        // ��֡����GPRS����
        while(size > 0 && DEV_GSM_Timing() < to)
        {
            n_reported = 0;

            // ÿ�η��͵����ݳ��Ȳ��ܳ�����AT+SOCSEND���������������ݳ���(=496�ֽ�)
            n_tobesent = size > MAX_LEN_GPRS_PACKET_SEND ? MAX_LEN_GPRS_PACKET_SEND : size;

            // ������Ϣͷ	:��\r\n
            sprintf(at, cmd_at[mode], id, n_tobesent);

            ret = DEV_SendAT(at, (char *)cmd_ptn[mode], TO_GPRS_TX_FRAME, pos, err);
            if(ret == false)
            {
                pos = n_sentout;
                return ret;
            }
            // ��ѯ�����־ '>' �Ƿ�ճ��
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
			/* �жϷ��͵�ʣ�������Ƿ����1024(1K) */
			if(size > MAX_LEN_GPRS_PACKET_SEND)
			{
				/* ����1K */
				sprintf(ptn, "+ZIPSEND: OK");
				ret = DEV_FindPattern(ptn, TO_GPRS_TX_FRAME + 4, pos, err);
				if (!ret)
				{
					pos = n_sentout;
					return ret;	
				}
			}else
			{
				/* ���ڵ���1K */
				//�������һ���Ƚ��쳣�������\
				��������·���������ݺ�ģ�齫�᷵��һ����+ZIPSEND: OK�����\
				�����ڷ������������ݽϿ�������ֱ�ӷ��ؽ������ݳ�����Ϣ��+ZIPRECV:��\
				�����������Ϣ���ǵ������Һ��潫������֡�+ZIPSEND: OK�����\
				��ˣ���������������
				
			}
            n_reported = n_tobesent;

            // �Ƚ��������͵����ݳ��Ⱥ�ʵ�ʷ��͵����ݳ����Ƿ�һ��
            // ע: ���ڷ����ķ����ֽ����������������͵��ֽ���ʱ���ط�����Ϊ���������
            // ������PRS�����ڴ��������ͻȻ��ò��ȶ�ʱ(��GSM�ź�ͻȻ�жϻ�Է���������ʱ崻���)��
            // �ڴ�֮ǰ�������쳣�����ΪGSMӲ��/��������ش���ϣ����û��Ҫ�����ط�(�Ὣδ���ͳɹ�
            // �����ݼ��뵽ȫ�ֵ��ط����ݶ���������ط�)��
            if(n_reported == n_tobesent)
            {
                size        -= n_reported;		// ʣ�����ݵĳ��ȵݼ�
                ptr         += n_reported;		// ��ָ�����
                n_sentout   += n_reported;		// �ۼƷ����ֽ�������

                repeat       = MAX_TIMES_GPRS_SEND_PACKET; 	// Ϊ��һ������֡�ķ��������ط�����������
            }
            // �ոշ��͵����ݳ��Ȳ����ڴ��������ݳ���ʱ�������ط���ǰ����֡���ط�������ϵͳ�趨
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
       ����������abcd1234��ģ���յ�abcd1234
    */
    /**
      * @brief  ��ָ����GPRS�����Ͻ�������?
      *         һ���ڷ������ݺ�ŵ��ý������ݵĺ�����
      * @param  None
      * @retval None
      */
bool	DEV_GSM_Struct::GPRS_SOC_RX(int id, char *data, int &lens, int to, int &err, enum DEV_GPRS_Mode mode)
{
		int 	start, end;
		int		pos = -1, recvlength = 1, fetchlength = lens;

		char	pattern[32];

		bool  	ret;


//���ڽ��յ�TCPͨ�������뷢�͵�TCP����ͨ����ͬ����������ȡ������TCPͨ�������֤��
		//const char *cmd_at[2] = {"+ZIPRECV:%d,", "+ZIPRECVU:%d,"};
const char *cmd_at[2] = {"+ZIPRECV:", "+ZIPRECVU:"};
		assert_param(to > 5);

		DebugOutPrintf(DEBUG_INF, "to receive data on GPRS #%d...\r\n", id);

		// Find "+ZIPRECV:X"",9,0123456789"
		//sprintf(pattern, cmd_at[mode], id);
	sprintf(pattern, cmd_at[mode]);

		start = DEV_GSM_Timing();
		ret = DEV_FindPattern(pattern, to, pos, err);
		if(ret == false)
		{
				DebugOutPrintf(DEBUG_WRN, "failed to find %s.\r\n", pattern);

				lens = 0;
				err = ER_GSM_GPRS_RECV_ERROR;
				goto gsm_recv_err;
		}

pos += 2;

		// Find "+ZIPRECV:X,9"",0123456789"
		ret = DEV_FetchValue(pattern, &recvlength, 2, pos);

		DebugOutPrintf(DEBUG_INF, "%d bytes received.\r\n", recvlength);

		if(ret == false || recvlength <= 0 || recvlength > MAX_LEN_GPRS_FRAMERX)
		{
				DebugOutPrintf(DEBUG_ERR, "length of tcp packet is invalid.\r\n");

				lens = 0;
				err = ER_GSM_GPRS_RECV_ERROR;
				goto gsm_recv_err;
		}

		// Find "+ZIPRECV:X,9,""0123456789"
		pos += 1; // ���Ե�','
fetchlength = recvlength;
		ret = DEV_FetchHex(pattern, data, fetchlength, 2, pos);
		if ((ret == false) || (fetchlength != recvlength))
		{
				lens = fetchlength;
				err = ER_GSM_GPRS_RECV_ERROR;
				goto gsm_recv_err;
		}
		// ���ؽ��յ������ݵĳ���
		lens = fetchlength;
		end = DEV_GSM_Timing();

		DebugOutPrintf(DEBUG_INF, "it took %5d ms to recv %5d bytes data.\r\n",
									 (end - start), lens);

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
        // �����������ݣ����ͳ���(��ȫ���ͳɹ�����ɹ�)��������������Ρ�
        while(errors < MAX_TIMES_GPRS_SEND_DATA)
        {
            start = DEV_GSM_Timing();
            ret = (int)GPRS_SOC_TX(id, data, size, pos, err, mode);
            end = DEV_GSM_Timing();

            if(!ret || pos != size)
            {
                errors++;

                // ���η���֮����һ��ʱ��
                DEV_GSM_Delay(500);
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "it took %5d ms to send out %5d bytes data.\r\n",
                               (end - start), size);

                // ���ط��͵����ݳ���(ʵ��Ϊ����)
                return size;
            }
        }
        err = ER_GSM_GPRS_SEND_ERROR;
        return false;
    }

    bool	DEV_GSM_Struct::GSM_RecvSMS(char *pn, char* time, char *sms, int len, int &err)
    {
        int			ret, pos = -1;
        int			sms_index = 1, argc, buf_size;

#define 			ARGS_MAX_COUNT 	8
        const char 	sepr[] = ",\r\n";
        char		*argv[ARGS_MAX_COUNT];
        char		bufs[MAX_LEN_SMS_HEAD + MAX_LEN_CMD + 1];

        // gsmģ���ring�ж��¼��Ƿ������
        if (is_gsmring_pending == true)
        {
            // GSMģ��RING�жϲ����Ĵ���(GSMģ���ϵ����Զ�����һ��RING�ж�)
            if (cnt_gsmring_asserted > 0)
            {
                do
                {
                    ret = GSM_ListSMS(&sms_index, GSM_SMS_STA_ALL);
                    if(ret == false)
                    {
                        // û���յ��µĶ���Ϣ(����������)
                        if (sms_index == 0)
                        {
                            cnt_gsmring_asserted --;
                        }
                        DebugOutPrintf(DEBUG_WRN, "failed to list all sms in current memory.\r\n");
                        goto dev_gsm_recv_sms_err;
                    }
                    ret = GSM_ReadSMS(sms_index, bufs, MAX_LEN_SMS_HEAD + MAX_LEN_CMD + 1);
                    if(ret == false)
                    {
                        DebugOutPrintf(DEBUG_WRN, "failed to read sms[%d] in current memory.\r\n", sms_index);
                        goto dev_gsm_recv_sms_err;
                    }
                    // ��ȡ�ɹ�
					buf_size = __strlen(bufs);
                    pos = KMPMatch((char *)bufs, "OK", buf_size);
                    if(pos >= 0)
                    {
						cnt_gsmring_asserted --;
						argc = split_str((char *)bufs, buf_size, sepr, argv, ARGS_MAX_COUNT);
						if (argc <= 4)
						{
							goto dev_gsm_recv_sms_err;
						}
                        __trim_sms(pn,   argv[1]);
						__trim_sms(time, argv[3]);
                        __trim_sms(sms,  argv[4]);
                    }
                    ret = GSM_CheckSMSAck();
                    if(ret == false)
                    {
                        DebugOutPrintf(DEBUG_WRN, "failed to ack sms.\r\n");
                    }
                    // ɾ����ǰ���յ��Ķ��ţ�����洢���ж�����(����Ϊ50��)
                    ret = GSM_DeleteSMS(sms_index, err);
                    if(ret == false)
                    {
                        DebugOutPrintf(DEBUG_WRN, "failed to delete sms[%d] in current memory.\r\n", sms_index);
                        goto dev_gsm_recv_sms_err;
                    }
                }
                while (0);
                return true;
            }
            else
            {
                is_gsmring_pending = false;
                DebugOutPrintf(DEBUG_WRN, "no unread short message.\r\n");
            }
        }
dev_gsm_recv_sms_err:
        return false;
    }

    /* ����Ӧ������ */
	int	DEV_GSM_Struct::DEV_Adaptive_BaudRate(void)
	{
		/* ��������� */
		int baudratetab[7] = {9600,38400,115200,19200,14400,56000,57600};
		/* ������ */
		int num0;
		int err, pos = -1;
		
		/* ѭ����ѯ */
		for(num0 = 0;num0 < 7;num0++)
		{
			/* ���ò����� */
			COM3.Config(baudratetab[num0]);
			
			/* ����ATָ�� */
			if(DEV_SendAT("AT\r\n", "OK", TO_GPRS_GPCMD, pos, err) == true)
			{
				/* ��ǰ������Ϊģ��ͨ�Ų����� */
				return baudratetab[num0];
			}
		}
		
		return 0;
	}		
		
		
		
		
		
		
		
		
		
		
		
		
    bool	DEV_GSM_Struct::GSM_ConnnectServerExample(char *send, int slen, char *recv, int &rlen, int &err, enum DEV_GPRS_Mode mode, int id)
    {
        int 	start, end, tim = DEV_GSM_Timing();
        int		ret;
//      int		id = GPRS_ID1;
        char	ip[MAX_LEN_GPRS_IP];

        __strnset((char *)recv, 0, rlen);
		
        // ÿ��ʹ��gsmģ��ǰǿ�ƻ��ѡ�
        DEV_GSM_Wakeup();

        // ͨ��״̬�²��ܽ���gprs����
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
        // ��ѯRSSI
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
            // ����������ʧ�ܣ��򽫴���������д��Flash���Դ��ط�
            if(ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to open GPRS PPP within %d ms.\r\n", (end - start));

                err = ER_GSM_GPRS_LINK_ERROR;		// ����ʧ��
                goto gsm_conn_serv_err;
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "it took %5d ms to open GPRS PPP.\r\n", (end - start));
            }
        }

        // ���ָ����GPRS�����Ƿ���Ч������Ч������֮
        ret = GPRS_SOC_Status(id, mode);
        // ��ָ��GPRS���ӵ�ǰ���ڶϿ�״̬����������(���Ӻ����ڲ�����������������Σ�
        // ���ζ�ʧ�ܲŷ��س�����)
        if(ret == false)
        {
            start = DEV_GSM_Timing();
            ret = GPRS_SOC_Setup(id, mode);
            end = DEV_GSM_Timing();
            // ����������ʧ�ܣ��򽫴���������д��Flash���Դ��ط�
            if(ret == false)
            {
                DebugOutPrintf(DEBUG_WRN, "failed to set up GPRS connection within %d ms.\r\n", (end - start));

                err = ER_GSM_GPRS_LINK_ERROR;		// ����ʧ��
                goto gsm_conn_serv_err;
            }
            else
            {
                DebugOutPrintf(DEBUG_INF, "it took %5d ms to set up GPRS connection.\r\n", (end - start));
            }
        }

        // ѭ�������������ݣ�ÿ������2����Ϣ��
        ret = GSM_SendData(id,  (char *)send, slen, err, mode);
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
						//�������Լ���ӵ�һ���ж�
						/* �ж� ���� ���� ���� :�������0 ������ */
						if(rlen != 0)
						{
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
						}
        ret = GPRS_SOC_Close(id, mode);
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to close gprs #%d connection.\r\n", id);
        }
        
        ret = GPRS_LINK_Close();
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_ERR, "failed to close gprs PPP connection.\r\n");
        }
		
        DebugOutPrintf(DEBUG_INF, "it took %dms to complete data communication.\r\n",
                       DEV_GSM_Timing() - tim);
        // ÿ��ʹ����gsm��ǿ�������Խڵ硣
        DEV_GSM_Sleep();

        return true;
gsm_conn_serv_err:
        rlen = 0;

        ret = GPRS_SOC_Close(id, mode);
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to close gprs #%d connection.\r\n", id);
        }
        
        ret = GPRS_LINK_Close();
        if(ret == false)
        {
            DebugOutPrintf(DEBUG_WRN, "failed to close gprs PPP connection.\r\n");
        }

        DebugOutPrintf(DEBUG_INF, "it took %dms to complete data communication.\r\n",
                       DEV_GSM_Timing() - tim);
        // ÿ��ʹ����gsm��ǿ�������Խڵ硣
        DEV_GSM_Sleep();

        return false;
    }

    // ����ָ������Ϣ��ָ���ĺ��룬�����˶���Ϣ��ַ��ͺͷ���ʧ�ܺ��Զ��ط����ܡ�
    // ע: ����Ĵ�������ϢҪô��ASCII���룬Ҫô��UTF-8���룬�Ա�ͳһ��Ϊ�ַ�������
    // ʹ��gsm_send_sms�������οɷ��͵����������ΪMAX_CHAR_CNC_SMS_CN���塢����
    // �ɷ��͵����Ӣ���������ܴ˺궨�����ƣ������ܳ���ɷ����ջ�ռ�����(���η���269�ַ�û����)��
    bool	DEV_GSM_Struct::GSM_SendSMS(char *pn, char *ascii_utf8, int &err)
    {
        bool			ret;
        char			pns[MAX_LEN_PN + 1];	// ��׼����gsm����(������������ǰ׺)

        int				len	 = 0;	// �����͵��ַ�����
        T_SMS_LANGUAGE	language = SMS_LANGUAGE_ENGLISH;

        // ͨ��״̬�¿��������Ͷ���!!!
        if(__strlen(pn) <= 0)
        {
            DebugOutPrintf(DEBUG_ERR, "pn is null and redirect to uart:%s", ascii_utf8);

            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }

        // �����������Ƿ�Ϊ��
        len = __strlen(ascii_utf8);

        if(len <= 0)
        {
            DebugOutPrintf(DEBUG_WRN, "reply is null.\r\n");

            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }

        // ��ʽ��gsm����
        if(DEV_FormatPN(pn) == false)
        {
            err = ER_GSM_ERROR_UNKNOW;
            goto gsm_send_sms_err;
        }

        // ����gsm������ӹ�������ǰ׺(����pdu����ʱĬ���趨�����ʽ��Я����������ǰ׺)��
        __strcpy(pns, gsm_telecode);
        __strcat(pns, pn);

        // �������������Ƿ�Ϊ����
        if(is_all_ascii(ascii_utf8) == false)
        {
            language = SMS_LANGUAGE_CHINESE;
        }

        DebugOutPrintf(DEBUG_INF, "sms length = %d and is written in %d.\r\n", len, language);

        // �������͵Ķ�Ϣ�ַ��Ƿ񳬹�������������(���Ķ������ݲ���UTF8�������롢
        // Ӣ�Ķ������ݲ���ASCII�������룬ǰ��ÿ�ַ�ռ3�ֽڡ�����ÿ�ַ�ռ1�ֽ�)
        // ���͵ĵ���������󳤶�Ϊ4����������
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

            // �̶�����pdu��ʽ���Ͷ���(Ӣ����8bit���롢������ucs2����)
            ret = GSM_SMS_SendPDU(language, pns, ascii_utf8, err);
        }
        while (0);
        DEV_GSM_Sleep();

        if (!ret)
        {
            // ���ͳ����Ҵ����Ƕ�Ϣ������������£����浱ǰ����׼���ط�
            if(err == ER_GSM_SMS_PDU_CHAOS)	// �����Ͷ�Ϣ�����а�����֧�ֵ��ַ�����
            {
                // ���ڰ�����֧�ֵ��ַ�����Ķ��ţ������ط�
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
//            printf("Sent sms to %s.\r\n", pns);

            return true;
        }
				
	
gsm_send_sms_err:
      //  return false;//ԭ�ȵ����
			  return true;	//2017.11.17�����ظ����Ͷ���(������Ȼ�ܱ����ظ����Ͷ��ŵ���ʧȥ��������̵ļ�鹦��)			
    }
}

//vu16 USART3_RX_STA_1=0; //GSM_ZWC9.15
////���ڽ��ջ����� 	
//u8 USART3_RX_BUF_1[2048];//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.//GSM_ZWC9.15
/* ��ӡ���Ա�־ */
BOOL GSM_DEBUG_Flag = FALSE;// TRUE

#if (USART_RECEIVE_EN!=0)
/**
  * @brief  Description "This function handles PPP interrupt request."
  * @param  None
  * @retval None
  */
ARMAPI void USART3_IRQHandler(void)
{
    //���յ�����
    if (COM3.Received())
    {
        char ch = COM3.ReadByte();
#if (USART_DEBUG_EN != 0)
		
		/* ��ӡ������� */
		if(GSM_DEBUG_Flag == TRUE)
		{
			COM1.WriteByte(ch);
		}
#endif
        textInFIFO << ch;
		
				
//	USART3_RX_BUF_1[USART3_RX_STA_1++]=COM3.ReadByte();	//��¼���յ���ֵ //GSM_ZWC9.15	
		
		
    }
}
#endif


/**
  * @brief  Description "���жϺ���"
  * @param  None
  * @retval None
  */
ARMAPI void TIM7_IRQHandler(void)
{
    if (TIMER7.GetUpdateStatus())
    {
        input.TimoutInc(1);
//        LEDIO2.ToggleBits(BSP_LED_CH2);

        TIMER7.ClearUpdate();
    }
}

#endif /* DEV_GSM_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/




#endif



