#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h> 
#include <stdbool.h> 
#include "String_Utility.h"
 

 
//将十六进制字符串转化为十六进制数字
void StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
   char h1,h2;
   unsigned char s1,s2;
   int i;
  for (i=0; i<nLen; i++)
 {
   h1 = pbSrc[2*i];
   h2 = pbSrc[2*i+1];

    s1= toupper(h1) - 0x30;
   if (s1 > 9) 
   s1 -= 7;
   s2 = toupper(h2) - 0x30;
   if (s2 > 9) 
   s2 -= 7;
  pbDest[i] = s1*16 + s2;
 }
}
void HexToStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
   char	ddl,ddh;
   int i;
   
   for (i=0; i<nLen; i++)
   {
   ddh = 48 + pbSrc[i] / 16;
   ddl = 48 + pbSrc[i] % 16;
   if (ddh > 57) ddh = ddh + 7;
   if (ddl > 57) ddl = ddl + 7;
   pbDest[i*2] = ddh;
   pbDest[i*2+1] = ddl;
}

pbDest[nLen*2] = '\0';
}
//删除源字符串中的特征子字符串，并返回特征子字符串在源字符串中出现了几次;若源字符串中没有特征子字符串则返回0
int DeleteSubString(char* str_in,char* str_sub, char* str_out)
{
	int start = 0;    /* 记录开始比较下标 */
	int count = 0;    /* 记录子串个数 */
	int j = 0;        /* 记录子串的下标 */
	int k = 0;        /* 记录结果字符串的下标 */ 
    int i=0;
	for ( i = 0; str_in[i] != '\0'; i++)
	{
       start = i;      /* 临时保存比较下标 */
	   j = 0;          /* 每次开始比较的时候，子串都从0开始,如果第一个字符相等，
	                      那么就接着比较下一个字符，否则进入下一轮循环 */
       
	   while ((str_in[i] != '\0')&&(str_sub[j] != '\0')&&(str_in[i] == str_sub[j]))
	   {
		   i++;
		   j++;
	   }
 
	   if (str_sub[j] != '\0')                /* 如果不是子串 */
	   {
		   str_out[k++] = str_in[start];     /* 将当前比较的这个字符存入结果 */
	   }
	   else                         /* 如果是子串 */
	   {
           count++;                
		   i--;           /* 因为返回for循环之后，进行下一次循环的时候还要进行i++的运算的 */
	   }
	}
 
	return count;
}

//寻找一串指定长度的数据中是否含有子数据段，并记录每次出现在源数据的位置
//char* str_in : 指定长度的源数据
//char* str_sub : 要判断的子数据段
//int length : 指定长度
//int *cnt : 每次出现在源数据的位置的数组(返回的地址是判断子数据段出现的下一个地址)
//返回 int count : 出现次数
int Find_SubStringEx(char* str_in,char* str_sub, int length, int *cnt)
{

  int count = 0;    /* 记录子串个数 */
  int j = 0;        /* 记录子串的下标 */
  int i=0;
	
  for ( i = 0; i<length; i++)
  {
     j = 0;          /* 每次开始比较的时候，子串都从0开始,如果第一个字符相等，
                        那么就接着比较下一个字符，否则进入下一轮循环 */
       
     while ((str_in[i] != '\0')&&(str_in[i] == str_sub[j]))
     {
       i++;
       j++;
     }
 
     if (!(str_sub[j] != '\0'))                /* 如果不是子串 */
     {
       cnt[count] = i;
       count++;        
       i--;           /* 因为返回for循环之后，进行下一次循环的时候还要进行i++的运算的 */
     }
  }
 
  return count;
}

