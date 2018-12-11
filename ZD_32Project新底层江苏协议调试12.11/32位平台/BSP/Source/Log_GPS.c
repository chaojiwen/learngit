/************************************************************************************************/
/** @file              : Log_GPS.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��8��7��															*/
/** @brief             : c file																	*/
/** @description       : GPS��λ�߼��ļ�														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "System_Config.h"

#if (GPS_Config_Enable == 0x01)


#include "Log_GPS.h"

#include "Dev_GPS.h"

#include "Mcu_GPS.h"

#include "App_GPS.h"


#include "string.h"
#include "stdlib.h"





/************************************************************************************************/
/* GPS��λ ���ݼĴ��� */
INSTRU_GPS_POS GPS_POS_Inf = 
{
	/* γ�� */
	0.0,
	/* ���� */
	0.0,
	/* �ű��� */
	0.0,
	/* �ٶ� */
	0.0,
	/* ��λ */
	0.0,
	/* ˮƽ����  */
	0.0,
	/* UTCʱ�� */
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	/* ��λ״̬ */
	FALSE,
};

INSTRU_GPS_POS_Log_SR GPS_POS_Log_SR = 
{
	/* ��Ϣ����״̬ */
	FALSE,
	/* ���ݽ��ճ�ʱ�Ĵ��� */
	0u,

};


/************************************************************************************************/
/* ������	: GPS_Cal_Char_Num																	*/
/* ����		: *str:Ŀ���ַ���ָ��;ch:��ѯ�ַ���													*/
/* ���		: Ŀ���ַ����� ָ�����ַ� ��������													*/
/* ����		: ͳ��ָ���ַ�����ָ���ַ��ĸ�����													*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
static int GPS_Cal_Char_Num(char* str, char ch)
{
	int i;
	int len = strlen(str);
	int	num = 0;

	for(i = 0; i < len; i++)
	{
		if(str[i] == ch)
		{
			num++;
		}
	}

	return num;
}


/************************************************************************************************/
/* ������	: GPS_StrCpy																		*/
/* ����		: *dst:Ŀ���ַ���ָ��;*src:Դ�ַ���ָ��;n:��ȡ�ַ���������							*/
/* ���		: ��																				*/
/* ����		: ��Դ�ַ�������ȡָ���������ַ���ΪĿ���ַ������Զ�����ַ�����β���š�			*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
static void GPS_StrCpy(char* dst, char* src, int n)
{
	int i;

	for(i = 0; i < n; i++)
	{
		dst[i] = src[i];
	}

	dst[i++] = '\0';
}


/************************************************************************************************/
/* ������	: GPS_Is_Float_Digit																*/
/* ����		: *str:Ŀ���ַ���.																	*/
/* ���		: ��																				*/
/* ����		: ���ָ���ַ����Ƿ�Ϊ�����ֻ��š�												*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Is_Float_Digit(char* str)
{
	int i;
	int	len = strlen(str);

	for(i = 0; i < len; i++)
	{
		if(!((str[i] >= '0' && str[i] <= '9') || str[i] == '.'))
		{
			return FALSE;
		}
	}

	return TRUE;
}





#if (GPS_CMD_RMC_ON_OFF == 0x01)
/************************************************************************************************/
/* ������	: GPS_Ana_RMC																		*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ����GPRMC��Ϣ																		*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_RMC(void)
{
	/* Ŀ���ַ������� */
	u16 rmc_len;
	/* �ַ������� */
	int	char_ctr;
	/* �ַ������������ */
	int j_ctr;
	/* �ַ������� */
	char str[16];
	/* ���������� ��ʱ���� */
	float val = 0.0;
	
//	int  nn=0;

	/* ����GPRMC��Ϣ���� */
	rmc_len = strlen(GPRMC_MSG_Buf);
