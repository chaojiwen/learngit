/******************************************************************************
 * Ks0108.C - Ks0108 液晶控制器驱动程序
 *
 *
 * DESCRIPTION: -
 *
 * modification history
 * --------------------
 * 01a, 04/sep/2007, 梁炎昌 written
 * --------------------
 ******************************************************************************/

#include "Ks0108.h"
#include "Font.h"
#include "Dev_oled.h"
#include "Mcu_oled.h"
#include "Menu.h"
#include "ucos_ii.h"
#include <string.h>
#include <stdio.h>

/* OLED 控制引脚 */
#define OLED_Power_Open  	{GPIO_ResetBits(OLED_Power_PORT,OLED_Power_PIN);}
#define OLED_Power_Close 	{GPIO_SetBits(OLED_Power_PORT,OLED_Power_PIN);}
/********************************* 宏定义 **********************************/
#define Headline_PosX   (0u)   	//"标题"显示的位置
#define Headline_PosY   (0u)
#define Headline_XSize  (128u) 		//"标题"显示的尺寸
#define Headline_YSize  (16u)	

/********************************* GUI相关全局变量 **********************************/
u8  GUIEnFont = En_8x16;                //英文字体
u8  GUITextMode = GUI_TEXTMODE_NORMAL;  //显示模式

u8 Char_XSIZE;   //英文字体X宽度
u8 Char_YSIZE;   //英文字体Y宽度

u8 PosX = 0;     //当前系统X坐标
u8 PosY = 0;     //当前系统Y坐标




/********************************* 参数寄存器 **********************************/

/*
*****************************************************************************
* Display_Locate - LCD上指定位置 显示指定数据
* DESCRIPTION: -
*
* @Param x0:0--191横向坐标
* @Param y0:0--7  纵向坐标
* @Param DisplayData:写入的数据
* @Return :
* ----
*****************************************************************************
*/
void Display_Locate(u8 DisplayData, u8 x0, u8 y0)
{
	u8 j;

	for( j = 0 ; j < 8 ; j++ )
	{
		if((DisplayData<<j)&0x80)
		{
			OLED_DrawPoint(x0,y0+j,1);
		}else
		{
			OLED_DrawPoint(x0,y0+j,0);
		}	
	}
}


/*
*****************************************************************************
* Display_Clear - 清空LCD显示
* DESCRIPTION: -
*
* @Param void:
* @Return :
* ----
*****************************************************************************
*/
void Display_Clear(void)
{
	OLED_Clear();
}


/************************************************************************************************/
/* 函数名	: Display_Init											 							*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化SSD1306																		*/
/* 作用	 	: 																		*/
/* 创建日期 	: 2015/12/15																		*/
/************************************************************************************************/
void Display_Init(void)
{
	/* 配置OLED_GPIO */
	OLED_Config();
	/* 开OLED电源 */
	OLED_Power_Open;
	/* 等待电源稳定 */
	OSTimeDly(1);
	/* 复位 */
	Clr_OLED_RST;
	/* 等待 */
	OSTimeDly(1);
	/* 复位脚使能 */
	Set_OLED_RST;
	/* 等待 */
	OSTimeDly(100);
	/* 配置OLED_SSD1303 */	
	OLED_Init();
}




