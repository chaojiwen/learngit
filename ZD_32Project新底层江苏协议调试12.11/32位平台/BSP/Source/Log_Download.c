/*************************************************************************************************/
/*************************************     32λƽ̨����     ***************************************/
/* �������ڣ�2014/12/06																			 */
/*************************************************************************************************/
#include "interface.h"
#include "app.h"

#include <string.h>



#include "Log_Download.h"
#include "Log_GSM.h"

//�¶ȴ�����������
typedef struct {
	/* ��Ϣ�� ���� */
	u8 H_Data :4;
	/* ���ݼ��ܷ�ʽ */
	u8 L_Data :4;       
}Upgrade_Data_Decryption_Structure;

/************************************* �ļ� �궨�� ************************************************/



/************************************* �ļ����� ���� **********************************************/
static void Program_Inf_Tab_set(unsigned int bkp_start, unsigned char* bkp_ptr, unsigned int bkp_len);
static void flash_bytestream_read(unsigned int flash_start, unsigned char* data_ptr, unsigned int data_len);
static void flash_bytestream_write(unsigned int flash_start, unsigned char* data_ptr, unsigned int data_len);
static void Upgrade_Get_Data(void);
static void Upgrade_Decryption_Data(char* data,u16 no);
//static u16 crc_cal_by_byte(char* ptr,u32 len);

/************************************* �ļ����� **************************************************/
///* �������� ���ݻ��� */
//char IAP_Data_Buffer[IAP_PageSize];
///* �������� ���ݽṹ */
IAP_SR_Structure Mcu_IAP_Flash_Data;

/* ��� �������� ��״̬ ��Ϣ���� */
unsigned char Program_Upgrade_SR_Inf_Buffer[BLCOK_SYS_PARAM_SPACE];

