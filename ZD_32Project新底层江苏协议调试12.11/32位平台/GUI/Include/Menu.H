/************************************************************************************************/
/** @file              : Menu.c																	*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月16日															*/
/** @brief             : c file																	*/
/** @description       : GUI->参数管理															*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#ifndef __Menu_H
#define __Menu_H


#include "Dev_Fram.h"
#include "Log_Sensor.h"
#include "System_Config.h"
#include "Gsm_Api.h"

#define MenuWhat(n) {(u8 **)n,(sizeof(n)/sizeof(u8 *)),0}
/* 返回当前菜单 */
#define SelfMenuIndex FatherIndex[Layer-1]

/* Bar项的显示XY偏置 */
#define BarXOffset				(0u)
#define BarYOffset				(16u)
/* Bar项的XY尺寸 */
#define BarXSize				(7*12)
#define BarYSize				(16u)
/* Bar项的XY尺寸 */
#define DataXSize				(16u)
#define DataYSize				(16u)
/* 语言代码 */
/* 中文 */
#define CN    					(0u)
/* 英文 */
#define EN    					(1u)
/* 不初始化Flash */
#define FlashMode_NoAction     	(0u)
/* 系统系统负责初始化 */
#define FlashMode_AutoInit     	(1u)
/* 手动初始化 */
#define FlashMode_ManualInit   	(2u)
/* 重绘画面 */
#define FlashMode_ReFlash      	(3u)
/* 桌面信息重绘画面 */
#define FlashMode_DeskTop      	(4u)
/* 菜单深度最大值 */
#define MenuSize 				(7u)
/* 单屏可以显示的最大项 通常这是个固定值--因为你的液晶的大小决定了你一次可以显示的多少,你也可以在程序中修改,但记得维护它--因为所有的Bar型菜单都是要用到它的 */
#define DispMax  				(3u)
#define DispdataMax  			(5u)
/* DispMax */
extern u8 DispMin;
/* DispdataMax */
extern u8 DispdataMin;

/* Bar型菜单的数据结构 */
struct Menu
{
//  u8 const **Text;    //指向显示内容数组的指针
    u8 **Text;     		//指向显示内容数组的指针
    u8 TextNum;         //项个数
    void* (*Action)();  //对应的执行函数
};

/* 用户权限结构 */
typedef enum{Domestic = 0 , Admin = 1, Super_Admin = 2,}User_Type;

/* 对外声明定义于Menu.C的内容. */
/* 子项菜单返回 刷新标志 */
extern u8 ReFlash ;
/* 跳转进入子菜单 刷新标志 */
extern u8 Flash ;
/* 索引 */
extern u16 KeyFuncIndex ;
/* 父菜单备份数组 */
extern u16 FatherIndex[MenuSize+1];
/* 菜单层数 */
extern u8 Layer;
/* 当前菜单项备份 */
extern u8 ItemBackup[MenuSize];
/*  */
extern u8 ItemBackup_i;
/* 反显位置备份 */
extern u8 WhereBackup[MenuSize];
/*  */
extern u8 WhereBackup_i;
/* 菜单资源指针 */
extern struct Menu * BarMenu;
/* Tip指针 */
extern struct Menu * TipBar;
/* 当前Bar项  范围:[0,ItemNum-1]  包括0 包括ItemNum-1  共包括ItemNum个取值 */
extern u8 Item;
/* Bar项数 */
extern u8 ItemNum;
/* 当前反显位置,[0,DispMax-1]  反映反显的Bar在屏幕上的位置 */
extern u8 Where;
/* 参数选择窗口的光标位置 */
extern u8 Input_Para_Win_Cursor;
/* 项显示内容指针 */
extern u8 **DispItem;

/* 设置菜单指针 */
extern void Jump2Menu(u16 Index,u8 FlashMode);

/* 菜单 "开" "光" 中文选项 字符 */
#define Menu_ON_CN			("开")
#define Menu_OFF_CN			("关")
/* 菜单 "开" "光" 英文选项 字符 */
#define Menu_ON_EN			("ON")
#define Menu_OFF_EN			("OFF")



/* Ico图标索引 */
extern u8 const * const Ico_Index[];
/* 信号强度图标索引 */
extern u8 const * const Mobile_Signal_Index[];
/* 通道警报图标索引 */
extern u8 const * const  Alarm_Index[];
/* 电池电量图标索引 */
extern u8 const * const Battery_Power_Index[];
/* 传感器类型图标 */
extern u8 const * const Sensor_Type_Index[];
/* 用户类型图标 */
extern u8 const * const User_Type_Index[];
/* 传感器通道 */
extern u8 const * const Sensor_CH_Index[];
/* 数据存储开关图标 */
extern u8 const * const Menu_Save_Data_Index[];
/* 开始卸货图标 */
extern u8 const * const Menu_StartUpload_Index[];
/* Sim卡无网络信号 */
extern u8 const * const Mobile_SIM_NoSignal;
/* 未检测到Sim卡 */
extern u8 const * const Mobile_SIM_NotDetected;
/* SIM欠费停机,也有可能是空号 */
extern u8 const * const Mobile_SIM_ShutDown;