/*
*****************************************************************************
* Display_Chinese - 指定位置显示一个16x16的汉字
* DESCRIPTION: -
*
* @Param x0:X轴坐标0--128
* @Param y0:Y轴坐标0--64
* @Param UniCode:汉字内码
* @Return :
* ----
*****************************************************************************
*/
void Display_Chinese(u16 UniCode, u8 x0, u8 y0)
{
  u16 CodeID;
  u8 i, ImgData;
  u8 HZ_X = x0;
  // 在字库里搜索汉字
    for(CodeID=0; CodeID < MAX_Chinese_NUM; CodeID++)
	{
        // 找到 指定汉字的 Index 后, 跳出循环
        if(Han_Zi[CodeID].Code == UniCode)
		{
            break;
        }else if(CodeID == (MAX_Chinese_NUM-1)){//未检索到的汉字,显示个"##"提示吧
            GUI_SetEnFont(En_8x16);
			GUI_DispCharAt('#',x0,y0);
            GUI_DispCharAt('#',x0+Char_XSIZE,y0);
            return;
        }
    }
//----------------------------------
    switch (GUITextMode) 
	{
        case GUI_TEXTMODE_NORMAL :
//-------------------------------------------------------------------------
            // 写汉字的上半部分  (一个汉字 32 个字节,  显示为2行 * 16列)
            for(i=0; i<16; i++)
			{
                ImgData = Han_Zi[CodeID].Font[i];
                Display_Locate(ImgData, HZ_X, y0);
                HZ_X++;
            }
            // 写汉字的下半部分
            HZ_X = x0;
            for(i=16; i<32; i++)
			{
                ImgData = Han_Zi[CodeID].Font[i];
                Display_Locate(ImgData, HZ_X, y0+Char_YSIZE/2);
                HZ_X++;
            }
//-------------------------------------------------------------------------
             break;
        case GUI_TEXTMODE_REVERSE :
//-------------------------------------------------------------------------
            // 写汉字的上半部分  (一个汉字 32 个字节,  显示为2行 * 16列)
            for(i=0; i<16; i++)
			{
              ImgData =  Han_Zi[CodeID].Font[i]^0xFF;
              Display_Locate(ImgData, HZ_X, y0);
              HZ_X++;
            }
            // 写汉字的下半部分
            HZ_X = x0;
            for(i=16; i<32; i++)
			{
              ImgData =  Han_Zi[CodeID].Font[i]^0xFF;
              Display_Locate(ImgData, HZ_X, y0+Char_YSIZE/2);
              HZ_X++;
            }
//-------------------------------------------------------------------------
             break;

        default :
             break;
    }
	OLED_Refresh_Gram();
}


/*
*****************************************************************************
* Display_ASCII - 显示一个ASCII
* DESCRIPTION: -
*
* @Param X:X轴坐标 0--192  (注意边界)
* @Param Y:Y轴坐标 0-- 7   (注意边界)
* @Param Char:ASCII码
* @Return :
* ----
*****************************************************************************
*/
void Display_ASCII(u8 Char, u8 X, u8 Y)
{
	OLED_ShowChar(X,Y,Char,GUIEnFont,1);
	OLED_Refresh_Gram();
}

/*
*****************************************************************************
* Display_InvASCII - 反显示ASCII
* DESCRIPTION: -
*
* @Param X:X轴坐标  0--191
* @Param Y:Y轴坐标  0--7
* @Param Char:ASCII码
* @Return :
* ----
*****************************************************************************
*/
void Display_InvASCII(u8 Char, u8 X, u8 Y)
{
	u8 temp,t,t1;
	u8 y0=Y,x0=X;
	Char=Char-' ';//得到偏移后的值				   
	for(t=0;Y < Char_YSIZE + y0;t++)
		{   
		if(GUIEnFont==En_6x12)		temp=asc2_1206[Char][t];  		//调用1206字体
		else if(GUIEnFont==En_8x16) temp=asc2_1608[Char][t];		//调用1608字体
		else if(GUIEnFont==En_5x8) 	temp=asc2_0805[Char][t];		//调用0808字体
		else if(GUIEnFont==En_12x24)temp=asc2_2412[Char][t];		//调用2412字体
		else if(GUIEnFont==En_16x48)temp=asc2_4816[Char][t];		//调用2412字体
		temp ^= 0XFF;	                          
		for(t1=0;t1<8;t1++)
			{
				if((Y+t1-y0)==Char_YSIZE)break;
				if(temp&0x80)OLED_DrawPoint(X,Y+t1,1);
				else OLED_DrawPoint(X,Y+t1,0);
				temp<<=1;
			}
			X++;
			if((X-x0)==Char_XSIZE)
			{
				X=x0;
				Y+=8; 	
			} 
		} 

	OLED_Refresh_Gram();
}


/*
*****************************************************************************
* GUI_SetTextMode - 设置文字显示模式
* DESCRIPTION: -
*
* @Param TextMode:模式代码
* @Return :模式代码
* ----
*****************************************************************************
*/

void GUI_SetTextMode(u8 TextMode)
{
    GUITextMode = TextMode;
}