// printf("GPSģ������:\r\n");
//	for(nn=0;nn<rmc_len;nn++)
//	printf("%c",GPRMC_MSG_Buf[nn]);

	
	/* �����Ϣ�ַ����ж��ŵĸ����Ƿ�Ϲ� */
	if(GPS_Cal_Char_Num(GPRMC_MSG_Buf, ',') != 12)
	{
		/* Ŀ���ַ����е� ',' �ַ�������ָ����������ͬ�� */
		/* ���ؽ���ʧ�ܡ� */
		return FALSE;
	}

	/* ��λ�ַ�������. */
	char_ctr = 0;
	
	/* �Թ���һ������. */
	for(; char_ctr < rmc_len; char_ctr++)
	{
		/* �жϵ�ǰ�ַ��Ƿ�Ϊ ',' */
		if(GPRMC_MSG_Buf[char_ctr] == ',')
		{
			/* �ҵ� ',' �ַ� */
			break;
		}
	}

	/* ָ�򶺺ŵ���һ���ַ� */
	char_ctr++;

	
	/*  1����ȡUTCʱ�� */
	/* ��RMC��Ϣ����ȡ2���ַ�UTCʱ��(ʱ)�� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* �ַ�������+2 */
	char_ctr += 2;
	/* �����ȡUTCʱ���Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}
	/* ����ȡ���ַ�ת�����������ݡ� */
	GPS_POS_Inf.UTC[3] = (unsigned char)atoi(str);

	/* ��RMC��Ϣ����ȡ2���ַ�UTCʱ��(��)�� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* �ַ�������+2 */
	char_ctr += 2;
	/* �����ȡUTCʱ���Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}
	/* ����ȡ���ַ�ת�����������ݡ� */
	GPS_POS_Inf.UTC[4] = (unsigned char)atoi(str);

	/* ��RMC��Ϣ����ȡ5���ַ�UTCʱ��(��ͺ���)�� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 5);
	/* �ַ�������+5 */
	char_ctr += 5;
	/* �����ȡUTCʱ���Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}
	/* ����ȡ���ַ�ת�����������ݡ� */
	GPS_POS_Inf.UTC[5] = (unsigned char)atoi(str);


	/* ����','���š� */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* 2����ȡ��λ״̬ */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 1);
	/* �ַ�������+1 */
	char_ctr += 1;
	/* ����RMC��Ϣ���ö�λ״̬�� */
	GPS_POS_Inf.Pos_State = (!strcmp(str, "A")?TRUE:FALSE);

	/* ����','���š� */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');

	/* 3����ȡ γ�� ���ݡ� */
	/* γ�����ݵ��������֡� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	
	
//	printf("γ������:%s\r\n",str);
	
	/* �ַ�������+2 */
	char_ctr += 2;
	/* �����ȡγ���ַ��Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}
	/* ����ȡ���ַ�ת�����������ݡ� */
	val = (float)atoi(str);

	/* γ�����ݵ�С�����֡� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 8);
//		printf("γ��С��:%s\r\n",str);
	/* �ַ�������+8 */
	char_ctr += 8;
	/* �����ȡγ���ַ��Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}

	/* ��ȡγ��С����2λ���ݡ� */
	val += atof(str)/60;

	//������滹��С��������Ӿ�ȷ������(�ڸ���ʡ���ˣ���֪��Ϊʲô)
	//val = ab+(cd/60)+(efgh/600000)

	/* ���� γ������ */
	GPS_POS_Inf.Lat = val;

	/* ����','���š� */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');

	/* N/S(��γΪ������γΪ��) */
	/* ��ȡN/S�ַ� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 1);
	/* �ַ�������+1 */
	char_ctr += 1;
	/* ���ñ�γ����γλ��״̬�� */
	GPS_POS_Inf.Lat *= (!strcmp(str, "N")?1:-1);

	/* ����','���š� */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* 4����ȡ ���� ���ݡ� */
	/* �������ݵ��������֡� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 3);
//		printf("��������:%s\r\n",str);
	
	/* �ַ�������+3 */
	char_ctr += 3;
	/* �����ȡ�����ַ��Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}

	/* ����ȡ�ľ����ַ�ת�����������ݡ� */
	val = (float)atoi(str);

	/* �������ݵ�С�����֡� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 8);
	
//	printf("����С��:%s\r\n",str);
	/* �ַ�������+8 */
	char_ctr += 8;
	/* �����ȡ�����ַ��Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}

	/* ��ȡ����С����2λ���ݡ� */
	val += atof(str)/60;
	/* ���þ������� */
	GPS_POS_Inf.Lon = val;

	/* ����','���š� */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* ����GPS��λ�õ��ľ��ȼ����ն�������ʱ���� */
	// ʱ���������������: g_time_zone = longitude2timezone(de_gpspos.lon);

	/* E/W(����Ϊ��������Ϊ��) */
	/* ��ȡE/W�ַ� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 1);
	/* �ַ�������+1 */
	char_ctr += 1;
	/* ���ö���������λ��״̬�� */
	GPS_POS_Inf.Lat *= (!strcmp(str, "E")?1:-1);

	/* ����','���š� */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');

	/* 5����ȡ �ٶ� ���ݡ� */
	/* ��λ�ַ��������� */
	j_ctr = 0;
	/* ��ȡ�ٶ���Ϣ */
	while(GPRMC_MSG_Buf[char_ctr] != ',')
	{
		str[j_ctr++] = GPRMC_MSG_Buf[char_ctr++];
	}
	/* ����ַ����������š� */
	str[j_ctr++] = '\0';
	/* �����ȡ�ٶ��ַ��Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}
	/* �����ٶ����ݡ�(RMC��Ϣ�е��ٶȵ�λΪ�ڣ���Ҫת��Ϊkm/h��λ) */
	GPS_POS_Inf.Spd = 1.852*atof(str);

	/* ����','���š� */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* 6����ȡ ��λ ���ݡ� */
	/* ��λ�ַ��������� */
	j_ctr = 0;
	/* ��ȡ��λ��Ϣ */
	while(GPRMC_MSG_Buf[char_ctr] != ',')
	{
		str[j_ctr++] = GPRMC_MSG_Buf[char_ctr++];
	}
	/* ����ַ����������š� */
	str[j_ctr++] = '\0';
	/* �����ȡ��λ�ַ��Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}
	/* ���÷�λ���ݡ� */
	GPS_POS_Inf.Cog = atof(str);

	/* ����','���š� */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* 7����ȡ ���� ���ݡ� */
	/* ��RMC��Ϣ����ȡ2���ַ�UTCʱ��(��)�� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* �ַ�������+2 */
	char_ctr += 2;
	/* �����ȡ��λ�ַ��Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}
	/* ����ȡ���ַ�ת�����������ݡ� */
	GPS_POS_Inf.UTC[2] = (unsigned char)atoi(str);

	/* ��RMC��Ϣ����ȡ2���ַ�UTCʱ��(��)�� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* �ַ�������+2 */
	char_ctr += 2;
	/* �����ȡ��λ�ַ��Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}
	/* ����ȡ���ַ�ת�����������ݡ� */
	GPS_POS_Inf.UTC[1] = (unsigned char)atoi(str);

	/* ��RMC��Ϣ����ȡ2���ַ�UTCʱ��(��)�� */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* �ַ�������+2 */
	char_ctr += 2;
	/* �����ȡ��λ�ַ��Ƿ�Ϊ�����ֻ��š� */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* ����Ϊ�����ֻ��š� */
		return FALSE;
	}
	/* ����ȡ���ַ�ת�����������ݡ� */
	GPS_POS_Inf.UTC[0] = (unsigned char)atoi(str);

	/* �����ɹ� */
	return TRUE;
}
#endif

