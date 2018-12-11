/**
  ******************************************************************************
  * @file              : sms_utils.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 27/09/2014 17:41:57
  * @brief             : h file
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
#ifndef SMS_UTILS_H
#define SMS_UTILS_H

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include <stdio.h>
#include <stdbool.h>
#include "string_utils.h"
//! @}
/*******************************************************************************
*
*
*
*
*******************************************************************************/
// Invert phone number in pair unit, 'F' attached to make length odd.
// e.g.: 86 13 80 07 57 51 1  --> 68 31 08 70 75 15 F1, F is attached.
finline unsigned int pdu_invert(char* dst, char* src, unsigned int len_src)
{
	unsigned int	i 			= 0;
	unsigned int 	len_dst		= 0;
	char 			c			= 0;	

	len_dst = len_src;

	for(i = 0 ; i < len_src; i += 2)
	{
		c = *src++;		
		*dst++ = *src++;	
		*dst++ = c;		
	}

	if(len_src & 1)
	{
		*(dst-2) = 'F';	
		len_dst++;		
	}

	*dst = '\0';

	return len_dst;
}

// Recover inverted phone number.
// e.g.: 68 31 08 70 75 15 F1 --> 86 13 80 07 57 51 1F, F should be deleted.
finline unsigned int  pdu_recover(char* dst, char* src, unsigned int len_src)
{	
	unsigned int	i  =0;
	unsigned int 	len_dst;	
	char 			c;		

	len_dst = len_src;

	for(i = 0; i < len_src; i += 2)
	{
		c = *src++;	
		*dst++ = *src++;	
		*dst++ = c;		
	}

	if(*(dst-1) == 'F')
	{
		dst--;
		len_dst--;		
	}

	*dst = '\0';

	return len_dst;
}

// Convert string in ASCI format into values in 8-bit.
// Note:
// 1, two continuous '0' in src will produce a 0x00 value in dst, 
//    which will be conflict with the ender of string '\0', so dst 
//    can not be regarded as a normal string herein;
finline unsigned int pdu_atoh(unsigned char* hex, char* asc, unsigned int len_asc)
{
	unsigned int 		i;
	unsigned int		len_hex	= 0;
	unsigned char 		quad 	= 0;
	unsigned char 		octet 	= 0;
	
	for(i = 0; i < len_asc; i++)
	{	
		if(*asc >= 'a' && *asc <= 'f')
		{
			quad = *asc -'a'+10;
		}
		else if(*asc >= 'A' && *asc <= 'F')
		{
			quad = *asc -'A'+10;
		}
		else if(*asc >= '0' && *asc <= '9')
		{
			quad = *asc -'0';
		}

		asc++;

		octet |= quad;

		if(i&1)
		{			
			*hex++ = octet;
			len_hex++;

			octet = 0;
		}
		else
		{
			octet <<= 4;
		}
		
	}

	return len_hex;	
}

// Convert hexical value into ASCII stringin big endian.
finline unsigned int pdu_htoa(char*asc, unsigned char* hex, unsigned int len_hex)
{	
	unsigned int 	i ;	  
	char			tbl_htoa[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};	 

	// recoveried by WJC, 17:57, 2012-04-07 
	for(i = 0; i < len_hex; i++)
	{
		*asc++ = tbl_htoa[(*hex>>4)&0x0F];	
		*asc++ = tbl_htoa[*hex & 0x0F];

		hex++;
	}
	
	*asc = '\0';

	return (len_hex*2);	
	
/*
	 for(i = 0; i < len_hex*2; i++)
	 {
		*asc++ = *hex++;		
		len_asc ++;
	 }
	 *asc = '\0';
	 return len_hex;
*/
}

// Encod 8-bit ASCII character into 7-bit code and convert value of 7-bit code into string(double characters).
finline int pdu_encode7bit(unsigned char* octect, char* ascii, unsigned int len_ascii)
{
	unsigned int		i;
	unsigned int		len_octect = 0;
	unsigned int		mod;

	for(i = 0; i < len_ascii; i++)
	{
		// 检查是否有非ASCII字符
		if(ascii[i] > 0x7F)
		{
			return false;
		}
		
		mod = i & 7;

		if(mod !=  7)
		{
			octect[len_octect++] = (ascii[i]>>mod) | ascii[i+1]<<(7-mod);
		}
	}

	return len_octect;
}

// Decode 7bit- encoded data into 8bit encoded data and conveert value into ASCII string.
finline unsigned int pdu_decode7bit(char* asc, unsigned char* hex, unsigned int len_hex)
{
	int 	len_asc = 0;
	int 	i;
	int 	mod = 0;

	for(i = 0; i < len_hex; i++)
	{
		mod = i%7;

		if(mod == 0)
		{		
			*asc = (unsigned char)(hex[i]<<(mod+1));
			*asc = (unsigned char)(*asc>>1);		}
		else
		{			
			*asc = (unsigned char)(hex[i]<<(mod+1));
			*asc = (unsigned char)(*asc>>1);
			*asc |= (hex[i-1]>>(8-mod));	
			
			// 如果当前字节序号为7的倍数(对7取余值为6)，则还要提取本字节的前7位
			if(mod == 6)
			{
				asc++;
				len_asc++;

				*asc = (unsigned char)(hex[i]>>1);
			}
		}
		
		asc++;
		len_asc++;		
	}

	*asc = '\0';

	return len_asc;
}

