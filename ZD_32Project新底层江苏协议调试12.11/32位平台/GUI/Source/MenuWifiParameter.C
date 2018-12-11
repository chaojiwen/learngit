/************************************************************************************************/
/** @file              : MenuWifiParameter.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2018年09月18日															*/
/** @date changed      : 2018年09月18日															*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->Wifi参数设置														*/
/************************************************************************************************/
#include "Gsm_Api.h"

#if (GSM_Module_Type == 0x02)

#include "Ks0108.H"
#include "Menu.H"

#include "Mcu_Config.h"

#include "string.h"

#define          WifiItemNum              (5u)  //查询项目菜单的个数

/* wifi界面 中文 菜单项目栏 */
static u8 * const MenuWifi_CN[WifiItemNum] = 
{
	{"工作状态"},
	{"MAC地址"},
	{"网络名称及密码"},
	{"网络加密及IP方式"},
	{"Wlan参数"},
};

/* Wifi界面 英文 菜单项目栏 */
static u8 * const MenuWifi_EN[WifiItemNum] = 
{
	{"Work State"},
	{"MAC  Address"},
	{"Network NamePWD"},
	{"NetworkAndIP Way"},
	{"Wlan Parameter"},
};



/* wifi界面菜单项目栏 */
const struct Menu MenuWifi_Item[] = 
{
	MenuWhat(MenuWifi_CN),
	MenuWhat(MenuWifi_EN),
};


/* 显示Wifi状态 中文 菜单项目栏 */
u8 * const MenuWifiWorkState_CN[]=
{
	{"故障"},
	{"正常"},
};
/* 显示Wifi状态 英文 菜单项目栏 */
u8 * const MenuWifiWorkState_EN[]=
{
	{"Fault"},
	{"Normal"},
};
/* 显示Wifi状态字符串X位置 */
#define MenuWifiWorkStateStr_PosX		(48u)
/* 显示Wifi状态字符串Y位置 */
#define MenuWifiWorkStateStr_PosY		(Line_2_PosY)


/* 显示WifiMAC地址字符串X位置 */
#define MenuWifiMAC_AddressStr_PosX		(8u)
/* 显示WifiMAC地址字符串Y位置 */
#define MenuWifiMAC_AddressStr_PosY		(Line_2_PosY)
/* 显示WifiMAC地址 中文 菜单项目栏 */
u8 * const MenuWifiMAC_Address_CN[]=
{
	{"5c:cf:7f:74:e8:5a"},
	{"5d:cf:7f:74:e8:5a"},
};

/* 显示WifiMAC地址 英文 菜单项目栏 */
u8 * const MenuWifiMAC_Address_EN[]=
{
	{"5c:cf:7f:74:e8:5a"},
	{"5d:cf:7f:74:e8:5a"},
};


/* 显示WifiNamePWD字符串X位置 */
#define MenuWifiNamePWDStr_PosX		(8u)
/* 显示WifiNamePWD字符串Y位置 */
#define MenuWifiNamePWDStr_PosY		(Line_2_PosY)

/* 显示WifiNamePWD 中文 菜单项目栏 */
u8 * const MenuWifiNamePWD_CN[]=
{
	{"HiWiFi_1S"},
	{"Open DaulWAN"},
	{"TP-LINK_363A68"},
	{"abcdEWFSATYFGaw"},
};

/* 显示WifiNamePWD 英文 菜单项目栏 */
u8 * const MenuWifiNamePWD_EN[]=
{
  {"HiWiFi_1S"},
	{"Open DaulWAN"},
};

/* 显示WifiNetWorkEncryptionWay字符串X位置 */
#define MenuWifiNetWorkEncryptionWayStr_PosX		(8u)
/* 显示WifiNetWorkEncryptionWay字符串Y位置 */
#define MenuWifiNetWorkEncryptionWayStr_PosY		(Line_2_PosY)

/* 显示WifiWlanParameter字符串X位置 */
#define MenuWifiWlanParameterStr_PosX		(8u)
/* 显示WifiWlanParameter字符串Y位置 */
#define MenuWifiWlanParameterStr_PosY		(Line_2_PosY)