#if (GPS_CMD_GSA_ON_OFF == 0x01)
/************************************************************************************************/
/* ������	: GPS_Ana_GSA																		*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ����GPGSA��Ϣ																		*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_GSA(void)
{
	/* Ŀ���ַ������� */
	u16 gsa_len;
	/* ˮƽ���� �ַ������� */
	u16 hdop_char_len;
	/* �ַ������� */
	int	char_ctr;
	/* �ַ������������ */
	int j_ctr = 0,i_ctr = 0;
	/* �������� */
	int	commas = 0;
	/* �ַ������� */
	char str[12];


	
	/* ����GPGSA��Ϣ���� */
 	gsa_len = strlen(GPGSA_MSG_Buf);
	
	/* �����Ϣ�ַ����ж��ŵĸ����Ƿ�Ϲ� */
	if(GPS_Cal_Char_Num(GPGSA_MSG_Buf, ',') != 17)
	{
		/* Ŀ���ַ����е� ',' �ַ�������ָ����������ͬ�� */
		/* ���ؽ���ʧ�ܡ� */
		return FALSE;
	}

	/* ��ָ���ַ���ĩβ�ַ� */
	char_ctr = gsa_len-1;

	/* ��GPGSA��Ϣ��β����ʼ ����'*' */
	while(char_ctr--)
	{
		/* ���*λ�� */
		if(GPGSA_MSG_Buf[char_ctr] == '*')
		{
			/* ���ҵ�'*'�� */
			break;
		}
	}

	/* ��δ��⵽*������FALSE */
	if(char_ctr <= 0)
	{
		/* ��� ˮƽ���� ������ */
		GPS_POS_Inf.HDOP = 0.00;
		/* û�в��ҵ�'*'�ַ� */
		return FALSE;
	}

	/* ��⵹��2�����ŵ�λ�ò���¼ */
	while(char_ctr--)
	{
		/* �����Ϣ�ַ����ж��� */
		if(GPGSA_MSG_Buf[char_ctr] == ',')
		{
			/* �ۼӶ��ż����� */
			commas++;
			/* �ж϶������� */
			if(commas == 1)
			{
				/* ��¼1������ʱ�ַ�ָ���λ�á� */
				i_ctr = char_ctr;
			}
			else if(commas == 2)
			{
				/* ��¼2������ʱ�ַ�ָ���λ�á� */
				j_ctr = char_ctr;
				/* �������� */
				break;
			}
		}
	}

	/* ��δ��⵽2�����Ż�2������֮��ľ��볬��5������NG */
	if((char_ctr <= 0) || ((i_ctr-j_ctr) > 5))
	{
		/* ��� ˮƽ���� ������ */
		GPS_POS_Inf.HDOP = 0.00;
		/* û�в��ҵ�'*'�ַ� */
		return FALSE;
	}

	/* ��ȡ��������֮����ַ� */
	char_ctr = 0;
	j_ctr += 1;
	/* ��ȡ ˮƽ���� ��Ϣ */
	while(j_ctr < i_ctr)
	{
		str[char_ctr++] = GPGSA_MSG_Buf[j_ctr++];
	}
	/* ����ַ����������š� */
	str[char_ctr++] = '\0';

	/* ���� HDOP ��Ϣ���� */
	hdop_char_len = strlen(str);

	/* ���HDOP���Ƿ�Ϊ�� */
	if(hdop_char_len <= 0)
	{
		/* ��� ˮƽ���� ������ */
		GPS_POS_Inf.HDOP = 0.00;
		/* û��ˮƽ�������� */
		return FALSE;
	}

	/* ��HDOP�ַ���ת��Ϊ������ */
	GPS_POS_Inf.HDOP  = atof(str);

	/* �����ɹ� */
	return TRUE;

}
#endif

