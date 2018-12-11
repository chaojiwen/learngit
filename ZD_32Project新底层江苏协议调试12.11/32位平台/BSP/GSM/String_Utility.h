#ifndef _STRING_UTILITY_H   
#define _STRING_UTILITY_H	

#define Swap16(s) ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff))  


typedef    unsigned char uint8_t;


int     String_KMPMatch(char *s,char *p);//字符串匹配函数,在原字符串s中查找是否含有目标字符串p 如含有则返回出现在s中的位置否则返回-1
int     DeleteSubString(char* str_in,char* str_sub, char* str_out);//删除源字符串中的特征子字符串，并返回特征子字符串在源字符串中出现了几次;若源字符串中没有特征子字符串则返回0
void    HexToStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen);//将十六进制数字转化为十六进制字符串
void    StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen);//将十六进制字符串转化为十六进制数字
void    ConvertIP_StringToArray(char *str_src, unsigned char *str_out, unsigned char sub_cnt, char *sub_mark);//将字符串形式根据指定标志截取数组形式(比如IP地址/Mac地址)
void    getNext(char *p,int *next);//求算模式串每个位置处的最长后缀与前缀相同的长度， 而求算next[]数组的值


int     Find_SubStringEx(char* str_in,char* str_sub, int length, int *cnt);
int     Find_SubString(char* str_in,char* str_sub, int length);


uint8_t IsASSIC(char * str);//判断是否纯ASSIC编码

void    gsm_char2hex(char *hex,char ch);//把字符转换成16进制字符

int     Utils_utf8_to_ucs2( unsigned char* ucs2, char* utf8, int len);//将内容转化为UCS2码
// 对指定的输入信息采用Unicode Big Endian编码，由于系统的输入信息
// 默认采用Unicode Big Endian编码，因此对输入信息无需做编码转换，
// 而只需原样拷贝即可。
int     Utils_pdu_encode16bit(unsigned char* encoded, char* utf8, int len_utf8);
void    Utils_Hex2Str( const char *sSrc,  char *sDest, int nSrcLen );//将整型数转化为字符串
int     Utils_strlen (const char  *pstr);//计算字符串长度
#endif //_STRING_UTILITY_H