/*
*****************************************************************************
* GUI_SetEnFont - 设置显示的英文字体
* DESCRIPTION: -
* 目前支持两种字体8x16 5x8
* @Param EnFont:字体代码
* @Return :
* ----
*****************************************************************************
*/
void GUI_SetEnFont(u8 EnFont)
{
	if(EnFont == En_6x12)
	{
		GUIEnFont = En_6x12;
		Char_XSIZE = En_6x12_XSIZE;
		Char_YSIZE = En_6x12_YSIZE;
	}else if(EnFont == En_8x16) 
	{
		GUIEnFont = En_8x16;
		Char_XSIZE = En_8x16_XSIZE;
		Char_YSIZE = En_8x16_YSIZE;
	}else if(EnFont == En_5x8)
	{
		GUIEnFont = En_5x8;
		Char_XSIZE = En_5x8_XSIZE;
		Char_YSIZE = En_5x8_YSIZE;
	}else if(EnFont == En_12x24)
	{
		GUIEnFont = En_12x24;
		Char_XSIZE = En_12x24_XSIZE;
		Char_YSIZE = En_12x24_YSIZE;	
	}else if(EnFont == En_16x48)
	{
		GUIEnFont = En_16x48;
		Char_XSIZE = En_16x48_XSIZE;
		Char_YSIZE = En_16x48_YSIZE;	
	}

}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void GUI_Delay(u16 Period)
{
	OSTimeDly(Period);
}
/*
*****************************************************************************
* GUI_DispCharAt - 指定位置显示一个ASCII码
* DESCRIPTION: - 字体使用是系统设定的字体
*
* @Param x:X轴坐标
* @Param y:Y轴坐标
* @Param c:ASCII码
* @Return :
* ----
*****************************************************************************
*/
void GUI_DispCharAt(u8 c, u8 x, u8 y)
{
    switch (GUITextMode) 
	{
        case GUI_TEXTMODE_NORMAL :
             Display_ASCII(c, x, y);
             break;
        case GUI_TEXTMODE_REVERSE :
             Display_InvASCII(c, x, y);
             break;
        default :
             break;
    }
}

/*
*****************************************************************************
* GUI_Init - 初始化GUI显示
* DESCRIPTION: -
*
* @Param void:
* @Return :
* ----
*****************************************************************************
*/
void GUI_Init(void)
{
    Display_Init();
    GUIEnFont = En_8x16;
    GUITextMode = GUI_TEXTMODE_NORMAL;
    Char_XSIZE = En_8x16_XSIZE;
    Char_YSIZE = En_8x16_YSIZE;
    PosX = 0;
    PosY = 0;

}

void GUI_Clear(void)
{
    Display_Clear();
}


/*
*****************************************************************************
* GUI_DispStringAt - 指定位置显示一个字符串
* DESCRIPTION: -
* 支持中英混合 当中英混合时英文字体强制为8x16 如果单单英文 按照设定字体
* @Param x0:X轴坐标
* @Param y0:Y轴坐标
* @Param s:字符串
* @Return :
* ----
*****************************************************************************
*/
void GUI_DispStringAt(u8 *s,u8 x0,u8 y0)
{
    u8 i;

    i = 0;
    while(s[i]!='\0'){
        if (s[i] < 0x80) {//是ASCII
             if(s[i] == '\n'){
                    x0 = 0;
                    y0 += Char_YSIZE;
             } else {
                if (x0 > (LCD_XSIZE-1)) {//换行
                    x0 = 8; //原先是x0 = 0;当超出一行内容后超出的部分在第二行的x0=8处显示
                    y0 += Char_YSIZE;
                }
                switch (GUITextMode) {
                    case GUI_TEXTMODE_NORMAL :
                         Display_ASCII(s[i], x0, y0);
                         break;
                    case GUI_TEXTMODE_REVERSE :
                         Display_InvASCII(s[i], x0, y0);
                         break;

                    default :
                         break;
                }
                x0 += Char_XSIZE;
             }
        } else {//是中文
            if (x0 > LCD_XSIZE-16){
                x0 = 0;
                y0 += Chinese_YSIZE;
            }
            Display_Chinese(s[i]*256+s[i+1],x0,y0);//中文
            i++;
            x0 += Chinese_XSIZE;
       }
       i++;
    }
}


