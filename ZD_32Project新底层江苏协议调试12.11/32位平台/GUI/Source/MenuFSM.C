/************************************************************************************************/
/** @file              : MenuFSM.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月15日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->FSM界面														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 

#include "KS0108.H"
#include "Menu.H"

#include "Log_Print.h"

#include "Dev_oled.h"

#include "Mcu_button.h"
#include "Mcu_Beep.h"

#include "Gsm_Api.h"


//#include "Mcu_Usart.h"

//FIXME!
//改进想法:如何在菜单中可以接管其它有效按键,这里程序仅仅接管了方向按键,若能根据1--9这些数字按键跳转也是不错
//额外的问题是如何在应用程序中得到按键而却不需要在应用程序中做键盘检查循环(抢占CPU时间了,会使得原有可以在背景运行的程序无法执行)
/*
*   A>改进FSM编号的机制,使得维护更简单,能否自动编号?这样会使得添加FSM表容易
*   B>
*
*
*/
//-----------------------------------
//MenuTop.C
void DispMenuDeskTop					(void);
void DispMenuDeskTopUp					(void);
void DispMenuDeskTopDown				(void);
void DispMenuDeskTopLeft				(void);
void DispMenuDeskTopRight				(void);
void DispMenuDeskTopOK					(void);
void DispMenuDeskTopCancel			(void);
//{
//...首界面
//}   
//IcoMenu
void DispMenuIcoInit					(void);
void DispMenuIcoUp						(void);
void DispMenuIcoDown					(void);
void DispMenuIcoLeft					(void);
void DispMenuIcoRight					(void);
void DispMenuIcoOK						(void);
void DispMenuIcoCancel					(void);
//StartRecord
void DispMenuStartRecordInit			(void);
void DispMenuStartRecordUp				(void);
void DispMenuStartRecordDown			(void);
void DispMenuStartRecordLeft			(void);
void DispMenuStartRecordRight			(void);
void DispMenuStartRecordOK				(void);
void DispMenuStartRecordCancel			(void);
//StopRecord
void DispMenuStopRecordInit				(void);
void DispMenuStopRecordUp				(void);
void DispMenuStopRecordDown				(void);
void DispMenuStopRecordLeft				(void);
void DispMenuStopRecordRight			(void);
void DispMenuStopRecordOK				(void);
void DispMenuStopRecordCancel			(void);

//{
//...1级菜单
//}   
//报警设置
void DispMenuAlarmSetInit				(void);
void DispMenuAlarmSetUp				(void);
void DispMenuAlarmSetDown				(void);
void DispMenuAlarmSetLeft				(void);
void DispMenuAlarmSetRight			(void);
void DispMenuAlarmSetOK				(void);
void DispMenuAlarmSetCancel			(void);
//{//设置界面
//通道阈值
void DispMenuChaTsdInit				(void);
void DispMenuChaTsdUp					(void);
void DispMenuChaTsdDown				(void);
void DispMenuChaTsdLeft				(void);
void DispMenuChaTsdRight				(void);
void DispMenuChaTsdOK					(void);
void DispMenuChaTsdCancel				(void);
//传感器上下限选择
void DispMenuSenUpLowlitSecInit	(void);
void DispMenuSenUpLowlitSecUp		(void);
void DispMenuSenUpLowlitSecDown	(void);
void DispMenuSenUpLowlitSecLeft	(void);
void DispMenuSenUpLowlitSecRight	(void);
void DispMenuSenUpLowlitSecOK		(void);
void DispMenuSenUpLowlitSecCancel(void);
//传感器上限设置
void DispMenuSenUpLitSetInit		(void);
void DispMenuSenUpLitSetUp			(void);
void DispMenuSenUpLitSetDown		(void);
void DispMenuSenUpLitSetLeft		(void);
void DispMenuSenUpLitSetRight		(void);
void DispMenuSenUpLitSetOK			(void);
void DispMenuSenUpLitSetCancel		(void);
//传感器下限设置
void DispMenuSenLowLitSetInit		(void);
void DispMenuSenLowLitSetUp			(void);
void DispMenuSenLowLitSetDown		(void);
void DispMenuSenLowLitSetLeft		(void);
void DispMenuSenLowLitSetRight		(void);
void DispMenuSenLowLitSetOK			(void);
void DispMenuSenLowLitSetCancel	(void);
//声光报警
void DispMenuSudLgtAamInit			(void);
void DispMenuSudLgtAamUp				(void);
void DispMenuSudLgtAamDown			(void);
void DispMenuSudLgtAamLeft			(void);
void DispMenuSudLgtAamRight			(void);
void DispMenuSudLgtAamOK				(void);
void DispMenuSudLgtAamCancel		(void);
//声光报警总开关
void DispMenuSudLgtAamMtrSthSetInit		(void);
void DispMenuSudLgtAamMtrSthSetUp		(void);
void DispMenuSudLgtAamMtrSthSetDown		(void);
void DispMenuSudLgtAamMtrSthSetLeft		(void);
void DispMenuSudLgtAamMtrSthSetRight	(void);
void DispMenuSudLgtAamMtrSthSetOK		(void);
void DispMenuSudLgtAamMtrSthSetCancel	(void);
//低电量声光报警设置
void DispMenuLowPwrSudLgtAamSetInit		(void);
void DispMenuLowPwrSudLgtAamSetUp		(void);
void DispMenuLowPwrSudLgtAamSetDown		(void);
void DispMenuLowPwrSudLgtAamSetLeft		(void);
void DispMenuLowPwrSudLgtAamSetRight	(void);
void DispMenuLowPwrSudLgtAamSetOK		(void);
void DispMenuLowPwrSudLgtAamSetCancel	(void);
//掉电声光报警设置
void DispMenuCutPwrSudLgtAamSetInit		(void);
void DispMenuCutPwrSudLgtAamSetUp		(void);
void DispMenuCutPwrSudLgtAamSetDown		(void);
void DispMenuCutPwrSudLgtAamSetLeft		(void);
void DispMenuCutPwrSudLgtAamSetRight	(void);
void DispMenuCutPwrSudLgtAamSetOK		(void);
void DispMenuCutPwrSudLgtAamSetCancel	(void);
//选择声光报警通道
void DispMenuSudLgtAamChaSecInit			(void);
void DispMenuSudLgtAamChaSecUp				(void);
void DispMenuSudLgtAamChaSecDown			(void);
void DispMenuSudLgtAamChaSecLeft			(void);
void DispMenuSudLgtAamChaSecRight		(void);
void DispMenuSudLgtAamChaSecOK				(void);
void DispMenuSudLgtAamChaSecCancel		(void);
//声光报警通道设置(开/关)
void DispMenuSudLgtAamSetInit				(void);
void DispMenuSudLgtAamSetUp					(void);
void DispMenuSudLgtAamSetDown				(void);
void DispMenuSudLgtAamSetLeft				(void);
void DispMenuSudLgtAamSetRight				(void);
void DispMenuSudLgtAamSetOK					(void);
void DispMenuSudLgtAamSetCancel			(void);
//短信报警
void DispMenuSMSAamInit						(void);
void DispMenuSMSAamUp							(void);
void DispMenuSMSAamDown						(void);
void DispMenuSMSAamLeft						(void);
void DispMenuSMSAamRight						(void);
void DispMenuSMSAamOK							(void);
void DispMenuSMSAamCancel						(void);
//短信报警总开
void DispMenuSMSAamMtrSthSetInit			(void);
void DispMenuSMSAamMtrSthSetUp				(void);
void DispMenuSMSAamMtrSthSetDown			(void);
void DispMenuSMSAamMtrSthSetLeft			(void);
void DispMenuSMSAamMtrSthSetRight		(void);
void DispMenuSMSAamMtrSthSetOK				(void);
void DispMenuSMSAamMtrSthSetCancel		(void);
//低电量短信报警
void DispMenuLowPwrSMSAamSetInit			(void);
void DispMenuLowPwrSMSAamSetUp				(void);
void DispMenuLowPwrSMSAamSetDown			(void);
void DispMenuLowPwrSMSAamSetLeft			(void);
void DispMenuLowPwrSMSAamSetRight		(void);
void DispMenuLowPwrSMSAamSetOK				(void);
void DispMenuLowPwrSMSAamSetCancel		(void);
//断电短信报警
void DispMenuCutPwrSMSAamSetInit			(void);
void DispMenuCutPwrSMSAamSetUp				(void);
void DispMenuCutPwrSMSAamSetDown			(void);
void DispMenuCutPwrSMSAamSetLeft			(void);
void DispMenuCutPwrSMSAamSetRight		(void);
void DispMenuCutPwrSMSAamSetOK				(void);
void DispMenuCutPwrSMSAamSetCancel		(void);
//短信报警通道选择
void DispMenuSMSAamChaSecInit				(void);
void DispMenuSMSAamChaSecUp					(void);
void DispMenuSMSAamChaSecDown				(void);
void DispMenuSMSAamChaSecLeft				(void);
void DispMenuSMSAamChaSecRight				(void);
void DispMenuSMSAamChaSecOK					(void);
void DispMenuSMSAamChaSecCancel			(void);
//短信报警设置
void DispMenuSMSAamSetInit					(void);
void DispMenuSMSAamSetUp						(void);
void DispMenuSMSAamSetDown					(void);
void DispMenuSMSAamSetLeft					(void);
void DispMenuSMSAamSetRight					(void);
void DispMenuSMSAamSetOK						(void);
void DispMenuSMSAamSetCancel				(void);
//短信报警号码
void DispMenuSMSAamPoeNOInit				(void);
void DispMenuSMSAamPoeNOUp					(void);
void DispMenuSMSAamPoeNODown				(void);
void DispMenuSMSAamPoeNOLeft				(void);
void DispMenuSMSAamPoeNORight				(void);
void DispMenuSMSAamPoeNOOK					(void);
void DispMenuSMSAamPoeNOCancel				(void);
//短信报警号码设置
void DispMenuSMSAamPoeNOSetInit			(void);
void DispMenuSMSAamPoeNOSetUp				(void);
void DispMenuSMSAamPoeNOSetDown			(void);
void DispMenuSMSAamPoeNOSetLeft			(void);
void DispMenuSMSAamPoeNOSetRight			(void);
void DispMenuSMSAamPoeNOSetOK				(void);
void DispMenuSMSAamPoeNOSetCancel		(void);
//短信重复报警间隔
void DispMenuSMSAamRptIrlInit				(void);
void DispMenuSMSAamRptIrlUp					(void);
void DispMenuSMSAamRptIrlDown				(void);
void DispMenuSMSAamRptIrlLeft				(void);
void DispMenuSMSAamRptIrlRight				(void);
void DispMenuSMSAamRptIrlOK					(void);
void DispMenuSMSAamRptIrlCancel			(void);
//报警延时时间
void DispMenuAamDlyInit						(void);
void DispMenuAamDlyUp							(void);
void DispMenuAamDlyDown						(void);
void DispMenuAamDlyLeft						(void);
void DispMenuAamDlyRight						(void);
void DispMenuAamDlyOK							(void);
void DispMenuAamDlyCancel						(void);
//运行参数
void DispMenuRunParaInit						(void);
void DispMenuRunParaUp							(void);
void DispMenuRunParaDown						(void);
void DispMenuRunParaLeft						(void);
void DispMenuRunParaRight						(void);
void DispMenuRunParaOK							(void);
void DispMenuRunParaCancel					(void);
//正常状态数据存储间隔
void DispMenuNmlSaveIrlInit					(void);
void DispMenuNmlSaveIrlUp						(void);
void DispMenuNmlSaveIrlDown					(void);
void DispMenuNmlSaveIrlLeft					(void);
void DispMenuNmlSaveIrlRight				(void);
void DispMenuNmlSaveIrlOK						(void);
void DispMenuNmlSaveIrlCancel				(void);
//报警状态数据存储间隔
void DispMenuAamSaveIrlInit					(void);
void DispMenuAamSaveIrlUp						(void);
void DispMenuAamSaveIrlDown					(void);
void DispMenuAamSaveIrlLeft					(void);
void DispMenuAamSaveIrlRight				(void);
void DispMenuAamSaveIrlOK						(void);
void DispMenuAamSaveIrlCancel				(void);
//网络参数
void DispMenuNwkParaInit						(void);
void DispMenuNwkParaUp							(void);
void DispMenuNwkParaDown						(void);
void DispMenuNwkParaLeft						(void);
void DispMenuNwkParaRight						(void);
void DispMenuNwkParaOK							(void);
void DispMenuNwkParaCancel					(void);
//上传数据网络地址
void DispMenuUldNwkAddInit					(void);
void DispMenuUldNwkAddUp						(void);
void DispMenuUldNwkAddDown					(void);
void DispMenuUldNwkAddLeft					(void);
void DispMenuUldNwkAddRight					(void);
void DispMenuUldNwkAddOK						(void);
void DispMenuUldNwkAddCancel				(void);
//更新程序网络地址
void DispMenuUreNwkAddInit					(void){};
void DispMenuUreNwkAddUp						(void){};
void DispMenuUreNwkAddDown					(void){};
void DispMenuUreNwkAddLeft					(void){};
void DispMenuUreNwkAddRight					(void){};
void DispMenuUreNwkAddOK						(void){};
void DispMenuUreNwkAddCancel				(void){};
//系统设置
void DispMenuStmSetInit						(void);
void DispMenuStmSetUp							(void);
void DispMenuStmSetDown						(void);
void DispMenuStmSetLeft						(void);
void DispMenuStmSetRight						(void);
void DispMenuStmSetOK							(void);
void DispMenuStmSetCancel						(void);
//系统语言选择
 void DispMenuStmLgeInit					 	(void);
 void DispMenuStmLgeUp							(void);
 void DispMenuStmLgeDown					 	(void);
 void DispMenuStmLgeLeft					 	(void);
 void DispMenuStmLgeRight						(void);
 void DispMenuStmLgeOK							(void);
 void DispMenuStmLgeCancel					(void);
 //系统时钟
  void DispMenuStmCokInit						(void);
  void DispMenuStmCokUp 						(void);
  void DispMenuStmCokDown						(void);
  void DispMenuStmCokLeft						(void);
  void DispMenuStmCokRight					(void);
  void DispMenuStmCokOK 						(void);
  void DispMenuStmCokCancel 				 	(void);