/* 显示WifiNetWorkEncryptionWay 英文 菜单项目栏 */
u8 * const MenuWifiNetWorkEncryptionWay_EN[]=
{
  {"Disable"},
	{"OPENWEP"},
	{"SHAREDWEP"},
	{"WPA-PSK TKIP"},
	{"WPA-PSK AES"},
	{"WPA2-PSK TKIP"},
  {"WPA2-PSK AES"},
};
/* 显示WifiNetWorkEncryptionWay 中文 菜单项目栏 */
u8 * const MenuWifiNetWorkEncryptionWay_CN[]=
{
  {"Disable"},
	{"OPENWEP"},
	{"SHAREDWEP"},
	{"WPA-PSK TKIP"},
	{"WPA-PSK AES"},
	{"WPA2-PSK TKIP"},
  {"WPA2-PSK AES"},
};

/* 显示WifiWlanParameter 中文 菜单项目栏 */
u8 * const MenuWifiWlanParameter_CN[]=
{
	{"HiWiFi_1S"},
	{"Open DaulWAN"},
	{"TP-LINK_363A68"},
	{"abcdEWFSATYFGaw"},
};

/* 显示WifiWlanParameter 英文 菜单项目栏 */
u8 * const MenuWifiWlanParameter_EN[]=
{
  {"HiWiFi_1S"},
	{"Open DaulWAN"},
};