/*
*****************************************************************************
* GUI_DispStringAtBar - 显示一个字符串在一个BAR中
* DESCRIPTION: -
* 可以设定对齐方式
* @Param s:字符串
* @Param x0:BAR起始X轴坐标
* @Param x1:BAR结束点的X轴坐标
* @Param y0:BAR的Y轴坐标
* @Param Mode:对齐方式 居中于BAR 左对齐BAR 右对齐BAR
* @Return :
* ----
*****************************************************************************
*/
void GUI_DispStringAtBar(u8 *s ,u8 x0,u8 y0, u8 x1,u8 Mode)
{
    u8 ENCount;//英文统计
    u8 CNCount;//中文统计
    u8 XLen;
    u8 X;
    u8 Y;
    u8 i;
	
	if(x1 >(MenuBarPosX-1))
	{
		x1 = (MenuBarPosX-1);
	}
    CNCount = 0;
    ENCount = 0;
    XLen = 0;
    i = 0;
    while(s[i]!='\0'){//统计中英文个数
        if (s[i] > 0x80) {
            CNCount ++;
            i++;
        } else {
           ENCount++;
        }
        i++;
    }
 
    XLen = ENCount*8 + CNCount*16;

    if (XLen > (MenuBarPosX-1)) {//我们只能显示一行
        XLen = (MenuBarPosX-1);
    }
//识别对其方式属性
    switch (Mode&0x30) {//只有这两个Bit是这个属性
        case GUI_TA_LEFT: //左对齐
            XLen = 0;
            break;
        case GUI_TA_HCENTER:  //居中
            XLen  = ((x1-x0)-XLen)/2;
            break;
        case GUI_TA_RIGHT:   //右对齐
            XLen  = (x1-x0)-XLen;
            break;
        default:
            break;
    }
//--------------------------------------
//得到喀什显示的起点坐标
    X = x0+XLen;
    Y = y0;

//---------------------
//清除字符前面的空白部分
    if(Mode&GUI_JUST_TEXT){
    } else {
        x0 = X;
        if (GUITextMode == GUI_TEXTMODE_REVERSE){
            for(i = 0; i <x0; i++){
                Display_Locate(0xFF, i,y0);
                Display_Locate(0xFF, i,y0+8);
            }
        }else{
            for(i = 0; i <x0; i++){
                Display_Locate(0x00, i,y0);
                Display_Locate(0x00, i,y0+8);
            }
        }
    }

//-----------------------
    i = 0;
    while(s[i]!='\0'){
        if (s[i] < 0x80) {//是ASCII
            switch (GUITextMode) {
                case GUI_TEXTMODE_NORMAL :
                     Display_ASCII(s[i], X, Y);
                     break;
                case GUI_TEXTMODE_REVERSE :
                     Display_InvASCII(s[i], X, Y);
                     break;
                default :
                     break;
            }
            X += 8;
        } else {//是中文
            Display_Chinese(s[i]*256+s[i+1],X,Y);//中文
            i++;
            X += Char_XSIZE*2;
       }
//       s++;
        i++;
		if(X > (MenuBarPosX-1))
		{
			/* 超出显示 范围 */
			break;
		}
    }
//清除字符后面的空白部分
    if(Mode&GUI_JUST_TEXT){
        GUI_SetEnFont(En_6x12);
        return;
    } else {
        x0 = X;
        if (GUITextMode == GUI_TEXTMODE_REVERSE){
            for(i = 0; i <x1-x0; i++){

	                Display_Locate(0xFF, x0+i,y0);
	                Display_Locate(0xFF, x0+i,y0+8);
            }
        }else{
            for(i = 0; i <x1-x0; i++){
	                Display_Locate(0x00, x0+i,y0);
	                Display_Locate(0x00, x0+i,y0+8);				
            }
        }
		OLED_Refresh_Gram();
        return;
    }
//---------------------------------------
}