//系统版本
 void DispMenuStmVsnInit					  	(void);
 void DispMenuStmVsnUp						  	(void);
 void DispMenuStmVsnDown					  	(void);
 void DispMenuStmVsnLeft					  	(void);
 void DispMenuStmVsnRight				   	(void);
 void DispMenuStmVsnOK						  	(void);
 void DispMenuStmVsnCancel					(void);
//仪器编号
 void DispMenuItuIDInit					  	(void);
 void DispMenuItuIDUp						  	(void);
 void DispMenuItuIDDown					  	(void);
 void DispMenuItuIDLeft					  	(void);
 void DispMenuItuIDRight				   	(void);
 void DispMenuItuIDOK						  	(void);
 void DispMenuItuIDCancel						(void);
//恢复出厂设置
 void DispMenuRteFtySetInit					(void);
 void DispMenuRteFtySetUp						(void);
 void DispMenuRteFtySetDown					(void);
 void DispMenuRteFtySetLeft					(void);
 void DispMenuRteFtySetRight				(void);
 void DispMenuRteFtySetOK						(void);
 void DispMenuRteFtySetCancel				(void);
//清除历史数据
 void DispMenuCerHtcDataInit				(void);
 void DispMenuCerHtcDataUp					(void);
 void DispMenuCerHtcDataDown				(void);
 void DispMenuCerHtcDataLeft				(void);
 void DispMenuCerHtcDataRight				(void);
 void DispMenuCerHtcDataOK					(void);
 void DispMenuCerHtcDataCancel				(void);
//打印GSM调试信息设置
void DispMenuGsmDebugInit					(void);
void DispMenuGsmDebugUp						(void);
void DispMenuGsmDebugDown					(void);
void DispMenuGsmDebugLeft					(void);
void DispMenuGsmDebugRight					(void);
void DispMenuGsmDebugOK						(void);
void DispMenuGsmDebugCancel					(void);
 
////固件升级
//void DispMenuFweUreInit 			   		(void){};
//void DispMenuFweUreUp				   		(void){};
//void DispMenuFweUreDown 			   		(void){};
//void DispMenuFweUreLeft 			   		(void){};
//void DispMenuFweUreRight			   		(void){};
//void DispMenuFweUreOK				   		(void){};
//void DispMenuFweUreCancel			   		(void){};
	

//固件升级
void DispMenuFweUreInit 			   		(void);
void DispMenuFweUreUp				   		  (void);
void DispMenuFweUreDown 			   		(void);
void DispMenuFweUreLeft 			   		(void);
void DispMenuFweUreRight			   		(void);
void DispMenuFweUreOK				   		  (void);
void DispMenuFweUreCancel			   		(void);



	
	
	
	
	
	
	

#if (Print_Function_Enable == 0x01)
//打印设置
 void DispMenuPitSetInit						(void);
 void DispMenuPitSetUp							(void);
 void DispMenuPitSetDown						(void);
 void DispMenuPitSetLeft						(void);
 void DispMenuPitSetRight						(void);
 void DispMenuPitSetOK							(void);
 void DispMenuPitSetCancel					(void);
//打印起始时间
void DispMenuPitSatTimeSetInit 			(void);
void DispMenuPitSatTimeSetUp				(void);
void DispMenuPitSatTimeSetDown 			(void);
void DispMenuPitSatTimeSetLeft 			(void);
void DispMenuPitSatTimeSetRight			(void);
void DispMenuPitSatTimeSetOK				(void);
void DispMenuPitSatTimeSetCancel			(void);
//打印结束时间
void DispMenuPitEndTimeSetInit 			(void);
void DispMenuPitEndTimeSetUp				(void);
void DispMenuPitEndTimeSetDown 			(void);
void DispMenuPitEndTimeSetLeft 			(void);
void DispMenuPitEndTimeSetRight			(void);
void DispMenuPitEndTimeSetOK				(void);
void DispMenuPitEndTimeSetCancel			(void);
//打印时间间隔
void DispMenuPitTimeIrlSetInit 			(void);
void DispMenuPitTimeIrlSetUp				(void);
void DispMenuPitTimeIrlSetDown 			(void);
void DispMenuPitTimeIrlSetLeft 			(void);
void DispMenuPitTimeIrlSetRight			(void);
void DispMenuPitTimeIrlSetOK				(void);
void DispMenuPitTimeIrlSetCancel			(void);
//打印数据
void DispMenuPitDataInit			 			(void);
void DispMenuPitDataUp							(void);
void DispMenuPitDataDown 						(void);
void DispMenuPitDataLeft 						(void);
void DispMenuPitDataRight						(void);
void DispMenuPitDataOK							(void);
void DispMenuPitDataCancel					(void);

#endif

/* 网关菜单界面 */
#if (Gateway_NJSY_Enable == 0x01)

/* 网关设置 */
void DispMenuGWInit							   (void);
void DispMenuGWUp							     (void);
void DispMenuGWDown							   (void);
void DispMenuGWLeft							   (void);
void DispMenuGWRight						   (void);
void DispMenuGWOK							     (void);
void DispMenuGWCancel						   (void);

/* 网关工作状态 */
void DispMenuGWWorkStateInit				(void);
void DispMenuGWWorkStateUp					(void);
void DispMenuGWWorkStateDown				(void);
void DispMenuGWWorkStateLeft				(void);
void DispMenuGWWorkStateRight				(void);
void DispMenuGWWorkStateOK					(void);
void DispMenuGWWorkStateCancel		  (void);

/* 网关地址 */
void DispMenuGWAdderInit					(void);
void DispMenuGWAdderUp						(void);
void DispMenuGWAdderDown					(void);
void DispMenuGWAdderLeft					(void);
void DispMenuGWAdderRight					(void);
void DispMenuGWAdderOK						(void);
void DispMenuGWAdderCancel					(void);

/* 网关信道 */
void DispMenuGWChannelInit					(void);
void DispMenuGWChannelUp					(void);
void DispMenuGWChannelDown					(void);
void DispMenuGWChannelLeft					(void);
void DispMenuGWChannelRight					(void);
void DispMenuGWChannelOK					(void);
void DispMenuGWChannelCancel				(void);

/* 网关数据有效时间 */
void DispMenuGWActiveTimeInit				(void);
void DispMenuGWActiveTimeUp					(void);
void DispMenuGWActiveTimeDown				(void);
void DispMenuGWActiveTimeLeft				(void);
void DispMenuGWActiveTimeRight				(void);
void DispMenuGWActiveTimeOK					(void);
void DispMenuGWActiveTimeCancel				(void);

/* 网关工作模式（正常/调试） */
void DispMenuGWModeInit						(void);
void DispMenuGWModeUp						(void);
void DispMenuGWModeDown						(void);
void DispMenuGWModeLeft						(void);
void DispMenuGWModeRight					(void);
void DispMenuGWModeOK						(void);
void DispMenuGWModeCancel					(void);

#elif (Gateway_HZZH_Enable == 0x01)

/* 网关设置 */
void DispMenuGWHZZHInit						(void);
void DispMenuGWHZZHUp						  (void);
void DispMenuGWHZZHDown						(void);
void DispMenuGWHZZHLeft						(void);
void DispMenuGWHZZHRight					(void);
void DispMenuGWHZZHOK						  (void);
void DispMenuGWHZZHCancel					(void);

/* 网关工作状态 */
void DispMenuGWHZZHWorkStateInit			(void);
void DispMenuGWHZZHWorkStateUp				(void);
void DispMenuGWHZZHWorkStateDown			(void);
void DispMenuGWHZZHWorkStateLeft			(void);
void DispMenuGWHZZHWorkStateRight			(void);
void DispMenuGWHZZHWorkStateOK				(void);
void DispMenuGWHZZHWorkStateCancel		(void);

/* 网关唤醒间隔 */
void DispMenuGWHZZHWkupTimeInit				(void);
void DispMenuGWHZZHWkupTimeUp				  (void);
void DispMenuGWHZZHWkupTimeDown				(void);
void DispMenuGWHZZHWkupTimeLeft				(void);
void DispMenuGWHZZHWkupTimeRight			(void);
void DispMenuGWHZZHWkupTimeOK				  (void);
void DispMenuGWHZZHWkupTimeCancel			(void);

#endif

/* Wifi菜单界面 */
#if (GSM_Module_Type == 0x02)
/* Wifi参数 */
void DispMenuWifiInit							   (void);
void DispMenuWifiUp							     (void);
void DispMenuWifiDown							   (void);
void DispMenuWifiLeft							   (void);
void DispMenuWifiRight						   (void);
void DispMenuWifiOK							     (void);
void DispMenuWifiCancel						   (void);
#endif

