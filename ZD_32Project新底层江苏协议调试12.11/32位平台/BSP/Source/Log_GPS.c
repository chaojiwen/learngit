/************************************************************************************************/
/** @file              : Log_GPS.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年8月7日															*/
/** @brief             : c file																	*/
/** @description       : GPS定位逻辑文件														*/
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
/* GPS定位 数据寄存器 */
INSTRU_GPS_POS GPS_POS_Inf = 
{
	/* 纬度 */
	0.0,
	/* 经度 */
	0.0,
	/* 磁变量 */
	0.0,
	/* 速度 */
	0.0,
	/* 方位 */
	0.0,
	/* 水平精度  */
	0.0,
	/* UTC时间 */
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	/* 定位状态 */
	FALSE,
};

INSTRU_GPS_POS_Log_SR GPS_POS_Log_SR = 
{
	/* 信息解析状态 */
	FALSE,
	/* 数据接收超时寄存器 */
	0u,

};


/************************************************************************************************/
/* 函数名	: GPS_Cal_Char_Num																	*/
/* 输入		: *str:目标字符串指针;ch:查询字符。													*/
/* 输出		: 目标字符串中 指定的字符 的数量。													*/
/* 作用		: 统计指定字符串中指定字符的个数。													*/
/* 创建日期	: 2015/8/10																			*/
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
/* 函数名	: GPS_StrCpy																		*/
/* 输入		: *dst:目标字符串指针;*src:源字符串指针;n:提取字符串数量。							*/
/* 输出		: 无																				*/
/* 作用		: 从源字符串中提取指定个数的字符并为目标字符数组自动添加字符串结尾符号。			*/
/* 创建日期	: 2015/8/10																			*/
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
/* 函数名	: GPS_Is_Float_Digit																*/
/* 输入		: *str:目标字符串.																	*/
/* 输出		: 无																				*/
/* 作用		: 检查指定字符串是否为纯数字或点号。												*/
/* 创建日期	: 2015/8/10																			*/
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
/* 函数名	: GPS_Ana_RMC																		*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 解析GPRMC消息																		*/
/* 创建日期	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_RMC(void)
{
	/* 目标字符串数量 */
	u16 rmc_len;
	/* 字符计数器 */
	int	char_ctr;
	/* 字符串缓存计数器 */
	int j_ctr;
	/* 字符串缓存 */
	char str[16];
	/* 浮点型数据 临时变量 */
	float val = 0.0;
	
//	int  nn=0;

	/* 计算GPRMC信息长度 */
	rmc_len = strlen(GPRMC_MSG_Buf);
