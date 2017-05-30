#ifndef _UTILS_
#define _UTILS_

#define NOP()  { __asm volatile ("NOP\n"); }
#define BKPT() { __asm volatile ("BKPT\n"); }

static inline void __enable_irq()               { __asm volatile ("cpsie i"); }
static inline void __disable_irq()              { __asm volatile ("cpsid i"); }

static inline void __enable_fault_irq()         { __asm volatile ("cpsie f"); }
static inline void __disable_fault_irq()        { __asm volatile ("cpsid f"); }

static inline void __NOP()                      { __asm volatile ("nop"); }
static inline void __WFI()                      { __asm volatile ("wfi"); }
static inline void __WFE()                      { __asm volatile ("wfe"); }
static inline void __SEV()                      { __asm volatile ("sev"); }
static inline void __ISB()                      { __asm volatile ("isb"); }
static inline void __DSB()                      { __asm volatile ("dsb"); }
static inline void __DMB()                      { __asm volatile ("dmb"); }
static inline void __CLREX()                    { __asm volatile ("clrex"); }

#include "types.h"

/** puts the CPU to sleep until an interrupt occurs */
void waitForInterrupt();

/** temporarily disable interrupts (for atomic access - only do this for a short time) */
void disableInterrupts();

/** re-enable interrupts */
void enableInterrupts();

#define SET(peripheral, reg, field, value) ((peripheral)->(reg) = ((peripheral)->(reg) & ~(peripheral##_##reg##_##field)) | peripheral##_##reg##_##field##_##value)

#endif