#if (GPS_CMD_GGA_ON_OFF == 0x01)
/************************************************************************************************/
/* ������	: GPS_Ana_GGA																		*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ����GGA��Ϣ																		*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_GGA(void)
{
	__nop();
}
#endif

#if (GPS_CMD_GLL_ON_OFF == 0x01)
/************************************************************************************************/
/* ������	: GPS_Ana_GLL																		*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ����GLL��Ϣ																		*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_GLL(void)
{
	__nop();
}
#endif

#if (GPS_CMD_GSV_ON_OFF == 0x01)
/************************************************************************************************/
/* ������	: GPS_Ana_GSV																		*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ����GSV��Ϣ																		*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_GSV(void)
{
	__nop();
}
#endif

#if (GPS_CMD_VTG_ON_OFF == 0x01)
/************************************************************************************************/
/* ������	: GPS_Ana_VTG																		*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ����VTG��Ϣ																		*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_VTG(void)
{
	__nop();
}
#endif

/************************************************************************************************/
/* ������	: GPS_Log_Ana_Inf																	*/
/* ����		: ��			 																	*/
/* ���		: TRUE:GPS��Ϣ�����ɹ���FALSE:GPS��Ϣ����ʧ�ܣ�																				*/
/* ����		: ����GPS��λ��Ϣ�� 																*/
/* ��������	: 2015/8/10																			*/
/************************************************************************************************/
BOOL GPS_Log_Ana_Inf(void)
{
#if (GPS_CMD_RMC_ON_OFF == 0x01)
	BOOL gps_ana_rmc_state = FALSE;
#endif

#if (GPS_CMD_GSA_ON_OFF == 0x01)
	BOOL gps_ana_gsa_state = FALSE;
#endif

#if (GPS_CMD_GGA_ON_OFF == 0x01)
	BOOL gps_ana_gga_state = FALSE;
#endif

#if (GPS_CMD_GLL_ON_OFF == 0x01)
	BOOL gps_ana_gll_state = FALSE;	
#endif

#if (GPS_CMD_GSV_ON_OFF == 0x01)
	BOOL gps_ana_gsv_state = FALSE;
#endif

#if (GPS_CMD_VTG_ON_OFF == 0x01)
	BOOL gps_ana_vtg_state = FALSE;
#endif

			
/* ����GPRMC��Ϣ */
#if (GPS_CMD_RMC_ON_OFF == 0x01)
	gps_ana_rmc_state = GPS_Ana_RMC();
#endif
			
/* ����GPGSA��Ϣ */
#if (GPS_CMD_GSA_ON_OFF == 0x01)
	gps_ana_gsa_state = GPS_Ana_GSA();
#endif

/* ����GPGGA��Ϣ */
#if (GPS_CMD_GGA_ON_OFF == 0x01)
	gps_ana_gga_state = GPS_Ana_GGA();
#endif

/* ����GPGLL��Ϣ */
#if (GPS_CMD_GLL_ON_OFF == 0x01)
	gps_ana_gll_state = GPS_Ana_GLL();
#endif

/* ����GPGSV��Ϣ */
#if (GPS_CMD_GSV_ON_OFF == 0x01)
	gps_ana_gsv_state = GPS_Ana_GSV();
#endif

/* ����GPVTG��Ϣ */
#if (GPS_CMD_VTG_ON_OFF == 0x01)
	gps_ana_vtg_state = GPS_Ana_VTG();
#endif

// ������Ϣ��ȡ�Ƿ�ɹ���������RMC��GSA��Ϣ�󶼽�������Ϣ����ȡ��־����λ
// (�Ӷ���GPS�����жϴ�������п����ٴμ�Ⲣ��ȡRMC��Ϣ��GSA��Ϣ)
	/* ��λGPRMC��Ϣ����״̬ */
	GPSRMC_Data_Rec_State = FALSE;
	/* ��λGPGSA��Ϣ����״̬ */
	GPSGSA_Data_Rec_State = FALSE;
	/* ���GPRMC���ݻ��档 */
	memset(GPRMC_MSG_Buf, '\0', GPRMC_MSG_Buf_Size);
	/* ���GPGSA���ݻ��档 */
	memset(GPGSA_MSG_Buf, '\0', GPGSA_MSG_Buf_Size);

/* �ж�GPS��Ϣ�Ƿ�����ɹ� */
	if(
#if (GPS_CMD_RMC_ON_OFF == 0x01)
		(gps_ana_rmc_state == FALSE) ||
#endif
#if (GPS_CMD_GSA_ON_OFF == 0x01)
		(gps_ana_gsa_state == FALSE) ||
#endif
#if (GPS_CMD_GGA_ON_OFF == 0x01)
		(gps_ana_gga_state == FALSE) ||
#endif
#if (GPS_CMD_GLL_ON_OFF == 0x01)
		(gps_ana_gll_state == FALSE) ||
#endif
#if (GPS_CMD_GSV_ON_OFF == 0x01)
		(gps_ana_gsv_state == FALSE) ||
#endif
#if (GPS_CMD_VTG_ON_OFF == 0x01)
		(gps_ana_vtg_state == FALSE) ||
#endif
	TRUE
	)
	{
		/* ����ʧ�� */
		return TRUE;
	}else
	{
		/* �����ɹ� */
		return FALSE;
	}


}


