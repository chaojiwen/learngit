/**
  ******************************************************************************
  * @file              : string_utils.h
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
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include <stdio.h>
#include <stdbool.h>
//! @}

enum debug_degrees
{
    DEBUG_ALL = 0x8000,
    DEBUG_INF = 0x0800,
    DEBUG_WRN = 0x0400,
    DEBUG_ERR = 0x0200,
    DEBUG_NED = 0x0080,
};
int DebugOutPrintf(enum debug_degrees deg, const char *format, ...);
/*============================ MACROS ========================================*/
//说明：
//	判断一个字符是否是white-space character
finline bool IsSpaceChar(const char c)						\
{ return (c == '\0') || ((c >= '\t') && (c <= '\r')) || (c == ' '); }

// 检查命令字符串是否合法的宏(可打印字符)
finline bool IS_TXT_COMMAND_CHARACTER(const char byte)		\
{ return ((byte >= 0x20 && byte < 0x7F)); }

//// Hex转BCD码
//#define HEX2BCD(x) (((x) % 10) + ((((x) / 10) % 10) << 4))  /*20 -> 20H*/

// 得到一个field在结构体(struct)中的偏移量 
#define FPOS(type, field) ((dword)&((type *)0)-> field) 

// 得到一个结构体中field所占用的字节数 
#define FSIZ(type, field ) sizeof(((type *)0)->field) 

// 防止溢出的一个方法 
#define INC_SAT(val) (val=((val)+1>(val))?(val)+1:(val)) 

// 判断字符是不是16进制的数值
#define HEXCHK(c) (((c)>='0' && (c)<='9')||\
    ((c) >= 'A' && (c) <= 'F') ||\
    ((c) >= 'a' && (c) <= 'f') ) 

// 判断字符是不是小写
#define ISLOWER(c) (((c) >= 'a' && (c) <= 'z') ) 

// 判断是否是format格式化字符串
finline bool IS_FORMAT_STRING(const char *pstr)
{
    while (( pstr != (char *)0) &&	/* Calc str len until NULL ptr (see Note #2a) ...       */
            (*pstr != (char  )0))  	/* ... or NULL char found      (see Note #2b).          */
    {
        if (*pstr == 0x25)	//'%'
            return true;
        pstr++;
    }
    return false;
}

// 绝对值
finline int __abs(int n)
{
    if (n < 0)
    {
        return -n;
    }
    return n;
}

// 根据年月日计算星期几
finline int __calc_weekday(int yyyy, int mm, int dd)
{
	int cent = yyyy / 100;
	if (1 == mm)
	{
		yyyy --;
		mm = 13;
	} else if (2 == mm)
	{
		yyyy --;
		mm = 14;
	}
	yyyy = yyyy % 100;
	return (int)((int)(cent/4) - (int)(2*cent) + (int)(yyyy/4) + yyyy + (int)((mm+1)*13/5) + dd - 1)%7;
}

finline int  __strlen (const char  *pstr)
{
    int len;

    len = 0;
    while (( pstr != (char *)0) &&	/* Calc str len until NULL ptr (see Note #2a) ...       */
            (*pstr != (char  )0))  	/* ... or NULL char found      (see Note #2b).          */
    {
        len++;
        pstr++;
    }

    return (len);
}

finline void __strnset(char *s, int ch, int n)
{
    int i;

    for(i = 0; i < n; i++)
    {
        *(s + i) = ch;
    }
}

// 从源字符串中提取指定个数的字符并为目标字符数组自动添加字符串结尾符号。
finline void __strncpy(char *dst, char *src, int n)
{
    int i;

    for(i = 0; i < n; i++)
    {
        dst[i] = src[i];
    }

    dst[i++] = '\0';
}

finline int __strcmp(const char *s1, const char *s2)
{
    for ( ; *s1 == *s2 ; s1++, s2++ )
    {
        if ( *s1 == '\0' )
            break;
    } // for

    return (*s1 - *s2);
} // my_strcmp()


