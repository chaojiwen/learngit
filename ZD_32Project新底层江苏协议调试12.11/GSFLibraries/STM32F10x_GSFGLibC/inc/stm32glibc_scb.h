/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_scb.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-06
* Last Update        : 2014-11-06
* Description        : SCB(系统控制模块) firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_SCB_H
#define __STM32GLIBC_SCB_H

#include "stm32glibc_type.h"



/* Configuration Mode enumeration --------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
namespace	mcucore
{
    /** @addtogroup CMSIS_CM3_SCB CMSIS CM3 SCB
    memory mapped structure for System Control Block (SCB)
    @{
    */
    enum PriorityGroupingStyle
    {
        GRP16_SUB1	= BByte(011),		// 16 priority groups, 1(none) sub priority
        GRP8_SUB2	= BByte(100),		// 8 priority groups, 2 sub priority (none)
        GRP4_SUB4	= BByte(101),		// 4 priority groups, 4 sub priority (none)
        GRP2_SUB8	= BByte(110),		// 2 priority groups, 8 sub priority (none)
        GRP1_SUB16	= BByte(111),		// 1 (none priority groups, 16 sub priority (none)
    };

    enum AIRCR_KEY
    {
        CONST_REGISTER_KEY = 0x05FA,
        CONST_READ_KEY = 0xFA05,
    };
    // 注意每次写这个AIRCR寄存器都要在VECTKEYSTAT的bit field里写入0x05fa
    enum SCB_AIRCR
    {
        VECTKEYSTAT		= BitFromTo(31, 16),// Register key
        ENDIANESS		= Bit(15),			//
        PRIGROUP		= BitFromTo(10, 8),	// Interrupt priority grouping field
        SYSRESETREQ		= Bit(2),			// System reset request
        VECTCLRACTIVE 	= Bit(1),			// Reserved for Debug use.
        VECTRESET		= Bit(0),			// Reserved for Debug use.
    };

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! CPU ID 基址寄存器 寄存器(SCB_CPUID)	@0xE000ED00
    struct SCB_CPUID_Struct
    {
        //! 设备已定义的版本号
        __I uint32_t REVISION                      : 4;	//Bit0 ~ Bit3
        //! 系列处理器的编号
        __I uint32_t PARTNO                        : 12;	//Bit4 ~ Bit15
        __I uint32_t Resv1                         : 4;	//Bit16 ~ Bit19
        //! 设备已定义的变量号
        __I uint32_t VARIANT                       : 4;	//Bit20 ~ Bit23
        //! 实现者代码。ARM为0x41
        __I uint32_t IMPLEMENTER                   : 8;	//Bit24 ~ Bit31
    };
    typedef struct SCB_CPUID_Struct SCB_CPUID_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 中断控制状态寄存器 (SCB_ICSR)		@0xE000ED04
    struct SCB_ICSR_Struct
    {
        //! 激活ISR的号码域
        __I uint32_t VECTACIVE                     : 10;	//Bit0 ~ Bit9
        //! 激活的基本级
        __I uint32_t RETTOBASE                     : 1;	//Bit10
        __I uint32_t Resv1                         : 1;	//Bit11
        //! 挂起ISR的号码域
        __I uint32_t VECTPENDING                   : 10;	//Bit12 ~ Bit21
        //! 中断挂起标志
        __I uint32_t ISRPENDING                    : 1;	//Bit22
        //!
        __I uint32_t ISRPREEMPT                    : 1;	//Bit23
        __I uint32_t Resv2                         : 1;	//Bit24
        //! 清除挂起SysTick位
        __IO uint32_t  PENDSTCLR                     : 1;	//Bit25
        __I uint32_t Resv3                         : 7;	//Bit26 ~ Bit31
    };
    typedef struct SCB_ICSR_Struct SCB_ICSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 向量表偏移寄存器 (SCB_VTOR)			@0xE000ED08
    struct SCB_VTOR_Struct
    {
        __IO uint32_t Resv1							: 7;	//Bit0 ~ Bit6
        //! 向量表的基址偏移域
        __IO uint32_t TBLOFF							: 22;	//Bit7 ~ Bit28
        //! 向量表基址位于Code（0）或RAM（1）处
        __IO uint32_t TBLBASE						: 1;	//Bit29
        __IO uint32_t Resv2							: 2;	//Bit30 ~ Bit31
    };
    typedef struct SCB_VTOR_Struct SCB_VTOR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 应用中断与复位控制寄存器 (SCB_AIRCR)	@0xE000ED0C
    struct SCB_AIRCR_Struct
    {
        //!	系统复位位
        __IO uint32_t VECTRESET						: 1;	//Bit0
        //! 清除有效向量位
        __IO uint32_t VECTCLRACTIVE					: 1;	//Bit1
        //! 让信号在外部系统有效，表示请求复位
        __IO uint32_t SYSRESETREQ					: 1;	//Bit2
        __IO uint32_t Resv1							: 5;	//Bit3 ~ Bit7
        //! 中断优先级分组域
        __IO uint32_t PRIGROUP						: 3;	//Bit8 ~ Bit10
        __IO uint32_t Resv2							: 4;	//Bit11 ~ Bit14
        //! 数据的字节顺序位
        __IO uint32_t ENDIANESS						: 1;	//Bit15
        //! 读取时为0xFA05
        //! 注册码（register key）:
        //! 对寄存器进行写操作时要求在 VECTKEY域中写入0x5FA。否则写入值被忽略。
        __IO uint32_t VECTKEY						: 16;	//Bit16 ~ Bit31
    };
    typedef struct SCB_AIRCR_Struct SCB_AIRCR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 系统控制寄存器 (SCB_SCR)			@0xE000ED10
    struct SCB_SCR_Struct
    {
        //!
        __IO uint32_t Resv1							: 1;	//Bit0
        //! 当从处理器模式返回到线程模式时，开始“退出时睡眠”
        __IO uint32_t SLEEPONEXIT					: 1;	//Bit1
        //! 深度睡眠位：
        __IO uint32_t SLEEPDEEP						: 1;	//Bit2
        __IO uint32_t Resv2							: 1;	//Bit3
        //! 使能时，在中断从没有挂起到挂起时 SEVONPEND 可以唤醒 WFE。
        __IO uint32_t SEVONPEND						: 1;	//Bit4
        __IO uint32_t Resv3							: 27;	//Bit5 ~ Bit31
    };
    typedef struct SCB_SCR_Struct SCB_SCR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 配置控制寄存器 (SCB_CCR)			@0xE000ED14
    struct SCB_CCR_Struct
    {
        //!	为0时，默认下在从上次异常返回时只能进入线程模式
        __IO uint32_t NONEBASETHRDENA				: 1;	//Bit0
        //! 如果写成 1，那么用户代码可以写软件触发中断寄存器以触发（挂起）
        __IO uint32_t USERSETMPEND					: 1;	//Bit1
        __IO uint32_t Resv1							: 1;	//Bit2
        //! 不对齐访问陷阱。
        __IO uint32_t UNALIGN_TRP					: 1;	//Bit3
        //! 除0陷阱。
        __IO uint32_t DIV_0_TRP 						: 1;	//Bit4
        __IO uint32_t Resv2							: 3;	//Bit5 ~ Bit7
        //! 一般将它用于探测系统设备和桥接器以检测并纠正控制信道问题。
        __IO uint32_t BFHFNMIGN						: 1;	//Bit8
        __IO uint32_t Resv3							: 23;	//Bit9 ~ Bit31
    };
    typedef struct SCB_CCR_Struct SCB_CCR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 系统处理器优先级寄存器 (SCB_SHP)	@0xE000ED18
    struct SCB_SHP_Struct
    {
        volatile uint32_t PRI_4	                        : 8;	//Bit0 ~ Bit7
        volatile uint32_t PRI_5	                        : 8;	//Bit8 ~ Bit15
        volatile uint32_t PRI_6	                        : 8;	//Bit16 ~ Bit23
        volatile uint32_t PRI_7	                        : 8;	//Bit24 ~ Bit31
        volatile uint32_t PRI_8	                        : 8;	//Bit0 ~ Bit7
        volatile uint32_t PRI_9	                        : 8;	//Bit8 ~ Bit15
        volatile uint32_t PRI_10                        : 8;	//Bit16 ~ Bit23
        volatile uint32_t PRI_11                        : 8;	//Bit24 ~ Bit31
        volatile uint32_t PRI_12                        : 8;	//Bit0 ~ Bit7
        volatile uint32_t PRI_13                        : 8;	//Bit8 ~ Bit15
        volatile uint32_t PRI_14                        : 8;	//Bit16 ~ Bit23
        volatile uint32_t PRI_15                        : 8;	//Bit24 ~ Bit31
    };
    typedef struct SCB_SHP_Struct SCB_SHP_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 系统处理器控制与状态寄存器 (SCB_SHCSR)	@0xE000ED24
    struct SCB_SHCSR_Struct
    {
        //!	如果MemManage已激活，那么为1
        __IO uint32_t MEMFAULTACT					: 1;	//Bit0
        //! 如果BusFault 已激活，那么为1
        __IO uint32_t BUSFAULTACT					: 1;	//Bit1
        __IO uint32_t Resv1							: 1;	//Bit2
        //! 如果UsageFault已激活，那么为1
        __IO uint32_t USGFAULTACT					: 1;	//Bit3
        __IO uint32_t Resv2							: 3;	//Bit4 ~ Bit6
        //! 如果SVCall已激活，那么为1
        __IO uint32_t SVCALLACT 						: 1;	//Bit7
        //! 如果监控器已激活，那么为1
        __IO uint32_t MONITORACT						: 1;	//Bit8
        __IO uint32_t Resv3							: 1;	//Bit9
        //! 如果PendSV已激活，那么为1
        __IO uint32_t PENDSVACT 						: 1;	//Bit10
        //! 如果SysTick已激活，那么为1
        __IO uint32_t SYSTICKACT						: 1;	//Bit11
        __IO uint32_t Resv4							: 1;	//Bit12
        //! 如果在开始调用 MemManage 时被一个更高优先级的中断取代而导致
        __IO uint32_t MEMFAULTPENDED					: 1;	//Bit13
        //! 如果在开始调用 BusFault 时被一个更高优先级的中断取代而导致
        __IO uint32_t BUSFAULTPENDED					: 1;	//Bit14
        //! 如果在开始调用 SVCall 时被一个更高优先级的中断取代而导致 SVCall
        __IO uint32_t SVCALLPENDED					: 1;	//Bit15
        //! 设为0时禁能，设为1时使能
        __IO uint32_t MEMFAULTENA					: 1;	//Bit16
        //! 设为0时禁能，设为1时使能
        __IO uint32_t BUSFAULTENA					: 1;	//Bit17
        //! 设为0时禁能，设为1时使能
        __IO uint32_t USGFAULTENA					: 1;	//Bit18
        __IO uint32_t Resv5							: 13;	//Bit19 ~ Bit31
    };
    typedef struct SCB_SHCSR_Struct SCB_SHCSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 可配置故障状态寄存器 (SCB_CFSR)		0xE000ED28
    struct SCB_CFSR_Struct
    {
        //!
        //! 存储器管理故障状态寄存器
        //!
        //!	指令访问违犯标志
        __IO uint32_t IACCVIOL						: 1;	//Bit0
        //! 数据访问违犯标志
        __IO uint32_t DACCVIOL						: 1;	//Bit1
        __IO uint32_t Resv1							: 1;	//Bit2
        //! 异常返回时的出栈操作引起了 1 个或 1 个以上的访问违犯
        __IO uint32_t MUNSTKERR						: 1;	//Bit3
        //! 进入异常时的压栈操作引起了1个或 1个以上的访问违犯
        __IO uint32_t MSTKERR						: 1;	//Bit4
        __IO uint32_t Resv2							: 2;	//Bit5 ~ Bit6
        //! 存储器管理地址寄存器（MMAR）地址有效标志
        __IO uint32_t MMARVALID 						: 1;	//Bit7
        //!
        //! 总线故障状态寄存器
        //!
        //!	指令总线错误标志
        __IO uint32_t IBUSERR						: 1;	//Bit0
        //! 精确的数据总线错误返回
        __IO uint32_t PRECISERR						: 1;	//Bit1
        //! 不精确的数据总线错误
        __IO uint32_t IMPRECISERR					: 1;	//Bit2
        //! 异常返回时的出栈操作引起了 1 个或 1 个以上的总线故障
        __IO uint32_t UNSTKERR						: 1;	//Bit3
        //! 进入异常时的压栈操作引起了1个或1个以上的总线故障
        __IO uint32_t STKERR							: 1;	//Bit4
        __IO uint32_t Resv3							: 2;	//Bit5 ~ Bit6
        //! 如果总线故障地址寄存器（BFAR）包含有效的地址，那么该位置位
        __IO uint32_t BFARVALID 						: 1;	//Bit7
        //!
        //! 使用故障状态寄存器
        //!
        //!	在处理器试图执行一个未定义的指令时置位UNDEFINSTR标志
        __IO uint32_t UNDEFINSTR						: 1;	//Bit0
        //! EPSR 和指令的非法组合，由未定义指令引发的除外
        __IO uint32_t INVSTATE						: 1;	//Bit1
        //! 试图将 EXC_RETURN 非法载入 PC
        __IO uint32_t INVPC							: 1;	//Bit2
        //! 试图使用协处理器指令。处理器不支持协处理器指令
        __IO uint32_t NOCP							: 1;	//Bit3
        __IO uint32_t Resv4							: 4;	//Bit4 ~ Bit7
        //! 当UNALIGN_TRP使能且存在试图进行不对齐的存储器访问时，该故障产生
        __IO uint32_t UNALIGNED 						: 1;	//Bit8
        //! 当DIV_0_TRP使能且SDIV或 UDIV指令的除数为0时，该故障产生
        __IO uint32_t DIVBYZERO 						: 1;	//Bit9
        __IO uint32_t Resv5							: 6;	//Bit10 ~ Bit15
    };
    typedef struct SCB_CFSR_Struct SCB_CFSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 硬故障状态寄存器(SCB_HFSR)			@0xE000ED2C
    struct SCB_HFSR_Struct
    {
        //!
        __IO uint32_t Resv1							: 1;	//Bit0
        //! 因为在异常处理（总线故障）过程中读向量表而导致发生故障，此时该位置位
        __IO uint32_t VECTTBL						: 1;	//Bit1
        __IO uint32_t Resv2							: 28;	//Bit2 ~ Bit29
        //! 硬故障在接收到可配置故障时激活，由于优先级或因为可配置故障被禁能而不
        __IO uint32_t FORCED 						: 1;	//Bit30
        //! 如果出现与调试有关的故障，那么该位置位。
        __IO uint32_t DEBUGEVT						: 1;	//Bit31
    };
    typedef struct SCB_HFSR_Struct SCB_HFSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 调试故障状态寄存器 (SCB_DFSR)		@0xE000ED30
    struct SCB_DFSR_Struct
    {
        //!	中止请求标志
        __IO uint32_t HALTED							: 1;	//Bit0
        //! BKPT标志由flash修补代码中的BKPT 指令设置，也可以由正常代码设置
        __IO uint32_t BKPT							: 1;	//Bit1
        //! 数据观察点与跟踪（DWT）标志
        __IO uint32_t DWTIRAP 						: 1;	//Bit2
        //! 向量捕获标志
        __IO uint32_t VCATCH							: 1;	//Bit3
        //! 外部调试请求标志
        __IO uint32_t EXTERNAL						: 1;	//Bit4
        __IO uint32_t Resv1							: 27;	//Bit5 ~ Bit31
    };
    typedef struct SCB_DFSR_Struct SCB_DFSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 存储器管理故障地址寄存器(SCB_MMFAR)	@0xE000ED34
    struct SCB_MMFAR_Struct
    {
        //!	Mem Manage 故障的地址域
        __IO uint32_t ADDRESS						: 32;	//Bit0 ~ Bit31
    };
    typedef struct SCB_MMFAR_Struct SCB_MMFAR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 总线故障地址寄存器 (SCB_BFAR)	@0xE000ED38
    struct SCB_BFAR_Struct
    {
        //!	总线故障的地址域
        __IO uint32_t ADDRESS						: 32;	//Bit0 ~ Bit31
    };
    typedef struct SCB_BFAR_Struct SCB_BFAR_Typedef;

    typedef struct
    {
        __I uint32_t CPUID;	/*!< Offset: 0x00  CPU ID Base Register                                  */
        __IO uint32_t  ICSR;		/*!< Offset: 0x04  Interrupt Control State Register                      */
        __IO uint32_t  VTOR;		/*!< Offset: 0x08  Vector Table Offset Register                          */
        __IO uint32_t  AIRCR;	/*!< Offset: 0x0C  Application Interrupt / Reset Control Register        */
        __IO uint32_t  SCR;		/*!< Offset: 0x10  System Control Register                               */
        __IO uint32_t  CCR;		/*!< Offset: 0x14  Configuration Control Register                        */
        __IO uint8_t   SHP[12];	/*!< Offset: 0x18  System Handlers Priority Registers (4-7, 8-11, 12-15) */
        __IO uint32_t  SHCSR;	/*!< Offset: 0x24  System Handler Control and State Register             */
        __IO uint32_t  CFSR;		/*!< Offset: 0x28  Configurable Fault Status Register                    */
        __IO uint32_t  HFSR;		/*!< Offset: 0x2C  Hard Fault Status Register                            */
        __IO uint32_t  DFSR;		/*!< Offset: 0x30  Debug Fault Status Register                           */
        __IO uint32_t  MMFAR;	/*!< Offset: 0x34  Mem Manage Address Register                           */
        __IO uint32_t  BFAR;		/*!< Offset: 0x38  Bus Fault Address Register                            */
        __IO uint32_t  AFSR;		/*!< Offset: 0x3C  Auxiliary Fault Status Register                       */
        __I uint32_t PFR[2];	/*!< Offset: 0x40  Processor Feature Register                            */
        __I uint32_t DFR;		/*!< Offset: 0x48  Debug Feature Register                                */
        __I uint32_t ADR;		/*!< Offset: 0x4C  Auxiliary Feature Register                            */
        __I uint32_t MMFR[4];	/*!< Offset: 0x50  Memory Model Feature Register                         */
        __I uint32_t ISAR[5];	/*!< Offset: 0x60  ISA Feature Register                                  */

        PriorityGroupingStyle GetPriorityGrouping();
        void SetPriorityGrouping(PriorityGroupingStyle s);

        /**
          * @brief  Sets the vector table location and Offset.
          * @param  NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
          *   This parameter can be one of the following values:
          *     @arg NVIC_VectTab_RAM
          *     @arg NVIC_VectTab_FLASH
          * @param  Offset: Vector Table base offset field. This value must be a multiple
          *         of 0x200.
          * @retval None
          */
        finline void SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
        {
            /* Check the parameters */
            //(IS_NVIC_VECTTAB(NVIC_VectTab))	NVIC_VectTab_RAM,NVIC_VectTab_FLASH
            //(IS_NVIC_OFFSET(Offset))

            VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
        }
        void SystemPriorityConfig(IRQn_Type IRQn, uint8_t priority = 0xF)
        {
            if(IRQn < 0)
            {
                /* set Priority for Cortex-M3 System Interrupts */
                SHP[((uint32_t)(IRQn) & 0xF) - 4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);
            }
        }
        void SystemReset(void);
    } SCB_Typedef;
}
//!
//! @{
using mcucore::SCB_Typedef;
#define stSCB	((SCB_Typedef *)           SCB_BASE)
#define MCUSCB	(*stSCB)
//! @}
//!
//! @{
/*
#define ITM_BASE            (0xE0000000)			//!< ITM Base Address
#define SCS_BASE            (0xE000E000)			//!< System Control Space Base Address
#define SysTick_BASE        (SCS_BASE +  0x0010)	//!< SysTick Base Address
#define NVIC_BASE           (SCS_BASE +  0x0100)	//!< NVIC Base Address
#define SCB_BASE            (SCS_BASE +  0x0D00)	//!< System Control Block Base Address
#define CoreDebug_BASE      (0xE000EDF0)			//!< Core Debug Base Address
*/
//! @}

#endif /* __STM32GLIBC_SCB_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