/* 用户权限寄存器 */
extern User_Type User;

/* 菜单语言寄存器 */
extern u8* Language;

/* 数字输入窗口 中英文 菜单栏 */
extern u8 * const Menu_Digital_Input[11];
/* 是否 选择选项项目栏 中文  */
extern u8 * const Menu_Yes_NO_CN[2];
/* 是否 选择选项项目栏 英文  */
extern u8 * const Menu_Yes_NO_EN[2];


/* Ico菜单界面项目栏 */
extern const struct Menu Menu_Ico[];
/* 设置菜单界面项目栏 */
extern const struct Menu Menu_Alarm_Set[];





/* 通道界面（无传感器时） 中文 菜单栏 */
extern u8 * const Cha_NULL_CN_Tab;
/* 通道界面（无传感器时） 英文 菜单栏 */
extern u8 * const Cha_NULL_EN_Tab;
/* 通道选择中文列表 */
extern u8 * Cha_Tsd_CN_Tab[];
/* 通道选择英文列表 */
extern u8 * Cha_Tsd_EN_Tab[];
/* 通道阈值界面 中文 菜单项目栏 */
extern u8 * Menu_Cha_Tsd_CN[Sensor_CH_Config_Num];
/* 通道阈值界面 英文 菜单项目栏 */
extern u8 * Menu_Cha_Tsd_EN[Sensor_CH_Config_Num];
/* 通道阈值界面 中英文 菜单栏 */
extern const struct Menu Menu_Cha_Tsd[];



/* 传感器上下限选择项目栏 */
extern const struct Menu Menu_Sen_Up_Low_lit_Item[];



/* 传感器选择界面 中英文 项目栏 */
extern const struct Menu Menu_Sen_Lit_Sec[];

/* 传感器设置界面 中英文 项目栏 */
extern const struct Menu Menu_Sen_Lit_Set[];


/* 声光报警界面 中英文 菜单栏 */
extern const struct Menu Menu_Sud_Lgt_Aam[];
/* 声光报警通道选择界面 中英文 菜单栏 */
extern const struct Menu Menu_Sud_Lgt_Aam_Cha_Sec[];


/* 短信菜单项目栏 */
extern const struct Menu Menu_SMS_Aam[];
/*短信报警通道选择界面 中英文 菜单栏 */
extern const struct Menu Menu_SMS_Aam_Cha_Sec[];
/*短信报警号码选择界面 中英文 菜单栏 */
extern const struct Menu Menu_SMS_Aam_Poe_NO[];


/* 运行参数菜单项目栏 */
extern const struct Menu Menu_Run_Para[];

/* 网络参数菜单项目栏 */
extern const struct Menu Menu_Nwk_Para[];

/* 系统设置菜单项目栏 */
extern const struct Menu Menu_Stm_Set[];

/* 打印设置菜单项目栏 */
extern const struct Menu Menu_Pit_Set[];

#if (Gateway_NJSY_Enable == 0x01)
/* 打印设置菜单项目栏 */
extern const struct Menu MenuGW_Item[];

#elif (Gateway_HZZH_Enable == 0x01)
extern const struct Menu MenuGWHZZH_Item[];
#endif

#if (GSM_Module_Type == 0x02)//Wifi模块

extern const struct Menu MenuWifi_Item[];
#endif






/* 时钟设置菜单项目栏 */
extern const struct Menu Menu_Clock[];
/* 打印设置菜单项目栏 */
extern const struct Menu Menu_Print[];
/* 打印传感器数据界面菜单项目栏 */
extern const struct Menu Print_Sensor_Data[];

/* 将128*64 纵向平均分4块（16*4）*/
#define LCD_XSIZE  		(128u)
#define LCD_YSIZE  		(64u)

#define Line_1_PosX   	(0u)       	//"进入"显示的位置
#define Line_1_PosY   	(0u)
#define Line_1_XSize  	(LCD_XSIZE) //"进入"显示的尺寸
#define Line_1_YSize  	(16u)			

#define Line_2_PosX   	(0u)       	//"进入"显示的位置
#define Line_2_PosY   	(16u)
#define Line_2_XSize  	(LCD_XSIZE)	//"进入"显示的尺寸
#define Line_2_YSize  	(16u)			

#define Line_3_PosX   	(0u)       	//"进入"显示的位置
#define Line_3_PosY   	(32u)
#define Line_3_XSize	(LCD_XSIZE)	//"进入"显示的尺寸
#define Line_3_YSize  	(16u)			