/*
*****************************************************************************
* GUI_DrawIcon - 制定位置显示一个32x32的图标
* DESCRIPTION: -
*
* @Param x0:X轴坐标
* @Param y0:Y轴坐标 注意边界!
* @Param Ico:图标数据
* @Return :
* ----
*****************************************************************************
*/
void GUI_DrawIcon(u8 const *Ico,u8 x0,u8 y0)
{
    u8 i;
    u16 Count;
    Count = 0;
    if( y0 > LCD_YSIZE){//边界保护
        y0 = LCD_YSIZE;
    }

    if(x0 > LCD_XSIZE){
        x0 = LCD_XSIZE;
    }

    for(i=0;i<32;i++){//第一行
		Display_Locate(Ico[Count++],x0+i,y0);
    }

	y0 += 8;
    for(i=0;i<32;i++){//第二行
        Display_Locate(Ico[Count++],x0+i,y0);
    }

	y0 += 8;
    for(i=0;i<32;i++){//第三行
        Display_Locate(Ico[Count++],x0+i,y0);
    }

	y0 += 8;
    for(i=0;i<32;i++){//第四行
        Display_Locate(Ico[Count++],x0+i,y0);
    }

	OLED_Refresh_Gram();
}

/*
*********************************************************************************************************
*	函 数 名: GUI_Disp_Headline
*	功能说明: 动态 调整 显示 父目录 标题
*	形    参：标题 字符串
*	返 回 值: 无
*********************************************************************************************************
*/
void GUI_Disp_Headline(u8 *String)
{	
	/* 存放标题字符串缓存. */
	char hle_str_buf[16] = {0};
	/* 字符串床度寄存器 */
	u8 hle_len;
	/* 计算 字符串 长度 */
	hle_len = strlen((const char *)String);
	
	/* 判断标题字符长度。 */
	if(hle_len > (Headline_XSize/En_8x16_XSIZE))
	{
		/* 标题字符长度大于16个字节，截取前16个字节字符信息. */
		strncpy(hle_str_buf,(char*)String,(Headline_XSize/En_8x16_XSIZE));
	}else
	{
		/* 字符串长度小于16字节 */
		strncpy(hle_str_buf,(char*)String,hle_len);
	}
	
	
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
	
	/* 小于8个字符 */
	GUI_DispStringAt((u8*)hle_str_buf,((Headline_XSize/En_8x16_XSIZE)-hle_len)*(En_8x16_XSIZE/2)+Headline_PosX,Headline_PosY);

}


/*
*****************************************************************************
* GUI_Draw_Battery_Power - 制定位置显示一个32x16的图标
* DESCRIPTION: -
*
* @Param x0:X轴坐标
* @Param y0:Y轴坐标 注意边界!
* @Param Ico:图标数据
* @Return :
* ----
*****************************************************************************
*/
void GUI_Draw_Bar_BMP(u8 const *Ico,u8 x0,u8 y0)
{
    u8 i;
    u16 Count;
    Count = 0;
    if( y0 > LCD_YSIZE){//边界保护
        y0 = LCD_YSIZE;
    }

    if(x0 > LCD_XSIZE){
        x0 = LCD_XSIZE;
    }

    for(i=0;i<21;i++){//第一列
		Display_Locate(Ico[Count++],x0+i,y0);
    }
	y0 += 8;
    for(i=0;i<21;i++){//第二列
        Display_Locate(Ico[Count++],x0+i,y0);
    }

	OLED_Refresh_Gram();
}
/*显示通道号图片*/
void GUI_Draw_CH_NO(u8 const *Ico,u8 x0,u8 y0)
{
/* 通道标号图片的宽度 */
#define CH_BMP_XSize	(32u)
/* 通道标号图片的高度 */
#define CH_BMP_YSize	(16u)

	u8 i;
    u16 Count;
    Count = 0;
    if( y0 > LCD_YSIZE){//边界保护
        y0 = LCD_YSIZE;
    }

    if(x0 > LCD_XSIZE){
        x0 = LCD_XSIZE;
    }

    for(i=0;i<CH_BMP_XSize;i++){//第一列
		Display_Locate(Ico[Count++],x0+i,y0);
    }
	y0 += CH_BMP_YSize/2;
    for(i=0;i<CH_BMP_XSize;i++){//第二列
        Display_Locate(Ico[Count++],x0+i,y0);
    }

	OLED_Refresh_Gram();
	
#undef CH_BMP_XSize
#undef CH_BMP_YSize
}

