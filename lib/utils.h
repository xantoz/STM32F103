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
#include "vfunc.h"

/**
 * @brief NOP-based/cycle-counting based delay routine. Works regardless of current SYSCLK
 *        frequency, by virtue of using g_clock.sysclkFreq for scaling (see clock.h).
 *
 * @note The lower the SYSCLK frequency is, the longer the minimum possible delay becomes. The
 *       overhead, while accounted for, is at least 12 cycles long, which translates to a minimum
 *       delay of 1.5 us at 8 MHz SYSCLK. So do not expect this to be accurate for small values of
 *       us relative to SYSCLK, or very accurate overall. However it should be usable enough when
 *       precise timings are not needed. It becomes more accurate for larger SYSCLK as more time is
 *       spent inside the tight loop, relative to on the overhead.
 *
 * @note If you do not wish to have the delay prolonged by interrupts, make sure to disable them
 *       before calling.
 *
 * @param us [in]   Delay in microseconds
 */
extern void delay_us(int32_t us);

#define BITBAND_SRAM_REF 0x20000000
#define BITBAND_SRAM_BASE 0x22000000
#define BITBAND_SRAM(a,b) ((BITBAND_SRAM_BASE + (a-BITBAND_SRAM_REF)*32 + (b*4))) // Convert SRAM address
#define BITBAND_PERI_REF 0x40000000
#define BITBAND_PERI_BASE 0x42000000
#define BITBAND_PERI(a,b) ((BITBAND_PERI_BASE + (a-BITBAND_PERI_REF)*32 + (b*4))) // Convert PERI address

#define DIV_ROUND_CLOSEST(n, d) \
    ((((n) < 0) ^ ((d) < 0)) ? (((n) - (d)/2)/(d)) : (((n) + (d)/2)/(d)))

#define __SET4(peripheral, reg, field, value) ((peripheral).reg = ((peripheral).reg & ~(peripheral##_##reg##_##field)) | peripheral##_##reg##_##field##_##value)
#define __SET3(peripheral, reg, field) ((peripheral).reg |= (peripheral##_##reg##_##field))
#define SET(...) VFUNC(__SET, __VA_ARGS__)

#define RESET(peripheral, reg, field) ((peripheral).reg &= ~(peripheral##_##reg##_##field))

#endif