//寻找一串指定长度的数据中是否含有子数据段
//char* str_in : 指定长度的源数据
//char* str_sub : 要判断的子数据段
//int length : 指定长度
//返回 int count : 出现次数
int Find_SubString(char* str_in,char* str_sub, int length)
{

  int count = 0;    /* 记录子串个数 */
  int j = 0;        /* 记录子串的下标 */
  int i=0;
  for ( i = 0; i<length; i++)
  {
     j = 0;          /* 每次开始比较的时候，子串都从0开始,如果第一个字符相等，
                        那么就接着比较下一个字符，否则进入下一轮循环 */
       
     while ((str_in[i] != '\0')&&(str_in[i] == str_sub[j]))
     {
       i++;
       j++;
     }
 
     if (!(str_sub[j] != '\0'))                /* 如果不是子串 */
     {
       count++;        
       i--;           /* 因为返回for循环之后，进行下一次循环的时候还要进行i++的运算的 */
     }
  }
 
  return count;
}



//将字符串形式根据指定标志截取数组形式(比如IP地址/Mac地址)
void ConvertIP_StringToArray(char *str_src, unsigned char *str_out, unsigned char sub_cnt, char *sub_mark)
{
  char *result = NULL;
  char temp[32] = {0};  
    unsigned char i=0;

  memcpy(temp, str_src, strlen(str_src));
    result = strtok(temp, sub_mark);  
    str_out[0] = atoi(result);
    for(i=0; i<sub_cnt; i++)
    {
    result = strtok(NULL, sub_mark); 
      str_out[i+1] = atoi(result);
    }
}









//求算模式串每个位置处的最长后缀与前缀相同的长度， 而求算next[]数组的值
void getNext(char *p,int *next)
{
    int j,k;
    next[0]=-1;
    j=0;
    k=-1;
    while(j<strlen(p)-1)
    {
        if(k==-1||p[j]==p[k])    //匹配的情况下,p[j]==p[k]
        {
            j++;
            k++;
            next[j]=k;
        }
        else                   //p[j]!=p[k]
            k=next[k];
    }
}

int String_KMPMatch(char *s,char *p)
{
    int next[100];
    int i,j;
    i=0;
    j=0;
    getNext(p,next);
    while(i<strlen(s))
    {
        if(j==-1||s[i]==p[j])
        {
            i++;
            j++;
        }
        else
        {
            j=next[j];       //消除了指针i的回溯
        }
        if(j==strlen(p))
            return i-strlen(p);
    }
    return -1;
}

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/**
 * @brief  IsASSIC 判断是否纯ASSIC编码
 * @param  str: 字符串指针
 * @retval 纯ASSIC编码返回TRUE，非纯ASSIC编码返回FALSE
 */
uint8_t IsASSIC(char * str)
{


    while(*str)
    {
        if(*str>0x7F)
        {
            return   false;
        }
        str++;
    }
    
    return   true;
}

/**
 * @brief  gsm_char2hex 把字符转换成16进制字符
 * @param  hex: 16进制字符存储的位置指针，ch：字符
 * @retval 无
 */
void gsm_char2hex(char *hex,char ch)
{
    const char numhex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		*hex++  = numhex[(ch & 0xF0)>>4];
    *hex    = numhex[ ch & 0x0F];
}

/*****************************************************************************/
// 将UTF-8编码的字符串(UTF8编码兼容ASCII编码)转化成UCS2(Unicode Big Endian)编码的字节流。
int Utils_utf8_to_ucs2( unsigned char* ucs2, char* utf8, int len)//将内容转化为UCS2码
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
// 对指定的输入信息采用Unicode Big Endian编码，由于系统的输入信息
// 默认采用Unicode Big Endian编码，因此对输入信息无需做编码转换，
// 而只需原样拷贝即可。
int Utils_pdu_encode16bit(unsigned char* encoded, char* utf8, int len_utf8)
{
	int len_encoded = 0;

	len_encoded = Utils_utf8_to_ucs2(encoded, utf8, len_utf8);

	return len_encoded;	
}
void  Utils_Hex2Str( const char *sSrc,  char *sDest, int nSrcLen )//将整型数转化为字符串
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
int  Utils_strlen (const char  *pstr)
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