/************************************************************************************************/
/* ������	: GPS_Log_Set_Rece_Timeout_Para														*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ����GPS���ճ�ʱ����																*/
/* ��������	: 2015/8/11																			*/
/************************************************************************************************/
void GPS_Log_Set_Rece_Timeout_Para(void)
{

	/*  ����GPS���ճ�ʱ���� */
	GPS_POS_Inf.Lat 	= GPS_Data_Rece_Timeout_Para;
	GPS_POS_Inf.Lon 	= GPS_Data_Rece_Timeout_Para;
	GPS_POS_Inf.Alt 	= GPS_Data_Rece_Timeout_Para;
	GPS_POS_Inf.Spd 	= GPS_Data_Rece_Timeout_Para;
	GPS_POS_Inf.Cog 	= GPS_Data_Rece_Timeout_Para;
	GPS_POS_Inf.HDOP	= GPS_Data_Rece_Timeout_Para;

	memset(GPS_POS_Inf.UTC, 0x00, 6);

	GPS_POS_Inf.Pos_State = FALSE;

}







/************************************************************************************************/
/* ������	: GPS_Log_RESET_POS_Inf																*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ��ʼ��GPS��λ����(��λ����)														*/
/* ��������	: 2015/8/7																			*/
/************************************************************************************************/
void GPS_Log_RESET_POS_Inf(void)
{
	GPS_POS_Inf.Lat 	= 0.0;
	GPS_POS_Inf.Lon 	= 0.0;
	GPS_POS_Inf.Alt 	= 0.0;
	GPS_POS_Inf.Spd 	= 0.0;
	GPS_POS_Inf.Cog 	= 0.0;
	GPS_POS_Inf.HDOP	= 0.0;

	memset(GPS_POS_Inf.UTC, 0x00, 6);

	GPS_POS_Inf.Pos_State = FALSE;
}


