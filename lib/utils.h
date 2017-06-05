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

/**
 * @brief NOP-based/cycle-counting based delay routine. Works regardless of current SYSCLK
 *        frequency, by virtue of using g_clock.sysclkFreq for scaling (see clock.h).
 *
 * @note The lower the SYSCLK frequency is, the lower the minimum possible delay becomes. So do not
 *       expect it to be able to delay for as short as 1 us. The overhead is at least 16 cycles
 *       long, which translates to a minimum delay of 2 us at 8 MHz. So do not expect this to be
 *       accurate for small values of us, or very accurate overall, but it should be usable enough
 *       for things where the exactness of the delay is not very important.
 *
 * @param us [in]   Delay in microseconds
 */
extern void delay_us(int32_t us);

#define SET(peripheral, reg, field, value) ((peripheral).(reg) = ((peripheral).(reg) & ~(peripheral##_##reg##_##field)) | peripheral##_##reg##_##field##_##value)

#endif