//用户权限设置界面
void DispMenuUserPermSetInit				(void);
void DispMenuUserPermSetUp					(void);
void DispMenuUserPermSetDown				(void);
void DispMenuUserPermSetLeft				(void);
void DispMenuUserPermSetRight				(void);
void DispMenuUserPermSetOK					(void);
void DispMenuUserPermSetCancel			(void);




/* 当前菜单的函数指针 */
void (*KeyFuncPtr)();
/* 菜单索引数据结构 */
typedef struct{							   
        u16 KeyStateIndex; 	//当前状态索引号
        u16 KeyUpState; 	//按下"向上"键时转向的状态索引号
        u16 KeyDnState; 	//按下"向下"键时转向的状态索引号
        u16 KeyLState;  	//按下"向左"键时转向的状态索引号
        u16 KeyRState;  	//按下"向右"键时转向的状态索引号
        u16 KeyOKState;  	//长按下"向右"键时转向的状态索引号
        u16 KeyCancelState;	//长按下"向右"键时转向的状态索引号
        void (*CurrentOperate)(); //当前状态应该执行的功能操作
} KbdTabStruct;

/*
********************************************************************************
   FSM菜单实现机制说明:
A.一些约定:只有4个按键来做菜单周转,这四个按键Up/Down/Left/Down 对应功能是:确认/回退/向上/向下
B.每一层的bar型菜单需要大概5个状态(也就是5个函数)来实现,分别是MenuInit/MenuUp/MenuDown/MenuLeft/MenuRight
对应的功能解释:
MenuInit : 初始化菜单/重新初始化菜单,在进入一个子Bar菜单时,该函数把Bar显示指针根据语言指向子Bar的资源数组,
获得需要的参数:该子菜单多少个Bar项(ItemNum)/每个Bar项的字符长度(Size)/指向Bar显示内容的指针(DispItem).
而后,调用函数BarMenuInit,该函数初始化全部Bar显示--这就是初始化菜单.
若是是从子菜单返回时,那么在返回前,Pop函数被调用,各种菜单参数被还原,在进入本函数后,本函数根据得到参数显示Bar项
初始化菜单/重新初始化菜单的不同点在于:
初始化时      高亮显示的位置是在顶部,对应选中的Item也是在顶部
重新初始化时  由于用户通过按键向上/向下来选择Bar项,而后通过按键确认来进入子菜单,为了在退出子菜单时能得到跟进入
前一致的显示内容,因此需要备份一些相关数据:高亮的位置/选中的Item/当前FSM的Index号  在退出子菜单时Pop还原这些数据
本函数根据这些还原数据恢复显示.

MenuUp   : 根据Bar显示内容执行对应动作函数/或者周转到某层子菜单
MenuDown : 返回上层菜单 通常是简单调用Pop函数
MenuLeft : 移至上一个Bar项 通常是调用BarMenuLeft函数--该函数维护Bar的显示后FSM状态返回MenunInit,但由于没有打开Flash/Reflash项,因此没有执行MenuInit函数
MenuRight: 移至下一个Bar项 通常是调用BarMenuright函数--该函数维护Bar的显示后FSM状态返回MenunInit,但由于没有打开Flash/Reflash项,因此没有执行MenuInit函数
MenuOK   : 长按Up按键	   通常是在数据输入的时候做 确定 将选择的数据写入 寄存器。  		
MenuCancel:长按Down按键	   暂时还没使用，	
********************************************************************************
*/
const KbdTabStruct KeyTab[]=
{
   {MenuDeskTopIndex,
    MenuDeskTopIndex+1,
    MenuDeskTopIndex+2,
    MenuDeskTopIndex+3,
    MenuDeskTopIndex+4,
    MenuDeskTopIndex+5,
    MenuDeskTopIndex+6,			   (*DispMenuDeskTop)},
   {MenuDeskTopIndex+1,0,0,0,0,0,0,(*DispMenuDeskTopUp)},
   {MenuDeskTopIndex+2,0,0,0,0,0,0,(*DispMenuDeskTopDown)},
   {MenuDeskTopIndex+3,0,0,0,0,0,0,(*DispMenuDeskTopLeft)},
   {MenuDeskTopIndex+4,0,0,0,0,0,0,(*DispMenuDeskTopRight)},
   {MenuDeskTopIndex+5,0,0,0,0,0,0,(*DispMenuDeskTopOK)},
   {MenuDeskTopIndex+6,0,0,0,0,0,0,(*DispMenuDeskTopCancel)},

   {MenuIcoIndex,
    MenuIcoIndex+1,
    MenuIcoIndex+2,
    MenuIcoIndex+3,
    MenuIcoIndex+4,
    MenuIcoIndex+5,
    MenuIcoIndex+6,			   (*DispMenuIcoInit)},
   {MenuIcoIndex+1,0,0,0,0,0,0,(*DispMenuIcoUp)},
   {MenuIcoIndex+2,0,0,0,0,0,0,(*DispMenuIcoDown)},
   {MenuIcoIndex+3,0,0,0,0,0,0,(*DispMenuIcoLeft)},
   {MenuIcoIndex+4,0,0,0,0,0,0,(*DispMenuIcoRight)},
   {MenuIcoIndex+5,0,0,0,0,0,0,(*DispMenuIcoOK)},
   {MenuIcoIndex+6,0,0,0,0,0,0,(*DispMenuIcoCancel)},
   
   {MenuStartRecordIndex,
    MenuStartRecordIndex+1,
    MenuStartRecordIndex+2,
    MenuStartRecordIndex+3,
    MenuStartRecordIndex+4,
    MenuStartRecordIndex+5,
    MenuStartRecordIndex+6,			   (*DispMenuStartRecordInit)},
   {MenuStartRecordIndex+1,0,0,0,0,0,0,(*DispMenuStartRecordUp)},
   {MenuStartRecordIndex+2,0,0,0,0,0,0,(*DispMenuStartRecordDown)},
   {MenuStartRecordIndex+3,0,0,0,0,0,0,(*DispMenuStartRecordLeft)},
   {MenuStartRecordIndex+4,0,0,0,0,0,0,(*DispMenuStartRecordRight)},
   {MenuStartRecordIndex+5,0,0,0,0,0,0,(*DispMenuStartRecordOK)},
   {MenuStartRecordIndex+6,0,0,0,0,0,0,(*DispMenuStartRecordCancel)},
   
   {MenuStopRecordIndex,
    MenuStopRecordIndex+1,
    MenuStopRecordIndex+2,
    MenuStopRecordIndex+3,
    MenuStopRecordIndex+4,
    MenuStopRecordIndex+5,
    MenuStopRecordIndex+6,			  (*DispMenuStopRecordInit)},
   {MenuStopRecordIndex+1,0,0,0,0,0,0,(*DispMenuStopRecordUp)},
   {MenuStopRecordIndex+2,0,0,0,0,0,0,(*DispMenuStopRecordDown)},
   {MenuStopRecordIndex+3,0,0,0,0,0,0,(*DispMenuStopRecordLeft)},
   {MenuStopRecordIndex+4,0,0,0,0,0,0,(*DispMenuStopRecordRight)},
   {MenuStopRecordIndex+5,0,0,0,0,0,0,(*DispMenuStopRecordOK)},
   {MenuStopRecordIndex+6,0,0,0,0,0,0,(*DispMenuStopRecordCancel)},

   {MenuAlarmSetIndex,
    MenuAlarmSetIndex+1,
    MenuAlarmSetIndex+2,
    MenuAlarmSetIndex+3,
    MenuAlarmSetIndex+4,
    MenuAlarmSetIndex+5,
    MenuAlarmSetIndex+6,			(*DispMenuAlarmSetInit)},
   {MenuAlarmSetIndex+1,0,0,0,0,0,0,(*DispMenuAlarmSetUp)},
   {MenuAlarmSetIndex+2,0,0,0,0,0,0,(*DispMenuAlarmSetDown)},
   {MenuAlarmSetIndex+3,0,0,0,0,0,0,(*DispMenuAlarmSetLeft)},
   {MenuAlarmSetIndex+4,0,0,0,0,0,0,(*DispMenuAlarmSetRight)},
   {MenuAlarmSetIndex+5,0,0,0,0,0,0,(*DispMenuAlarmSetOK)},
   {MenuAlarmSetIndex+6,0,0,0,0,0,0,(*DispMenuAlarmSetCancel)},

   {MenuChaTsdIndex,
    MenuChaTsdIndex+1,
    MenuChaTsdIndex+2,
    MenuChaTsdIndex+3,
    MenuChaTsdIndex+4,
    MenuChaTsdIndex+5,
    MenuChaTsdIndex+6,			  (*DispMenuChaTsdInit)},
   {MenuChaTsdIndex+1,0,0,0,0,0,0,(*DispMenuChaTsdUp)},
   {MenuChaTsdIndex+2,0,0,0,0,0,0,(*DispMenuChaTsdDown)},
   {MenuChaTsdIndex+3,0,0,0,0,0,0,(*DispMenuChaTsdLeft)},
   {MenuChaTsdIndex+4,0,0,0,0,0,0,(*DispMenuChaTsdRight)},
   {MenuChaTsdIndex+5,0,0,0,0,0,0,(*DispMenuChaTsdOK)},
   {MenuChaTsdIndex+6,0,0,0,0,0,0,(*DispMenuChaTsdCancel)},

   {MenuSenUpLowlitSecIndex,
    MenuSenUpLowlitSecIndex+1,
    MenuSenUpLowlitSecIndex+2,
    MenuSenUpLowlitSecIndex+3,
    MenuSenUpLowlitSecIndex+4,
    MenuSenUpLowlitSecIndex+5,
    MenuSenUpLowlitSecIndex+6,			  (*DispMenuSenUpLowlitSecInit)},
   {MenuSenUpLowlitSecIndex+1,0,0,0,0,0,0,(*DispMenuSenUpLowlitSecUp)},
   {MenuSenUpLowlitSecIndex+2,0,0,0,0,0,0,(*DispMenuSenUpLowlitSecDown)},
   {MenuSenUpLowlitSecIndex+3,0,0,0,0,0,0,(*DispMenuSenUpLowlitSecLeft)},
   {MenuSenUpLowlitSecIndex+4,0,0,0,0,0,0,(*DispMenuSenUpLowlitSecRight)},
   {MenuSenUpLowlitSecIndex+5,0,0,0,0,0,0,(*DispMenuSenUpLowlitSecOK)},
   {MenuSenUpLowlitSecIndex+6,0,0,0,0,0,0,(*DispMenuSenUpLowlitSecCancel)},

   {MenuSenUpLitSetIndex,
    MenuSenUpLitSetIndex+1,
    MenuSenUpLitSetIndex+2,
    MenuSenUpLitSetIndex+3,
    MenuSenUpLitSetIndex+4,
    MenuSenUpLitSetIndex+5,
    MenuSenUpLitSetIndex+6,			   (*DispMenuSenUpLitSetInit)},
   {MenuSenUpLitSetIndex+1,0,0,0,0,0,0,(*DispMenuSenUpLitSetUp)},
   {MenuSenUpLitSetIndex+2,0,0,0,0,0,0,(*DispMenuSenUpLitSetDown)},
   {MenuSenUpLitSetIndex+3,0,0,0,0,0,0,(*DispMenuSenUpLitSetLeft)},
   {MenuSenUpLitSetIndex+4,0,0,0,0,0,0,(*DispMenuSenUpLitSetRight)},
   {MenuSenUpLitSetIndex+5,0,0,0,0,0,0,(*DispMenuSenUpLitSetOK)},
   {MenuSenUpLitSetIndex+6,0,0,0,0,0,0,(*DispMenuSenUpLitSetCancel)},

   {MenuSenLowLitSetIndex,
    MenuSenLowLitSetIndex+1,
    MenuSenLowLitSetIndex+2,
    MenuSenLowLitSetIndex+3,
    MenuSenLowLitSetIndex+4,
    MenuSenLowLitSetIndex+5,
    MenuSenLowLitSetIndex+6,			(*DispMenuSenLowLitSetInit)},
   {MenuSenLowLitSetIndex+1,0,0,0,0,0,0,(*DispMenuSenLowLitSetUp)},
   {MenuSenLowLitSetIndex+2,0,0,0,0,0,0,(*DispMenuSenLowLitSetDown)},
   {MenuSenLowLitSetIndex+3,0,0,0,0,0,0,(*DispMenuSenLowLitSetLeft)},
   {MenuSenLowLitSetIndex+4,0,0,0,0,0,0,(*DispMenuSenLowLitSetRight)},
   {MenuSenLowLitSetIndex+5,0,0,0,0,0,0,(*DispMenuSenLowLitSetOK)},
   {MenuSenLowLitSetIndex+6,0,0,0,0,0,0,(*DispMenuSenLowLitSetCancel)},

   {MenuSudLgtAamIndex,
    MenuSudLgtAamIndex+1,
    MenuSudLgtAamIndex+2,
    MenuSudLgtAamIndex+3,
    MenuSudLgtAamIndex+4,
    MenuSudLgtAamIndex+5,
    MenuSudLgtAamIndex+6,			 (*DispMenuSudLgtAamInit)},
   {MenuSudLgtAamIndex+1,0,0,0,0,0,0,(*DispMenuSudLgtAamUp)},
   {MenuSudLgtAamIndex+2,0,0,0,0,0,0,(*DispMenuSudLgtAamDown)},
   {MenuSudLgtAamIndex+3,0,0,0,0,0,0,(*DispMenuSudLgtAamLeft)},
   {MenuSudLgtAamIndex+4,0,0,0,0,0,0,(*DispMenuSudLgtAamRight)},
   {MenuSudLgtAamIndex+5,0,0,0,0,0,0,(*DispMenuSudLgtAamOK)},
   {MenuSudLgtAamIndex+6,0,0,0,0,0,0,(*DispMenuSudLgtAamCancel)},
   
   {MenuSudLgtAamMtrSthSetIndex,
	MenuSudLgtAamMtrSthSetIndex+1,
	MenuSudLgtAamMtrSthSetIndex+2,
	MenuSudLgtAamMtrSthSetIndex+3,
	MenuSudLgtAamMtrSthSetIndex+4,
	MenuSudLgtAamMtrSthSetIndex+5,
	MenuSudLgtAamMtrSthSetIndex+6,			  (*DispMenuSudLgtAamMtrSthSetInit)},
   {MenuSudLgtAamMtrSthSetIndex+1,0,0,0,0,0,0,(*DispMenuSudLgtAamMtrSthSetUp)},
   {MenuSudLgtAamMtrSthSetIndex+2,0,0,0,0,0,0,(*DispMenuSudLgtAamMtrSthSetDown)},
   {MenuSudLgtAamMtrSthSetIndex+3,0,0,0,0,0,0,(*DispMenuSudLgtAamMtrSthSetLeft)},
   {MenuSudLgtAamMtrSthSetIndex+4,0,0,0,0,0,0,(*DispMenuSudLgtAamMtrSthSetRight)},
   {MenuSudLgtAamMtrSthSetIndex+5,0,0,0,0,0,0,(*DispMenuSudLgtAamMtrSthSetOK)},
   {MenuSudLgtAamMtrSthSetIndex+6,0,0,0,0,0,0,(*DispMenuSudLgtAamMtrSthSetCancel)},

   {MenuLowPwrSudLgtAamSetIndex,
    MenuLowPwrSudLgtAamSetIndex+1,
    MenuLowPwrSudLgtAamSetIndex+2,
    MenuLowPwrSudLgtAamSetIndex+3,
    MenuLowPwrSudLgtAamSetIndex+4,
    MenuLowPwrSudLgtAamSetIndex+5,
    MenuLowPwrSudLgtAamSetIndex+6,			  (*DispMenuLowPwrSudLgtAamSetInit)},
   {MenuLowPwrSudLgtAamSetIndex+1,0,0,0,0,0,0,(*DispMenuLowPwrSudLgtAamSetUp)},
   {MenuLowPwrSudLgtAamSetIndex+2,0,0,0,0,0,0,(*DispMenuLowPwrSudLgtAamSetDown)},
   {MenuLowPwrSudLgtAamSetIndex+3,0,0,0,0,0,0,(*DispMenuLowPwrSudLgtAamSetLeft)},
   {MenuLowPwrSudLgtAamSetIndex+4,0,0,0,0,0,0,(*DispMenuLowPwrSudLgtAamSetRight)},
   {MenuLowPwrSudLgtAamSetIndex+5,0,0,0,0,0,0,(*DispMenuLowPwrSudLgtAamSetOK)},
   {MenuLowPwrSudLgtAamSetIndex+6,0,0,0,0,0,0,(*DispMenuLowPwrSudLgtAamSetCancel)},

   {MenuCutPwrSudLgtAamSetIndex,
    MenuCutPwrSudLgtAamSetIndex+1,
    MenuCutPwrSudLgtAamSetIndex+2,
    MenuCutPwrSudLgtAamSetIndex+3,
    MenuCutPwrSudLgtAamSetIndex+4,
    MenuCutPwrSudLgtAamSetIndex+5,
    MenuCutPwrSudLgtAamSetIndex+6,			  (*DispMenuCutPwrSudLgtAamSetInit)},
   {MenuCutPwrSudLgtAamSetIndex+1,0,0,0,0,0,0,(*DispMenuCutPwrSudLgtAamSetUp)},
   {MenuCutPwrSudLgtAamSetIndex+2,0,0,0,0,0,0,(*DispMenuCutPwrSudLgtAamSetDown)},
   {MenuCutPwrSudLgtAamSetIndex+3,0,0,0,0,0,0,(*DispMenuCutPwrSudLgtAamSetLeft)},
   {MenuCutPwrSudLgtAamSetIndex+4,0,0,0,0,0,0,(*DispMenuCutPwrSudLgtAamSetRight)},
   {MenuCutPwrSudLgtAamSetIndex+5,0,0,0,0,0,0,(*DispMenuCutPwrSudLgtAamSetOK)},
   {MenuCutPwrSudLgtAamSetIndex+6,0,0,0,0,0,0,(*DispMenuCutPwrSudLgtAamSetCancel)},

   {MenuSudLgtAamChaSecIndex,
    MenuSudLgtAamChaSecIndex+1,
    MenuSudLgtAamChaSecIndex+2,
    MenuSudLgtAamChaSecIndex+3,
    MenuSudLgtAamChaSecIndex+4,
    MenuSudLgtAamChaSecIndex+5,
    MenuSudLgtAamChaSecIndex+6,			   (*DispMenuSudLgtAamChaSecInit)},
   {MenuSudLgtAamChaSecIndex+1,0,0,0,0,0,0,(*DispMenuSudLgtAamChaSecUp)},
   {MenuSudLgtAamChaSecIndex+2,0,0,0,0,0,0,(*DispMenuSudLgtAamChaSecDown)},
   {MenuSudLgtAamChaSecIndex+3,0,0,0,0,0,0,(*DispMenuSudLgtAamChaSecLeft)},
   {MenuSudLgtAamChaSecIndex+4,0,0,0,0,0,0,(*DispMenuSudLgtAamChaSecRight)},
   {MenuSudLgtAamChaSecIndex+5,0,0,0,0,0,0,(*DispMenuSudLgtAamChaSecOK)},
   {MenuSudLgtAamChaSecIndex+6,0,0,0,0,0,0,(*DispMenuSudLgtAamChaSecCancel)},

   {MenuSudLgtAamSetIndex,
    MenuSudLgtAamSetIndex+1,
    MenuSudLgtAamSetIndex+2,
    MenuSudLgtAamSetIndex+3,
    MenuSudLgtAamSetIndex+4,
    MenuSudLgtAamSetIndex+5,
    MenuSudLgtAamSetIndex+6,			(*DispMenuSudLgtAamSetInit)},
   {MenuSudLgtAamSetIndex+1,0,0,0,0,0,0,(*DispMenuSudLgtAamSetUp)},
   {MenuSudLgtAamSetIndex+2,0,0,0,0,0,0,(*DispMenuSudLgtAamSetDown)},
   {MenuSudLgtAamSetIndex+3,0,0,0,0,0,0,(*DispMenuSudLgtAamSetLeft)},
   {MenuSudLgtAamSetIndex+4,0,0,0,0,0,0,(*DispMenuSudLgtAamSetRight)},
   {MenuSudLgtAamSetIndex+5,0,0,0,0,0,0,(*DispMenuSudLgtAamSetOK)},
   {MenuSudLgtAamSetIndex+6,0,0,0,0,0,0,(*DispMenuSudLgtAamSetCancel)},

   {MenuSMSAamIndex,
    MenuSMSAamIndex+1,
    MenuSMSAamIndex+2,
    MenuSMSAamIndex+3,
    MenuSMSAamIndex+4,
    MenuSMSAamIndex+5,
    MenuSMSAamIndex+6,			  (*DispMenuSMSAamInit)},
   {MenuSMSAamIndex+1,0,0,0,0,0,0,(*DispMenuSMSAamUp)},
   {MenuSMSAamIndex+2,0,0,0,0,0,0,(*DispMenuSMSAamDown)},
   {MenuSMSAamIndex+3,0,0,0,0,0,0,(*DispMenuSMSAamLeft)},
   {MenuSMSAamIndex+4,0,0,0,0,0,0,(*DispMenuSMSAamRight)},
   {MenuSMSAamIndex+5,0,0,0,0,0,0,(*DispMenuSMSAamOK)},
   {MenuSMSAamIndex+6,0,0,0,0,0,0,(*DispMenuSMSAamCancel)},

   {MenuSMSAamMtrSthSetIndex,
    MenuSMSAamMtrSthSetIndex+1,
    MenuSMSAamMtrSthSetIndex+2,
    MenuSMSAamMtrSthSetIndex+3,
    MenuSMSAamMtrSthSetIndex+4,
    MenuSMSAamMtrSthSetIndex+5,
    MenuSMSAamMtrSthSetIndex+6,			   (*DispMenuSMSAamMtrSthSetInit)},
   {MenuSMSAamMtrSthSetIndex+1,0,0,0,0,0,0,(*DispMenuSMSAamMtrSthSetUp)},
   {MenuSMSAamMtrSthSetIndex+2,0,0,0,0,0,0,(*DispMenuSMSAamMtrSthSetDown)},
   {MenuSMSAamMtrSthSetIndex+3,0,0,0,0,0,0,(*DispMenuSMSAamMtrSthSetLeft)},
   {MenuSMSAamMtrSthSetIndex+4,0,0,0,0,0,0,(*DispMenuSMSAamMtrSthSetRight)},
   {MenuSMSAamMtrSthSetIndex+5,0,0,0,0,0,0,(*DispMenuSMSAamMtrSthSetOK)},
   {MenuSMSAamMtrSthSetIndex+6,0,0,0,0,0,0,(*DispMenuSMSAamMtrSthSetCancel)},

   {MenuLowPwrSMSAamSetIndex,
    MenuLowPwrSMSAamSetIndex+1,
    MenuLowPwrSMSAamSetIndex+2,
    MenuLowPwrSMSAamSetIndex+3,
    MenuLowPwrSMSAamSetIndex+4,
    MenuLowPwrSMSAamSetIndex+5,
    MenuLowPwrSMSAamSetIndex+6,			   (*DispMenuLowPwrSMSAamSetInit)},
   {MenuLowPwrSMSAamSetIndex+1,0,0,0,0,0,0,(*DispMenuLowPwrSMSAamSetUp)},
   {MenuLowPwrSMSAamSetIndex+2,0,0,0,0,0,0,(*DispMenuLowPwrSMSAamSetDown)},
   {MenuLowPwrSMSAamSetIndex+3,0,0,0,0,0,0,(*DispMenuLowPwrSMSAamSetLeft)},
   {MenuLowPwrSMSAamSetIndex+4,0,0,0,0,0,0,(*DispMenuLowPwrSMSAamSetRight)},
   {MenuLowPwrSMSAamSetIndex+5,0,0,0,0,0,0,(*DispMenuLowPwrSMSAamSetOK)},
   {MenuLowPwrSMSAamSetIndex+6,0,0,0,0,0,0,(*DispMenuLowPwrSMSAamSetCancel)},

   {MenuCutPwrSMSAamSetIndex,
    MenuCutPwrSMSAamSetIndex+1,
    MenuCutPwrSMSAamSetIndex+2,
    MenuCutPwrSMSAamSetIndex+3,
    MenuCutPwrSMSAamSetIndex+4,
    MenuCutPwrSMSAamSetIndex+5,
    MenuCutPwrSMSAamSetIndex+6,			   (*DispMenuCutPwrSMSAamSetInit)},
   {MenuCutPwrSMSAamSetIndex+1,0,0,0,0,0,0,(*DispMenuCutPwrSMSAamSetUp)},
   {MenuCutPwrSMSAamSetIndex+2,0,0,0,0,0,0,(*DispMenuCutPwrSMSAamSetDown)},
   {MenuCutPwrSMSAamSetIndex+3,0,0,0,0,0,0,(*DispMenuCutPwrSMSAamSetLeft)},
   {MenuCutPwrSMSAamSetIndex+4,0,0,0,0,0,0,(*DispMenuCutPwrSMSAamSetRight)},
   {MenuCutPwrSMSAamSetIndex+5,0,0,0,0,0,0,(*DispMenuCutPwrSMSAamSetOK)},
   {MenuCutPwrSMSAamSetIndex+6,0,0,0,0,0,0,(*DispMenuCutPwrSMSAamSetCancel)},

   {MenuSMSAamChaSecIndex,
    MenuSMSAamChaSecIndex+1,
    MenuSMSAamChaSecIndex+2,
    MenuSMSAamChaSecIndex+3,
    MenuSMSAamChaSecIndex+4,
    MenuSMSAamChaSecIndex+5,
    MenuSMSAamChaSecIndex+6,			(*DispMenuSMSAamChaSecInit)},
   {MenuSMSAamChaSecIndex+1,0,0,0,0,0,0,(*DispMenuSMSAamChaSecUp)},
   {MenuSMSAamChaSecIndex+2,0,0,0,0,0,0,(*DispMenuSMSAamChaSecDown)},
   {MenuSMSAamChaSecIndex+3,0,0,0,0,0,0,(*DispMenuSMSAamChaSecLeft)},
   {MenuSMSAamChaSecIndex+4,0,0,0,0,0,0,(*DispMenuSMSAamChaSecRight)},
   {MenuSMSAamChaSecIndex+5,0,0,0,0,0,0,(*DispMenuSMSAamChaSecOK)},
   {MenuSMSAamChaSecIndex+6,0,0,0,0,0,0,(*DispMenuSMSAamChaSecCancel)},

   {MenuSMSAamSetIndex,
    MenuSMSAamSetIndex+1,
    MenuSMSAamSetIndex+2,
    MenuSMSAamSetIndex+3,
    MenuSMSAamSetIndex+4,
    MenuSMSAamSetIndex+5,
    MenuSMSAamSetIndex+6,			 (*DispMenuSMSAamSetInit)},
   {MenuSMSAamSetIndex+1,0,0,0,0,0,0,(*DispMenuSMSAamSetUp)},
   {MenuSMSAamSetIndex+2,0,0,0,0,0,0,(*DispMenuSMSAamSetDown)},
   {MenuSMSAamSetIndex+3,0,0,0,0,0,0,(*DispMenuSMSAamSetLeft)},
   {MenuSMSAamSetIndex+4,0,0,0,0,0,0,(*DispMenuSMSAamSetRight)},
   {MenuSMSAamSetIndex+5,0,0,0,0,0,0,(*DispMenuSMSAamSetOK)},
   {MenuSMSAamSetIndex+6,0,0,0,0,0,0,(*DispMenuSMSAamSetCancel)},

   {MenuSMSAamPoeNOIndex,
    MenuSMSAamPoeNOIndex+1,
    MenuSMSAamPoeNOIndex+2,
    MenuSMSAamPoeNOIndex+3,
    MenuSMSAamPoeNOIndex+4,
    MenuSMSAamPoeNOIndex+5,
    MenuSMSAamPoeNOIndex+6,			   (*DispMenuSMSAamPoeNOInit)},
   {MenuSMSAamPoeNOIndex+1,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOUp)},
   {MenuSMSAamPoeNOIndex+2,0,0,0,0,0,0,(*DispMenuSMSAamPoeNODown)},
   {MenuSMSAamPoeNOIndex+3,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOLeft)},
   {MenuSMSAamPoeNOIndex+4,0,0,0,0,0,0,(*DispMenuSMSAamPoeNORight)},
   {MenuSMSAamPoeNOIndex+5,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOOK)},
   {MenuSMSAamPoeNOIndex+6,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOCancel)},

   {MenuSMSAamPoeNOSetIndex,
    MenuSMSAamPoeNOSetIndex+1,
    MenuSMSAamPoeNOSetIndex+2,
    MenuSMSAamPoeNOSetIndex+3,
    MenuSMSAamPoeNOSetIndex+4,
    MenuSMSAamPoeNOSetIndex+5,
    MenuSMSAamPoeNOSetIndex+6,			  (*DispMenuSMSAamPoeNOSetInit)},
   {MenuSMSAamPoeNOSetIndex+1,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOSetUp)},
   {MenuSMSAamPoeNOSetIndex+2,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOSetDown)},
   {MenuSMSAamPoeNOSetIndex+3,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOSetLeft)},
   {MenuSMSAamPoeNOSetIndex+4,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOSetRight)},
   {MenuSMSAamPoeNOSetIndex+5,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOSetOK)},
   {MenuSMSAamPoeNOSetIndex+6,0,0,0,0,0,0,(*DispMenuSMSAamPoeNOSetCancel)},

   {MenuSMSAamRptIrlIndex,
    MenuSMSAamRptIrlIndex+1,
    MenuSMSAamRptIrlIndex+2,
    MenuSMSAamRptIrlIndex+3,
    MenuSMSAamRptIrlIndex+4,
    MenuSMSAamRptIrlIndex+5,
    MenuSMSAamRptIrlIndex+6,			(*DispMenuSMSAamRptIrlInit)},
   {MenuSMSAamRptIrlIndex+1,0,0,0,0,0,0,(*DispMenuSMSAamRptIrlUp)},
   {MenuSMSAamRptIrlIndex+2,0,0,0,0,0,0,(*DispMenuSMSAamRptIrlDown)},
   {MenuSMSAamRptIrlIndex+3,0,0,0,0,0,0,(*DispMenuSMSAamRptIrlLeft)},
   {MenuSMSAamRptIrlIndex+4,0,0,0,0,0,0,(*DispMenuSMSAamRptIrlRight)},
   {MenuSMSAamRptIrlIndex+5,0,0,0,0,0,0,(*DispMenuSMSAamRptIrlOK)},
   {MenuSMSAamRptIrlIndex+6,0,0,0,0,0,0,(*DispMenuSMSAamRptIrlCancel)},   
   
   {MenuAamDlyIndex,
    MenuAamDlyIndex+1,
    MenuAamDlyIndex+2,
    MenuAamDlyIndex+3,
    MenuAamDlyIndex+4,
    MenuAamDlyIndex+5,
    MenuAamDlyIndex+6,			  (*DispMenuAamDlyInit)},
   {MenuAamDlyIndex+1,0,0,0,0,0,0,(*DispMenuAamDlyUp)},
   {MenuAamDlyIndex+2,0,0,0,0,0,0,(*DispMenuAamDlyDown)},
   {MenuAamDlyIndex+3,0,0,0,0,0,0,(*DispMenuAamDlyLeft)},
   {MenuAamDlyIndex+4,0,0,0,0,0,0,(*DispMenuAamDlyRight)},
   {MenuAamDlyIndex+5,0,0,0,0,0,0,(*DispMenuAamDlyOK)},
   {MenuAamDlyIndex+6,0,0,0,0,0,0,(*DispMenuAamDlyCancel)},

   {MenuRunParaIndex,
    MenuRunParaIndex+1,
    MenuRunParaIndex+2,
    MenuRunParaIndex+3,
    MenuRunParaIndex+4,
    MenuRunParaIndex+5,
    MenuRunParaIndex+6,			   (*DispMenuRunParaInit)},
   {MenuRunParaIndex+1,0,0,0,0,0,0,(*DispMenuRunParaUp)},
   {MenuRunParaIndex+2,0,0,0,0,0,0,(*DispMenuRunParaDown)},
   {MenuRunParaIndex+3,0,0,0,0,0,0,(*DispMenuRunParaLeft)},
   {MenuRunParaIndex+4,0,0,0,0,0,0,(*DispMenuRunParaRight)},
   {MenuRunParaIndex+5,0,0,0,0,0,0,(*DispMenuRunParaOK)},
   {MenuRunParaIndex+6,0,0,0,0,0,0,(*DispMenuRunParaCancel)},

   {MenuNmlSaveIrlIndex,
    MenuNmlSaveIrlIndex+1,
    MenuNmlSaveIrlIndex+2,
    MenuNmlSaveIrlIndex+3,
    MenuNmlSaveIrlIndex+4,
    MenuNmlSaveIrlIndex+5,
    MenuNmlSaveIrlIndex+6,			  (*DispMenuNmlSaveIrlInit)},
   {MenuNmlSaveIrlIndex+1,0,0,0,0,0,0,(*DispMenuNmlSaveIrlUp)},
   {MenuNmlSaveIrlIndex+2,0,0,0,0,0,0,(*DispMenuNmlSaveIrlDown)},
   {MenuNmlSaveIrlIndex+3,0,0,0,0,0,0,(*DispMenuNmlSaveIrlLeft)},
   {MenuNmlSaveIrlIndex+4,0,0,0,0,0,0,(*DispMenuNmlSaveIrlRight)},
   {MenuNmlSaveIrlIndex+5,0,0,0,0,0,0,(*DispMenuNmlSaveIrlOK)},
   {MenuNmlSaveIrlIndex+6,0,0,0,0,0,0,(*DispMenuNmlSaveIrlCancel)},

   {MenuAamSaveIrlIndex,
    MenuAamSaveIrlIndex+1,
    MenuAamSaveIrlIndex+2,
    MenuAamSaveIrlIndex+3,
    MenuAamSaveIrlIndex+4,
    MenuAamSaveIrlIndex+5,
    MenuAamSaveIrlIndex+6,			  (*DispMenuAamSaveIrlInit)},
   {MenuAamSaveIrlIndex+1,0,0,0,0,0,0,(*DispMenuAamSaveIrlUp)},
   {MenuAamSaveIrlIndex+2,0,0,0,0,0,0,(*DispMenuAamSaveIrlDown)},
   {MenuAamSaveIrlIndex+3,0,0,0,0,0,0,(*DispMenuAamSaveIrlLeft)},
   {MenuAamSaveIrlIndex+4,0,0,0,0,0,0,(*DispMenuAamSaveIrlRight)},
   {MenuAamSaveIrlIndex+5,0,0,0,0,0,0,(*DispMenuAamSaveIrlOK)},
   {MenuAamSaveIrlIndex+6,0,0,0,0,0,0,(*DispMenuAamSaveIrlCancel)},

   {MenuNwkParaIndex,
    MenuNwkParaIndex+1,
    MenuNwkParaIndex+2,
    MenuNwkParaIndex+3,
    MenuNwkParaIndex+4,
    MenuNwkParaIndex+5,
    MenuNwkParaIndex+6,			   (*DispMenuNwkParaInit)},
   {MenuNwkParaIndex+1,0,0,0,0,0,0,(*DispMenuNwkParaUp)},
   {MenuNwkParaIndex+2,0,0,0,0,0,0,(*DispMenuNwkParaDown)},
   {MenuNwkParaIndex+3,0,0,0,0,0,0,(*DispMenuNwkParaLeft)},
   {MenuNwkParaIndex+4,0,0,0,0,0,0,(*DispMenuNwkParaRight)},
   {MenuNwkParaIndex+5,0,0,0,0,0,0,(*DispMenuNwkParaOK)},
   {MenuNwkParaIndex+6,0,0,0,0,0,0,(*DispMenuNwkParaCancel)},

   {MenuUldNwkAddIndex,
    MenuUldNwkAddIndex+1,
    MenuUldNwkAddIndex+2,
    MenuUldNwkAddIndex+3,
    MenuUldNwkAddIndex+4,
    MenuUldNwkAddIndex+5,
    MenuUldNwkAddIndex+6,			 (*DispMenuUldNwkAddInit)},
   {MenuUldNwkAddIndex+1,0,0,0,0,0,0,(*DispMenuUldNwkAddUp)},
   {MenuUldNwkAddIndex+2,0,0,0,0,0,0,(*DispMenuUldNwkAddDown)},
   {MenuUldNwkAddIndex+3,0,0,0,0,0,0,(*DispMenuUldNwkAddLeft)},
   {MenuUldNwkAddIndex+4,0,0,0,0,0,0,(*DispMenuUldNwkAddRight)},
   {MenuUldNwkAddIndex+5,0,0,0,0,0,0,(*DispMenuUldNwkAddOK)},
   {MenuUldNwkAddIndex+6,0,0,0,0,0,0,(*DispMenuUldNwkAddCancel)},

   {MenuUdeNwkAddIndex,
    MenuUdeNwkAddIndex+1,
    MenuUdeNwkAddIndex+2,
    MenuUdeNwkAddIndex+3,
    MenuUdeNwkAddIndex+4,
    MenuUdeNwkAddIndex+5,
    MenuUdeNwkAddIndex+6,			 (*DispMenuUreNwkAddInit)},
   {MenuUdeNwkAddIndex+1,0,0,0,0,0,0,(*DispMenuUreNwkAddUp)},
   {MenuUdeNwkAddIndex+2,0,0,0,0,0,0,(*DispMenuUreNwkAddDown)},
   {MenuUdeNwkAddIndex+3,0,0,0,0,0,0,(*DispMenuUreNwkAddLeft)},
   {MenuUdeNwkAddIndex+4,0,0,0,0,0,0,(*DispMenuUreNwkAddRight)},
   {MenuUdeNwkAddIndex+5,0,0,0,0,0,0,(*DispMenuUreNwkAddOK)},
   {MenuUdeNwkAddIndex+6,0,0,0,0,0,0,(*DispMenuUreNwkAddCancel)},

   {MenuStmSetIndex,
    MenuStmSetIndex+1,
    MenuStmSetIndex+2,
    MenuStmSetIndex+3,
    MenuStmSetIndex+4,
    MenuStmSetIndex+5,
    MenuStmSetIndex+6,			  (*DispMenuStmSetInit)},
   {MenuStmSetIndex+1,0,0,0,0,0,0,(*DispMenuStmSetUp)},
   {MenuStmSetIndex+2,0,0,0,0,0,0,(*DispMenuStmSetDown)},
   {MenuStmSetIndex+3,0,0,0,0,0,0,(*DispMenuStmSetLeft)},
   {MenuStmSetIndex+4,0,0,0,0,0,0,(*DispMenuStmSetRight)},
   {MenuStmSetIndex+5,0,0,0,0,0,0,(*DispMenuStmSetOK)},
   {MenuStmSetIndex+6,0,0,0,0,0,0,(*DispMenuStmSetCancel)},

   {MenuStmLgeIndex,
    MenuStmLgeIndex+1,
    MenuStmLgeIndex+2,
    MenuStmLgeIndex+3,
    MenuStmLgeIndex+4,
    MenuStmLgeIndex+5,
    MenuStmLgeIndex+6,			  (*DispMenuStmLgeInit)},
   {MenuStmLgeIndex+1,0,0,0,0,0,0,(*DispMenuStmLgeUp)},
   {MenuStmLgeIndex+2,0,0,0,0,0,0,(*DispMenuStmLgeDown)},
   {MenuStmLgeIndex+3,0,0,0,0,0,0,(*DispMenuStmLgeLeft)},
   {MenuStmLgeIndex+4,0,0,0,0,0,0,(*DispMenuStmLgeRight)},
   {MenuStmLgeIndex+5,0,0,0,0,0,0,(*DispMenuStmLgeOK)},
   {MenuStmLgeIndex+6,0,0,0,0,0,0,(*DispMenuStmLgeCancel)},

   {MenuStmCokIndex,
    MenuStmCokIndex+1,
    MenuStmCokIndex+2,
    MenuStmCokIndex+3,
    MenuStmCokIndex+4,
    MenuStmCokIndex+5,
    MenuStmCokIndex+6,			  (*DispMenuStmCokInit)},
   {MenuStmCokIndex+1,0,0,0,0,0,0,(*DispMenuStmCokUp)},
   {MenuStmCokIndex+2,0,0,0,0,0,0,(*DispMenuStmCokDown)},
   {MenuStmCokIndex+3,0,0,0,0,0,0,(*DispMenuStmCokLeft)},
   {MenuStmCokIndex+4,0,0,0,0,0,0,(*DispMenuStmCokRight)},
   {MenuStmCokIndex+5,0,0,0,0,0,0,(*DispMenuStmCokOK)},
   {MenuStmCokIndex+6,0,0,0,0,0,0,(*DispMenuStmCokCancel)},

   {MenuStmVsnIndex,
    MenuStmVsnIndex+1,
    MenuStmVsnIndex+2,
    MenuStmVsnIndex+3,
    MenuStmVsnIndex+4,
    MenuStmVsnIndex+5,
    MenuStmVsnIndex+6,			  (*DispMenuStmVsnInit)},
   {MenuStmVsnIndex+1,0,0,0,0,0,0,(*DispMenuStmVsnUp)},
   {MenuStmVsnIndex+2,0,0,0,0,0,0,(*DispMenuStmVsnDown)},
   {MenuStmVsnIndex+3,0,0,0,0,0,0,(*DispMenuStmVsnLeft)},
   {MenuStmVsnIndex+4,0,0,0,0,0,0,(*DispMenuStmVsnRight)},
   {MenuStmVsnIndex+5,0,0,0,0,0,0,(*DispMenuStmVsnOK)},
   {MenuStmVsnIndex+6,0,0,0,0,0,0,(*DispMenuStmVsnCancel)},

   {MenuItuIDIndex,
    MenuItuIDIndex+1,
    MenuItuIDIndex+2,
    MenuItuIDIndex+3,
    MenuItuIDIndex+4,
    MenuItuIDIndex+5,
    MenuItuIDIndex+6,			 (*DispMenuItuIDInit)},
   {MenuItuIDIndex+1,0,0,0,0,0,0,(*DispMenuItuIDUp)},
   {MenuItuIDIndex+2,0,0,0,0,0,0,(*DispMenuItuIDDown)},
   {MenuItuIDIndex+3,0,0,0,0,0,0,(*DispMenuItuIDLeft)},
   {MenuItuIDIndex+4,0,0,0,0,0,0,(*DispMenuItuIDRight)},
   {MenuItuIDIndex+5,0,0,0,0,0,0,(*DispMenuItuIDOK)},
   {MenuItuIDIndex+6,0,0,0,0,0,0,(*DispMenuItuIDCancel)},

   {MenuCerHtcDataIndex,
    MenuCerHtcDataIndex+1,
    MenuCerHtcDataIndex+2,
    MenuCerHtcDataIndex+3,
    MenuCerHtcDataIndex+4,
    MenuCerHtcDataIndex+5,
    MenuCerHtcDataIndex+6,			  (*DispMenuCerHtcDataInit)},
   {MenuCerHtcDataIndex+1,0,0,0,0,0,0,(*DispMenuCerHtcDataUp)},
   {MenuCerHtcDataIndex+2,0,0,0,0,0,0,(*DispMenuCerHtcDataDown)},
   {MenuCerHtcDataIndex+3,0,0,0,0,0,0,(*DispMenuCerHtcDataLeft)},
   {MenuCerHtcDataIndex+4,0,0,0,0,0,0,(*DispMenuCerHtcDataRight)},
   {MenuCerHtcDataIndex+5,0,0,0,0,0,0,(*DispMenuCerHtcDataOK)},
   {MenuCerHtcDataIndex+6,0,0,0,0,0,0,(*DispMenuCerHtcDataCancel)},
   
   {MenuRteFtySetIndex,
    MenuRteFtySetIndex+1,
    MenuRteFtySetIndex+2,
    MenuRteFtySetIndex+3,
    MenuRteFtySetIndex+4,
    MenuRteFtySetIndex+5,
    MenuRteFtySetIndex+6,			 (*DispMenuRteFtySetInit)},
   {MenuRteFtySetIndex+1,0,0,0,0,0,0,(*DispMenuRteFtySetUp)},
   {MenuRteFtySetIndex+2,0,0,0,0,0,0,(*DispMenuRteFtySetDown)},
   {MenuRteFtySetIndex+3,0,0,0,0,0,0,(*DispMenuRteFtySetLeft)},
   {MenuRteFtySetIndex+4,0,0,0,0,0,0,(*DispMenuRteFtySetRight)},
   {MenuRteFtySetIndex+5,0,0,0,0,0,0,(*DispMenuRteFtySetOK)},
   {MenuRteFtySetIndex+6,0,0,0,0,0,0,(*DispMenuRteFtySetCancel)},
   
   {MenuGsmDebugIndex,
    MenuGsmDebugIndex+1,
    MenuGsmDebugIndex+2,
    MenuGsmDebugIndex+3,
    MenuGsmDebugIndex+4,
    MenuGsmDebugIndex+5,
    MenuGsmDebugIndex+6,			(*DispMenuGsmDebugInit)},
   {MenuGsmDebugIndex+1,0,0,0,0,0,0,(*DispMenuGsmDebugUp)},
   {MenuGsmDebugIndex+2,0,0,0,0,0,0,(*DispMenuGsmDebugDown)},
   {MenuGsmDebugIndex+3,0,0,0,0,0,0,(*DispMenuGsmDebugLeft)},
   {MenuGsmDebugIndex+4,0,0,0,0,0,0,(*DispMenuGsmDebugRight)},
   {MenuGsmDebugIndex+5,0,0,0,0,0,0,(*DispMenuGsmDebugOK)},
   {MenuGsmDebugIndex+6,0,0,0,0,0,0,(*DispMenuGsmDebugCancel)},
   
   {MenuFweUreIndex,
    MenuFweUreIndex+1,
    MenuFweUreIndex+2,
    MenuFweUreIndex+3,
    MenuFweUreIndex+4,
    MenuFweUreIndex+5,
    MenuFweUreIndex+6,			  (*DispMenuFweUreInit)},
   {MenuFweUreIndex+1,0,0,0,0,0,0,(*DispMenuFweUreUp)},
   {MenuFweUreIndex+2,0,0,0,0,0,0,(*DispMenuFweUreDown)},
   {MenuFweUreIndex+3,0,0,0,0,0,0,(*DispMenuFweUreLeft)},
   {MenuFweUreIndex+4,0,0,0,0,0,0,(*DispMenuFweUreRight)},
   {MenuFweUreIndex+5,0,0,0,0,0,0,(*DispMenuFweUreOK)},
   {MenuFweUreIndex+6,0,0,0,0,0,0,(*DispMenuFweUreCancel)},

