#ifndef _MEMORYMAP_
#define _MEMORYMAP_

#include "types.h"

/* -----------------------------------
   --- VECTOR TABLE  -----------------
   -----------------------------------

The vector table specifies the boot stack position, the boot address and interrupt function pointers. The initial vector table is located in FLASH at 0x00000000, providing the entry point in the boot process. It can be relocated to RAM. */

typedef struct {
    void* STACK_ENTRY;              //Initial Stack pointer
    void* BOOT_HANDLER;             //Reset handler
    void* NMI_HANDLER;              //NMI handler
    void* HARDFAULT_HANDLER;        //Hard fault handler
    void* MPUFAULT_HANDLER;         //Memory protection unit fault handler
    void* BUSFAULT_HANDLER;         //Bus fault handler
    void* USAGEFAULT_HANDLER;       //Usage fault handler
    void* RESERVED1;
    void* RESERVED2;
    void* RESERVED3;
    void* RESERVED4;
    void* SVCALL_HANDLER;           //SVCall handler
    void* DEBUGMON_HANDLER;         //Debug monitor handler
    void* RESERVED5;
    void* PENDSV_HANDLER;           //PendSV handler
    void* SYSTICK_HANDLER;          //The SysTick handler
    void* IRQ0;                     //IRQ0
    void* IRQ1;                     //IRQ1
    void* IRQ2;                     //IRQ2
    void* IRQ3;                     //IRQ3
    void* IRQ4;                     //IRQ4
    void* IRQ5;                     //IRQ5
    void* IRQ6;                     //IRQ6
    void* IRQ7;                     //IRQ7
    void* IRQ8;                     //IRQ8
    void* IRQ9;                     //IRQ9
    void* IRQ10;                    //IRQ10
    void* IRQ11;                    //IRQ11
    void* IRQ12;                    //IRQ12
    void* IRQ13;                    //IRQ13
    void* IRQ14;                    //IRQ14
    void* IRQ15;                    //IRQ15
    void* IRQ16;                    //IRQ16
    void* IRQ17;                    //IRQ17
    void* IRQ18;                    //IRQ18
    void* IRQ19;                    //IRQ19
    void* IRQ20;                    //IRQ20
    void* IRQ21;                    //IRQ21
    void* IRQ22;                    //IRQ22
    void* IRQ23;                    //IRQ23
    void* IRQ24;                    //IRQ24
    void* IRQ25;                    //IRQ25
    void* IRQ26;                    //IRQ26
    void* IRQ27;                    //IRQ27
    void* IRQ28;                    //IRQ28
    void* IRQ29;                    //IRQ29
    void* IRQ30;                    //IRQ30
    void* IRQ31;                    //IRQ31
    void* IRQ32;                    //IRQ32
    void* IRQ33;                    //IRQ33
    void* IRQ34;                    //IRQ34
    void* IRQ35;                    //IRQ35
    void* IRQ36;                    //IRQ36
    void* IRQ37;                    //IRQ37
    void* IRQ38;                    //IRQ38
    void* IRQ39;                    //IRQ39
    void* IRQ40;                    //IRQ40
    void* IRQ41;                    //IRQ41
    void* IRQ42;                    //IRQ42
    void* IRQ43;                    //IRQ43
    void* IRQ44;                    //IRQ44
    void* IRQ45;                    //IRQ45
    void* IRQ46;                    //IRQ46
    void* IRQ47;                    //IRQ47
    void* IRQ48;                    //IRQ48
    void* IRQ49;                    //IRQ49
    void* IRQ50;                    //IRQ50
    void* IRQ51;                    //IRQ51
    void* IRQ52;                    //IRQ52
    void* IRQ53;                    //IRQ53
    void* IRQ54;                    //IRQ54
    void* IRQ55;                    //IRQ55
    void* IRQ56;                    //IRQ56
    void* IRQ57;                    //IRQ57
    void* IRQ58;                    //IRQ58
    void* IRQ59;                    //IRQ59
    void* IRQ60;                    //IRQ60
    void* IRQ61;                    //IRQ61
    void* IRQ62;                    //IRQ62
    void* IRQ63;                    //IRQ63
    void* IRQ64;                    //IRQ64
    void* IRQ65;                    //IRQ65
    void* IRQ66;                    //IRQ66
    void* IRQ67;                    //IRQ67
} VECTOR_TABLE;

#define FLASH_BASE ((void*)0x08000000)      //!< FLASH base address in the alias region
#define SRAM_BASE ((void*)0x20000000)       //!< SRAM base address in the alias region
#define PERIPH_BASE ((void*)0x40000000)     //!< Peripheral base address in the alias region

#define SRAM_BB_BASE ((void*)0x22000000)    //!< SRAM base address in the bit-band region
#define PERIPH_BB_BASE ((void*)0x42000000)  //!< Peripheral base address in the bit-band region

#define FSMC_R_BASE ((void*)0xA0000000)     //!< FSMC registers base address

/*!< Memory mapping of Cortex-M3 Hardware */
#define SCS_BASE            ((void*)0xE000E000)             //!< System Control Space Base Address
#define ITM_BASE            ((void*)0xE0000000)             //!< ITM Base Address
#define CoreDebug_BASE      ((void*)0xE000EDF0)             //!< Core Debug Base Address
#define SysTick_BASE        (SCS_BASE +  0x0010)            //!< SysTick Base Address
#define NVIC_BASE           (SCS_BASE +  0x0100)            //!< NVIC Base Address
#define SCB_BASE            (SCS_BASE +  0x0D00)            //!< System Control Block Base Address

/*!< Peripheral memory map */
#define APB1PERIPH_BASE (PERIPH_BASE)
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x20000)

#define AFIO_BASE             (APB2PERIPH_BASE + 0x0000)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400)

#define FLASH_R_BASE (AHBPERIPH_BASE + 0x2000) //!< Flash registers base address
#define OB_BASE ((void*)0x1FFFF800)    //!< Flash Option Bytes base address

#endif
