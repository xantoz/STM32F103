/**
 * @file nvic.h
 *
 * @brief NVIC register definitions
 */

#ifndef _NVIC_
#define _NVIC_

#include "types.h"
#include "IRQn.h"
#include "debug.h"

/**
 * @brief Memory mapped structure for Nested Vectored Interrupt Controller (NVIC)
 */
struct NVIC_Regs
{
    __IO uint32_t ISER[8];   //!< Offset: 0x000  Interrupt Set Enable Register
         uint32_t RESERVED0[24];
    __IO uint32_t ICER[8];   //!< Offset: 0x080  Interrupt Clear Enable Register
         uint32_t RSERVED1[24];
    __IO uint32_t ISPR[8];   //!< Offset: 0x100  Interrupt Set Pending Register
         uint32_t RESERVED2[24];
    __IO uint32_t ICPR[8];   //!< Offset: 0x180  Interrupt Clear Pending Register
         uint32_t RESERVED3[24];
    __IO uint32_t IABR[8];   //!< Offset: 0x200  Interrupt Active bit Register
         uint32_t RESERVED4[56];
    __IO uint8_t  IP[240];   //!< Offset: 0x300  Interrupt Priority Register (8Bit wide)
         uint32_t RESERVED5[644];
    __O  uint32_t STIR;      //!< Offset: 0xE00  Software Trigger Interrupt Register
};

extern volatile struct NVIC_Regs NVIC;

static INLINE void __NVIC_SetMask(volatile uint32_t * const base, const enum IRQn interrupt)
{
    assert(!IRQn_IS_CORE_PERIPHERAL(interrupt));

    const uint8_t reg = interrupt / 32;
    const uint32_t mask = 1 << (interrupt % 32);
    base[reg] = mask;
}

static INLINE bool __NVIC_GetMask(volatile uint32_t * const base, const enum IRQn interrupt)
{
    assert(!IRQn_IS_CORE_PERIPHERAL(interrupt));

    const uint8_t reg = interrupt / 32;
    const uint32_t mask = 1 << (interrupt % 32);
    return (base[reg] & mask) ? true : false;
}

static inline void NVIC_EnableInterrupt       (const enum IRQn interrupt) { __NVIC_SetMask(NVIC.ISER, interrupt);        }
static inline void NVIC_DisableInterrupt      (const enum IRQn interrupt) { __NVIC_SetMask(NVIC.ICER, interrupt);        }
static inline bool NVIC_IsInterruptEnabled    (const enum IRQn interrupt) { return __NVIC_GetMask(NVIC.ISER, interrupt); }
static inline void NVIC_SetInterruptPending   (const enum IRQn interrupt) { __NVIC_SetMask(NVIC.ISPR, interrupt);        }
static inline void NVIC_ClearInterruptPending (const enum IRQn interrupt) { __NVIC_SetMask(NVIC.ICPR, interrupt);        }
static inline bool NVIC_IsInterruptPending    (const enum IRQn interrupt) { return __NVIC_GetMask(NVIC.ISPR, interrupt); }
static inline bool NVIC_IsInterruptActive     (const enum IRQn interrupt) { return __NVIC_GetMask(NVIC.IABR, interrupt); }

static inline void NVIC_SetInterruptPriority(const enum IRQn interrupt, const uint8_t prio)
{
    assert(!IRQn_IS_CORE_PERIPHERAL(interrupt));
    NVIC.IP[interrupt] = prio;
}

static inline uint8_t NVIC_GetInterruptPriority(const enum IRQn interrupt)
{
    assert(!IRQn_IS_CORE_PERIPHERAL(interrupt));
    return NVIC.IP[interrupt];
}

static inline void NVIC_TriggerInterrupt(const enum IRQn interrupt)
{
    assert(!IRQn_IS_CORE_PERIPHERAL(interrupt));
    NVIC.STIR = interrupt;
}

#if 0
static inline uint8_t NVIC_GetInterruptGroupPriorityBits()
{
    return 7 - ((SCB.AIRCR & AIRCR_PRIGROUP_MASK) >> 8);
}

static inline void NVIC_SetInterruptGroupPriorityBits(const uint8_t groupLength)
{
    SCB.AIRCR = AIRCR_VECTKEY | ((7-groupLength) << 8);
}

static inline void NVIC_ResetSystem()
{
    SCB.AIRCR = AIRCR_VECTKEY | AIRCR_SYSRESETREQ;
}
#endif /* 0 */

#endif /* _NVIC_ */
