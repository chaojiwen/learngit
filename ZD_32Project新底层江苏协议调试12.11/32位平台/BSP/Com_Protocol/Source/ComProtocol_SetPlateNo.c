/************************************************************************************************/
/** @file              : ComProtocol_SetPlateNo.c												*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017��02��14��															*/
/** @brief             : c file																	*/
/** @description       : �����������--������--ͨ��Э��--���ó��ƺ��� Ӧ���ļ�						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**�ļ���     : ComProtocol_SetPlateNo.c															*/
/**����       : ��GB2313��ʽ�ĳ��ƺ��룬ת��ΪUTF8��ʽ											*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2017��02��14��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "ComProtocol_SetPlateNo.h"

#include <stdio.h>
#include <string.h>


/*******************************************************************************/
/********************************* ���ݽṹ *************************************/
/* GB2312ת��ΪUTF-8���ݽṹ */
typedef struct {
	uint16_t	GB2312Code;
	uint8_t  	UTF8Code[3];
}ComProtocol_UTF8;

/********************************* UTF8����� **********************************/
/* UTF8�������� */
#define ComProtocol_UTF8Num					(32u)
/* ���UTF-8����Ҫ������ת�����ƺ��е����ı��� */
const ComProtocol_UTF8 ComProtocol_GB2312ToUTF8[ComProtocol_UTF8Num] =
{
	{0xB2D8 ,0xE8,0x97,0x8F},//'��'
	{0xB4A8 ,0xE5,0xB7,0x9D},//'��'
	{0xB6F5 ,0xE9,0x84,0x82},//'��'
	{0xB8CA ,0xE7,0x94,0x98},//'��'
	{0xB8D3 ,0xE8,0xB5,0xA3},//'��'
	{0xB9F0 ,0xE6,0xA1,0x82},//'��'
	{0xB9F3 ,0xE8,0xB4,0xB5},//'��'
	{0xBADA ,0xE9,0xBB,0x91},//'��'
	{0xBBA6 ,0xE6,0xB2,0xAA},//'��'
	{0xBCAA ,0xE5,0x90,0x89},//'��'
	{0xBCBD ,0xE5,0x86,0x80},//'��'
	{0xBDF2 ,0xE6,0xB4,0xA5},//'��'
	{0xBDFA ,0xE6,0x99,0x8B},//'��'
	{0xBEA9 ,0xE4,0xBA,0xAC},//'��'
	
	{0xC1C9 ,0xE8,0xBE,0xBD},//'��'
	{0xC2B3 ,0xE9,0xB2,0x81},//'³'
	{0xC3C9 ,0xE8,0x92,0x99},//'��'
	{0xC3F6 ,0xE9,0x97,0xBD},//'��'
	{0xC4FE ,0xE5,0xAE,0x81},//'��'
	{0xC7E0 ,0xE9,0x9D,0x92},//'��'
	{0xC7ED ,0xE7,0x90,0xBC},//'��'
	{0xC9C2 ,0xE9,0x99,0x95},//'��'
	{0xCBD5 ,0xE8,0x8B,0x8F},//'��'
	{0xCDEE ,0xE7,0x9A,0x96},//'��'
	{0xCFE6 ,0xE6,0xB9,0x98},//'��'
	
	{0xD0C2 ,0xE6,0x96,0xB0},//'��'
	{0xD3E5 ,0xE6,0xB8,0x9D},//'��'
	{0xD4A5 ,0xE8,0xB1,0xAB},//'ԥ'
	{0xD4C1 ,0xE7,0xB2,0xA4},//'��'
	{0xD4C6 ,0xE4,0xBA,0x91},//'��'
	{0xD5E3 ,0xE6,0xB5,0x99},//'��'
	{0x0000 ,0x00,0x00,0x00},//''
};

/************************************************************************************************/
/* ������	: ComProtocol_SetPlateNo										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ���ó��ƺ���					 													*/
/* ��������	: 2016/12/23																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
void ComProtocol_SetPlateNo(uint8_t* cbuf,char* plateNoString)
{
	
	/* �ַ������� */
	uint8_t cStrLen;
	/* ������ */
	uint8_t num0,num1;
	/* ���ݻ���ָ�� */
	uint8_t databufp;
	
	
	/* �����ַ������� */
	cStrLen = strlen(plateNoString);
	
	/* �ж��ַ��������Ƿ񳬳��趨���ֵ */
	if(12 < cStrLen)
	{
		/* ���������õ�����ַ������� */
		__nop();
		return;
	}
	
	/* ת�����뷽ʽ */
	for(num0 = 0,databufp = 0 ;num0 < cStrLen ;num0++)
	{
		/* �жϵ�ǰ�ַ������Ļ���ASCII�롣 */
		if ((((uint8_t*)plateNoString)[num0]) < 0x80)
		{
			/* ASCII�� */
			/* ASCII�ַ�����UTF8�ַ� */
			cbuf[databufp] = plateNoString[num0];
			/* ����ָ�� */
			databufp += 1;
			/* ���������������� */
			continue;
		}else
		{
			/* ��������ָ�� */
			uint8_t* cCodeHighp = ((uint8_t*)(&plateNoString[num0]));
			uint8_t* cCodeLowp = ((uint8_t*)(&plateNoString[num0+1]));
			
			/* ����(GBK) */
			/* ���ҵ�ǰGBK���ĵ�UTF8���롣 */
			for(num1 = 0;num1 < ComProtocol_UTF8Num ;num1++)
			{
				/* ��ѯ��ǰ���������Ӧ��UTF8���� */
				if(ComProtocol_GB2312ToUTF8[num1].GB2312Code == (*cCodeLowp + (((uint16_t)(*cCodeHighp))<<8)))
				{
					/* ��UTF8ֵת�Ƶ������С� */
					cbuf[databufp] 	 = ComProtocol_GB2312ToUTF8[num1].UTF8Code[0];
					cbuf[databufp+1] = ComProtocol_GB2312ToUTF8[num1].UTF8Code[1];
					cbuf[databufp+2] = ComProtocol_GB2312ToUTF8[num1].UTF8Code[2];
					/* ���û���ָ�� */
					databufp += 3;
					num0 +=1;
					/* ���������������� */
					break;

				}else if(num1 == (ComProtocol_UTF8Num-1))
				{
					/* û�п�ѡ�����ġ� */
					__nop();
					break;
				}
			}
		}
	}
}












/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