// printf("GPS模块数据:\r\n");
//	for(nn=0;nn<rmc_len;nn++)
//	printf("%c",GPRMC_MSG_Buf[nn]);

	
	/* 检查消息字符串中逗号的个数是否合规 */
	if(GPS_Cal_Char_Num(GPRMC_MSG_Buf, ',') != 12)
	{
		/* 目标字符串中的 ',' 字符数量与指定的数量不同。 */
		/* 返回解析失败。 */
		return FALSE;
	}

	/* 复位字符计数器. */
	char_ctr = 0;
	
	/* 略过第一个逗号. */
	for(; char_ctr < rmc_len; char_ctr++)
	{
		/* 判断当前字符是否为 ',' */
		if(GPRMC_MSG_Buf[char_ctr] == ',')
		{
			/* 找到 ',' 字符 */
			break;
		}
	}

	/* 指向逗号的下一个字符 */
	char_ctr++;

	
	/*  1、获取UTC时间 */
	/* 从RMC信息中提取2个字符UTC时间(时)。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* 字符计数器+2 */
	char_ctr += 2;
	/* 检查提取UTC时间是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}
	/* 将提取的字符转换成整形数据。 */
	GPS_POS_Inf.UTC[3] = (unsigned char)atoi(str);

	/* 从RMC信息中提取2个字符UTC时间(分)。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* 字符计数器+2 */
	char_ctr += 2;
	/* 检查提取UTC时间是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}
	/* 将提取的字符转换成整形数据。 */
	GPS_POS_Inf.UTC[4] = (unsigned char)atoi(str);

	/* 从RMC信息中提取5个字符UTC时间(秒和毫秒)。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 5);
	/* 字符计数器+5 */
	char_ctr += 5;
	/* 检查提取UTC时间是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}
	/* 将提取的字符转换成整形数据。 */
	GPS_POS_Inf.UTC[5] = (unsigned char)atoi(str);


	/* 查找','符号。 */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* 2、获取定位状态 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 1);
	/* 字符计数器+1 */
	char_ctr += 1;
	/* 根据RMC信息设置定位状态。 */
	GPS_POS_Inf.Pos_State = (!strcmp(str, "A")?TRUE:FALSE);

	/* 查找','符号。 */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');

	/* 3、获取 纬度 数据。 */
	/* 纬度数据的整数部分。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	
	
//	printf("纬度整数:%s\r\n",str);
	
	/* 字符计数器+2 */
	char_ctr += 2;
	/* 检查提取纬度字符是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}
	/* 将提取的字符转换成整形数据。 */
	val = (float)atoi(str);

	/* 纬度数据的小数部分。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 8);
//		printf("纬度小数:%s\r\n",str);
	/* 字符计数器+8 */
	char_ctr += 8;
	/* 检查提取纬度字符是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}

	/* 获取纬度小数后2位数据。 */
	val += atof(str)/60;

	//这个后面还有小数后面更加精确的数据(遮盖力省略了，不知道为什么)
	//val = ab+(cd/60)+(efgh/600000)

	/* 设置 纬度数据 */
	GPS_POS_Inf.Lat = val;

	/* 查找','符号。 */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');

	/* N/S(北纬为正、南纬为负) */
	/* 获取N/S字符 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 1);
	/* 字符计数器+1 */
	char_ctr += 1;
	/* 设置北纬、南纬位置状态。 */
	GPS_POS_Inf.Lat *= (!strcmp(str, "N")?1:-1);

	/* 查找','符号。 */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* 4、获取 经度 数据。 */
	/* 经度数据的整数部分。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 3);
//		printf("经度整数:%s\r\n",str);
	
	/* 字符计数器+3 */
	char_ctr += 3;
	/* 检查提取经度字符是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}

	/* 将提取的经度字符转换成整形数据。 */
	val = (float)atoi(str);

	/* 经度数据的小数部分。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 8);
	
//	printf("经度小数:%s\r\n",str);
	/* 字符计数器+8 */
	char_ctr += 8;
	/* 检查提取经度字符是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}

	/* 获取经度小数后2位数据。 */
	val += atof(str)/60;
	/* 设置经度数据 */
	GPS_POS_Inf.Lon = val;

	/* 查找','符号。 */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* 根据GPS定位得到的经度计算终端所处的时区。 */
	// 时区可以在这里计算: g_time_zone = longitude2timezone(de_gpspos.lon);

	/* E/W(东经为正、西经为负) */
	/* 获取E/W字符 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 1);
	/* 字符计数器+1 */
	char_ctr += 1;
	/* 设置东经、西经位置状态。 */
	GPS_POS_Inf.Lat *= (!strcmp(str, "E")?1:-1);

	/* 查找','符号。 */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');

	/* 5、获取 速度 数据。 */
	/* 复位字符串计数器 */
	j_ctr = 0;
	/* 获取速度信息 */
	while(GPRMC_MSG_Buf[char_ctr] != ',')
	{
		str[j_ctr++] = GPRMC_MSG_Buf[char_ctr++];
	}
	/* 添加字符串结束符号。 */
	str[j_ctr++] = '\0';
	/* 检查提取速度字符是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}
	/* 设置速度数据。(RMC消息中的速度单位为节，需要转换为km/h单位) */
	GPS_POS_Inf.Spd = 1.852*atof(str);

	/* 查找','符号。 */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* 6、获取 方位 数据。 */
	/* 复位字符串计数器 */
	j_ctr = 0;
	/* 获取方位信息 */
	while(GPRMC_MSG_Buf[char_ctr] != ',')
	{
		str[j_ctr++] = GPRMC_MSG_Buf[char_ctr++];
	}
	/* 添加字符串结束符号。 */
	str[j_ctr++] = '\0';
	/* 检查提取方位字符是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}
	/* 设置方位数据。 */
	GPS_POS_Inf.Cog = atof(str);

	/* 查找','符号。 */
	while(GPRMC_MSG_Buf[char_ctr++] != ',');


	/* 7、获取 日期 数据。 */
	/* 从RMC信息中提取2个字符UTC时间(日)。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* 字符计数器+2 */
	char_ctr += 2;
	/* 检查提取方位字符是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}
	/* 将提取的字符转换成整形数据。 */
	GPS_POS_Inf.UTC[2] = (unsigned char)atoi(str);

	/* 从RMC信息中提取2个字符UTC时间(月)。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* 字符计数器+2 */
	char_ctr += 2;
	/* 检查提取方位字符是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}
	/* 将提取的字符转换成整形数据。 */
	GPS_POS_Inf.UTC[1] = (unsigned char)atoi(str);

	/* 从RMC信息中提取2个字符UTC时间(年)。 */
	GPS_StrCpy(str, GPRMC_MSG_Buf+char_ctr, 2);
	/* 字符计数器+2 */
	char_ctr += 2;
	/* 检查提取方位字符是否为纯数字或点号。 */
	if(GPS_Is_Float_Digit(str) != TRUE)
	{
		/* 不是为纯数字或点号。 */
		return FALSE;
	}
	/* 将提取的字符转换成整形数据。 */
	GPS_POS_Inf.UTC[0] = (unsigned char)atoi(str);

	/* 解析成功 */
	return TRUE;
}
#endif

