#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h> 
#include <stdbool.h> 
#include "String_Utility.h"
 

 
//��ʮ�������ַ���ת��Ϊʮ����������
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
//ɾ��Դ�ַ����е��������ַ������������������ַ�����Դ�ַ����г����˼���;��Դ�ַ�����û���������ַ����򷵻�0
int DeleteSubString(char* str_in,char* str_sub, char* str_out)
{
	int start = 0;    /* ��¼��ʼ�Ƚ��±� */
	int count = 0;    /* ��¼�Ӵ����� */
	int j = 0;        /* ��¼�Ӵ����±� */
	int k = 0;        /* ��¼����ַ������±� */ 
    int i=0;
	for ( i = 0; str_in[i] != '\0'; i++)
	{
       start = i;      /* ��ʱ����Ƚ��±� */
	   j = 0;          /* ÿ�ο�ʼ�Ƚϵ�ʱ���Ӵ�����0��ʼ,�����һ���ַ���ȣ�
	                      ��ô�ͽ��űȽ���һ���ַ������������һ��ѭ�� */
       
	   while ((str_in[i] != '\0')&&(str_sub[j] != '\0')&&(str_in[i] == str_sub[j]))
	   {
		   i++;
		   j++;
	   }
 
	   if (str_sub[j] != '\0')                /* ��������Ӵ� */
	   {
		   str_out[k++] = str_in[start];     /* ����ǰ�Ƚϵ�����ַ������� */
	   }
	   else                         /* ������Ӵ� */
	   {
           count++;                
		   i--;           /* ��Ϊ����forѭ��֮�󣬽�����һ��ѭ����ʱ��Ҫ����i++������� */
	   }
	}
 
	return count;
}

//Ѱ��һ��ָ�����ȵ��������Ƿ��������ݶΣ�����¼ÿ�γ�����Դ���ݵ�λ��
//char* str_in : ָ�����ȵ�Դ����
//char* str_sub : Ҫ�жϵ������ݶ�
//int length : ָ������
//int *cnt : ÿ�γ�����Դ���ݵ�λ�õ�����(���صĵ�ַ���ж������ݶγ��ֵ���һ����ַ)
//���� int count : ���ִ���
int Find_SubStringEx(char* str_in,char* str_sub, int length, int *cnt)
{

  int count = 0;    /* ��¼�Ӵ����� */
  int j = 0;        /* ��¼�Ӵ����±� */
  int i=0;
	
  for ( i = 0; i<length; i++)
  {
     j = 0;          /* ÿ�ο�ʼ�Ƚϵ�ʱ���Ӵ�����0��ʼ,�����һ���ַ���ȣ�
                        ��ô�ͽ��űȽ���һ���ַ������������һ��ѭ�� */
       
     while ((str_in[i] != '\0')&&(str_in[i] == str_sub[j]))
     {
       i++;
       j++;
     }
 
     if (!(str_sub[j] != '\0'))                /* ��������Ӵ� */
     {
       cnt[count] = i;
       count++;        
       i--;           /* ��Ϊ����forѭ��֮�󣬽�����һ��ѭ����ʱ��Ҫ����i++������� */
     }
  }
 
  return count;
}

//Ѱ��һ��ָ�����ȵ��������Ƿ��������ݶ�
//char* str_in : ָ�����ȵ�Դ����
//char* str_sub : Ҫ�жϵ������ݶ�
//int length : ָ������
//���� int count : ���ִ���
int Find_SubString(char* str_in,char* str_sub, int length)
{

  int count = 0;    /* ��¼�Ӵ����� */
  int j = 0;        /* ��¼�Ӵ����±� */
  int i=0;
  for ( i = 0; i<length; i++)
  {
     j = 0;          /* ÿ�ο�ʼ�Ƚϵ�ʱ���Ӵ�����0��ʼ,�����һ���ַ���ȣ�
                        ��ô�ͽ��űȽ���һ���ַ������������һ��ѭ�� */
       
     while ((str_in[i] != '\0')&&(str_in[i] == str_sub[j]))
     {
       i++;
       j++;
     }
 
     if (!(str_sub[j] != '\0'))                /* ��������Ӵ� */
     {
       count++;        
       i--;           /* ��Ϊ����forѭ��֮�󣬽�����һ��ѭ����ʱ��Ҫ����i++������� */
     }
  }
 
  return count;
}



//���ַ�����ʽ����ָ����־��ȡ������ʽ(����IP��ַ/Mac��ַ)
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









//����ģʽ��ÿ��λ�ô������׺��ǰ׺��ͬ�ĳ��ȣ� ������next[]�����ֵ
void getNext(char *p,int *next)
{
    int j,k;
    next[0]=-1;
    j=0;
    k=-1;
    while(j<strlen(p)-1)
    {
        if(k==-1||p[j]==p[k])    //ƥ��������,p[j]==p[k]
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
            j=next[j];       //������ָ��i�Ļ���
        }
        if(j==strlen(p))
            return i-strlen(p);
    }
    return -1;
}

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/**
 * @brief  IsASSIC �ж��Ƿ�ASSIC����
 * @param  str: �ַ���ָ��
 * @retval ��ASSIC���뷵��TRUE���Ǵ�ASSIC���뷵��FALSE
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
 * @brief  gsm_char2hex ���ַ�ת����16�����ַ�
 * @param  hex: 16�����ַ��洢��λ��ָ�룬ch���ַ�
 * @retval ��
 */
void gsm_char2hex(char *hex,char ch)
{
    const char numhex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		*hex++  = numhex[(ch & 0xF0)>>4];
    *hex    = numhex[ ch & 0x0F];
}

/*****************************************************************************/
// ��UTF-8������ַ���(UTF8�������ASCII����)ת����UCS2(Unicode Big Endian)������ֽ�����
int Utils_utf8_to_ucs2( unsigned char* ucs2, char* utf8, int len)//������ת��ΪUCS2��
{
	unsigned int i = 0;
	unsigned int j = 0;
	
	unsigned short doublebytes = 0x0000;

	while(i < len)
	{
		doublebytes = 0x0000;

		if(utf8[i] < 0x80)							// ASCII�ַ�
		{
			ucs2[j++] = 0x00;
			ucs2[j++] = utf8[i++];		
		}
		else if(utf8[i] >= 0xE0 && utf8[i] < 0xF0)	// �����ֽں��� 1110**** 10****** 10******  
		{
			doublebytes	 =  (utf8[i++])<<12;
			doublebytes |= ((utf8[i++]&0x3F)<<6);
			doublebytes |=  (utf8[i++]&0x3F);

			ucs2[j++] = (doublebytes>>8)&0xFF;
			ucs2[j++] = doublebytes&0xFF;			
		}
		else	// ���ڼȷ�ASCII�����ַ�UTF8������ַ������账���ֱ�ӷ��ش���
		{
			// printf("neither utf8 nor ascii encoded character found in stream!\r\n");

			return false;
		}
	}
	
	return j;	// ����ת�����ucs2�ֽ���
}
// ��ָ����������Ϣ����Unicode Big Endian���룬����ϵͳ��������Ϣ
// Ĭ�ϲ���Unicode Big Endian���룬��˶�������Ϣ����������ת����
// ��ֻ��ԭ���������ɡ�
int Utils_pdu_encode16bit(unsigned char* encoded, char* utf8, int len_utf8)
{
	int len_encoded = 0;

	len_encoded = Utils_utf8_to_ucs2(encoded, utf8, len_utf8);

	return len_encoded;	
}
void  Utils_Hex2Str( const char *sSrc,  char *sDest, int nSrcLen )//��������ת��Ϊ�ַ���
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