#if (Print_Function_Enable == 0x01)

   {MenuPitSetIndex,
    MenuPitSetIndex+1,
    MenuPitSetIndex+2,
    MenuPitSetIndex+3,
    MenuPitSetIndex+4,
    MenuPitSetIndex+5,
    MenuPitSetIndex+6,			  (*DispMenuPitSetInit)},
   {MenuPitSetIndex+1,0,0,0,0,0,0,(*DispMenuPitSetUp)},
   {MenuPitSetIndex+2,0,0,0,0,0,0,(*DispMenuPitSetDown)},
   {MenuPitSetIndex+3,0,0,0,0,0,0,(*DispMenuPitSetLeft)},
   {MenuPitSetIndex+4,0,0,0,0,0,0,(*DispMenuPitSetRight)},
   {MenuPitSetIndex+5,0,0,0,0,0,0,(*DispMenuPitSetOK)},
   {MenuPitSetIndex+6,0,0,0,0,0,0,(*DispMenuPitSetCancel)},

   {MenuPitSatTimeSetIndex,
    MenuPitSatTimeSetIndex+1,
    MenuPitSatTimeSetIndex+2,
    MenuPitSatTimeSetIndex+3,
    MenuPitSatTimeSetIndex+4,
    MenuPitSatTimeSetIndex+5,
    MenuPitSatTimeSetIndex+6,			 (*DispMenuPitSatTimeSetInit)},
   {MenuPitSatTimeSetIndex+1,0,0,0,0,0,0,(*DispMenuPitSatTimeSetUp)},
   {MenuPitSatTimeSetIndex+2,0,0,0,0,0,0,(*DispMenuPitSatTimeSetDown)},
   {MenuPitSatTimeSetIndex+3,0,0,0,0,0,0,(*DispMenuPitSatTimeSetLeft)},
   {MenuPitSatTimeSetIndex+4,0,0,0,0,0,0,(*DispMenuPitSatTimeSetRight)},
   {MenuPitSatTimeSetIndex+5,0,0,0,0,0,0,(*DispMenuPitSatTimeSetOK)},
   {MenuPitSatTimeSetIndex+6,0,0,0,0,0,0,(*DispMenuPitSatTimeSetCancel)},

   {MenuPitEndTimeSetIndex,
    MenuPitEndTimeSetIndex+1,
    MenuPitEndTimeSetIndex+2,
    MenuPitEndTimeSetIndex+3,
    MenuPitEndTimeSetIndex+4,
    MenuPitEndTimeSetIndex+5,
    MenuPitEndTimeSetIndex+6,			 (*DispMenuPitEndTimeSetInit)},
   {MenuPitEndTimeSetIndex+1,0,0,0,0,0,0,(*DispMenuPitEndTimeSetUp)},
   {MenuPitEndTimeSetIndex+2,0,0,0,0,0,0,(*DispMenuPitEndTimeSetDown)},
   {MenuPitEndTimeSetIndex+3,0,0,0,0,0,0,(*DispMenuPitEndTimeSetLeft)},
   {MenuPitEndTimeSetIndex+4,0,0,0,0,0,0,(*DispMenuPitEndTimeSetRight)},
   {MenuPitEndTimeSetIndex+5,0,0,0,0,0,0,(*DispMenuPitEndTimeSetOK)},
   {MenuPitEndTimeSetIndex+6,0,0,0,0,0,0,(*DispMenuPitEndTimeSetCancel)},

   {MenuPitTimeIrlSetIndex,
    MenuPitTimeIrlSetIndex+1,
    MenuPitTimeIrlSetIndex+2,
    MenuPitTimeIrlSetIndex+3,
    MenuPitTimeIrlSetIndex+4,
    MenuPitTimeIrlSetIndex+5,
    MenuPitTimeIrlSetIndex+6,			 (*DispMenuPitTimeIrlSetInit)},
   {MenuPitTimeIrlSetIndex+1,0,0,0,0,0,0,(*DispMenuPitTimeIrlSetUp)},
   {MenuPitTimeIrlSetIndex+2,0,0,0,0,0,0,(*DispMenuPitTimeIrlSetDown)},
   {MenuPitTimeIrlSetIndex+3,0,0,0,0,0,0,(*DispMenuPitTimeIrlSetLeft)},
   {MenuPitTimeIrlSetIndex+4,0,0,0,0,0,0,(*DispMenuPitTimeIrlSetRight)},
   {MenuPitTimeIrlSetIndex+5,0,0,0,0,0,0,(*DispMenuPitTimeIrlSetOK)},
   {MenuPitTimeIrlSetIndex+6,0,0,0,0,0,0,(*DispMenuPitTimeIrlSetCancel)}, 

   {MenuPitDataIndex,
    MenuPitDataIndex+1,
    MenuPitDataIndex+2,
    MenuPitDataIndex+3,
    MenuPitDataIndex+4,
    MenuPitDataIndex+5,
    MenuPitDataIndex+6,			   (*DispMenuPitDataInit)},
   {MenuPitDataIndex+1,0,0,0,0,0,0,(*DispMenuPitDataUp)},
   {MenuPitDataIndex+2,0,0,0,0,0,0,(*DispMenuPitDataDown)},
   {MenuPitDataIndex+3,0,0,0,0,0,0,(*DispMenuPitDataLeft)},
   {MenuPitDataIndex+4,0,0,0,0,0,0,(*DispMenuPitDataRight)},
   {MenuPitDataIndex+5,0,0,0,0,0,0,(*DispMenuPitDataOK)},
   {MenuPitDataIndex+6,0,0,0,0,0,0,(*DispMenuPitDataCancel)},    