/* CRC �ֽ���ʽ�� */
unsigned int crc_ta_8[256]={ 
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/************************************************************************************************/
/**	�� �� ��: crc_cal_by_byte																	*/
/**	����˵��: ���ֽڼ���CRC,��ָ�������ݶ� ��������У��.												*/
/**	��    �Σ�unsigned char* ptr:ָ���ͻ����������ֽ�\unsigned char len:Ҫ���͵����ֽ���			*/
/**	�� �� ֵ: unsigned int																		*/
/**	ע��	   : �����ڳ���ռ�ϴ���CRC�����ٶ�Ҫ��ϸߵļ������΢����ϵͳ,�㷨��Ҫ˵�������㱾�ֽں��CRC.	*/
/**			 �������һ�ֽ���ʽCRC��ĵ�8λ����8λ���ټ�����һ�ֽ�CRC ����8 λ��Ҳ��ȡ��8 λ���ͱ��ֽ�	*/
/**			 ֮�ͺ�����õ�CRC��.																	*/
/************************************************************************************************/
static u16 crc_cal_by_byte(char* ptr,u32 len)
{
    u16 crc = 0;

    while(len-- != 0)
    {
        unsigned int high = (unsigned int)(crc/256); //ȡCRC��8λ
        crc <<= 8;
        crc ^= crc_ta_8[high^*ptr];
        ptr++;
    }

    return crc;
}

/************************************************************************************************/
/**	�� �� ��: Remote_Upgrade																		*/
/**	����˵��: Զ������																			*/
/**	��    �Σ���																					*/
/**	�� �� ֵ: �������� ����																		*/
/************************************************************************************************/
BOOL Remote_Upgrade(void)
{
	u16 updated_state;
	u32 num;
	IAP_Data_Inf_Structure* data_inf;
	

//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		/* �׶�1:��ȡ ���³��� ��Ϣ�����ݿ� ������ */
//		printf("\r�׶�1:��ȡ���³�����Ϣ.(���ݿ� ����)\r\n");
//	}
	
	
	
	/* ��ʼ�������ݰ�. */
	Upgrade_Get_Data();
	/* �ж� ���� ��ȡ ��� */
	if(Mcu_IAP_Flash_Data.RX_COM_State == FALSE)
	{
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			printf("\r�׶�1:��ȡ���³�����Ϣʧ��.\r\n");	
//		}
		return FALSE;
	}	
	
	/* �����ݿ��� ���� ���³��� ��Ϣ(����ֻ�У�1�����ݿ�����) ������ */
	/* ����Ϣָ�룬ָ�����ݻ��� */
	data_inf = ((IAP_Data_Inf_Structure*)Mcu_IAP_Flash_Data.Data_Buffer);
	/* ��ȡ ��Ϣ */
	Mcu_IAP_Flash_Data.Data_Block_Total = data_inf->Block_Total;
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		/* �������ݳɹ�������ӡ ��� ��Ϣ */
//		printf("\r�׶�1:���ݳɹ�. ���³�������ݿ�����:%d\r\n",Mcu_IAP_Flash_Data.Data_Block_Total);	
//	}
	
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		printf("\r�׶�2:ɾ��������������.\r\n");	
//	}
	/* Flash ���� */
	FLASH_Unlock();
	/* �׶�2:���� ���س������� ���� *///������ƬFLASH ʱ��̫���ˣ����¿��Ź���Ч
	for(num = 0; num < BLOCK_CODESTORE_SPACE; num += FLASH_PAGE_SIZE)
	{
		FLASH_ErasePage(BLOCK_CODESTORE_START+num);	
	}
	/* Flash ���� */		
	FLASH_Lock();
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		printf("\r�׶�2:������������ɾ�����.\r\n");	
//	}
	/* �������³���ǰ���Flash�еĳ�����±�־ */
	updated_state = 0;
	Program_Inf_Tab_set(ENV_APP_UPDATED_START, (u8*)&updated_state ,ENV_APP_UPDATED_SIZE);
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		printf("\r�׶�2:ɾ��������±�־.\r\n");
//		
//		printf("\r�׶�3:��ȡ�͹̻����³�������.\r\n");		
//	}
	/* �׶�3:��ȡ�͹̻� ���³��� ���ݣ�д��FLASH�� */
	for((Mcu_IAP_Flash_Data.Data_Block_No = 1);
		(Mcu_IAP_Flash_Data.Data_Block_No < (Mcu_IAP_Flash_Data.Data_Block_Total+1));
		(Mcu_IAP_Flash_Data.Data_Block_No++))
	
	{	
		/* ��ʼ��ȡ ���³��� ���ݰ� */
		/* ��ʼ�������ݰ�. */
		Upgrade_Get_Data();
		/* �ж� ���� ��ȡ ��� */
		if(Mcu_IAP_Flash_Data.RX_COM_State == TRUE)
		{
//			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//			{
//				/* �ɹ����� ���ݰ� */
//				printf("\r�׶�3: ���³������ݽ��ճɹ������ݰ�:%d.\r\n",Mcu_IAP_Flash_Data.Data_Block_No);	
//				/* ���ݻ�ȡ �ɹ� */
//				/* �����ݿ� ���� У�� ���У���� */
//				printf("\r�׶�3: ���ݰ�����У��.\r\n");
//			}
			/* ��1K���ݽ���У��. */
			Mcu_IAP_Flash_Data.Data_Block_Check = crc_cal_by_byte((Mcu_IAP_Flash_Data.Data_Buffer+IAP_Page_Inf_Size),
																  (data_inf->Block_Len));
			/* �ж� У������� �� �������ṩ������ �Ƿ���ͬ. */
			if(Mcu_IAP_Flash_Data.Data_Block_Check == (data_inf->Block_Check))
			{
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					/* ����У������ */
//					printf("\r�׶�3: ���ݰ�У������.\r\n");
//				}
				/* �����ݿ�������ǽ��� ���� */
				Upgrade_Decryption_Data((Mcu_IAP_Flash_Data.Data_Buffer+IAP_Page_Inf_Size),data_inf->Block_Len);	
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					printf("\r�׶�3: ���ݰ��������.\r\n");
//					/* �̻����� */
//					printf("\r�׶�3: ��ʼ�̻����ݰ�����.\r\n");
//				}
				
				/* ������������ */
				OSSchedLock();
				/* Flash ���� */
				FLASH_Unlock();
				//��������д�� �ɹ�/ʧ�� ��û���жϡ����������𣿣���
				for(num = 0; num < data_inf->Block_Len; num += 4)
				{
					FLASH_ProgramWord((Mcu_IAP_Flash_Data.Write_Pointer+num),
									 (*(int*)(Mcu_IAP_Flash_Data.Data_Buffer+IAP_Page_Inf_Size+num)));
				}
				/* Flash ���� */		
				FLASH_Lock();
				/* ������������ */ 
				OSSchedUnlock();
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					/* ���ݹ̻� ��� */
//					printf("\r�׶�3: �̻����ݰ��������.\r\n");
//				}
				/* ���� ��һ�ε� д��ַָ�� */
				Mcu_IAP_Flash_Data.Write_Pointer += IAP_Page_Data_Size;
			}else
			{
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					/*  У��������»�ȡ���� */
//					printf("\r�׶�3: ���ݿ����У�����.\r\n");
//				}
				/* ���»�ȡ ��ǰ ���ݿ� */
				Mcu_IAP_Flash_Data.Data_Block_No--;
			}
		}else
		{
//			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//			{
//				printf("\r�׶�3:��ȡ���³�������ʧ�ܣ����»�ȡ��ǰ���ݿ�.\r\n");				
//			}
		
			/* ���»�ȡ ��ǰ ���ݿ� */
			Mcu_IAP_Flash_Data.Data_Block_No--;
		}		
	}
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		/* �׶�3: �������ݰ����ء��̻����. */
//		printf("\r�׶�3: �������ݰ����ء��̻����.\r\n");		
//	}

	/* �׶�3:�������³���ǰ���Flash�еĳ�����±�־ */ 
	updated_state = 1;
	Program_Inf_Tab_set(ENV_APP_UPDATED_START, (u8*)&updated_state, ENV_APP_UPDATED_SIZE);
////	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
////	{
////		printf("\r�׶�3:���ó�����±�־.\r\n");
////		
////		printf("\r�׶�4:������������.\r\n");		
////	}

	/* �׶�4:������������ */
//	mcu_reset();
	__set_FAULTMASK(1);//�ر����ж�
    NVIC_SystemReset();//����Ƭ������
	
	
	
	
	/* ���� ���� �ɹ�. */
	return TRUE;
}

/************************************************************************************************/
/**	�� �� ��: Upgrade_Decryption_Data															*/
/**	����˵��: �� ��ȡ �����ݿ� ���� ����															*/
/**	��    �Σ�data ���ݻ���ָ�룬no������������														*/
/**	�� �� ֵ: ���ݻ�ȡ��TURE�ɹ���FALSEʧ��														*/
/************************************************************************************************/
static void Upgrade_Decryption_Data(char* data,u16 no)
{
	u16 num;
	Upgrade_Data_Decryption_Structure* data_buffer;
	
	for(num = 0; num < no; num++)
	{
		/* ��ȡ �������� */
		data_buffer = ((Upgrade_Data_Decryption_Structure*)(data + num));
		/* �������� */
		*(data+num) = ((data_buffer->H_Data<<4)+ data_buffer->L_Data);
	}
}

/************************************************************************************************/
/**	�� �� ��: Upgrade_Get_Data																	*/
/**	����˵��: ��ȡ GPRS �������������																*/
/**	��    �Σ���																					*/
/**	�� �� ֵ: ���ݻ�ȡ��TURE�ɹ���FALSEʧ��														*/
/************************************************************************************************/
static void Upgrade_Get_Data(void)
{
	INT8U err;
	
	/* ��ʼ��ȡ ���³��� ���ݰ� */
//	printf("\r�������ݰ�:%d.\r\n",Mcu_IAP_Flash_Data.Data_Block_No);
//	/* ���� ���³��� �¼���־ */	
//	OSFlagPost(					//���־���ź�
//	Str_GPRS_Upg_Get_Data_Flag, //���ͱ�־��ָ��
//	(OS_FLAGS)0XFF, 			//ѡ��Ҫ���͵��ź� ����1λ���ź� 0010  ͬ����2ǿ��ת��ΪOS_FLAGS�͵�����
//								//��Ϊ�ź�ΪOS_FLAGS�͵�
//	OS_FLAG_SET,				//�ź���Ч��ѡ�� �ź���1  OS_FLAG_SETΪ��1  OS_FLAG_CLRΪ��0
//	&err);						//��־�ź�
	/* GSM �ź�ǿ�� */
////	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Download_IAP_Data_EVENT);//zwc�޸�
	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,IAP_EVENT);//zwc�޸�
	
	
	
	
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		/* �ȴ����� */
//		printf("\r�ȴ����ݰ�:%d.\r\n",Mcu_IAP_Flash_Data.Data_Block_No);		
//	}

	OSFlagPend(								//�����־�ź�
	Str_GPRS_Upg_RXC_Data_Flag,				//�����־�ź�ָ��
	(OS_FLAGS)0XFF,							//������ �����0�͵�1λ�ź� 0011 �����ǰ�����3ǿ��ת��ΪOS_FLAGS���͵����ݣ�
											//��Ϊ�������ͱ�־�ź��е��źŶ���OS_FLAGS���͵�����
											//OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,   
											//�ź�ȫ��1��ʾ�ź���Ч ����OS_FLAG_CONSUME��ʾ��
											//����ȴ����¼������������Ӧ���¼���־λ
	OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,	//�ź�ȫ��1��ʾ�ź���Ч  û�мӲ���OS_FLAG_CONSUME�����Բ��������־λ
	0,										//�ȴ�ʱ�ޣ�0��ʾ���޵ȴ�
	&err									//������Ϣ
	);
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		printf("\r�յ����ݰ�:%d.\r\n",Mcu_IAP_Flash_Data.Data_Block_No);		
//	}
}

