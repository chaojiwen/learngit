/************************************************************************************************/
/** @file              : ComProtocol_SetPlateNo.c												*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017年02月14日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--变送器--通信协议--设置车牌号码 应用文件						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : ComProtocol_SetPlateNo.c															*/
/**作用       : 将GB2313格式的车牌号码，转换为UTF8格式											*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2017年02月14日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#include "ComProtocol_SetPlateNo.h"

#include <stdio.h>
#include <string.h>


/*******************************************************************************/
/********************************* 数据结构 *************************************/
/* GB2312转换为UTF-8数据结构 */
typedef struct {
	uint16_t	GB2312Code;
	uint8_t  	UTF8Code[3];
}ComProtocol_UTF8;

/********************************* UTF8编码表 **********************************/
/* UTF8编码数量 */
#define ComProtocol_UTF8Num					(32u)
/* 这个UTF-8表主要是用来转换车牌号中的中文编码 */
const ComProtocol_UTF8 ComProtocol_GB2312ToUTF8[ComProtocol_UTF8Num] =
{
	{0xB2D8 ,0xE8,0x97,0x8F},//'藏'
	{0xB4A8 ,0xE5,0xB7,0x9D},//'川'
	{0xB6F5 ,0xE9,0x84,0x82},//'鄂'
	{0xB8CA ,0xE7,0x94,0x98},//'甘'
	{0xB8D3 ,0xE8,0xB5,0xA3},//'赣'
	{0xB9F0 ,0xE6,0xA1,0x82},//'桂'
	{0xB9F3 ,0xE8,0xB4,0xB5},//'贵'
	{0xBADA ,0xE9,0xBB,0x91},//'黑'
	{0xBBA6 ,0xE6,0xB2,0xAA},//'沪'
	{0xBCAA ,0xE5,0x90,0x89},//'吉'
	{0xBCBD ,0xE5,0x86,0x80},//'冀'
	{0xBDF2 ,0xE6,0xB4,0xA5},//'津'
	{0xBDFA ,0xE6,0x99,0x8B},//'晋'
	{0xBEA9 ,0xE4,0xBA,0xAC},//'京'
	
	{0xC1C9 ,0xE8,0xBE,0xBD},//'辽'
	{0xC2B3 ,0xE9,0xB2,0x81},//'鲁'
	{0xC3C9 ,0xE8,0x92,0x99},//'蒙'
	{0xC3F6 ,0xE9,0x97,0xBD},//'闽'
	{0xC4FE ,0xE5,0xAE,0x81},//'宁'
	{0xC7E0 ,0xE9,0x9D,0x92},//'青'
	{0xC7ED ,0xE7,0x90,0xBC},//'琼'
	{0xC9C2 ,0xE9,0x99,0x95},//'陕'
	{0xCBD5 ,0xE8,0x8B,0x8F},//'苏'
	{0xCDEE ,0xE7,0x9A,0x96},//'皖'
	{0xCFE6 ,0xE6,0xB9,0x98},//'湘'
	
	{0xD0C2 ,0xE6,0x96,0xB0},//'新'
	{0xD3E5 ,0xE6,0xB8,0x9D},//'渝'
	{0xD4A5 ,0xE8,0xB1,0xAB},//'豫'
	{0xD4C1 ,0xE7,0xB2,0xA4},//'粤'
	{0xD4C6 ,0xE4,0xBA,0x91},//'云'
	{0xD5E3 ,0xE6,0xB5,0x99},//'浙'
	{0x0000 ,0x00,0x00,0x00},//''
};

/************************************************************************************************/
/* 函数名	: ComProtocol_SetPlateNo										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 设置车牌号码					 													*/
/* 创建日期	: 2016/12/23																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void ComProtocol_SetPlateNo(uint8_t* cbuf,char* plateNoString)
{
	
	/* 字符串长度 */
	uint8_t cStrLen;
	/* 计数器 */
	uint8_t num0,num1;
	/* 数据缓存指针 */
	uint8_t databufp;
	
	
	/* 计算字符串长度 */
	cStrLen = strlen(plateNoString);
	
	/* 判断字符串长度是否超出设定最大值 */
	if(12 < cStrLen)
	{
		/* 超出可设置的最大字符串长度 */
		__nop();
		return;
	}
	
	/* 转换编码方式 */
	for(num0 = 0,databufp = 0 ;num0 < cStrLen ;num0++)
	{
		/* 判断当前字符是中文还是ASCII码。 */
		if ((((uint8_t*)plateNoString)[num0]) < 0x80)
		{
			/* ASCII码 */
			/* ASCII字符等于UTF8字符 */
			cbuf[databufp] = plateNoString[num0];
			/* 设置指针 */
			databufp += 1;
			/* 继续查找其他文字 */
			continue;
		}else
		{
			/* 中文内码指针 */
			uint8_t* cCodeHighp = ((uint8_t*)(&plateNoString[num0]));
			uint8_t* cCodeLowp = ((uint8_t*)(&plateNoString[num0+1]));
			
			/* 中文(GBK) */
			/* 查找当前GBK中文的UTF8编码。 */
			for(num1 = 0;num1 < ComProtocol_UTF8Num ;num1++)
			{
				/* 查询当前中文内码对应的UTF8编码 */
				if(ComProtocol_GB2312ToUTF8[num1].GB2312Code == (*cCodeLowp + (((uint16_t)(*cCodeHighp))<<8)))
				{
					/* 将UTF8值转移到缓存中。 */
					cbuf[databufp] 	 = ComProtocol_GB2312ToUTF8[num1].UTF8Code[0];
					cbuf[databufp+1] = ComProtocol_GB2312ToUTF8[num1].UTF8Code[1];
					cbuf[databufp+2] = ComProtocol_GB2312ToUTF8[num1].UTF8Code[2];
					/* 设置缓存指针 */
					databufp += 3;
					num0 +=1;
					/* 继续查找其他文字 */
					break;

				}else if(num1 == (ComProtocol_UTF8Num-1))
				{
					/* 没有可选的中文。 */
					__nop();
					break;
				}
			}
		}
	}
}












/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
