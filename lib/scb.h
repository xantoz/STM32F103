/**
 * @file scb.h
 *
 * @brief SCB register definitions
 */

#ifndef _SCB_
#define _SCB_

/**
 * @brief Memory mapped structure for System Control Block
 */
struct SCB_Regs
{
    __I  uint32_t CPUID;    //!< Offset: 0x00  CPU ID Base Register
    __IO uint32_t ICSR;     //!< Offset: 0x04  Interrupt Control State Register
    __IO uint32_t VTOR;     //!< Offset: 0x08  Vector Table Offset Register
    __IO uint32_t AIRCR;    //!< Offset: 0x0C  Application Interrupt / Reset Control Register
    __IO uint32_t SCR;      //!< Offset: 0x10  System Control Register
    __IO uint32_t CCR;      //!< Offset: 0x14  Configuration Control Register
    __IO uint8_t  SHP[12];  //!< Offset: 0x18  System Handlers Priority Registers (4-7, 8-11, 12-15)
    __IO uint32_t SHCSR;    //!< Offset: 0x24  System Handler Control and State Register
    __IO uint32_t CFSR;     //!< Offset: 0x28  Configurable Fault Status Register
    __IO uint32_t HFSR;     //!< Offset: 0x2C  Hard Fault Status Register
    __IO uint32_t DFSR;     //!< Offset: 0x30  Debug Fault Status Register
    __IO uint32_t MMFAR;    //!< Offset: 0x34  Mem Manage Address Register
    __IO uint32_t BFAR;     //!< Offset: 0x38  Bus Fault Address Register
    __IO uint32_t AFSR;     //!< Offset: 0x3C  Auxiliary Fault Status Register
    __I  uint32_t PFR[2];   //!< Offset: 0x40  Processor Feature Register
    __I  uint32_t DFR;      //!< Offset: 0x48  Debug Feature Register
    __I  uint32_t ADR;      //!< Offset: 0x4C  Auxiliary Feature Register
    __I  uint32_t MMFR[4];  //!< Offset: 0x50  Memory Model Feature Register
    __I  uint32_t ISAR[5];  //!< Offset: 0x60  ISA Feature Register
};

extern struct SCB_Regs SCB;

/**
 * @brief SCB CPUID Register Definitions
 */
enum SCB_CPUID_Flags
{
    SCB_CPUID_IMPLEMENTER_Pos = 24,                                     /*!< SCB CPUID: IMPLEMENTER Position */
    SCB_CPUID_IMPLEMENTER_Msk = (0xFFul << SCB_CPUID_IMPLEMENTER_Pos),  /*!< SCB CPUID: IMPLEMENTER Mask */

    SCB_CPUID_VARIANT_Pos     = 20,                                     /*!< SCB CPUID: VARIANT Position */
    SCB_CPUID_VARIANT_Msk     = (0xFul << SCB_CPUID_VARIANT_Pos),       /*!< SCB CPUID: VARIANT Mask */

    SCB_CPUID_PARTNO_Pos      = 4,                                      /*!< SCB CPUID: PARTNO Position */
    SCB_CPUID_PARTNO_Msk      = (0xFFFul << SCB_CPUID_PARTNO_Pos),      /*!< SCB CPUID: PARTNO Mask */

    SCB_CPUID_REVISION_Pos    = 0,                                      /*!< SCB CPUID: REVISION Position */
    SCB_CPUID_REVISION_Msk    = (0xFul << SCB_CPUID_REVISION_Pos),      /*!< SCB CPUID: REVISION Mask */
};

/**
 * @brief SCB Interrupt Control State Register Definitions
 */
enum SCB_ICSR_Flags
{
    SCB_ICSR_NMIPENDSET_Pos  = 31,                                      /*!< SCB ICSR: NMIPENDSET Position */
    SCB_ICSR_NMIPENDSET_Msk  = (1ul << SCB_ICSR_NMIPENDSET_Pos),        /*!< SCB ICSR: NMIPENDSET Mask */

    SCB_ICSR_PENDSVSET_Pos   = 28,                                      /*!< SCB ICSR: PENDSVSET Position */
    SCB_ICSR_PENDSVSET_Msk   = (1ul << SCB_ICSR_PENDSVSET_Pos),         /*!< SCB ICSR: PENDSVSET Mask */