/************************************************************************************************/
/* ������	: GPS_Log_Rece_Data_State_Check															*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: GPS��λ ���ݽ���״̬���															*/
/* ��������	: 2015/8/11																			*/
/************************************************************************************************/
void GPS_Log_Rece_Data_State_Check(void)
{


	/* �ж�GPS���ݽ����Ƿ�ʱ */
	if((GPS_POS_Log_SR.Rece_Timeout_Reg >= GPS_Data_Rece_Timeout))
	{
		/* GPS���ݽ��ճ�ʱ */

		/* �ж�GPS���ݽ���״̬ */
		if(GPS_POS_Log_SR.Ana_State == TRUE)
		{
			/* ���GPS��λ���� */
			GPS_Log_RESET_POS_Inf();
			/* ����GPS���ճ�ʱ�߼����� */
			GPS_Log_Set_Rece_Timeout_Para();
			/* ����GPS���ճ�ʱӦ�ò��� */
			GPS_App_Set_Rece_Timeout_Para();
			/* ������ݽ���״̬ */
			GPS_POS_Log_SR.Ana_State = FALSE;
		}
		
	}else
	{
		/* δ��ʱ */
		/* �ۼ�GPS��ʱ���� */
		GPS_POS_Log_SR.Rece_Timeout_Reg ++;
	}
}



/************************************************************************************************/
/* ������	: GPS_log_init																		*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ��ʼ������GPS��λ�߼���Ϣ��														*/
/* ��������	: 2015/8/7																			*/
/************************************************************************************************/
void GPS_Log_Init(void)
{

	/* ��ʼ��GPS��λ����(��λ����) */ 
	GPS_Log_RESET_POS_Inf();





}


#endif


/******************* (C)HZ Zeda Team *****END OF FILE****/