/************************************************************************************************/
/* 函数名	: DispMenuWifiInit																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示Wifi界面信息																	*/
/* 创建日期	: 2018/09/18																		*/
/* 修改日期	: 2018/09/18																		*/
/************************************************************************************************/
void DispMenuWifiInit(void)
{
	
	
  if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		       /* 清除屏幕显示 */
		       GUI_Clear();
			    		/* 判断当前的语言 */
		       if(*Language)
		       {
		       	/* 英文 */
		       	/* 显示设备状态 */
		       		GUI_Disp_Headline(MenuWifi_EN[0]);
		           if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==0)//Wifi模块不响应AT指令
			       	{
			          	GUI_DispStringAt(MenuWifiWorkState_EN[0],(MenuWifiWorkStateStr_PosX-8),MenuWifiWorkStateStr_PosY);
			       	
			       	}
			       	else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==1)//Wifi模块响应AT指令
			       	{
			       	   GUI_DispStringAt(MenuWifiWorkState_EN[1],(MenuWifiWorkStateStr_PosX-8),MenuWifiWorkStateStr_PosY);
			       	}
			       	else
			       	{
			       	  GUI_DispStringAt(MenuWifiWorkState_EN[0],(MenuWifiWorkStateStr_PosX-8),MenuWifiWorkStateStr_PosY);
			       	}
		       }else
		       {
		       	/* 中文 */
		       		GUI_Disp_Headline(MenuWifi_CN[0]);
		            if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==0)//Wifi模块不响应AT指令
				       {
				       	GUI_DispStringAt(MenuWifiWorkState_CN[0],MenuWifiWorkStateStr_PosX,MenuWifiWorkStateStr_PosY);
				       }
				       else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==1)//Wifi模块响应AT指令
				       {
				         GUI_DispStringAt(MenuWifiWorkState_CN[1],MenuWifiWorkStateStr_PosX,MenuWifiWorkStateStr_PosY);
				       }
				       else
				       {
				       	GUI_DispStringAt(MenuWifiWorkState_CN[0],MenuWifiWorkStateStr_PosX,MenuWifiWorkStateStr_PosY);
				       }
		       }
					 
           Item=0;
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuWifiUp																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入选择的Wifi界面																	*/
/* 创建日期	: 2018/09/18																		*/
/* 修改日期	: 2018/09/18																		*/
/************************************************************************************************/
void DispMenuWifiUp(void)
{
/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuWifiDown																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出Wifi界面																		*/
/* 创建日期	: 2018/09/18																		*/
/* 修改日期	: 2018/09/18																		*/
/************************************************************************************************/
void DispMenuWifiDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

static  void WifiBarMenuLeft(void)
{
    //维护Item,得到需要反显的Item
    if (Item == 0) {
        Item = WifiItemNum-1;
    } else {
        Item--;
    }
    Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
}
static  void WifiBarMenuRight(void)
{
//---维护Item
    if (Item == (WifiItemNum-1)) {
        Item = 0;
    } else {
        Item++;
    }
    Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
}

   
/************************************************************************************************/
/* 函数名	: DispMenuWifiLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择Wifi界面的项目															*/
/* 创建日期	: 2018/09/18																		*/
/* 修改日期	: 2018/09/18																		*/
/************************************************************************************************/
void DispMenuWifiLeft(void)
{
   WifiBarMenuLeft();
	switch(Item)
	{
    case 0:	/* 进入 wifi模块工作状态 界面 */
		{
		    /* 清除屏幕显示 */
		   GUI_Clear();
		/* 判断当前的语言 */
		if(*Language)
		{
			/* 英文 */
			/* 显示设备状态 */
				GUI_Disp_Headline(MenuWifi_EN[0]);
			
			  if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==0)//Wifi模块不响应AT指令
				{
			   	GUI_DispStringAt(MenuWifiWorkState_EN[0],(MenuWifiWorkStateStr_PosX-8),MenuWifiWorkStateStr_PosY);
				
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==1)//Wifi模块响应AT指令
				{
				   GUI_DispStringAt(MenuWifiWorkState_EN[1],(MenuWifiWorkStateStr_PosX-8),MenuWifiWorkStateStr_PosY);
				}
				else
				{
				  GUI_DispStringAt(MenuWifiWorkState_EN[0],(MenuWifiWorkStateStr_PosX-8),MenuWifiWorkStateStr_PosY);
				}
				
				
				
		}else
		{
			/* 中文 */
			  GUI_Disp_Headline(MenuWifi_CN[0]);
				if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==0)//Wifi模块不响应AT指令
				{
					GUI_DispStringAt(MenuWifiWorkState_CN[0],MenuWifiWorkStateStr_PosX,MenuWifiWorkStateStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==1)//Wifi模块响应AT指令
				{
				  GUI_DispStringAt(MenuWifiWorkState_CN[1],MenuWifiWorkStateStr_PosX,MenuWifiWorkStateStr_PosY);
				}
				else
				{
					GUI_DispStringAt(MenuWifiWorkState_CN[0],MenuWifiWorkStateStr_PosX,MenuWifiWorkStateStr_PosY);
				}
     }
       break;
     }
		case 1:/* 进入 wifiMAC地址 界面 */
		{

		   GUI_Clear();
	    	/* 判断当前的语言 */
		  if(*Language)
				
		  {		GUI_Disp_Headline(MenuWifi_EN[1]);
				  if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address)>0)
					{
					  GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address,(MenuWifiMAC_AddressStr_PosX-8),MenuWifiMAC_AddressStr_PosY);
					}
					else
					{
					 GUI_DispStringAt("      NULL       ",(MenuWifiMAC_AddressStr_PosX-8),MenuWifiMAC_AddressStr_PosY);
					} 
		  		
		  }else
		  {
		  	/* 中文 */
		  	/* 显示设备状态 */

				  GUI_Disp_Headline(MenuWifi_CN[1]);
				  if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address)>0)
					{
					  GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address,MenuWifiMAC_AddressStr_PosX,MenuWifiMAC_AddressStr_PosY);
					}
					else
					{
					 GUI_DispStringAt("      空       ",(MenuWifiMAC_AddressStr_PosX),MenuWifiMAC_AddressStr_PosY);
					}					
		  		
		  }

            break;
     }

    case 2:	/* 进入 wifi名称及密码 界面 */
		{
			 GUI_Clear();
	    		/* 判断当前的语言 */
		if(*Language)
		{
	
			/* 英文 */
			/* 显示设备状态 */
				GUI_DispStringAt("Network Name->",MenuWifiNamePWDStr_PosX-8,Line_1_PosY);
			  if(strlen(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name)>0)
				{
				  GUI_DispStringAt((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name,(MenuWifiNamePWDStr_PosX-8),MenuWifiNamePWDStr_PosY);
				}
				else
				{
				  GUI_DispStringAt("      NULL       ",(MenuWifiNamePWDStr_PosX-8),MenuWifiNamePWDStr_PosY);
				}

				GUI_DispStringAt("Network PWD->",(MenuWifiNamePWDStr_PosX-8),Line_3_PosY);
				if(strlen(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password)>0)
				{
				  GUI_DispStringAt((u8*)		Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password,MenuWifiNamePWDStr_PosX-8,Line_4_PosY);
				}
				else
				{
				  GUI_DispStringAt("      NULL       ",MenuWifiNamePWDStr_PosX-8,Line_4_PosY);
				}
				
		}else
		{
			/* 中文 */
			/* 显示设备状态 */
			  GUI_DispStringAt("网络名称->",MenuWifiNamePWDStr_PosX,Line_1_PosY);
				 if(strlen(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name)>0)
				{
				  GUI_DispStringAt((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name,(MenuWifiNamePWDStr_PosX),MenuWifiNamePWDStr_PosY);
				}
				else
				{
				  GUI_DispStringAt("      空       ",(MenuWifiNamePWDStr_PosX),MenuWifiNamePWDStr_PosY);
				}
				GUI_DispStringAt("网络密码->",MenuWifiNamePWDStr_PosX,Line_3_PosY);
				if(strlen(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password)>0)
				{
				  GUI_DispStringAt((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password,MenuWifiNamePWDStr_PosX,Line_4_PosY);
				}
				else
				{
				  GUI_DispStringAt("      空       ",MenuWifiNamePWDStr_PosX,Line_4_PosY);
				}
		}
         break;
     }
    case 3:/* 进入 Wifi加密及IP地址获取方式 界面 */
		{
			 GUI_Clear();
	    		/* 判断当前的语言 */
		if(*Language)
		{
			/* 英文 */
			/* 显示设备状态 */
				GUI_DispStringAt("Encryption Way->",MenuWifiNetWorkEncryptionWayStr_PosX-8,Line_1_PosY);
			  if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==0)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[0],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==1)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[1],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==2)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[2],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
			  else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==3)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[3],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==4)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[4],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==5)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[5],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}	
			  else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==6)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[6],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[0],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				
				
				
				GUI_DispStringAt("IPGetWay->",(MenuWifiNetWorkEncryptionWayStr_PosX-8),Line_3_PosY);
				if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==0)
				{
						GUI_DispStringAt("Static",MenuWifiNetWorkEncryptionWayStr_PosX-8,Line_4_PosY);
				}
				else  if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==1)
				{
					 GUI_DispStringAt("DHCP",MenuWifiNetWorkEncryptionWayStr_PosX-8,Line_4_PosY);
				}
				else
				{
				   GUI_DispStringAt("DHCP",MenuWifiNetWorkEncryptionWayStr_PosX-8,Line_4_PosY);
				}
		}else
		{
			/* 中文 */
			/* 显示设备状态 */
			  GUI_DispStringAt("网络加密方式->",MenuWifiNetWorkEncryptionWayStr_PosX,Line_1_PosY);
			if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==0)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[0],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==1)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[1],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==2)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[2],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
			  else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==3)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[3],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==4)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[4],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==5)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[5],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}	
			  else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==6)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[6],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[0],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				GUI_DispStringAt("IP获取方式->",MenuWifiNetWorkEncryptionWayStr_PosX,Line_3_PosY);
				if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==0)
				{
						GUI_DispStringAt("Static",MenuWifiNetWorkEncryptionWayStr_PosX,Line_4_PosY);
				}
				else  if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==1)
				{
					 GUI_DispStringAt("DHCP",MenuWifiNetWorkEncryptionWayStr_PosX,Line_4_PosY);
				}
				else
				{
				   GUI_DispStringAt("DHCP",MenuWifiNetWorkEncryptionWayStr_PosX,Line_4_PosY);
				}
		}
           break;
     }
		 case 4:
		{
			/* 进入 Wifi 本地IP地址、子网掩码及网关地址 界面 */
			 GUI_Clear();
	    		/* 判断当前的语言 */
		if(*Language)
		{
			/* 英文 */
			/* 显示设备状态 */
				GUI_DispStringAt("IP-Gateway-Mask",MenuWifiWlanParameterStr_PosX-8,Line_1_PosY);
			  	 if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP)>0)
					 {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,MenuWifiWlanParameterStr_PosX-8,Line_2_PosY);
					 } 
           else
					 {
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_2_PosY);
					 }
				  if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP)>0)
				  {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,MenuWifiWlanParameterStr_PosX-8,Line_3_PosY);	
				  }
					else
					{
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_3_PosY);	  
					}
					 if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask)>0)
					 {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,MenuWifiWlanParameterStr_PosX-8,Line_4_PosY);
					 
					 }
					 else
					 {
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_4_PosY);
					 }
	    
		}else
		{
			/* 中文 */
			/* 显示设备状态 */
			  GUI_DispStringAt("IP-网关-掩码",MenuWifiWlanParameterStr_PosX,Line_1_PosY);
         if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP)>0)
					 {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,MenuWifiWlanParameterStr_PosX-8,Line_2_PosY);
					 } 
           else
					 {
					   GUI_DispStringAt("      空       ",MenuWifiWlanParameterStr_PosX-8,Line_2_PosY);
					 }
					 
				  if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP)>0)
				  {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,MenuWifiWlanParameterStr_PosX-8,Line_3_PosY);	
				  }
					else
					{
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_3_PosY);	  
					}
					 if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask)>0)
					 {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,MenuWifiWlanParameterStr_PosX-8,Line_4_PosY);
					 
					 }
					 else
					 {
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_4_PosY);
					 }
		}
         break;
     }
        default:
		{
			/* 未定义界面,设置界面。 */
            Jump2Menu(MenuWifiIndex,FlashMode_NoAction);
			break;
		}
  }	
}