/************************************************************************************************/
/* 函数名	: GUI_Draw_Card_SN_NO														 	 	*/
/* 输入  	: sn_no:SN号；x0:X坐标；y0:Y坐标														*/
/* 输出		: 无 																				*/
/* 作用		: 显示卡片SN号(前4位数据)							 									*/
/* 创建日期	: 2015/4/23																			*/
/************************************************************************************************/
void GUI_Draw_Card_SN_NO(u32 sn_no,u8 x0,u8 y0)
{
/* 显示SM号数量(默认是4个) */
#define Dis_SN_NO_Num		(4u)
	
	u8 num;
	u8 value;
	
	/* 设置字体 */
	GUI_SetEnFont(En_8x16);
	
    for(num = 0;num < Dis_SN_NO_Num;num ++)
	{
        value = (((sn_no>>(4*num))&0xF)+0x30);
		GUI_DispCharAt(value,(x0+(Dis_SN_NO_Num-1-num)*En_8x16_XSIZE),y0);
    }
	
#undef Dis_SN_NO_Num
}

/*
*****************************************************************************
* GUI_Draw_Battery_Power - 制定位置显示一个32x16的图标
* DESCRIPTION: -
*
* @Param x0:X轴坐标
* @Param y0:Y轴坐标 注意边界!
* @Param Ico:图标数据
* @Return :
* ----
*****************************************************************************
*/
void GUI_Draw_Sensor_ICO(u8 const *Ico,u8 x0,u8 y0)
{
    u8 i;
    u16 Count;
    Count = 0;
    if( y0 > LCD_YSIZE){//边界保护
        y0 = LCD_YSIZE;
    }

    if(x0 > LCD_XSIZE){
        x0 = LCD_XSIZE;
    }

    for(i=0;i<24;i++){//第一列
		Display_Locate(Ico[Count++],x0+i,y0);
    }
	y0 += 8;

    for(i=0;i<24;i++){//第二列
        Display_Locate(Ico[Count++],x0+i,y0);
    }
	y0 += 8;

    for(i=0;i<24;i++){//第三行
        Display_Locate(Ico[Count++],x0+i,y0);
    }
	y0 += 8;

    for(i=0;i<24;i++){//第四行
        Display_Locate(Ico[Count++],x0+i,y0);
    }
	y0 += 8;

    for(i=0;i<24;i++){//第五行
        Display_Locate(Ico[Count++],x0+i,y0);
    }
	y0 += 8;

    for(i=0;i<24;i++){//第六行
        Display_Locate(Ico[Count++],x0+i,y0);
    }

	OLED_Refresh_Gram();
}