#endif

   {MenuUserPermSetIndex,
    MenuUserPermSetIndex+1,
    MenuUserPermSetIndex+2,
    MenuUserPermSetIndex+3,
    MenuUserPermSetIndex+4,
    MenuUserPermSetIndex+5,
    MenuUserPermSetIndex+6,			   (*DispMenuUserPermSetInit)},
   {MenuUserPermSetIndex+1,0,0,0,0,0,0,(*DispMenuUserPermSetUp)},
   {MenuUserPermSetIndex+2,0,0,0,0,0,0,(*DispMenuUserPermSetDown)},
   {MenuUserPermSetIndex+3,0,0,0,0,0,0,(*DispMenuUserPermSetLeft)},
   {MenuUserPermSetIndex+4,0,0,0,0,0,0,(*DispMenuUserPermSetRight)},
   {MenuUserPermSetIndex+5,0,0,0,0,0,0,(*DispMenuUserPermSetOK)},
   {MenuUserPermSetIndex+6,0,0,0,0,0,0,(*DispMenuUserPermSetCancel)}, 



/* 网关菜单界面 */
#if (Gateway_NJSY_Enable == 0x01)

   /* 网关设置 */
   {MenuGWIndex,
    MenuGWIndex+1,
    MenuGWIndex+2,
    MenuGWIndex+3,
    MenuGWIndex+4,
    MenuGWIndex+5,
    MenuGWIndex+6,			  (*DispMenuGWInit)},
   {MenuGWIndex+1,0,0,0,0,0,0,(*DispMenuGWUp)},
   {MenuGWIndex+2,0,0,0,0,0,0,(*DispMenuGWDown)},
   {MenuGWIndex+3,0,0,0,0,0,0,(*DispMenuGWLeft)},
   {MenuGWIndex+4,0,0,0,0,0,0,(*DispMenuGWRight)},
   {MenuGWIndex+5,0,0,0,0,0,0,(*DispMenuGWOK)},
   {MenuGWIndex+6,0,0,0,0,0,0,(*DispMenuGWCancel)}, 
   
  
   /* 网关工作状态显示 */
   {MenuGWWorkStateIndex,
    MenuGWWorkStateIndex+1,
    MenuGWWorkStateIndex+2,
    MenuGWWorkStateIndex+3,
    MenuGWWorkStateIndex+4,
    MenuGWWorkStateIndex+5,
    MenuGWWorkStateIndex+6,			   (*DispMenuGWWorkStateInit)},
   {MenuGWWorkStateIndex+1,0,0,0,0,0,0,(*DispMenuGWWorkStateUp)},
   {MenuGWWorkStateIndex+2,0,0,0,0,0,0,(*DispMenuGWWorkStateDown)},
   {MenuGWWorkStateIndex+3,0,0,0,0,0,0,(*DispMenuGWWorkStateLeft)},
   {MenuGWWorkStateIndex+4,0,0,0,0,0,0,(*DispMenuGWWorkStateRight)},
   {MenuGWWorkStateIndex+5,0,0,0,0,0,0,(*DispMenuGWWorkStateOK)},
   {MenuGWWorkStateIndex+6,0,0,0,0,0,0,(*DispMenuGWWorkStateCancel)}, 
   
     /* 网关地址 */
   {MenuGWAdderIndex,
    MenuGWAdderIndex+1,
    MenuGWAdderIndex+2,
    MenuGWAdderIndex+3,
    MenuGWAdderIndex+4,
    MenuGWAdderIndex+5,
    MenuGWAdderIndex+6,			   (*DispMenuGWAdderInit)},
   {MenuGWAdderIndex+1,0,0,0,0,0,0,(*DispMenuGWAdderUp)},
   {MenuGWAdderIndex+2,0,0,0,0,0,0,(*DispMenuGWAdderDown)},
   {MenuGWAdderIndex+3,0,0,0,0,0,0,(*DispMenuGWAdderLeft)},
   {MenuGWAdderIndex+4,0,0,0,0,0,0,(*DispMenuGWAdderRight)},
   {MenuGWAdderIndex+5,0,0,0,0,0,0,(*DispMenuGWAdderOK)},
   {MenuGWAdderIndex+6,0,0,0,0,0,0,(*DispMenuGWAdderCancel)}, 
   
   /* 网关信道 */
   {MenuGWChannelIndex,
    MenuGWChannelIndex+1,
    MenuGWChannelIndex+2,
    MenuGWChannelIndex+3,
    MenuGWChannelIndex+4,
    MenuGWChannelIndex+5,
    MenuGWChannelIndex+6,			 (*DispMenuGWChannelInit)},
   {MenuGWChannelIndex+1,0,0,0,0,0,0,(*DispMenuGWChannelUp)},
   {MenuGWChannelIndex+2,0,0,0,0,0,0,(*DispMenuGWChannelDown)},
   {MenuGWChannelIndex+3,0,0,0,0,0,0,(*DispMenuGWChannelLeft)},
   {MenuGWChannelIndex+4,0,0,0,0,0,0,(*DispMenuGWChannelRight)},
   {MenuGWChannelIndex+5,0,0,0,0,0,0,(*DispMenuGWChannelOK)},
   {MenuGWChannelIndex+6,0,0,0,0,0,0,(*DispMenuGWChannelCancel)}, 
   
   /* 网关信道 */
   {MenuGWActiveTimeIndex,
    MenuGWActiveTimeIndex+1,
    MenuGWActiveTimeIndex+2,
    MenuGWActiveTimeIndex+3,
    MenuGWActiveTimeIndex+4,
    MenuGWActiveTimeIndex+5,
    MenuGWActiveTimeIndex+6,			(*DispMenuGWActiveTimeInit)},
   {MenuGWActiveTimeIndex+1,0,0,0,0,0,0,(*DispMenuGWActiveTimeUp)},
   {MenuGWActiveTimeIndex+2,0,0,0,0,0,0,(*DispMenuGWActiveTimeDown)},
   {MenuGWActiveTimeIndex+3,0,0,0,0,0,0,(*DispMenuGWActiveTimeLeft)},
   {MenuGWActiveTimeIndex+4,0,0,0,0,0,0,(*DispMenuGWActiveTimeRight)},
   {MenuGWActiveTimeIndex+5,0,0,0,0,0,0,(*DispMenuGWActiveTimeOK)},
   {MenuGWActiveTimeIndex+6,0,0,0,0,0,0,(*DispMenuGWActiveTimeCancel)}, 
   
   /* 网关信道 */
   {MenuGWModeIndex,
    MenuGWModeIndex+1,
    MenuGWModeIndex+2,
    MenuGWModeIndex+3,
    MenuGWModeIndex+4,
    MenuGWModeIndex+5,
    MenuGWModeIndex+6,			  (*DispMenuGWModeInit)},
   {MenuGWModeIndex+1,0,0,0,0,0,0,(*DispMenuGWModeUp)},
   {MenuGWModeIndex+2,0,0,0,0,0,0,(*DispMenuGWModeDown)},
   {MenuGWModeIndex+3,0,0,0,0,0,0,(*DispMenuGWModeLeft)},
   {MenuGWModeIndex+4,0,0,0,0,0,0,(*DispMenuGWModeRight)},
   {MenuGWModeIndex+5,0,0,0,0,0,0,(*DispMenuGWModeOK)},
   {MenuGWModeIndex+6,0,0,0,0,0,0,(*DispMenuGWModeCancel)}, 
   