#if (GPS_CMD_GSA_ON_OFF == 0x01)
/************************************************************************************************/
/* 函数名	: GPS_Ana_GSA																		*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 解析GPGSA消息																		*/
/* 创建日期	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_GSA(void)
{
	/* 目标字符串数量 */
	u16 gsa_len;
	/* 水平精度 字符串数量 */
	u16 hdop_char_len;
	/* 字符计数器 */
	int	char_ctr;
	/* 字符串缓存计数器 */
	int j_ctr = 0,i_ctr = 0;
	/* 逗号数量 */
	int	commas = 0;
	/* 字符串缓存 */
	char str[12];


	
	/* 计算GPGSA信息长度 */
 	gsa_len = strlen(GPGSA_MSG_Buf);
	
	/* 检查消息字符串中逗号的个数是否合规 */
	if(GPS_Cal_Char_Num(GPGSA_MSG_Buf, ',') != 17)
	{
		/* 目标字符串中的 ',' 字符数量与指定的数量不同。 */
		/* 返回解析失败。 */
		return FALSE;
	}

	/* 先指向字符串末尾字符 */
	char_ctr = gsa_len-1;

	/* 从GPGSA信息的尾部开始 查找'*' */
	while(char_ctr--)
	{
		/* 检测*位置 */
		if(GPGSA_MSG_Buf[char_ctr] == '*')
		{
			/* 查找到'*'。 */
			break;
		}
	}

	/* 若未检测到*，返回FALSE */
	if(char_ctr <= 0)
	{
		/* 清除 水平精度 参数。 */
		GPS_POS_Inf.HDOP = 0.00;
		/* 没有查找到'*'字符 */
		return FALSE;
	}

	/* 检测倒数2个逗号的位置并记录 */
	while(char_ctr--)
	{
		/* 检查消息字符串中逗号 */
		if(GPGSA_MSG_Buf[char_ctr] == ',')
		{
			/* 累加逗号计数器 */
			commas++;
			/* 判断逗号数量 */
			if(commas == 1)
			{
				/* 记录1个逗号时字符指针的位置。 */
				i_ctr = char_ctr;
			}
			else if(commas == 2)
			{
				/* 记录2个逗号时字符指针的位置。 */
				j_ctr = char_ctr;
				/* 跳出查找 */
				break;
			}
		}
	}

	/* 若未检测到2个逗号或2个逗号之间的距离超过5，返回NG */
	if((char_ctr <= 0) || ((i_ctr-j_ctr) > 5))
	{
		/* 清除 水平精度 参数。 */
		GPS_POS_Inf.HDOP = 0.00;
		/* 没有查找到'*'字符 */
		return FALSE;
	}

	/* 提取两个逗号之间的字符 */
	char_ctr = 0;
	j_ctr += 1;
	/* 获取 水平精度 信息 */
	while(j_ctr < i_ctr)
	{
		str[char_ctr++] = GPGSA_MSG_Buf[j_ctr++];
	}
	/* 添加字符串结束符号。 */
	str[char_ctr++] = '\0';

	/* 计算 HDOP 信息长度 */
	hdop_char_len = strlen(str);

	/* 检查HDOP域是否为空 */
	if(hdop_char_len <= 0)
	{
		/* 清除 水平精度 参数。 */
		GPS_POS_Inf.HDOP = 0.00;
		/* 没有水平精度数据 */
		return FALSE;
	}

	/* 将HDOP字符串转换为浮点数 */
	GPS_POS_Inf.HDOP  = atof(str);

	/* 解析成功 */
	return TRUE;

}
#endif

#if (GPS_CMD_GGA_ON_OFF == 0x01)
/************************************************************************************************/
/* 函数名	: GPS_Ana_GGA																		*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 解析GGA消息																		*/
/* 创建日期	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_GGA(void)
{
	__nop();
}
#endif

#if (GPS_CMD_GLL_ON_OFF == 0x01)
/************************************************************************************************/
/* 函数名	: GPS_Ana_GLL																		*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 解析GLL消息																		*/
/* 创建日期	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_GLL(void)
{
	__nop();
}
#endif

#if (GPS_CMD_GSV_ON_OFF == 0x01)
/************************************************************************************************/
/* 函数名	: GPS_Ana_GSV																		*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 解析GSV消息																		*/
/* 创建日期	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_GSV(void)
{
	__nop();
}
#endif

#if (GPS_CMD_VTG_ON_OFF == 0x01)
/************************************************************************************************/
/* 函数名	: GPS_Ana_VTG																		*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 解析VTG消息																		*/
/* 创建日期	: 2015/8/10																			*/
/************************************************************************************************/
static BOOL GPS_Ana_VTG(void)
{
	__nop();
}
#endif