/*
*****************************************************************************
* GUI_DispDecAt - 显示十进制数值
* DESCRIPTION: -
* 处理长度最多5个数字(因为u16--->65536)
* @Param v:显示的数据
* @Param x:X轴坐标
* @Param y:Y轴坐标  XY 均是起点位置坐标 也就是数值最高位的坐标
* @Param Len:指定的显示长度1--5内
* @Return :
*
*****************************************************************************
*/
void GUI_DispDecAt(u16 v, u16 x, u16 y, u8 Len)
{
    u8 i;
    u8 CharBuf[5];
    u8 HighByte;

    HighByte = 0;
    for(i = 0; i < 5; i++){
        CharBuf[i] = (u8)(v%10);
        v = v/10;
        if(CharBuf[i]){
            HighByte = i;
        }
    }
    //第0位无论如何也显示
    i = 0;
    GUI_DispCharAt(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
    for(i = 1; i < Len; i++){
        if(CharBuf[i]){
            GUI_DispCharAt(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
        }else if(i > HighByte){
            GUI_DispCharAt(' ',x+((Len-1)-i)*Char_XSIZE,y);
        }
    }

}


/*
*****************************************************************************
* HBar - 显示一个水平的进度条
* DESCRIPTION: -
* 附加有百分比显示
* @Param x0:进度条起点X轴坐标   0-->127
* @Param x1:进度条结束点X坐标   0-->127  必须大于x0  百分比显示于该坐标之后
* @Param y:进度条Y轴坐标        0--7
* @Param percent:当前百分值     0-->100
* @Return :
*
*****************************************************************************
*/
void HBar(u8 y, u8 x0, u8 x1,u8 percent)
{
    u8 u8Temp;
    u8 i;
    float Center;
    Center = (x1-x0);
    Center *= percent;
    Center /= 100;
//    u8Temp = (x1-x0)*percent/100;//这个计算做法在430上能用，但C51下似乎必须用浮点算
    u8Temp = (u8)Center;
    Display_Locate(0xFF, x0, y);
    Display_Locate(0xFF, x1, y);
    for(i = 1; i < u8Temp; i++){
        Display_Locate(0xBD, x0+i, y);
    }
    for(i = x0+u8Temp+1; i < x1; i++){
        Display_Locate(0x81, i, y);
    }

	OLED_Refresh_Gram();
}
/*                  x1 +3
|-------------------|
|                   ||
|                   |||
|                   ||||
|                   ||||
--------------------
 -------------------
  ------------------
x0--->x1+3
y0--->y1

*/
void TipClr( u8 x0, u8 y0, u8 x1, u8 y1)
{
	OLED_Fill(x0,y0,x1,y1,0);
}
//--------------
/*
Bar的算法
    ___
 |   |
 |   |
| |<-|-----BarLen
 |   L
 |   |
 |   |
 |  _|_
Bar的滑动距离是L-BarLen
为了美观,可以在开始和结尾部分多流出来一些点,那么滑动距离要扣除这些点的长度,并在计算结果
得到0的时候,添加上上端要留出来的点BarRemainDot
2种显示方式:
一种是BarLen是定长的,
一种BarLen是根据显示总共的项数定下来的
*/


//--------------
//Bar的长度
//预留出来的点
#define BarRemainDot  3
//数字显示位置
//#define BarNumPosX (128-8+2)
#define BarNumPosY (3)
//Bar的显示开始/结束位置
//#define BarBeginPosX (126)
#define BarBeginPosY (0*8)
#define BarEndPosX (126)
#define BarEndPosY (6*8)

__IO u8 const BarCode0[]={0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x01,0x00};
__IO u8 const BarCode1[]={0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF};
extern u8 ItemBackup_i;
extern u8 ItemBackup[];

void Bar(u8 Item_,u8 ItemNum_,u8 BarPosX,u8 BarNumPosX)
{
    u8 YOffset;
    u16 Temp;
    u8 BarLen;
    u8 Y;


    BarLen = (BarEndPosY-BarBeginPosY-BarRemainDot)/(ItemNum_);//BarLen根据ItemNum_得到
    if (BarLen == 0) {
        BarLen = 5;
    }
    BarLen = 8;
    Temp = Item_*(BarEndPosY-BarBeginPosY-BarLen-BarRemainDot);//BarRemainDot是被扣除的部分
    Temp = Temp/(ItemNum_-1);
    YOffset = (u8)Temp;
    if(!Temp){//顶端，把预留的加上
        YOffset = BarRemainDot;
    }

    for(Y = 0;Y < BarEndPosY/8;Y++){
        if((Y != (YOffset/8))&&(Y != (YOffset/8+1))){
            Display_Locate((0x00),BarPosX,Y*8+16);//清除 X=125 列
			OLED_Refresh_Gram();
            Display_Locate((0xFF),BarPosX+1,Y*8+16);//X=126列画线
			OLED_Refresh_Gram();
            Display_Locate((0x00),BarPosX+2,Y*8+16);//清除 X=127 列
			OLED_Refresh_Gram();
        }else{//Y = YOffset/8 Y = YOffset/8＋1
            Display_Locate((BarCode0[YOffset%8]),BarPosX,(YOffset/8)*8+16);
			Display_Locate((0xFF-BarCode0[YOffset%8]),BarPosX+1,(YOffset/8)*8+16);
			Display_Locate((BarCode0[YOffset%8]),BarPosX+2,(YOffset/8)*8+16);
			if((YOffset/8)+1 < (BarEndPosY/8)){//防止下越界
                Display_Locate((BarCode1[YOffset%8]),BarPosX,(YOffset/8+1)*8+16);
				Display_Locate((0xFF-BarCode1[YOffset%8]),BarPosX+1,(YOffset/8+1)*8+16);
				Display_Locate((BarCode1[YOffset%8]),BarPosX+2,(YOffset/8+1)*8+16);
			}
        }
    }
	OLED_Refresh_Gram();
}