/************************************************************************************************/
/**	�� �� ��: Program_Inf_Tab_set																*/
/**	����˵��: ����ָ���������ͱ��ݲ�����															*/
/**	��    �Σ�bkp_start��val																		*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/
static void Program_Inf_Tab_set(unsigned int bkp_start, unsigned char* bkp_ptr, unsigned int bkp_len)
{
	/* ������ */
	
	/* ��� ���ݻ��� */
	memset(Program_Upgrade_SR_Inf_Buffer,0X00,BLCOK_SYS_PARAM_SPACE);

	/* ��Flash�е�ȫ��ϵͳ����һ���Զ�����RAM */ 
	flash_bytestream_read(BLCOK_SYS_PARAM_START, Program_Upgrade_SR_Inf_Buffer, BLCOK_SYS_PARAM_SPACE);
	
	/* �ڶ�Ӧ�� ��Ϣλ д������ */
	/* д���� ���� ��־ */
	memcpy((u8*)(Program_Upgrade_SR_Inf_Buffer+bkp_start),bkp_ptr,bkp_len);
	
	/* ������������ */   
	OSSchedLock();
	/* Flash ���� */
	FLASH_Unlock();
	/* ������ǰҳ(ϵͳ�����洢��Flash��һ������ҳ���ҳ����ʼ��ַ��ʼ�洢) */ 
	FLASH_ErasePage(BLCOK_SYS_PARAM_START);

	/* ��RAM�и��µ���������д��Flash */ 
	flash_bytestream_write(BLCOK_SYS_PARAM_START, Program_Upgrade_SR_Inf_Buffer, BLCOK_SYS_PARAM_SPACE);	
	/* Flash ���� */		
	FLASH_Lock();
	/* ������������ */ 
	OSSchedUnlock();
}