#elif (Gateway_HZZH_Enable == 0x01)
   
   /* 网关设置 */
   {MenuGWHZZHIndex,
    MenuGWHZZHIndex+1,
    MenuGWHZZHIndex+2,
    MenuGWHZZHIndex+3,
    MenuGWHZZHIndex+4,
    MenuGWHZZHIndex+5,
    MenuGWHZZHIndex+6,			  (*DispMenuGWHZZHInit)},
   {MenuGWHZZHIndex+1,0,0,0,0,0,0,(*DispMenuGWHZZHUp)},
   {MenuGWHZZHIndex+2,0,0,0,0,0,0,(*DispMenuGWHZZHDown)},
   {MenuGWHZZHIndex+3,0,0,0,0,0,0,(*DispMenuGWHZZHLeft)},
   {MenuGWHZZHIndex+4,0,0,0,0,0,0,(*DispMenuGWHZZHRight)},
   {MenuGWHZZHIndex+5,0,0,0,0,0,0,(*DispMenuGWHZZHOK)},
   {MenuGWHZZHIndex+6,0,0,0,0,0,0,(*DispMenuGWHZZHCancel)}, 
   
   /* 网关工作状态显示 */
   {MenuGWHZZHWorkStateIndex,
    MenuGWHZZHWorkStateIndex+1,
    MenuGWHZZHWorkStateIndex+2,
    MenuGWHZZHWorkStateIndex+3,
    MenuGWHZZHWorkStateIndex+4,
    MenuGWHZZHWorkStateIndex+5,
    MenuGWHZZHWorkStateIndex+6,			   (*DispMenuGWHZZHWorkStateInit)},
   {MenuGWHZZHWorkStateIndex+1,0,0,0,0,0,0,(*DispMenuGWHZZHWorkStateUp)},
   {MenuGWHZZHWorkStateIndex+2,0,0,0,0,0,0,(*DispMenuGWHZZHWorkStateDown)},
   {MenuGWHZZHWorkStateIndex+3,0,0,0,0,0,0,(*DispMenuGWHZZHWorkStateLeft)},
   {MenuGWHZZHWorkStateIndex+4,0,0,0,0,0,0,(*DispMenuGWHZZHWorkStateRight)},
   {MenuGWHZZHWorkStateIndex+5,0,0,0,0,0,0,(*DispMenuGWHZZHWorkStateOK)},
   {MenuGWHZZHWorkStateIndex+6,0,0,0,0,0,0,(*DispMenuGWHZZHWorkStateCancel)}, 
   
     /* 网关地址 */
   {MenuGWHZZHWkupTimeIndex,
    MenuGWHZZHWkupTimeIndex+1,
    MenuGWHZZHWkupTimeIndex+2,
    MenuGWHZZHWkupTimeIndex+3,
    MenuGWHZZHWkupTimeIndex+4,
    MenuGWHZZHWkupTimeIndex+5,
    MenuGWHZZHWkupTimeIndex+6,			  (*DispMenuGWHZZHWkupTimeInit)},
   {MenuGWHZZHWkupTimeIndex+1,0,0,0,0,0,0,(*DispMenuGWHZZHWkupTimeUp)},
   {MenuGWHZZHWkupTimeIndex+2,0,0,0,0,0,0,(*DispMenuGWHZZHWkupTimeDown)},
   {MenuGWHZZHWkupTimeIndex+3,0,0,0,0,0,0,(*DispMenuGWHZZHWkupTimeLeft)},
   {MenuGWHZZHWkupTimeIndex+4,0,0,0,0,0,0,(*DispMenuGWHZZHWkupTimeRight)},
   {MenuGWHZZHWkupTimeIndex+5,0,0,0,0,0,0,(*DispMenuGWHZZHWkupTimeOK)},
   {MenuGWHZZHWkupTimeIndex+6,0,0,0,0,0,0,(*DispMenuGWHZZHWkupTimeCancel)}, 

   