finline char *__strcpy(char *s1, const char *s2)
{
    while (*s2 != '\0')
    {
        *(s1++) = *(s2++);
    }
    *s1 = '\0';

    return s1;
} // my_strcpy()

finline char *__strcpy2(char *dst, char *src)
{
    char *ret      = dst;

    while ((*dst++ = *src++) != '\0');

    return ret;
}

finline char *__strcat (char *dst, const char *src)
{
    char *ret = dst;
    while (*dst++ != '\0');
    --dst;
    while ((*dst++ = *src++) != '\0');
    return ret;
}

finline char *__strstr (const char *s1, const char *s2)
{
    int i;
    int s1len = __strlen (s1);
    int s2len = __strlen (s2);

    if (s2len > s1len)
        return NULL;

    for (i = 0; i <= s1len - s2len; ++i)
    {
        int j;
        for (j = 0; j < s2len; ++j)
            if (s1[i + j] != s2[j])
                break;
        if (j == s2len)
            return (char *) s1 + i;
    }

    return NULL;
}

finline char *__toUpper(char *s)
{
    char *ret = s;
    while (*s != '\0')
    {
        if (ISLOWER(*(s)))
			*(s) -= 0x20;
		s ++;
    }

    return ret;
}

// 将十六进制表示的数值字符串转换为整型数值。
finline int __atoih(char *asc_h)
{
    int len = __strlen(asc_h);
    int	val = 0;
    int	i;

    for(i = 0; i < len; i++)
    {
        if(asc_h[i] >= '0' && asc_h[i] <= '9')
        {
            val += asc_h[i] - '0' + 0;
        }
        else if(asc_h[i] >= 'a' && asc_h[i] <= 'f')
        {
            val += asc_h[i] - 'a' + 10;
        }
        else if(asc_h[i] >= 'A' && asc_h[i] <= 'F')
        {
            val += asc_h[i] - 'A' + 10;
        }
        else
        {
            return val;
        }

        // 处理最后一个字符时不移位
        if(i < (len - 1))
        {
            val <<= 4;
        }
    }

    return val;
}

// 将可能包含正负号信息的十进制字符串转换为数值。
finline int __atoid(char *asc_d)
{
    char 	*ptr   = asc_d;
    int		sign;
    int 	val = 0;

    if(*ptr == '+')
    {
        sign = 1;

        ptr++;
    }
    else if(*ptr == '-')
    {
        sign = -1;

        ptr++;
    }
    else
    {
        sign = 1;
    }

    while(*ptr != '\0')
    {
        if(*ptr >= '0' && *ptr <= '9')
        {
            val *= 10;

            val += *ptr - '0';

            ptr++;
        }
        else
        {
            break;
        }
    }

    return sign * val;
}

// 将十进制(支持负数)或十六进制(以0x或0X开头，不支持负数)表示的数值字符串变化为数值。
finline int	__atoi(char *asc)
{
    // 略过字符串前面的空格
    while(*asc == ' ')
    {
        asc++;
    }

    // 检查是否带0x或0X前缀
    if((asc[0] == '0' && asc[1] == 'x') || (asc[0] == '0' && asc[1] == 'X'))
    {
        // 若带0x或0X前缀，按照十六进制转换
        return __atoih((char *)asc + 2);
    }
    else
    {
        // 若不带0x或0X前缀，按照十进制转换
        return __atoid((char *)asc);
    }
}

// 检查指定字符串中是否为全数字字符。
finline bool is_all_digit(char *str)
{
    int	i;
    int 	len = __strlen(str);

    for(i = 0; i < len; i++)
    {
        if(!(str[i] >= '0' && str[i] <= '9'))
        {
            return false;
        }
    }

    return true;
}

// 检查指定字符串是否为全ascii字符。
finline bool is_all_ascii(char *str)
{
    int i;
    int len = __strlen(str);

    for(i = 0; i < len; i++)
    {
        if(str[i] > 0x7F)
        {
            return false;
        }
    }

    return true;
}
/*! \note separator
 *  \param pchBuffer
 *  \param chByte
 *  \retval true separator handle on-going
 *  \retval false separator handle finish
 */