#define Line_4_PosX   	(0u)       	//"进入"显示的位置
#define Line_4_PosY   	(48u)
#define Line_4_XSize  	(LCD_XSIZE)	//"进入"显示的尺寸
#define Line_4_YSize  	(16u)		

/* 首界面 第一行 状态栏图标位置顺序机大小。 */

#define Mobile_Signal_BMP_PosX     			(0u)
#define Mobile_Signal_BMP_PosY     			(0u)
#define Mobile_Signal_BMP_XSize    			(21u)
#define Mobile_Signal_BMP_YSize    			(16u)

#define Menu_Save_Data_BMP_PosX     		(Mobile_Signal_BMP_PosX + Mobile_Signal_BMP_XSize + 2)
#define Menu_Save_Data_BMP_PosY     		(0u)
#define Menu_Save_Data_BMP_XSize    		(21u)
#define Menu_Save_Data_BMP_YSize    		(16u)

#define Sensor_CH_PosX     					    (Menu_Save_Data_BMP_PosX + Menu_Save_Data_BMP_XSize + 2)
#define Sensor_CH_PosY     					    (0u)
#define Sensor_CH_XSize    					    (32u)
#define Sensor_CH_YSize    					    (16u)
                                        
#define Alarm_BMP_PosX     					    (Sensor_CH_PosX + Sensor_CH_XSize + 2)
#define Alarm_BMP_PosY    					    (0)
#define Alarm_BMP_XSize   					    (21)
#define Alarm_BMP_YSize    					    (16)

#define Battery_Power_BMP_PosX     			(Alarm_BMP_PosX + Alarm_BMP_XSize + 5)
#define Battery_Power_BMP_PosY     			(0u)
#define Battery_Power_BMP_XSize    			(21u)
#define Battery_Power_BMP_YSize    			(16u)



/* Font1608 字体大小 */
#define Font1608_Size                    (8u)
/* 菜单Bar */                            
#define MenuBarPosX                      (125u)
#define MenuBarNumPosX                   (120u)
                                         

/* 界面索引 */
#define MenuDeskTopIndex       			                 (0u)
#define MenuIcoIndex          			                 (MenuDeskTopIndex 				+ 7u)
#define MenuStartRecordIndex                         (MenuIcoIndex 					+ 7u)
#define MenuStopRecordIndex          	               (MenuStartRecordIndex 			+ 7u)

//
#define MenuAlarmSetIndex     			                 (MenuStopRecordIndex 			+ 7u)
////
#define MenuChaTsdIndex					                     (MenuAlarmSetIndex 				+ 7u)
#define MenuSenUpLowlitSecIndex			                 (MenuChaTsdIndex 				+ 7u)
#define MenuSenUpLitSetIndex			                   (MenuSenUpLowlitSecIndex 		+ 7u)
#define MenuSenLowLitSetIndex			                   (MenuSenUpLitSetIndex 			+ 7u)
////                                                 
#define MenuSudLgtAamIndex				                   (MenuSenLowLitSetIndex			+ 7u)
#define MenuSudLgtAamMtrSthSetIndex		               (MenuSudLgtAamIndex				+ 7u)
#define MenuLowPwrSudLgtAamSetIndex		               (MenuSudLgtAamMtrSthSetIndex	+ 7u)
#define MenuCutPwrSudLgtAamSetIndex		               (MenuLowPwrSudLgtAamSetIndex	+ 7u)
#define MenuSudLgtAamChaSecIndex		                 (MenuCutPwrSudLgtAamSetIndex	+ 7u)
#define MenuSudLgtAamSetIndex			                   (MenuSudLgtAamChaSecIndex		+ 7u)
////                                                 
#define MenuSMSAamIndex					                     (MenuSudLgtAamSetIndex			+ 7u)
#define MenuSMSAamMtrSthSetIndex		                 (MenuSMSAamIndex				+ 7u)
#define MenuLowPwrSMSAamSetIndex		                 (MenuSMSAamMtrSthSetIndex		+ 7u)
#define MenuCutPwrSMSAamSetIndex		                 (MenuLowPwrSMSAamSetIndex		+ 7u)
#define MenuSMSAamChaSecIndex			                   (MenuCutPwrSMSAamSetIndex		+ 7u)
#define MenuSMSAamSetIndex				                   (MenuSMSAamChaSecIndex			+ 7u)
#define MenuSMSAamPoeNOIndex			                   (MenuSMSAamSetIndex 			+ 7u)
#define MenuSMSAamPoeNOSetIndex		                   (MenuSMSAamPoeNOIndex 			+ 7u)
#define MenuSMSAamRptIrlIndex			                   (MenuSMSAamPoeNOSetIndex 		+ 7u)
////
#define MenuAamDlyIndex					                     (MenuSMSAamRptIrlIndex 			+ 7u)
////
#define MenuRunParaIndex				                     (MenuAamDlyIndex 				+ 7u)
//
#define MenuNmlSaveIrlIndex				                   (MenuRunParaIndex 				+ 7u)
#define MenuAamSaveIrlIndex				                   (MenuNmlSaveIrlIndex 			+ 7u)
//                                                   
#define MenuNwkParaIndex				                     (MenuAamSaveIrlIndex 			+ 7u)
////
#define MenuUldNwkAddIndex				                   (MenuNwkParaIndex 				+ 7u)
#define MenuUdeNwkAddIndex				                   (MenuUldNwkAddIndex 			+ 7u)
//
#define MenuStmSetIndex					                     (MenuUdeNwkAddIndex 			+ 7u)
////                                                 
#define MenuStmLgeIndex					                     (MenuStmSetIndex 				+ 7u)
#define MenuStmCokIndex					                     (MenuStmLgeIndex 				+ 7u)
#define MenuStmVsnIndex					                     (MenuStmCokIndex 				+ 7u)
#define MenuItuIDIndex					                     (MenuStmVsnIndex 				+ 7u)
#define MenuCerHtcDataIndex				                   (MenuItuIDIndex 				+ 7u)
#define MenuRteFtySetIndex				                   (MenuCerHtcDataIndex 			+ 7u)
#define MenuGsmDebugIndex				                     (MenuRteFtySetIndex 			+ 7u)
#define MenuFweUreIndex					                     (MenuGsmDebugIndex 				+ 7u)