/************************************************************************************************/
/**	�� �� ��: flash_bytestream_read																*/
/**	����˵��: ��ָ��Flash��ʼ��ַ����ȡָ�����ȵ��ֽ������ݵ�ָ���Ļ����С�							*/
/**	��    �Σ�flash_start data_ptr data_len���������ֽ����ĳ���									*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/		
static void flash_bytestream_read(unsigned int flash_start, unsigned char* data_ptr, unsigned int data_len)
{	
	int				i;
	unsigned int 	data_len2 = 0;
	unsigned short	halfword = 0x0000;
	
	// �����������ֽ��������Ƿ�Ϊ����
	data_len2 = data_len&1?data_len-1:data_len;

	// ��ȡ���ż�����ֽ�
	for(i = 0; i < data_len2; i += 2)
	{
		halfword = *(unsigned short*)(flash_start+i);

		// �Զ�����˫�ֽ��������ֽڵ�����
		*data_ptr++ = halfword&0xFF;
		*data_ptr++ = (halfword>>8)&0xFF;
	}

	// ������������ֽ�������Ϊ���������ȡ���һ���ֽ�
	if(data_len2 < data_len)
	{	
		halfword = *(unsigned short*)(flash_start+i);
		
		*data_ptr  = halfword&0xFF;
	}
}

/************************************************************************************************/
/**	�� �� ��: flash_bytestream_write																*/
/**	����˵��: ��ָ�����ȵ��ֽ�������д�뵽ָ����Flash��ʼ��ַ��										*/
/**	��    �Σ�flash_start data_ptr data_len���������ֽ����ĳ���									*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/	
static void flash_bytestream_write(unsigned int flash_start, unsigned char* data_ptr, unsigned int data_len)
{
	int 			i;
	unsigned int	data_len2 = 0;	

	// ����¼�ĳ����Ƿ�Ϊ����
	data_len2 = data_len&1?(data_len-1):data_len;

	// д�����ż�����ֽ�
	for(i = 0; i < data_len2; i += 2)
	{
		// �ֽ����������ƴ�˴洢��ʽ������ARM�ں˲���С�˸�ʽ��д���ݣ����д���ֽ�������ʱ�ɲ���˫�ֽ�д�룬
		// ���Ƕ����ֽ���ʱ����Ҫ��˫�ֽڶ�������Ҫ��˫�ֽ��������ֽڵ�����
		FLASH_ProgramHalfWord(flash_start+i, *((unsigned short*)data_ptr));	

		data_ptr += 2;
	}

	// ����¼����Ϊ������ĩβ�ֽڲ�0x00(����ʱ��ȥ)��д��
	if(data_len2 < data_len)
	{
		FLASH_ProgramHalfWord(flash_start+i, (*(unsigned char*)data_ptr<<8) | 0x00);
	}
}





/************************************************************************************************/
/**	�� �� ��: Flash_Write_SN_NO																	*/
/**	����˵��: ����SN��																			*/
/**	��    �Σ���д��sn��ָ��																		*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/	
void Flash_Write_SN_NO(unsigned char* sn_ptr)
{
	/* ���� SN �� */ 
	Program_Inf_Tab_set(ENV_SN_NO_START,sn_ptr,ENV_SN_NO_SIZE);
}

