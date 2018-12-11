#ifndef _STRING_UTILITY_H   
#define _STRING_UTILITY_H	

#define Swap16(s) ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff))  


typedef    unsigned char uint8_t;


int     String_KMPMatch(char *s,char *p);//�ַ���ƥ�亯��,��ԭ�ַ���s�в����Ƿ���Ŀ���ַ���p �纬���򷵻س�����s�е�λ�÷��򷵻�-1
int     DeleteSubString(char* str_in,char* str_sub, char* str_out);//ɾ��Դ�ַ����е��������ַ������������������ַ�����Դ�ַ����г����˼���;��Դ�ַ�����û���������ַ����򷵻�0
void    HexToStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen);//��ʮ����������ת��Ϊʮ�������ַ���
void    StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen);//��ʮ�������ַ���ת��Ϊʮ����������
void    ConvertIP_StringToArray(char *str_src, unsigned char *str_out, unsigned char sub_cnt, char *sub_mark);//���ַ�����ʽ����ָ����־��ȡ������ʽ(����IP��ַ/Mac��ַ)
void    getNext(char *p,int *next);//����ģʽ��ÿ��λ�ô������׺��ǰ׺��ͬ�ĳ��ȣ� ������next[]�����ֵ


int     Find_SubStringEx(char* str_in,char* str_sub, int length, int *cnt);
int     Find_SubString(char* str_in,char* str_sub, int length);


uint8_t IsASSIC(char * str);//�ж��Ƿ�ASSIC����

void    gsm_char2hex(char *hex,char ch);//���ַ�ת����16�����ַ�

int     Utils_utf8_to_ucs2( unsigned char* ucs2, char* utf8, int len);//������ת��ΪUCS2��
// ��ָ����������Ϣ����Unicode Big Endian���룬����ϵͳ��������Ϣ
// Ĭ�ϲ���Unicode Big Endian���룬��˶�������Ϣ����������ת����
// ��ֻ��ԭ���������ɡ�
int     Utils_pdu_encode16bit(unsigned char* encoded, char* utf8, int len_utf8);
void    Utils_Hex2Str( const char *sSrc,  char *sDest, int nSrcLen );//��������ת��Ϊ�ַ���
int     Utils_strlen (const char  *pstr);//�����ַ�������
#endif //_STRING_UTILITY_H