    SCB_ICSR_PENDSVCLR_Pos   = 27,                                      /*!< SCB ICSR: PENDSVCLR Position */
    SCB_ICSR_PENDSVCLR_Msk   = (1ul << SCB_ICSR_PENDSVCLR_Pos),         /*!< SCB ICSR: PENDSVCLR Mask */

    SCB_ICSR_PENDSTSET_Pos   = 26,                                      /*!< SCB ICSR: PENDSTSET Position */
    SCB_ICSR_PENDSTSET_Msk   = (1ul << SCB_ICSR_PENDSTSET_Pos),         /*!< SCB ICSR: PENDSTSET Mask */

    SCB_ICSR_PENDSTCLR_Pos   = 25,                                      /*!< SCB ICSR: PENDSTCLR Position */
    SCB_ICSR_PENDSTCLR_Msk   = (1ul << SCB_ICSR_PENDSTCLR_Pos),         /*!< SCB ICSR: PENDSTCLR Mask */

    SCB_ICSR_ISRPREEMPT_Pos  = 23,                                      /*!< SCB ICSR: ISRPREEMPT Position */
    SCB_ICSR_ISRPREEMPT_Msk  = (1ul << SCB_ICSR_ISRPREEMPT_Pos),        /*!< SCB ICSR: ISRPREEMPT Mask */

    SCB_ICSR_ISRPENDING_Pos  = 22,                                      /*!< SCB ICSR: ISRPENDING Position */
    SCB_ICSR_ISRPENDING_Msk  = (1ul << SCB_ICSR_ISRPENDING_Pos),        /*!< SCB ICSR: ISRPENDING Mask */

    SCB_ICSR_VECTPENDING_Pos = 12,                                      /*!< SCB ICSR: VECTPENDING Position */
    SCB_ICSR_VECTPENDING_Msk = (0x1FFul << SCB_ICSR_VECTPENDING_Pos),   /*!< SCB ICSR: VECTPENDING Mask */

    SCB_ICSR_RETTOBASE_Pos   = 11,                                      /*!< SCB ICSR: RETTOBASE Position */
    SCB_ICSR_RETTOBASE_Msk   = (1ul << SCB_ICSR_RETTOBASE_Pos),         /*!< SCB ICSR: RETTOBASE Mask */

    SCB_ICSR_VECTACTIVE_Pos  = 0,                                       /*!< SCB ICSR: VECTACTIVE Position */
    SCB_ICSR_VECTACTIVE_Msk  = (0x1FFul << SCB_ICSR_VECTACTIVE_Pos),    /*!< SCB ICSR: VECTACTIVE Mask */
};


/**
 * @brief SCB Interrupt Control State Register Definitions
 */
enum SCB_VTOR_Flags
{
    SCB_VTOR_TBLBASE_Pos = 29,                                  /*!< SCB VTOR: TBLBASE Position */
    SCB_VTOR_TBLBASE_Msk = (0x1FFul << SCB_VTOR_TBLBASE_Pos),   /*!< SCB VTOR: TBLBASE Mask */

    SCB_VTOR_TBLOFF_Pos  = 7,                                   /*!< SCB VTOR: TBLOFF Position */
    SCB_VTOR_TBLOFF_Msk  = (0x3FFFFFul << SCB_VTOR_TBLOFF_Pos), /*!< SCB VTOR: TBLOFF Mask */
};

/**
 * @brief SCB Application Interrupt and Reset Control Register Definitions
 */
enum SCB_AIRCR_Flags
{
    SCB_AIRCR_VECTKEY_Pos       = 16,                                       /*!< SCB AIRCR: VECTKEY Position */
    SCB_AIRCR_VECTKEY_Msk       = (0xFFFFul << SCB_AIRCR_VECTKEY_Pos),      /*!< SCB AIRCR: VECTKEY Mask */

    SCB_AIRCR_VECTKEYSTAT_Pos   = 16,                                       /*!< SCB AIRCR: VECTKEYSTAT Position */
    SCB_AIRCR_VECTKEYSTAT_Msk   = (0xFFFFul << SCB_AIRCR_VECTKEYSTAT_Pos),  /*!< SCB AIRCR: VECTKEYSTAT Mask */

    SCB_AIRCR_ENDIANESS_Pos     = 15,                                       /*!< SCB AIRCR: ENDIANESS Position */
    SCB_AIRCR_ENDIANESS_Msk     = (1ul << SCB_AIRCR_ENDIANESS_Pos),         /*!< SCB AIRCR: ENDIANESS Mask */