/************************************************************************************************/
/**	�� �� ��: Flash_Read_SN_NO																	*/
/**	����˵��: ��SN��																				*/
/**	��    �Σ�������sn��ָ��																		*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/	
void Flash_Read_SN_NO(unsigned char* sn_ptr)
{
	/* �� SN �� */
	flash_bytestream_read(BLCOK_SYS_PARAM_START+ENV_SN_NO_START,sn_ptr,ENV_SN_NO_SIZE);
}

/************************************************************************************************/
/**	�� �� ��: Flash_Write_Pgm_Ude_State															*/
/**	����˵��: ���ó������״̬																	*/
/**	��    �Σ���д��sn��ָ��																		*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/	
void Flash_Write_Pgm_Ude_State(unsigned char* state_ptr)
{
	/* ���� SN �� */ 
	Program_Inf_Tab_set(ENV_APP_UPDATED_START,state_ptr,ENV_APP_UPDATED_SIZE);
}

/************************************************************************************************/
/**	�� �� ��: Flash_Read_Pgm_Ude_State															*/
/**	����˵��: ���������״̬																		*/
/**	��    �Σ�������sn��ָ��																		*/
/**	�� �� ֵ: ��																					*/
/************************************************************************************************/	
void Flash_Read_Pgm_Ude_State(unsigned char* sn_ptr)
{
	/* �� SN �� */
	flash_bytestream_read(BLCOK_SYS_PARAM_START+ENV_APP_UPDATED_START,sn_ptr,ENV_APP_UPDATED_SIZE);
}




/******************* (C) ZHOU *****END OF FILE****/