#if (false)
void test_codec_7bit(void)
{
	char	asc[256];
	unsigned char	bin[256] = 
		"\x54\x74\x19\x34\x44\xA7\xDD\xE5\x79\x19\x44\x2E\xB3\xCB\xE7\x30\x3D\xFD\x76\x83\xE8\x6F\x10\x1D\x5D\x06\x25\xDD\xF4\xB2\xDC\x1D\xA6\xA7\xDF\xEE\x30\x1B";
	// unsigned char	bin[256] = 
		"\x10\x1D\x5D\x06\xC9\xCB\xF3\x7A\x1B\x4E\x4F\xBF\xDD\xA0\xB7\x19\x34\x4D\xE3\x5B\xD0\xB0\x9C\x9E\x07\xD1\xC3\xEC\xF5\x1C\x14\x9E\x83\xCA\x61\x39\x3B\x0F\x0A\xCF\x41\xF0\xF7\x7C\x9E\x16\xB3\xCB\x2E\x10\x15\x5D\x06";
	
	int		len;


	// len = pdu_encode7bit(bin, asc1, strlen(asc1));
	
	len = pdu_decode7bit(asc, bin, 38);

	// printf("len = %d, asc = %s\r\n", len, asc);

	while(1);

}
#endif

// 将UTF-8编码的字符串(UTF8编码兼容ASCII编码)转化成UCS2(Unicode Big Endian)编码的字节流。
finline int utf8_to_ucs2( unsigned char* ucs2, char* utf8, int len)//将内容转化为UCS2码
{
	unsigned int i = 0;
	unsigned int j = 0;
	
	unsigned short doublebytes = 0x0000;

	while(i < len)
	{
		doublebytes = 0x0000;

		if(utf8[i] < 0x80)							// ASCII字符
		{
			ucs2[j++] = 0x00;
			ucs2[j++] = utf8[i++];		
		}
		else if(utf8[i] >= 0xE0 && utf8[i] < 0xF0)	// 是三字节汉字 1110**** 10****** 10******  
		{
			doublebytes	 =  (utf8[i++])<<12;
			doublebytes |= ((utf8[i++]&0x3F)<<6);
			doublebytes |=  (utf8[i++]&0x3F);

			ucs2[j++] = (doublebytes>>8)&0xFF;
			ucs2[j++] = doublebytes&0xFF;			
		}
		else	// 对于既非ASCII编码又非UTF8编码的字符，不予处理而直接返回错误
		{
			// printf("neither utf8 nor ascii encoded character found in stream!\r\n");

			return false;
		}
	}
	
	return j;	// 返回转化后的ucs2字节数
}

finline int pdu_encode8bit(char* dst, char* src, int len_src)
{
	int 		i;

	// 不能使用strcpy函数执行字符拷贝工作!!!
	for(i = 0; i < len_src; i++)
	{
		dst[i] = src[i];
	}

	return len_src;
}

finline int pdu_decode8bit(char *dst, char* src, int len_src)
{
	int 			i;

	// 不能使用strcpy函数执行字符拷贝工作!!!
	for(i = 0; i < len_src; i++)
	{
		dst[i] = src[i];
	}

	return len_src;
}


// 对指定的输入信息采用Unicode Big Endian编码，由于系统的输入信息
// 默认采用Unicode Big Endian编码，因此对输入信息无需做编码转换，
// 而只需原样拷贝即可。
finline int pdu_encode16bit(unsigned char* encoded, char* utf8, int len_utf8)
{
	int len_encoded = 0;

	len_encoded = utf8_to_ucs2(encoded, utf8, len_utf8);

	return len_encoded;	
}

// 将GSM模块接收到并通过窜口输出的信息转换为Unicode Big Endian编码，由于
// GSM模块接收到并通过窜口输出的信息本身就是Unicode Big Endian编码的，因此
// 无需转换编码而只需原样拷贝即可。(经过对M660+模块策测试发现，text模式下
// 模块对于接收到的pdu模式的短消息直接输出端消息内容的Unicode Big Endian编码值)
finline int pdu_decode16bit(unsigned char* decoded, unsigned char* ucs2, int len_ucs2)
{
	unsigned int i;	
	unsigned int len_decoded = 0;
	
	// swap characters within a pair
	for(i = 0; i < len_ucs2;)
	{
		*decoded++ = ucs2[i++];
		*decoded++ = ucs2[i++];

		len_decoded += 2;
	}

	return len_decoded;	
}


//!
void __getNext(char *p, int len, int *next)
{
    int j, k;
    next[0] = -1;
    j = 0;
    k = -1;
    while (j < len - 1)
    {
        if(k == -1 || p[j] == p[k])  //匹配的情况下,p[j]==p[k]
        {
            j++;
            k++;
            next[j & (127)]=k;
        }
        else //p[j]!=p[k]
		{
            k=next[k & (127)];
		}
    }
}
/**
  * 描述: 模式匹配算法
  *       一种改进的字符串匹配算法
  *       http://www.cnblogs.com/dolphin0520/archive/2011/08/24/2151846.html
  * 参数: 
  * 返回: 
  */
int KMPMatch(char *s,char *p, int l)
{
    int next[128];
    int i, j;
	int len = __strlen(p);
	int plen = __strlen(s);
    i = 0;
    j = 0;
	
	assert_param(len > 1);
    __getNext(p, len, next);
    while((i < plen) && (i < l))
    {
        if(j == -1 || s[i] == p[j])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];       //取消了指针i的回溯
        }
        if(j == len)
            return i - len;
    }
    return -1;
}


/*============================ TYPES =========================================*/
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:STM32F103C(8-B)Tx

/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* SMS_UTILS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