    SCB_AIRCR_PRIGROUP_Pos      = 8,                                        /*!< SCB AIRCR: PRIGROUP Position */
    SCB_AIRCR_PRIGROUP_Msk      = (7ul << SCB_AIRCR_PRIGROUP_Pos),          /*!< SCB AIRCR: PRIGROUP Mask */

    SCB_AIRCR_SYSRESETREQ_Pos   = 2,                                        /*!< SCB AIRCR: SYSRESETREQ Position */
    SCB_AIRCR_SYSRESETREQ_Msk   = (1ul << SCB_AIRCR_SYSRESETREQ_Pos),       /*!< SCB AIRCR: SYSRESETREQ Mask */

    SCB_AIRCR_VECTCLRACTIVE_Pos = 1,                                        /*!< SCB AIRCR: VECTCLRACTIVE Position */
    SCB_AIRCR_VECTCLRACTIVE_Msk = (1ul << SCB_AIRCR_VECTCLRACTIVE_Pos),     /*!< SCB AIRCR: VECTCLRACTIVE Mask */

    SCB_AIRCR_VECTRESET_Pos     = 0,                                        /*!< SCB AIRCR: VECTRESET Position */
    SCB_AIRCR_VECTRESET_Msk     = (1ul << SCB_AIRCR_VECTRESET_Pos),         /*!< SCB AIRCR: VECTRESET Mask */
};

/**
 * @brief SCB System Control Register Definitions
 */
enum SCB_SCR_Flags
{
    SCB_SCR_SEVONPEND_Pos   = 4,                                 /*!< SCB SCR: SEVONPEND Position */
    SCB_SCR_SEVONPEND_Msk   = (1ul << SCB_SCR_SEVONPEND_Pos),    /*!< SCB SCR: SEVONPEND Mask */

    SCB_SCR_SLEEPDEEP_Pos   = 2,                                 /*!< SCB SCR: SLEEPDEEP Position */
    SCB_SCR_SLEEPDEEP_Msk   = (1ul << SCB_SCR_SLEEPDEEP_Pos),    /*!< SCB SCR: SLEEPDEEP Mask */

    SCB_SCR_SLEEPONEXIT_Pos = 1,                                 /*!< SCB SCR: SLEEPONEXIT Position */
    SCB_SCR_SLEEPONEXIT_Msk = (1ul << SCB_SCR_SLEEPONEXIT_Pos),  /*!< SCB SCR: SLEEPONEXIT Mask */
};

/**
 * @brief SCB Configuration Control Register Definitions
 */
enum SCB_CCR_Flags
{
    SCB_CCR_STKALIGN_Pos       = 9,                                    /*!< SCB CCR: STKALIGN Position */
    SCB_CCR_STKALIGN_Msk       = (1ul << SCB_CCR_STKALIGN_Pos),        /*!< SCB CCR: STKALIGN Mask */

    SCB_CCR_BFHFNMIGN_Pos      = 8,                                    /*!< SCB CCR: BFHFNMIGN Position */
    SCB_CCR_BFHFNMIGN_Msk      = (1ul << SCB_CCR_BFHFNMIGN_Pos),       /*!< SCB CCR: BFHFNMIGN Mask */

    SCB_CCR_DIV_0_TRP_Pos      = 4,                                    /*!< SCB CCR: DIV_0_TRP Position */
    SCB_CCR_DIV_0_TRP_Msk      = (1ul << SCB_CCR_DIV_0_TRP_Pos),       /*!< SCB CCR: DIV_0_TRP Mask */

    SCB_CCR_UNALIGN_TRP_Pos    = 3,                                    /*!< SCB CCR: UNALIGN_TRP Position */
    SCB_CCR_UNALIGN_TRP_Msk    = (1ul << SCB_CCR_UNALIGN_TRP_Pos),     /*!< SCB CCR: UNALIGN_TRP Mask */

    SCB_CCR_USERSETMPEND_Pos   = 1,                                    /*!< SCB CCR: USERSETMPEND Position */
    SCB_CCR_USERSETMPEND_Msk   = (1ul << SCB_CCR_USERSETMPEND_Pos),    /*!< SCB CCR: USERSETMPEND Mask */

    SCB_CCR_NONBASETHRDENA_Pos = 0,                                    /*!< SCB CCR: NONBASETHRDENA Position */
    SCB_CCR_NONBASETHRDENA_Msk = (1ul << SCB_CCR_NONBASETHRDENA_Pos),  /*!< SCB CCR: NONBASETHRDENA Mask */
};