/************************************************************************************************/
/* 函数名	: GPS_Log_Ana_Inf																	*/
/* 输入		: 无			 																	*/
/* 输出		: TRUE:GPS信息解析成功；FALSE:GPS信息解析失败；																				*/
/* 作用		: 解析GPS定位信息。 																*/
/* 创建日期	: 2015/8/10																			*/
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

			
/* 解析GPRMC消息 */
#if (GPS_CMD_RMC_ON_OFF == 0x01)
	gps_ana_rmc_state = GPS_Ana_RMC();
#endif
			
/* 解析GPGSA消息 */
#if (GPS_CMD_GSA_ON_OFF == 0x01)
	gps_ana_gsa_state = GPS_Ana_GSA();
#endif

/* 解析GPGGA消息 */
#if (GPS_CMD_GGA_ON_OFF == 0x01)
	gps_ana_gga_state = GPS_Ana_GGA();
#endif

/* 解析GPGLL消息 */
#if (GPS_CMD_GLL_ON_OFF == 0x01)
	gps_ana_gll_state = GPS_Ana_GLL();
#endif

/* 解析GPGSV消息 */
#if (GPS_CMD_GSV_ON_OFF == 0x01)
	gps_ana_gsv_state = GPS_Ana_GSV();
#endif

/* 解析GPVTG消息 */
#if (GPS_CMD_VTG_ON_OFF == 0x01)
	gps_ana_vtg_state = GPS_Ana_VTG();
#endif

// 不管信息提取是否成功，解析完RMC和GSA消息后都将两种消息的提取标志都复位
// (从而在GPS串口中断处理程序中可以再次检测并提取RMC消息和GSA消息)
	/* 复位GPRMC消息接收状态 */
	GPSRMC_Data_Rec_State = FALSE;
	/* 复位GPGSA消息接收状态 */
	GPSGSA_Data_Rec_State = FALSE;
	/* 清除GPRMC数据缓存。 */
	memset(GPRMC_MSG_Buf, '\0', GPRMC_MSG_Buf_Size);
	/* 清除GPGSA数据缓存。 */
	memset(GPGSA_MSG_Buf, '\0', GPGSA_MSG_Buf_Size);

/* 判断GPS信息是否解析成功 */
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
		/* 解析失败 */
		return TRUE;
	}else
	{
		/* 解析成功 */
		return FALSE;
	}


}


/************************************************************************************************/
/* 函数名	: GPS_Log_Set_Rece_Timeout_Para														*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 设置GPS接收超时参数																*/
/* 创建日期	: 2015/8/11																			*/
/************************************************************************************************/
void GPS_Log_Set_Rece_Timeout_Para(void)
{

	/*  设置GPS接收超时参数 */
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
/* 函数名	: GPS_Log_RESET_POS_Inf																*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 初始化GPS定位参数(复位参数)														*/
/* 创建日期	: 2015/8/7																			*/
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
/* 函数名	: GPS_Log_Rece_Data_State_Check															*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: GPS定位 数据接收状态检查															*/
/* 创建日期	: 2015/8/11																			*/
/************************************************************************************************/
void GPS_Log_Rece_Data_State_Check(void)
{


	/* 判断GPS数据接收是否超时 */
	if((GPS_POS_Log_SR.Rece_Timeout_Reg >= GPS_Data_Rece_Timeout))
	{
		/* GPS数据接收超时 */

		/* 判断GPS数据解析状态 */
		if(GPS_POS_Log_SR.Ana_State == TRUE)
		{
			/* 清除GPS定位参数 */
			GPS_Log_RESET_POS_Inf();
			/* 设置GPS接收超时逻辑参数 */
			GPS_Log_Set_Rece_Timeout_Para();
			/* 设置GPS接收超时应用参数 */
			GPS_App_Set_Rece_Timeout_Para();
			/* 清除数据解析状态 */
			GPS_POS_Log_SR.Ana_State = FALSE;
		}
		
	}else
	{
		/* 未超时 */
		/* 累加GPS超时计数 */
		GPS_POS_Log_SR.Rece_Timeout_Reg ++;
	}
}



/************************************************************************************************/
/* 函数名	: GPS_log_init																		*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 初始化配置GPS定位逻辑信息。														*/
/* 创建日期	: 2015/8/7																			*/
/************************************************************************************************/
void GPS_Log_Init(void)
{

	/* 初始化GPS定位参数(复位参数) */ 
	GPS_Log_RESET_POS_Inf();





}


#endif


/******************* (C)HZ Zeda Team *****END OF FILE****/