#endif
	 
	 
/* Wifi菜单参数界面 */
#if (GSM_Module_Type == 0x02)
   /* Wifi参数设置 */
   {MenuWifiIndex,
    MenuWifiIndex+1,
    MenuWifiIndex+2,
    MenuWifiIndex+3,
    MenuWifiIndex+4,
    MenuWifiIndex+5,
    MenuWifiIndex+6,			  (*DispMenuWifiInit)},
   {MenuWifiIndex+1,0,0,0,0,0,0,(*DispMenuWifiUp)},
   {MenuWifiIndex+2,0,0,0,0,0,0,(*DispMenuWifiDown)},
   {MenuWifiIndex+3,0,0,0,0,0,0,(*DispMenuWifiLeft)},
   {MenuWifiIndex+4,0,0,0,0,0,0,(*DispMenuWifiRight)},
   {MenuWifiIndex+5,0,0,0,0,0,0,(*DispMenuWifiOK)},
   {MenuWifiIndex+6,0,0,0,0,0,0,(*DispMenuWifiCancel)},  
#endif	

	 
};



/************************************************************************************************/
/* 函数名	: Menu_Display																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 这个函数在显示任务里周期性调用，计策当前按键状态，菜单显示和更新状态功能。					*/
/* 创建日期	: 2015/6/15																			*/
/************************************************************************************************/ 
void Menu_Display(u8 key_state)
{
	/* 这样写思路清晰，耗空间。 */
	if (key_state == KEY_DOWN_Sure) 	//短按确定键
	{
        KeyFuncIndex	=	KeyTab[KeyFuncIndex].KeyUpState;
        KeyFuncPtr		=	KeyTab[KeyFuncIndex].CurrentOperate;
		    /* 发送蜂鸣器信号 */
		    Beep_Menu_Key_Sound();
		    /* 执行当前按键的操作 */
        (*KeyFuncPtr)();
	}
	else if (key_state == KEY_LONG_Sure)//长按确定键
	{
        KeyFuncIndex	=	KeyTab[KeyFuncIndex].KeyOKState;
        KeyFuncPtr		=	KeyTab[KeyFuncIndex].CurrentOperate;
		    /* 发送蜂鸣器信号 */
		    Beep_Menu_Key_Sound();
		    /* 执行当前按键的操作 */
       (*KeyFuncPtr)();
	}

	
	if (key_state == KEY_DOWN_Forward) 	//向前
	{
        KeyFuncIndex	=	KeyTab[KeyFuncIndex].KeyRState;
        KeyFuncPtr		=	KeyTab[KeyFuncIndex].CurrentOperate;
		    /* 发送蜂鸣器信号 */
		    Beep_Menu_Key_Sound();
		    /* 执行当前按键的操作 */
        (*KeyFuncPtr)();
	}
	if (key_state == KEY_DOWN_Backward) 	//向后
	{
        KeyFuncIndex	=	KeyTab[KeyFuncIndex].KeyLState;
        KeyFuncPtr		=	KeyTab[KeyFuncIndex].CurrentOperate;
		    /* 发送蜂鸣器信号 */
		    Beep_Menu_Key_Sound();
		    /* 执行当前按键的操作 */
        (*KeyFuncPtr)();
	}

	if (key_state == KEY_DOWN_Cancel) 	//取消
	{
		    KeyFuncIndex	=	KeyTab[KeyFuncIndex].KeyDnState;
        KeyFuncPtr		=	KeyTab[KeyFuncIndex].CurrentOperate;
		    /* 发送蜂鸣器信号 */
		    Beep_Menu_Key_Sound();
		    /* 执行当前按键的操作 */
       (*KeyFuncPtr)();	
	}
	else if (key_state == KEY_LONG_Cancel)
	{
        KeyFuncIndex	=	KeyTab[KeyFuncIndex].KeyCancelState;
        KeyFuncPtr		=	KeyTab[KeyFuncIndex].CurrentOperate;
		    /* 发送蜂鸣器信号 */
		    Beep_Menu_Key_Sound();
		    /* 执行当前按键的操作 */
        (*KeyFuncPtr)();
	}
	
	/* 执行对应动作的函数 */
	if(Flash)
	{	
		/* 根据当前的函数指针序号，查表获得对应的函数指针。 */
        KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
		/* 执行函数 */
        (*KeyFuncPtr)();
		/* 清除函数动作 */
        Flash = 0;
   }
}