/**
 * @brief SCB System Handler Control and State Register Definitions
 */
enum SCB_SHCSR_Flags
{
    SCB_SHCSR_USGFAULTENA_Pos    = 18,                                     /*!< SCB SHCSR: USGFAULTENA Position */
    SCB_SHCSR_USGFAULTENA_Msk    = (1ul << SCB_SHCSR_USGFAULTENA_Pos),     /*!< SCB SHCSR: USGFAULTENA Mask */

    SCB_SHCSR_BUSFAULTENA_Pos    = 17,                                     /*!< SCB SHCSR: BUSFAULTENA Position */
    SCB_SHCSR_BUSFAULTENA_Msk    = (1ul << SCB_SHCSR_BUSFAULTENA_Pos),     /*!< SCB SHCSR: BUSFAULTENA Mask */

    SCB_SHCSR_MEMFAULTENA_Pos    = 16,                                     /*!< SCB SHCSR: MEMFAULTENA Position */
    SCB_SHCSR_MEMFAULTENA_Msk    = (1ul << SCB_SHCSR_MEMFAULTENA_Pos),     /*!< SCB SHCSR: MEMFAULTENA Mask */

    SCB_SHCSR_SVCALLPENDED_Pos   = 15,                                     /*!< SCB SHCSR: SVCALLPENDED Position */
    SCB_SHCSR_SVCALLPENDED_Msk   = (1ul << SCB_SHCSR_SVCALLPENDED_Pos),    /*!< SCB SHCSR: SVCALLPENDED Mask */

    SCB_SHCSR_BUSFAULTPENDED_Pos = 14,                                     /*!< SCB SHCSR: BUSFAULTPENDED Position */
    SCB_SHCSR_BUSFAULTPENDED_Msk = (1ul << SCB_SHCSR_BUSFAULTPENDED_Pos),  /*!< SCB SHCSR: BUSFAULTPENDED Mask */

    SCB_SHCSR_MEMFAULTPENDED_Pos = 13,                                     /*!< SCB SHCSR: MEMFAULTPENDED Position */
    SCB_SHCSR_MEMFAULTPENDED_Msk = (1ul << SCB_SHCSR_MEMFAULTPENDED_Pos),  /*!< SCB SHCSR: MEMFAULTPENDED Mask */

    SCB_SHCSR_USGFAULTPENDED_Pos = 12,                                     /*!< SCB SHCSR: USGFAULTPENDED Position */
    SCB_SHCSR_USGFAULTPENDED_Msk = (1ul << SCB_SHCSR_USGFAULTPENDED_Pos),  /*!< SCB SHCSR: USGFAULTPENDED Mask */

    SCB_SHCSR_SYSTICKACT_Pos     = 11,                                     /*!< SCB SHCSR: SYSTICKACT Position */
    SCB_SHCSR_SYSTICKACT_Msk     = (1ul << SCB_SHCSR_SYSTICKACT_Pos),      /*!< SCB SHCSR: SYSTICKACT Mask */

    SCB_SHCSR_PENDSVACT_Pos      = 10,                                     /*!< SCB SHCSR: PENDSVACT Position */
    SCB_SHCSR_PENDSVACT_Msk      = (1ul << SCB_SHCSR_PENDSVACT_Pos),       /*!< SCB SHCSR: PENDSVACT Mask */

    SCB_SHCSR_MONITORACT_Pos     = 8,                                      /*!< SCB SHCSR: MONITORACT Position */
    SCB_SHCSR_MONITORACT_Msk     = (1ul << SCB_SHCSR_MONITORACT_Pos),      /*!< SCB SHCSR: MONITORACT Mask */

    SCB_SHCSR_SVCALLACT_Pos      = 7,                                      /*!< SCB SHCSR: SVCALLACT Position */
    SCB_SHCSR_SVCALLACT_Msk      = (1ul << SCB_SHCSR_SVCALLACT_Pos),       /*!< SCB SHCSR: SVCALLACT Mask */

    SCB_SHCSR_USGFAULTACT_Pos    = 3,                                      /*!< SCB SHCSR: USGFAULTACT Position */
    SCB_SHCSR_USGFAULTACT_Msk    = (1ul << SCB_SHCSR_USGFAULTACT_Pos),     /*!< SCB SHCSR: USGFAULTACT Mask */