finline bool IsSeparator(uint8_t const *pchBuffer, uint8_t chByte)
{
    while ('\0' != *pchBuffer)
    {
        if (chByte == *pchBuffer)
        {
            return true;
        }
        pchBuffer++;
    }

    return false;
}

/*! \note string copy
 *  \param toHere
 *  \param fromHere
 *  \retval true copy string succeeded
 *  \retval false copy string failed
 */
finline bool str_cpy(uint8_t *toHere, const uint8_t *fromHere)
{
    if ((NULL == toHere) || (NULL == fromHere))
    {
        return false;
    }

    while ('\0' != *fromHere)
    {
        *toHere = *fromHere;
        toHere++;
        fromHere++;
    }

    *toHere = *fromHere;

    return true;
}

/*! \note string compare
 *  \param pchString1
 *  \param pchString2
 *  \retval true string compare on-going
 *  \retval false string compare finish
 */
finline bool str_cmp(uint8_t *pchString1, uint8_t *pchString2)
{
    while (*pchString1 == *pchString2)
    {
        if ('\0' == *pchString1)
        {
            return true;
        }
        else
        {
            pchString1++;
            pchString2++;
        }
    }

    return false;
}

/*! \note string length
 *  \param pchString
 *  \return string length
 */
finline uint8_t str_len(uint8_t *pchString)
{
    uint8_t chLen = 0;
    while ('\0' != *pchString)
    {
        pchString++;
        chLen++;
    }

    return chLen;
}


/*============================ TYPES =========================================*/
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/

/*=============================================================================
* Function		: make_argv
* Description	: 分解字符串为多个子串
* Input Para	:
* Output Para	:
* Return Value  :
=============================================================================*/
finline bool isSeparator(char ch, const char *separators)
{
    while ('\0' != *separators)
    {
		if (ch == *separators)
		{
			return true;
		}
        separators++;
    }
	return false;
}

finline char* __trim_sms(char* dst, const char* s)
{
    while ('\0' != *s && ( ('\"' == *s || '\r' == *s || '\n' == *s)))
    {
        s++;
    }

    while ('\0' != *s && (!('\"' == *s || '\r' == *s || '\n' == *s)))
    {
		*(dst++) = *(s++);
    }
	*dst = '\0';
    return dst;
}

finline char* __trim_time(char* dst, const char* s)
{
    while ('\0' != *s && ( ('/' == *s || '-' == *s || ',' == *s || ' ' == *s || ':' == *s)))
    {
        s++;
    }

    while ('\0' != *s && (!('/' == *s || '-' == *s || ',' == *s || ' ' == *s || ':' == *s)))
    {
		*(dst++) = *(s++);
    }
	*dst = '\0';
    return dst;
}

finline char* __trim_sep(char* dst, const char* s, const char* sep)
{
    while ('\0' != *s && ( isSeparator(*s, sep)))
    {
        s++;
    }

    while ('\0' != *s && (!isSeparator(*s, sep)))
    {
		*(dst++) = *(s++);
    }
	*dst = '\0';
    return dst;
}

/**
  * @brief  Description "分解字符串为多个子串"
  * @param  s			字符串
  * @param  size		字符串长度
  * @retval bool		分解字符串是否成功
  */
finline int split_str(char *s, int size, const char *separators, char *argv[], int av_max)
{
    char *se;
    int n;

    n = 0;
    se = s + size - 1;
    // trim space
    while (s < se && *s && isSeparator(*s, separators))
    {
        s++;
    }

    while (s < se && *s && n < av_max - 1)
    {
        if (*s)
        {
            argv[n++] = s;
        }

        while (s < se && *s && ! isSeparator(*s, separators))
        {
            s++;
        }

        if (s < se && *s)
        {
            *s++ = '\0';// argv analysis completion
        }

        while (s < se && *s && isSeparator(*s, separators))
        {
            s++;
        }
    }

    argv[n] = NULL;

    return n;
}
/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* STRING_UTILS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