/************************************************************************************************/
/* 函数名	: DispMenuWifiRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择Wifi界面的项目															*/
/* 创建日期	: 2018/09/18																		*/
/* 修改日期	: 2018/09/18																		*/
/************************************************************************************************/
void DispMenuWifiRight(void)
{
    WifiBarMenuRight();
switch(Item)
	{
    case 0:	/* 进入 wifi模块工作状态 界面 */
		{
		    /* 清除屏幕显示 */
		   GUI_Clear();
		/* 判断当前的语言 */
		if(*Language)
		{
			/* 英文 */
			/* 显示设备状态 */
				GUI_Disp_Headline(MenuWifi_EN[0]);
			
			  if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==0)//Wifi模块不响应AT指令
				{
			   	GUI_DispStringAt(MenuWifiWorkState_EN[0],(MenuWifiWorkStateStr_PosX-8),MenuWifiWorkStateStr_PosY);
				
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==1)//Wifi模块响应AT指令
				{
				   GUI_DispStringAt(MenuWifiWorkState_EN[1],(MenuWifiWorkStateStr_PosX-8),MenuWifiWorkStateStr_PosY);
				}
				else
				{
				  GUI_DispStringAt(MenuWifiWorkState_EN[0],(MenuWifiWorkStateStr_PosX-8),MenuWifiWorkStateStr_PosY);
				}
				
				
				
		}else
		{
			/* 中文 */
			  GUI_Disp_Headline(MenuWifi_CN[0]);
				if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==0)//Wifi模块不响应AT指令
				{
					GUI_DispStringAt(MenuWifiWorkState_CN[0],MenuWifiWorkStateStr_PosX,MenuWifiWorkStateStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State==1)//Wifi模块响应AT指令
				{
				  GUI_DispStringAt(MenuWifiWorkState_CN[1],MenuWifiWorkStateStr_PosX,MenuWifiWorkStateStr_PosY);
				}
				else
				{
					GUI_DispStringAt(MenuWifiWorkState_CN[0],MenuWifiWorkStateStr_PosX,MenuWifiWorkStateStr_PosY);
				}
     }
       break;
     }
		case 1:/* 进入 wifiMAC地址 界面 */
		{

		   GUI_Clear();
	    	/* 判断当前的语言 */
		  if(*Language)
				
		  {		GUI_Disp_Headline(MenuWifi_EN[1]);
				  if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address)>0)
					{
					  GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address,(MenuWifiMAC_AddressStr_PosX-8),MenuWifiMAC_AddressStr_PosY);
					}
					else
					{
					 GUI_DispStringAt("      NULL       ",(MenuWifiMAC_AddressStr_PosX-8),MenuWifiMAC_AddressStr_PosY);
					} 
		  		
		  }else
		  {
		  	/* 中文 */
		  	/* 显示设备状态 */

				  GUI_Disp_Headline(MenuWifi_CN[1]);
				  if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address)>0)
					{
					  GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address,MenuWifiMAC_AddressStr_PosX,MenuWifiMAC_AddressStr_PosY);
					}
					else
					{
					 GUI_DispStringAt("      空       ",(MenuWifiMAC_AddressStr_PosX),MenuWifiMAC_AddressStr_PosY);
					}					
		  		
		  }

            break;
     }

    case 2:	/* 进入 wifi名称及密码 界面 */
		{
			 GUI_Clear();
	    		/* 判断当前的语言 */
		if(*Language)
		{
	
			/* 英文 */
			/* 显示设备状态 */
				GUI_DispStringAt("Network Name->",MenuWifiNamePWDStr_PosX-8,Line_1_PosY);
			  if(strlen(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name)>0)
				{
				  GUI_DispStringAt((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name,(MenuWifiNamePWDStr_PosX-8),MenuWifiNamePWDStr_PosY);
				}
				else
				{
				  GUI_DispStringAt("      NULL       ",(MenuWifiNamePWDStr_PosX-8),MenuWifiNamePWDStr_PosY);
				}

				GUI_DispStringAt("Network PWD->",(MenuWifiNamePWDStr_PosX-8),Line_3_PosY);
				if(strlen(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password)>0)
				{
				  GUI_DispStringAt((u8*)		Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password,MenuWifiNamePWDStr_PosX-8,Line_4_PosY);
				}
				else
				{
				  GUI_DispStringAt("      NULL       ",MenuWifiNamePWDStr_PosX-8,Line_4_PosY);
				}
				
		}else
		{
			/* 中文 */
			/* 显示设备状态 */
			  GUI_DispStringAt("网络名称->",MenuWifiNamePWDStr_PosX,Line_1_PosY);
				 if(strlen(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name)>0)
				{
				  GUI_DispStringAt((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name,(MenuWifiNamePWDStr_PosX),MenuWifiNamePWDStr_PosY);
				}
				else
				{
				  GUI_DispStringAt("      空       ",(MenuWifiNamePWDStr_PosX),MenuWifiNamePWDStr_PosY);
				}
				GUI_DispStringAt("网络密码->",MenuWifiNamePWDStr_PosX,Line_3_PosY);
				if(strlen(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password)>0)
				{
				  GUI_DispStringAt((u8*)Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password,MenuWifiNamePWDStr_PosX,Line_4_PosY);
				}
				else
				{
				  GUI_DispStringAt("      空       ",MenuWifiNamePWDStr_PosX,Line_4_PosY);
				}
		}
         break;
     }
    case 3:/* 进入 Wifi加密及IP地址获取方式 界面 */
		{
			 GUI_Clear();
	    		/* 判断当前的语言 */
		if(*Language)
		{
			/* 英文 */
			/* 显示设备状态 */
				GUI_DispStringAt("Encryption Way->",MenuWifiNetWorkEncryptionWayStr_PosX-8,Line_1_PosY);
			  if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==0)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[0],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==1)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[1],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==2)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[2],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
			  else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==3)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[3],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==4)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[4],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==5)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[5],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}	
			  else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==6)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[6],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_EN[0],(MenuWifiNetWorkEncryptionWayStr_PosX-8),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				
				
				
				GUI_DispStringAt("IPGetWay->",(MenuWifiNetWorkEncryptionWayStr_PosX-8),Line_3_PosY);
				if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==0)
				{
						GUI_DispStringAt("Static",MenuWifiNetWorkEncryptionWayStr_PosX-8,Line_4_PosY);
				}
				else  if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==1)
				{
					 GUI_DispStringAt("DHCP",MenuWifiNetWorkEncryptionWayStr_PosX-8,Line_4_PosY);
				}
				else
				{
				   GUI_DispStringAt("DHCP",MenuWifiNetWorkEncryptionWayStr_PosX-8,Line_4_PosY);
				}
		}else
		{
			/* 中文 */
			/* 显示设备状态 */
			  GUI_DispStringAt("网络加密方式->",MenuWifiNetWorkEncryptionWayStr_PosX,Line_1_PosY);
			if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==0)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[0],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==1)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[1],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==2)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[2],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
			  else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==3)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[3],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==4)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[4],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==5)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[5],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}	
			  else if(Instru_Wifi_Fix_Tab.Inf.Wifi_Encryption_Way==6)
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[6],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				else
				{
				  GUI_DispStringAt(MenuWifiNetWorkEncryptionWay_CN[0],(MenuWifiNetWorkEncryptionWayStr_PosX),MenuWifiNetWorkEncryptionWayStr_PosY);
				}
				GUI_DispStringAt("IP获取方式->",MenuWifiNetWorkEncryptionWayStr_PosX,Line_3_PosY);
				if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==0)
				{
						GUI_DispStringAt("Static",MenuWifiNetWorkEncryptionWayStr_PosX,Line_4_PosY);
				}
				else  if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==1)
				{
					 GUI_DispStringAt("DHCP",MenuWifiNetWorkEncryptionWayStr_PosX,Line_4_PosY);
				}
				else
				{
				   GUI_DispStringAt("DHCP",MenuWifiNetWorkEncryptionWayStr_PosX,Line_4_PosY);
				}
		}
           break;
     }
		 case 4:
		{
			/* 进入 Wifi 本地IP地址、子网掩码及网关地址 界面 */
			 GUI_Clear();
	  	    		/* 判断当前的语言 */
		if(*Language)
		{
			/* 英文 */
			/* 显示设备状态 */
				GUI_DispStringAt("IP-Gateway-Mask",MenuWifiWlanParameterStr_PosX-8,Line_1_PosY);
			  	 if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP)>0)
					 {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,MenuWifiWlanParameterStr_PosX-8,Line_2_PosY);
					 } 
           else
					 {
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_2_PosY);
					 }
				  if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP)>0)
				  {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,MenuWifiWlanParameterStr_PosX-8,Line_3_PosY);	
				  }
					else
					{
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_3_PosY);	  
					}
					 if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask)>0)
					 {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,MenuWifiWlanParameterStr_PosX-8,Line_4_PosY);
					 
					 }
					 else
					 {
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_4_PosY);
					 }
	    
		}else
		{
			/* 中文 */
			/* 显示设备状态 */
			  GUI_DispStringAt("IP-网关-掩码",MenuWifiWlanParameterStr_PosX,Line_1_PosY);
         if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP)>0)
					 {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,MenuWifiWlanParameterStr_PosX-8,Line_2_PosY);
					 } 
           else
					 {
					   GUI_DispStringAt("      空       ",MenuWifiWlanParameterStr_PosX-8,Line_2_PosY);
					 }
					 
				  if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP)>0)
				  {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,MenuWifiWlanParameterStr_PosX-8,Line_3_PosY);	
				  }
					else
					{
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_3_PosY);	  
					}
					 if(strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask)>0)
					 {
					   GUI_DispStringAt((u8*)Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,MenuWifiWlanParameterStr_PosX-8,Line_4_PosY);
					 
					 }
					 else
					 {
					   GUI_DispStringAt("      NULL       ",MenuWifiWlanParameterStr_PosX-8,Line_4_PosY);
					 }
		}
         break;
     }
        default:
		{
			/* 未定义界面,设置界面。 */
            Jump2Menu(MenuWifiIndex,FlashMode_NoAction);
			break;
		}
  }	
	
	
	
}

/************************************************************************************************/
/* 函数名	: DispMenuWifiOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）														*/
/* 创建日期	: 2018/09/18																		*/
/* 修改日期	: 2018/09/18																		*/
/************************************************************************************************/
void DispMenuWifiOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuWifiIndex,FlashMode_NoAction);
	
}

/************************************************************************************************/
/* 函数名	: DispMenuWifiCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）														*/
/* 创建日期	: 2018/09/18																		*/
/* 修改日期	: 2018/09/18																		*/
/************************************************************************************************/
void DispMenuWifiCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuWifiIndex,FlashMode_NoAction);
}  


#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