    SCB_SHCSR_BUSFAULTACT_Pos    = 1,                                      /*!< SCB SHCSR: BUSFAULTACT Position */
    SCB_SHCSR_BUSFAULTACT_Msk    = (1ul << SCB_SHCSR_BUSFAULTACT_Pos),     /*!< SCB SHCSR: BUSFAULTACT Mask */

    SCB_SHCSR_MEMFAULTACT_Pos    = 0,                                      /*!< SCB SHCSR: MEMFAULTACT Position */
    SCB_SHCSR_MEMFAULTACT_Msk    = (1ul << SCB_SHCSR_MEMFAULTACT_Pos),     /*!< SCB SHCSR: MEMFAULTACT Mask */
};

/**
 * @brief SCB Configurable Fault Status Registers Definitions
 */
enum SCB_CFSR_Regs
{
    SCB_CFSR_USGFAULTSR_Pos = 16,                                     /*!< SCB CFSR: Usage Fault Status Register Position */
    SCB_CFSR_USGFAULTSR_Msk = (0xFFFFul << SCB_CFSR_USGFAULTSR_Pos),  /*!< SCB CFSR: Usage Fault Status Register Mask */

    SCB_CFSR_BUSFAULTSR_Pos = 8,                                      /*!< SCB CFSR: Bus Fault Status Register Position */
    SCB_CFSR_BUSFAULTSR_Msk = (0xFFul << SCB_CFSR_BUSFAULTSR_Pos),    /*!< SCB CFSR: Bus Fault Status Register Mask */

    SCB_CFSR_MEMFAULTSR_Pos = 0,                                      /*!< SCB CFSR: Memory Manage Fault Status Register Position */
    SCB_CFSR_MEMFAULTSR_Msk = (0xFFul << SCB_CFSR_MEMFAULTSR_Pos),    /*!< SCB CFSR: Memory Manage Fault Status Register Mask */
};

/**
 * @brief SCB Hard Fault Status Registers Definitions
 */
enum SCB_HFSR_Flags
{
    SCB_HFSR_DEBUGEVT_Pos = 31,                              /*!< SCB HFSR: DEBUGEVT Position */
    SCB_HFSR_DEBUGEVT_Msk = (1ul << SCB_HFSR_DEBUGEVT_Pos),  /*!< SCB HFSR: DEBUGEVT Mask */

    SCB_HFSR_FORCED_Pos   = 30,                              /*!< SCB HFSR: FORCED Position */
    SCB_HFSR_FORCED_Msk   = (1ul << SCB_HFSR_FORCED_Pos),    /*!< SCB HFSR: FORCED Mask */

    SCB_HFSR_VECTTBL_Pos  = 1,                               /*!< SCB HFSR: VECTTBL Position */
    SCB_HFSR_VECTTBL_Msk  = (1ul << SCB_HFSR_VECTTBL_Pos),   /*!< SCB HFSR: VECTTBL Mask */
};

/**
 * @brief SCB Debug Fault Status Register Definitions
 */
enum SCB_DFSR_Flags
{
    SCB_DFSR_EXTERNAL_Pos = 4,                              /*!< SCB DFSR: EXTERNAL Position */
    SCB_DFSR_EXTERNAL_Msk = (1ul << SCB_DFSR_EXTERNAL_Pos), /*!< SCB DFSR: EXTERNAL Mask */

    SCB_DFSR_VCATCH_Pos   = 3,                              /*!< SCB DFSR: VCATCH Position */
    SCB_DFSR_VCATCH_Msk   = (1ul << SCB_DFSR_VCATCH_Pos),   /*!< SCB DFSR: VCATCH Mask */

    SCB_DFSR_DWTTRAP_Pos  = 2,                              /*!< SCB DFSR: DWTTRAP Position */
    SCB_DFSR_DWTTRAP_Msk  = (1ul << SCB_DFSR_DWTTRAP_Pos),  /*!< SCB DFSR: DWTTRAP Mask */

    SCB_DFSR_BKPT_Pos     = 1,                              /*!< SCB DFSR: BKPT Position */
    SCB_DFSR_BKPT_Msk     = (1ul << SCB_DFSR_BKPT_Pos),     /*!< SCB DFSR: BKPT Mask */

    SCB_DFSR_HALTED_Pos   = 0,                              /*!< SCB DFSR: HALTED Position */
    SCB_DFSR_HALTED_Msk   = (1ul << SCB_DFSR_HALTED_Pos),   /*!< SCB DFSR: HALTED Mask */
};

#endif /* _SCB_ */