#if (Print_Function_Enable == 0x01)
////
#define MenuPitSetIndex					                     (MenuFweUreIndex 			+ 7u)
//
#define MenuPitSatTimeSetIndex			                 (MenuPitSetIndex 				+ 7u)
#define MenuPitEndTimeSetIndex			                 (MenuPitSatTimeSetIndex 		+ 7u)
#define MenuPitTimeIrlSetIndex			                 (MenuPitEndTimeSetIndex 		+ 7u)
#define MenuPitDataIndex				                     (MenuPitTimeIrlSetIndex 		+ 7u)
////
#define MenuUserPermSetIndex			                   (MenuPitDataIndex 				+ 7u)
#else
////
#define MenuUserPermSetIndex			                   (MenuFweUreIndex 				+ 7u)
#endif

/* 网关菜单界面 */
#if (Gateway_NJSY_Enable == 0x01)
#define MenuGWIndex						                       (MenuUserPermSetIndex 			+ 7u)
#define MenuGWWorkStateIndex			                   (MenuGWIndex 					    + 7u)
#define MenuGWAdderIndex				                     (MenuGWWorkStateIndex 			+ 7u)
#define MenuGWChannelIndex				                   (MenuGWAdderIndex 				  + 7u)
#define MenuGWActiveTimeIndex			                   (MenuGWChannelIndex 			  + 7u)
#define MenuGWModeIndex					                     (MenuGWActiveTimeIndex 		+ 7u)
#elif (Gateway_HZZH_Enable == 0x01)
#define MenuGWHZZHIndex					                     (MenuUserPermSetIndex 			+ 7u)
#define MenuGWHZZHWorkStateIndex		                 (MenuGWHZZHIndex 				  + 7u)
#define MenuGWHZZHWkupTimeIndex			                 (MenuGWHZZHWorkStateIndex 	+ 7u)
#endif



/* Wifi菜单界面 */
#if   ((Gateway_NJSY_Enable == 0x01)&&(GSM_Module_Type == 0x02))
#define MenuWifiIndex						                       (MenuGWModeIndex 			 + 7u)
#elif ((Gateway_HZZH_Enable == 0x01)&&(GSM_Module_Type == 0x02))
#define MenuWifiIndex						                       (MenuGWHZZHWkupTimeIndex+ 7u)
#else  
#define MenuWifiIndex						                       (MenuUserPermSetIndex 			+ 7u)
#endif





/* Menu */
void InitMenuVal(void);
void Parameter_Setting_Successful(void); 
void Parameter_Setting_Failure(void); 
void MenuInputPara(void);
void MenuInputParaWinLeft(u16 Index);
void MenuInputParaWinRight(u16 Index);
void BarMenuInit(void);
void BarMenuLeft(void);
void BarMenuRight(void);
void PUSH(void);
void POP(void);
/* MenuFSM */
void Menu_Display(u8 key_state);
/* MenuDesktop */
void Menu_DeskTop_Switch_Disp	(u32 time);
void Menu_Idle_State_Check		(u32 time);
void Menu_DeskTop_Switch_Reset	(u32 time);
void Menu_Idle_State_Reset		(u32 time);
	
#endif


